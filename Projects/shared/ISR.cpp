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

#include <project_config.h>
import key_adapter;
import usart;
import spi;

#ifdef GUI_ENABLE
import gui;
import lcd;
#endif
#ifdef FreeRTOS_ENABLE
#include "cmsis_os2.h"
#endif




#define KEY_RAM (*((volatile unsigned short *)0x6006000C)) // 键盘接口地址
//extern DMA_HandleTypeDef hdma_memtomem_dma2_stream6;





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
extern void adc1_isr();
__weak void adc1_isr() {}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC1)
    {
        adc1_isr();
    }
}


#ifdef BSP_USE_PWR
void HAL_PWR_PVDCallback(void)
{
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_PVDO); // 清除 PVD 标志
    if (__HAL_PWR_GET_FLAG(PWR_FLAG_PVDO)) // 检查是否是电压下降
    {
        // 断电时执行的代码
//        Execute_Shutdown_Code();

//        // 保存关键数据到后备寄存器
//        HAL_PWR_EnableBkUpAccess(); // 允许访问后备寄存器
//        __HAL_RCC_BKP_CLK_ENABLE(); // 启用后备寄存器时钟
//        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, 0x12345678); // 示例：保存数据
    }
    else
    {
        // 电压恢复时执行的代码（可选）

    }

}
#endif

/*******************************中断服务例程**************************************/

extern "C" {
void EXTI0_IRQHandler()
{
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
    PlatformKey::isr_entry(KEY_RAM & 0xF);//获取键值
}

#ifdef DMA_SPI_ENABLE
 void DMA1_Stream4_IRQHandler()
{
    /* 使用HAL库预定义宏检测标志 */
    if(__HAL_DMA_GET_FLAG(&bsp::spi::hdma_spi2_tx, __HAL_DMA_GET_TC_FLAG_INDEX(&bsp::spi::hdma_spi2_tx)))
    {
        // 清除传输完成标志
        __HAL_DMA_CLEAR_FLAG(&bsp::spi::hdma_spi2_tx, __HAL_DMA_GET_TC_FLAG_INDEX(&bsp::spi::hdma_spi2_tx));

        // 更新HAL状态机（关键！否则下次传输无法启动）
        bsp::spi::hdma_spi2_tx.State = HAL_DMA_STATE_READY;
        __HAL_UNLOCK(&bsp::spi::hdma_spi2_tx);

        // 直接执行后续操作（示例：关闭片选+通知渲染完成）
        bsp::spi::cs_high();
        gui::Render::display_flush_ready();
    }
}
#endif



/**
 * @brief DMA中断
*/
// 主打一个狂野
#define DMA2_S6CR (*((volatile uint32_t *)0x400264A0))
// DMA中断,里面他宝贝的真啰嗦
void DMA2_Stream6_IRQHandler(void)
{
//    /*  检查传输完成标志（TCIF）是否被设置，即是否传输完成标志位*/
//    if (DMA2->HISR & 0x1 << 21)
//    {
//        DMA2->HISR &= ~(0x1 << 21);// 清除标志位
//        // 检查中断使能标志位
//        if (DMA2_S6CR & (DMA_IT_TC))
//        {
//            DMA2_S6CR &= ~(DMA_IT_TC);// 清除标志位
//            /* Change the DMA state */
//            hdma_memtomem_dma2_stream6.State = HAL_DMA_STATE_READY;// 不能少,因为Start_IT里需要靠它来开启
//            /* Process Unlocked */
//            __HAL_UNLOCK(&hdma_memtomem_dma2_stream6);// 不能少
//        }
//#ifndef GUI_ENABLE
//      GUI::display_flush_ready();
//#endif
//    }
}

}



//DMA_USART_ENABLE 里封印着ESP8266的中断回调函数
#ifdef DMA_USART_ENABLE
DMA_HandleTypeDef hdma_usart1_tx;
uint8_t UartRxData;
uint8_t UartRxFlag;
uint8_t UartIntRxbuf[500];
uint16_t UartRxIndex;

//串口1在1字节接收完成回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

    if(huart==&bsp::usart::huart1)//判断是否串口1
    {
//        UartRxFlag=0x55;//接收标志置位
        UartIntRxbuf[UartRxIndex]=UartRxData;//数据写入缓冲区
        UartRxIndex++;//记载数目加1
        if(UartRxIndex>=500)//缓冲区是500字节，如果存满，归零
        {
            UartRxIndex=0;
        }
        HAL_UART_Receive_IT(&bsp::usart::huart1,(unsigned char*)&UartRxData,1);//继续接收下一字节
    }

}
// UART发送完成中断调用。 UART_DMATransmitCplt  和 UART_EndTransmit_IT 调用


#else
uint8_t UartIntRxbuf[500];
uint16_t UartRxIndex;
 uint8_t UartRxData;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

    if(huart==&bsp::usart::huart1)//判断是否串口1
    {
        UartIntRxbuf[UartRxIndex]=UartRxData;//数据写入缓冲区
        UartRxIndex++;//记载数目加1
        if(UartRxIndex>=500)//缓冲区是500字节，如果存满，归零
        {
            UartRxIndex=0;
        }
        HAL_UART_Receive_IT(&bsp::usart::huart1,(unsigned char*)&UartRxData,1);//继续接收下一字节
    }

}
#endif
void SysTick_Handler()
{
    HAL_IncTick();
}



extern "C" {
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
    while (1)
    {
    }
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
    while (1)
    {
        __BKPT();
    }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
    while (1)
    {
    }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
    while (1)
    {
    }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
    while (1)
    {
    }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
}

void EXTI9_5_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
}

}

