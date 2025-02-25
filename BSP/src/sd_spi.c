#include "sd_spi.h"
#include "stm32f4xx_hal.h"
#include "spi.h"

extern SPI_HandleTypeDef hspi2;
#define SD_SPI_CLK                       RCC_APB1Periph_SPI2
#define SD_SPI_APBxClock_FUN            RCC_APB1PeriphClockCmd

#define SD_SPI_SCK_PIN                   GPIO_PIN_10
#define SD_SPI_SCK_GPIO_PORT             GPIOB
#define SD_SPI_SCK_GPIO_CLK              RCC_APB2Periph_GPIOB

#define SD_SPI_MISO_PIN                  GPIO_PIN_2
#define SD_SPI_MISO_GPIO_PORT            GPIOC
#define SD_SPI_MISO_GPIO_CLK             RCC_APB2Periph_GPIOC

#define SD_SPI_MOSI_PIN                  GPIO_PIN_3
#define SD_SPI_MOSI_GPIO_PORT            GPIOC
#define SD_SPI_MOSI_GPIO_CLK             RCC_APB2Periph_GPIOC

#define SD_CS_PIN                        GPIO_PIN_1
#define SD_CS_GPIO_PORT                  GPIOC
#define SD_CS_GPIO_CLK                   RCC_APB2Periph_GPIOA

//#define SD_DETECT_PIN                    GPIO_Pin_0
//#define SD_DETECT_GPIO_PORT              GPIOE
//#define SD_DETECT_GPIO_CLK               RCC_APB2Periph_GPIOE


/**SDIO SPI模式 GPIO Configuration
 * 除了CS，与SPI2保持一致
PC1      ------> CS
PC2      ------> SDIO_D0 -------->miso
NC       ------> SDIO_D1 -------->NC
NC       ------> SDIO_D2 -------->NC
PC1      ------> SDIO_D3 -------->CS
PB10     ------> SDIO_CLK ------->CLK
PC3      ------> SDIO_CMD-------->mosi
*/

// 片选信号CS输出低电平
void SD_CS_LOW()
{
    HAL_GPIO_WritePin(SD_CS_GPIO_PORT, SD_CS_PIN, GPIO_PIN_RESET);
}
//  片选信号CS输出高电平
void SD_CS_HIGH()
{
    HAL_GPIO_WritePin(SD_CS_GPIO_PORT, SD_CS_PIN, GPIO_PIN_SET);
}

/*C文件*/
SD_CardInfo SDCardInfo; //用于存储卡的信息
uint8_t  SD_Type = SD_TYPE_NOT_SD;  //存储卡的类型


//SPI模式
SD_Error spi_sd_init()
{
#ifndef LCD_SPI_PORT_ENABLE
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
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    hspi2.Instance = SPI2;
    hspi2.Init.Mode = SPI_MODE_MASTER;
    hspi2.Init.Direction = SPI_DIRECTION_2LINES;

    hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
    hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
    hspi2.Init.NSS = SPI_NSS_SOFT;// 软硬件片选
    hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
    hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi2.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&hspi2) != HAL_OK)
    {
        Error_Handler();
    }
#endif

    uint32_t i = 0;
    sd_spi_speed_low(); /* 设置到低速模式 注：初始化时,时钟频率<400kHz */
    /*!< CS高电平 */
    SD_CS_HIGH();

    /*!< CS高电平期间，发送 空 字节数据 0xFF, 10 次 */
    /*!< Rise CS and MOSI for 80 clocks cycles */
    for (i = 0; i <= 10; i++) {
        /*!< 发送 空 字节数据 0xFF */
        SD_WriteByte(SD_DUMMY_BYTE);
    }

    if (SD_GoIdleState() == SD_RESPONSE_FAILURE)
        return SD_RESPONSE_FAILURE;

//获取卡的类型,最多尝试10次
    i=10;
    do
    {
        SD_GetCardType();
    } while (SD_Type == SD_TYPE_NOT_SD || i-- > 0);

    SD_Error error;
    //不支持的卡
    if (SD_Type == SD_TYPE_NOT_SD)
        return SD_RESPONSE_FAILURE;

    error= SD_GetCardInfo(&SDCardInfo);
    sd_spi_speed_high(); /* 恢复到原先模式 */
    return error;

}

/**
  * @brief  设置 SD 卡 SPI 低速模式（初始化时时钟频率 < 400kHz）
  * @note   基于 STM32 HAL 库实现，需确保 SPI2 时钟源频率已知（例如 42MHz APB1）
  *         选择 SPI_BAUDRATEPRESCALER_128 实现 328kHz 低速模式
  */
void sd_spi_speed_low(void)
{
    // 检查预分频器宏有效性（需适配具体 HAL 库版本）
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BAUDRATEPRESCALER_1));

    __HAL_SPI_DISABLE(&hspi2);              // 关闭 SPI 总线
    hspi2.Instance->CR1 &= 0xFFC7;          // 清除 BR[2:0] 位（位 3-5）
    hspi2.Instance->CR1 |= SPI_BAUDRATEPRESCALER_256;  // 设置 128 分频
    __HAL_SPI_ENABLE(&hspi2);               // 重新使能 SPI
}

/**
  * @brief  设置 SD 卡 SPI 低速模式（初始化时时钟频率 < 400kHz）
  * @note   基于 STM32 HAL 库实现，需确保 SPI2 时钟源频率已知（例如 42MHz APB1）
  *         选择 SPI_BAUDRATEPRESCALER_128 实现 328kHz 低速模式
  */
