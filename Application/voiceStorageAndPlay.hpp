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
#ifndef FURINA_VOICESTORAGEANDPLAY_HPP
#define FURINA_VOICESTORAGEANDPLAY_HPP
#include "App_Conf.h"
#if ENABLE_VOICE_STORAGE_AND_PLAY
// 不启用FreeRTOS
#define APP_NO_RTOS

/*************************模块级**************************/
#define USE_PLAYER  // 使用播放器
#define USE_FLASH_STORAGE // 使用Flash存储

/**************************板级**************************/
#define USE_SPI  // 使用SPI
#define USE_SPI_FLASH  // 使用SPI Flash
#define USE_TIMER // 使用定时器
#define USE_ADC  // 使用ADC
#define USE_DAC // 使用DAC
#define USE_FFT //使用FFT


/**************************其他**************************/
// 使用软件CS控制



#endif
#endif //FURINA_VOICESTORAGEANDPLAY_HPP
