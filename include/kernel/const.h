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
/* 其他的端口 */
#define PCR		0x65			/* 平面控制寄存器 */
#define PORT_B          0x61	/* 8255端口B的I/O端口(键盘，蜂鸣…) */
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
/* 主8259A上的 */
#define	CLOCK_IRQ		    0       /* 时钟中断请求号 */
#define	KEYBOARD_IRQ	    1       /* 键盘中断请求号 */
#define	CASCADE_IRQ		    2	    /* 第二个AT控制器的级联启用 */
#define	ETHER_IRQ		    3	    /* 默认以太网中断向量 */
#define	SECONDARY_IRQ	    3	    /* RS232 interrupt vector for port 2  */
#define	RS232_IRQ		    4	    /* RS232 interrupt vector for port 1 */
#define	XT_WINI_IRQ		    5	    /* xt风格硬盘 */
#define	FLOPPY_IRQ		    6	    /* 软盘 */
#define	PRINTER_IRQ		    7       /* 打印机 */
/* 从8259A上的 */
#define REAL_CLOCK_IRQ      8       /* 实时时钟 */
#define DIRECT_IRQ2_IRQ     9       /* 重定向IRQ2 */
#define RESERVED_10_IRQ     10      /* 保留待用 */
#define RESERVED_11_IRQ     11      /* 保留待用 */
#define MOUSE_IRQ           12      /* PS/2 鼠标 */
#define FPU_IRQ             13      /* FPU 异常 */
#define	AT_WINI_IRQ		    14	    /* at风格硬盘 */
#define RESERVED_15_IRQ     15      /* 保留待用 */

// 虚拟地址转化为物理地址
#define	vir2phys(vir) ((phys_bytes)(KERNEL_DATA_SEG_BASE) + (vir_bytes)(vir))

#endif