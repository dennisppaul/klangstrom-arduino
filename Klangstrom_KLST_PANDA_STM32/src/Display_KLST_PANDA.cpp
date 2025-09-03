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
#ifdef KLST_PERIPHERAL_ENABLE_DISPLAY
#ifdef KLST_PANDA_STM32

#ifndef KLST_PERIPHERAL_ENABLE_EXTERNAL_MEMORY
#error "KLST_PERIPHERAL_ENABLE_EXTERNAL_MEMORY must be defined for display"
#endif

#include "main.h"
#include "tim.h"
#include "ltdc.h"
#include "Display.h"
#include "Display_KLST_PANDA.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

extern TIM_HandleTypeDef htim3;

static uint32_t frame_counter = 0;

int16_t display_get_width() {
    return KLST_DISPLAY_WIDTH;
}

int16_t display_get_height() {
    return KLST_DISPLAY_HEIGHT;
}

void display_backlight_init() {
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
}

void display_set_backlight(const float brightness) {
    const uint32_t mPeriod = htim3.Init.Period;
    uint32_t       mPhase  = static_cast<uint32_t>(mPeriod * brightness);
    mPhase                 = MAX(1, MIN(mPeriod, mPhase));
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, mPhase > 0 ? mPhase : 1);
}

bool display_init_BSP(const TouchPanelMode touch_panel_mode) {
    /* display = LTDC + DMA2D + backlight + touch panel */
    MX_LTDC_Init();
    // MX_DMA2D_Init();
    MX_TIM3_Init(); // backlight

    display_switch_off();
    display_LTDC_init();
    display_renderer_init();
    display_switch_on();

    touch_init(touch_panel_mode);

    display_backlight_init();
    display_set_backlight(0.5f);

    return true;
}

void display_switch_on() {
    HAL_GPIO_WritePin(_DISPLAY_ON_OFF_GPIO_Port, _DISPLAY_ON_OFF_Pin, GPIO_PIN_SET);
}

void display_switch_off() {
    HAL_GPIO_WritePin(_DISPLAY_ON_OFF_GPIO_Port, _DISPLAY_ON_OFF_Pin, GPIO_PIN_RESET);
}

#ifdef __cplusplus
}
#endif

#endif // KLST_PANDA_STM32
#endif // KLST_PERIPHERAL_ENABLE_DISPLAY
