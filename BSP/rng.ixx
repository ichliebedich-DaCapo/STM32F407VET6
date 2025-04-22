// rng.ixx
module;
// ==================包含头文件====================
#include <project_config.h>
// ==================包含头文件====================

export module rng;  // 声明模块

export namespace bsp::rng {
    // 硬件句柄
    inline RNG_HandleTypeDef hrng = {
        .Instance = RNG,
        .State = HAL_RNG_STATE_RESET
    };

    void init() noexcept;          ///< 初始化硬件随机数生成器
    uint32_t get_random() noexcept; ///< 获取32位随机数
    void check_status() noexcept;  ///< 检查并清除错误状态
}



// ================== 实现导出的函数 ==================
/**
 * @brief 初始化RNG外设
 * @details 使能RNG时钟并配置硬件，失败时触发错误处理
 * @warning 需确保project_config.h已包含必要的HAL库定义
 */
void bsp::rng::init() noexcept {
    __HAL_RCC_RNG_CLK_ENABLE();
    if (HAL_RNG_Init(&hrng) != HAL_OK) {
        Error_Handler();  // 需在project_config.h中声明
    }
}

/**
 * @brief 生成并返回随机数
 * @return 成功返回有效随机数，失败返回0
 * @note 生成失败时自动清除SEIS/CECIS错误标志
 */
uint32_t bsp::rng::get_random() noexcept {
    uint32_t value = 0;
    const HAL_StatusTypeDef status = HAL_RNG_GenerateRandomNumber(&hrng, &value);
    check_status();  // 自动检查错误状态
    return (status == HAL_OK) ? value : 0;
}

/**
 * @brief 检查并清除硬件错误状态
 * @details 处理种子错误(SECS)和时钟错误(CECS)
 */
void bsp::rng::check_status() noexcept {
    if (__HAL_RNG_GET_FLAG(&hrng, RNG_FLAG_SECS)) {
        __HAL_RNG_CLEAR_FLAG(&hrng, RNG_FLAG_SECS);
    }
    if (__HAL_RNG_GET_FLAG(&hrng, RNG_FLAG_CECS)) {
        __HAL_RNG_CLEAR_FLAG(&hrng, RNG_FLAG_CECS);
    }
}