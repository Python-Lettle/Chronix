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
int strcmp(const char* stri1, const char* str2);

#endif