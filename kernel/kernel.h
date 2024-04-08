/**
 * Copyright (C) 2024 Lettle All rights reserved.
 * See the copyright notice in the file LICENSE.
 * Created by Lettle on 2024/4/8.
 * QQ: 1071445082
 * Email: 1071445082@qq.com
 * gitee: https://gitee.com/lettle/
 * github: https://github.com/python-lettle/
 * bilibili: https://space.bilibili.com/420393625
 * 
 * 这里将大部分库一起导入
 */
#ifndef CHRONIX_KERNEL_H
#define CHRONIX_KERNEL_H

#include <kernel/config.h>
#include <kernel/const.h>
#include <kernel/type.h>

#include <string.h>

//============================================================
// init.c
//============================================================
/**
 * @brief 内核初始化
 */
void kernel_init(void);

#endif