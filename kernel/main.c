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

void test_int()
{
    terminal.print(&terminal, "Test function here!\n");
}

void chronix_main()
{
    keyboard_init();
    interrupt_unlock();     // 放在别的地方就无了

    // Print memory size
    uint32_t memsize = boot_param->memsize / 1024 / 1024;       // MB
    if (memsize != 0) {
        terminal.print(&terminal, "Memory size: ");
        terminal.print_int(&terminal, memsize, 10);
        terminal.print(&terminal, " MB");
        terminal.new_line(&terminal);
    } else {
        terminal.print(&terminal, "Can not calculate memory size.\n");
    }

    int * num_ptr = memman.malloc_phy(&memman, sizeof(int));
    terminal.print_int(&terminal, num_ptr, 16);
    terminal.new_line(&terminal);
    *num_ptr = 26214;

    terminal.show_head(&terminal);

    level0(_io_hlt);
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
        terminal.print(&terminal, " ****\n");
        if(error_no != 0) {
            terminal.print(&terminal, "**** Panic number:");
            terminal.print_int(&terminal, error_no, 16);
            terminal.print(&terminal, " ****\n");
        }
    }
    // 陷入死循环 不可操作
    while (1)
    {
        /* code */
    }
    
}
