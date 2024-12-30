/**
 * @file monitor.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "monitor.h"
#include "pro_name.h"

#if USE_MONITOR

#ifndef MONITOR_SDL_INCLUDE_PATH
#  define MONITOR_SDL_INCLUDE_PATH <SDL2/SDL.h>
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include MONITOR_SDL_INCLUDE_PATH
#include "../indev/mouse.h"
#include "../indev/keyboard.h"
#include "../indev/mousewheel.h"

/*********************
 *      DEFINES
 *********************/
#define SDL_REFR_PERIOD     50  /*ms*/

#ifndef MONITOR_ZOOM_X
#define MONITOR_ZOOM_X        1
#endif

#ifndef MONITOR_ZOOM_Y
#define MONITOR_ZOOM_Y        1
#endif

#ifndef MONITOR_HOR_RES
#define MONITOR_HOR_RES        LV_HOR_RES
#endif

#ifndef MONITOR_VER_RES
#define MONITOR_VER_RES        LV_VER_RES
#endif

/**********************
 *      TYPEDEFS
 **********************/
typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    volatile bool sdl_refr_qry;
#if MONITOR_DOUBLE_BUFFERED
    uint32_t * tft_fb_act;
#else
    uint32_t *tft_fb;
#endif
} monitor_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void window_create(monitor_t *m);

static void window_update(monitor_t *m);

int quit_filter(void *userdata, SDL_Event *event);

static void monitor_sdl_clean_up(void);

static void monitor_sdl_init(void);

static void sdl_event_handler(lv_timer_t *t);

static void monitor_sdl_refr(lv_timer_t *t);

/***********************
 *   GLOBAL PROTOTYPES
 ***********************/

/**********************
 *  STATIC VARIABLES
 **********************/
monitor_t monitor;

#if MONITOR_DUAL
monitor_t monitor2;
#endif

static volatile bool sdl_inited = false;
static volatile bool sdl_quit_qry = false;


/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Initialize the monitor
 */
void monitor_init(void)
{
    monitor_sdl_init();
    lv_timer_create(sdl_event_handler, 10, NULL);
}

/**
 * Flush a buffer to the marked area
 * @param drv pointer to driver where this function belongs
 * @param area an area where to copy `color_p`
 * @param color_p an array of pixel to copy to the `area` part of the screen
 */
void monitor_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    lv_coord_t hres = disp_drv->hor_res;
    lv_coord_t vres = disp_drv->ver_res;

    /*Return if the area is out the screen*/
    if (area->x2 < 0 || area->y2 < 0 || area->x1 > hres - 1 || area->y1 > vres - 1)
    {
        lv_disp_flush_ready(disp_drv);
        return;
    }

#if MONITOR_DOUBLE_BUFFERED
    monitor.tft_fb_act = (uint32_t *)color_p;
#else /*MONITOR_DOUBLE_BUFFERED*/

    int32_t y;
#if LV_COLOR_DEPTH != 24 && LV_COLOR_DEPTH != 32    /*32 is valid but support 24 for backward compatibility too*/
    int32_t x;
    for (y = area->y1; y <= area->y2 && y < disp_drv->ver_res; y++)
    {
        for (x = area->x1; x <= area->x2; x++)
        {
            monitor.tft_fb[y * disp_drv->hor_res + x] = lv_color_to32(*color_p);
            color_p++;
        }
    }
#else
    uint32_t w = lv_area_get_width(area);
    for(y = area->y1; y <= area->y2 && y < disp_drv->ver_res; y++) {
        memcpy(&monitor.tft_fb[y * MONITOR_HOR_RES + area->x1], color_p, w * sizeof(lv_color_t));
        color_p += w;
    }
#endif
#endif /*MONITOR_DOUBLE_BUFFERED*/

    monitor.sdl_refr_qry = true;

    /* TYPICALLY YOU DO NOT NEED THIS
     * If it was the last part to refresh update the texture of the window.*/
    if (lv_disp_flush_is_last(disp_drv))
    {
        monitor_sdl_refr(NULL);
    }

    /*IMPORTANT! It must be called to tell the system the flush is ready*/
    lv_disp_flush_ready(disp_drv);

}


#if MONITOR_DUAL

/**
 * Flush a buffer to the marked area
 * @param drv pointer to driver where this function belongs
 * @param area an area where to copy `color_p`
 * @param color_p an array of pixel to copy to the `area` part of the screen
 */
