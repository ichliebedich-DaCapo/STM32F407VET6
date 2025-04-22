// debug.ixx
module;
// ================== 包含底层硬件头文件 ==================
#include <project_config.h>
#include "core_cm4.h"  // 确保包含 Cortex-M4 内核头文件

export module debug;  // 声明模块

export namespace bsp::debug {
    // 常量定义 ----------------------------------------------------
    constexpr uint32_t ITM_UNLOCK_KEY = 0xC5ACCE55;  ///< ITM寄存器解锁密钥

    // 位域位置定义 (与CMSIS-Core保持一致)
    constexpr uint32_t DBGMCU_TRACE_ENABLE = DBGMCU_CR_TRACE_IOEN;
    constexpr uint32_t COREDEBUG_TRACE_ENABLE = CoreDebug_DEMCR_TRCENA_Msk;
    constexpr uint32_t ITM_TCR_TXENA_Msk =  (1 << 3);

    // TPIU异步模式配置
    constexpr uint32_t SWO_BAUD_DIVISOR = 83;  ///< 84MHz时钟下波特率为1MHz
    constexpr uint32_t TPIU_ASYNC_PROTOCOL = 0x2; ///< 异步UART模式

    // 函数声明 ----------------------------------------------------
    void init_trace() noexcept;// 初始化ITM和SWO调试接口

    /// @brief 通过ITM端口发送单个字符
    /// @param ch 要发送的ASCII字符
    void send_char(char ch) noexcept;
}

// ================== 模块内部实现 ==================
namespace {
    // ITM端口启用掩码
    constexpr uint32_t ITM_PORT0_ENABLE = 1UL << 0;

}

// ================== 实现导出函数 ==================
/// @brief 初始化ITM和SWO调试接口
/// @note 配置流程：
/// 1. 启用调试模块时钟
/// 2. 配置TPIU协议和波特率
/// 3. 解锁并启用ITM模块
void bsp::debug::init_trace() noexcept {
    // 启用调试模块时钟
    DBGMCU->CR |= DBGMCU_TRACE_ENABLE;
    CoreDebug->DEMCR |= COREDEBUG_TRACE_ENABLE;

    // 配置TPIU协议参数
    TPI->ACPR = SWO_BAUD_DIVISOR;    // 波特率 = 主频/(ACPR+1)
    TPI->SPPR = TPIU_ASYNC_PROTOCOL; // 选择异步UART模式
    TPI->FFCR = 0x00;                // 禁用数据包格式化

    // 解锁并配置ITM
    ITM->LAR = ITM_UNLOCK_KEY;       // 解除ITM写保护
    ITM->TCR = ITM_TCR_ITMENA_Msk |  // 启用ITM
               ITM_TCR_SYNCENA_Msk | // 启用同步时间戳
               ITM_TCR_TXENA_Msk;    // 启用全局传输
    ITM->TER = ITM_PORT0_ENABLE;     // 启用端口0
}

// ================== 标准输出重定向 ==================
namespace {
    struct StdoutRedirector {
        // 重定向标准输出到ITM
        static int write(int fd, const char* buf, int len) noexcept {
            for (int i=0; i<len; ++i) {
                ITM_SendChar(buf[i]);  // 使用 ITM_SendChar 发送字符
            }
            return len;
        }
    };
}

// 替换标准库的_write实现
extern "C" int _write(int fd, char* buf, int len) {
    return StdoutRedirector::write(fd, buf, len);
}