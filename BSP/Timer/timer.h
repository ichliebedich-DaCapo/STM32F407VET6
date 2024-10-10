//
// Created by 34753 on 2024/10/10.
//

#ifndef FURINA_TIMER_H
#define FURINA_TIMER_H

#include "JYZQ_Conf.h"

#if USE_TIMER
#ifdef __cplusplus
extern "C" {
#endif

void timer2_init(uint32_t arr, uint32_t psc);

void timer6_init(uint32_t arr, uint32_t psc);


#ifdef __cplusplus
}
#endif
#endif

#endif //FURINA_TIMER_H
