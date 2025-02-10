//
// Created by DV on 2025/2/5.
//
#include "touch.h"
#include "i2c.h"
#include "delay.h"

/**************宏定义******************/
#define FT6336_OK                 1
#define FT6336_ERROR              0

#define FT_FALSE                  0
#define FT_TRUE                   1
#define FT_FALSE1                 2
#define FT_FALSE2                 3

#define FT6336_ADDR              0x71

//FT5426 部分寄存器定义
#define FT_DEVIDE_MODE           0x00         //FT6336模式控制寄存器
#define FT_REG_NUM_FINGER        0x02         //触摸状态寄存器

#define FT_TP1_REG               0X03         //第一个触摸点数据地址
#define FT_TP2_REG               0X09         //第二个触摸点数据地址

#define FT_ID_G_CIPHER_MID       0x9F         //芯片代号（中字节） 默认值0x26
#define FT_ID_G_CIPHER_LOW       0xA0         //芯片代号（低字节） 0x01: Ft6336G  0x02: Ft6336U
#define FT_ID_G_LIB_VERSION      0xA1         //版本
#define FT_ID_G_CIPHER_HIGH      0xA3         //芯片代号（高字节） 默认0x64
#define FT_ID_G_MODE             0xA4         //FT6636中断模式控制寄存器
#define FT_ID_G_FOCALTECH_ID     0xA8         //VENDOR ID 默认值为0x11
#define FT_ID_G_THGROUP          0x80         //触摸有效值设置寄存器
#define FT_ID_G_PERIODACTIVE     0x88         //激活状态周期设置寄存器

#define PANNEL_ID                0x11


/**GPIO引脚定义
PB5     ------> CTP_RST
PB6     ------> CTP_INT
*/
#define TOUCH_RST_Pin GPIO_PIN_5
#define TOUCH_RST_GPIO_Port GPIOB
#define TOUCH_INT_Pin GPIO_PIN_6
#define TOUCH_INT_GPIO_Port GPIOB

// 函数重定义
#define FT_RST_L     HAL_GPIO_WritePin(TOUCH_RST_GPIO_Port, TOUCH_RST_Pin, GPIO_PIN_RESET)
#define FT_RST_H     HAL_GPIO_WritePin(TOUCH_RST_GPIO_Port, TOUCH_RST_Pin, GPIO_PIN_SET)



//触摸初始化函数（驱动芯片FT6336U）（返回1成功返回0失败）
uint8_t touch_init( void )
{
    uint8_t id=100;

    // 初始化i2c1
    i2c1_Init();
    //配置GPIO
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOB_CLK_ENABLE();
    HAL_GPIO_WritePin(TOUCH_RST_GPIO_Port, TOUCH_RST_Pin,GPIO_PIN_RESET);
    GPIO_InitStruct.Pin = TOUCH_RST_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(TOUCH_RST_GPIO_Port, &GPIO_InitStruct);

    //配置外部中断 可以放在BSP\src\key_exit中
    GPIO_InitStruct.Pin = TOUCH_INT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(TOUCH_INT_GPIO_Port, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    ft6336_rest();


    if(!ft6336_RdReg(FT_ID_G_FOCALTECH_ID,&id, 1))
    {
        return FT_FALSE1;//I2C通信故障
    }
    if(id != PANNEL_ID)
    {
        return FT_FALSE2;//寄存器值不匹配
    }
    return id;
}


void ft6336_rest( void )
{
    FT_RST_L;
    delay_us(10);
    FT_RST_H;
    HAL_Delay( 50);
}

uint8_t ft6336_WeReg( uint16_t regAdd, uint8_t *pData, uint16_t Size )
{
    HAL_StatusTypeDef status;

    status = I2C_WriteRegister(FT6336_ADDR, regAdd,pData, Size);
    if( status == HAL_OK)
        return FT6336_OK;
    else
        return FT6336_ERROR;
}

uint8_t ft6336_RdReg( uint16_t regAdd, uint8_t *pData, uint16_t Size )
{
    HAL_StatusTypeDef status;
    status = I2C_ReadRegister(FT6336_ADDR, regAdd,pData, Size);
    return (status == HAL_OK) ? FT6336_OK : FT6336_ERROR;
}

int32_t touch_read_single_point(int32_t *last_x, int32_t *last_y)
{
    uint8_t point_number;
    uint8_t touch_pos_buf[4];

    // 读取触摸点的数量
    ft6336_RdReg(FT_REG_NUM_FINGER, &point_number, 1);

    // 如果没有触摸点，返回0
    if (point_number == 0) {
        return 0;
    }

    // 读取第一个触摸点的坐标
    ft6336_RdReg(FT_TP1_REG, touch_pos_buf, 4);

    // 解析触摸点的坐标 横屏
    *last_x = 480 - (((uint16_t)(touch_pos_buf[2] & 0x0F) << 8) + touch_pos_buf[3]);
    *last_y = ((uint16_t)(touch_pos_buf[0] & 0x0F) << 8) + touch_pos_buf[1];

    // 返回1表示有触摸点
    return 1;
}



