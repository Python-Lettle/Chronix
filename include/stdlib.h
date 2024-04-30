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
#ifndef CHRONIX_STDLIB_H
#define CHRONIX_STDLIB_H

/**
 * @brief 将字符串转换成int型数字
 * 
 * @param str 要转换成数字的字符串
 * @param base：转换后的进制数，可以是10进制、16进制等，范围必须在 2-36
 * @return int 转换后的数字
 */
int atoi(const char *str, int base);

/**
 * @brief 将int型数字转换成字符串
 * 
 * @param num：要转换的数据
 * @param str：目标字符串的地址
 * @param base：转换后的进制数，可以是10进制、16进制等，范围必须在 2-36
 */
char * itoa(int num, char str[], int base);

/**
 * @brief 秒 转化为 毫秒
 */
#define sec2ms(s) (s * 1000)

#endif