void sd_spi_speed_high(void)
{
    // 检查预分频器宏有效性（需适配具体 HAL 库版本）
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BAUDRATEPRESCALER_1));

    __HAL_SPI_DISABLE(&hspi2);              // 关闭 SPI 总线
    hspi2.Instance->CR1 &= 0xFFC7;          // 清除 BR[2:0] 位（位 3-5）
    hspi2.Instance->CR1 |= SPI_BAUDRATEPRESCALER_2;  // 设置 128 分频
    __HAL_SPI_ENABLE(&hspi2);               // 重新使能 SPI
}


/**
* @brief  让SD卡进入空闲模式.
* @param  None
* @retval SD卡响应:
*         - SD_RESPONSE_FAILURE: 失败
*         - SD_RESPONSE_NO_ERROR: 成功
*/
SD_Error SD_GoIdleState(void)
{
    /*!< 片选CS低电平 */
    SD_CS_LOW();

    /*!< 发送 CMD0 (SD_CMD_GO_IDLE_STATE) 让SD卡切换至SPI模式 */
    SD_SendCmd(SD_CMD_GO_IDLE_STATE, 0, 0x95);

    /*!< 等待R1响应返回的状态为SD_IN_IDLE_STATE */
    if (SD_GetResponse(SD_IN_IDLE_STATE))
    {
        /*!< 响应不是空闲状态，失败返回 */
        return SD_RESPONSE_FAILURE;
    }

    /*!< 片选CS高电平 */
    SD_CS_HIGH();

    /*!< 发送 空 字节 0xFF */
    SD_WriteByte(SD_DUMMY_BYTE);

    /*初始化成功返回*/
    return SD_RESPONSE_NO_ERROR;
}

/**
* @brief  获取SD卡的版本类型，并区分SDSC和SDHC
* @param  无
* @retval SD响应:
*         - SD_RESPONSE_FAILURE: 失败
*         - SD_RESPONSE_NO_ERROR: 成功
*/
SD_Error SD_GetCardType(void)
{
    uint32_t i = 0;
    uint32_t Count = 0xFFF;

    uint8_t R7R3_Resp[4];
    uint8_t R1_Resp;

    SD_CS_HIGH();

    /*!< 发送空字节延时 0xFF */
    SD_WriteByte(SD_DUMMY_BYTE);

    /*!< 片选信号CS低电平 */
    SD_CS_LOW();


    /*!< 发送 CMD8 命令，带0x1AA检查参数*/
    SD_SendCmd(SD_CMD_SEND_IF_COND, 0x1AA, 0x87);

    /*!< 等待R1响应 */
    while (( (R1_Resp = SD_ReadByte()) == 0xFF) && Count) {
        Count--;
    }
    if (Count == 0) {
        /*!< 等待超时 */
        return SD_RESPONSE_FAILURE;
    }

    //响应 = 0x05   非V2.0的卡
    if (R1_Resp == (SD_IN_IDLE_STATE|SD_ILLEGAL_COMMAND)) {
        /* 激活SD卡 */
        do {
            /*!< 片选信号CS高电平 */
            SD_CS_HIGH();

            /*!< 发送空字节延时 0xFF */
            SD_WriteByte(SD_DUMMY_BYTE);

            /*!< 片选信号CS低电平 */
            SD_CS_LOW();

            /*!< 发送CMD1完成V1 版本卡的初始化 */
            SD_SendCmd(SD_CMD_SEND_OP_COND, 0, 0xFF);
            /*!< Wait for no error Response (R1 Format) equal to 0x00 */
        } while (SD_GetResponse(SD_RESPONSE_NO_ERROR));
        //V1版本的卡完成初始化

        SD_Type = SD_TYPE_V1;

        //不处理MMC卡

        //初始化正常

    }
        //响应 0x01   V2.0的卡
    else if (R1_Resp == SD_IN_IDLE_STATE) {
        /*!< 读取CMD8 的R7响应 */
        for (i = 0; i < 4; i++) {
            R7R3_Resp[i] = SD_ReadByte();
        }

        /*!< 片选信号CS高电平 */
        SD_CS_HIGH();

        /*!< 发送空字节延时 0xFF */
        SD_WriteByte(SD_DUMMY_BYTE);

        /*!< 片选信号CS低电平 */
        SD_CS_LOW();

        //判断该卡是否支持2.7-3.6V电压
        if (R7R3_Resp[2]==0x01 && R7R3_Resp[3]==0xAA) {
            //支持电压范围，可以操作
            Count = 200;
            //发卡初始化指令CMD55+ACMD41
            do {
                //CMD55，以强调下面的是ACMD命令
                SD_SendCmd(SD_CMD_APP_CMD, 0, 0xFF);
                // SD_IN_IDLE_STATE
                if (!SD_GetResponse(SD_RESPONSE_NO_ERROR))
                    return SD_RESPONSE_FAILURE; //超时返回

                //ACMD41命令带HCS检查位
                SD_SendCmd(SD_ACMD_SD_SEND_OP_COND, 0x40000000, 0xFF);

                if (Count-- == 0)
                    return SD_RESPONSE_FAILURE; //重试次数超时
            } while (SD_GetResponse(SD_RESPONSE_NO_ERROR));

            //初始化指令完成，读取OCR信息，CMD58

            //-----------鉴别SDSC SDHC卡类型开始-----------

            Count = 200;
            do {
                /*!< 片选信号CS高电平 */
                SD_CS_HIGH();

                /*!< 发送空字节延时 0xFF */
                SD_WriteByte(SD_DUMMY_BYTE);

                /*!< 片选信号CS低电平 */
                SD_CS_LOW();

                /*!< 发送CMD58 读取OCR寄存器 */
                SD_SendCmd(SD_CMD_READ_OCR, 0, 0xFF);
            } while ( SD_GetResponse(SD_RESPONSE_NO_ERROR) || Count-- == 0);

            if (Count == 0)
                return SD_RESPONSE_FAILURE; //重试次数超时

            //响应正常，读取R3响应

            /*!< 读取CMD58的R3响应 */
            for (i = 0; i < 4; i++) {
                R7R3_Resp[i] = SD_ReadByte();
            }

            //检查接收到OCR中的bit30(CCS)
            //CCS = 0:SDSC       CCS = 1:SDHC
            if (R7R3_Resp[0]&0x40) { //检查CCS标志
                SD_Type = SD_TYPE_V2HC;
            } else {
                SD_Type = SD_TYPE_V2;
            }
            //-----------鉴别SDSC SDHC版本卡的流程结束-----------
        }
    }

    /*!< 片选信号CS高电平 */
    SD_CS_HIGH();

    /*!< 发送空字节延时 0xFF */
    SD_WriteByte(SD_DUMMY_BYTE);

    //初始化正常返回
    return SD_RESPONSE_NO_ERROR;
}

