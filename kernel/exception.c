/**
 * Copyright (C) 2024 Lettle All rights reserved.
 * See the copyright notice in the file LICENSE.
 * Created by Lettle on 2024/4/10.
 * QQ: 1071445082
 * Email: 1071445082@qq.com
 * gitee: https://gitee.com/lettle/
 * github: https://github.com/python-lettle/
 * bilibili: https://space.bilibili.com/420393625
 */

#include "kernel.h"
#include "global.h"
#include <stdlib.h>

/* 异常信息表 */
char* exception_table[] = {
        "#DE Divide Error", 
        "#DB RESERVED", 
        "—  NMI Interrupt", 
        "#BP Breakpoint", 
        "#OF Overflow", 
        "#BR BOUND Range Exceeded", 
        "#UD Invalid Opcode (Undefined Opcode)", 
        "#NM Device Not Available (No Math Coprocessor)", 
        "#DF Double Fault", 
        "    Coprocessor Segment Overrun (reserved)", 
        "#TS Invalid TSS", 
        "#NP Segment Not Present", 
        "#SS Stack-Segment Fault", 
        "#GP General Protection", 
        "#PF Page Fault", 
        "—  (Intel reserved. Do not use.)", 
        "#MF x87 FPU Floating-Point Error (Math Fault)", 
        "#AC Alignment Check", 
        "#MC Machine Check", 
        "#XF SIMD Floating-Point Exception", 
};

void exception_handler(int int_vector, int err_num)
{
    // 非屏蔽中断，忽略
    if (err_num == 2) 
    {
        terminal.print(&terminal, exception_table[int_vector]);
        terminal.print(&terminal, "\n");
        return;
    }

    // 内核发生异常，宕机
    if(exception_table[int_vector] == 0){
        panic("not in table exception", 666);
    } else {
        panic(exception_table[int_vector], err_num);
    }
}