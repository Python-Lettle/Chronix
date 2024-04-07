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

// 基于POSIX标准

/* 标准输入/输出流的文件描述符 */
#define STDIN       0
#define STDOUT      1

/* 以下定义也在<unistd.h>中，它们不应该发生冲突！ */
/* "fseek"更改文件指针位置的三种操作，这些值不应被更改！  */
#define SEEK_SET	0	/* 从文件起点开始 */
#define SEEK_CUR	1	/* 从当前位置开始  */
#define SEEK_END	2	/* 从文件末尾开始  */

#define STR_DEFAULT_LEN 512

// 函数

/**
 * @brief 直接输出字符串到指定行列，不考虑折行等问题，即默认字符串长度不超过一行且没有换行符
 */
void print_str(const char* str, int row, int col);

/**
 * @brief 只输出一个字符到指定位置
 */
void putchar(char ch, int row, int col);

#endif