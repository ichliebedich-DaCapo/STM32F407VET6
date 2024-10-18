#ifndef EVENTS_INIT_H_
#define EVENTS_INIT_H_


#include "lvgl.h"

using stateHandler = void (*)(bool);
using event = lv_event_t *;

/**
 * @brief 初始化事件
 */
class Events
{
public:
    static auto init() -> void;

// 绑定事件
    static inline auto
    bond(lv_obj_t *&obj, lv_event_cb_t event_cb, void *user_data = nullptr, lv_event_code_t filter = LV_EVENT_ALL)
    {
        return lv_obj_add_event_cb(obj, event_cb, filter, user_data);
    }

/**
* @brief 通用事件处理
* @param e
* @param handler
*/
    static inline auto handler(event e, stateHandler handler)-> void
    {
        switch (bool is_checked;lv_event_get_code(e))
        {
            case LV_EVENT_CLICKED:
                is_checked = lv_obj_has_state(e->current_target, LV_STATE_CHECKED);
                handler(is_checked);
                break;
            default:
                break;
        }
    }
};

// 取别名




#endif /* EVENTS_INIT_H_ */
