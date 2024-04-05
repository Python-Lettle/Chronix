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
#include <common.h>

/**
 * @brief 计算指定行列的偏移地址
 * 
 * @param row 
 * @param col 
 * @return uint16_t* 
 */
uint16_t* get_vga_mem_ptr(uint8_t row, uint8_t col)
{
    // 每行有80个字符，每个字符占两个字节
    return (uint16_t*)((unsigned char*)0xb8000 + 2 * (row * 80 + col));
}

/**
 * @brief 直接输出字符串到指定行列，不考虑折行等问题，即默认字符串长度不超过一行且没有换行符
 * 
 * @param str 要输出的字符串
 * @param row 行
 * @param col 列
 */
void print_str(const char* str, int row, int col)
{
    uint16_t* mem_ptr = get_vga_mem_ptr(row, col);
    while (*str != '\0') {
        *mem_ptr++ = (*str++ & 0xff) | (0x07 << 8); // 0x07 是属性编码，左移8位后与字符编码进行或运算
    }
}

void putchar(char ch, int row, int col)
{
    uint16_t* mem_ptr = get_vga_mem_ptr(row, col);
    // 设置字符（低8位）和属性（高8位，例如白色前景，黑色背景）
    *mem_ptr = (ch & 0xff) | (0x07 << 8); // 0x07 是属性编码，左移8位后与字符编码进行或运算
}