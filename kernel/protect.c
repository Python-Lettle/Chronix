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

/* 全局描述符表GDT */
SegDescriptor gdt[GDT_SIZE];

void protect_init(void)
{
    /* 将 LOADER 中的 GDT 拷贝到内核中新的 GDT 中 */
    memcpy(*((uint32_t *) vir2phys(&gdt_ptr[2])), &gdt, *((uint16_t *) vir2phys(&gdt_ptr[0])));

    /* 算出新 GDT 的基地址和界限，设置新的 gdt_ptr */
    uint16_t* p_gdt_limit = (uint16_t*)vir2phys(&gdt_ptr[0]);
    uint32_t* p_gdt_base = (uint32_t*)vir2phys(&gdt_ptr[2]);
    *p_gdt_limit = GDT_SIZE * DESCRIPTOR_SIZE - 1;
    *p_gdt_base = vir2phys(&gdt);
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
    p_desc->base_middle	= (base >> 16) & 0x0FF;     /* 段基址 2		(1 字节) */
    p_desc->access		= attribute & 0xFF;         /* 属性 1 */
    p_desc->granularity = ((limit >> 16) & 0x0F) |  /* 段界限 2 + 属性 2 */
                          ((attribute >> 8) & 0xF0);
    p_desc->base_high	= (base >> 24) & 0x0FF;     /* 段基址 3		(1 字节) */
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

