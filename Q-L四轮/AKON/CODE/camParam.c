#include "camParam.h"


// 内参矩阵
AT_DTCM_SECTION_ALIGN_INIT(float K[3][3], 8) = {{86.6713, 0, 94.9502},
                                                {0, 29.7204, 14.9738},
                                                {0,       0,      1}};

// 畸变参数(matlab fisheye 模型)
AT_DTCM_SECTION_ALIGN_INIT(float D[4], 8) = {-0.0265, 5.7124e-04, 0 ,0};
                                                
//                    0.9930          0.0845          -0.1602
//                    0.1891          0.2068          -0.9145
//                   -0.1955          0.9523           0.2144

// 逆透视变换矩阵(去畸变原图->俯视)
AT_DTCM_SECTION_ALIGN_INIT(float H_inv[3][3], 8) = {{ 6.4659e+03,  3.0017e+03, 6.4726},
                                                    { -2.0425e+03, 247.5607, -23.9674},
                                                    { 3.4131,      31.5116,    0.2144}};
