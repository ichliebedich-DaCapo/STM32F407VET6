//
// Created by 34753 on 2024/10/7.
//

#ifndef FURINA_KEYTASKHANDLER_H
#define FURINA_KEYTASKHANDLER_H
#include "JYZQ_Conf.h"
#if ENABLE_KEY_TASK_HANDLE
#include "main.h"
#include "key.h"
#ifndef APP_NO_RTOS
void keyTaskHandler_init();
#else
inline void keyTaskHandler_init()
{
    Key::init();
}
#endif


#endif// ENABLE_KEY_TASK_HANDLE
#endif //FURINA_KEYTASKHANDLER_H
