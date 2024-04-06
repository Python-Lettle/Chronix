/**
 * Copyright (C) 2024 Lettle All rights reserved.
 * See the copyright notice in the file LICENSE.
 * Created by Lettle on 2024/3/31.
 * QQ: 1071445082
 * Email: 1071445082@qq.com
 * gitee: https://gitee.com/lettle/
 * github: https://github.com/python-lettle/
 * bilibili: https://space.bilibili.com/420393625
 */

#include <stdio.h>
#include <stdlib.h>

#include "components/Terminal.h"

/**
 * @brief Place parameters in 0x500 in the struct, used to obtain startup parameters.
 */
struct BOOT_PARAM
{
    int memsize;
};

void chronix_main(void)      // 2001c
{
    // Boot params
    struct BOOT_PARAM *boot_param = (struct BOOT_PARAM *)0x500;

    Terminal terminal;
    Terminal_init(&terminal, 6, 0);

    // Print memory size
    int memsize = boot_param->memsize / 1024;       // KB
    if (memsize != 0) {
        char memsize_str[10] = {0};
        terminal.print(&terminal, "OS memory size(KB): ");
        terminal.print(&terminal, itoa(memsize, memsize_str, 10));
        terminal.print(&terminal, "\n");
    } else {
        terminal.print(&terminal, "Can not calculate memory size.\n");
    }
    terminal.print(&terminal, "root@Chronix$");
    
    while (1){}
}
