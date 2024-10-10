//
// Created by 34753 on 2024/10/10.
//

#include "player.h"

#if USE_PLAYER

#include "dac.h"
#include "adc.h"
#include "timer.h"
auto Player::init() -> void
{
    dac_init();
    adc1_init();
    timer2_init(49,104);// 分频为16KHz
    timer6_init(49,104);
}

#endif
