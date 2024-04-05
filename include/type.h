#ifndef CHRONIX_TYPE_H
#define CHRONIX_TYPE_H

typedef int size_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef char bool;

#define TRUE 1
#define FALSE 0

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