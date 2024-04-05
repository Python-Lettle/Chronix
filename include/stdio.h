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

#ifndef CHRONIX_STDIO_H
#define CHRONIX_STDIO_H
#include <common.h>

/**
 * @brief 直接输出字符串到指定行列，不考虑折行等问题，即默认字符串长度不超过一行且没有换行符
 * 
 * @param str 要输出的字符串
 * @param row 行
 * @param col 列
 */
void print_str(const char* str, int row, int col);

/**
 * @brief 只输出一个字符到指定位置
 * @param ch  要输出的字符
 * @param row 行
 * @param col 列
 */
void putchar(char ch, int row, int col);

#endif