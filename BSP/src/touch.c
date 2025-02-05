//
// Created by DV on 2025/2/5.
//
#include "touch.h"
#include "i2c.h"
#include "stm32f4xx_hal.h"

/**************�궨��******************/
#define FT_TRUE                  1
#define FT_FALSE                 0

//I2C��д����
#define FT_CMD_WR                0X70         //д����
#define FT_CMD_RD                0X71         //������

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

// GPIO���Ŷ���

#define TOUCH_RST_Pin GPIO_PIN_5
#define TOUCH_RST_GPIO_Port GPIOB
#define TOUCH_INT_Pin GPIO_PIN_6
#define TOUCH_INT_GPIO_Port GPIOB



// �����ض���
#define FT_RST_L     HAL_GPIO_WritePin(TOUCH_RST_GPIO_Port, TOUCH_RST_Pin, GPIO_PIN_RESET)
#define FT_RST_H     HAL_GPIO_WritePin(TOUCH_RST_GPIO_Port, TOUCH_RST_Pin, GPIO_PIN_SET)

/*Configure GPIO pin : PB6 */

//������ʼ������������оƬFT6336U��������1�ɹ�����0ʧ�ܣ�
uint8_t TOUCH_Init( void )
{

    i2c_TypeDef ucAck = I2C_FAIL;
    int set = FT_FALSE;
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
            FT_CMD_WR,          // �豸��ַ��дģʽ��
            reg,                // �Ĵ�����ַ
            I2C_MEMADD_SIZE_8BIT, // �Ĵ�����ַ���ȣ�8λ��
            buff,               // ����
            len,                // ���ݳ���
            HAL_MAX_DELAY
    );
    return (status == HAL_OK) ? FT_TRUE : FT_FALSE;
}

int ft6336_rd_register(uint16_t reg, uint8_t *buff, uint8_t len)
{
    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(
            &hi2c1,
            FT_CMD_RD,          // �豸��ַ����ģʽ��
            reg,                // �Ĵ�����ַ
            I2C_MEMADD_SIZE_8BIT, // �Ĵ�����ַ���ȣ�8λ��
            buff,               // ���ݻ�����
            len,                // ���ݳ���
            HAL_MAX_DELAY
    );
    return (status == HAL_OK) ? FT_TRUE : FT_FALSE;
}

int ft6336_rd_sigleReg(uint16_t reg, uint8_t *val)
{
    return ft6336_rd_register(reg, val, 1); // ��ȡ1�ֽ�
}



int ft6336_Init(void) {
    int set = FT_FALSE;
    uint8_t id;

    // ��ʼ��I2C��GPIO
    I2C_Init();                // ʹ�����I2C��ʼ������
    ft6336_TouchIO_Init();
    ft6336_rest();

    // ����豸�Ƿ����
    if (i2c_CheckDevice(FT_CMD_WR) != I2C_OK) {
        return set;
    }

    // ��ȡоƬID
    if (ft6336_rd_sigleReg(FT_ID_G_FOCALTECH_ID, &id) == FT_TRUE && id == PANNEL_ID) {
        set = FT_TRUE;
    }

    return set;
}