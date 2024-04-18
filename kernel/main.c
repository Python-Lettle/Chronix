/**
 * Copyright (C) 2024 Lettle All rights reserved.
 * See the copyright notice in the file LICENSE.
 * Created by Lettle on 2024/3/31.
 * QQ: 1071445082
 * Email: 1071445082@qq.com
 * gitee: https://gitee.com/lettle/
 * github: https://github.com/python-lettle/
 * bilibili: https://space.bilibili.com/420393625
 */

#include <stdlib.h>
#include "global.h"
#include "prototype.h"

void chronix_main()
{
    keyboard_init();
    interrupt_unlock();     // 放在别的地方就无了

    // Print memory size
    uint32_t memsize = boot_param->memsize / 1024 /1024;       // MB
    if (memsize != 0) {
        terminal.print(&terminal, "OS memory size(MB): ");
        terminal.print_int(&terminal, memsize, 10);
        terminal.print(&terminal, "\n");
    } else {
        terminal.print(&terminal, "Can not calculate memory size.\n");
    }

    terminal.show_head(&terminal);

    // 不能让内核陷入hlt 不然键盘中断就无了
    while (1){}
    
}

/**
 *	内核遇到了不可恢复的异常或错误，立即准备宕机
 */
void panic(const char* msg, int error_no)
{
    /* 有错误消息的话，请先打印 */
    if(msg != 0) {
        terminal.print(&terminal, "\n**** Chronix panic:");
        terminal.print(&terminal, msg);
        terminal.print(&terminal, " ****\n");
        if(error_no != 0) {
            terminal.print(&terminal, "**** Panic number:");
            terminal.print_int(&terminal, error_no, 16);
            terminal.print(&terminal, " ****\n");
        }
    }
    // 陷入死循环 不可操作
    // while(1) {}
}
