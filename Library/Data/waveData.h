//
// Created by fairy on 2024/10/12.
//

#ifndef FURINA_WAVEDATA_H
#define FURINA_WAVEDATA_H

#include <stdint.h>
#include "JYZQ_Conf.h"
//#include <stdint-gcc.h>
/**
 * @brief 波形数据
 * @details 使用的是MATLAB生成的波形数据，之所以不使用Python是因为Python的精度没有MATLAB高
 *  代码放在后面了
 */
extern const uint8_t sineWave[256];
extern const uint8_t triangleWave[256];
extern const uint8_t squareWave[256];
extern const uint8_t sawtoothWave[256];
const uint8_t sine_wave[128] = {
        128, 134, 140, 146, 152, 158, 165, 170, 176, 182, 188, 193, 198, 203, 208, 213, 218, 222, 226, 230, 234, 237,
        240, 243, 245, 248, 250, 251, 253, 254, 254, 255, 255, 255, 254, 254, 253, 251, 250, 248, 245, 243, 240, 237,
        234, 230, 226, 222, 218, 213, 208, 203, 198, 193, 188, 182, 176, 170, 165, 158, 152, 146, 140, 134, 127, 121,
        115, 109, 103, 97, 90, 85, 79, 73, 67, 62, 57, 52, 47, 42, 37, 33, 29, 25, 21, 18, 15, 12, 10, 7, 5, 4, 2, 1, 1,
        0, 0, 0, 1, 1, 2, 4, 5, 7, 10, 12, 15, 18, 21, 25, 29, 33, 37, 42, 47, 52, 57, 62, 67, 73, 79, 85, 90, 97, 103,
        109, 115, 121
};
/*% 定义参数
        N = 256; % 采样点数
        t = (0:N-1) / (N-1); % 时间向量，确保最后一个点和第一个点对齐

% 生成波形
        sineWave = round(127.5 * sin(2 * pi * t) + 127.5); % 正弦波
        triangleWave = round(255 * abs(2 * mod(t, 0.5) - 0.5)); % 三角波
        squareWave = round((sin(2 * pi * t) >= 0) * 255); % 方波
        sawtoothWave = round(255 * mod(t, 1)); % 锯齿波

% 将波形转换为C语言数组形式
        functionToCArray('sineWave', sineWave);
functionToCArray('triangleWave', triangleWave);
functionToCArray('squareWave', squareWave);
functionToCArray('sawtoothWave', sawtoothWave);

% 辅助函数，用于将向量转换成C语言数组形式并打印
        function functionToCArray(name, wave)
fprintf('const uint8_t %s[%d] = {\n', name, length(wave));
fprintf('%d', wave(1));
for i = 2:length(wave)
fprintf(', %d', wave(i));
end
        fprintf('\n};\n\n');
end*/


#endif //FURINA_WAVEDATA_H
