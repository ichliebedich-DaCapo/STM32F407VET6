//
// Created by DV on 2024/10/14.
//

#include "fft.hpp"
#include "arm_const_structs.h"

#ifdef USE_FFT

auto FFT_Calculation::FFT_Calculate(float *DATA, int num, int N) -> float
{

//    arm_rfft_fast_instance_f32 S;
//    arm_cfft_init_f32(&arm_cfft_sR_f32_len1024,fftArray);
//    arm_cfft_f32(&arm_cfft_sR_f32_len1024, fftArray, IFFTFLAG, BITREVERSE);
//    arm_cmplx_mag_f32(inputArray, outputArray, NUM_SAMPLES / 2);
//    return array_arm_cmplx_mag[N];
}


//计算失真度
void THD() {
//    thd_basic = fft_outputbuf[10];
//
//    u[0] = fft_outputbuf[20];
//    u[1] = fft_outputbuf[30];
//    u[2] = fft_outputbuf[40];
//    u[3] = fft_outputbuf[50];
//    u[4] = fft_outputbuf[60];
//
//    arm_power_f32(u, 4, &sum);
//    arm_sqrt_f32(sum, &thd_high);
//
//    thd = thd_high / thd_basic;
}

#endif


