//
// Created by fairy on 2024/10/8.
//

#include "spi.h"
#include "stm32f4xx_hal.h"
//#define USE_SPI_DMA   //lcd GUI isr 都有
#define UES_SPI_SD
SPI_HandleTypeDef hspi2;
SPI_HandleTypeDef hspi3;
DMA_HandleTypeDef hdma_spi2_tx;
DMA_HandleTypeDef hdma_spi2_rx;


/**
 * @brief SPI3初始化
 * @details 硬件SPI，软件片选，片选引脚需自行初始化
 */
void spi3_init()
{
    __HAL_RCC_SPI3_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /**SPI3 GPIO Configuration
    PC10     ------> SPI3_SCK
    PC11     ------> SPI3_MISO
    PC12     ------> SPI3_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    hspi3.Instance = SPI3;
    hspi3.Init.Mode = SPI_MODE_MASTER;
    hspi3.Init.Direction = SPI_DIRECTION_2LINES;
    hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi3.Init.NSS = SPI_NSS_SOFT;
    hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi3.Init.CRCPolynomial = 10;


    if (HAL_SPI_Init(&hspi3) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief SPI2初始化
 * @details 硬件SPI,使用的是软件片选，片选引脚依然是PB12
 */
void spi2_init()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* SPI2 clock enable */
    __HAL_RCC_SPI2_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**SPI2 GPIO Configuration
    PC2     ------> SPI2_MISO
    PC3     ------> SPI2_MOSI
    PB10     ------> SPI2_SCK
    PB12     ------> SPI2_NSS
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

#ifdef UES_SPI_SD
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
#endif

    hspi2.Instance = SPI2;
    hspi2.Init.Mode = SPI_MODE_MASTER;
    hspi2.Init.Direction = SPI_DIRECTION_2LINES;

    hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
//    hspi2.Init.DataSize = SPI_DATASIZE_16BIT;

    hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi2.Init.NSS = SPI_NSS_SOFT;// 软硬件片选
    hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
//    hspi2.Init.FirstBit = SPI_FIRSTBIT_LSB;
    hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi2.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&hspi2) != HAL_OK)
    {
        Error_Handler();
    }
#ifdef USE_SPI_DMA
    hdma_spi2_tx.Instance = DMA1_Stream4;
    hdma_spi2_tx.Init.Channel = DMA_CHANNEL_0;
    hdma_spi2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_spi2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi2_tx.Init.MemInc = DMA_MINC_ENABLE;
    //8位
    hdma_spi2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spi2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    //半字
//    hdma_spi2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
//    hdma_spi2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;

    hdma_spi2_tx.Init.Mode = DMA_NORMAL;
    hdma_spi2_tx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_spi2_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
//    hdma_spi2_tx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
//    hdma_spi2_tx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;


    HAL_DMA_Init(&hdma_spi2_tx);

    hdma_spi2_rx.Instance = DMA1_Stream3;
    hdma_spi2_rx.Init.Channel = DMA_CHANNEL_0;
    hdma_spi2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_spi2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi2_rx.Init.MemInc = DMA_MINC_ENABLE;
    //8位
    hdma_spi2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spi2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    //半字
//    hdma_spi2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
//    hdma_spi2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;

    hdma_spi2_rx.Init.Mode = DMA_NORMAL;
    hdma_spi2_rx.Init.Priority = DMA_PRIORITY_MEDIUM;
    hdma_spi2_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;


    HAL_DMA_Init(&hdma_spi2_rx);

#endif


#ifdef USE_SPI_DMA
    __HAL_LINKDMA(&hspi2, hdmatx, hdma_spi2_tx);
    __HAL_LINKDMA(&hspi2, hdmarx, hdma_spi2_rx);
#endif

}

void spi2_cs_low()
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
}
void spi2_cs_high()
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
}

void spi2_sendByte(unsigned char data)
{
    HAL_SPI_Transmit(&hspi2, &data, 1, HAL_MAX_DELAY);
}

/*启用SPI+DMA向LCD传输数据*/
void spi2_sendByteArray(unsigned char *data, unsigned int len)
{
    HAL_SPI_Transmit(&hspi2, data, len, HAL_MAX_DELAY);
}

#ifdef USE_SPI_DMA
void spi2_dma_Init(void)
{

    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA1_Stream4_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);

    HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);

}




#endif