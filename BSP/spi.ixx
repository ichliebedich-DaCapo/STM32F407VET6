module;
// ==================包含头文件====================
#include <project_config.h>
// ==================包含头文件====================

export module spi; // 声明模块

export namespace bsp::spi
{
    // HAL句柄导出
    inline SPI_HandleTypeDef hspi2; ///< SPI2主句柄
    inline SPI_HandleTypeDef hspi3; ///< SPI3主句柄
    inline DMA_HandleTypeDef hdma_spi2_tx; ///< SPI2发送DMA句柄
    inline DMA_HandleTypeDef hdma_spi2_rx; ///< SPI2接收DMA句柄

    void init_spi2() noexcept; ///< 初始化SPI2接口
    void init_spi3() noexcept; ///< 初始化SPI3接口
    void cs_low() noexcept; ///< 拉低SPI2片选
    void cs_high() noexcept; ///< 拉高SPI2片选
    void send_byte(uint8_t data) noexcept; ///< 单字节传输
    void send_bytes(uint8_t *data, uint32_t len) noexcept; ///< 多字节传输

#ifdef DMA_SPI_ENABLE
    void dma_init() noexcept;        ///< DMA中断初始化
#endif
}

// ================== 模块内部实现 ==================

// ================== 实现导出的函数 ==================
/**
 * @brief 初始化SPI3接口
 * @details 配置为全双工主模式，软件片选
 *          SCK=42MHz (APB1时钟84MHz / 2分频)
 */
void bsp::spi::init_spi3() noexcept
{
    __HAL_RCC_SPI3_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // SPI3 GPIO配置参数
    GPIO_InitTypeDef spi3_gpio_config = {
        .Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12,
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
        .Alternate = GPIO_AF6_SPI3
    };
    HAL_GPIO_Init(GPIOC, &spi3_gpio_config);

    // SPI参数配置
    hspi3.Instance = SPI3;
    hspi3.Init = {
        .Mode = SPI_MODE_MASTER,
        .Direction = SPI_DIRECTION_2LINES,
        .DataSize = SPI_DATASIZE_8BIT,
        .CLKPolarity = SPI_POLARITY_LOW,
        .CLKPhase = SPI_PHASE_1EDGE,
        .NSS = SPI_NSS_SOFT,
        .BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2,
        .FirstBit = SPI_FIRSTBIT_MSB,
        .TIMode = SPI_TIMODE_DISABLE,
        .CRCCalculation = SPI_CRCCALCULATION_DISABLE,
        .CRCPolynomial = 10
    };

    if (HAL_SPI_Init(&hspi3) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief 初始化SPI2接口
 * @details 配置为全双工主模式，硬件片选引脚PB12
 *          SCK=42MHz (APB1时钟84MHz / 2分频)
 */
void bsp::spi::init_spi2() noexcept
{
    __HAL_RCC_SPI2_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // SPI2数据线配置
    GPIO_InitTypeDef spi2_data_gpio = {
        .Pin = GPIO_PIN_2 | GPIO_PIN_3,
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
        .Alternate = GPIO_AF5_SPI2
    };
    HAL_GPIO_Init(GPIOC, &spi2_data_gpio);

    // SPI2时钟线配置
    GPIO_InitTypeDef spi2_clk_gpio = {
        .Pin = GPIO_PIN_10,
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
        .Alternate = GPIO_AF5_SPI2
    };
    HAL_GPIO_Init(GPIOB, &spi2_clk_gpio);

    // 片选引脚初始化
    GPIO_InitTypeDef cs_gpio = {
        .Pin = GPIO_PIN_12,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_VERY_HIGH
    };
    HAL_GPIO_Init(GPIOB, &cs_gpio);
    cs_high(); // 初始状态拉高片选

    // SPI参数配置
    hspi2.Instance = SPI2;
    hspi2.Init = {
        .Mode = SPI_MODE_MASTER,
        .Direction = SPI_DIRECTION_2LINES,
        .DataSize = SPI_DATASIZE_8BIT,
        .CLKPolarity = SPI_POLARITY_LOW,
        .CLKPhase = SPI_PHASE_1EDGE,
        .NSS = SPI_NSS_SOFT,
        .BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2,
        .FirstBit = SPI_FIRSTBIT_MSB,
        .TIMode = SPI_TIMODE_DISABLE,
        .CRCCalculation = SPI_CRCCALCULATION_DISABLE,
        .CRCPolynomial = 10
    };

    if (HAL_SPI_Init(&hspi2) != HAL_OK)
    {
        Error_Handler();
    }

#ifdef DMA_SPI_ENABLE
    __HAL_RCC_DMA1_CLK_ENABLE();

    HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);

    HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);

    // DMA发送配置
    hdma_spi2_tx.Instance = DMA1_Stream4;
    hdma_spi2_tx.Init = {
        .Channel = DMA_CHANNEL_0,
        .Direction = DMA_MEMORY_TO_PERIPH,
        .PeriphInc = DMA_PINC_DISABLE,
        .MemInc = DMA_MINC_ENABLE,
        .PeriphDataAlignment = DMA_PDATAALIGN_BYTE,// 接收半字
        .MemDataAlignment = DMA_MDATAALIGN_BYTE,// 跟字节序有关
        .Mode = DMA_NORMAL,
        .Priority = DMA_PRIORITY_HIGH,
        .FIFOMode = DMA_FIFOMODE_DISABLE
    };
    HAL_DMA_Init(&hdma_spi2_tx);

    // DMA接收配置
    hdma_spi2_rx.Instance = DMA1_Stream3;
    hdma_spi2_rx.Init = {
        .Channel = DMA_CHANNEL_0,
        .Direction = DMA_PERIPH_TO_MEMORY,
        .PeriphInc = DMA_PINC_DISABLE,
        .MemInc = DMA_MINC_ENABLE,
        .PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD,// 接收半字
        .MemDataAlignment = DMA_MDATAALIGN_HALFWORD,
        .Mode = DMA_NORMAL,
        .Priority = DMA_PRIORITY_MEDIUM,
        .FIFOMode = DMA_FIFOMODE_DISABLE
    };
    HAL_DMA_Init(&hdma_spi2_rx);

    __HAL_LINKDMA(&hspi2, hdmatx, hdma_spi2_tx);
    __HAL_LINKDMA(&hspi2, hdmarx, hdma_spi2_rx);

//    dma_init();
    __HAL_SPI_DISABLE_IT(&hspi2, SPI_IT_TXE);
    __HAL_DMA_ENABLE_IT(&hdma_spi2_rx, DMA_IT_TC);

#endif


}

