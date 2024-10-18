#ifndef EVENTS_INIT_H_
#define EVENTS_INIT_H_

#include "gui_guider.hpp"

/**
 * @brief 初始化事件
 */
class Events:public GUI_Base
{
public:
     static auto init()-> void ;

// 绑定事件
static inline auto bond(lv_obj_t * &obj, lv_event_cb_t event_cb, lv_event_code_t filter=LV_EVENT_ALL){
   return lv_obj_add_event_cb(obj, event_cb, filter,gui);
}
};


#endif /* EVENT_CB_H_ */