/**
* @brief  读取卡的CSD寄存器
*         在SPI模式下，读取CSD的方式与读取数据块类似
* @param  SD_csd: 存储CSD寄存器的SD_CSD结构体指针
* @retval SD响应:
*         - SD_RESPONSE_FAILURE: 失败
*         - SD_RESPONSE_NO_ERROR: 成功
*/
SD_Error SD_GetCSDRegister(SD_CSD* SD_csd)
{
    uint32_t i = 0;
    SD_Error rvalue = SD_RESPONSE_FAILURE;
    uint8_t CSD_Tab[16];

    /*!< 片选信号CS低电平*/
    SD_CS_LOW();
    /*!< 发送 CMD9 (CSD register) 或 CMD10(CID register)命令 */
    SD_SendCmd(SD_CMD_SEND_CSD, 0, 0xFF);
    /*!< 等待 R1 响应 (0x00 is no errors) */
    if (!SD_GetResponse(SD_RESPONSE_NO_ERROR)) {
        //等待数据块Token
        if (!SD_GetResponse(SD_START_DATA_SINGLE_BLOCK_READ)) {
            for (i = 0; i < 16; i++) {
                /*!< 存储 CSD 寄存器的值到 CSD_Tab */
                CSD_Tab[i] = SD_ReadByte();
            }
        }
        /*!< 读取 CRC 校验字节 (此处不校验，但SD卡有该流程，需要接收) */
        SD_WriteByte(SD_DUMMY_BYTE);
        SD_WriteByte(SD_DUMMY_BYTE);
        /*!< 设置返回值，表示成功接收到寄存器数据 */
        rvalue = SD_RESPONSE_NO_ERROR;
    }
    /*!< 片选信号CS高电平*/
    SD_CS_HIGH();
    /*!< 发送 dummy 空字节: 8 时钟周期的延时 */
    SD_WriteByte(SD_DUMMY_BYTE);

    /*!< 字节 0 处理*/
    SD_csd->CSDStruct = (CSD_Tab[0] & 0xC0) >> 6;
    SD_csd->SysSpecVersion = (CSD_Tab[0] & 0x3C) >> 2;
    SD_csd->Reserved1 = CSD_Tab[0] & 0x03;

    /*!< 字节 1 处理*/
    SD_csd->TAAC = CSD_Tab[1];

    /*!< 字节 2 处理*/
    SD_csd->NSAC = CSD_Tab[2];

    /*!< 字节 3 处理*/
    SD_csd->MaxBusClkFrec = CSD_Tab[3];

    /*!< 字节 4 处理*/
    SD_csd->CardComdClasses = CSD_Tab[4] << 4;

    /*!< 字节 5 处理*/
    SD_csd->CardComdClasses |= (CSD_Tab[5] & 0xF0) >> 4;
    SD_csd->RdBlockLen = CSD_Tab[5] & 0x0F;

    /*!< 字节 6 处理*/
    SD_csd->PartBlockRead = (CSD_Tab[6] & 0x80) >> 7;
    SD_csd->WrBlockMisalign = (CSD_Tab[6] & 0x40) >> 6;
    SD_csd->RdBlockMisalign = (CSD_Tab[6] & 0x20) >> 5;
    SD_csd->DSRImpl = (CSD_Tab[6] & 0x10) >> 4;
    SD_csd->Reserved2 = 0; /*!< Reserved */

    SD_csd->DeviceSize = (CSD_Tab[6] & 0x03) << 10;

    //V1卡与SDSC卡的信息
    if ((SD_Type == SD_TYPE_V1) || (SD_Type == SD_TYPE_V2))
    {
        /*!< 字节 7 处理*/
        SD_csd->DeviceSize |= (CSD_Tab[7]) << 2;

        /*!< 字节 8 处理*/
        SD_csd->DeviceSize |= (CSD_Tab[8] & 0xC0) >> 6;

        SD_csd->MaxRdCurrentVDDMin = (CSD_Tab[8] & 0x38) >> 3;
        SD_csd->MaxRdCurrentVDDMax = (CSD_Tab[8] & 0x07);

        /*!< 字节 9 处理*/
        SD_csd->MaxWrCurrentVDDMin = (CSD_Tab[9] & 0xE0) >> 5;
        SD_csd->MaxWrCurrentVDDMax = (CSD_Tab[9] & 0x1C) >> 2;
        SD_csd->DeviceSizeMul = (CSD_Tab[9] & 0x03) << 1;
        /*!< 字节 10 处理*/
        SD_csd->DeviceSizeMul |= (CSD_Tab[10] & 0x80) >> 7;
    }
        //SDHC卡的信息
    else if (SD_Type == SD_TYPE_V2HC)
    {
        SD_csd->DeviceSize = (CSD_Tab[7] & 0x3F) << 16;

        SD_csd->DeviceSize |= (CSD_Tab[8] << 8);

        SD_csd->DeviceSize |= (CSD_Tab[9]);
    }


    SD_csd->EraseGrSize = (CSD_Tab[10] & 0x40) >> 6;
    SD_csd->EraseGrMul = (CSD_Tab[10] & 0x3F) << 1;

    /*!< 字节 11 处理*/
    SD_csd->EraseGrMul |= (CSD_Tab[11] & 0x80) >> 7;
    SD_csd->WrProtectGrSize = (CSD_Tab[11] & 0x7F);

    /*!< 字节 12 处理*/
    SD_csd->WrProtectGrEnable = (CSD_Tab[12] & 0x80) >> 7;
    SD_csd->ManDeflECC = (CSD_Tab[12] & 0x60) >> 5;
    SD_csd->WrSpeedFact = (CSD_Tab[12] & 0x1C) >> 2;
    SD_csd->MaxWrBlockLen = (CSD_Tab[12] & 0x03) << 2;

    /*!< 字节 13 处理*/
    SD_csd->MaxWrBlockLen |= (CSD_Tab[13] & 0xC0) >> 6;
    SD_csd->WriteBlockPaPartial = (CSD_Tab[13] & 0x20) >> 5;
    SD_csd->Reserved3 = 0;
    SD_csd->ContentProtectAppli = (CSD_Tab[13] & 0x01);

    /*!< 字节 14 处理*/
    SD_csd->FileFormatGrouop = (CSD_Tab[14] & 0x80) >> 7;
    SD_csd->CopyFlag = (CSD_Tab[14] & 0x40) >> 6;
    SD_csd->PermWrProtect = (CSD_Tab[14] & 0x20) >> 5;
    SD_csd->TempWrProtect = (CSD_Tab[14] & 0x10) >> 4;
    SD_csd->FileFormat = (CSD_Tab[14] & 0x0C) >> 2;
    SD_csd->ECC = (CSD_Tab[14] & 0x03);

    /*!< 字节 15 处理*/
    SD_csd->CSD_CRC = (CSD_Tab[15] & 0xFE) >> 1;
    SD_csd->Reserved4 = 1;

    /*!< 返回接收状态 */
    return rvalue;
}

