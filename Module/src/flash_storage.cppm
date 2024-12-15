
module flash_storage:impl;

import flash_storage;

/**
 * @brief 为中断中的读取作预加载
 * @details 先判断此前是否处于读的状态，若不是则重置索引并预加载到缓冲区里
 *          此外还需要考虑初始addr不是256的整数倍
 */
template<
        void (*WriteFunc)(const uint8_t *data, uint32_t length, uint32_t address),
        void (*ReadFunc)(uint8_t *data, uint32_t length, uint32_t address),
        void (*EraseSectorFunc)(uint32_t sectorAddress)
>
auto FlashStorage<WriteFunc, ReadFunc, EraseSectorFunc>::read_isr_ready() -> void
{
    if (check_write_sign())
    {
        set_read_sign();
        reset_index();// 编译器应该会为我优化的

        /*从Flash的首页加载地址*/

        // 判断地址是否是页数的整数倍
        index = addr % page_size;
        addr_remaining = page_size - index;
        if (index != 0)
            set_read_incomplete_sign();

        /* 加载该页数据*/
        ReadFunc(pForeBuffer, addr, addr_remaining);
        /* 加载下一页数据*/
        ReadFunc(pBackBuffer, addr - index + page_size, page_size);
    }
}

/**
 * @brief 为中断中的写入作预加载
 * @details 先判断此前是否处于写的状态，若不是则重置索引
 */
template<
        void (*WriteFunc)(const uint8_t *data, uint32_t length, uint32_t address),
        void (*ReadFunc)(uint8_t *data, uint32_t length, uint32_t address),
        void (*EraseSectorFunc)(uint32_t sectorAddress)
>
auto FlashStorage<WriteFunc, ReadFunc, EraseSectorFunc>::write_isr_ready() -> void
{

    if (check_read_sign())
    {
        set_write_sign();
        reset_index();
    }

    // 判断地址是否是页数的整数倍
    index = addr % page_size;
    addr_remaining = page_size - index;
    if (index != 0)
        set_write_incomplete_sign();

}

/**
 * @brief 读取数据
 * @return 返回该字节数据
 * @note 本想加个读写标志判断的，但这样太影响效率了。想了想，干脆把它们变成读写的前置操作。
 *      这样既提高了效率，又不会与其他模块耦合在一块（提供接口供Player调用）
 */
template<
        void (*WriteFunc)(const uint8_t *data, uint32_t length, uint32_t address),
        void (*ReadFunc)(uint8_t *data, uint32_t length, uint32_t address),
        void (*EraseSectorFunc)(uint32_t sectorAddress)
>
auto FlashStorage<WriteFunc, ReadFunc, EraseSectorFunc>::read_isr() -> uint8_t
{
    if (index == page_size - 1)
    {
        uint8_t temp = pForeBuffer[page_size - 1];
        switch_buffer();
        set_read_full_sign();// 标记为读满
        return temp;
    } else
    {
        return pForeBuffer[index++];
    }
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
        switch_buffer();
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
    if (check_read_full_sign())
    {
        clear_read_full_sign();
        ReadFunc(pBackBuffer, addr, page_size);
        addr += page_size;
    }
    if (check_write_full_sign())
    {
        clear_write_full_sign();

        if (check_write_incomplete_sign())
        {
            clear_write_incomplete_sign();
            /* 进行残缺的写入页数据*/
            WriteFunc(pBackBuffer, addr, addr_remaining);
            addr += addr_remaining;
        }
    }
}

