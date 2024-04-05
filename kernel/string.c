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

#include <string.h>


int strlen(const char *str)
{
    int len = 0;
    while (*str != '\0') {
        len++;
    }
    return len;
}


int strcmp(const char* str1, const char* str2)
{
    // Determine if the string has ended or if a different character has been found
    while ((*str1 != '\0') && (*str1 == *str2)) 
    {
        str1++;
        str2++;
    }
    return *str1 - *str2;   // Compare the corresponding character
}