/**
  * @brief  Read the CID card register.
  *         Reading the contents of the CID register in SPI mode is a simple
  *         read-block transaction.
  * @param  SD_cid: pointer on an CID register structure
  * @retval The SD Response:
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
SD_Error SD_GetCIDRegister(SD_CID* SD_cid)
{
    uint32_t i = 0;
    SD_Error rvalue = SD_RESPONSE_FAILURE;
    uint8_t CID_Tab[16];

    /*!< SD chip select low */
    SD_CS_LOW();

    /*!< Send CMD10 (CID register) */
    SD_SendCmd(SD_CMD_SEND_CID, 0, 0xFF);

    /*!< Wait for response in the R1 format (0x00 is no errors) */
    if (!SD_GetResponse(SD_RESPONSE_NO_ERROR))
    {
        if (!SD_GetResponse(SD_START_DATA_SINGLE_BLOCK_READ))
        {
            /*!< Store CID register value on CID_Tab */
            for (i = 0; i < 16; i++)
            {
                CID_Tab[i] = SD_ReadByte();
            }
        }
        /*!< Get CRC bytes (not really needed by us, but required by SD) */
        SD_WriteByte(SD_DUMMY_BYTE);
        SD_WriteByte(SD_DUMMY_BYTE);
        /*!< Set response value to success */
        rvalue = SD_RESPONSE_NO_ERROR;
    }
    /*!< SD chip select high */
    SD_CS_HIGH();
    /*!< Send dummy byte: 8 Clock pulses of delay */
    SD_WriteByte(SD_DUMMY_BYTE);

    /*!< Byte 0 */
    SD_cid->ManufacturerID = CID_Tab[0];

    /*!< Byte 1 */
    SD_cid->OEM_AppliID = CID_Tab[1] << 8;

    /*!< Byte 2 */
    SD_cid->OEM_AppliID |= CID_Tab[2];

    /*!< Byte 3 */
    SD_cid->ProdName1 = CID_Tab[3] << 24;

    /*!< Byte 4 */
    SD_cid->ProdName1 |= CID_Tab[4] << 16;

    /*!< Byte 5 */
    SD_cid->ProdName1 |= CID_Tab[5] << 8;

    /*!< Byte 6 */
    SD_cid->ProdName1 |= CID_Tab[6];

    /*!< Byte 7 */
    SD_cid->ProdName2 = CID_Tab[7];

    /*!< Byte 8 */
    SD_cid->ProdRev = CID_Tab[8];

    /*!< Byte 9 */
    SD_cid->ProdSN = CID_Tab[9] << 24;

    /*!< Byte 10 */
    SD_cid->ProdSN |= CID_Tab[10] << 16;

    /*!< Byte 11 */
    SD_cid->ProdSN |= CID_Tab[11] << 8;

    /*!< Byte 12 */
    SD_cid->ProdSN |= CID_Tab[12];

    /*!< Byte 13 */
    SD_cid->Reserved1 |= (CID_Tab[13] & 0xF0) >> 4;
    SD_cid->ManufactDate = (CID_Tab[13] & 0x0F) << 8;

    /*!< Byte 14 */
    SD_cid->ManufactDate |= CID_Tab[14];

    /*!< Byte 15 */
    SD_cid->CID_CRC = (CID_Tab[15] & 0xFE) >> 1;
    SD_cid->Reserved2 = 1;

    /*!< Return the reponse */
    return rvalue;
}

