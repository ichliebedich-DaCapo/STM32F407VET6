//
// Created by 34753 on 2024/10/7.
//

#ifndef FURINA_FSMC_H
#define FURINA_FSMC_H
#include "JYZQ_Conf.h"
#if USE_FSMC

#ifdef __cplusplus
extern "C" {
#endif



void fsmc_init();
void fsmc_dma_init();




#ifdef __cplusplus
}
#endif
#endif // USE_FSMC
#endif //FURINA_FSMC_H
