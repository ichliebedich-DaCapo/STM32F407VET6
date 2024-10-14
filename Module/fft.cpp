//
// Created by DV on 2024/10/14.
//

#include "fft.hpp"

#if USE_FFT

auto FFT_Calculation::FFT_Calculate(float *DATA, int num, int N) -> float {
    float array_FFT_output[num];        //储存FFT变换后的512个数据
    float array_arm_cmplx_mag[num];     //储存FFT变换后的512个数据的幅值信息

    arm_rfft_fast_instance_f32 S;
    arm_rfft_fast_init_f32(&S, fftSize);        //初始化结构体S中的参数
    arm_rfft_fast_f32(&S, array_f32, array_FFT_output, 0);          //fft正变换
    arm_cmplx_mag_f32(array_FFT_output, array_arm_cmplx_mag, num);  //计算幅值

    return array_arm_cmplx_mag[N];
}
#endif


