//
// Created by 34753 on 2024/10/12.
//

#include "signalGenerator.h"
#include "timer.h"
#include "dac.h"
#include "wave_signal.h"
#include "key.h"


void app_init()
{
    timer6_init(FREQ_84M_to_1K);
    dac_init();
}

void key_handler()
{
    switch (key.getCode())
    {
        case keyk0:// 播放
            if (key.getState(keyk0))
            {
                WaveSignal::on();
            } else
            {
                WaveSignal::off();
            }
            break;
    }
}

void timer6_isr()
{
    WaveSignal::generate();
}