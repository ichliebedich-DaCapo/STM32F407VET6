//
// Created by DV on 2025/2/5.
//
#include "touch.h"
#include "i2c.h"
#include "stm32f4xx_hal.h"

/**************宏定义******************/
#define FT_TRUE                  1
#define FT_FALSE                 0

//I2C读写命令
#define FT_CMD_WR                0X70         //写命令
#define FT_CMD_RD                0X71         //读命令

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

// MATCH VALUE LIST
#define PANNEL_ID                0x11

// GPIO引脚定义

#define TOUCH_RST_Pin GPIO_PIN_5
#define TOUCH_RST_GPIO_Port GPIOB
#define TOUCH_INT_Pin GPIO_PIN_6
#define TOUCH_INT_GPIO_Port GPIOB



// 函数重定义
#define FT_RST_L     HAL_GPIO_WritePin(TOUCH_RST_GPIO_Port, TOUCH_RST_Pin, GPIO_PIN_RESET)
#define FT_RST_H     HAL_GPIO_WritePin(TOUCH_RST_GPIO_Port, TOUCH_RST_Pin, GPIO_PIN_SET)

/*Configure GPIO pin : PB6 */

//触摸初始化函数（驱动芯片FT6336U）（返回1成功返回0失败）
uint8_t TOUCH_Init( void )
{

    i2c_TypeDef ucAck = I2C_FAIL;
    int set = FT_FALSE;
    uint8_t id;

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

    //配置外部中断
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    ft6336_rest();


    // check i2c addresss
    ucAck = i2c_CheckDevice( FT_CMD_WR );
    if( ucAck != I2C_OK ){
        return set;
    }

    // read chip id
    ft6336_rd_sigleReg(FT_ID_G_FOCALTECH_ID,&id);
    if(id != PANNEL_ID)
    {
        return set;
    }
    set = FT_TRUE;

    return set;

}


static void  ft6336_rest( void )
{
    FT_RST_L;
    touch_delay_us( 10 );
    FT_RST_H;
    touch_delay_us( 50000 );
}

int i2c_wr_register(uint16_t reg, uint8_t *buff, uint8_t len)
{
    HAL_StatusTypeDef status = HAL_I2C_Mem_Write(
            &hi2c1,
            FT_CMD_WR,          // 设备地址（写模式）
            reg,                // 寄存器地址
            I2C_MEMADD_SIZE_8BIT, // 寄存器地址长度（8位）
            buff,               // 数据
            len,                // 数据长度
            HAL_MAX_DELAY
    );
    return (status == HAL_OK) ? FT_TRUE : FT_FALSE;
}

int ft6336_rd_register(uint16_t reg, uint8_t *buff, uint8_t len)
{
    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(
            &hi2c1,
            FT_CMD_RD,          // 设备地址（读模式）
            reg,                // 寄存器地址
            I2C_MEMADD_SIZE_8BIT, // 寄存器地址长度（8位）
            buff,               // 数据缓冲区
            len,                // 数据长度
            HAL_MAX_DELAY
    );
    return (status == HAL_OK) ? FT_TRUE : FT_FALSE;
}

int ft6336_rd_sigleReg(uint16_t reg, uint8_t *val)
{
    return ft6336_rd_register(reg, val, 1); // 读取1字节
}



int ft6336_Init(void) {
    int set = FT_FALSE;
    uint8_t id;

    // 初始化I2C和GPIO
    I2C_Init();                // 使用你的I2C初始化函数
    ft6336_TouchIO_Init();
    ft6336_rest();

    // 检测设备是否存在
    if (i2c_CheckDevice(FT_CMD_WR) != I2C_OK) {
        return set;
    }

    // 读取芯片ID
    if (ft6336_rd_sigleReg(FT_ID_G_FOCALTECH_ID, &id) == FT_TRUE && id == PANNEL_ID) {
        set = FT_TRUE;
    }

    return set;
}