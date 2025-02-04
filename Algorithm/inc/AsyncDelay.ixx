export module async_delay;


export
{
#include <cstdint>

    /**
     * @brief 异步延迟类
     * @tparam get_tick 获取当前时间函数，基本单位一般为1ms
     * @note 目前思路是，先用这三个32位变量，delay_tick这个是可以换成函数形参的
     *
     */
    template<uint32_t(*get_tick)()>
    class AsyncDelay
    {
    public:
        AsyncDelay() : start_tick(0), final_tick(0), delay_tick(100) {}
        explicit AsyncDelay(uint32_t delay_tick) : start_tick(0), final_tick(0), delay_tick(delay_tick) {}

        void reset();// 重置
        bool is_timeout();// 是否超时
        void set_delay_tick(uint32_t tick);
    private:
        uint32_t start_tick;
        uint32_t final_tick;
        uint32_t delay_tick;// 默认100个tick
    };
}

template<uint32_t(*get_tick)()>
void AsyncDelay<get_tick>::reset()
{
    start_tick = get_tick();
    final_tick = start_tick + delay_tick;
}

template<uint32_t(*get_tick)()>
bool AsyncDelay<get_tick>::is_timeout()
{
    // 非阻塞延迟
    if (final_tick < start_tick)
    {
        // 检测溢出情况
        if (get_tick() > final_tick && get_tick() < start_tick)
        {
            // 重置
            start_tick = get_tick();
            final_tick = start_tick + delay_tick;
            // 标志记为1
            return true;
        }
    }
    else
    {
        if (get_tick() > final_tick)
        {
            // 重置
            start_tick = get_tick();
            final_tick = start_tick + delay_tick;
            // 标志记为1
            return true;
        }
    }
    return false;
}

template<uint32_t(*get_tick)()>
void AsyncDelay<get_tick>::set_delay_tick(uint32_t tick)
{
    delay_tick = tick;
}
