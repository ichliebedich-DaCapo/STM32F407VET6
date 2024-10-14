//
// Created by 34753 on 2024/10/8.
//
#include "w25qxx.h"

#ifdef USE_SPI_FLASH

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

/************************内联函数********************/

// 初始化片选引脚
static inline void w25qxx_spi_select(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /*片选初始化*/
    GPIO_InitStruct.Pin = W25QXX_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP; // 默认上拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(W25QXX_CS_GPIO_Port, &GPIO_InitStruct);
    HAL_GPIO_WritePin(W25QXX_CS_GPIO_Port, W25QXX_CS_PIN, GPIO_PIN_SET);
}

/**
 * @brief  使用SPI发送一个字节的数据
 * @param  byte：要发送的数据
 * @retval 返回接收到的数据
 */
static inline uint8_t SPI_FLASH_TransferByte(uint8_t byte)
{
    uint8_t temp = 0;
    HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(&FLASH_SPI_Handle, &byte, &temp, 1, HAL_MAX_DELAY);
    assert(status == HAL_OK && "SPI transfer failed");
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
    SPI_FLASH_TransferByte(WriteEnable); /* 发送写使能命令*/
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
    SPI_FLASH_TransferByte(ReadStatusReg); /* 发送 读状态寄存器 命令 */
    do
    {
        FLASH_Status = SPI_FLASH_TransferByte(Dummy_Byte);
    } while (FLASH_Status & WIP_Flag); // 检测BUSY位是否为0,0表示已完成不忙
    W25QXX_CS_HIGH();
}

/**
 * @brief 开始写入操作
 * @param none
 * @retval none
 */
static inline void SPI_FLASH_BeginWrite(void)
{
    SPI_FLASH_WriteEnable();
    SPI_FLASH_WaitForWriteEnd();
    W25QXX_CS_LOW();
}

/**
 * @brief 结束写入操作
 * @param none
 * @retval none
 */
static inline void SPI_FLASH_EndWrite(void)
{
    W25QXX_CS_HIGH();
    SPI_FLASH_WaitForWriteEnd();
}

/**
 * @brief  发送地址
 * @param  Address：要发送的地址
 * @retval 无
 */
static inline void SPI_FLASH_TransferAddress(uint32_t Address)
{
    SPI_FLASH_TransferByte((Address >> 16) & 0xFF);
    SPI_FLASH_TransferByte((Address >> 8) & 0xFF);
    SPI_FLASH_TransferByte(Address & 0xFF);
}

/**
 * @brief  发送数据
 * @param  pBuffer，要发送的数据指针
 * @param  NumBytes，要发送的数据长度
 * @retval 无
 */
static inline void SPI_FLASH_TransmitData(uint8_t *pBuffer, uint16_t NumBytes)
{
    for (uint16_t i = 0; i < NumBytes; ++i)
    {
        SPI_FLASH_TransferByte(pBuffer[i]);
    }
}

/**
 * @brief  接收数据
 * @param  pBuffer，接收数据的指针
 * @param  NumBytes，要接收的数据长度
 * @retval 无
 */
static inline void SPI_FLASH_ReceiveData(uint8_t *pBuffer, uint16_t NumBytes)
{
    for (uint16_t i = 0; i < NumBytes; ++i)
    {
        pBuffer[i] = SPI_FLASH_TransferByte(Dummy_Byte);
    }
}


/************************接口函数********************/

/**
 * @brief 初始化SPI Flash
 * @details 先进行硬件SPI3的初始化，再进行片选引脚的初始化
 *          然后初始化SPI Flash的参数
 */
void w25qxx_init(void)
{
    w25qxx_spi_select();
    spi3_init();
}

/**
 * @brief  擦除FLASH扇区
 * @param  SectorAddr：要擦除的扇区地址
 * @retval 无
 */
void w25qxx_sector_erase(uint32_t SectorAddr)
{
    SPI_FLASH_BeginWrite();
    SPI_FLASH_TransferByte(SectorErase);
    SPI_FLASH_TransferAddress(SectorAddr);
    SPI_FLASH_EndWrite();
}

/**
 * @brief 擦除整片扇区
 * @param 无
 * @retval 无
 */
