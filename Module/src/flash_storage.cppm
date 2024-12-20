
module flash_storage:impl;

import flash_storage;


/**
 * @brief 中断预写入函数
 * @details 先判断此前是否处于写的状态，若不是则重置索引
 */
template<
        void (*WriteFunc)(const uint8_t *data, uint32_t length, uint32_t address),
        void (*ReadFunc)(uint8_t *data, uint32_t length, uint32_t address),
        void (*EraseSectorFunc)(uint32_t sectorAddress)
>
auto FlashStorage<WriteFunc, ReadFunc, EraseSectorFunc>::write_isr_pre(uint32_t address) -> void
{
    addr = address;
    // 判断地址是否是页数的整数倍
    index = addr % page_size;
    addr_remaining = page_size - index;
    if (index != 0)
        set_write_incomplete_sign();
}


template<
        void (*WriteFunc)(const uint8_t *data, uint32_t length, uint32_t address),
        void (*ReadFunc)(uint8_t *data, uint32_t length, uint32_t address),
        void (*EraseSectorFunc)(uint32_t sectorAddress)
>
auto FlashStorage<WriteFunc, ReadFunc, EraseSectorFunc>::write_isr(uint8_t byte) -> void
{
    if (index == page_size - 1)
    {
        pForeBuffer[page_size - 1] = byte;
        switch_buffer();// 切换缓冲区
        set_write_full_sign();// 标记为写满
    } else
    {
        pForeBuffer[index++] = byte;
    }
}


/**
 * @brief 后台处理函数，用于在主循环中调用
 * @note 检测对应标志就进行相应处理，不要做多余的操作
 */
template<
        void (*WriteFunc)(const uint8_t *data, uint32_t length, uint32_t address),
        void (*ReadFunc)(uint8_t *data, uint32_t length, uint32_t address),
        void (*EraseSectorFunc)(uint32_t sectorAddress)
>
auto FlashStorage<WriteFunc, ReadFunc, EraseSectorFunc>::background_processing() -> void
{
    // 写满标志检测
    if (check_write_full_sign())
    {
        // 残缺写入检测
        if (check_write_incomplete_sign())
        {
            /* 进行残缺的写入页数据*/
            WriteFunc(pBackBuffer, addr, addr_remaining);//写入完整一页
            addr += addr_remaining;
        } else
        {
            /* 进行完整页的写入 */
            WriteFunc(pForeBuffer, addr, page_size);//写入完整一页
            addr += page_size;
        }
    }
}

