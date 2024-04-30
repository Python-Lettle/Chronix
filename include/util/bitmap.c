/**
 * Copyright (C) 2024 Lettle All rights reserved.
 * See the copyright notice in the file LICENSE.
 * Created by Lettle on 2024/4/27.
 * QQ: 1071445082
 * Email: 1071445082@qq.com
 * gitee: https://gitee.com/lettle/
 * github: https://github.com/python-lettle/
 * bilibili: https://space.bilibili.com/420393625
 */

#include "bitmap.h"

void bitmap_init(bitmap_t* self, uint32_t* array, int total_bits)
{
    self->total_bits = total_bits;
    self->array_size = total_bits / 32;
    self->array = array;
}

bool bitmap_get_bit(bitmap_t* self, int index)
{
    return GET_BIT(self->array[index / 32], index % 32);
}

void bitmap_set_bit_0(bitmap_t* self, int index)
{
    int array_index = index / 32;
    int bit_offset = index % 32;
    CLEAR_BIT(self->array[array_index], bit_offset);
}

void bitmap_set_bit_1(bitmap_t* self, int index)
{
    int array_index = index / 32;
    int bit_offset = index % 32;
    self->array[array_index] |= (1 << bit_offset);
}

int bitmap_first_0 (bitmap_t* self)
{
    for (int i = 0; i < self->array_size; i++) {
        if (self->array[i] == -1) continue;
        for (int j = 0; j < 32; j++) {
            if ((self->array[i] & (1 << j)) == 0) {
                return i * 32 + j;
            }
        }
    }
}

int bitmap_first_0_from (bitmap_t* self, int from)
{
    for (int i = from / 32; i < self->array_size; i++) {
        if (self->array[i] == -1) {
            from = 0;
            continue;
        }
        for (int j = from % 32; j < 32; j++) {
            if ((self->array[i] & (1 << j)) == 0) {
                return i * 32 + j;
            }
        }
    }
}

int bitmap_first_n_0 (bitmap_t* self, int n)
{
    if (n <= 0) return -1;

    int count = 0, i,j;
    for (i = 0; i < self->array_size; i++) {
        if (self->array[i] == -1) continue;
        for (j = 0; j < 32; j++) {
            if ((self->array[i] & (1 << j)) == 0) {
                count++;
                if (count == n) return i * 32 + j - count + 1;
            } else {
                count = 0;
            }
        }
    }

    if (count < n) return -1;
}

int bitmap_first_n_0_from (bitmap_t* self, int n, int from)
{
    int count = 0, i,j;
    for (i = from / 32; i < self->array_size; i++) {
        if (self->array[i] == -1) {
            from = 0;
            continue;
        }
        for (j = from % 32; j < 32; j++) {
            if ((self->array[i] & (1 << j)) == 0) {
                count++;
                if (count == n) return i * 32 + j - count + 1;
            } else {
                count = 0;
            }
        }
    }
    if (count < n) return -1;
}