; Copyright (C) 2024 Lettle All rights reserved.
; See the copyright notice in the file LICENSE.
; Created by Lettle on 2024/3/31.
; QQ: 1071445082
; gitee: https://gitee.com/lettle/
; github: https://github.com/python-lettle/
; bilibili: https://space.bilibili.com/420393625

; Boot 
KERNEL_ENTRY_POINT_PHY_ADDR     equ 0x2000

; 页目录开始地址:		1M
PAGE_DIR_BASE		equ	0x100000
; 页表开始地址:		1M + 4K
PAGE_TABLE_BASE		equ	0x101000

; LOADER.BIN 被加载的位置　--- 段地址
LOADER_SEG        equ     0x1000
; LOADER.BIN 被加载的位置　--- 偏移地址 预留出来以便存一些东西 避免边缘问题
LOADER_OFFSET     equ     0x100
; LOADER.BIN 被加载到的位置　--- 物理地址　(= LOADER_SEG * 10h)
LOADER_PHY_ADDR   equ LOADER_SEG * 10h

; KERNEL.BIN 被加载到的位置 ----  段地址
KERNEL_SEG	    equ	    0x2000
; KERNEL.BIN 被加载到的位置 ---- 偏移地址
KERNEL_OFFSET	equ	    0
; KERNEL.BIN 被加载到的位置　--- 物理地址　(= KERNEL_SEG * 10h)
KERNEL_PHY_ADDR	equ	KERNEL_SEG * 10h

;; 为kernel.bin装载保留的内存大小(字节)，内核文件大小不应该超过这个大小！
KERNEL_HAVE_SPACE   equ  KERNEL_PHY_ADDR - LOADER_PHY_ADDR

VIDEO_SEGMENT_ADDRESS equ 0xb800

; 储存启动时传递给内核的参数
; 500H ~ 600H: 内存大小 (256B)
BOOT_PARAMS equ 0x500
MEM_SIZE_PARAM equ BOOT_PARAMS