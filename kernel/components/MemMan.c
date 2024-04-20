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

// private
pde_t* get_pde(MemMan *self, int index)
{
    pde_t *pde_out;
    if (index >= PDE_COUNT || index < 0) {
        return 0;
    }
    *pde_out = self->pde_base[index];
    return pde_out;
}

uint32_t parse_phys_addr(MemMan *self, uint32_t virtual_address)
{
    uint32_t offset = virtual_address & 0xFFF;
    uint32_t pte_index = (virtual_address >> 12) & 0x3FF;
    uint32_t pde_index = (virtual_address >> 22) & 0x3FF;

    pde_t * pde = get_pde(self, pde_index);
    pte_t * pte = (pte_t *)pde->frame;

    terminal.print(&terminal, "pde avail=");
    terminal.print_int(&terminal, pde->available, 10);
    terminal.new_line(&terminal);
    terminal.print(&terminal, "pde_index=");
    terminal.print_int(&terminal, pde_index, 10);
    terminal.new_line(&terminal);
    terminal.print(&terminal, "pte_index=");
    terminal.print_int(&terminal, pte_index, 10);
    terminal.new_line(&terminal);
    terminal.print(&terminal, "offset(16)=");
    terminal.print_int(&terminal, offset, 16);
    terminal.new_line(&terminal);

    return pte->frame + offset;
}

void MemMan_init(MemMan *man, int memsize)
{
    man->memsize = memsize;
    man->pde_base = (pde_t*)PAGE_DIR_BASE;
    man->pte_tables_base = (pte_t*)PAGE_TABLE_BASE;

    man->parse_phys_addr = parse_phys_addr;
}

