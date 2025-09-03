/*
* Klangstrom
*
* This file is part of the *Klangstrom* library (https://github.com/dennisppaul/klangstrom-libraries).
* Copyright (c) 2024 Dennis P Paul.
*
* This library is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3.
*
* This library is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "Klangstrom.h"
#if defined(KLST_PANDA_STM32) || defined(KLST_CATERPILLAR_STM32)
#ifdef KLST_PERIPHERAL_ENABLE_ON_BOARD_MIC

#include "Console.h"
#include "dfsdm.h"
#include "Microphone.h"
#include "DMAMemoryAllocator.h"

#ifdef __cplusplus
extern "C" {
#endif

extern DFSDM_Filter_HandleTypeDef hdfsdm1_filter0;
extern DFSDM_Filter_HandleTypeDef hdfsdm1_filter1;

static int32_t*    dma_RX_buffer_filter0 = nullptr;
static int32_t*    dma_RX_buffer_filter1 = nullptr;
static uint32_t    fBufferLength         = 0;
static Microphone* fMicrophone           = nullptr; // TODO this assumes that there is only one microphone
static bool        fChannel0Completed    = false;
static bool        fChannel1Completed    = false;

void microphone_init_BSP(Microphone* microphone) {
    fMicrophone           = microphone;
    fBufferLength         = microphone->audioinfo->block_size * 2;
    dma_RX_buffer_filter0 = static_cast<int32_t*>(dma_malloc(fBufferLength * sizeof(int32_t))); // `* 2` for double buffer
    dma_RX_buffer_filter1 = static_cast<int32_t*>(dma_malloc(fBufferLength * sizeof(int32_t))); // `* 2` for double buffer

    if (dma_RX_buffer_filter0 == nullptr || dma_RX_buffer_filter1 == nullptr) {
        console_error("Microphone: could not allocate memory for DMA buffer");
        return;
    }

    MX_DFSDM1_Init();
#ifdef MICROPHONE_DEBUG
    const float mSampleRateFilter0 = static_cast<float>(SystemCoreClock) / hdfsdm1_channel0.Init.OutputClock.Divider / hdfsdm1_filter0.Init.FilterParam.Oversampling / hdfsdm1_filter0.Init.FilterParam.IntOversampling;
    const float mSampleRateFilter1 = static_cast<float>(SystemCoreClock) / hdfsdm1_channel1.Init.OutputClock.Divider / hdfsdm1_filter1.Init.FilterParam.Oversampling / hdfsdm1_filter1.Init.FilterParam.IntOversampling;
    console_status("Microphone: SystemCoreClock %i", SystemCoreClock);
    console_status("Microphone: computed sample rate: LEFT: %f + RIGHT: %f", mSampleRateFilter0, mSampleRateFilter1);
#endif

    microphone_start(microphone);
}

void microphone_start(Microphone* microphone) {
    if (HAL_DFSDM_FilterRegularStart_DMA(&hdfsdm1_filter0, dma_RX_buffer_filter0, fBufferLength) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_DFSDM_FilterRegularStart_DMA(&hdfsdm1_filter1, dma_RX_buffer_filter1, fBufferLength) != HAL_OK) {
        Error_Handler();
    }
}

void microphone_stop(Microphone* microphone) {
    if (HAL_DFSDM_FilterRegularStop_DMA(&hdfsdm1_filter0) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_DFSDM_FilterRegularStop_DMA(&hdfsdm1_filter1) != HAL_OK) {
        Error_Handler();
    }
}

static void handle_bufferdata(const uint16_t buffer_offset) {
#ifdef MICROPHONE_DEBUG
    static float   max0     = 0.0;
    static float   max1     = 0.0;
    const int32_t* mBuffer0 = dma_RX_buffer_filter0 + buffer_offset;
    const int32_t* mBuffer1 = dma_RX_buffer_filter1 + buffer_offset;

    float sum0             = 0.0;
    float sum1             = 0.0;
    float constexpr mScale = 1.0f / pow(2, 23); // normalize to float range -1.0 to 1.0
    for (uint32_t i = 0; i < fMicrophone->audioinfo->block_size; i++) {
        const int32_t raw_value0 = mBuffer0[i] >> 8;
        const int32_t raw_value1 = mBuffer1[i] >> 8;
        const float   sample0    = static_cast<float>(raw_value0) * mScale;
        const float   sample1    = static_cast<float>(raw_value1) * mScale;
        if (sample0 > 0) {
            sum0 += sample0;
            if (sample0 > max0) {
                max0 = sample0;
            }
        } else {
            sum0 += sample0 * -1;
        }
        if (sample1 > 0) {
            sum1 += sample1;
            if (sample1 > max1) {
                max1 = sample1;
            }
        } else {
            sum1 += sample1 * -1;
        }
    }
    sum0 /= fMicrophone->audioinfo->block_size;
    sum1 /= fMicrophone->audioinfo->block_size;
    console_clear();
    console_println("%09.3f / %09.3f", sum0, sum1);
    console_println("%09.3f / %09.3f", max0, max1);
#else
    if (fMicrophone != nullptr && fMicrophone->audioblock != nullptr && fMicrophone->callback_audioblock != nullptr) {
        const int32_t* mBuffer0 = dma_RX_buffer_filter0 + buffer_offset;
        const int32_t* mBuffer1 = dma_RX_buffer_filter1 + buffer_offset;
        float constexpr mScale  = 1.0f / pow(2, 23); // normalize to float range -1.0 to 1.0
                                                     // ( see "Table 289. Filter maximum output resolution (peak data values from filter output) for some FOSR values"
                                                     // in "RM0468 Reference manual STM32H723/733 ..." p1313 )
        float** mInputBuffer = fMicrophone->audioblock->input;
        for (uint32_t i = 0; i < fMicrophone->audioinfo->block_size; i++) {
            const int32_t mSample0I = mBuffer0[i] >> 8;
            const int32_t mSample1I = mBuffer1[i] >> 8;
            const float   mSample0f = static_cast<float>(mSample0I) * mScale;
            const float   mSample1f = static_cast<float>(mSample1I) * mScale;
            mInputBuffer[0][i]      = mSample0f;
            mInputBuffer[1][i]      = mSample1f;
        }
        fMicrophone->callback_audioblock(fMicrophone->audioblock);
    }
#endif
}

/**
  * @brief  Half regular conversion complete callback.
  * @param  hdfsdm_filter : DFSDM filter handle.
  * @retval None
  */
