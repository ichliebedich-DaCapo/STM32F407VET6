//
// Created by 34753 on 2024/10/7.
//

#ifndef FURINA_KEY_EXIT_H
#define FURINA_KEY_EXIT_H

#include "Module_Conf.h"

#ifdef USE_KEY_EXTI
#ifdef __cplusplus
extern "C" {
#endif


void key_exti_init();

#endif

#ifdef __cplusplus
}
#endif
#endif //FURINA_KEY_EXIT_H