/**
* @brief  获取SD卡的信息.
* @param  SD_CardInfo 结构体指针
* @retval SD响应:
*         - SD_RESPONSE_FAILURE: 失败
*         - SD_RESPONSE_NO_ERROR: 成功
*/
SD_Error SD_GetCardInfo(SD_CardInfo *cardinfo)
{
    SD_Error status = SD_RESPONSE_FAILURE;

    //读取CSD寄存器
    status = SD_GetCSDRegister(&(cardinfo->SD_csd));
    //读取CID寄存器
    status = SD_GetCIDRegister(&(cardinfo->SD_cid));

    if ((SD_Type == SD_TYPE_V1) || (SD_Type == SD_TYPE_V2)) {
        //块数目基数： CSize + 1
        cardinfo->CardCapacity = (cardinfo->SD_csd.DeviceSize + 1) ;
        //块数目 = 块数目基数*块数目乘数。块数目乘数： 2的 (C_SIZE_MULT + 2)次方
        cardinfo->CardCapacity *= (1 << (cardinfo->SD_csd.DeviceSizeMul + 2));
        // 块大小：2的READ_BL_LEN 次方
        cardinfo->CardBlockSize = 1 << (cardinfo->SD_csd.RdBlockLen);
        //卡容量 = 块数目*块大小
        cardinfo->CardCapacity *= cardinfo->CardBlockSize;
    } else if (SD_Type == SD_TYPE_V2HC) { //SDHC卡
        cardinfo->CardCapacity = (uint64_t)(cardinfo->SD_csd.DeviceSize + 1) * 512 * 1024;
        cardinfo->CardBlockSize = 512;
    }

    /*!< 返回SD响应 */
    return status;
}

/**
* @brief  从SD卡读取一个数据块
* @param  pBuffer: 指针，用于存储读取到的数据
* @param  ReadAddr: 要读取的SD卡内部地址
* @param  BlockSize: 块大小
* @retval SD响应:
*         - SD_RESPONSE_FAILURE: 失败
*         - SD_RESPONSE_NO_ERROR: 成功
*/
SD_Error SD_ReadBlock(uint8_t* pBuffer, uint64_t ReadAddr, uint16_t BlockSize)
{
    uint32_t i = 0;
    SD_Error rvalue = SD_RESPONSE_FAILURE;

    //SDHC卡块大小固定为512，且读命令中的地址的单位是sector
    if (SD_Type == SD_TYPE_V2HC) {
        BlockSize = 512;
        ReadAddr /= 512;
    }

    /*!< 片选CS低电平*/
    SD_CS_LOW();

    /*!< 发送 CMD17 (SD_CMD_READ_SINGLE_BLOCK) 以读取一个数据块 */
    SD_SendCmd(SD_CMD_READ_SINGLE_BLOCK, ReadAddr, 0xFF);

    /*!< 检查R1响应 */
    if (!SD_GetResponse(SD_RESPONSE_NO_ERROR)) {
        /*!< 检查读取单个数据块的Token */
        if (!SD_GetResponse(SD_START_DATA_SINGLE_BLOCK_READ)) {
            /*!< 读取一个数据块的数据 :  NumByteToRead 个数据 */
            for (i = 0; i < BlockSize; i++) {
                /*!< 接收一个字节到pBuffer */
                *pBuffer = SD_ReadByte();

                /*!< 指针加1*/
                pBuffer++;
            }
            /*!< 读取 CRC 校验字节 (此处不校验，但SD卡有该流程，需要接收) */
            SD_ReadByte();
            SD_ReadByte();
            /*!< 设置返回值，表示成功接收到寄存器数据*/
            rvalue = SD_RESPONSE_NO_ERROR;
        }
    }
    /*!< 片选信号CS高电平 */
    SD_CS_HIGH();

    /*!< 发送 dummy 空字节: 8 时钟周期的延时 */
    SD_WriteByte(SD_DUMMY_BYTE);

    /*!< 返回接收状态 */
    return rvalue;
}

