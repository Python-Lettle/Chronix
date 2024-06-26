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
#include "protect.h"
#include <kernel/type.h>
#include <kernel/const.h>
#include <string.h>
#include "global.h"
#include "prototype.h"
#include "components/Terminal.h"

/* 全局描述符表GDT */
SegDescriptor gdt[GDT_SIZE];
/* 中断描述符表IDT */
Gate idt[IDT_SIZE];
/* 任务状态段TSS(Task-State Segment) */
Tss tss;

/* 中断门信息 */
struct gate_desc_s {
    uint8_t vector;            /* 中断向量号 */
    int_handler_t handler;  /* 处理例程 */
    uint8_t privilege;         /* 门权限 */
};

/* 中断门信息表 */
struct gate_desc_s int_gate_table[] = {
        /* ************* 异常 *************** */
        { INT_VECTOR_DIVIDE, divide_error, KERNEL_PRIVILEGE },
        { INT_VECTOR_DEBUG, single_step_exception, KERNEL_PRIVILEGE },
        { INT_VECTOR_NMI, nmi, KERNEL_PRIVILEGE },
        { INT_VECTOR_BREAKPOINT, breakpoint_exception, KERNEL_PRIVILEGE },
        { INT_VECTOR_OVERFLOW, overflow, KERNEL_PRIVILEGE },
        { INT_VECTOR_BOUNDS, bounds_check, KERNEL_PRIVILEGE },
        { INT_VECTOR_INVAL_OP, inval_opcode, KERNEL_PRIVILEGE },
        { INT_VECTOR_COPROC_NOT, copr_not_available, KERNEL_PRIVILEGE },
        { INT_VECTOR_DOUBLE_FAULT, double_fault, KERNEL_PRIVILEGE },
        { INT_VECTOR_COPROC_SEG, copr_seg_overrun, KERNEL_PRIVILEGE },
        { INT_VECTOR_INVAL_TSS, inval_tss, KERNEL_PRIVILEGE },
        { INT_VECTOR_SEG_NOT, segment_not_present, KERNEL_PRIVILEGE },
        { INT_VECTOR_STACK_FAULT, stack_exception, KERNEL_PRIVILEGE },
        { INT_VECTOR_PROTECTION, general_protection, KERNEL_PRIVILEGE },
        { INT_VECTOR_PAGE_FAULT, page_fault, KERNEL_PRIVILEGE },
        { INT_VECTOR_COPROC_ERR, copr_error, KERNEL_PRIVILEGE },
        /* ************* 硬件中断 *************** */
        { INT_VECTOR_IRQ0 + 0, hwint00, KERNEL_PRIVILEGE },
        { INT_VECTOR_IRQ0 + 1, hwint01, KERNEL_PRIVILEGE },
        { INT_VECTOR_IRQ0 + 2, hwint02, KERNEL_PRIVILEGE },
        { INT_VECTOR_IRQ0 + 3, hwint03, KERNEL_PRIVILEGE },
        { INT_VECTOR_IRQ0 + 4, hwint04, KERNEL_PRIVILEGE },
        { INT_VECTOR_IRQ0 + 5, hwint05, KERNEL_PRIVILEGE },
        { INT_VECTOR_IRQ0 + 6, hwint06, KERNEL_PRIVILEGE },
        { INT_VECTOR_IRQ0 + 7, hwint07, KERNEL_PRIVILEGE },
        { INT_VECTOR_IRQ8 + 0, hwint08, KERNEL_PRIVILEGE },
        { INT_VECTOR_IRQ8 + 1, hwint09, KERNEL_PRIVILEGE },
        { INT_VECTOR_IRQ8 + 2, hwint10, KERNEL_PRIVILEGE },
        { INT_VECTOR_IRQ8 + 3, hwint11, KERNEL_PRIVILEGE },
        { INT_VECTOR_IRQ8 + 4, hwint12, KERNEL_PRIVILEGE },
        { INT_VECTOR_IRQ8 + 5, hwint13, KERNEL_PRIVILEGE },
        { INT_VECTOR_IRQ8 + 6, hwint14, KERNEL_PRIVILEGE },
        { INT_VECTOR_IRQ8 + 7, hwint15, KERNEL_PRIVILEGE },
        /* ************* 软件中断 *************** */
        { INT_VECTOR_LEVEL0, level0_sys_call, TASK_PRIVILEGE },         /* 提供给系统任务的系统调用：提权 */
 };


