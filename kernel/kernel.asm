;============================================================================
;   导入和导出
;----------------------------------------------------------------------------
; 导入函数
extern chronix_main                 ; 内核主函数 --- main.c
extern kernel_init                  ; 初始化函数 --- init.c
extern exception_handler            ; 异常统一处理例程

; 导入变量
extern gdt_ptr                      ; protect.c / global.h
extern idt_ptr                      ; protect.c / global.h

; 导出函数
global _start                       ; 导出_start程序开始符号，链接器需要它
global _io_hlt

; 异常处理
global divide_error
global single_step_exception
global nmi
global breakpoint_exception
global overflow
global bounds_check
global inval_opcode
global copr_not_available
global double_fault
global copr_seg_overrun
global inval_tss
global segment_not_present
global stack_exception
global general_protection
global page_fault
global copr_error

; 以下选择子值必须与 portect.h 中保持一致！
SELECTOR_TEXT       equ 8                   ; LOADER 里面已经确定了的
SELECTOR_DATA       equ 16                  ; LOADER 里面已经确定了的，可读写数据段
SELECTOR_TSS        equ 32                  ; TSS. 从外层调到内存时 SS 和 ESP 的值从里面获得

SELECTOR_KERNEL_CS  equ SELECTOR_TEXT       ; 内核代码段
SELECTOR_KERNEL_DS  equ SELECTOR_DATA       ; 内核数据段

;============================================================================
;   内核堆栈段
;----------------------------------------------------------------------------
[section .bss]
StackSpace:     resb 4 * 1024       ; 4KB栈空间
StackTop:
;============================================================================
;   内核代码段
;----------------------------------------------------------------------------
[section .text]
_start:     ; 内核程序入口
    ; 注意! 在使用 C 代码的时候一定要保证 ds, es, ss 这几个段寄存器的值是一样的
    ; 因为编译器有可能编译出使用它们的代码, 而编译器默认它们是一样的. 比如串拷贝操作会用到 ds 和 es。
    ; 寄存器复位
    mov ax, ds
    mov es, ax
    mov fs, ax
    mov ss, ax              ; es = fs = ss = 内核数据段
    mov esp, StackTop       ; 栈顶

    sgdt [gdt_ptr]
    ; 进入主函数之前先进行初始化
    call kernel_init
    lgdt [gdt_ptr]
	lidt [idt_ptr]
	; 一个跳转指令，让以上设置及时生效
    jmp csinit

csinit:
	; 加载任务状态段 TSS
    xor eax, eax
    mov ax, SELECTOR_TSS
    ltr ax

    ; 跳入C语言编写的主函数
    jmp chronix_main
    
_io_hlt:
    hlt
    jmp _io_hlt

;============================================================================
;   异常处理
;----------------------------------------------------------------------------
divide_error:
	push	0xffffffff	; 没有错误代码，用0xffffffff表示
	push	0		    ; 中断向量号	= 0
	jmp	exception
single_step_exception:
	push	0xffffffff	; 没有错误代码，用0xffffffff表示
	push	1		    ; 中断向量号	= 1
	jmp	exception
nmi:
	push	0xffffffff	; 没有错误代码，用0xffffffff表示
	push	2		    ; 中断向量号	= 2
	jmp	exception
breakpoint_exception:
	push	0xffffffff	; 没有错误代码，用0xffffffff表示
	push	3		    ; 中断向量号	= 3
	jmp	exception
overflow:
	push	0xffffffff	; 没有错误代码，用0xffffffff表示
	push	4		    ; 中断向量号	= 4
	jmp	exception
bounds_check:
	push	0xffffffff	; 没有错误代码，用0xffffffff表示
	push	5		    ; 中断向量号	= 5
	jmp	exception
inval_opcode:
	push	0xffffffff	; 没有错误代码，用0xffffffff表示
	push	6		    ; 中断向量号	= 6
	jmp	exception
copr_not_available:
	push	0xffffffff	; 没有错误代码，用0xffffffff表示
	push	7		    ; 中断向量号	= 7
	jmp	exception
double_fault:
	push	8		    ; 中断向量号	= 8
	jmp	exception
copr_seg_overrun:
	push	0xffffffff	; 没有错误代码，用0xffffffff表示
	push	9		    ; 中断向量号	= 9
	jmp	exception
inval_tss:
	push	10		    ; 中断向量号	= 10
	jmp	exception
segment_not_present:
	push	11		    ; 中断向量号	= 11
	jmp	exception
stack_exception:
	push	12		    ; 中断向量号	= 12
	jmp	exception
general_protection:
	push	13		    ; 中断向量号	= 13
	jmp	exception
page_fault:
	push	14		    ; 中断向量号	= 14
	jmp	exception
copr_error:
	push	0xffffffff	; 没有错误代码，用0xffffffff表示
	push	16		    ; 中断向量号	= 16
	jmp	exception

exception:
	call	exception_handler
	add	esp, 4 * 2	    ; 让栈顶指向 EIP，堆栈中从顶向下依次是：EIP、CS、EFLAGS
.down:
	hlt                 ; CPU停止运转，宕机
    jmp .down
