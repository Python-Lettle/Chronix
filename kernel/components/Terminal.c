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

//========================================
// 输入函数
//========================================
_TERMINAL_FUNC(Terminal_input, char ch)
{
    self->input_buffer[self->buffer_index++] = ch;

    // char code[10] = {0};
    // itoa(ch, code, 10);
    // self->print(self, code);

    // 条件允许的情况下输出缓冲区
    self->print_buffer(self);
}

//========================================
// 输出类函数
//========================================
// print_buffer
_TERMINAL_FUNC_NOARG(Terminal_print_buffer)
{
    self->input_buffer[self->buffer_index] = '\0'; // 调整要输出的字符串
    self->print(self, self->input_buffer);
    self->deletable += self->buffer_index;

    // 将缓冲区及其下标置0
    self->buffer_index = 0;
    self->input_buffer[0] = '\0';
}

// print
_TERMINAL_FUNC(Terminal_print, const char *str)
{
    // 分析 str 中的换行符
    while(*str != '\0') {
        if(*str == '\n' || *str == 13) {
            self->new_line(self);
        } else if (*str == 8) {
            if(self->deletable>0) self->deletable--;
            self->backspace(self);
        } else {
            putchar(*str, self->row, self->col++);
        }
        str++;
    }

    self->refresh_cursor(self);
}


_TERMINAL_FUNC_NOARG(Terminal_new_line)
{
    self->row++;
    self->row = self->row % 25;
    self->col = 0;
}

//========================================
// 功能函数
//========================================

_TERMINAL_FUNC_NOARG(Terminal_refresh_cursor)
{
    unsigned short position = self->row * 80 + self->col; // 假设屏幕宽度为80个字符
    out_byte(0x3D4, position & 0xFF); // 设置低8位
    out_byte(0x3D5, (position >> 8) & 0xFF); // 设置高8位
    out_byte(0x3D4, 0x0F);
    out_byte(0x3D5, 0x0F);
}

_TERMINAL_FUNC_NOARG(Terminal_backspace)
{
    if (self->deletable > 0) {
        self->deletable--;
        if (self->col == 0) {
            if (self->row == 0) return;
            putchar(' ', self->row, self->col);
            self->row--;
            self->col = 79;
        } else {
            putchar(' ', self->row, --self->col);
        }
    }
}

void Terminal_init(Terminal *terminal, int row, int col)
{
    terminal->row = row;
    terminal->col = col;
    terminal->deletable = 0;
    terminal->print = Terminal_print;
    terminal->new_line = Terminal_new_line;
    terminal->in_char = Terminal_input;
    terminal->print_buffer = Terminal_print_buffer;
    terminal->refresh_cursor = Terminal_refresh_cursor;
    terminal->backspace = Terminal_backspace;

    terminal->input_buffer[0] = '\0';
    terminal->buffer_index = 0;
}