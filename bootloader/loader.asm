; Copyright (C) 2024 Lettle All rights reserved.
; See the copyright notice in the file LICENSE.
; Created by Lettle on 2024/3/31.
; QQ: 1071445082
; gitee: https://gitee.com/lettle/
; github: https://github.com/python-lettle/
; bilibili: https://space.bilibili.com/420393625

;FAT16目录项结构(32B)：
;名称		    偏移	    长度	描述
DIR_Name	    equ 0x00	;11	文件名8B，扩展名3B
DIR_Attr	    equ 0x0b	;1	文件属性
;保留	        0x0c	        10	保留位
DIR_WrtTime     equ 0x16	;2	最后一次写入时间
DIR_WrtDate     equ 0x18	;2	最后一次写入日期
DIR_FstClus     equ 0x1a	;2	起始簇号
DIR_FileSize    equ 0x1c	;4	文件大小

DISK_BUFFER equ 0x1e00              ;读磁盘用的缓存区
DISK_SIZE_M equ 4                   ; 磁盘容量，单位M。
FAT1_SECTORS equ 32                 ; FAT1占用扇区数
ROOT_DIR_SECTORS equ 32             ; 根目录占用扇区数
SECTOR_NUM_OF_FAT1_START equ 1      ; FAT1起始扇区号
SECTOR_NUM_OF_ROOT_DIR_START equ 33 ; 根目录起始扇区号
SECTOR_NUM_OF_DATA_START equ 65     ; 数据区起始扇区号，对应簇号为2。
SECTOR_CLUSTER_BALANCE equ 63       ; 簇号加上该值正好对应扇区号。
FILE_NAME_LENGTH equ 11             ; 文件名8字节加扩展名3字节共11字节。
DIR_ENTRY_SIZE equ 32               ; 目录项为32字节。
DIR_ENTRY_PER_SECTOR equ 16         ; 每个扇区能存放目录项的数目。

%include "addresses.inc"
%include "pm.inc"

org LOADER_ADDR
; 得到内存数
    mov ebx, 0              ; ebx = 后续值，开始时需为0
    mov di, _MemCheckBuffer ; es:di 指向一个地址范围描述符结构(Address Range Descriptor Structure)
.MemChkLoop:
	mov eax, 0E820h		; eax = 0000E820h
	mov ecx, 20			; ecx = 地址范围描述符结构的大小
	mov edx, 0534D4150h	; edx = 'SMAP'
	int 15h
	jc .MemChkFail		; 如果产生的进位，即CF = 1，跳转到.MemChkFail
	add di, 20
	inc dword [_ddMCRCount]	; _dwMCRNumber = ARDS　的个数
	cmp ebx, 0
	jne .MemChkLoop		; ebx != 0，继续进行循环
	jmp .MemChkOK		; ebx == 0，得到内存数OK
.MemChkFail:
	mov dword [_ddMCRCount], 0
.MemChkOK:

    ; 直接跳入32位保护模式
    cli                     ; 屏蔽中断
    lgdt [GDTPtr]           ; 初始化GDT
    ; 把 cr0 的最低位置为 1，开启保护模式
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    jmp Selector_code:PModeMain

;============================================================================
;   32位代码段
;----------------------------------------------------------------------------
[bits 32]
PModeMain:
    mov ax, 0x10        ; 将数据段寄存器ds和附加段寄存器es置为0x10
    mov ds, ax
    mov es, ax
    mov fs, ax          ; fs和gs寄存器由操作系统使用，这里统一设成0x10
    mov gs, ax
    mov ax, 0x18        ; 将栈段寄存器ss置为0x18
    mov ss, ax
    mov ebp, 0x7c00     ; 现在栈顶指向 0x7c00
    mov esp, ebp

    ; 打印字符串 'Chronix is in protected mode now!'
    push PM_STRING
    call Print

    ; 计算内存大小
    call CalMemSize
    ; 启动分页机制
    call SetupPaging

    ;--------------------
    ; 寻找内核文件
    ;--------------------
    ; 读取根目录的第1个扇区(1个扇区可以存放16个目录项)【计划根目录目录项个数不能超过16个】
    mov esi,SECTOR_NUM_OF_ROOT_DIR_START
    mov edi,DISK_BUFFER 
    mov ecx,1                   ; 读取1个扇区
    call read_one_sector32

    ; 查找kernel.bin文件
    cld         ; cld告诉程序si，di向地址增加的方向移动，std相反。
    mov bx,0    ; bx用来记录遍历的目录项序号(0~15)。
    next_dir_entry:
    mov si,bx
    shl si,5    ; 乘以32
    add si,DISK_BUFFER              ; 指向目录项中的文件名
    mov di,kernel_file_name_string  ; 指向kernel文件名
    mov cx,FILE_NAME_LENGTH         ; 文件名长度
    repe cmpsb                      ; 如果相等则比较下一个字节，直到ZF为0或CX为0，ZF为0表示发现不相等的字节，CX为0表示这一串字节相等。
    jcxz kernel_found               ; cx=0
    inc bx
    cmp bx,DIR_ENTRY_PER_SECTOR
    jl next_dir_entry
    jmp kernel_not_found

    kernel_found:
    ; 获取kernel起始簇号
    shl bx,5                ; 目录项序号乘以32得到目录项的偏移量
    add bx,DISK_BUFFER      ; 目录项在内存中的地址
    mov bx,[bx+DIR_FstClus] ; 保存kernel的起始簇号

    ; 读取FAT1的第1个扇区
    mov esi,SECTOR_NUM_OF_FAT1_START 
    mov edi,DISK_BUFFER 
    call read_one_sector32

    ; 按簇读kernel
    mov ebp,KERNEL_ADDR ; 目标地址初值
    read_kernel:
    xor esi,esi
    mov si,bx
    add esi,SECTOR_CLUSTER_BALANCE
    mov edi,ebp
    call read_one_sector32
    add ebp,512 ; 下一个目标地址

    ; 获取下一个簇号（每个FAT表项为2字节）
    shl bx,1                ; 乘2，每个FAT表项占2个字节
    mov bx,[bx+DISK_BUFFER]
    cmp bx,0xfff8           ; 大于等于0xfff8表示文件的最后一个簇
    jb read_kernel          ; jb无符号小于则跳转，jl有符号小于则跳转。

    goto_kernel:
    ; 打印字符串 "Kernel.bin Loading..."
    push kernel_found_str
    call Print

    ; 将MemSize当作参数 保存至 0x500
    mov eax, [_ddMemSize]
    mov [MEM_SIZE_PARAM], eax
    jmp dword KERNEL_ADDR ; 跳转到kernel
    jmp stop32
    

