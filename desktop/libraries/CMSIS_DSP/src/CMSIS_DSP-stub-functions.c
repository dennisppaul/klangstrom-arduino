#include "arm_math_types.h"
#include "dsp/transform_functions.h"

void arm_add_f32(const float32_t * pSrcA, const float32_t * pSrcB, float32_t * pDst, uint32_t blockSize) {}

void arm_radix4_butterfly_inverse_f32( float32_t * pSrc, uint16_t fftLen, const float32_t * pCoef, uint16_t twidCoefModifier, float32_t onebyfftLen) {}

void arm_bitreversal_f32( float32_t * pSrc, uint16_t fftSize, uint16_t bitRevFactor, const uint16_t * pBitRevTab) {}

void arm_radix4_butterfly_f32( float32_t * pSrc, uint16_t fftLen, const float32_t * pCoef, uint16_t twidCoefModifier) {}

arm_status arm_cfft_radix4_init_f32( arm_cfft_radix4_instance_f32 * S, uint16_t fftLen, uint8_t ifftFlag, uint8_t bitReverseFlag) { return ARM_MATH_SUCCESS; }
