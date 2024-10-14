//
// Created by DV on 2024/10/14.
//

#ifndef FURINA_DSP_HPP
#define FURINA_DSP_HPP

#include "JYZQ_Conf.h"
#include "arm_math.h"
/* 预编译命令 */
#if USE_FFT
// 头文件

// 宏定义
/**
 * @brief FFT
 * @note 实现FFT计算
 *
 */
class FFT_Calculation
{
public:

    static auto FFT_Calculate(float *DATA, int num, int N)->float;
private:
   static constexpr const int fftSize=512;
};




// 接口

/* 预编译命令 */
#endif
#endif //FURINA_DSP_HPP