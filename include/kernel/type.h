/**
 * Copyright (C) 2024 Lettle All rights reserved.
 * See the copyright notice in the file LICENSE.
 * Created by Lettle on 2024/4/5.
 * QQ: 1071445082
 * Email: 1071445082@qq.com
 * gitee: https://gitee.com/lettle/
 * github: https://github.com/python-lettle/
 * bilibili: https://space.bilibili.com/420393625
 * 
 * 这个文件里的定义仅供内核使用，与公共的type.h区分开
 */

#ifndef CHRONIX_TYPE_H
#define CHRONIX_TYPE_H

// 内存相关

/* 虚拟内存块，一个块在INTEL的Chronix中是256个字节 */
typedef unsigned int vir_clicks;
/* 物理地址（字节） */
typedef unsigned long phys_bytes;
/* 物理内存块，一个块在INTEL的Chronix中是256个字节 */
typedef unsigned int phys_clicks;
typedef struct memory_map
{
    phys_bytes base;    // 这块内存的基地址
    phys_bytes size;    // 这块内存的大小
} MemoryMap;

/**
 * @brief Loader已经将启动参数放置在0x500中，这个结构体用来获取。
 */
struct BOOT_PARAM
{
    int memsize;
};

#endif