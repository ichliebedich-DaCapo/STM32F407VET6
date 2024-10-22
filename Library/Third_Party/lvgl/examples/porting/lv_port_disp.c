/**
 * @file lv_port_disp_templ.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp.h"
#include <stdbool.h>
#include "lcd.h"
#include "Module_Conf.h"
/*********************
 *      DEFINES
 *********************/

#define MY_DISP_HOR_RES    480
#define MY_DISP_VER_RES    320
#define MY_DISP_BUF_SIZE    10
/**********************
 *      TYPEDEFS
 **********************/
extern DMA_HandleTypeDef hdma_memtomem_dma2_stream6;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

static lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/

void lv_port_disp_init(void)
{
    /* Example for 1) */
    static lv_disp_draw_buf_t draw_buf_dsc_1;
    static lv_color_t buf_1[MY_DISP_HOR_RES * MY_DISP_BUF_SIZE];                          /*A buffer for 10 rows*/
    lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL,
                          MY_DISP_HOR_RES * MY_DISP_BUF_SIZE);   /*Initialize the display buffer*/

    /*-----------------------------------
     * Register the display in lvgl
     *----------------------------------*/


    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_flush;

    /*Set a display buffer*/
    disp_drv.draw_buf = &draw_buf_dsc_1;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process) when disp_flush() is called by lvgl
 */
void disp_enable_update(void)
{
    disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by lvgl
 */
void disp_disable_update(void)
{
    disp_flush_enabled = false;
}


static void disp_flush(lv_disp_drv_t *disp_drv1, const lv_area_t *area, lv_color_t *color_p)
{
#ifdef USE_FSMC_DMA
    LCD_Set_Window(area->x1, area->y1, area->x2, area->y2);//设置LCD屏幕的扫描区域
    HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream6, (uint32_t) color_p, (uint32_t) TFT_DATA_ADDR,
                     ((area->x2 + 1) - area->x1) * ((area->y2 + 1) - area->y1));
#else
     LCD_Color_Fill(area->x1, area->y1, area->x2, area->y2, (const uint16_t *)color_p);
    disp_drv.draw_buf->flushing = 0;
    disp_drv.draw_buf->flushing_last = 0;
#endif
}

void LVGL_LCD_FSMC_DMA_pCallback(DMA_HandleTypeDef *_hdma)
{
    // lv_disp_flush_ready(&disp_drv);
    //为了减少栈帧的使用
    disp_drv.draw_buf->flushing = 0;
    disp_drv.draw_buf->flushing_last = 0;
}