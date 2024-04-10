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

#ifndef CHRONIX_CONST_H
#define CHRONIX_CONST_H

/* 8259A终端控制器端口 */
#define INT_M_CTL           0x20    /* I/O port for interrupt controller         <Master> */
#define INT_M_CTLMASK       0x21    /* setting bits in this port disables ints   <Master> */
#define INT_S_CTL           0xA0    /* I/O port for second interrupt controller  <Slave>  */
#define INT_S_CTLMASK       0xA1    /* setting bits in this port disables ints   <Slave>  */
/* 中断控制器的神奇数字EOI，可以用于控制中断的打开和关闭，当然，这个宏可以被类似功能的引用 */
#define EOI             0x20    /* EOI，发送给8259A端口1，以重新启用中断 */
#define DISABLE         0       /* 用于在中断后保持当前中断关闭的代码 */
#define ENABLE          EOI	    /* 用于在中断后重新启用当前中断的代码 */
/* 固定系统调用向量。 */
#define INT_VECTOR_LEVEL0           0x66	    /* 用于系统任务提权到 0 的调用向量 */
#define INT_VECTOR_SYS_CALL         0x94        /* Chronix 386 系统调用向量 */
/* BIOS中断向量 和 保护模式下所需的中断向量 */
#define INT_VECTOR_BIOS_IRQ0        0x00
#define INT_VECTOR_BIOS_IRQ8        0x10
#define	INT_VECTOR_IRQ0				0x20    // 32
#define	INT_VECTOR_IRQ8				0x28    // 40
/* 硬件中断数量 */
#define NR_IRQ_VECTORS      16      /* 中断请求的数量 */

// 虚拟地址转化为物理地址
#define	vir2phys(vir) ((phys_bytes)(KERNEL_DATA_SEG_BASE) + (vir_bytes)(vir))

#endif