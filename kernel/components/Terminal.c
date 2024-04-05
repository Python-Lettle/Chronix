/**
 * Copyright (C) 2024 Lettle All rights reserved.
 * See the copyright notice in the file LICENSE.
 * Created by Lettle on 2024/4/5.
 * QQ: 1071445082
 * Email: 1071445082@qq.com
 * gitee: https://gitee.com/lettle/
 * github: https://github.com/python-lettle/
 * bilibili: https://space.bilibili.com/420393625
 */
#include "Terminal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void Terminal_print(Terminal *self, const char *str)
{
    // 分析 str 中的换行符
    while(*str != '\0') {
        if(*str == '\n') {
            self->row++;
            self->col = 0;
        } else {
            putchar(*str, self->row, self->col++);
        }
        str++;
    }

    // print_str(str, self->row, self->col);
}

void Terminal_init(Terminal *terminal, int row, int col)
{
    terminal->row = row;
    terminal->col = col;
    terminal->print = Terminal_print;
}