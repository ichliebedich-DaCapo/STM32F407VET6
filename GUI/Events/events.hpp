#ifndef EVENTS_INIT_H_
#define EVENTS_INIT_H_
/**
 * @brief 事件初始化
 * @note
 *  有时候出于某种简化写法的目的，会使用宏定义做一些很邪恶的事情。当然这种代码风格挺糟糕的，但真的很爽
*/
#include "GUI_Base.hpp"


/*************别名***************/

//using stateHandler = void (*)(bool);
using event = lv_event_t *;

//namespace events
//{
//}

//******************出于某种简化的目的，我使用了非常邪恶的宏定义*******************//


/**
 * @brief void*类型的"语法糖"，用于生成匿名函数，简化函数定义过程.如果你要调用这个函数那么还需要在后面加上();
 * @param ... 函数体
 * @note 仿Koltin的lambda表达式，只能说宏定义是真滴强大
 * @example 假设现在有一个函数void bond(void*); 需要传一个void*的函数，那么
 *          正常调用：
 *              bond([]()
 *              {
 *              int a=0;
 *              a=a+2;
 *              })
 *
 *          使用宏：
 *           bond(fun(
 *           int a=0;
 *           a=a+2;
 *           ))
 *
 *           调用宏所代表的函数：
 *           fun(
 *           int a=0;
 *           a=a+2;
 *           )();
 *
 */
#define fun(...) [](){ __VA_ARGS__}
// 用于调用
#define FUN(...) [](){ __VA_ARGS__}()


// 用于定时器的状态事件回调
#define timer_fun(...) [](lv_timer_t*){__VA_ARGS__}



/**
 * @brief 用于图片按钮的状态事件回调
 * @param handler stateHandler类型的函数形参
 * @details 你需要定义一个stateHandler类型的函数
 * @example 可以使用lambda表达式：
            state_fun([](bool check)
            {
                if(check)
                {
                 int a = 0;
                 a=a+2;
                }
                else
                {
                int a = 0;
                }
           });
 */
#define imgbtn_fun(handler) [](event e){ \
switch (lv_event_get_code(e))\
{\
case LV_EVENT_CLICKED:\
handler(lv_obj_has_state(e->current_target, LV_STATE_CHECKED));\
break;\
default:\
break;\
}\
}

/**
 * @brief 用于按钮的事件回调
 * @param handler void*类型的函数形参
 * @example 可以使用lambda表达式：
            state_fun([]()
            {
                 int a = 0;
                 a=a+2;
           });
 */
#define btn_fun(handler) [](event e){ \
switch (lv_event_get_code(e))\
{\
case LV_EVENT_CLICKED:\
handler();\
break;\
default:\
break;\
}\
}

/**
 * @brief 用于图片按钮的状态事件回调
 * @param press_fun void*类型，按下回调函数，
 * @param release_fun void*类型，释放回调函数
 */
#define imgbtn_fun2(press_fun, release_fun)     [](event e){\
switch (lv_event_get_code(e))\
{\
case LV_EVENT_CLICKED:\
if(lv_obj_has_state(e->current_target, LV_STATE_CHECKED))\
{\
press_fun();\
}\
else\
{\
release_fun();\
}\
break;\
default:\
break;\
}\
}

// 函数模板，用于生成事件处理函数


/**
 * @brief 初始化事件
 */
class Events : public GUI_Base
{
public:
    static auto init() -> void;

    // 绑定事件
    static inline auto
    bond(Obj obj, lv_event_cb_t event_cb, void *user_data = nullptr, lv_event_code_t filter = LV_EVENT_ALL);

    // 通用事件处理
//    static inline auto handler(event e, stateHandler cb) -> void;
};

/**
 * @brief 绑定事件
 * @note 用于按钮、图片按钮这些二值状态的组件
 *      传参可以使用imgbtn_fun()或btn_fun()宏来生成，简化了函数定义过程
 * @param obj 组件对象
 * @param event_cb 事件回调函数
 * @param user_data 用户数据指针，默认为nullptr
 * @param filter 事件过滤器，默认为LV_EVENT_ALL
 * @return
 * @example bond(gui->main.btn_mode, imgbtn_fun([](bool check)
 *                                      {
 *                                         if (check)
 *                                           {
 *                                               // 实现
 *                                           } else
 *                                           {
 *                                               // 实现
 *                                           }
 *                                       }));
 */
auto Events::bond(Obj obj, lv_event_cb_t event_cb, void *user_data, lv_event_code_t filter)
{
    return lv_obj_add_event_cb(obj, event_cb, filter, user_data);
}


///**
//* @brief 通用事件处理,两态按钮
//* @param e
//* @param handler
//*/
//auto Events::handler(event e, stateHandler cb) -> void
//{
//    switch (lv_event_get_code(e))
//    {
//        case LV_EVENT_CLICKED:
//            cb(lv_obj_has_state(e->current_target, LV_STATE_CHECKED));
//            break;
//        default:
//            break;
//    }
//}



#endif /* EVENTS_INIT_H_ */