void HAL_DFSDM_FilterRegConvHalfCpltCallback(DFSDM_Filter_HandleTypeDef* hdfsdm_filter) {
    static constexpr uint16_t mBufferOffset = 0;
    if (hdfsdm_filter == &hdfsdm1_filter0) {
        fChannel0Completed = true;
    }
    if (hdfsdm_filter == &hdfsdm1_filter1) {
        fChannel1Completed = true;
    }
    if (fChannel0Completed && fChannel1Completed) {
        fChannel0Completed = false;
        fChannel1Completed = false;
        handle_bufferdata(mBufferOffset);
    }
}

/**
  * @brief  Regular conversion complete callback.
  * @note   In interrupt mode, user has to read conversion value in this function using HAL_DFSDM_FilterGetRegularValue.
  * @param  hdfsdm_filter : DFSDM filter handle.
  * @retval None
  */
void HAL_DFSDM_FilterRegConvCpltCallback(DFSDM_Filter_HandleTypeDef* hdfsdm_filter) {
    static const uint16_t mBufferOffset = fBufferLength / 2;
    if (hdfsdm_filter == &hdfsdm1_filter0) {
        fChannel0Completed = true;
    }
    if (hdfsdm_filter == &hdfsdm1_filter1) {
        fChannel1Completed = true;
    }
    if (fChannel0Completed && fChannel1Completed) {
        fChannel0Completed = false;
        fChannel1Completed = false;
        handle_bufferdata(mBufferOffset);
    }
}

#ifdef __cplusplus
}
#endif

#endif // KLST_PERIPHERAL_ENABLE_ON_BOARD_MIC
#endif // defined(KLST_PANDA_STM32) || defined(KLST_CATERPILLAR_STM32)