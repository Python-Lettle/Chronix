; 打印字符串函数。
; 输入参数：ds:si，di。
; 输出参数：无。
; ds:si 表示字符串起始地址，以0为结束符。
; di 表示字符串在屏幕上显示的起始位置（0~1999）。
print16:
    mov ah,0xf ; ah表示字符属性，0x07表示黑底白字。
    shl di,1    ; 乘2（屏幕上每个字符对应2个显存字节）。
    .start_char:
    mov al,[si]
    cmp al,0
    jz .end_print
    mov [gs:di],ax ; 将字符和属性放到对应的显存中。
    inc si
    add di,2
    jmp .start_char
    .end_print:
    ret

; 读取硬盘1个扇区（主硬盘控制器主盘）
; 输入参数：esi，ds:di。
; esi LBA扇区号
; ds:di 将数据写入到的内存起始地址
; 输出参数：无。
read_one_sector16:
    ; 第1步：检查硬盘控制器状态
    mov dx,0x1f7
    .not_ready1:
    nop
    in al,dx        ; 读0x1f7端口
    and al,0xc0     ; 第7位为1表示硬盘忙，第6位为1表示硬盘控制器已准备好，正在等待指令。
    cmp al,0x40     ; 当第7位为0，且第6位为1，则进入下一个步。
    jne .not_ready1 ; 若未准备好，则继续判断。
    ; 第2步：设置要读取的扇区数
    mov dx,0x1f2
    mov al,1
    out dx,al ;读取1个扇区
    ; 第3步：将LBA地址存入0x1f3~0x1f6
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
    ; 第4步：设置device端口
    shr eax,8
    and al,0x0f     ; LBA第24~27位
    or al,0xe0      ; 设置7~4位为1110，表示LBA模式，主盘
    mov dx,0x1f6
    out dx,al
    ; 第5步：向0x1f7端口写入读命令0x20
    mov dx,0x1f7
    mov al,0x20
    out dx,al
    ; 第6步：检测硬盘状态
    .not_ready2:
    nop
    in al,dx        ; 读0x1f7端口
    and al,0x88     ; 第7位为1表示硬盘忙，第3位为1表示硬盘控制器已准备好数据传输。
    cmp al,0x08     ; 当第7位为0，且第3位为1，进入下一步。
    jne .not_ready2 ; 若未准备好，则继续判断。
    ; 第7步：从0x1f0端口读数据
    mov cx,256      ; 每次读取2字节，一个扇区需要读256次。
    mov dx,0x1f0
    .go_on_read:
    in ax,dx
    mov [di],ax
    add di,2
    loop .go_on_read
    ret

