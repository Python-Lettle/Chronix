/**
 * Copyright (C) 2024 Lettle All rights reserved.
 * See the copyright notice in the file LICENSE.
 * Created by Lettle on 2024/4/8.
 * QQ: 1071445082
 * Email: 1071445082@qq.com
 * gitee: https://gitee.com/lettle/
 * github: https://github.com/python-lettle/
 * bilibili: https://space.bilibili.com/420393625
 */

#ifndef CHRONIX_KERNEL_GLOBAL_H
#define CHRONIX_KERNEL_GLOBAL_H

#include <kernel/type.h>
#include "components/Terminal.h"
#include "components/MemMan.h"
#include "protect.h"

extern MemoryMap kernel_map;            /* 内核内存映像 */
extern struct BOOT_PARAM *boot_param;   /* 启动参数 */
extern Terminal terminal;
extern MemMan memman;
extern Gate idt[IDT_SIZE];

irq_handler_t irq_handler_table[NR_IRQ_VECTORS]; /* interrupt_8259.c */

uint8_t gdt_ptr[6];                     /* kernel.asm */
uint8_t idt_ptr[6];                     /* IDT指针，同上 */

void test_int(void);
syscall_t level0_func;                  /* 提权成功的函数 */



#endif