void w25qxx_chip_erase(void)
{
    SPI_FLASH_BeginWrite();
    SPI_FLASH_TransferByte(ChipErase);
    SPI_FLASH_EndWrite();
}

/**
 * @brief  对FLASH按页写入数据，调用本函数写入数据前需要先擦除扇区
 * @param  pBuffer，要写入数据的指针
 * @param WriteAddr，写入地址
 * @param  NumByteToWrite，写入数据长度，必须小于等于页大小，没有设置判断语句
 * @retval 无
 */
void w25qxx_page_write(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    SPI_FLASH_BeginWrite();
    SPI_FLASH_TransferByte(PageProgram);
    SPI_FLASH_TransferAddress(WriteAddr);
    SPI_FLASH_TransmitData(pBuffer, NumByteToWrite);
    SPI_FLASH_EndWrite();
}

/**
 * @brief  读取FLASH数据
 * @param  pBuffer，存储读出数据的指针
 * @param   ReadAddr，读取地址
 * @param   NumByteToRead，读取数据长度
 * @retval 无
 */
void w25qxx_buffer_read(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
    W25QXX_CS_LOW();
    SPI_FLASH_TransferByte(ReadData);
    SPI_FLASH_TransferAddress(ReadAddr);
    SPI_FLASH_ReceiveData(pBuffer, NumByteToRead);
    W25QXX_CS_HIGH();
}

uint8_t w25qxx_read_byte(uint32_t ReadAddr)
{
    uint8_t temp;
    W25QXX_CS_LOW();
    SPI_FLASH_TransferByte(ReadData);
    SPI_FLASH_TransferAddress(ReadAddr);
    temp = SPI_FLASH_TransferByte(Dummy_Byte);
    W25QXX_CS_HIGH();
    return temp;
}

void w25qxx_write_byte(uint32_t WriteAddr, uint8_t byte)
{
    SPI_FLASH_BeginWrite();
    SPI_FLASH_TransferByte(PageProgram);
    SPI_FLASH_TransferAddress(WriteAddr);
    SPI_FLASH_TransferByte(byte);
    SPI_FLASH_EndWrite();
}

///**
// * @brief  使用DMA批量读取FLASH数据
// * @param  pBuffer，存储读出数据的指针
// * @param   ReadAddr，读取地址
// * @param   NumByteToRead，读取数据长度
// * @retval 无
// */
//void SPI_FLASH_BufferRead_DMA(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
//{
//    W25QXX_CS_LOW();
//    SPI_FLASH_TransferByte(ReadData);
//    SPI_FLASH_TransferAddress(ReadAddr);
//
//    /* 使用DMA读取数据 */
//    HAL_StatusTypeDef status = HAL_SPI_Receive_DMA(&FLASH_SPI_Handle, pBuffer, NumByteToRead);
//    assert(status == HAL_OK && "DMA receive failed");
//
//    /* 等待DMA传输完成 */
//    while (__HAL_SPI_GET_FLAG(&FLASH_SPI_Handle, SPI_FLAG_BSY));
//    W25QXX_CS_HIGH();
//}
//
///**
// * @brief  使用DMA批量写入FLASH数据
// * @param  pBuffer，要写入数据的指针
// * @param WriteAddr，写入地址
// * @param  NumByteToWrite，写入数据长度，必须小于等于页大小，没有设置判断语句
// * @retval 无
// */
//void SPI_FLASH_PageWrite_DMA(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
//{
//    SPI_FLASH_BeginWrite();
//    SPI_FLASH_TransferByte(PageProgram);
//    SPI_FLASH_TransferAddress(WriteAddr);
//
//    /* 使用DMA写入数据 */
//    HAL_StatusTypeDef status = HAL_SPI_Transmit_DMA(&FLASH_SPI_Handle, pBuffer, NumByteToWrite);
//    assert(status == HAL_OK && "DMA transmit failed");
//
//    /* 等待DMA传输完成 */
//    while (__HAL_SPI_GET_FLAG(&FLASH_SPI_Handle, SPI_FLAG_BSY));
//    SPI_FLASH_EndWrite();
//}





#endif