void monitor_flush2(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    lv_coord_t hres = disp_drv->hor_res;
    lv_coord_t vres = disp_drv->ver_res;

    /*Return if the area is out the screen*/
    if(area->x2 < 0 || area->y2 < 0 || area->x1 > hres - 1 || area->y1 > vres - 1) {
        lv_disp_flush_ready(disp_drv);
        return;
    }

#if MONITOR_DOUBLE_BUFFERED
    monitor2.tft_fb_act = (uint32_t *)color_p;

    monitor2.sdl_refr_qry = true;

    /*IMPORTANT! It must be called to tell the system the flush is ready*/
    lv_disp_flush_ready(disp_drv);
#else

    int32_t y;
#if LV_COLOR_DEPTH != 24 && LV_COLOR_DEPTH != 32    /*32 is valid but support 24 for backward compatibility too*/
    int32_t x;
    for(y = area->y1; y <= area->y2 && y < disp_drv->ver_res; y++) {
        for(x = area->x1; x <= area->x2; x++) {
            monitor2.tft_fb[y * disp_drv->hor_res + x] = lv_color_to32(*color_p);
            color_p++;
        }

    }
#else
    uint32_t w = lv_area_get_width(area);
    for(y = area->y1; y <= area->y2 && y < disp_drv->ver_res; y++) {
        memcpy(&monitor2.tft_fb[y * disp_drv->hor_res + area->x1], color_p, w * sizeof(lv_color_t));
        color_p += w;
    }
#endif

    monitor2.sdl_refr_qry = true;

    /* TYPICALLY YOU DO NOT NEED THIS
     * If it was the last part to refresh update the texture of the window.*/
    if(lv_disp_flush_is_last(disp_drv)) {
        monitor_sdl_refr(NULL);
    }

    /*IMPORTANT! It must be called to tell the system the flush is ready*/
    lv_disp_flush_ready(disp_drv);
#endif
}
#endif

/**********************
 *   STATIC FUNCTIONS
 **********************/


/**
 * SDL main thread. All SDL related task have to be handled here!
 * It initializes SDL, handles drawing and the mouse.
 */

static void sdl_event_handler(lv_timer_t *t)
{
    (void) t;

    /*Refresh handling*/
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
#if USE_MOUSE != 0
        mouse_handler(&event);
#endif

#if USE_MOUSEWHEEL != 0
        mousewheel_handler(&event);
#endif

#if USE_KEYBOARD
        keyboard_handler(&event);
#endif
        if ((&event)->type == SDL_WINDOWEVENT)
        {
            switch ((&event)->window.event)
            {
#if SDL_VERSION_ATLEAST(2, 0, 5)
                case SDL_WINDOWEVENT_TAKE_FOCUS:
#endif
                case SDL_WINDOWEVENT_EXPOSED:
                    window_update(&monitor);
#if MONITOR_DUAL
                    window_update(&monitor2);
#endif
                    break;
                default:
                    break;
            }
        }
    }

    /*Run until quit event not arrives*/
    if (sdl_quit_qry)
    {
        monitor_sdl_clean_up();// 销毁纹理、渲染器、窗口，然后退出SDL
        exit(0);
    }
}

/**
 * SDL main thread. All SDL related task have to be handled here!
 * It initializes SDL, handles drawing and the mouse.
 */

static void monitor_sdl_refr(lv_timer_t *t)
{
    (void) t;

    /*Refresh handling*/
    if (monitor.sdl_refr_qry != false)
    {
        monitor.sdl_refr_qry = false;
        window_update(&monitor);
    }

#if MONITOR_DUAL
    if(monitor2.sdl_refr_qry != false) {
        monitor2.sdl_refr_qry = false;
        window_update(&monitor2);
    }
#endif
}

/**
 * 退出过滤器
 * @param userdata
 * @param event
 * @return
 * @note 这种设计模式允许你集中管理退出条件，并且可以在多个地方响应退出请求，
 * 同时保持代码的整洁和可维护性。此外，通过这种方式，你可以很容易地扩展功能，
 * 比如添加更多的退出条件或者改变退出行为。
 */
int quit_filter(void *userdata, SDL_Event *event)
{
    (void) userdata; // 忽略未使用的参数以避免编译警告

    if (event->type == SDL_WINDOWEVENT)
    {
        // 如果是窗口事件，则进一步检查具体是什么类型的窗口事件
        if (event->window.event == SDL_WINDOWEVENT_CLOSE)
        {
            // 如果窗口关闭事件（例如用户点击了窗口的关闭按钮），则设置 sdl_quit_qry 为 true
            sdl_quit_qry = true;
        }
    }
    else if (event->type == SDL_QUIT)
    {
        // 如果是 SDL_QUIT 事件（例如用户选择了退出菜单项或者系统请求程序退出），也设置 sdl_quit_qry 为 true
        sdl_quit_qry = true;
    }

    return 1; // 返回非零值表示事件被允许通过过滤器继续处理
}

