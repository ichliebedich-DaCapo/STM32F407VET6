//
// Created by fairy on 2024/9/22.
//
#ifndef FURINA_APP_CONF_H
#define FURINA_APP_CONF_H
/**
 *  @brief JYZQの配置文件
 *  @note  主要是用于控制调试相关的宏，同时展现模块和驱动有哪些。这里的宏后缀都有"__"，而应用层宏后缀没有"__"
 *          之所以是两个"_"，是因为方便使用“Alt+J”快捷键多选，不会误删单个"_"
 *       这里集成了所有的模块和驱动的宏，而应用层的头文件里只有部分宏（需要哪部分宏就启用哪个）
 */

#include "app.hpp"

/**************************常用应用级*****************************/
#define USE_FSMC_DMA // 一般启用DMA
#define FreeRTOS_DEBUG  1  // 是否开启FreeRTOS调试模式。调试时，开了之后在负荷不大的情况下，也并不怎么影响实时性。发行时记得关闭

/****需要在app.hpp中定义宏，不过由于CMakelists检测功能限制，如果不开启该功能需要直接把宏删除而不是注释，然后再Cmake一下*****/
//#define FreeRTOS_ENABLE // 【默认关闭FreeRTOS】 因为一些项目比较简单且对实时性要求很高，使用FreeRTOS会增加开销
//#define GUI_DISABLE   // 【默认开启GUI】

/// 板级驱动




/****************************宏定义的频率****************************/
// 用于填入形参表为(uint32_t arr, uint32_t psc)的函数中，以方便修改频率。也可直接用设置频率的那个函数
#define FREQ_84M_to_50   1679,999     // 84MHz -> 50Hz
#define FREQ_84M_to_100  839,999    // 84MHz -> 100Hz
#define FREQ_84M_to_200  419,999    // 84MHz -> 200Hz
#define FREQ_84M_to_400  209,999    // 84MHz -> 400Hz
#define FREQ_84M_to_500  167,999    // 84MHz -> 500Hz
#define FREQ_84M_to_1K  83,999    // 84MHz -> 1KHz
#define FREQ_84M_to_2K   41,999   // 84MHz -> 2KHz
#define FREQ_84M_to_5K   167,99   // 84MHz -> 5KHz
#define FREQ_84M_to_8K   104,99  // 84MHz -> 8KHz
#define FREQ_84M_to_10K   83,99  // 84MHz -> 10KHz
#define FREQ_84M_to_12K   69,99  // 84MHz -> 12KHz
#define FREQ_84M_to_16K   104,49  // 84MHz -> 16KHz
#define FREQ_84M_to_20K   41,99  // 84MHz -> 20KHz

// 针对于音频的频率（默认采样点为256个）
#define FREQ_84M_to_256x8K  1,19  // 84MHz -> 256*8KHz
#define FREQ_84M_to_256x16K  1,9  // 84MHz -> 256*16KHz
#define FREQ_84M_to_128x8K 1,40 // 84MHz -> 128*8KHz
#define FREQ_84M_to_128x16K 1,19 // 84MHz -> 128*16KHz
#define FREQ_84M_to_256x800 40,9 // 84MHz -> 256*800Hz
#define FREQ_84M_to_256x1k  40,7 // 84MHz -> 256*1KHz



// ************************************************************ //

#define FREQ_168M_to_100  1679,999  // 168MHz -> 100Hz
#define FREQ_168M_to_500  335,999   // 168MHz -> 500Hz
#define FREQ_168M_to_1K  167,999   // 168MHz -> 1KHz
#define FREQ_168M_to_2K  83,999    // 168MHz -> 2KHz
#define FREQ_168M_to_5K  335,99    // 168MHz -> 5KHz
#define FREQ_168M_to_8K  209,99    // 168MHz -> 8KHz
#define FREQ_168M_to_10K  167,99    // 168MHz -> 10KHz
#define FREQ_168M_to_12K  139,99    // 168MHz -> 12KHz
#define FREQ_168M_to_16K  99,104   // 168MHz -> 16KHz
#define FREQ_168M_to_20K  83,99    // 168MHz -> 20KHz

// 针对于音频的频率（默认采样点为256个）
#define FREQ_168M_to_256x8K  2,19  // 168MHz -> 256*8KHz
#define FREQ_168M_to_256x16K  4,19  // 168MHz -> 256*16KHz

#define SECTION(name) __attribute__((section(name)))

/**
 * @brief  使用CCMRAM处的内存
 * @note   注意，CCMRAM定义的全局变量或静态局部变量不会在定义时初始化，其值随机。并且无法使用DMA
 *      使用示例：
 *      ①定义变量： CCMRAM int a;
 *                  a = 1234;
 *      ②定义函数(暂时没打算使用，因为没有修改链接脚本和启动文件，而且它也不适合保存函数代码)： CCMRAM void func(void);
 */
#define CCMRAM_VAR __attribute__((section(".ccmram")))


#endif //FURINA_APP_CONF_H
