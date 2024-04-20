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

#ifndef CHRONIX_TERMINAL_H
#define CHRONIX_TERMINAL_H

#include <type.h>

// 终端标准函数头
#define _TERMINAL_FUNC_NOARG(funcname) void funcname(Terminal *self)
#define _TERMINAL_FUNC(funcname, args ...) void funcname(Terminal *self, args)

// 终端缓冲区大小
#define TERMINAL_INPUT_BUFFER_SIZE 32
#define MAX_CMD_LEN 64      // 指令名最长大小

/**
 * @brief 帮助操作系统管理终端输出位置、输出颜色
 * 
 */
typedef struct Terminal Terminal; // 前向声明结构体类型 
struct Terminal
{
    //====================
    // 属性区
    //====================
    // 光标位置
    int row;
    int col;

    // 可删除字符数
    char input_command[2000];
    int input_command_index;

    // 输入缓冲区
    char input_buffer[TERMINAL_INPUT_BUFFER_SIZE];
    uint8_t buffer_index;       // 缓冲区下一个可占用下标，从0开始依次增加，输出后一次清零


    //====================
    // 函数区
    //====================
    /**
     * @brief 终端输出
     * 
     * @param self 指向 Terminal 对象的指针
     * @param str 需要输出的字符串
     */
    void (*print)(Terminal *self, const char *str);

    /**
     * @brief 终端输出
     * 
     * @param self 指向 Terminal 对象的指针
     * @param num 需要输出的数字
     * @param base 以几进制的形式输出
     */
    void (*print_int)(Terminal *self, int num, int base);

    /**
     * @brief 退格，即删除上一个
     */
    void (*backspace)(Terminal *self);

    /**
     * @brief 终端换行
     */
    void (*new_line)(Terminal *self);

    /**
     * @brief 将缓冲区未输出到屏幕的内容全部输出，顺便清空缓冲区
     */
    void (*print_buffer)(Terminal *self);

    /**
     * @brief 键盘中断输入一个字符
     */
    void (*in_char)(Terminal *self, char ch);

    void (*refresh_cursor)(Terminal *self);

    /**
     * @brief 解析命令
     */
    void (*command_exec)(Terminal *self);

    void (*show_head)(Terminal *self);
};

/**
 * @brief 初始化 Terminal 对象，将 row 和 col 赋值给 Terminal 对象的 row 和 col 属性
 */
void Terminal_init(Terminal *terminal, int row, int col);

#endif