;============================================================================
;   打印函数，它类似与C语言中的printf，但它不支持'%'可变参数
; 函数原型：Print(void* ds:ptr)，ptr指向要打印的字符串，字符串以0结尾
;----------------------------------------------------------------------------
Print:
    push esi
    push edi

    mov esi, [esp + 4 * 3]      ; 得到字符串地址
    mov edi, [ddDispPosition]   ; 得到显示位置
    mov ah, 0xf                 ; 黑底白字
.1:
    lodsb                       ; ds:esi -> al, esi++
    test al, al
    jz .PrintEnd                ; 遇到了0，结束打印
    cmp al, 10
    je .2
    ; 如果不是0，也不是'\n'，那么我们认为它是一个可打印的普通字符
    mov [gs:edi], ax
    add edi, 2                  ; 指向下一列
    jmp .1
.2: ; 处理换行符'\n'
    push eax
    mov eax, edi                ; eax = 显示位置
    mov bl, 160
    div bl                      ; 显示位置 / 160，商eax就是当前所在行数
    inc eax                     ; 行数++
    mov bl, 160
    mul bl                      ; 行数 * 160，得出这行的显示位置
    mov edi, eax                ; edi = 新的显示位置
    pop eax
    jmp .1
.PrintEnd:
    mov dword [ddDispPosition], edi ; 打印完毕，更新显示位置

    pop edi
    pop esi
    ret
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
