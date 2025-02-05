//
// Created by fairy on 2024/10/7.
//

#ifndef FURINA_FSMC_H
#define FURINA_FSMC_H
#include <bsp_config.h>


#ifdef __cplusplus
extern "C" {
#endif


void fsmc_init();
#ifdef USE_FSMC_DMA
void fsmc_dma_init();
#endif // USE_FSMC_DMA



#ifdef __cplusplus
}
#endif

#endif //FURINA_FSMC_H