void protect_init(void)
{
    /* 将 LOADER 中的 GDT 拷贝到内核中新的 GDT 中 */
    memcpy(
        vir2phys(&gdt),                                   // src:LOADER中旧的GDT基地址
        *((uint32_t *)vir2phys(&gdt_ptr[2])),             // dest:新的GDT基地址
        *((uint16_t*)vir2phys(&gdt_ptr[0])) + 1           // size:旧GDT的段界限 + 1
    );
    /* 算出新 GDT 的基地址和界限，设置新的 gdt_ptr */
    uint16_t* p_gdt_limit = (uint16_t*)vir2phys(&gdt_ptr[0]);
    uint32_t* p_gdt_base = (uint32_t*)vir2phys(&gdt_ptr[2]);
    *p_gdt_limit = GDT_SIZE * DESCRIPTOR_SIZE - 1;
    *p_gdt_base = vir2phys(&gdt);

    /* 算出IDT的基地址和界限，设置新的 idt_ptr */
    uint16_t* p_idt_limit = (uint16_t*)vir2phys(&idt_ptr[0]);
    uint32_t* p_idt_base = (uint32_t*)vir2phys(&idt_ptr[2]);
    *p_idt_limit = IDT_SIZE * sizeof(Gate) - 1;
    *p_idt_base = vir2phys(&idt);

    /* 初始化所有中断门描述符到 IDT中 */
    struct gate_desc_s* p_gate = &int_gate_table[0];
    for(; p_gate < &int_gate_table[sizeof(int_gate_table) / sizeof(struct gate_desc_s)]; p_gate++){
        init_gate(p_gate->vector, DA_386IGate, p_gate->handler, p_gate->privilege);
    }

    /* 初始化任务状态段TSS，并为处理器寄存器和其他任务切换时应保存的信息提供空间。
     * 我们只使用了某些域的信息，这些域定义了当发生中断时在何处建立新堆栈。
     * 下面init_seg_desc的调用保证它可以用GDT进行定位。
     */
    memset(&tss, 0, sizeof(tss));
    tss.ss0 = SELECTOR_KERNEL_DS;
    init_segment_desc(&gdt[TSS_INDEX], vir2phys(&tss), sizeof(tss) - 1, DA_386TSS);
    tss.iobase = sizeof(tss);           /* 空 I/O 位图 */
}

/*=========================================================================*
 *				init_segment_desc				   *
 *				初始化段描述符
 *=========================================================================*/
void init_segment_desc(SegDescriptor *p_desc,phys_bytes base,phys_bytes limit,uint16_t attribute)
{
    /* 初始化一个数据段描述符 */
    p_desc->limit_low	= limit & 0x0FFFF;          /* 段界限 1		(2 字节) */
    p_desc->base_low	= base & 0x0FFFF;           /* 段基址 1		(2 字节) */
    p_desc->base_middle	= (base >> 16) & 0xFF;      /* 段基址 2		(1 字节) */
    p_desc->access		= attribute & 0xFF;         /* 属性 1 */
    p_desc->granularity = ((limit >> 16) & 0x0F) |  /* 段界限 2 + 属性 2 */
                          ((attribute >> 8) & 0xF0);
    p_desc->base_high	= (base >> 24) & 0x0FF;     /* 段基址 3		(1 字节) */
}

/*=========================================================================*
 *				init_gate				   *
 *				初始化一个 386门描述符
 *=========================================================================*/
void init_gate(uint8_t vector,uint8_t desc_type,int_handler_t handler,uint8_t privilege)
{
    // 得到中断向量对应的门结构
    Gate* p_gate = &idt[vector];
    // 取得处理函数的基地址
    uint32_t base_addr = (uint32_t)handler;
    // 一一赋值
    p_gate->offset_low = base_addr & 0xFFFF;
    p_gate->selector = SELECTOR_KERNEL_CS;
    p_gate->dcount = 0;
    p_gate->attr = desc_type | (privilege << 5);
    // p_gate->offset_high = (base_addr >> 16) & 0xFFFF;
}

/*=========================================================================*
 *				seg2phys				   *
 *		由段名求其在内存中的物理地址
 *=========================================================================*/
phys_bytes seg2phys(uint16_t seg)
{
    SegDescriptor* p_dest = &gdt[seg >> 3];
    return (p_dest->base_high << 24 | p_dest->base_middle << 16 | p_dest->base_low);
}

