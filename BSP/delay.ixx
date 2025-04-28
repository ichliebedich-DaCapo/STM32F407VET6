module; // 全局模块片段用于包含C头文件
// ==================包含头文件====================
#include <project_config.h>
// ==================包含头文件====================
export module delay; // 声明模块

// ================== 导出函数 ====================
export namespace bsp::delay
{
    void us(uint16_t us_) noexcept;

    void ms(uint32_t ms_) noexcept;

}



namespace bsp::delay
{
    void us(const uint16_t us_) noexcept
    {
        const uint16_t delay_ticks = us_ << 2; // 计算所需计数（4MHz→1µs=4ticks）
        const uint16_t start = TIM7->CNT; // 直接读取16位计数器

        // 通过无符号数差值自动处理溢出
        while (static_cast<uint16_t>((TIM7->CNT - start)) < delay_ticks) {}
    }

    void ms(const uint32_t ms_) noexcept
    {
        const uint32_t start = uwTick;
        while (uwTick - start < ms_) {
            __WFI(); // 进入休眠模式，等待中断唤醒
        }
    }



}



