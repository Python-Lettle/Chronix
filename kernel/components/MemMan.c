/**
 * Copyright (C) 2024 Lettle All rights reserved.
 * See the copyright notice in the file LICENSE.
 * Created by Lettle on 2024/4/20.
 * QQ: 1071445082
 * Email: 1071445082@qq.com
 * gitee: https://gitee.com/lettle/
 * github: https://github.com/python-lettle/
 * bilibili: https://space.bilibili.com/420393625
 */
#include "MemMan.h"
#include "../global.h"

// 内存位图变量
static uint32_t bitmap_array[MAX_MEM_SIZE_KB / MEM_PAGE_SIZE / 32] = {0};
static bitmap_t bitmap;

static MemInfo mem_info_array[MEM_INFO_MAX];

// private
pde_t* get_pde(MemMan *self, int index)
{
    pde_t *pde_out;
    if (index >= PDE_COUNT || index < 0) return 0;
    *pde_out = self->pde_base[index];
    return pde_out;
}

pte_t* get_pte(pde_t *pde, int index)
{
    pte_t *pte_out;
    if (index >= PTE_COUNT_PER_TABLE || index < 0) return 0;
    *pte_out = ((pte_t *)pde->frame)[index];
    return pte_out;
}

uint32_t parse_phys_addr(MemMan *self, uint32_t virtual_address)
{
    uint32_t offset = virtual_address & 0xFFF;
    uint32_t pte_index = (virtual_address >> 12) & 0x3FF;
    uint32_t pde_index = (virtual_address >> 22) & 0x3FF;

    pde_t * pde = get_pde(self, pde_index);
    pte_t * pte = get_pte(pde, pte_index);

    terminal.print(&terminal, "pde present=");
    terminal.print_int(&terminal, pde->present, 10);
    terminal.new_line(&terminal);
    terminal.print(&terminal, "pde_index=");
    terminal.print_int(&terminal, pde_index, 10);
    terminal.new_line(&terminal);
    terminal.print(&terminal, "pde frame=");
    terminal.print_int(&terminal, pde->frame, 16);
    terminal.new_line(&terminal);

    terminal.print(&terminal, "pte present=");
    terminal.print_int(&terminal, pte->present, 10);
    terminal.new_line(&terminal);
    terminal.print(&terminal, "pte_index=");
    terminal.print_int(&terminal, pte_index, 10);
    terminal.new_line(&terminal);
    terminal.print(&terminal, "pte frame=");
    terminal.print_int(&terminal, pte->frame, 16);
    terminal.new_line(&terminal);

    // 返回地址
    if ((pde->present & pte->present) == 1) {
        terminal.print(&terminal, "offset(16)=");
        terminal.print_int(&terminal, offset, 16);
        terminal.new_line(&terminal);
        return pte->frame + offset;
    } else {
        terminal.print(&terminal, "Unavailable page.\n");
        return 0;
    }
}

uint32_t parse_vir_addr(MemMan *self, int pde_index, int pte_index, int offset)
{
    return offset | (pte_index<<12) | (pde_index<<22);
} 

void* malloc_phy (MemMan *self)
{
    // 寻找一个空闲页
    int empty_page_index = bitmap_first_0(self->mem_bitmap);
    // 在 bitmap 中占用
    bitmap_set_bit_1(self->mem_bitmap, empty_page_index);
    return empty_page_index*4096;
}

