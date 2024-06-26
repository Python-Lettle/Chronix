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

/* 端口数据类型，用于访问I/O端口 */
typedef unsigned port_t;
/* 寄存器数据类型，用于访问存储器段和CPU寄存器 */
typedef unsigned reg_t;

// 内存相关

typedef unsigned int vir_bytes;

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

// 假设PDE和PTE都是32位的，并包含一些常见的标志位
typedef struct
{
    uint32_t present       : 1;   // 页面是否存在于物理内存中
    uint32_t rw            : 1;   // 读/写权限
    uint32_t user          : 1;   // 用户/内核模式权限
    uint32_t pwt           : 1;   // Page Write-Through
    uint32_t pcd           : 1;   // Page Cache Disable
    uint32_t accessed      : 1;   // 页面是否被访问过
    uint32_t dirty         : 1;   // 页面是否被修改过

    uint32_t pat           : 1;   // Page Attribute Table
    uint32_t global        : 1;   // 全局页
    uint32_t available     : 3;   // 可用字段，供系统使用

    uint32_t frame         : 20;  // 物理页帧号
} pde_t;

typedef struct
{
    uint32_t present       : 1;   // 页面是否存在于物理内存中
    uint32_t rw            : 1;   // 读/写权限
    uint32_t user          : 1;   // 用户/内核模式权限
    uint32_t pwt           : 1;   // Page Write-Through
    uint32_t pcd           : 1;   // Page Cache Disable
    uint32_t accessed      : 1;   // 页面是否被访问过
    uint32_t dirty         : 1;   // 页面是否被修改过

    uint32_t pat           : 1;   // Page Attribute Table
    uint32_t global        : 1;   // 全局页
    uint32_t available     : 3;   // 可用字段，供系统使用
    
    uint32_t frame         : 20;  // 物理页帧号
} pte_t;

/**
 * @brief 内存管理使用的结构体
 * vir_address  内存虚拟地址
 * bitmap_index 位图起始索引
 * page_num     占用n个页
 */
typedef struct
{
    uint32_t vir_address;   // 分配的虚拟地址
    uint32_t bitmap_index;  // 0~1048575
    uint32_t page_num;      // 占用了多少页
} MemInfo;


/**
 * @brief Loader已经将启动参数放置在0x500中，这个结构体用来获取。
 */
struct BOOT_PARAM
{
    uint32_t memsize;
};

/* 硬件（异常）中断处理函数原型 */
typedef void (*int_handler_t)(void);
/* 中断请求处理函数原型 */
typedef int (*irq_handler_t)(int irq);
/* 系统调用函数原型 */
typedef void (*syscall_t) (void);

#endif