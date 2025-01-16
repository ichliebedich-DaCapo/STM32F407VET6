//
// Created by fairy on 2024/10/8.
//
#include "w25qxx.h"
#include "spi.h"

extern SPI_HandleTypeDef hspi2;

/*FLASH_SPI*/
#define FLASH_SPI SPI2
#define FLASH_SPI_Handle hspi2
#define FAST_READ_WRITE // 目前看好像是有问题

#ifndef USE_HARD_CS
// 片选引脚
#define W25QXX_CS_GPIO_Port GPIOB
#define W25QXX_CS_PIN GPIO_PIN_12
#define W25QXX_CS_HIGH() HAL_GPIO_WritePin(W25QXX_CS_GPIO_Port, W25QXX_CS_PIN, GPIO_PIN_SET)
#define W25QXX_CS_LOW() HAL_GPIO_WritePin(W25QXX_CS_GPIO_Port, W25QXX_CS_PIN, GPIO_PIN_RESET)
#endif


/*FLASH常用命令*/
#define WriteEnable 0x06   // 写使能
#define ReadStatusReg 0x05 // 读状态寄存器
#define WriteStatusReg 0x01
#define ReadData 0x03
#define PageProgram 0x02 // 页写入
#define SectorErase 0xD8
#define ChipErase 0xC7 // 整片擦除

/*标志*/
#define Dummy_Byte 0xFF
#define WIP_Flag 0x01

/*FLASH页大小*/
#define W25QXX_PAGE_SIZE 256

/************************内联函数********************/

// 【暂时没用，本体在SPI文件】初始化片选引脚
static inline void w25qxx_spi_select(void)
{
#ifndef USE_HARD_CS
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /*片选初始化*/
    GPIO_InitStruct.Pin = W25QXX_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP; // 默认上拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(W25QXX_CS_GPIO_Port, &GPIO_InitStruct);
    HAL_GPIO_WritePin(W25QXX_CS_GPIO_Port, W25QXX_CS_PIN, GPIO_PIN_SET);
#endif
}

/**
 * @brief  使用SPI发送一个字节的数据
 * @param  byte：要发送的数据
 * @retval 返回接收到的数据
 */
static inline uint8_t SPI_FLASH_TransferByte(uint8_t byte)
{
    uint8_t temp = 0;
    HAL_SPI_TransmitReceive(&FLASH_SPI_Handle, &byte, &temp, 1, HAL_MAX_DELAY);
    return temp;
}

/**
 * @brief  向FLASH发送 写使能 命令
 * @param  none
 * @retval none
 */
static inline void SPI_FLASH_WriteEnable(void)
{
#ifndef USE_HARD_CS
    W25QXX_CS_LOW();
#endif
    SPI_FLASH_TransferByte(WriteEnable); /* 发送写使能命令*/
#ifndef USE_HARD_CS
    W25QXX_CS_HIGH();
#endif
}

/**
 * @brief  等待WIP(BUSY)标志被置0，即等待到FLASH内部数据写入完毕
 * @param  none
 * @retval none
 */
static inline void SPI_FLASH_WaitForWriteEnd(void)
{
    uint8_t FLASH_Status = 0;
#ifndef USE_HARD_CS
    W25QXX_CS_LOW();
#endif
    SPI_FLASH_TransferByte(ReadStatusReg); /* 发送 读状态寄存器 命令 */
    do
    {
        FLASH_Status = SPI_FLASH_TransferByte(Dummy_Byte);
    } while (FLASH_Status & WIP_Flag); // 检测BUSY位是否为0,0表示已完成不忙
#ifndef USE_HARD_CS
    W25QXX_CS_HIGH();
#endif
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
#ifndef USE_HARD_CS
    W25QXX_CS_LOW();
#endif
}

/**
 * @brief 结束写入操作
 * @param none
 * @retval none
 */
static inline void SPI_FLASH_EndWrite(void)
{
#ifndef USE_HARD_CS
    W25QXX_CS_HIGH();
#endif
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
#ifdef FAST_READ_WRITE
    HAL_SPI_Transmit(&FLASH_SPI_Handle, pBuffer, NumBytes, HAL_MAX_DELAY);
#else
    for (uint16_t i = 0; i < NumBytes; ++i)
    {
        SPI_FLASH_TransferByte(pBuffer[i]);
    }
#endif
}

