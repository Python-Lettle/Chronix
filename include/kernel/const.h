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

#ifndef CHRONIX_CONST_H
#define CHRONIX_CONST_H

// 虚拟地址转化为物理地址
#define	vir2phys(vir) ((phys_bytes)(KERNEL_DATA_SEG_BASE) + (vir_bytes)(vir))

#endif