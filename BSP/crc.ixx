module;
#include <project_config.h>

export module crc;


export namespace bsp::crc
{
    inline CRC_HandleTypeDef hcrc;

    void init();
}


void bsp::crc::init()
{
    __HAL_RCC_CRC_CLK_ENABLE();

    hcrc.Instance = CRC;
    if (HAL_CRC_Init(&hcrc) != HAL_OK)
    {
        Error_Handler();
    }
}
