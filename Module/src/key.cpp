//
// Created by fairy on 2024/9/22.
//
#include "key.hpp"


extern void key_handler();
__attribute__((weak)) void key_handler(){/*由用户自己实现*/}

/**
 * 设置按键状态函数
 * @param maxKeyStates 设置存储按键状态的最大值，最大为4
 * @return
 * @note 真是见鬼！不知道当初怎么想到的
 *      功能：存储按键状态，每次按下按键，状态值加1，超过最大值后重置为0。
 *      比如你设为0（相当于不用这个函数），那么每次按下按键都会返回1。如果你设为1，那么每两次按下按键就会返回1，就像开关一样
 *      if(key.stateHandler(0)){}
 */
uint8_t Key::stateHandler(uint8_t maxKeyStates)
{
    uint8_t temp = (state >> (code * 2)) & 0x3;//取出对应键位的值
    temp = ((temp + 1) & 0x3) % maxKeyStates;//切换状态
    state = (state & ~(0x3 << (code * 2))) | (temp << (code * 2));//设置新状态，先清零再设置
    return temp;
}



#ifdef APP_NO_RTOS
auto Key::handler() -> void
{
    if(sign)
    {
        sign = 0;
        key_handler();
    }
}
#else
auto Key::handler()-> void{}
#endif


//#ifdef APP_NO_RTOS
//auto Key::handler()->void
//{
//
//}
//#endif




#ifndef APP_NO_RTOS
#include "cmsis_os2.h"

osSemaphoreId_t keySemHandle;


osThreadId_t keyTaskHandler;
const osThreadAttr_t keyTask_attributes = {
        .name = "keyTask",
        .stack_size = 256 * 4,
        .priority = (osPriority_t) osPriorityNormal,
};

void keyTask(void *argument);


// 初始化按键任务
void keyTaskHandler_init()
{
    Key::init();
    keyTaskHandler = osThreadNew(keyTask, nullptr, &keyTask_attributes);
    // 创建二值信号量，初始值为0
    keySemHandle = osSemaphoreNew(1, 0, nullptr);
}

// 按键任务处理
void keyTask(void *argument)
{
    for(;;)
    {
        osSemaphoreAcquire(keySemHandle, osWaitForever);
        /* xxx_taskHandle(); */

    }
}
#endif



//// 外部中断回调函数,先放在这，以后找到合适的位置再安放
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//    switch (GPIO_Pin)
//    {
//        /*开启外部中断0*/
//
//        case GPIO_PIN_0:
//            Key::setCode(KEY_RAM & 0xF);//获取键值
//
//#ifndef APP_NO_RTOS
//            /*释放信号量*/
//            osSemaphoreRelease(keySemHandle);
//#else
//            Key::setSign();
//#endif
//            break;
//
//            /*开启外部中断1*/
//
//        case GPIO_PIN_1:
//#if EXTI1_OPEN
//            //    frequency = FPGA_RAM;
//            osSemaphoreRelease(keySemHandle);
//            break;
//#endif
//        default:
//            break;
//    }
//}





