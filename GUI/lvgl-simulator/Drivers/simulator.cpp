//
// Created by fairy on 2025/1/1 15:52.
//
#include "simulator.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <cstring>

static volatile int keep_running = 1;

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;
static uint16_t tft_fb[480 * 320];


bool simulator_is_running() { return keep_running; }


/**
 * @brief 初始化 SDL
 * @note 初始化了 SDL 窗口、渲染器、纹理，并创建了一个更新纹理的线程
 */
void simulator_init(int argc, char *argv[])
{
    // 忽略参数
    (void) argc, (void) argv;

    // 初始化 SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
        keep_running = false;
        return;
    }

    // 创建窗口
    window = SDL_CreateWindow("LVGL Simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              480, 320, 0);

    // 创建渲染器（此处使用硬件加速，可以使用软件加速SDL_RENDERER_SOFTWARE）
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // 创建纹理
    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING,
                                480, 320);


    if (!window || !renderer || !texture)
    {
        std::cerr << "Failed to create window/renderer/texture: " << SDL_GetError() << std::endl;
        SDL_Quit();
        keep_running = false;
        return;
    }

    // 创建更新纹理的线程
    SDL_CreateThread([](void *)
                     {
                         while (keep_running)
                         {
                             SDL_UpdateTexture(texture, nullptr, tft_fb, 480 * sizeof(uint16_t));
                             SDL_RenderClear(renderer);
                             SDL_RenderCopy(renderer, texture, nullptr, nullptr);
                             SDL_RenderPresent(renderer);
                             SDL_Delay(10);
                         }
                         return 1;
                     }, "updateSDLGram", nullptr);
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
    static SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                keep_running = false;
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

    // 短暂休眠
    SDL_Delay(10);
}


/*****************************************************************************/
uint32_t rgb565_to_rgb8888(uint16_t &color)
{
    uint8_t r = ((color >> 11) & 0x1F) * 8;
    uint8_t g = ((color >> 5) & 0x3F) * 4;
    uint8_t b = (color & 0x1F) * 8;
    return (r << 16) | (g << 8) | b;
}


void LCD_Set_Pixel(uint16_t x, uint16_t y, uint16_t color)
{
    tft_fb[x + y * 480] = color;
}


void LCD_Color_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, const uint16_t *color)
{
    uint16_t width = xend - xsta + 1;
    uint16_t height = yend - ysta + 1;
    uint32_t row_bytes = width * sizeof(uint16_t);
    auto *tft_ptr = (tft_fb+xsta+ysta * 480);
    auto *color_ptr = color;
    for (int y = 0; y < height; ++y)
    {
        memcpy(tft_ptr + y * width, color_ptr+y * width,row_bytes);
    }
}


void LCD_Color_Clean(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color)
{
    uint16_t width = xend - xsta + 1;
    uint16_t height = yend - ysta + 1;
    uint32_t row_bytes = width * sizeof(uint16_t);
    auto *tft_ptr = (tft_fb+xsta+ysta * 480);
    for (int y = 0; y < height; ++y)
    {
        memset(tft_ptr + y * width, color, row_bytes);
    }
}

void LCD_Clear(uint16_t color)
{
    LCD_Color_Clean(0, 0, 480 - 1, 320 - 1, color);
}