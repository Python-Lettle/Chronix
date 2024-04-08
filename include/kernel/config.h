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

/* 控制器任务的数量（/dev/cN设备类）。 */
#define NR_CONTROLLERS          1
/* 系统任务数量 */
#define NR_TASKS    (0 + NR_CONTROLLERS)
/* 进程表中的用户进程的槽数，这个配置决定了Chronix能同时运行多少个用户进程。 */
#define NR_PROCS          32

#endif