module;
#include <lvgl.h>
export module gui_timer;
import widget;


export namespace gui::compose
{
    /***********************************定时器**************************************/
    /**
     *  @brief 定时器封装类
     *  @note 需要自己调用create函数创建定时器，可以使用remove来删除定时器
     */
    class Timer
    {
    public:
        // 默认为关闭状态
        auto create(lv_timer_cb_t timer_cb, uint32_t period = 1000, void *user_data = nullptr) -> void
        {
            if(!_timer)
            {
                _timer = lv_timer_create(timer_cb, period, user_data);
                pause();
            }
        }

        // 删除定时器
        auto remove() -> void
        {
            lv_timer_del(_timer);
            _timer = nullptr;
        }

        // 恢复定时器
        auto resume() -> void
        {
            lv_timer_resume(_timer);
        }

        // 暂停定时器
        auto pause() -> void
        {
            lv_timer_pause(_timer);
        }

        // 设置周期
        auto set_period(uint32_t period) const -> void
        {
            lv_timer_set_period(_timer, period);
        }

    private:
        lv_timer_t *_timer{};
    };
}