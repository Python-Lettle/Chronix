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

    //==============================
    // 解析各种命令
    //==============================
    if (!strcmp(command, "ls")) {
        self->print(self, ". ..");

    } else if (!strcmp(command, "addr")) {
        // 获取第一个参数
        char arg1[MAX_CMD_LEN];
        int arg1_index = 0;
        p++;
        while((*p!='\0') && (*p != ' ')) {
            arg1[arg1_index++] = *p;
            p++;
        }
        arg1[arg1_index] = '\0';

        self->print(self, "parsing ");
        int address = atoi(arg1, 10);
        self->print_int(self, address, 16);
        self->print(self, " to physical address...\n");
        
        memman.parse_phys_addr(&memman, address);

    } else if (!strcmp(command, "clear")) {
        self->clear(self);

    } else if (!strcmp(command, "bitmap")) {
        // 获取第一个参数
        char arg1[MAX_CMD_LEN];
        int arg1_index = 0;
        p++;
        while((*p!='\0') && (*p != ' ')) {
            arg1[arg1_index++] = *p;
            p++;
        }
        arg1[arg1_index] = '\0';
        // 判断第一个参数
        if (!(strcmp(arg1, "first"))) {
            terminal.print(&terminal, "First empty memory page bitmap index: ");
            terminal.print_int(&terminal, bitmap_first_0(memman.mem_bitmap), 10);
        } else {
            memman.bitmap_show(&memman, atoi(arg1, 10));
        }
    
    } else if (!strcmp(command, "meminfo")) {
        // 获取第一个参数
        char arg1[MAX_CMD_LEN];
        int arg1_index = 0;
        p++;
        while((*p!='\0') && (*p != ' ')) {
            arg1[arg1_index++] = *p;
            p++;
        }
        arg1[arg1_index] = '\0';
        
        memman.meminfo_show(&memman, atoi(arg1, 10));

    } else if (!strcmp(command, "createmem")) {
        terminal.print(&terminal, "Memory address: ");
        terminal.print_int(&terminal, memman.malloc_phy(&memman, 10), 16);

    } else if (!strcmp(command, "freemem")) {
        // 获取第一个参数
        char arg1[MAX_CMD_LEN];
        int arg1_index = 0;
        p++;
        while((*p!='\0') && (*p != ' ')) {
            arg1[arg1_index++] = *p;
            p++;
        }
        arg1[arg1_index] = '\0';

        if (memman.free_phy(&memman, atoi(arg1, 16))) {
            terminal.print(&terminal, "Memory free done!");
        }

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
    char num_str[40] = {0};
    itoa(num, num_str, base);
    self->print(self, num_str);
}

_TERMINAL_FUNC(Terminal_print_info, const char *str)
{
    self->print(self, "[Kernel Info] ");
    self->print(self, str);
}

// 清屏函数
_TERMINAL_FUNC_NOARG(Terminal_clear)
{
    // 输出满屏空白
    uint16_t* vga_p = (uint16_t *)0xb8000;
    for(int i=0; i<2000; i++) {
        *vga_p = (' ' & 0xff) | (0x07 << 8);
        vga_p++;
    }

    // 重置行列和光标
    self->row = -1;
    self->col = 0;
    self->refresh_cursor(self);
}

// 滚屏函数
_TERMINAL_FUNC(Terminal_scrollup, int line)
{
    memcpy(0xb8000, 0xb80A0, 3840); // 从第二行到最后一行整体向上复制一行
    // 清空最后一行
    for (int i=0; i<80; i++) {
        memset(0xb8f00+i*2, (' ' & 0xff), 1);
        memset(0xb8f00+i*2+1, (0x07 << 8), 1);
    }
    self->col = 0;
    self->refresh_cursor(self);
}
/**
 * @brief 换行
 */
_TERMINAL_FUNC_NOARG(Terminal_new_line)
{
    if (self->row < 24) {
        self->row++;
    } else {
        // 触发滚屏
        self->scrollup(self);
    }
    
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
    terminal->print_info = Terminal_print_info;
    terminal->scrollup = Terminal_scrollup;
    terminal->clear = Terminal_clear;
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