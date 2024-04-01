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
; LOADER.BIN 被加载的位置　--- 偏移地址
LOADER_OFFSET     equ     0x100
; LOADER.BIN 被加载到的位置　--- 物理地址　(= LOADER_SEG * 10h)
LOADER_PHY_ADDR     equ LOADER_SEG

; KERNEL.BIN 被加载到的位置 ----  段地址
KERNEL_SEG	    equ	    0x2000
; KERNEL.BIN 被加载到的位置 ---- 偏移地址
KERNEL_OFFSET	equ	    0
; KERNEL.BIN 被加载到的位置　--- 物理地址　(= KERNEL_SEG * 10h)
KERNEL_PHY_ADDR	equ	KERNEL_SEG

;; 为kernel.bin装载保留的内存大小(字节)，内核文件大小不应该超过这个大小！
KERNEL_HAVE_SPACE   equ LOADER_PHY_ADDR - KERNEL_PHY_ADDR

DISK_BUFFER equ 0x1e00 ;读磁盘用的缓存区，放到kernel之前的512字节。

VIDEO_SEGMENT_ADDRESS equ 0xb800
