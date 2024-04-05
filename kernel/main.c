/**
 * Copyright (C) 2024 Lettle All rights reserved.
 * See the copyright notice in the file LICENSE.
 * Created by Lettle on 2024/3/31.
 * QQ: 1071445082
 * gitee: https://gitee.com/lettle/
 * github: https://github.com/python-lettle/
 * bilibili: https://space.bilibili.com/420393625
 */

#include <stdio.h>
#include <stdlib.h>
// int display_position = (80 * 6 + 0) * 2;     // 从第 6 行第 0 列开始显示


void chronix_main(int memsize){
    memsize = memsize / 1024;       // 单位KB
    if (memsize != 0) {
        char memsize_str[10] = {0};
        print_str("OS memory size(KB): ", 6, 0);
        print_str(itoa(memsize, memsize_str, 10), 6, 20);
    } else {
        print_str("Can not calculate memory size.", 6, 0);
    }
    print_str("root@ChronixOS", 7, 0);
    
    while (1){}
}
