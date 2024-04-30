/**
 * Copyright (C) 2024 Lettle All rights reserved.
 * See the copyright notice in the file LICENSE.
 * Created by Lettle on 2024/4/5.
 * QQ: 1071445082
 * Email: 1071445082@qq.com
 * gitee: https://gitee.com/lettle/
 * github: https://github.com/python-lettle/
 * bilibili: https://space.bilibili.com/420393625
 * 
 * 这里是公共的type定义
 */

#ifndef TYPE_H
#define TYPE_H

// 常用数据类型定义
typedef int size_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef char bool;

#define TRUE 1
#define FALSE 0

#define	GET_BIT(x, bit)	((x & (1 << bit)) >> bit)	/* 获取第bit位 */
#define	CLEAR_BIT(x, bit)	(x &= ~(1 << bit))	/* 清零第bit位 */

/**
 * @brief 判断一个char是否表示一个数字
 * 
 * @param c 
 * @return true 
 * @return false 
 */
bool is_digit(char c);

/**
 * @brief 判断一个char是否表示一个字母，大小写均可
 */
bool is_alpha(char c);
/**
 * @brief 判断一个char是否表示一个小写字母
 */
bool is_lower(char c);
/**
 * @brief 判断一个char是否表示一个大写字母
 */
bool is_upper(char c);
/**
 * @brief 判断一个char是否表示一个空格
 */
bool is_space(char c);
/**
 * @brief 判断一个char是否表示一个数字或字母
 */
bool is_alpha_digit(char c);
/**
 * @brief 判断一个char是否表示一个符号
 */
bool is_sign(char c);

#endif