/**
* @brief  往SD卡写入一个数据块
* @param  pBuffer: 指针，指向将要写入的数据
* @param  WriteAddr: 要写入的SD卡内部地址
* @param  BlockSize: 块大小
* @retval SD响应:
*         - SD_RESPONSE_FAILURE: 失败
*         - SD_RESPONSE_NO_ERROR: 成功
*/
SD_Error SD_WriteBlock(uint8_t* pBuffer, uint64_t WriteAddr, uint16_t BlockSize)
{
    uint32_t i = 0;
    SD_Error rvalue = SD_RESPONSE_FAILURE;

    //SDHC卡块大小固定为512，且写命令中的地址的单位是sector
    if (SD_Type == SD_TYPE_V2HC) {
        BlockSize = 512;
        WriteAddr /= 512;
    }

    /*!< 片选CS低电平*/
    SD_CS_LOW();

    /*!< 发送 CMD24 (SD_CMD_WRITE_SINGLE_BLOCK) 以写入一个数据块 */
    SD_SendCmd(SD_CMD_WRITE_SINGLE_BLOCK, WriteAddr, 0xFF);

    /*!< 检查R1响应 */
    if (!SD_GetResponse(SD_RESPONSE_NO_ERROR)) {
        /*!< 发送 dummy 空字节 */
        SD_WriteByte(SD_DUMMY_BYTE);

        /*!< 发送 一个数据块起始Token表示 开始传输数据 */
        SD_WriteByte(0xFE);

        /*!< 写入一个数据块的数据 */
        for (i = 0; i < BlockSize; i++) {
            /*!< 发送指针指向的字节 */
            SD_WriteByte(*pBuffer);
            /*!< 指针加1 */
            pBuffer++;
        }
        /*!< 两个字节的空CRC校验码，默认不验证 */
        SD_ReadByte();
        SD_ReadByte();

        /*!< 读取SD卡数据响应 */
        if (SD_GetDataResponse() == SD_DATA_OK) {
            rvalue = SD_RESPONSE_NO_ERROR;
        }
    }
    /*!< 片选信号CS高电平 */
    SD_CS_HIGH();
    /*!< 发送 dummy 空字节: 8 时钟周期的延时 */
    SD_WriteByte(SD_DUMMY_BYTE);

    /*!< 返回接收状态 */
    return rvalue;
}

/**
  * @brief  从SD卡读取多个数据块
  * @param  pBuffer: 接收读取数据的缓冲区指针
  * @param  ReadAddr: SD卡内部读取起始地址（单位：字节/扇区）
  * @param  BlockSize: SD卡数据块大小（SDHC卡自动处理为512字节）
  * @param  NumberOfBlocks: 需要读取的块数量
  * @retval SD卡响应状态：
  *         - SD_RESPONSE_FAILURE: 操作失败
  *         - SD_RESPONSE_NO_ERROR: 操作成功
  */
SD_Error SD_ReadMultiBlocks(uint8_t* pBuffer, uint64_t ReadAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
    uint32_t i = 0, Offset = 0;
    SD_Error rvalue = SD_RESPONSE_FAILURE;

    /* SDHC卡固定块大小为512字节，地址按扇区计算 */
    if (SD_Type == SD_TYPE_V2HC)
    {
        BlockSize = 512;
        ReadAddr /= 512;  // 将字节地址转换为扇区地址
    }

    /* 拉低SD卡片选信号 */
    SD_CS_LOW();

    /* 数据传输过程 */
    while (NumberOfBlocks--)
    {
        /* 发送CMD17命令（单块读取） */
        SD_SendCmd(SD_CMD_READ_SINGLE_BLOCK, ReadAddr + Offset, 0xFF);

        /* 检查读块命令的R1响应（0x00表示无错误） */
        if (SD_GetResponse(SD_RESPONSE_NO_ERROR))
        {
            return SD_RESPONSE_FAILURE;
        }

        /* 等待数据起始令牌（0xFE） */
        if (!SD_GetResponse(SD_START_DATA_SINGLE_BLOCK_READ))
        {
            /* 读取块数据内容 */
            for (i = 0; i < BlockSize; i++)
            {
                *pBuffer = SD_ReadByte();  // 读取当前字节
                pBuffer++;                 // 移动缓冲区指针
            }

            /* 更新下一个读取地址偏移 */
            Offset += 512;

            /* 读取CRC校验字节（协议要求但不使用） */
            SD_ReadByte();
            SD_ReadByte();

            rvalue = SD_RESPONSE_NO_ERROR;
        }
        else
        {
            rvalue = SD_RESPONSE_FAILURE;
        }

        /* 发送填充字节防止读操作失败 */
        SD_WriteByte(SD_DUMMY_BYTE);
    }

    /* 释放SD卡片选信号 */
    SD_CS_HIGH();

    /* 发送填充字节提供8个时钟周期延迟 */
    SD_WriteByte(SD_DUMMY_BYTE);

    return rvalue;
}

