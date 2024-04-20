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

/**
 * 需要管理的页数量:
 * 
 */
#define PDE_COUNT 1024 // 页目录项的数量
#define PTE_COUNT_PER_TABLE 1024 // 每个页表的页表项数量

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
    pte_t* pte_tables_base;

    //====================
    // 函数区
    //====================
    uint32_t (*parse_phys_addr) (MemMan *self, uint32_t virtual_address);
};

/**
 * @brief 初始化 MemMan 对象，配置要管理的内存大小
 * 
 * @param memsize 总内存大小
 */
void MemMan_init(MemMan *man, int memsize);

#endif