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

int memcmp(const void *s1, const void *s2, int n)
{
    /* 健壮性 */
    if ((s1 == 0) || (s2 == 0)) {
        return (s1 - s2);
    }

    register const char *p1 = s1;
    register const char *p2 = s2;
    int i;
    for (i = 0; i < n; i++,p1++,p2++) {
        if (*p1 != *p2) {
            return (*p1 - *p2);
        }
    }
    return 0;
}

int strlen(const char *str)
{
    int len = 0;
    while (*str != '\0') {
        len++;
    }
    return len;
}


int strcmp(register const char *s1, register const char *s2)
{
    while (*s1 == *s2++) {
        if (*s1++ == '\0') {
            return 0;
        }
    }
    if (*s1 == '\0') return -1;
    if (*--s2 == '\0') return 1;
    return (unsigned char) *s1 - (unsigned char) *s2;
}


/*===========================================================================*
 *				strncmp					     *
 *			  比较size个字符
 *===========================================================================*/
int strncmp(register const char *s1,register const char *s2,register int n)
{
    if (n) {
        do {
            if (*s1 != *s2++){
                break;
            }
            if (*s1++ == '\0'){
                return 0;
            }
        } while (--n > 0);

        if (n > 0) {
            if (*s1 == '\0') return -1;
            if (*--s2 == '\0') return 1;
            return (unsigned char) *s1 - (unsigned char) *s2;
        }
    }
    return 0;
}