[section .lib]
; 导出函数
global strcpy
global memcpy

;================================================================================================
; char* strcpy(char* p_dst, char* p_src);
strcpy:
    push ebp
	mov ebp, esp

    mov edi, [ebp + 8]			; edi -> 目的地地址
	mov esi, [ebp + 12]			; esi -> 源地址（要复制的数据首地址）
.1:
	mov al, [esi]				; @
	inc esi						; #
								; #-> 逐字节移动
	mov byte [edi], al			; #
	inc edi						; @

	cmp al, 0					; 是否遇到 '\0'
	jnz .1						; 没遇到就进行循环，遇到说明移动完成

	mov eax, [ebp + 8]			; 返回值
	
	pop ebp
	ret

;================================================================================================
; void* memcpy(void* es:p_dst, void* ds:p_src, int size);
memcpy:
    push ebp
    mov ebp, esp

    push esi
    push edi
    push ecx

    mov edi, [ebp + 8]  ; edi -> 目的地地址
    mov esi, [ebp + 12]	; esi -> 源地址（要复制的数据首地址）
	mov ecx, [ebp + 16]	; ecx <- 计数器（复制的数据大小）
.1:
	cmp ecx, 0			; 判断计数器
	jz	.2				; if(计数器器 == 0); jmp .2

	mov al, [ds:esi]		; @
	inc esi					; #
							; #-> 逐字节移动并复制
	mov byte [es:edi], al	; #
	inc edi					; @

	dec ecx				; 计数器--
	jmp	.1				; 循环
.2:
	mov eax, [ebp + 8]	; 设置返回值

	pop ecx
	pop edi
	pop esi
	mov esp, ebp
	pop ebp

	ret