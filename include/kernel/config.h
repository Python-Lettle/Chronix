/**
 * Copyright (C) 2024 Lettle All rights reserved.
 * See the copyright notice in the file LICENSE.
 * Created by Lettle on 2024/4/7.
 * QQ: 1071445082
 * Email: 1071445082@qq.com
 * gitee: https://gitee.com/lettle/
 * github: https://github.com/python-lettle/
 * bilibili: https://space.bilibili.com/420393625
 */

#ifndef CHRONIX_CONFIG_H
#define CHRONIX_CONFIG_H

#define BOOT_PARAM_ADDR 0x500       // boot参数地址
#define FREE_MEM_ADDR 0x100000      // 可用内存起始点

#define MAX_MEM_SIZE_MB 4096        // 操作系统内核支持的最大内存(MB)
#define MAX_MEM_SIZE_KB 4194304     // 操作系统内核支持的最大内存(KB)

/* 内核代码段、数据段基地址
 * 注意：要和GDT中设置的值保持一致！
 */
#define KERNEL_TEXT_SEG_BASE    0
#define KERNEL_DATA_SEG_BASE    0

/* Chronix所启用的控制台的数量等定义 */
#define NR_CONSOLES           	3	/* 系统控制台数量(1 ~ 9) */
#define	NR_RS_LINES	   		    0	/* rs232终端数量(0 ~ 2) */
#define	NR_PTYS		  	 	    0	/* 伪终端数量(0 ~ 64) */
/* 控制器任务的数量（/dev/cN设备类）。 */
#define NR_CONTROLLERS          1
/* 系统任务数量 */
#define NR_TASKS    (0 + NR_CONTROLLERS)
/* 进程表中的用户进程的槽数，这个配置决定了Chronix能同时运行多少个用户进程。 */
#define NR_PROCS          32

#endif