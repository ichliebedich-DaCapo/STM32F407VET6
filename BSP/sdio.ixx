// sdio.ixx
module;
// ==================包含头文件====================
#include <project_config.h>
// ==================包含头文件====================

export module sdio;  // 声明模块

export namespace bsp::sdio {
    // HAL句柄导出
    inline SD_HandleTypeDef hsd;         ///< SDIO主句柄
    inline DMA_HandleTypeDef hdma_sdio;   ///< SDIO DMA句柄

    void init() noexcept;  ///< 初始化SDIO接口
}

// ================== 模块内部实现 ==================
namespace {
    // GPIO配置参数（保持与原始代码一致）
    constexpr GPIO_InitTypeDef sdio_gpio_config = {
        .Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12,
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
        .Alternate = GPIO_AF12_SDIO
    };

    // 命令线GPIO配置
    constexpr GPIO_InitTypeDef cmd_gpio_config = {
        .Pin = GPIO_PIN_2,
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
        .Alternate = GPIO_AF12_SDIO
    };
}

// ================== 实现导出的函数 ==================
/**
 * @brief 初始化SDIO接口
 * @details 完整配置流程：
 *          1. 启用GPIO和DMA时钟
 *          2. 配置数据线和命令线GPIO
 *          3. 初始化DMA流
 *          4. 配置SDIO中断
 *          5. 初始化SDIO外设
 * @warning 需确保project_config.h已正确配置时钟树
 */
void bsp::sdio::init() noexcept {
    // GPIO时钟使能
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    // 数据线GPIO初始化
    HAL_GPIO_Init(GPIOC, &sdio_gpio_config);
    
    // 命令线GPIO初始化
    HAL_GPIO_Init(GPIOD, &cmd_gpio_config);

    // DMA初始化
    __HAL_RCC_DMA2_CLK_ENABLE();
    hdma_sdio.Instance = DMA2_Stream3;
    hdma_sdio.Init = {
        .Channel = DMA_CHANNEL_4,
        .Direction = DMA_PERIPH_TO_MEMORY,
        .PeriphInc = DMA_PINC_DISABLE,
        .MemInc = DMA_MINC_ENABLE,
        .PeriphDataAlignment = DMA_PDATAALIGN_WORD,
        .MemDataAlignment = DMA_MDATAALIGN_WORD,
        .Mode = DMA_PFCTRL,
        .Priority = DMA_PRIORITY_LOW,
        .FIFOMode = DMA_FIFOMODE_ENABLE,
        .FIFOThreshold = DMA_FIFO_THRESHOLD_FULL,
        .MemBurst = DMA_MBURST_INC4,
        .PeriphBurst = DMA_PBURST_INC4
    };
    if (HAL_DMA_Init(&hdma_sdio) != HAL_OK) {
        Error_Handler();
    }

    // 中断配置
    HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
    HAL_NVIC_SetPriority(SDIO_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(SDIO_IRQn);

    // 链接DMA到SDIO句柄
    __HAL_LINKDMA(&hsd, hdmarx, hdma_sdio);
    __HAL_LINKDMA(&hsd, hdmatx, hdma_sdio);

    // SDIO外设初始化
    __HAL_RCC_SDIO_CLK_ENABLE();
    hsd.Instance = SDIO;
    hsd.Init = {
        .ClockEdge = SDIO_CLOCK_EDGE_RISING,
        .ClockBypass = SDIO_CLOCK_BYPASS_DISABLE,
        .ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE,
        .BusWide = SDIO_BUS_WIDE_4B,
        .HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE,
        .ClockDiv = 4
    };
    if (HAL_SD_Init(&hsd) != HAL_OK || 
        HAL_SD_ConfigWideBusOperation(&hsd, SDIO_BUS_WIDE_4B) != HAL_OK) {
        Error_Handler();
    }
}

// ================== 中断处理连接 ==================
extern "C" {
    /**
     * @brief SDIO中断服务函数
     * @warning 必须保持C链接规范
     */
    void SDIO_IRQHandler() {
        HAL_SD_IRQHandler(&bsp::sdio::hsd);
    }

    /**
     * @brief DMA2 Stream3中断服务函数
     */
    void DMA2_Stream3_IRQHandler() {
        HAL_DMA_IRQHandler(&bsp::sdio::hdma_sdio);
    }
}