; Copyright (C) 2024 Lettle All rights reserved.
; See the copyright notice in the file LICENSE.
; Created by Lettle on 2024/3/31.
; QQ: 1071445082
; gitee: https://gitee.com/lettle/
; github: https://github.com/python-lettle/
; bilibili: https://space.bilibili.com/420393625

;FAT16参数区：
BS_OEMName 		db 'Chronix '   ; 厂商名称（8字节，含空格）
BPB_BytesPerSec dw 0x0200		; 每扇区字节数
BPB_SecPerClus	db 0x01			; 每簇扇区数
BPB_RsvdSecCnt	dw 0x0001		; 保留扇区数（引导扇区的扇区数）
BPB_NumFATs		db 0x01			; FAT表的份数
BPB_RootEntCnt	dw 0x0200		; 根目录可容纳的目录项数
BPB_TotSec16	dw 0x2000 		; 扇区总数(4MB)
BPB_Media		db 0xf8 		; 介质描述符
BPB_FATSz16		dw 0x0020		; 每个FAT表扇区数
BPB_SecPerTrk	dw 0x0020		; 每磁道扇区数
BPB_NumHeads	dw 0x0040		; 磁头数
BPB_hiddSec		dd 0x00000000	; 隐藏扇区数
BPB_TotSec32	dd 0x00000000	; 如果BPB_TotSec16是0，由这个值记录扇区数。
BS_DrvNum		db 0x80			; int 13h的驱动器号
BS_Reserved1	db 0x00			; 未使用
BS_BootSig		db 0x29			; 扩展引导标记
BS_VolID		dd 0x00000000   ; 卷序列号
BS_VolLab		db 'Chronix    '; 卷标（11字节，含空格）
BS_FileSysType	db 'FAT16   '	; 文件系统类型（8字节，含空格）