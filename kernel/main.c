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

// int display_position = (80 * 6 + 0) * 2;     // 从第 6 行第 0 列开始显示
int row = 4;
int col = 0;

void chronix_main(void){
    low_print("root@ChronixOS: /$", row, col);
    while (1){}
}
