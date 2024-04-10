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

#ifndef CHRONIX_PROTOTYPE_H
#define CHRONIX_PROTOTYPE_H

#include <kernel/type.h>

/**
 *	内核遇到了不可恢复的异常或错误，立即准备宕机
 */
void panic(const char* msg, int error_no);
void _io_hlt(void);                     /* kernel.asm 中的htl*/

//============================================================
// init.c
//============================================================
/**
 * @brief 内核初始化
 */
void kernel_init(void);
//============================================================
// interrupt_8259.c
//============================================================
/**
 * @brief 中断初始化
 */
void interrupt_init(void);
void put_irq_handler(int irq, irq_handler_t handler);

/**==================================================
 * kernel_lib32.asm
 * ================================================== */
uint8_t in_byte(port_t port);
void out_byte(port_t port, uint8_t value);
uint16_t in_word(port_t port);
void out_word(port_t port, uint16_t value);
void interrupt_lock(void);
void interrupt_unlock(void);
void enable_irq(int int_request);
void disable_irq(int int_request);

/**==================================================
 * kernel.asm
 * ================================================== */
// 所有中断处理入口，一共16个(两个8259A)
void hwint00 (void);
void hwint01 (void);
void hwint02 (void);
void hwint03 (void);
void hwint04 (void);
void hwint05 (void);
void hwint06 (void);
void hwint07 (void);
void hwint08 (void);
void hwint09 (void);
void hwint10 (void);
void hwint11 (void);
void hwint12 (void);
void hwint13 (void);
void hwint14 (void);
void hwint15 (void);

// 异常处理入口
void divide_error(void);
void single_step_exception(void);
void nmi(void);
void breakpoint_exception(void);
void overflow(void);
void bounds_check(void);
void inval_opcode(void);
void copr_not_available(void);
void double_fault(void);
void inval_tss(void);
void copr_not_available(void);
void segment_not_present(void);
void stack_exception(void);
void general_protection(void);
void page_fault(void);
void copr_seg_overrun(void);
void copr_error(void);

#endif