//
// Created by fairy on 2024/10/10.
//
/**
* @brief 中断服务例程
 * @note 用于处理各种中断回调函数，之所以不用stm32f4xx_it.c,是因为本工程是由C/C++混编，
 *      中断服务例程可能会调用到C++文件相关的代码。
 * @不详 这里发生了奇怪的事，有时候在这里定义甚至包括声明中断处理函数，但是链接时会把函数忽略，即使使用了
 *      extern "C"包含。但map文件确实没有任何相关函数符号，即使是-O0优化。编译也看了，会编译这个文件
 *      真是奇怪。难道是没有头文件的原因？但为什么EXTI0中断可以
 * @解决不详 好吧EXTI0也不可以，原因很简单，不能直接把该文件编译成静态库再取链接启动文件(除非该文件有头文件)。
 *      而是和启动文件一样，作为资源文件一起链接其他静态库
*/
#include "JYZQ_Conf.h"
#include "stm32f4xx_hal.h"
#include "timer.h"
#include "adc.h"
#include "key.hpp"
#include "usart.h"

#ifdef GUI_ENABLE

#include "GUI.hpp"

#endif

#include "fsmc.h"

#define KEY_RAM (*((volatile unsigned short *)0x6006000C)) // 键盘接口地址
extern DMA_HandleTypeDef hdma_memtomem_dma2_stream6;


// 由用户自行实现的中断处理函数
//extern void timer6_isr();

extern void adc1_isr();

// 弱定义，避免找不到

__weak void adc1_isr() {}

/** TIM中断回调函数
 * @note 关于TIM6已经转移至timer.c里了
 * */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
//    if (htim->Instance == TIM6)
//    {
////        timer6_isr();
//    }
}


/*ADC中断回调函数*/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC1)
    {
        adc1_isr();
    }
}


extern "C" {
void EXTI0_IRQHandler()
{
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
    Key::setCode(KEY_RAM & 0xF);//获取键值
#ifndef APP_NO_RTOS
    /*释放信号量*/
    osSemaphoreRelease(keySemHandle);
#else
    Key::setSign();
#endif
}

/**
 * @brief DMA中断
*/
// 主打一个狂野
#define DMA2_S6CR (*((volatile uint32_t *)0x400264A0))
// DMA中断,里面他宝贝的真啰嗦
void DMA2_Stream6_IRQHandler(void)
{
    /*  检查传输完成标志（TCIF）是否被设置，即是否传输完成标志位*/
    if (DMA2->HISR & 0x1 << 21)
    {
        DMA2->HISR &= ~(0x1 << 21);// 清除标志位
        // 检查中断使能标志位
        if (DMA2_S6CR & (DMA_IT_TC))
        {
            DMA2_S6CR &= ~(DMA_IT_TC);// 清除标志位
            /* Change the DMA state */
            hdma_memtomem_dma2_stream6.State = HAL_DMA_STATE_READY;// 不能少,因为Start_IT里需要靠它来开启
            /* Process Unlocked */
            __HAL_UNLOCK(&hdma_memtomem_dma2_stream6);// 不能少
        }
#ifdef GUI_ENABLE
        GUI::LVGL_LCD_FSMC_DMA_pCallback();
#endif
    }
}


}