kernel_not_found:
    ; 打印字符串 'Chronix is in protected mode now!'
    push kernel_notfound_str
    call Print
stop32:
    hlt
    jmp stop32
 
%include "util32.inc"

[section .data32]
align 32

;----------------------------------------------------------------------------
;   16位实模式下使用的数据地址
;----------------------------------------------------------------------------
; 字符串 ---
_strMemSize:        db "Memory Size: ", 0
_strKB:             db "KB", 10, 0
_strSetupPaging:    db "Setup paging.", 10, 0
; 变量 ---
_ddMCRCount:        dd 0        ; 检查完成的ARDS的数量，为0则代表检查失败
_ddMemSize:         dd 0        ; 内存大小
_ddDispPosition:    dd (80 * 4 + 0) * 2 ; 初始化显示位置为第 4 行第 0 列
; 地址范围描述符结构(Address Range Descriptor Structure)
_ARDS:
    _ddBaseAddrLow:  dd 0        ; 基地址低32位
    _ddBaseAddrHigh: dd 0        ; 基地址高32位
    _ddLengthLow:    dd 0        ; 内存长度（字节）低32位
    _ddLengthHigh:   dd 0        ; 内存长度（字节）高32位
    _ddType:         dd 0        ; ARDS的类型，用于判断是否可以被OS使用
; 内存检查结果缓冲区，用于存放没存检查的ARDS结构，256字节是为了对齐32位，256/20=12.8
; ，所以这个缓冲区可以存放12个ARDS。
_MemCheckBuffer:          times 256 db 0

PM_STRING: db 'Chronix is in protected mode now!', 10, 0
kernel_notfound_str: db "Kernel Not Found", 10, 0
kernel_found_str: db "Kernel.bin Loading...", 10, 0
paging_start_str: db "Paging start!", 10, 0
kernel_file_name_string: db 'KERNEL  BIN', 0

ddDispPosition: dd 0xb8000 +  (80 * 1 + 0) * 2

;============================================================================
;   GDT全局描述符表相关信息
;----------------------------------------------------------------------------
gdt_start:
; 第一个描述符必须是空描述符
gdt_null:
    dd 0
    dd 0
; 代码段描述符
gdt_code:
    dw 0xffff ; Limit (bits 0-15)
    dw 0x0 ; Base (bits 0-15)
    db 0x0 ; Base (bits 16-23)
    db 10011010b ; Access Byte
    db 11001111b ; Flags , Limit (bits 16-19)
    db 0x0 ; Base (bits 24-31)
; 数据段描述符
gdt_data:
    dw 0xffff ; Limit (bits 0-15)
    dw 0x0 ; Base (bits 0-15)
    db 0x0 ; Base (bits 16-23)
    db 10010010b ; Access Byte
    db 11001111b ; Flags , Limit (bits 16-19)
    db 0x0 ; Base (bits 24-31)
; 栈段描述符
gdt_stack:
    dw 0x7c00 ; Limit (bits 0-15)
    dw 0x0 ; Base (bits 0-15)
    db 0x0 ; Base (bits 16-23)
    db 10010010b ; Access Byte
    db 01000000b ; Flags , Limit (bits 16-19)
    db 0x0 ; Base (bits 24-31)
gdt_end:

; GDT descriptior
GDTPtr:
dw gdt_end - gdt_start - 1 ; Size of our GDT, always less one of the true size
dd gdt_start ; Start address of our GDT

; 段选择子
Selector_code    equ gdt_code - gdt_start
Selector_data    equ gdt_data - gdt_start
Selector_stack   equ gdt_stack - gdt_start
