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

#ifndef CHRONIX_PROTECT_H
#define CHRONIX_PROTECT_H

#include <type.h>
#include <kernel/config.h>

/*================================================================================================*/
/* 描述符表项结构 */
/*================================================================================================*/
typedef struct descriptor_s {
    char limit[sizeof(uint16_t)];
    char base[sizeof(uint32_t)];		/* really u24_t + pad for 286 */
} Descriptor;

/*================================================================================================*/
/* 门描述符 */
/*================================================================================================*/
typedef struct gate_s
{
    uint16_t	offset_low;	    /* Offset Low */
    uint16_t	selector;	    /* Selector */
    uint8_t	    dcount;		    /* 该字段只在调用门描述符中有效。
                                   如果在利用调用门调用子程序时引起特权级的转换和堆栈的改变，需要将外层堆栈中的参数复制到内层堆栈。
				                   该双字计数字段就是用于说明这种情况发生时，要复制的双字参数的数量。 */

    uint8_t	    attr;		    /* P(1) DPL(2) DT(1) TYPE(4) */
    uint16_t	offset_high;	/* Offset High */
} Gate;

/*================================================================================================*/
/* 表大小 */
/*================================================================================================*/
#define GDT_SIZE (LDT_FIRST_INDEX + NR_TASKS + NR_PROCS) /* 全局描述符表 */
#define IDT_SIZE (INT_VECTOR_SYS_CALL + 1)  /* 只取最高的向量 */
#define LDT_SIZE         2	                /* Flyanx0.11 每个进程只有两个段，一个是正文段（代码段），
                                             * 另一个是数据段，而堆栈段则和数据段共用，以后可能会分的更
                                             * 细，但对于现在来说，这样就够了。
                                             */

/*================================================================================================*/
/* 固定的全局描述符。 */
/*================================================================================================*/
#define DUMMY_INDEX         0   /* GDT的头，无用 */
#define TEXT_INDEX          1   /* 0~4G，32位可读代码段 */
#define DATA_INDEX          2   /* 0~4G，32位可读写数据段 */
#define VIDEO_INDEX         3   /* 显存首地址，特权级3 */

#define TSS_INDEX           4   /* 任务状态段 */
#define LDT_FIRST_INDEX     5   /* 本地描述符 */

/*================================================================================================*/
/* 选择子 = (描述符索引 * 描述符大小) */
/*================================================================================================*/
#define SELECTOR_DUMMY      DUMMY_INDEX * DESCRIPTOR_SIZE
#define SELECTOR_TEXT       TEXT_INDEX * DESCRIPTOR_SIZE
#define SELECTOR_DATA       DATA_INDEX * DESCRIPTOR_SIZE
#define SELECTOR_VIDEO      VIDEO_INDEX * DESCRIPTOR_SIZE | SA_RPL3       /* DPL(特权级) = 3 */


#define DESCRIPTOR_SIZE             8   /* 描述符大小 */

#endif