/**
 * @brief 拉低SPI2片选信号
 */
void bsp::spi::cs_low() noexcept
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
}

/**
 * @brief 拉高SPI2片选信号
 */
void bsp::spi::cs_high() noexcept
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
}

/**
 * @brief SPI2单字节阻塞传输
 * @param data 要发送的字节数据
 */
void bsp::spi::send_byte(uint8_t data) noexcept
{
    HAL_SPI_Transmit(&hspi2, &data, 1, HAL_MAX_DELAY);
}

/**
 * @brief SPI2多字节阻塞传输
 * @param data 数据缓冲区指针
 * @param len 数据长度
 */
void bsp::spi::send_bytes(uint8_t *data, uint32_t len) noexcept
{
    HAL_SPI_Transmit(&hspi2, data, len, HAL_MAX_DELAY);
}

#ifdef DMA_SPI_ENABLE
/**
 * @brief 初始化DMA中断
 * @details 配置DMA流的NVIC中断优先级
 */
void bsp::spi::dma_init() noexcept {
    __HAL_RCC_DMA1_CLK_ENABLE();

    HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);

    HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
}
#endif

// ================== 中断处理连接 ==================
extern "C"
{
/**
 * @brief DMA接收流中断服务函数
 */
#ifdef DMA_SPI_ENABLE
void DMA1_Stream3_IRQHandler()
{
    HAL_DMA_IRQHandler(&bsp::spi::hdma_spi2_rx);
}

/**
 * @brief DMA发送流中断服务函数
 */
void DMA1_Stream4_IRQHandler()
{
    HAL_DMA_IRQHandler(&bsp::spi::hdma_spi2_tx);
}
#endif
}
