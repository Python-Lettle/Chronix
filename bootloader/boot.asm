;============================================================================
; Copyright (C) 2024 Lettle All rights reserved.
; See the copyright notice in the file LICENSE.
; Created by Lettle on 2024/4/16.
; QQ: 1071445082
; Email: 1071445082@qq.com
; gitee: https://gitee.com/lettle/
; github: https://github.com/python-lettle/
; bilibili: https://space.bilibili.com/420393625
;============================================================================
; 常量定义区
DISK_BUFFER equ 0x1000              ; 读磁盘用的缓存区
DISK_SIZE_M equ 4                   ; 磁盘容量，单位M。
SECTOR_NUM_OF_FAT1_START equ 1      ; FAT1起始扇区号
SECTOR_NUM_OF_ROOT_DIR_START equ 33 ; 根目录起始扇区号
SECTOR_CLUSTER_BALANCE equ 63       ; 簇号加上该值正好对应扇区号。
FILE_NAME_LENGTH equ 11             ; 文件名8字节加扩展名3字节共11字节。
DIR_ENTRY_PER_SECTOR equ 16         ; 每个扇区能存放目录项的数目。
DIR_FstClus     equ    26    ; 2     起始簇号

KERNEL_ENTRY_POINT equ 0x2000

org 0x7c00
jmp _start16
nop

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

[bits 16]
_start16:
    ; 清屏
    mov ax, 0600h
    mov bx, 0700h
    mov cx, 0
    mov dx, 184fh
    int 10h     ; 调用0x10号中断

    cli             ; 屏蔽中断
    lgdt [GDTPtr]   ; 初始化GDT
    ; 把 cr0 的最低位置为 1，开启保护模式
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp Selector_code:PModeMain
 
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
    mov ebp,KERNEL_ENTRY_POINT ; 目标地址初值
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
    ; push kernel_found_str
    ; call Print
    jmp dword KERNEL_ENTRY_POINT ; 跳转到kernel
    jmp stop32
    
kernel_not_found:
stop32:
    hlt
    jmp stop32

; 变量 ---
; ddDispPosition: dd 0xb8000 +  (80 * 1 + 0) * 2
; 字符串常量
kernel_file_name_string: db 'KERNEL  BIN', 0
; kernel_found_str: db "Kernel Found", 10, 0

;============================================================================
; GDT
;============================================================================
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

;============================================================================
; 读取硬盘1个扇区函数（主通道主盘）【经测试，一次读多个扇区会有各种问题，需要分开一个扇区一个扇区的读。】
; 输入参数：esi，edi。
; 输出参数：无。
; esi 起始LBA扇区号
; edi 将数据写入的内存地址
;----------------------------------------------------------------------------
read_one_sector32:
    ; 第1步：设置要读取的扇区数
    mov dx,0x1f2
    mov al,1
    out dx,al ;读取的扇区数
    ; 第2步：将LBA地址存入0x1f3~0x1f6
    mov eax,esi
    ; LBA地址7~0位写入端口0x1f3
    mov dx,0x1f3
    out dx,al
    ; LBA地址15~8位写入端口写入0x1f4
    shr eax,8
    mov dx,0x1f4
    out dx,al
    ; LBA地址23~16位写入端口0x1f5
    shr eax,8
    mov dx,0x1f5
    out dx,al
    ; LBA28模式
    shr eax,8
    and al,0x0f ; LBA第24~27位
    or al,0xe0  ; 设置7~4位为1110，表示LBA模式，主盘
    mov dx,0x1f6
    out dx,al
    ; 第3步：向0x1f7端口写入读命令0x20
    mov dx,0x1f7
    mov al,0x20
    out dx,al
    ; 第4步：检测硬盘状态【该方法IDE硬盘正常，但SATA硬盘会表现为一直忙，无限循环卡在这里。】
    .not_ready:
    nop
    in al,dx    ; 读0x1f7端口
    and al,0x88 ; 第3位为1表示硬盘控制器已准备好数据传输，第7位为1表示硬盘忙。
    cmp al,0x08
    jnz .not_ready ; 若未准备好，继续等
    ; 第5步：从0x1f0端口读数据
    mov cx,256  ; 每次读取2字节，一个扇区需要读256次。
    mov dx,0x1f0
    .go_on_read:
    in ax,dx
    mov [edi],ax
    add edi,2
    loop .go_on_read
    ret

times 510-($-$$) db 0
db 0x55,0xaa