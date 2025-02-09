//
// Created by DV on 2025/2/4.
//

#include "i2c.h"

I2C_HandleTypeDef hi2c1;
/* I2C1 init function */
void i2c1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 100000;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        Error_Handler();
    }
    //I2C���ж��¼���Ҫ�Ǳ�ʾ��������кʹ����һЩ�¼�������I2Cͨ��ʱһ��Ӧ��ʽͨ�ţ��������������ѯʽ�������ƣ���ʾ��������I2C1���жϡ�
//    /* I2C1 interrupt Init */
//    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0, 0);
//    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);

}
// д���ݵ�I2C�豸
void I2C_Write(uint8_t DevAddress, uint8_t *pData, uint16_t Size) {
    HAL_I2C_Master_Transmit(&hi2c1, DevAddress, pData, Size, HAL_MAX_DELAY);
}

// ��I2C�豸��ȡ����
void I2C_Read(uint8_t DevAddress, uint8_t *pData, uint16_t Size) {
     HAL_I2C_Master_Receive(&hi2c1, DevAddress, pData, Size, HAL_MAX_DELAY);
}
/**
  * @brief  ��ȡI2C�豸�Ĵ�������
  * @param  DevAddress: �豸��ַ
  * @param  RegAddress: �Ĵ�����ַ
  * @param  pData: ��ȡ������
  * @param  Size: ��ȡ�����ݳ���
  * @retval HAL_StatusTypeDef: ����״̬��HAL_OK��ʾ�ɹ���
  */
HAL_StatusTypeDef I2C_ReadRegister(uint16_t DevAddress, uint16_t RegAddress, uint8_t *pData, uint16_t Size) {
    return HAL_I2C_Mem_Read(&hi2c1, DevAddress, RegAddress, I2C_MEMADD_SIZE_8BIT, pData, Size, HAL_MAX_DELAY);
}

/**
  * @brief  д��I2C�豸�Ĵ�������
  * @param  DevAddress: �豸��ַ
  * @param  RegAddress: �Ĵ�����ַ
  * @param  pData: д�������
  * @param  Size: д������ݳ���
  * @retval HAL_StatusTypeDef: ����״̬��HAL_OK��ʾ�ɹ���
  */
HAL_StatusTypeDef I2C_WriteRegister(uint16_t DevAddress, uint16_t RegAddress, uint8_t *pData,uint16_t Size) {
    return HAL_I2C_Mem_Write(&hi2c1, DevAddress, RegAddress, I2C_MEMADD_SIZE_8BIT, pData, Size, HAL_MAX_DELAY);
}
