//
// Created by 34753 on 2024/9/22.
//
/**
 * @brief 语音存储和回放
 * @details 可以存储和播放录音，同时添加了变速、快进、显示频谱功能
 * 播放：通过启停按键keyk0控制
 * 录音：通过启停按键keyk1控制
 * 速度：通过按键keyk2和keyk3控制,倍速为0.75和1.5
 * @note
 * 播放和录音通过读写FLASH外设M25P16开实现，为了统一，读写都使用了双缓冲机制。
 * 频谱分析使用的是CMSIS的DSP库里的FFT，独自开辟了一个缓冲区
 */
// 下面是使用到的模块和驱动
//#define USE_PLAYER 1 // 使用播放器
//#define USE_FLASH_STORAGE 1// 使用Flash存储
//#define USE_SPI 1 // 使用SPI
//#define USE_FSMC 1  // 使用FSMC
//#define USE_LCD 1   // 使用LCD
//#define USE_KEY_EXTI  1// 使用按键的外部中断
//#define USE_SPI_FLASH 1 // 使用SPI Flash
//#define USE_TIMER 1// 使用定时器
//#define USE_ADC 1 // 使用ADC
//#define USE_DAC 1 // 使用DAC

#ifndef FURINA_VOICESTORAGEANDPLAY_HPP
#define FURINA_VOICESTORAGEANDPLAY_HPP
#include "JYZQ_Conf.h"
#if ENABLE_VOICE_STORAGE_AND_PLAY
// 不启用FreeRTOS
#define APP_NO_RTOS

#endif
#endif //FURINA_VOICESTORAGEANDPLAY_HPP