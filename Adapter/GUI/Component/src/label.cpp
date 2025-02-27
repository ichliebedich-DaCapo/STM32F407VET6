//
// Created by fairy on 2025/1/2 16:41.
//

#include "label.hpp"


/**
 * @brief 初始化文本框
 * @param component    文本框对象指针
 * @param text  文本字符串
 * @param font  文本框字体
 */







/**
 * @brief 设置字体和字间距
 * @param font 字体格式
 * @param space 字间距，默认为0
 * @return
 */





/**
 * @brief 设置格式化文本内容
 * @param fmt 格式字符串
 * @param args 变长参数
 * @note 莫名其妙出现混账的乱码问题，感觉不出有什么错误，可确实显示不对，真是魂淡
 *      使用auto简化之后就没有任何问题了，只能说auto既能简化，还不用管这些乱七八糟的的事，真神
 *      使用前一定要设置好对象，该函数由于变长参数，无法在后面追加Obj参数,所以无法设置缺省.
 *          现在发现无法转义浮点数%f，这并非是这种格式的问题，而是lv_label_set_text_fmt本身就具备这个问题
 */




