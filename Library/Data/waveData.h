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
