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

#include <kernel/config.h>
#include <stdio.h>
#include <stdlib.h>

#include "components/Terminal.h"
#include "global.h"
#include "kernel.h"


void chronix_main(void)      // 2001c
{
    kernel_init();

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
