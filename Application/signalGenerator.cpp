//
// Created by 34753 on 2024/10/12.
//

#include "signalGenerator.h"
#include "timer.h"
#include "dac.h"
void app_init()
{
    timer6_init(83,63);
    dac_init();
}

void timer6_isr()
{

}