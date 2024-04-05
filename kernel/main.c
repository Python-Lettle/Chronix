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
// int display_position = (80 * 6 + 0) * 2;     // 从第 6 行第 0 列开始显示

void chronix_main(int memsize){     // 2001c

    Terminal terminal;
    Terminal_init(&terminal, 6, 0);

    // Print memory size
    memsize = memsize / 1024;       // KB
    if (memsize != 0) {
        char memsize_str[10] = {0};
        terminal.print(&terminal, "OS memory size(KB): ");
        terminal.print(&terminal, itoa(memsize, memsize_str, 10));
        terminal.print(&terminal, "\n");
    } else {
        terminal.print(&terminal, "Can not calculate memory size.\n");
    }
    terminal.print(&terminal, "root@ChronixOS");
    
    while (1){}
}
