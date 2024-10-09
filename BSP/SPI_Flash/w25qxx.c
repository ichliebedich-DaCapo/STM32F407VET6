//
// Created by 34753 on 2024/10/8.
//

#include "w25qxx.h"
#if USE_SPI_FLASH

/*FLASH_SPI*/
#define FLASH_SPI SPI3
#define FLASH_SPI_Handle hspi3

// 片选引脚
#define W25QXX_CS_GPIO_Port GPIOC
#define W25QXX_CS_PIN GPIO_PIN_9
#define W25QXX_CS_HIGH() HAL_GPIO_WritePin(W25QXX_CS_GPIO_Port, W25QXX_CS_PIN, GPIO_PIN_SET)
#define W25QXX_CS_LOW() HAL_GPIO_WritePin(W25QXX_CS_GPIO_Port, W25QXX_CS_PIN, GPIO_PIN_RESET)

/*FLASH常用命令*/
#define WriteEnable 0x06   // 写使能
#define ReadStatusReg 0x05 // 读状态寄存器
#define WriteStatusReg 0x01
#define ReadData 0x03
#define PageProgram 0x02 // 页写入
#define BlockErase 0xD8  // 块擦除
#define SectorErase 0x20
#define ChipErase 0xC7 // 整片擦除

/*标志*/
#define Dummy_Byte 0xFF
#define WIP_Flag 0x01


/*******内联函数******/
// 初始化片选引脚
static inline void w25qxx_spi_select(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /*片选初始化*/
    GPIO_InitStruct.Pin = W25QXX_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;// 默认上拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(W25QXX_CS_GPIO_Port, &GPIO_InitStruct);
    HAL_GPIO_WritePin(W25QXX_CS_GPIO_Port, W25QXX_CS_PIN, GPIO_PIN_SET);
}

/**
 * @brief  使用SPI发送一个字节的数据
 * @param  byte：要发送的数据
 * @retval 返回接收到的数据
 */
static inline uint8_t SPI_FLASH_SendByte(uint8_t byte)
{
    uint8_t temp = 0;
    HAL_SPI_TransmitReceive(&FLASH_SPI_Handle, &byte, &temp, 1, 1);
    return temp;
}

/**
 * @brief  向FLASH发送 写使能 命令
 * @param  none
 * @retval none
 */
static inline void SPI_FLASH_WriteEnable(void)
{
    W25QXX_CS_LOW();
    SPI_FLASH_SendByte(WriteEnable); /* 发送写使能命令*/
    W25QXX_CS_HIGH();
}

/**
 * @brief  等待WIP(BUSY)标志被置0，即等待到FLASH内部数据写入完毕
 * @param  none
 * @retval none
 */
static inline void SPI_FLASH_WaitForWriteEnd(void)
{
    uint8_t FLASH_Status = 0;
    W25QXX_CS_LOW();
    SPI_FLASH_SendByte(ReadStatusReg); /* 发送 读状态寄存器 命令 */
    do
    {
        FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);
    } while (FLASH_Status & WIP_Flag); // 检测BUSY位是否为0,0表示已完成不忙
    W25QXX_CS_HIGH();
}


/**
 * @brief 初始化SPI Flash
 * @details 先进行硬件SPI3的初始化，再进行片选引脚的初始化
 *          然后初始化SPI Flash的参数
 */
void w25qxx_init(void)
{
    w25qxx_spi_select();
    spi3_init();

    /*初始化可以不进行，默认为0x00*/
    SPI_FLASH_WriteEnable(); // 使能读写
    W25QXX_CS_LOW();
    SPI_FLASH_SendByte(0x50); /*这是反复改变BP位的关键*/
    W25QXX_CS_HIGH();

    W25QXX_CS_LOW();
    SPI_FLASH_SendByte(WriteStatusReg);
    SPI_FLASH_SendByte(0x00);
    W25QXX_CS_HIGH();
    SPI_FLASH_WaitForWriteEnd();
}


