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
#ifdef KLST_PERIPHERAL_ENABLE_LEDS
#if defined(KLST_PANDA_STM32) || defined(KLST_CATERPILLAR_STM32)

#include <cinttypes>
#include "tim.h"

#ifdef __cplusplus
extern "C" {
#endif

void led_init_BSP() {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, _LED_00_Pin | _LED_01_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pins : _LED_00_Pin _LED_01_Pin */
    GPIO_InitStruct.Pin   = _LED_00_Pin | _LED_01_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* --- */

    MX_TIM12_Init();

    /* --- */

    if (HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_2) != HAL_OK) {
        Error_Handler();
    }
}

uint8_t led_total_BSP() {
    return KLST_PANDA_STM32_NUM_LEDS;
}

void led_set_BSP(const int id, float intensity) {
    const uint32_t mDutyCycle = static_cast<uint32_t>(65535 * intensity);
    if (id == 0) {
        __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1, mDutyCycle);
    } else if (id == 1) {
        __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_2, mDutyCycle);
    }
}

#ifdef __cplusplus
}
#endif

#endif // KLST_PANDA_STM32
#endif // KLST_PERIPHERAL_ENABLE_LEDS