/**
 * Copyright (C) 2024 Lettle All rights reserved.
 * See the copyright notice in the file LICENSE.
 * Created by Lettle on 2024/4/10.
 * QQ: 1071445082
 * Email: 1071445082@qq.com
 * gitee: https://gitee.com/lettle/
 * github: https://github.com/python-lettle/
 * bilibili: https://space.bilibili.com/420393625
 */
#include "global.h"
#include "components/Terminal.h"
#include <stdlib.h>

/**
 * @brief 中断初始化
 */
int default_irq_handler(int irq);   // 写在这里才能看到
void interrupt_init(void)
{
    /* 初始化前先将中断响应关闭 */
    interrupt_lock();
    /* 1  向端口20H(主片)或a0H(从片)写入ICW1 */
    /* 00010001b -> 17 */
    out_byte(INT_M_CTL, 17);
    out_byte(INT_S_CTL, 17);
    /* 2 向端口21H(主片)或a1H(从片)写入ICW2 */
    /* 写入我们80386下的中断向量 */
    out_byte(INT_M_CTLMASK, INT_VECTOR_IRQ0);
    out_byte(INT_S_CTLMASK, INT_VECTOR_IRQ8);
    /* 3 向端口21H(主片)或a1H(从片)写入ICW3 */
    /* 主：00000100b -> 4，从：00000010b -> 2 */
    out_byte(INT_M_CTLMASK, 4);
    out_byte(INT_S_CTLMASK, 2);
    /* 4 向端口21H(主片)或a1H(从片)写入ICW4 */
    /* 0000001b -> 1 */
    out_byte(INT_M_CTLMASK, 1);
    out_byte(INT_S_CTLMASK, 1);

    /* 由于现在还没有配置中断例程，我们屏蔽所有中断，使其都不能发生 */
    out_byte(INT_M_CTLMASK, 0xff);
    out_byte(INT_S_CTLMASK, 0xff);

    /* 最后，我们初始化中断处理程序表，给每一个中断设置一个默认的中断处理例程 */
    int i = 0;
    for(; i < NR_IRQ_VECTORS; i++) {
        irq_handler_table[i] = default_irq_handler;
    }

    interrupt_unlock();
}

/*=========================================================================*
 *				put_irq_handler				   *
 *			   设置并注册中断处理例程
 *=========================================================================*/
void put_irq_handler(int irq, irq_handler_t handler)
{
    /* 一旦一个硬件的驱动编写完成，那么就可以调用本例程来
     * 为其设置真正的中断处理例程了，它将会替换掉初始化时
     * 默认的spurious_irq例程。
     */

    /* 注册过了？那么什么也不做 */
    if(irq_handler_table[irq] == handler) return;

    /* 开始设置
     * 先关闭对应的中断，再将中断处理例程替换旧的
     */
    disable_irq(irq);
    irq_handler_table[irq] = handler;
}


/*=========================================================================*
 *				default_irq_handler				   *
 *			    默认的中断处理例程
 *=========================================================================*/
int default_irq_handler(int irq)
{
    terminal.print(&terminal, "interrupt: ");
    char irq_str[10] = {0};
    itoa(irq, irq_str, 10);
    terminal.print(&terminal, irq_str);
    terminal.new_line(&terminal);
    return 0; /* 响应完毕不需要重新打开中断 */
}