/**
 * @brief  擦除FLASH扇区
 * @param  SectorAddr：要擦除的扇区地址
 * @retval 无
 */
void SPI_FLASH_SectorErase(uint32_t SectorAddr)
{
    SPI_FLASH_WriteEnable(); /* 发送FLASH写使能命令 */
    W25QXX_CS_LOW();
    SPI_FLASH_SendByte(SectorErase);
    SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16); // 高位
    SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);    // 中位
    SPI_FLASH_SendByte(SectorAddr & 0xFF);             // 低位
    W25QXX_CS_HIGH();
    SPI_FLASH_WaitForWriteEnd(); // 等待擦除完毕
}

/**
 * @brief 擦除整片扇区
 * @param 无
 * @retval 无
 */
void SPI_FLASH_ChipErase()
{
    SPI_FLASH_WriteEnable(); /* 发送FLASH写使能命令 */
    /* 擦除扇区 */
    W25QXX_CS_LOW();
    SPI_FLASH_SendByte(ChipErase); // 低位
    W25QXX_CS_HIGH();
    SPI_FLASH_WaitForWriteEnd(); // 等待擦除完毕
}

/**
 * @brief  对FLASH按页写入数据，调用本函数写入数据前需要先擦除扇区
 * @param  pBuffer，要写入数据的指针
 * @param WriteAddr，写入地址
 * @param  NumByteToWrite，写入数据长度，必须小于等于页大小，没有设置判断语句
 * @retval 无
 */
void SPI_FLASH_PageWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    SPI_FLASH_WaitForWriteEnd(); // 千辛万苦才得来的
    SPI_FLASH_WriteEnable();     /* 发送FLASH写使能命令 */
    W25QXX_CS_LOW();
    SPI_FLASH_SendByte(PageProgram);
    SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
    SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
    SPI_FLASH_SendByte(WriteAddr & 0xFF);

    /* 写入数据*/
    while (NumByteToWrite--)
    {
        SPI_FLASH_SendByte(*pBuffer);
        pBuffer++;
    }

    W25QXX_CS_HIGH();
    SPI_FLASH_WaitForWriteEnd(); /* 等待写入完毕*/
}

/**
 * @brief  读取FLASH数据
 * @param  pBuffer，存储读出数据的指针
 * @param   ReadAddr，读取地址
 * @param   NumByteToRead，读取数据长度
 * @retval 无
 */
void SPI_FLASH_BufferRead(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
    W25QXX_CS_LOW();
    SPI_FLASH_SendByte(ReadData);
    SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
    SPI_FLASH_SendByte((ReadAddr & 0xFF00) >> 8);
    SPI_FLASH_SendByte(ReadAddr & 0xFF);

    /* 读取数据 */
    while (NumByteToRead--)
    {
        *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
        pBuffer++;
    }
    W25QXX_CS_HIGH();
}

uint8_t SPI_FLASH_ReadByte(uint32_t ReadAddr)
{
    uint8_t temp;
    W25QXX_CS_LOW();
    SPI_FLASH_SendByte(ReadData);
    SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
    SPI_FLASH_SendByte((ReadAddr & 0xFF00) >> 8);
    SPI_FLASH_SendByte(ReadAddr & 0xFF);
    temp = SPI_FLASH_SendByte(Dummy_Byte);
    W25QXX_CS_HIGH();
    return temp;
}

void SPI_FLASH_WriteByte(uint32_t WriteAddr, uint8_t byte)
{
    // SPI_FLASH_WaitForWriteEnd(); //
    SPI_FLASH_WriteEnable(); /* 发送FLASH写使能命令 */
    W25QXX_CS_LOW();
    SPI_FLASH_SendByte(PageProgram);
    SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
    SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
    SPI_FLASH_SendByte(WriteAddr & 0xFF);
    SPI_FLASH_SendByte(byte);
    W25QXX_CS_HIGH();
    // SPI_FLASH_WaitForWriteEnd(); // 94.8us,如果是写入慢的可以加入这句
}




#endif