/**
* @brief 向SD卡写入多个数据块
* @param pBuffer: 指向待写入数据缓冲区的指针
* @param WriteAddr: 写入起始地址
* @param BlockSize: SD卡数据块大小
* @param NumberOfBlocks: 要写入的块数
* @retval SD卡响应状态:
* - SD_RESPONSE_FAILURE: 操作失败
* - SD_RESPONSE_NO_ERROR: 操作成功
*/
SD_Error SD_WriteMultiBlocks(uint8_t *pBuffer, uint64_t WriteAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
    uint32_t i = 0, Offset = 0;
    SD_Error rvalue = SD_RESPONSE_FAILURE;

// SDHC卡块大小固定为512字节，且写命令中的地址单位是扇区
    if (SD_Type == SD_TYPE_V2HC)
    {
        BlockSize = 512;
        WriteAddr /= 512;
    }
     /*!< 拉低SD卡片选信号 */
    SD_CS_LOW();
     /*!< 数据传输 */
    while (NumberOfBlocks--)
    {
        /*!< 发送CMD24命令（SD_CMD_WRITE_SINGLE_BLOCK）写入块 */
        SD_SendCmd(SD_CMD_WRITE_SINGLE_BLOCK, WriteAddr + Offset, 0xFF);
        /*!< 检查SD卡是否响应写块命令：R1响应（0x00表示无错误） */
        if (SD_GetResponse(SD_RESPONSE_NO_ERROR))
        {
            return SD_RESPONSE_FAILURE;
        }
        /*!< 发送填充字节 */
        SD_WriteByte(SD_DUMMY_BYTE);
        /*!< 发送数据令牌表示数据开始 */
        SD_WriteByte(SD_START_DATA_SINGLE_BLOCK_WRITE);
        /*!< 向SD卡写入块数据：按块大小循环写入数据 */
        for (i = 0; i < BlockSize; i++)
        {
        /*!< 发送当前指针指向的字节 */
            SD_WriteByte(*pBuffer);
        /*!< 指针移动到下一个待写入字节位置 */
            pBuffer++;
        }
        /*!< 更新下一个写入地址偏移 */
        Offset += 512;
        /*!< 写入CRC校验字节（实际不使用，但SD协议要求） */
        SD_ReadByte();
        SD_ReadByte();
        /*!< 读取数据响应 */
        if (SD_GetDataResponse() == SD_DATA_OK)
        {
        /*!< 设置操作成功标志 */
            rvalue = SD_RESPONSE_NO_ERROR;
        } else
        {
        /*!< 设置操作失败标志 */
            rvalue = SD_RESPONSE_FAILURE;
        }
    }
    /*!< 拉高SD卡片选信号 */
    SD_CS_HIGH();
    /*!< 发送填充字节：提供8个时钟周期的延迟 */
    SD_WriteByte(SD_DUMMY_BYTE);
    /*!< 返回最终操作状态 */
    return rvalue;
}

/**
* @brief  向SD卡发送一个字节.
* @param  Data: 要发送的数据.
* @retval 接收到的数据
*/
uint8_t SD_WriteByte(uint8_t Data)
{

    uint8_t rx_data=0xFF;
    // 全双工同步传输
    HAL_SPI_TransmitReceive(&hspi2, &Data, &rx_data, 1, HAL_MAX_DELAY);
    return rx_data;
}

/**
* @brief  从SD卡读取一个字节
* @param  None
* @retval 接收到的数据
*/
uint8_t SD_ReadByte(void)
{

    uint8_t tx_dummy = SD_DUMMY_BYTE;  // 建议定义为0xFF
    uint8_t rx_data = 0xFF;

    HAL_SPI_TransmitReceive(&hspi2, &tx_dummy, &rx_data, 1, HAL_MAX_DELAY);
    return rx_data;
}
/**
* @brief  发送SD命令
* @param  Cmd: 要发送的命令
* @param  Arg: 命令参数
* @param  Crc: CRC校验码.
* @retval None
*/
void SD_SendCmd(uint8_t Cmd, uint32_t Arg, uint8_t Crc)
{
    uint32_t i = 0x00;

    uint8_t Frame[6];

    Frame[0] = (Cmd | 0x40); /*!< Construct byte 1 */

    Frame[1] = (uint8_t)(Arg >> 24); /*!< Construct byte 2 */

    Frame[2] = (uint8_t)(Arg >> 16); /*!< Construct byte 3 */

    Frame[3] = (uint8_t)(Arg >> 8); /*!< Construct byte 4 */

    Frame[4] = (uint8_t)(Arg); /*!< Construct byte 5 */

    Frame[5] = (Crc); /*!< Construct CRC: byte 6 */

    for (i = 0; i < 6; i++) {
        SD_WriteByte(Frame[i]); /*!< Send the Cmd bytes */
    }
}

