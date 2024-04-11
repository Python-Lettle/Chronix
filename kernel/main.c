/**
 * Copyright (C) 2024 Lettle All rights reserved.
 * See the copyright notice in the file LICENSE.
 * Created by Lettle on 2024/3/31.
 * QQ: 1071445082
 * Email: 1071445082@qq.com
 * gitee: https://gitee.com/lettle/
 * github: https://github.com/python-lettle/
 * bilibili: https://space.bilibili.com/420393625
 */

#include <stdlib.h>
#include "global.h"
#include "prototype.h"

void chronix_main(void)      // 2001c
{

    // Print memory size
    if (boot_param->memsize != 0) {
        int memsize = boot_param->memsize / 1024;       // KB
        char memsize_str[10] = {0};
        terminal.print(&terminal, "OS memory size(KB): ");
        terminal.print(&terminal, itoa(memsize, memsize_str, 10));
        terminal.new_line(&terminal);
    } else {
        terminal.print(&terminal, "Can not calculate memory size.\n");
    }
    keyboard_init();
    interrupt_unlock();     // 放在别的地方就无了

    terminal.print(&terminal, "root@Chronix$ ");

    // 不能让内核陷入hlt 不然键盘中断就无了
    int a=0;
    while (1)
    {
        a++;
    }
    
}

/**
 *	内核遇到了不可恢复的异常或错误，立即准备宕机
 */
void panic(const char* msg, int error_no)
{
    /* 有错误消息的话，请先打印 */
    if(msg != 0) {
        terminal.print(&terminal, "\n**** Chronix panic:");
        terminal.print(&terminal, msg);
        terminal.print(&terminal, " ****");
        terminal.new_line(&terminal);
        
        if(error_no != 0) {
            terminal.print(&terminal, "**** Panic number:");
            char err_no_str[10] = {0};
            itoa(error_no, err_no_str, 10);
            terminal.print(&terminal, error_no);
            terminal.print(&terminal, " ****");
            terminal.new_line(&terminal);
        }
    }
    while(1) {_io_hlt();}
}
