//
// Created by fairy on 2024/9/22.
//
#ifndef FURINA_JYZQ_CONF_H
#define FURINA_JYZQ_CONF_H

#include "App_Conf.h"
/**
 *  @brief JYZQの配置文件
 *  @note  主要是用于控制调试相关的宏，同时展现模块和驱动有哪些。这里的宏后缀都有"__"，而应用层宏后缀没有"__"
 *          之所以是两个"_"，是因为方便使用“Alt+J”快捷键多选，不会误删单个"_"
 *       这里集成了所有的模块和驱动的宏，而应用层的头文件里只有部分宏（需要哪部分宏就启用哪个）
 */

// 混蛋的依赖关系
#if ENABLE_VOICE_STORAGE_AND_PLAY

#include "voiceStorageAndPlay.hpp"

#endif// 语音存储和播放
#if ENABLE_SIGNAL_GENERATOR
#include "signalGenerator.hpp"
#endif// 双音频信号发生器


/**************************调试/其他*************************/
// 是否开启FreeRTOS调试模式。调试时，开了之后在负荷不大的情况下，也并不怎么影响实时性。发行时记得关闭
#define FreeRTOS_DEBUG  1


// 是否开启FreeRTOS,由项目头文件的宏来控制。因为一些项目比较简单且对实时性要求很高，使用FreeRTOS会增加开销
/*#define APP_NO_RTOS*/ // 【这个宏默认是注释的】

/************************基本上都使用的驱动*************************/
#define USE_FSMC   // 使用FSMC
//#define USE_FSMC_DMA // 暂时出现了不可描述的错误，进入看门狗了
#define USE_LCD    // 使用LCD
#define USE_KEY_EXTI  // 使用按键的外部中断

/*************************模块级**************************/
#define USE_PLAYER__  // 使用播放器
#define USE_FLASH_STORAGE__ // 使用Flash存储
#define USE_WAVE_SIGNAL__ // 使用波形信号

/**************************板级**************************/
#define USE_SPI__  // 使用SPI
#define USE_SPI_FLASH__  // 使用SPI Flash
#define USE_TIMER__ // 使用定时器
#define USE_ADC__  // 使用ADC
#define USE_DAC__ // 使用DAC



/****************************宏定义的频率****************************/
// 用于填入形参表为(uint32_t arr, uint32_t psc)的函数中，以方便修改频率
#define FREQ_84M_to_100  839,999    // 84MHz -> 100Hz
#define FREQ_84M_to_500  167,999    // 84MHz -> 500Hz
#define FREQ_84M_to_1K  83,999    // 84MHz -> 1KHz
#define FREQ_84M_to_2K   41,999   // 84MHz -> 2KHz
#define FREQ_84M_to_5K   167,99   // 84MHz -> 5KHz
#define FREQ_84M_to_8K   104,99  // 84MHz -> 8KHz
#define FREQ_84M_to_10K   83,99  // 84MHz -> 10KHz
#define FREQ_84M_to_12K   69,99  // 84MHz -> 12KHz
#define FREQ_84M_to_16K   49,104  // 84MHz -> 16KHz
#define FREQ_84M_to_20K   41,99  // 84MHz -> 20KHz

// 针对于音频的频率（默认采样点为256个）
#define FREQ_84M_to_256x8K  1,39   // 84MHz -> 256*8KHz
#define FREQ_84M_to_256x16K  2,19  // 84MHz -> 256*16KHz

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


#endif //FURINA_JYZQ_CONF_H
