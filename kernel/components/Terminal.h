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

#ifndef CHRONIX_TERMINAL_H
#define CHRONIX_TERMINAL_H

/**
 * @brief 帮助操作系统管理终端输出位置、输出颜色
 * 
 */
typedef struct Terminal Terminal; // 前向声明结构体类型 
struct Terminal
{
    int row;
    int col;

    /**
     * @brief 终端输出
     * 
     * @param self 指向 Terminal 对象的指针
     * @param str 需要输出的字符串
     */
    void (*print)(Terminal *self, const char *str);

    void (*new_line)(Terminal *self);
};

/**
 * @brief 初始化 Terminal 对象，将 row 和 col 赋值给 Terminal 对象的 row 和 col 属性
 */
void Terminal_init(Terminal *terminal, int row, int col);

#endif