static void monitor_sdl_clean_up(void)
{
    SDL_DestroyTexture(monitor.texture);
    SDL_DestroyRenderer(monitor.renderer);
    SDL_DestroyWindow(monitor.window);

#if MONITOR_DUAL
    SDL_DestroyTexture(monitor2.texture);
    SDL_DestroyRenderer(monitor2.renderer);
    SDL_DestroyWindow(monitor2.window);

#endif

    SDL_Quit();
}

static void monitor_sdl_init(void)
{
    /*Initialize the SDL*/
    SDL_Init(SDL_INIT_VIDEO);

    // 所谓事件过滤器就是对传入的事件进行判断处理
    SDL_SetEventFilter(quit_filter, NULL);

    window_create(&monitor);

    sdl_inited = true;
}


static void window_create(monitor_t *m)
{

    // 创建一个 SDL 窗口，窗口位置由系统决定（SDL_WINDOWPOS_UNDEFINED）
    m->window = SDL_CreateWindow("LVGL Simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                 MONITOR_HOR_RES*MONITOR_ZOOM_X , MONITOR_VER_RES*MONITOR_ZOOM_Y, 0);  // 最后一个参数可以设置为 SDL_WINDOW_BORDERLESS 来隐藏窗口边框


/**
 * @brief 创建一个与窗口关联的软件渲染器。
 *
 * @param m 指向 monitor_t 结构体的指针，包含窗口和其他相关资源。
 *
 * @details
 * 该函数初始化一个 SDL 渲染器，用于将图形内容绘制到指定的窗口中。
 * - 使用 CPU 进行所有绘图操作（软件渲染），适用于没有专用显卡的环境或简单的图形应用。
 * - 保证跨平台兼容性，因为不依赖于特定的 GPU 硬件。
 * - 允许直接访问和操作像素数据，适合需要特殊图像处理的应用场景。
 *
 * @param[in] window 指向 SDL_Window 对象的指针，表示渲染的目标窗口。
 *                   所有通过此渲染器绘制的内容都将显示在这个窗口中。
 * @param[in] index  渲染驱动程序索引。传递 -1 表示让 SDL 自动选择最佳的可用渲染驱动程序。
 *                   如果应用程序需要特定的硬件加速或其他特性，可以通过枚举可用的渲染驱动程序并选择适当的索引来指定。
 * @param[in] flags  渲染器标志，决定渲染器的工作模式：
 *                   - SDL_RENDERER_SOFTWARE: 使用软件渲染器，所有的绘图操作都由 CPU 处理。
 *                   - 优点包括跨平台兼容性和可以直接操作像素数据，适合某些特殊需求的应用场景。
 *                   - 缺点是在复杂图形或高帧率应用中性能可能不如硬件加速的渲染器。
 *                   - 适用于目标平台不支持硬件加速、需要在无专用显卡环境中运行或渲染内容相对简单的情况。
 *
 * @note 如果希望优先使用硬件加速但保持兼容性，可以考虑使用 SDL_RENDERER_ACCELERATED 标志，
 *       并检查返回值是否为 NULL，在必要时手动回退到 SDL_RENDERER_SOFTWARE。
 */
    m->renderer = SDL_CreateRenderer(m->window, -1, SDL_RENDERER_SOFTWARE);

/**
 * @brief 创建一个静态纹理，并设置其混合模式。
 *
 * @param m 指向 monitor_t 结构体的指针，包含渲染器和其他相关资源。
 *
 * @details
 * 该函数创建一个静态纹理，用于存储模拟器的帧缓冲区内容，并将其与渲染器关联。
 * 设置混合模式为 SDL_BLENDMODE_BLEND，允许透明度效果。
 *
 * @param[in] renderer 指向 SDL_Renderer 对象的指针，表示要使用的渲染器。
 * @param[in] format   纹理的像素格式，这里使用 SDL_PIXELFORMAT_ARGB8888 支持 alpha 通道。
 * @param[in] access   纹理访问类型，SDL_TEXTUREACCESS_STATIC 表明纹理内容不会频繁更新。
 * @param[in] width    纹理宽度，对应水平分辨率。
 * @param[in] height   纹理高度，对应垂直分辨率。
 */
    m->texture = SDL_CreateTexture(m->renderer,
                                   SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC,
                                   MONITOR_HOR_RES, MONITOR_VER_RES);
    SDL_SetTextureBlendMode(m->texture, SDL_BLENDMODE_BLEND);



/**
 * @brief 创建并设置窗口图标。
 *
 * @param m 指向 monitor_t 结构体的指针，包含窗口和其他相关资源。
 *
 * @details
 * 该函数从预定义的图标数据创建一个 SDL_Surface，并将其设置为窗口图标。
 * 完成后释放表面资源以节省内存。
 *
 * @param[in] iconData 图标数据的指针，通常是一个全局数组或静态变量。
 * @param[in] width    图标的宽度。
 * @param[in] height   图标的高度。
 * @param[in] depth    图像深度（位数）。
 * @param[in] pitch    每一行像素占用的字节数。
 * @param[in] Rmask    红色通道掩码。
 * @param[in] Gmask    绿色通道掩码。
 * @param[in] Bmask    蓝色通道掩码。
 * @param[in] Amask    Alpha 通道掩码。
 */
    SDL_Surface *iconSurface = SDL_CreateRGBSurfaceFrom(simulator_icon, 32, 32, 16, 32 * 2,
                                                        0xf000, 0x0f00, 0x00f0, 0x000f);
    SDL_SetWindowIcon(m->window, iconSurface);
    SDL_FreeSurface(iconSurface);  // 释放图标表面资源

/**
 * @brief 初始化帧缓冲区为灰色。
 *
 * @param m 指向 monitor_t 结构体的指针，包含纹理和其他相关资源。
 *
 * @details
 * 根据是否启用双缓冲 (MONITOR_DOUBLE_BUFFERED)，分别初始化帧缓冲区。
 * 如果启用了双缓冲，则直接更新纹理；否则，分配一块新的内存并填充为浅灰色 (0x44)。
 *
 * @param[in] texture  指向 SDL_Texture 对象的指针，表示要更新的纹理。
 * @param[in] fb      指向帧缓冲区的指针，用于存储像素数据。
 * @param[in] width    帧缓冲区的宽度。
 * @param[in] height   帧缓冲区的高度。
 */
#if MONITOR_DOUBLE_BUFFERED
    SDL_UpdateTexture(m->texture, NULL, m->tft_fb_act, MONITOR_HOR_RES * sizeof(uint32_t));
#else
    m->tft_fb = (uint32_t *) malloc(sizeof(uint32_t) * MONITOR_HOR_RES * MONITOR_VER_RES);
    memset(m->tft_fb, 0x44, MONITOR_HOR_RES * MONITOR_VER_RES * sizeof(uint32_t));  // 使用 0x44 填充，对应于浅灰色
#endif

/**
 * @brief 设置刷新查询标志为 true，表示需要刷新显示器。
 *
 * @param m 指向 monitor_t 结构体的指针，包含刷新查询标志。
 *
 * @details
 * 将 sdl_refr_qry 成员设置为 true，表明在接下来的主循环中应该刷新显示器，
 * 确保用户界面得到及时更新。
 */
    m->sdl_refr_qry = true;
}

static void window_update(monitor_t *m)
{
#if MONITOR_DOUBLE_BUFFERED == 0
    SDL_UpdateTexture(m->texture, NULL, m->tft_fb, MONITOR_HOR_RES * sizeof(uint32_t));
#else
    if(m->tft_fb_act == NULL) return;
    SDL_UpdateTexture(m->texture, NULL, m->tft_fb_act, MONITOR_HOR_RES * sizeof(uint32_t));
#endif
    SDL_RenderClear(m->renderer);
#if LV_COLOR_SCREEN_TRANSP
    SDL_SetRenderDrawColor(m->renderer, 0xff, 0, 0, 0xff);
    SDL_Rect r;
    r.x = 0; r.y = 0; r.w = MONITOR_HOR_RES; r.w = MONITOR_VER_RES;
    SDL_RenderDrawRect(m->renderer, &r);
#endif

    /*Update the renderer with the texture containing the rendered image*/
    SDL_RenderCopy(m->renderer, m->texture, NULL, NULL);
    SDL_RenderPresent(m->renderer);
}


void LCD_Set_Pixel(uint16_t x, uint16_t y, uint32_t color)
{
    monitor.tft_fb[y * 480 + x] = lv_color_to32(lv_color_hex(color));
}

void LCD_Color_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint32_t *color)
{
    for (int i = ysta; i <= yend; i++)
    {
        for (int j = xsta; j <= xend; j++)
        {
            LCD_Set_Pixel(j, i, color[j + i * xend]);
        }
    }
}

void LCD_Color_Clean(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint32_t color)
{
    for (int i = ysta; i <= yend; i++)
    {
        for (int j = xsta; j <= xend; j++)
        {
            LCD_Set_Pixel(j, i, color);
        }
    }
}


#endif /*USE_MONITOR*/
