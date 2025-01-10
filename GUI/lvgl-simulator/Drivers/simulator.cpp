//
// Created by fairy on 2025/1/1 15:52.
//
#include "simulator.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <cstring>

static volatile bool keep_running = true;

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;
static int HOR;// 屏幕宽度
static int VER;// 屏幕高度

// 屏幕显存
static uint16_t TFT_GRAM[320][480];
static uint16_t *GRAM = &TFT_GRAM[0][0];

// 触摸屏相关变量
static bool press_state = false;
static int32_t last_x = 0;
static int32_t last_y = 0;

/***********************************内部函数**************************************/
void mouse_handler(SDL_Event *event);


/*******************************模拟器接口************************************/
bool simulator_is_running() { return keep_running; }


/**
 * @brief 初始化 SDL
 * @note 初始化了 SDL 窗口、渲染器、纹理，并创建了一个更新纹理的线程.默认颜色编码为 RGB565
 */
void simulator_init(int32_t hor, int32_t ver)
{
    HOR = hor;
    VER = ver;

    // 初始化 SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
        keep_running = false;
        return;
    }

    // 创建窗口
    window = SDL_CreateWindow("LVGL Simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              hor, ver, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS);

    // 创建渲染器（此处使用硬件加速，可以使用软件加速SDL_RENDERER_SOFTWARE）
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // 创建纹理
    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING,
                                hor, ver);


    if (!window || !renderer || !texture)
    {
        std::cerr << "Failed to create window/renderer/texture: " << SDL_GetError() << std::endl;
        SDL_Quit();
        keep_running = false;
        return;
    }

    // 创建更新纹理的线程
//    SDL_CreateThread([](void *)
//                     {
//                         while (keep_running)
//                         {
//
//                             SDL_Delay(10);
//                         }
//                         return 1;
//                     }, "updateSDLGram", nullptr);
}


/**
 * @brief 退出 SDL
 */
void simulator_quit()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


/**
 * @brief 处理 SDL 事件
 */
void simulator_event_Handler()
{
    // 处理SDL事件
    static SDL_Event event;
    while (SDL_PollEvent(&event))
    {


        if (event.type == SDL_KEYDOWN)
        {
            printf("Key Press\n");
        }

        if (event.type == SDL_KEYUP)
        {
            printf("Key Up\n");
        }

        // 处理鼠标事件
        mouse_handler(&event);

        // 处理其他事件
        switch (event.type)
        {
            case SDL_QUIT:
                keep_running = false;
                break;

            case SDL_KEYDOWN:
        // 失效了，没有任何用
                break;

            case SDL_WINDOWEVENT:
                switch (event.window.event)
                {
                    case SDL_WINDOWEVENT_TAKE_FOCUS:
                    case SDL_WINDOWEVENT_EXPOSED:

                    default:
                        break;
                }
            default:
                break;
        }
    }

    // 更新纹理
    SDL_UpdateTexture(texture, nullptr, GRAM, HOR * sizeof(uint16_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}


/***********************************LCD驱动接口******************************************/
[[maybe_unused]] uint32_t rgb565_to_rgb8888(uint16_t &color)
{
    uint8_t r = ((color >> 11) & 0x1F) * 8;
    uint8_t g = ((color >> 5) & 0x3F) * 4;
    uint8_t b = (color & 0x1F) * 8;
    return (r << 16) | (g << 8) | b;
}


void LCD_Set_Pixel(uint16_t x, uint16_t y, uint16_t color)
{
    GRAM[x + y * HOR] = color;
}


void LCD_Color_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, const uint16_t *color)
{
    uint16_t width = xend - xsta + 1;

    // 遍历指定区域的行
    for (uint16_t y = ysta; y <= yend; y++)
    {
        uint16_t *gram_row = TFT_GRAM[y] + xsta;
        const uint16_t *color_row = color + (y - ysta) * width;// 计算起始位置

        memcpy(gram_row, color_row, width * sizeof(uint16_t));
    }
}


void LCD_Color_Clean(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color)
{
    uint16_t width = xend - xsta + 1;
    uint16_t height = yend - ysta + 1;
    uint32_t row_bytes = width * sizeof(uint16_t);
    auto *tft_ptr = (GRAM + xsta + ysta * HOR);
    for (int y = 0; y < height; ++y)
    {
        memset(tft_ptr + y * width, color, row_bytes);
    }
}

[[maybe_unused]] void LCD_Clear(uint16_t color)
{
    LCD_Color_Clean(0, 0, HOR - 1, VER - 1, color);
}


/***********************************触摸屏驱动接口******************************************/
int32_t touchpad_read_xy(int32_t *x, int32_t *y)
{
    // 读取触摸屏数据
    *x = last_x;
    *y = last_y;

    // 返回是否触摸
    return press_state;
}

void mouse_handler(SDL_Event *event)
{
    switch (event->type)
    {
        case SDL_MOUSEBUTTONUP:
            if (event->button.button == SDL_BUTTON_LEFT)
                press_state = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event->button.button == SDL_BUTTON_LEFT)
            {
                press_state = true;
                last_x = event->motion.x;
                last_y = event->motion.y;
            }
            break;
        case SDL_MOUSEMOTION:
            last_x = event->motion.x;
            last_y = event->motion.y;
            break;

        case SDL_FINGERUP:
            press_state = false;
            last_x = (int) ((float) HOR * event->tfinger.x);
            last_y = (int) ((float) VER * event->tfinger.y);
            break;
        case SDL_FINGERDOWN:
            press_state = true;
            last_x = (int) ((float) HOR * event->tfinger.x);
            last_y = (int) ((float) VER * event->tfinger.y);
            break;
        case SDL_FINGERMOTION:
            last_x = (int) ((float) HOR * event->tfinger.x);
            last_y = (int) ((float) VER * event->tfinger.y);
            break;
    }
}


/**************************键盘*****************************/

