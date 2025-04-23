#include "blue_tooth.h"
#include "usart.h"

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart1_rx;
#pragma pack(push, 1)
typedef struct {
    uint8_t header;     // 0xA5
    uint8_t version;    // 0x01
    float thd;          // 失真度
    float freq;         // 频率
    float rate;         // 采样率
    float gain;         // 增益
    float wave[24];     // 波形数据
    float harmonics[10];// 新增的10次谐波 ← 关键修改点
    uint8_t checksum;   // 校验和
} SensorDataPacket;
#pragma pack(pop)

// 静态断言确保结构体大小正确（155字节）
_Static_assert(sizeof(SensorDataPacket) == 155, "Invalid packet size");
static SensorDataPacket packet;
void send_bluetooth_data(float thd,float freq,float sampling_rate,float gain, int32_t* ADC_Data_lvgl,float* normalized)
{


//    等待上一次的数据发送完毕
#ifdef DMA_USART_ENABLE
    if(HAL_DMA_GetState(&hdma_usart1_tx) == HAL_DMA_STATE_READY)
    {
#endif
        // 初始化包头
        packet.header = 0xA5;
        packet.version = 0x01;

        // 填充基础参数
        packet.thd = thd;
        packet.freq = freq;
        packet.rate = sampling_rate;
        packet.gain = gain;

        // 填充波形和谐波数据()
        for(int i=0;i<24;++i)
        {
            packet.wave[i]=(float)(ADC_Data_lvgl[i]);
        }
        for(int i=0;i<10;++i)
        {
            packet.harmonics[i]=normalized[i+1];
        }

        // 计算校验和（需包含所有数据）
        uint8_t* p = (uint8_t*)&packet;
        packet.checksum = 0;
        for(int i=0; i<sizeof(packet)-1; i++) { // 排除checksum自身
            packet.checksum ^= p[i];
        }

#ifdef DMA_USART_ENABLE
        __HAL_DMA_DISABLE(&hdma_usart1_tx);
//        开始发送数据
        HAL_UART_Transmit_DMA(&huart1, (uint8_t*)&packet, sizeof(packet));
    }
#endif
#ifndef DMA_USART_ENABLE
    // 发送完整数据包
    HAL_UART_Transmit(&huart1, (uint8_t*)&packet, sizeof(packet), 0xFFFF);
#endif


}


//void HC05_Init(void)
//{
//    printf("AT+NAME=DV_bluetooth\r\n");
//    printf("AT+ROLE=0\r\n");
//    printf("AT+CMODE=1\r\n");
//    printf("AT+UART=9600,0,0\r\n");
//    printf("AT+PSWD=1234\r\n");
//    printf("AT+RESET\r\n");
//}