; Copyright (C) 2024 Lettle All rights reserved.
; See the copyright notice in the file LICENSE.
; Created by Lettle on 2024/3/31.
; QQ: 1071445082
; gitee: https://gitee.com/lettle/
; github: https://github.com/python-lettle/
; bilibili: https://space.bilibili.com/420393625

; 页目录开始地址:		1M
PAGE_DIR_BASE		equ	0x100000
; 页表开始地址:		1M + 4K
PAGE_TABLE_BASE		equ	0x101000

; LOADER.BIN 被加载的位置
LOADER_ADDR        equ     0x1000

; KERNEL.BIN 被加载到的位置
KERNEL_ADDR	    equ	    0x7e10

VIDEO_SEGMENT_ADDRESS equ 0xb800

; 储存启动时传递给内核的参数
; 500H ~ 600H: 内存大小 (256B)
BOOT_PARAMS equ 0x500
MEM_SIZE_PARAM equ BOOT_PARAMS