//
// Created by DV on 2025/2/5.
//
#include "touch.h"
#include "i2c.h"
#include "stm32f4xx_hal.h"

/**************�궨��******************/
#define FT6336_OK                 1
#define FT6336_ERROR              0

#define FT_TRUE                  1
#define FT_FALSE                 0

#define FT6336_ADDR              0X71

//FT5426 ���ּĴ�������
#define FT_DEVIDE_MODE           0x00         //FT6336ģʽ���ƼĴ���
#define FT_REG_NUM_FINGER        0x02         //����״̬�Ĵ���

#define FT_TP1_REG               0X03         //��һ�����������ݵ�ַ
#define FT_TP2_REG               0X09         //�ڶ������������ݵ�ַ

#define FT_ID_G_CIPHER_MID       0x9F         //оƬ���ţ����ֽڣ� Ĭ��ֵ0x26
#define FT_ID_G_CIPHER_LOW       0xA0         //оƬ���ţ����ֽڣ� 0x01: Ft6336G  0x02: Ft6336U
#define FT_ID_G_LIB_VERSION      0xA1         //�汾
#define FT_ID_G_CIPHER_HIGH      0xA3         //оƬ���ţ����ֽڣ� Ĭ��0x64
#define FT_ID_G_MODE             0xA4         //FT6636�ж�ģʽ���ƼĴ���
#define FT_ID_G_FOCALTECH_ID     0xA8         //VENDOR ID Ĭ��ֵΪ0x11
#define FT_ID_G_THGROUP          0x80         //������Чֵ���üĴ���
#define FT_ID_G_PERIODACTIVE     0x88         //����״̬�������üĴ���

// MATCH VALUE LIST
#define PANNEL_ID                0x11

/**GPIO���Ŷ���
PB5     ------> CTP_RST
PB6     ------> CTP_INT
*/
#define TOUCH_RST_Pin GPIO_PIN_5
#define TOUCH_RST_GPIO_Port GPIOB
#define TOUCH_INT_Pin GPIO_PIN_6
#define TOUCH_INT_GPIO_Port GPIOB

// �����ض���
#define FT_RST_L     HAL_GPIO_WritePin(TOUCH_RST_GPIO_Port, TOUCH_RST_Pin, GPIO_PIN_RESET)
#define FT_RST_H     HAL_GPIO_WritePin(TOUCH_RST_GPIO_Port, TOUCH_RST_Pin, GPIO_PIN_SET)



//������ʼ������������оƬFT6336U��������1�ɹ�����0ʧ�ܣ�
uint8_t TOUCH_Init( void )
{

    uint8_t id;

    // ��ʼ��i2c1
    i2c1_Init();
    //����GPIO
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOB_CLK_ENABLE();
    HAL_GPIO_WritePin(TOUCH_RST_GPIO_Port, TOUCH_RST_Pin,GPIO_PIN_RESET);
    GPIO_InitStruct.Pin = TOUCH_RST_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(TOUCH_RST_GPIO_Port, &GPIO_InitStruct);

    //�����ⲿ�ж�
    GPIO_InitStruct.Pin = TOUCH_INT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(TOUCH_INT_GPIO_Port, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    ft6336_rest();

    ft6336_RdReg(FT_ID_G_FOCALTECH_ID,&id, 1);
    if(id != PANNEL_ID)
    {
        return FT_FALSE;
    }
    return FT_TRUE;
}


static void ft6336_rest( void )
{
    FT_RST_L;
    touch_delay_us( 10 );
    FT_RST_H;
    touch_delay_us( 50000 );
}


uint8_t ft6336_WeReg( uint16_t regAdd, uint8_t *pData, uint16_t Size )
{
    HAL_StatusTypeDef status;

    status = I2C_ReadRegister(FT6336_ADDR, regAdd,pData, Size);
    if( status == HAL_OK)
        return FT6336_OK;
    else
        return FT6336_ERROR;
}

uint8_t ft6336_RdReg( uint16_t regAdd, uint8_t *pData, uint16_t Size )
{
    HAL_StatusTypeDef status;

    status = I2C_WriteRegister(FT6336_ADDR, regAdd,pData, Size);
    if( status == HAL_OK)
        return FT6336_OK;
    else
        return FT6336_ERROR;
}



static void touch_delay_us(uint32_t us)
{
    uint32_t i=0;

    while(us--){
        for(i=0;i<1000;i++);
    }
}


