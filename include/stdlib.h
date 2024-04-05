#ifndef CHRONIX_STDLIB_H
#define CHRONIX_STDLIB_H

/**
 * @brief 将字符串转换成int型数字
 * 
 * @param str 要转换成数字的字符串
 * @return int 转换后的数字
 */
int atoi(const char *str);

/**
 * value：要转换的数据
 * string：目标字符串的地址
 * radix：转换后的进制数，可以是10进制、16进制等，范围必须在 2-36
 */
char * itoa(int num, char str[], int base);

#endif