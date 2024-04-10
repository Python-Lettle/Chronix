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

#include <stdlib.h>
#include "global.h"

void chronix_main(void)      // 2001c
{

    // Print memory size
    if (boot_param->memsize != 0) {
        int memsize = boot_param->memsize / 1024;       // KB
        char memsize_str[10] = {0};
        terminal.print(&terminal, "OS memory size(KB): ");
        terminal.print(&terminal, itoa(memsize, memsize_str, 10));
        terminal.print(&terminal, "\n");
    } else {
        terminal.print(&terminal, "Can not calculate memory size.\n");
    }
    terminal.print(&terminal, "root@Chronix$\n");
    
    int a=0;
    int b=11/a;

    while (1){ _io_hlt();}
}