/**
 * @brief  接收数据
 * @param  pBuffer，接收数据的指针
 * @param  NumBytes，要接收的数据长度
 * @retval 无
 */
static inline void SPI_FLASH_ReceiveData(uint8_t *pBuffer, uint16_t NumBytes)
{
#ifdef FAST_READ_WRITE
    HAL_SPI_Receive(&FLASH_SPI_Handle, pBuffer, NumBytes, HAL_MAX_DELAY);
#else
    for (uint16_t i = 0; i < NumBytes; ++i)
    {
        pBuffer[i] = SPI_FLASH_TransferByte(Dummy_Byte);
    }
#endif
}


/************************接口函数********************/

/**
 * @brief 初始化SPI Flash * @details 先进行硬件SPI2的初始化，再进行片选引脚的初始化
 *          然后初始化SPI Flash的参数
 */
void w25qxx_init(void)
{
    /**
     * D     ------> 1：MOSI     PC3
     * Q     ------> 2：MISO     PC2
     * C     ------> 3：SCLOCK   PB10
     * S     ------> 4：CS       PB12

    初始化可以不进行，默认为0x00*/
    SPI_FLASH_WriteEnable(); // 使能读写
#ifndef USE_HARD_CS
    W25QXX_CS_LOW();
#endif
    SPI_FLASH_TransferByte(0x50); /*这是反复改变BP位的关键*/
#ifndef USE_HARD_CS
    W25QXX_CS_HIGH();
#endif
#ifndef USE_HARD_CS
    W25QXX_CS_LOW();
#endif
    SPI_FLASH_TransferByte(WriteStatusReg);
    SPI_FLASH_TransferByte(0x00);
#ifndef USE_HARD_CS
    W25QXX_CS_HIGH();
#endif
    SPI_FLASH_WaitForWriteEnd();
}

/**
 * @brief  擦除FLASH扇区
 * @param  SectorAddr：要擦除的扇区地址
 * @retval 无
 * @note SECTOR ERASE命令的地址可以是该扇区内的任意地址
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
 * @brief  将任意长度的数据写入FLASH
 * @param  pBuffer，要写入数据的指针
 * @param  WriteAddr，写入地址
 * @param  NumByteToWrite，写入数据长度
 * @retval 无
 */
void w25qxx_buffer_write(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint16_t bytesToWrite;
    uint32_t currentAddr = WriteAddr;

    while (NumByteToWrite > 0)
    {
        // 计算当前页剩余空间
        uint16_t pageOffset = currentAddr % W25QXX_PAGE_SIZE;
        bytesToWrite = W25QXX_PAGE_SIZE - pageOffset;

        // 如果剩余数据小于等于当前页剩余空间，则直接写入剩余数据
        if (bytesToWrite > NumByteToWrite)
        {
            bytesToWrite = NumByteToWrite;
        }

        // 擦除目标页（如果需要）
        if (pageOffset == 0)
        {
            w25qxx_sector_erase(currentAddr);
        }

        // 写入数据
        w25qxx_page_write(pBuffer, currentAddr, bytesToWrite);

        // 更新指针和计数
        currentAddr += bytesToWrite;
        pBuffer += bytesToWrite;
        NumByteToWrite -= bytesToWrite;
    }
}

/**
 * @brief  将uint16_t数组写入FLASH
 * @param  pBuf16，要写入的uint16_t数据指针
 * @param  WriteAddr，写入地址
 * @param  NumHalfWordsToWrite，写入的半字（uint16_t）数量
 * @retval 无
 */
