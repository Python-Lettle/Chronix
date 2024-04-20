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
#include "../global.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 解析命令
int command_exec(Terminal *self, const char* input_str)
{
    // 获取第一个空格以前的内容，就是指令名称
    char command[MAX_CMD_LEN];
    int command_index = 0;
    char *p = input_str;
    while((*p!='\0') && (*p!=' ')) {
        command[command_index++] = *p;
        p++;
    }
    command[command_index] = '\0';
    if (!strcmp(command, "ls")) {
        self->print(self, ". ..");
    } else if (!strcmp(command, "addr")) {
        char arg1[MAX_CMD_LEN];
        int arg1_index = 0;
        p++;
        while((*p!='\0') && (*p != ' ')) {
            arg1[arg1_index++] = *p;
            p++;
        }
        arg1[arg1_index] = '\0';

        self->print(self, "parsing ");
        int address = atoi(arg1);
        self->print_int(self, address, 16);
        self->print(self, " to physical address...\n");
        
        terminal.print_int(&terminal, memman.parse_phys_addr(&memman, address),16);
    } else {
        self->print(self, "command not found: ");
        self->print(self, command);
    }
}


//========================================
// 输入函数
//========================================
_TERMINAL_FUNC(Terminal_input, char ch)
{
    switch (ch)
    {
    case 8:
        /* 删除 */
        self->backspace(self);
        // 删缓存区
        if (self->buffer_index > 0) {
            self->input_buffer[self->buffer_index] = '\0';
            self->buffer_index--;
        }
        break;
    case '\n':
    case 13:
        self->command_exec(self);
        break;
    default:
        self->input_buffer[self->buffer_index++] = ch;
        self->input_command[++self->input_command_index] = ch;
        // 条件允许的情况下输出缓冲区
        self->print_buffer(self);
        break;
    }
}

//========================================
// 输出类函数
//========================================
// print_buffer
_TERMINAL_FUNC_NOARG(Terminal_print_buffer)
{
    // 向屏幕输出缓冲区内容
    self->input_buffer[self->buffer_index] = '\0'; // 调整要输出的字符串
    self->print(self, self->input_buffer);

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
            self->backspace(self);
        } else {
            putchar(*str, self->row, self->col++);
        }
        str++;
    }

    self->refresh_cursor(self);
}

_TERMINAL_FUNC(Terminal_print_int, int num, int base)
{
    char num_str[16] = {0};
    itoa(num, num_str, base);
    self->print(self, num_str);
}

/**
 * @brief 换行
 */
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
    uint16_t position = self->row * 80 + self->col; // 假设屏幕宽度为80个字符
    out_byte(0x3D4, 14); // 设置低8位
    out_byte(0x3D5, (position >> 8) & 0xFF); // 设置高8位
    out_byte(0x3D4, 15);
    out_byte(0x3D5, position & 0xFF);
}

_TERMINAL_FUNC_NOARG(Terminal_backspace)
{
    if (self->input_command_index >= 0) {
        // 删除命令区字符
        self->input_command[self->input_command_index--] = '\0';

        // 删除屏幕上的字符
        if (self->col == 0) {
            if (self->row == 0) return;
            putchar(' ', self->row, self->col);
            self->row--;
            self->col = 79;
        } else {
            putchar(' ', self->row, --self->col);
        }
        self->refresh_cursor(self);
    }
}

_TERMINAL_FUNC_NOARG(Terminal_show_head)
{
    self->print(self, "Chronix> ");
}

_TERMINAL_FUNC_NOARG(Terminal_command_exec)
{
    // 先折行
    self->new_line(self);

    // 调整命令
    self->input_command[++self->input_command_index] = '\0';

    // 解析命令
    command_exec(self, self->input_command);
    
    // 清空命令
    self->input_command[0] = '\0';
    self->input_command_index = -1;

    // 显示新的终端提示
    self->new_line(self);
    self->show_head(self);
}

void Terminal_init(Terminal *terminal, int row, int col)
{
    terminal->row = row;
    terminal->col = col;
    // terminal->deletable = 0;
    terminal->print = Terminal_print;
    terminal->print_int = Terminal_print_int;
    terminal->new_line = Terminal_new_line;
    terminal->in_char = Terminal_input;
    terminal->print_buffer = Terminal_print_buffer;
    terminal->refresh_cursor = Terminal_refresh_cursor;
    
    terminal->backspace = Terminal_backspace;
    terminal->show_head = Terminal_show_head;
    terminal->command_exec = Terminal_command_exec;

    terminal->input_buffer[0] = '\0';
    terminal->buffer_index = 0;

    terminal->input_command[0] = '\0';
    terminal->input_command_index = -1;
}