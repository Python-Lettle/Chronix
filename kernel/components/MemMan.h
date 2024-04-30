/**
 * Copyright (C) 2024 Lettle All rights reserved.
 * See the copyright notice in the file LICENSE.
 * Created by Lettle on 2024/4/19.
 * QQ: 1071445082
 * Email: 1071445082@qq.com
 * gitee: https://gitee.com/lettle/
 * github: https://github.com/python-lettle/
 * bilibili: https://space.bilibili.com/420393625
 */

#ifndef CHRONIX_MEMMAN_H
#define CHRONIX_MEMMAN_H

#include <kernel/type.h>
#include <kernel/config.h>
#include <util/bitmap.h>

/**
 * 需要管理的页数量:
 * 
 */
#define PDE_COUNT 1024              // 页目录项的数量
#define PTE_COUNT_PER_TABLE 1024    // 每个页表的页表项数量
#define MEM_PAGE_SIZE 4             // 内存页大小 4KB
#define KERNEL_PAGE 1281            // 初始5M+4K设置不可使用, 即 (5M+4K) / 4K = 1281 个页

#define MEM_INFO_MAX 1024

// 页目录开始地址:		1M
#define PAGE_DIR_BASE 0x100000
// 页表开始地址:		1M + 4K
#define PAGE_TABLE_BASE	0x101000

typedef struct MemMan MemMan;
struct MemMan
{
    //====================
    // 属性区
    //====================
    uint32_t memsize;

    pde_t* pde_base;

    bitmap_t* mem_bitmap;       // 内存位图

    MemInfo* mem_info;     // 最多保存 MEM_INFO_MAX 条虚拟地址的开启

    //====================
    // 函数区
    //====================
    uint32_t (*parse_phys_addr) (MemMan *self, uint32_t virtual_address);
    uint32_t (*parse_vir_addr) (MemMan *self, int pde_index, int pte_index, int offset);

    void* (*malloc) (MemMan *self, int size);
    void* (*malloc_phy) (MemMan *self, int size);
    bool  (*free) (MemMan *self, uint32_t vir_address);
    bool  (*free_phy) (MemMan *self, uint32_t address);

    bool (*bitmap_show) (MemMan *self, int index);
    void (*meminfo_show) (MemMan *self, int index);
};

/**
 * @brief 初始化 MemMan 对象，配置要管理的内存大小
 * 
 * @param memsize 总内存大小
 */
void MemMan_init(MemMan *man, int memsize);

#endif