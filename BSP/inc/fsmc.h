//
// Created by fairy on 2024/10/7.
//

#ifndef FURINA_FSMC_H
#define FURINA_FSMC_H
#include "JYZQ_Conf.h"
#ifdef USE_FSMC

#ifdef __cplusplus
extern "C" {
#endif


void fsmc_init();
#ifdef USE_FSMC_DMA
void fsmc_dma_init();
#endif




#ifdef __cplusplus
}
#endif
#endif // USE_FSMC
#endif //FURINA_FSMC_H
