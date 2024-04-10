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

#ifndef CHRONIX_EXCEPTION_H
#define CHRONIX_EXCEPTION_H

/* kernel.asm 提供的异常处理入口 */
void divide_error(void);
void single_step_exception(void);
void nmi(void);
void breakpoint_exception(void);
void overflow(void);
void bounds_check(void);
void inval_opcode(void);
void copr_not_available(void);
void double_fault(void);
void inval_tss(void);
void copr_not_available(void);
void segment_not_present(void);
void stack_exception(void);
void general_protection(void);
void page_fault(void);
void copr_seg_overrun(void);
void copr_error(void);
void divide_error(void);

#endif