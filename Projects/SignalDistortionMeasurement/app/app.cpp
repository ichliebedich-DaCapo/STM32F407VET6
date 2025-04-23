#include <project_config.h>
// 头文件
#include <key.hpp>
#ifdef GUI_ENABLE

#include "GUI.hpp"
#include "ui.hpp"


#endif
#ifdef FREERTOS_ENABLE
#include "cmsis_os2.h"
#endif
// 头文件
#include "key.hpp"
#include "timer.h"
#include "rng.h"
#include "lcd.h"
#include "touch.h"
#include "delay.h"
#include "key_adapter.hpp"
#include "rcc.h"
#include "debug.h"
#include "spi.h"
#include "fatfs.h"
#include "esp8266.h"
#include "usart.h"
#include "tcp.h"
#include "stm32f4xx_hal.h"
#include "fft.hpp"
#include "blue_tooth.h"
//DSP相关头文件
#include "arm_math.h"
#include "arm_const_structs.h"
import async_delay;
using AsyncDelayHAL=AsyncDelay<HAL_GetTick>;
AsyncDelayHAL async_delay;
//using namespace gui;// 使用命名空间，简化书写

/**宏定义*/
#define NUM_SAMPLES 1024//采样点
#define SINGLE_PERIOD_SAMPLES 24//单周期点数 给lvgl使用

/// 双缓冲模块
typedef struct
{
    uint16_t full; // 是否读满
    uint16_t current_div; // 当前分频
    uint16_t buf_div; // 缓冲区分频
    uint16_t gain; // 增益控制
    uint32_t period; // 周期
} WaveInfo;

// 556KHz
#define FPGA_BUFFER_REG ((volatile unsigned short *)0x60000000)
#define FPGA_INFO_REG ((volatile WaveInfo *)0x60020000)
#define FPGA_READ_REG (*((volatile unsigned short *)0x60000800))// 状态寄存器（写入）


#define AV 1           //放大增益
#define NUM_PEAKS 9     //取9-1个基波和谐波成分

#define IFFTFLAG 0  //正变换
#define BITREVERSE 1//逆序排列

FFT_Calculation fft;
/**变量*/
int16_t ADC_Data[NUM_SAMPLES]={};
int32_t ADC_Data_lvgl[SINGLE_PERIOD_SAMPLES]={};
int32_t fft_output_lvgl[128]={};
extern UART_HandleTypeDef  huart1;
extern uint8_t UartRxData;
uint32_t start_tick;

//复位FPGA
void FPGA_RESET()
{
    // 第一阶段：复位信号激活（低电平有效时省略此步）
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);  // 释放复位
    HAL_Delay(10);  // 保证稳定

    // 正式复位脉冲
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET); // 拉低复位
    HAL_Delay(50);  // 维持复位状态时间（典型值20-100ms）

    // 结束复位
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);  // 释放复位
}

// 函数
void app_init()
{
    usart1_init();
    HAL_UART_Receive_IT(&huart1,(unsigned char*)&UartRxData,1);//串口接收
    async_delay.set_delay_tick(1000);
    async_delay.reset();

    GPIO_InitTypeDef GPIO_InitStruct = {};
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    FPGA_RESET();

    start_tick = HAL_GetTick();

}


void key_handler()
{
    switch (PlatformKey::getCode())
    {
        case keyK0:

        break;


        default:
        break;
    }
}
/**********全局变量**********/
uint32_t data_div;
// 定义超时时间（单位：ms）
const uint32_t TIMEOUT_MS = 5000;  // 根据实际需求调整
void background_handler()
{
    // 超时检测（自动处理32位溢出）
    if (HAL_GetTick() - start_tick > TIMEOUT_MS)
    {
        using namespace gui::interface::ui;
        gui::interface::ui::gui_clear_text_data();
        memset(ADC_Data_lvgl,0,sizeof(ADC_Data_lvgl)); //第三个变量一定要是size_t，用sizeof()函数比较好
        gui::interface::ui::gui_generate_chart_data(ADC_Data_lvgl);
    }

    // 判断是否可读取
    if (FPGA_INFO_REG->full)
    {
//            uint32_t  current_tick= HAL_GetTick();
        FPGA_READ_REG = 1;
        for (int i = 0; i < NUM_SAMPLES; ++i)
        {

//              ADC_Data [i]= FSMC_MAP(FPGA_BUFFER_BASE+i);//FSMC_MAP存在未知问题
            ADC_Data[i] = FPGA_BUFFER_REG[i];
        }
        data_div = FPGA_INFO_REG->buf_div;//非实时分频    FPGA_INFO_REG.div是实时的分频
        FPGA_READ_REG = 0;
//            uint32_t post_tick = HAL_GetTick();
//            printf("time:%lu\r\n",post_tick-current_tick);

        if (gui::interface::ui::start_fft())
        {

            for (int i = 0; i < SINGLE_PERIOD_SAMPLES; ++i)
            {
                ADC_Data_lvgl[i] = ADC_Data[i];
            }

            fft.ADCdataToSpectrum(ADC_Data);//将ADC数据经FFT 转为频谱
            //20000/400000*1024=51
            //波形一次谐波频率=51*400000/1024
            fft.Amplitude_Convert();//转为幅值谱

            fft.find_peaks_fundaments(NUM_PEAKS, 47);//寻找极大值点 1/(25.6/1024)

            fft.voltageAmplitude_Convert();//把频率谱转为电压幅值谱（可以不用加，因为交流成分彼此成齐次性）

            fft.signalDistortionDegree(NUM_PEAKS);//计算出失真度

            fft.normalize_to_fundamental(NUM_PEAKS);

            float freq = 200000000 * 11.0 / FPGA_INFO_REG->period;
            float sampling_rate=200000000 / (data_div + 1) / 2;

            //蓝牙模块通信
            send_bluetooth_data(fft.get_thd(),freq,sampling_rate,FPGA_INFO_REG->gain,ADC_Data_lvgl,fft.get_fft_output_normalized());

            if(gui::interface::ui::gui_get_chart_type()==0)
            {
                //画一个周期信号波形
                gui::interface::ui::gui_generate_chart_data(ADC_Data_lvgl);
            }
            if (gui::interface::ui::gui_get_chart_type()==1)
            {
                fft.convert_fft_to_lvgl(fft.get_fft_output_ptr(),fft_output_lvgl,4,2047);
                //画频谱（十个点取一个最值）
                gui::interface::ui::gui_generate_chart_data(fft_output_lvgl);
            }
            //gui显示失真度、THD、频率等文本内容
            gui::interface::ui::gui_generate_text_data(fft.get_fft_output_normalized(), fft.get_thd(),
                                                       sampling_rate, freq);

        }
        start_tick = HAL_GetTick();
    }
//    }



}



void SignalAnalyzer_handler()
{

}
