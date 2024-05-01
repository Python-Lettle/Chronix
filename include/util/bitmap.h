/**
 * Copyright (C) 2024 Lettle All rights reserved.
 * See the copyright notice in the file LICENSE.
 * Created by Lettle on 2024/4/27.
 * QQ: 1071445082
 * Email: 1071445082@qq.com
 * gitee: https://gitee.com/lettle/
 * github: https://github.com/python-lettle/
 * bilibili: https://space.bilibili.com/420393625
 * 
 * Bitmap 解释：
 * 
 * 假设array是 uint8_t 类型的:
 *   array[0]: 0  0  0  0  0  0  0  0    [1]: 0  0  0  0  0  0   0  0
 *   index   : 7  6  5  4  3  2  1  0         15 14 13 12 11 10  9  8     
 * 此时array数字为 [0,0]
 *    
 * 若设第5位为1,则:
 *   array[0]: 0  0  1  0  0  0  0  0    [1]: 0  0  0  0  0  0   0  0
 *   index   : 7  6  5  4  3  2  1  0         15 14 13 12 11 10  9  8
 * 此时array数字为 [32,0]
 * 
 * 如果一个单元是全1的，那么这个单元数值为-1
 */

#ifndef CHRONIX_BITMAP_H
#define CHRONIX_BITMAP_H

#include <type.h>

#define	GET_BIT(x, bit)	((x & (1 << bit)) >> bit)	/* 获取第bit位 */
#define	CLEAR_BIT(x, bit)	(x = x & (~(1 << bit)))	/* 清零第bit位 */

/**
 * @brief bitmap 结构体，包含array数组大小和一共可以管理多少个bit
 */
typedef struct bit_map {
  uint32_t* array;
  int array_size;           // array数组大小
  int total_bits;           // 可管理bit数
} bitmap_t;

/**
 * @brief 初始化bitmap，设置bitmap数据指针，计算属性
 */
void bitmap_init(bitmap_t* self, uint32_t* array, int total_bits);

bool bitmap_get_bit(bitmap_t* self, int index);

/**
 * @brief 设置 index 位为 value
 * @param index int类型 该位在位图的哪一个位置
 * @param value 将 index 位置0
 */
void bitmap_set_bit_0(bitmap_t* self, int index);

/**
 * @brief 设置 index 位为 value
 * @param index int类型 该位在位图的哪一个位置
 * @param value 将 index 位置1
 */
void bitmap_set_bit_1(bitmap_t* self, int index);

/**
 * @brief 寻找 bitmap 中第一个0的位置
 * @param from 从这个位置开始查找
 */
int bitmap_first_0_from (bitmap_t* self, int from);
int bitmap_first_0 (bitmap_t* self);

/**
 * @brief 寻找 bitmap 中连续的n个0的位置
 * @param n 寻找n个0
 * @param from 从这个位置开始查找
 */
int bitmap_first_n_0_from (bitmap_t* self, int n, int from);
int bitmap_first_n_0 (bitmap_t* self, int n);



#endif