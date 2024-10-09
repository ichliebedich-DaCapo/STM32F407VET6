//
// Created by 34753 on 2024/10/7.
//

#include "keyTaskHandler.h"

#if ENABLE_KEY_TASK_HANDLE

#include "stm32f4xx_hal.h"

#define KEY_RAM (*((volatile unsigned short *)0x6006000C)) // 键盘接口地址

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

// 外部中断回调函数,先放在这，以后找到合适的位置再安放
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin)
    {
        /*开启外部中断0*/

        case GPIO_PIN_0:
            key.setCode(KEY_RAM & 0xF);//获取键值
            key.setSign();
#ifndef APP_NO_RTOS
            /*释放信号量*/
            osSemaphoreRelease(keySemHandle);
#else

#endif
            break;

            /*开启外部中断1*/

        case GPIO_PIN_1:
#if EXTI1_OPEN
            //    frequency = FPGA_RAM;
            osSemaphoreRelease(keySemHandle);
            break;
#endif
        default:
            break;
    }

}


#endif