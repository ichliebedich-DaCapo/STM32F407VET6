/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif

static unsigned int counter = 0;
static char buf[4];

static void blueCounter_plus_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        counter++;
        sprintf(buf,  "%d", counter);
        lv_label_set_text(guider_ui.blueCounter_counter, buf);
        break;
    }
    case LV_EVENT_LONG_PRESSED_REPEAT:
    {
        counter++;
        sprintf(buf,  "%d", counter);
        lv_label_set_text(guider_ui.blueCounter_counter, buf);
        break;
    }
    default:
        break;
    }
}

static void blueCounter_minus_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        if(counter) counter--;
        sprintf(buf,  "%d", counter);
        lv_label_set_text(guider_ui.blueCounter_counter, buf);
        break;
    }
    case LV_EVENT_LONG_PRESSED_REPEAT:
    {
        if(counter) counter--;
        sprintf(buf,  "%d", counter);
        lv_label_set_text(guider_ui.blueCounter_counter, buf);
        break;
    }
    default:
        break;
    }
}

void events_init_blueCounter (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->blueCounter_plus, blueCounter_plus_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->blueCounter_minus, blueCounter_minus_event_handler, LV_EVENT_ALL, ui);
}


void events_init(lv_ui *ui)
{

}
