/**
 * Copyright (C) 2024 Lettle All rights reserved.
 * See the copyright notice in the file LICENSE.
 * Created by Lettle on 2024/4/5.
 * QQ: 1071445082
 * Email: 1071445082@qq.com
 * gitee: https://gitee.com/lettle/
 * github: https://github.com/python-lettle/
 * bilibili: https://space.bilibili.com/420393625
 * 
 * 这个文件里的定义仅供内核使用，与公共的type.h区分开
 */

#ifndef CHRONIX_TYPE_H
#define CHRONIX_TYPE_H

#include <type.h>

// 内存相关

/* 虚拟内存块，一个块在INTEL的Chronix中是256个字节 */
typedef unsigned int vir_clicks;
/* 物理地址（字节） */
typedef unsigned long phys_bytes;
/* 物理内存块，一个块在INTEL的Chronix中是256个字节 */
typedef unsigned int phys_clicks;
typedef struct memory_map
{
    phys_bytes base;    // 这块内存的基地址
    phys_bytes size;    // 这块内存的大小
} MemoryMap;

/* 单位不一样，256字节 */
typedef struct memory_s
{
    phys_clicks base;
    phys_clicks size;
} Memory;

/* 受保护模式的段描述符
 * 段描述符是与Intel处理器结构相关的另一个结构,它是保证进程
 * 不会发生内存访问越限机制的一部分。
 */
typedef struct seg_descriptor_s 
{
    uint16_t limit_low;        /* 段界限低16位 */
    uint16_t base_low;         /* 段基址低16位 */
    uint8_t base_middle;       /* 段基址中8位 */
    uint8_t access;		    /* 访问权限：| P | DL | 1 | X | E | R | A | */
    uint8_t granularity;		/* 比较杂，最重要的有段粒度以及段界限的高4位| G | X  | 0 | A | LIMIT HIGHT | */
    uint8_t base_high;         /* 段基址高8位 */
} SegDescriptor;

/**
 * @brief Loader已经将启动参数放置在0x500中，这个结构体用来获取。
 */
struct BOOT_PARAM
{
    int memsize;
};

#endif