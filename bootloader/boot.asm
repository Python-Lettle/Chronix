;========================================
; 常量
;========================================
; FAT16目录项中各成员的偏移量：
; 名称                 偏移     长度    描述
DIR_Name        equ    0     ; 11    文件名8B，扩展名3B
DIR_Attr        equ    11    ; 1     目录项属性
;Reserved       equ    12    ; 10    保留位
DIR_WrtTime     equ    22    ; 2     最后一次写入时间
DIR_WrtDate     equ    24    ; 2     最后一次写入日期
DIR_FstClus     equ    26    ; 2     起始簇号
DIR_FileSize    equ    28    ; 4     文件大小

DISK_SIZE_M equ 4                           ; 磁盘容量，单位M。
FAT1_SECTORS     equ 32                     ; FAT1占用扇区数
ROOT_DIR_SECTORS equ 32                     ; 根目录占用扇区数
SECTOR_NUM_OF_FAT1_START     equ 1          ; FAT1表起始扇区号
SECTOR_NUM_OF_ROOT_DIR_START equ 33         ; 根目录区起始扇区号
SECTOR_NUM_OF_DATA_START     equ 65         ; 数据区起始扇区号，对应簇号为2。
SECTOR_CLUSTER_BALANCE       equ 63         ; 簇号加上该值正好对应扇区号。
FILE_NAME_LENGTH     equ 11                 ; 文件名8字节加扩展名3字节共11字节。
DIR_ENTRY_SIZE       equ 32                 ; 目录项为32字节。
DIR_ENTRY_PER_SECTOR equ 16                 ; 每个扇区能存放目录项的数目。

VIDEO_CHAR_MAX_COUNT equ 2000
BaseOfStack equ 0x7c00

%include "addresses.inc"

;========================================
; 跳入引导程序 定义FAT16参数
;========================================
org 0x7c00
jmp boot_start
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


[BITS 16]  ; 设置为16位模式
;========================================
; 引导程序开始
;========================================
boot_start:
    ; 初始化寄存器
    mov ax,cs
    mov ds,ax
    mov es,ax                   ; cmpsb会用到ds:si和es:di
    mov ss,ax
    mov sp, BaseOfStack
    mov ax, VIDEO_SEGMENT_ADDRESS
    mov gs,ax                   ; 本程序中gs专用于指向显存段

    ; 清屏
    call clear_screen

    ; 打印字符串："Chronix boot start."
    mov si,boot_start_string
    mov di,0 ;在屏幕第1行显示
    call print16

    ; 读取loader文件开始
    ; 读取根目录的第1个扇区（1个扇区可以存放16个目录项，我们用到的文件少，不会超过16个。）
    mov esi,SECTOR_NUM_OF_ROOT_DIR_START 
    mov di,DISK_BUFFER
    call read_one_sector16

    ; 在16个目录项中通过文件名查找文件
    cld                             ; cld将标志位DF置0，在串处理指令中控制每次操作后让si和di自动递增。std相反。下面repe cmpsb会用到。
    mov bx,0                        ; 用bx记录遍历第几个目录项。
next_dir_entry:
    mov si,bx
    shl si,5                        ; 乘以32（目录项的大小）
    add si,DISK_BUFFER              ; 源地址指向目录项中的文件名。
    mov di,loader_file_name_string  ; 目标地址指向loader程序在硬盘中的正确文件名。
    mov cx,FILE_NAME_LENGTH         ; 字符比较次数为FAT16文件名长度，每比较一个字符，cx会自动减一。
    repe cmpsb                      ; 逐字节比较ds:si和es:di指向的两个字符串。
    jcxz loader_found               ; 当cx为0时跳转。cx为0表示上面比较的两个字符串相同。找到了loader文件。
    inc bx
    cmp bx,DIR_ENTRY_PER_SECTOR
    jl next_dir_entry               ; 检查下一个目录项。
    jmp loader_not_found            ; 没有找到loader文件。

loader_found:
    ; 打印字符串："Loader found."
    mov si,loader_found_string
    mov di,80 ; 在屏幕第2行显示
    call print16

    ; 从目录项中获取loader文件的起始簇号
    shl bx,5 ; 乘以32
    add bx,DISK_BUFFER
    mov bx,[bx+DIR_FstClus]         ; loader的起始簇号

    ; 读取FAT1表的第1个扇区（我们用到的文件少且小，只用到了该扇区中的簇号）
    mov esi,SECTOR_NUM_OF_FAT1_START 
    mov di,DISK_BUFFER              ; 放到boot程序之后
    call read_one_sector16

    ; 按簇读loader
    mov bp,LOADER_SEG           ; loader文件内容读取到内存中的起始地址
read_loader:
    xor esi,esi                     ; esi清零
    mov si,bx                       ; 簇号
    add esi,SECTOR_CLUSTER_BALANCE
    mov di,bp
    call read_one_sector16
    add bp,512                      ; 下一个目标地址

    ; 获取下一个簇号（每个FAT表项为2字节）
    shl bx,1                        ; 乘2，每个FAT表项占2个字节
    mov bx,[bx+DISK_BUFFER]

    ;判断下一个簇号
    cmp bx,0xfff8       ; 大于等于0xfff8表示文件的最后一个簇
    jb read_loader      ; jb无符号小于则跳转，jl有符号小于则跳转。

read_loader_finish: ; 读取loader文件结束
    jmp LOADER_SEG  ; 跳转到loader在内存中的地址

loader_not_found: ; 没有找到loader文件。
    ; 打印字符串："Loader not found."
    mov si,loader_not_found_string
    mov di,80 ; 在屏幕第2行显示
    call print16

stop:
    hlt
    jmp stop

%include "util16.inc"


loader_file_name_string:db "LOADER  BIN",0          ; 文件名 11字节
boot_start_string:db "Chronix boot start.",0
loader_not_found_string:db "Loader not found.",0
loader_found_string:db "Loader found.",0

times 510-($-$$) db 0
db 0x55,0xaa
