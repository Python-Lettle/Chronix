/**
 * Copyright (C) 2024 Lettle All rights reserved.
 * See the copyright notice in the file LICENSE.
 * Created by Lettle on 2024/4/19.
 * QQ: 1071445082
 * Email: 1071445082@qq.com
 * gitee: https://gitee.com/lettle/
 * github: https://github.com/python-lettle/
 * bilibili: https://space.bilibili.com/420393625
 */

#ifndef CHRONIX_MEMMAN_H
#define CHRONIX_MEMMAN_H

#include <type.h>
#include <config.h>

/**
 * 需要管理的页数量:
 * 
 */
#define PAGE_NUM 

typedef struct MemMan MemMan;
struct MemMan
{
    //====================
    // 属性区
    //====================
    uint8_t *page_using;        //指针，将会指向 0x100000 ~ 0x1FFFFF 的页表 (256 * 4096)

    //====================
    // 函数区
    //====================

};

/**
 * @brief 初始化 MemMan 对象，配置要管理的内存大小
 * 
 * @param memsize 总内存大小
 */
void MemMan_init(MemMan *man, int memsize);

#endif