// 该函数默认一直有可用物理页
void* malloc (MemMan *self, int size)
{
    // malloc_phy (self, size)
    int page_num = size / 4096 + 1;                 // 计算一共需要多少个物理页

    int empty_page_index = bitmap_first_n_0(self->mem_bitmap, page_num);    // 寻找一个空闲页
    int phy_address = empty_page_index*4096;   

    // 在 bitmap 中占用
    for (int i=0; i<page_num; i++) {
        bitmap_set_bit_1(self->mem_bitmap, empty_page_index + i);
    }
    // malloc_phy end

    // 在页目录项和页表中找到一个可用的虚拟地址
    uint32_t address = 0;
    for (int pde_index = 0; pde_index < PDE_COUNT; pde_index++) {
        bool findPTE = FALSE;
        pde_t * pde = get_pde(self, pde_index);
        for (int pte_index = 1; pte_index < PTE_COUNT_PER_TABLE; pte_index++) {
            pte_t *pte = get_pte(pde, pte_index);
            terminal.print_int(&terminal, pte, 16);
            terminal.new_line(&terminal);
            if (pte->present == 0) {        // 该地址未被使用
                pde->present = 1;
                pte->present = 1;
                pte->frame = phy_address;
                address = self->parse_vir_addr(self, pde_index, pte_index, 0);
                findPTE = TRUE;
                break;
            }
        }
        if (findPTE) break;
    }

    // 填写 MemInfo
    for (int i=0; i<MEM_INFO_MAX; i++) {
        self->mem_info[i].vir_address = address;
        self->mem_info[i].page_num = page_num;
        self->mem_info[i].bitmap_index = empty_page_index;
    }

    return address;
}

bool free_phy (MemMan *self, uint32_t phy_address)
{
    int bitmap_index = phy_address / 4096;
    if (bitmap_get_bit(self->mem_bitmap, bitmap_index) == 1) {
        bitmap_set_bit_0(self->mem_bitmap, bitmap_index);
        return TRUE;
    }
    return FALSE;
}

bool free (MemMan *self, uint32_t vir_address)
{
    for (int i=0; i<MEM_INFO_MAX; i++) {
        if (self->mem_info[i].vir_address == vir_address) {
            for (int j=0; j<self->mem_info[i].page_num; j++) {
                bitmap_set_bit_0(self->mem_bitmap, self->mem_info[i].bitmap_index+j);
            }

            self->mem_info[i].vir_address = 0;
            self->mem_info[i].bitmap_index = 0;
            self->mem_info[i].page_num = 0;
        }
    }

    return FALSE;
}

void bitmap_show(MemMan *self, int index)
{
    terminal.print(&terminal, "bitmap [");
    terminal.print_int(&terminal, index, 10);
    terminal.print(&terminal, "] = ");
    terminal.print_int(&terminal, bitmap_get_bit(self->mem_bitmap, index), 10);
}

void meminfo_show(MemMan *self, int index)
{
    terminal.print(&terminal, "meminfo [");
    terminal.print_int(&terminal, index, 10);
    terminal.print(&terminal, "] = ");
    terminal.print_int(&terminal, self->mem_info[index].bitmap_index, 10);
    terminal.print(&terminal, ", ");
    terminal.print_int(&terminal, self->mem_info[index].page_num, 10);
    terminal.print(&terminal, ", ");
    terminal.print_int(&terminal, self->mem_info[index].vir_address, 16);
}

void MemMan_init(MemMan *man, int memsize)
{
    /* 初始化属性 */
    man->memsize = memsize;
    man->pde_base = (pde_t*)PAGE_DIR_BASE;
    man->mem_info = mem_info_array;
    /* 初始化函数 */
    man->parse_phys_addr = parse_phys_addr;
    man->parse_vir_addr = parse_vir_addr;
    man->malloc = malloc;
    man->malloc_phy = malloc_phy;
    man->free = free;
    man->free_phy = free_phy;
    man->bitmap_show = bitmap_show;
    man->meminfo_show = meminfo_show;


    /* 初始化bitmap */
    int array_use_len = memsize/1024/MEM_PAGE_SIZE;
    man->mem_bitmap = &bitmap;
    bitmap_init(man->mem_bitmap, bitmap_array, array_use_len);
    
    // 初始2M设置不可使用, 即1281个页
    int i = 0;
    for (; i<KERNEL_PAGE; i++) bitmap_set_bit_1(man->mem_bitmap, i);

    // 按照memsize设置可用区间
    for (; i<array_use_len; i++) bitmap_set_bit_0(man->mem_bitmap, i);

}

