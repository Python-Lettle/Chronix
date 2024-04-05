/**
 * Copyright (C) 2024 Lettle All rights reserved.
 * See the copyright notice in the file LICENSE.
 * Created by Lettle on 2024/4/5.
 * QQ: 1071445082
 * gitee: https://gitee.com/lettle/
 * github: https://github.com/python-lettle/
 * bilibili: https://space.bilibili.com/420393625
 */

#include <type.h>

bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}

bool is_alpha(char c)
{
    return is_lower(c) || is_upper(c);
}

bool is_alpha_digit(char c)
{
    return is_alpha(c) || is_digit(c);
}

bool is_space(char c)
{
    return c == ' ';
}

bool is_upper(char c)
{
    return c >= 'A' && c <= 'Z';
}

bool is_lower(char c)
{
    return c >= 'a' && c <= 'z';
}

bool is_sign(char c)
{
    return (c>='!' && c<='/') || (c>=':' && c<='@') || (c>='[' && c<='\\') || (c>='{' && c<='~');
}

