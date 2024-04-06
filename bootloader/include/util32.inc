

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
.prt_1:
    lodsb                       ; ds:esi -> al, esi++
    test al, al
    jz .PrintEnd                ; 遇到了0，结束打印
    cmp al, 10
    je .prt_2
    ; 如果不是0，也不是'\n'，那么我们认为它是一个可打印的普通字符
    mov [gs:edi], ax
    add edi, 2                  ; 指向下一列
    jmp .prt_1
.prt_2: ; 处理换行符'\n'
    push eax
    mov eax, edi                ; eax = 显示位置
    mov bl, 160
    sub eax, 0xb8000            ; Fix1: 先减去0xb8000
    div bl                      ; 显示位置 / 160，商eax就是当前所在行数
    inc eax                     ; 行数++
    mov bl, 160
    mul bl                      ; 行数 * 160，得出这行的显示位置
    add eax, 0xb8000            ; Fix2: 再加上0xb8000
    mov edi, eax                ; edi = 新的显示位置
    pop eax
    jmp .prt_1
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

;============================================================================
; 根据16位时计算的ARDS，计算内存大小
;----------------------------------------------------------------------------
CalMemSize:
    push esi
    push ecx
    push edx
    push edi

    mov esi, _MemCheckBuffer    ; ds:esi -> 缓冲区
    mov ecx, [_ddMCRCount]      ; ecx = ARDS的个数
.calmem_loop:
    mov edx, 5                  ; ARDS有5个成员变量
    mov edi, _ARDS
.calmem_1: ; 将缓冲区中的第 i 个ARDS结构拷贝到ds:edi中的ARDS结构
    push dword [esi]
    pop eax                 ; ds:eax -> 缓冲区中的第一个ADRS结构
    stosd                   ; 将ds:eax中的一个dword内容拷贝到ds:edi中，填充ADRS结构
    add esi, 4              ; ds:esi指向ARDS中的下一个成员变量
    dec edx                 ; j--
    cmp edx, 0
    jnz .calmem_1                  ; j != 0，继续填充
    ; j == 0，ARDS结构填充完毕
    cmp dword [_ddType], 1
    jne .calmem_2                  ; 不是OS可使用的内存范围，直接进入下个外循环看下一个ARDS
    ; 是OS可用的地址范围，我们计算这个ARDS的内存大小
    mov eax, [_ddBaseAddrLow]; eax = 基地址低32位
    add eax, [_ddLengthLow]  ; eax = 基地址低32位 + 长度低32位 --> 这个ARDS结构的指代的内存大小
                            ; 为什么不算高32为？因为32位既可以表示0~4G的内存范围，而32位CPU也只能识别0~4G
                            ; 我们编写的是32位操作系统，所以高32位是为64位操作系统做准备的，我们不需要。
    cmp eax, [_ddMemSize]
    jb .calmem_2
    mov dword [_ddMemSize], eax  ; 内存大小 = 最后一个基地址最大的ARDS的  基地址低32位 + 长度低32位
.calmem_2:
    loop .calmem_loop              ; jmp .loop, ecx--

    pop edi
    pop edx
    pop ecx
    pop esi
    ret

;============================================================================
;   打印内存大小(以KB显示)
;----------------------------------------------------------------------------
PrintMemSize:
    push ebx
    push ecx

    mov eax, [_ddMemSize]    ; eax = 内存大小
    xor edx, edx
    mov ebx, 1024
    div ebx                 ; eax / 1024 --> 内存大小(字节) / 1024 = 内存大小(KB)

    push eax                ; 保存计算好的内存大小
    ; 显示一个字符串"Memory Size: "
    push _strMemSize
    call Print
    add esp, 4

    ; 因为之前已经压入eax了，不需要再压入！
    call PrintInt
    add esp, 4

    ; 打印"KB"
    push _strKB
    call Print
    add esp, 4

    pop ecx
    pop ebx
    ret

;============================================================================
;   显示一个整形数
;----------------------------------------------------------------------------
PrintInt:
    mov	ah, 0Fh			; 0000b: 黑底    1111b: 白字
    mov	al, '0'
    push	edi
    mov	edi, [ddDispPosition]
    mov	[gs:edi], ax
    add edi, 2
    mov	al, 'x'
    mov	[gs:edi], ax
    add	edi, 2
    mov	[ddDispPosition], edi	; 显示完毕后，设置新的显示位置
    pop edi

	mov	eax, [esp + 4]
	shr	eax, 24
	call	PrintAl

	mov	eax, [esp + 4]
	shr	eax, 16
	call	PrintAl

	mov	eax, [esp + 4]
	shr	eax, 8
	call	PrintAl

	mov	eax, [esp + 4]
	call	PrintAl

	ret

;============================================================================
;   显示 AL 中的数字
;----------------------------------------------------------------------------
PrintAl:
	push ecx
	push edx
	push edi
	push eax

	mov edi, [ddDispPosition]	; 得到显示位置

	mov ah, 0Fh		; 0000b: 黑底	1111b: 白字
	mov dl, al
	shr al, 4
	mov ecx, 2
.prtal_begin:
	and al, 01111b
	cmp al, 9
	ja	.prtal_1
	add al, '0'
	jmp	.prtal_2
.prtal_1:
	sub al, 10
	add al, 'A'
.prtal_2:
	mov [gs:edi], ax
	add edi, 2

	mov al, dl
	loop .prtal_begin

	mov [ddDispPosition], edi	; 显示完毕后，设置新的显示位置

    pop eax
	pop edi
	pop edx
	pop ecx

	ret