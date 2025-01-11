//
// Created by fairy on 2025/1/11 22:39.
//
#include "debug.h"
#include "JYZQ_Conf.h"
// 头文件
#include "stm32f4xx_hal.h"


#define ITM_LAR_ACCESS_KEY    0xC5ACCE55  // ITM 解锁密钥
//#define ITM_TCR_ITMENA_Pos    0           // ITM 启用位
//#define ITM_TCR_SYNCENA_Pos   2           // 同步数据包启用位
#define ITM_TCR_TXENA_Pos     3           // TXENA 启用位
#define ITM_TER_PORTENA_Pos   0           // ITM 端口启用位

// 函数


// 初始化 ITM 和 SWO
void ITM_Init()
{
    // 启用 ITM 和 TPIU
    DBGMCU->CR |= DBGMCU_CR_TRACE_IOEN;  // 启用跟踪引脚
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;  // 启用 ITM 和 DWT

    // 配置 TPIU
    TPI->ACPR = 83;  // 设置 SWO 波特率 = CPU 时钟 / (ACPR + 1)
    TPI->SPPR = 2;   // 设置 SWO 协议为异步模式（UART）
    TPI->FFCR = 0x00;  // 禁用格式化器

    // 启用 ITM
    ITM->LAR = ITM_LAR_ACCESS_KEY;  // 解锁 ITM
    ITM->TCR = (1 << ITM_TCR_ITMENA_Pos) |  // 启用 ITM
               (1 << ITM_TCR_SYNCENA_Pos) | // 启用同步数据包
               (1 << ITM_TCR_TXENA_Pos);    // 启用 TXENA
    ITM->TER = (1 << ITM_TER_PORTENA_Pos);  // 启用 ITM 端口 0
}


#include <unistd.h>  // 提供 _write 函数的声明

int _write(int file, char *ptr, int len)
{
    for (int i = 0; i < len; ++i)
    {
        ITM_SendChar(ptr[i]);  // 使用 ITM_SendChar 发送字符
    }
    return len;
}



