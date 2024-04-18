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
#include <stdlib.h>

int atoi(const char *str)
{
    int result = 0;
    int sign = 1;
    if (*str == '-') {
        sign = -1;
        str++;
    }
    while (*str) {
        result = result * 10 + (*str - '0');
        str++; // skip leading spaces
    }
    return result * sign;
}

char * itoa(int num, char *str, int base)
{
    if (base == 16) {
        itoa_hex(num, str);
        return str;
    }
    int i = 0;
    int isNegative = 0;
    // 处理负数的情况
    if (num < 0 && base == 10) {
        isNegative = 1;
        num = -num;
    }

    // 反转处理数字
    do {
        int digit = num % base;
        if (digit < 10)
            str[i++] = '0' + digit;
        else
            str[i++] = 'a' + digit - 10;
    } while (num /= base);

    // 如果为负数，则添加 '-'
    if (isNegative)
        str[i++] = '-';
    str[i] = '\0'; // 字符串结束符

    // 反转字符串
    for (int j = 0; j < i / 2; j++) {
        char temp = str[j];
        str[j] = str[i - 1 - j];
        str[i - 1 - j] = temp;
    }
    return str;
}

void itoa_hex(int num, char str[]) {
    char digits[] = "0123456789abcdef";
    int i = 0;
    int isNegative = 0;

    // 处理负数的情况
    if (num < 0) {
        isNegative = 1;
        num = -num;
    }

    // 生成数字字符串
    do {
        str[i++] = digits[num % 16];
    } while (num /= 16);

    // 如果为负数，则添加 '-'
    if (isNegative) {
        str[i++] = '-';
    }
  
    str[i] = '\0'; // 添加字符串结束符

    // 反转字符串
    for (int j = 0; j < i / 2; j++) {
        char temp = str[j];
        str[j] = str[i - 1 - j];
        str[i - 1 - j] = temp;
    }
}