//
// Created by DV on 2024/12/9.
//

#ifndef FURINA_APP_HPP
#define FURINA_APP_HPP

// 不启用FreeRTOS
#define APP_NO_RTOS

/*************************模块级**************************/
//#define USE_FLASH_STORAGE // 使用Flash存储

/**************************板级**************************/
#define USE_SPI_FLASH  // 使用SPI Flash
#define USE_SPI  // 使用SPI
#define USE_TIMER // 使用定时器
#endif //FURINA_APP_HPP