/**
 * Copyright (C) 2024 Lettle All rights reserved.
 * See the copyright notice in the file LICENSE.
 * Created by Lettle on 2024/4/8.
 * QQ: 1071445082
 * Email: 1071445082@qq.com
 * gitee: https://gitee.com/lettle/
 * github: https://github.com/python-lettle/
 * bilibili: https://space.bilibili.com/420393625
 */
#include "prototype.h"
#include "global.h"
#include "components/Terminal.h"
#include "components/MemMan.h"
#include <type.h>

// 定义全局变量
struct BOOT_PARAM *boot_param;
Terminal terminal;
MemMan memman;

void kernel_init(void)
{
    // Boot params 获取
    boot_param = (struct BOOT_PARAM *)BOOT_PARAM_ADDR;
    // 初始化 Terminal
    Terminal_init(&terminal, 5, 0);
    
    protect_init();     // GDT, IDT, TSS 初始化
    interrupt_init();   // 中断初始化

    MemMan_init(&memman, boot_param->memsize);
}