/**
* @brief  获取SD卡的的响应
* @param  要检查的响应类型
* @retval SD响应:
*         - SD_RESPONSE_FAILURE: 失败
*         - SD_RESPONSE_NO_ERROR: 成功
*/
SD_Error SD_GetResponse(uint8_t Response)
{
    uint32_t Count = 0xFFF;

    /*!< 检查是否接收到 response 表示的响应 */
    while ((SD_ReadByte() != Response) && Count) {
        Count--;
    }
    if (Count == 0) {
        /*!< 检查超时 */
        return SD_RESPONSE_FAILURE;
    } else {
        /*!< 得到response表示的响应 */
        return SD_RESPONSE_NO_ERROR;
    }
}
/**
* @brief  获取SD卡的数据响应.
* @param  None
* @retval 返回响应状态 status: 读取到的数据响应 xxx0<status>1
*         - status 010: 接受数据
*         - status 101: CRC校验错误，拒绝数据
*         - status 110: 写入错误，拒绝数据
*         - status 111: 其它错误，拒绝数据
*/
uint8_t SD_GetDataResponse(void)
{
    uint32_t i = 0;
    uint8_t response, rvalue;

    while (i <= 64) {
        /*!< 读取响应 */
        response = SD_ReadByte();
        /*!< 屏蔽无关的数据位（前三位xxx） */
        response &= 0x1F;
        switch (response) {
            case SD_DATA_OK: {
                rvalue = SD_DATA_OK;
                break;
            }
            case SD_DATA_CRC_ERROR:
                return SD_DATA_CRC_ERROR;
            case SD_DATA_WRITE_ERROR:
                return SD_DATA_WRITE_ERROR;
            default: {
                rvalue = SD_DATA_OTHER_ERROR;
                break;
            }
        }
        /*!< 数据正常，退出循环 */
        if (rvalue == SD_DATA_OK)
            break;
        /*!< Increment loop counter */
        i++;
    }

    /*!< 等待空数据 */
    while (SD_ReadByte() == 0);

    /*!< 返回响应 */
    return response;
}
/***************测试函数********************/
/**
* @brief  测试对SD卡的单块读写操作
* @param  None
* @retval None
*/
uint32_t SD_SingleBlockTest(void)
{
    const uint32_t BLOCK_SIZE = 512; // 定义块大小
    uint8_t Buffer_Block_Tx[BLOCK_SIZE]; // 发送缓冲区
    uint8_t Buffer_Block_Rx[BLOCK_SIZE]; // 接收缓冲区
    SD_Error SD_WriteBlock_Status, SD_ReadBlock_Status;
    uint32_t TransferStatus = 66;

    /*------------------- 填充发送缓冲区 --------------------*/
    // 使用 0x320F 的字节模式填充（0x32 高字节，0x0F 低字节）
    for (uint32_t i = 0; i < BLOCK_SIZE; i += 2) {
        Buffer_Block_Tx[i] = 0x32;    // 填充高字节
        Buffer_Block_Tx[i + 1] = 0x0F; // 填充低字节
    }

    /*------------------- 块写入操作 ------------------------*/
    SD_WriteBlock_Status = SD_WriteBlock(Buffer_Block_Tx, 0x00, BLOCK_SIZE);
    if (SD_WriteBlock_Status != SD_RESPONSE_NO_ERROR) {
        return SD_WriteBlock_Status; // 返回写入错误代码
    }

    /*------------------- 块读取操作 ------------------------*/
    SD_ReadBlock_Status = SD_ReadBlock(Buffer_Block_Rx, 0x00, BLOCK_SIZE);
    if (SD_ReadBlock_Status != SD_RESPONSE_NO_ERROR) {
        return SD_ReadBlock_Status; // 返回读取错误代码
    }

    /*------------------- 数据校验 --------------------------*/
    for (uint32_t i = 0; i < BLOCK_SIZE; i++) {
        if (Buffer_Block_Tx[i] != Buffer_Block_Rx[i]) {
            TransferStatus = 99; // 标记数据不一致
            break;
        }
    }

    return TransferStatus; // 66=成功，99=校验失败


}
/**
  * @brief  SD卡多块读写测试（默认读写2个块）
  * @param  BlockCount: 要测试的块数量（默认2块）
  * @param  StartAddr: 起始地址（默认0x00）
  * @retval 测试状态码：
  *         - 0: 测试成功
  *         - 1~99: 错误代码（SD操作错误或数据校验失败）
  *         - 66: 初始状态/未执行校验
  */
uint32_t SD_MultiBlockTest(void)
{
    uint32_t BlockCount = 2;
    uint64_t StartAddr = 0x00;
    const uint32_t BLOCK_SIZE = 512;     // 固定块大小
    const uint32_t TOTAL_SIZE = BLOCK_SIZE * BlockCount; // 总数据量

    uint8_t Buffer_Tx[TOTAL_SIZE]; // 发送缓冲区
    uint8_t Buffer_Rx[TOTAL_SIZE]; // 接收缓冲区
    SD_Error writeStatus, readStatus;
    uint32_t TransferStatus = 66;        // 初始状态码

    /*==================== 数据生成阶段 ====================*/
    // 采用棋盘式数据模式（0xAA/0x55交替）增强测试有效性
    for(uint32_t i=0; i<TOTAL_SIZE; i++) {
        Buffer_Tx[i] = (i % 2 == 0) ? 0xAA : 0x55; // 偶数位0xAA，奇数位0x55
    }

    /*==================== 多块写入操作 ====================*/
    writeStatus = SD_WriteMultiBlocks(Buffer_Tx, StartAddr, BLOCK_SIZE, BlockCount);
    if(writeStatus != SD_RESPONSE_NO_ERROR) {
        return (uint32_t)writeStatus; // 返回具体错误码
    }

    /*==================== 多块读取操作 ====================*/
    readStatus = SD_ReadMultiBlocks(Buffer_Rx, StartAddr, BLOCK_SIZE, BlockCount);
    if(readStatus != SD_RESPONSE_NO_ERROR) {
        return (uint32_t)readStatus + 10; // 返回读取错误码+10以区分写入错误
    }

    /*==================== 数据校验阶段 ====================*/
    for(uint32_t i=0; i<TOTAL_SIZE; i++) {
        if(Buffer_Tx[i] != Buffer_Rx[i]) {
            TransferStatus = 99; // 数据校验失败
            // 可选：记录首次错误位置
            // printf("Data mismatch at offset 0x%08X: TX=0x%02X RX=0x%02X\n",
            //        i, Buffer_Tx[i], Buffer_Rx[i]);
            break;
        }
    }

    return (TransferStatus == 99) ? 99 : 0; // 返回最终状态
}