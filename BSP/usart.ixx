module;
// ==================包含头文件====================
#include <project_config.h>
// ==================包含头文件====================

export module usart;  // 声明模块

#include <cstdio>  // 标准IO支持

export namespace bsp::usart {
    // HAL句柄导出
    inline UART_HandleTypeDef huart1;          ///< USART1主句柄
    inline DMA_HandleTypeDef hdma_usart1_rx;    ///< 接收DMA句柄
    inline DMA_HandleTypeDef hdma_usart1_tx;    ///< 发送DMA句柄

    void init() noexcept;  ///< 初始化USART1接口
}

// ================== 模块内部实现 ==================


// ================== 实现导出的函数 ==================
/**
 * @brief 初始化USART1接口
 * @details 完整配置流程：
 *          1. 启用GPIO和DMA时钟
 *          2. 配置TX/RX引脚
 *          3. 初始化DMA通道（如果启用DMA_USART_ENABLE）
 *          4. 配置USART参数（115200bps,8N1）
 *          5. 使能相关中断
 * @warning 需在project_config.h中定义DMA_USART_ENABLE以启用DMA模式
 */
void bsp::usart::init() noexcept {
    // GPIO时钟使能
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef usart_gpio_config = {
        .Pin = GPIO_PIN_9 | GPIO_PIN_10,
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
        .Alternate = GPIO_AF7_USART1
    };
    HAL_GPIO_Init(GPIOA, &usart_gpio_config);

    // DMA时钟使能
    __HAL_RCC_DMA2_CLK_ENABLE();
    HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
    HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);

#ifdef DMA_USART_ENABLE
    // DMA接收配置
    hdma_usart1_rx.Instance = DMA2_Stream2;
    hdma_usart1_rx.Init = {
        .Channel = DMA_CHANNEL_4,
        .Direction = DMA_PERIPH_TO_MEMORY,
        .PeriphInc = DMA_PINC_DISABLE,
        .MemInc = DMA_MINC_ENABLE,
        .PeriphDataAlignment = DMA_PDATAALIGN_BYTE,
        .MemDataAlignment = DMA_MDATAALIGN_BYTE,
        .Mode = DMA_NORMAL,
        .Priority = DMA_PRIORITY_LOW,
        .FIFOMode = DMA_FIFOMODE_DISABLE
    };
    HAL_DMA_Init(&hdma_usart1_rx);
    __HAL_LINKDMA(&huart1, hdmarx, hdma_usart1_rx);

    // DMA发送配置
    hdma_usart1_tx.Instance = DMA2_Stream7;
    hdma_usart1_tx.Init = {
        .Channel = DMA_CHANNEL_4,
        .Direction = DMA_MEMORY_TO_PERIPH,
        .PeriphInc = DMA_PINC_DISABLE,
        .MemInc = DMA_MINC_ENABLE,
        .PeriphDataAlignment = DMA_PDATAALIGN_BYTE,
        .MemDataAlignment = DMA_MDATAALIGN_BYTE,
        .Mode = DMA_NORMAL,
        .Priority = DMA_PRIORITY_LOW,
        .FIFOMode = DMA_FIFOMODE_ENABLE
    };
    HAL_DMA_Init(&hdma_usart1_tx);
    __HAL_LINKDMA(&huart1, hdmatx, hdma_usart1_tx);
#endif

    // USART参数配置
    __HAL_RCC_USART1_CLK_ENABLE();
    huart1.Instance = USART1;
    huart1.Init = {
        .BaudRate = 115200,
        .WordLength = UART_WORDLENGTH_8B,
        .StopBits = UART_STOPBITS_1,
        .Parity = UART_PARITY_NONE,
        .Mode = UART_MODE_TX_RX,
        .HwFlowCtl = UART_HWCONTROL_NONE,
        .OverSampling = UART_OVERSAMPLING_16
    };
    HAL_UART_Init(&huart1);

    // 中断配置
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
}

// ================== 中断处理连接 ==================
extern "C" {
    /**
     * @brief USART1中断服务函数
     */
    void USART1_IRQHandler() {
        HAL_UART_IRQHandler(&bsp::usart::huart1);
    }

    /**
     * @brief DMA接收流中断服务函数
     */
    void DMA2_Stream2_IRQHandler() {
        HAL_DMA_IRQHandler(&bsp::usart::hdma_usart1_rx);
    }

    /**
     * @brief DMA发送流中断服务函数
     */
    void DMA2_Stream7_IRQHandler() {
        HAL_DMA_IRQHandler(&bsp::usart::hdma_usart1_tx);
    }
}

// ================== 标准输出重定向 ==================
extern "C" {
    /**
     * @brief GCC标准输出重定向
     */
    int _write(int fd, char *ptr, int len) {
        HAL_UART_Transmit(&bsp::usart::huart1, reinterpret_cast<uint8_t *>(ptr), len, 0xFFFF);
        return len;
    }

    /**
     * @brief 标准库字符输出重定向
     */
    int fputc(int ch, FILE *f) {
        HAL_UART_Transmit(&bsp::usart::huart1, reinterpret_cast<uint8_t *>(&ch), 1, 0xFFFF);
        return ch;
    }
}