void w25qxx_buffer_write_uint16(uint16_t *pBuf16, uint32_t WriteAddr, uint16_t NumHalfWordsToWrite)
{
    uint16_t bytesToWrite;
    uint32_t currentAddr = WriteAddr;

    while (NumHalfWordsToWrite > 0)
    {
        // 计算当前页剩余空间
        uint16_t pageOffset = currentAddr % W25QXX_PAGE_SIZE;
        bytesToWrite = (W25QXX_PAGE_SIZE - pageOffset) / 2;

        // 如果剩余数据小于等于当前页剩余空间，则直接写入剩余数据
        if (bytesToWrite > NumHalfWordsToWrite)
        {
            bytesToWrite = NumHalfWordsToWrite;
        }

        // 擦除目标页（如果需要）
        if (pageOffset == 0)
        {
            w25qxx_sector_erase(currentAddr);
        }

        // 写入数据
        w25qxx_page_write((uint8_t *)pBuf16, currentAddr, bytesToWrite * 2);

        // 更新指针和计数
        currentAddr += bytesToWrite * 2;
        pBuf16 += bytesToWrite;
        NumHalfWordsToWrite -= bytesToWrite;
    }
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
#ifndef USE_HARD_CS
    W25QXX_CS_LOW();
#endif
    SPI_FLASH_TransferByte(ReadData);
    SPI_FLASH_TransferAddress(ReadAddr);
    SPI_FLASH_ReceiveData(pBuffer, NumByteToRead);
#ifndef USE_HARD_CS
    W25QXX_CS_HIGH();
#endif
}

uint8_t w25qxx_read_byte(uint32_t ReadAddr)
{
    uint8_t temp;
#ifndef USE_HARD_CS
    W25QXX_CS_LOW();
#endif
    SPI_FLASH_TransferByte(ReadData);
    SPI_FLASH_TransferAddress(ReadAddr);
    temp = SPI_FLASH_TransferByte(Dummy_Byte);
#ifndef USE_HARD_CS
    W25QXX_CS_HIGH();
#endif
    return temp;
}


/**
 * @brief  从FLASH读取uint16_t数组
 * @param  pBuf16，存储读出数据的uint16_t指针
 * @param  ReadAddr，读取地址
 * @param  NumHalfWordsToRead，读取的半字（uint16_t）数量
 * @retval 无
 */
void w25qxx_buffer_read_uint16(uint16_t *pBuf16, uint32_t ReadAddr, uint16_t NumHalfWordsToRead)
{
    uint16_t bytesToRead;
    uint32_t currentAddr = ReadAddr;

    while (NumHalfWordsToRead > 0)
    {
        // 计算当前页剩余空间
        uint16_t pageOffset = currentAddr % W25QXX_PAGE_SIZE;
        bytesToRead = (W25QXX_PAGE_SIZE - pageOffset) / 2;

        // 如果剩余数据小于等于当前页剩余空间，则直接读取剩余数据
        if (bytesToRead > NumHalfWordsToRead)
        {
            bytesToRead = NumHalfWordsToRead;
        }

        // 读取数据
        w25qxx_buffer_read((uint8_t *)pBuf16, currentAddr, bytesToRead * 2);

        // 更新指针和计数
        currentAddr += bytesToRead * 2;
        pBuf16 += bytesToRead;
        NumHalfWordsToRead -= bytesToRead;
    }
}


void w25qxx_write_byte(uint32_t WriteAddr, uint8_t byte)
{
    SPI_FLASH_BeginWrite();
    SPI_FLASH_TransferByte(PageProgram);
    SPI_FLASH_TransferAddress(WriteAddr);
    SPI_FLASH_TransferByte(byte);
    SPI_FLASH_EndWrite();
}

/**
 * @brief  异步擦除整片扇区
 * @note 使用的时候必须手动调用w25qxx_async_is_busy()函数判断是否擦除完成
 */
void w25qxx_async_chip_erase()
{
    SPI_FLASH_BeginWrite();
    SPI_FLASH_TransferByte(ChipErase);
    W25QXX_CS_HIGH();
}

void w25qxx_async_sector_erase(uint32_t SectorAddr)
{
    SPI_FLASH_BeginWrite();
    SPI_FLASH_TransferByte(SectorErase);
    SPI_FLASH_TransferAddress(SectorAddr);
    W25QXX_CS_HIGH();
}

/**
 * @brief  判断是否擦除完成
 * @return ture表示完成，false表示正在忙
 */
uint8_t w25qxx_async_is_busy()
{
    uint8_t FLASH_Status = Dummy_Byte;
#ifndef USE_HARD_CS
    W25QXX_CS_LOW();
#endif
    SPI_FLASH_TransferByte(ReadStatusReg); /* 发送 读状态寄存器 命令 */
    FLASH_Status = SPI_FLASH_TransferByte(Dummy_Byte);
    FLASH_Status &= WIP_Flag; // 检测BUSY位是否为0,0表示已完成不忙
#ifndef USE_HARD_CS
    W25QXX_CS_HIGH();
#endif
    return FLASH_Status;
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



