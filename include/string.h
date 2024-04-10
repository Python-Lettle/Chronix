/**
 * Copyright (C) 2024 Lettle All rights reserved.
 * See the copyright notice in the file LICENSE.
 * Created by Lettle on 2024/4/6.
 * QQ: 1071445082
 * Email: 1071445082@qq.com
 * gitee: https://gitee.com/lettle/
 * github: https://github.com/python-lettle/
 * bilibili: https://space.bilibili.com/420393625
 */

#ifndef CHROMIUM_STRING_H
#define CHROMIUM_STRING_H


/**
 * @brief 比较两个内存区域
 */
int memcmp(const void *s1, const void *s2, int n);

/**
 * @brief 内存复制
 * 
 * @param p_dst  目的地址
 * @param p_src  源地址
 * @param size   复制大小
 */
void* memcpy(void* p_dst, void* p_src, int size);

/**
 * @brief 设置内存内容
*/
void* memset(void *_s, int _c, int _n);

/**
 * @return length of string
 */
int strlen(const char *str);

/**
 * @brief Compare two strings to see if they are the same
 * 
 * @param stri1 
 * @param str2 
 * @return int Same returns 0, else returns >0 or <0 number
 */
int strcmp(register const char *s1, register const char *s2);

/**
 * @brief 比较n个字符
 */
int strncmp(register const char *s1,register const char *s2,register int n);

/**
 * @brief 在汇编实现的字符串拷贝
 */
char* strcpy(char* p_dst, char* p_src);

#endif