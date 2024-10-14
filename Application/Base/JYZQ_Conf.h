//
// Created by fairy on 2024/9/22.
//
#ifndef FURINA_JYZQ_CONF_H
#define FURINA_JYZQ_CONF_H

/**************************调试/其他*************************/
// 是否开启FreeRTOS调试模式
// 调试时，开了之后在负荷不大的情况下，也并不怎么影响实时性。发行时记得关闭
#define FreeRTOS_DEBUG  1
#define ENABLE_KEY_TASK_HANDLE 1

// 是否开启FreeRTOS,由项目里的应用头文件来控制
// 考虑到有一些应用项目并不需要多线程，且对实时性要求很高，使用FreeRTOS既会增加代码体积，也会增加运行时开销
/*#define APP_NO_RTOS*/ // 【这个宏默认是注释的】

/************************应用级***************************/
#define ENABLE_VOICE_STORAGE_AND_PLAY 0
#define ENABLE_SIGNAL_GENERATOR 1   // 信号发生器（简单的双音频）


/*************************模块级**************************/
#define USE_PLAYER 0 // 使用播放器
#define USE_FLASH_STORAGE 0// 使用Flash存储
#define USE_WAVE_SIGNAL 1 // 使用波形信号
#define USE_CPU_TEMPERATURE 1 // 使用CPU测温计

/**************************板级**************************/
// 协议控制器放在上面，具体外设放在下面
#define USE_SPI 1 // 使用SPI
#define USE_FSMC 1  // 使用FSMC
#define USE_LCD 1   // 使用LCD
#define USE_KEY_EXTI  1// 使用按键的外部中断
#define USE_SPI_FLASH 1 // 使用SPI Flash
#define USE_TIMER 1// 使用定时器
#define USE_ADC 1 // 使用ADC
#define USE_DAC 1 // 使用DAC


/****************************宏定义的频率****************************/
// 用于填入形参表为(uint32_t arr, uint32_t psc)的函数中，以方便修改频率
#define FREQ_84M_to_100  839,999    // 84MHz -> 100Hz
#define FREQ_84M_to_500  167,999    // 84MHz -> 500Hz
#define FREQ_84M_to_1K  83,999    // 84MHz -> 1KHz
#define FREQ_84M_to_2K   41,999   // 84MHz -> 2KHz
#define FREQ_84M_to_5K   167,99   // 84MHz -> 5KHz
# define FREQ_84M_to_8K   104,99  // 84MHz -> 8KHz
#define FREQ_84M_to_10K   83,99  // 84MHz -> 10KHz
#define FREQ_84M_to_12K   69,99  // 84MHz -> 12KHz
#define FREQ_84M_to_16K   49,104  // 84MHz -> 16KHz
#define FREQ_84M_to_20K   41,99  // 84MHz -> 20KHz

// 针对于音频的频率（默认采样点为256个）
# define FREQ_84M_to_256x8K  1,39   // 84MHz -> 256*8KHz
# define FREQ_84M_to_256x16K  2,19  // 84MHz -> 256*16KHz

// ************************************************************ //

#define FREQ_168M_to_100  1679,999  // 168MHz -> 100Hz
#define FREQ_168M_to_500  335,999   // 168MHz -> 500Hz
# define FREQ_168M_to_1K  167,999   // 168MHz -> 1KHz
# define FREQ_168M_to_2K  83,999    // 168MHz -> 2KHz
# define FREQ_168M_to_5K  335,99    // 168MHz -> 5KHz
# define FREQ_168M_to_8K  209,99    // 168MHz -> 8KHz
# define FREQ_168M_to_10K  167,99    // 168MHz -> 10KHz
# define FREQ_168M_to_12K  139,99    // 168MHz -> 12KHz
# define FREQ_168M_to_16K  99,104   // 168MHz -> 16KHz
# define FREQ_168M_to_20K  83,99    // 168MHz -> 20KHz

// 针对于音频的频率（默认采样点为256个）
# define FREQ_168M_to_256x8K  2,19  // 168MHz -> 256*8KHz
# define FREQ_168M_to_256x16K  4,19  // 168MHz -> 256*16KHz


#endif //FURINA_JYZQ_CONF_H
