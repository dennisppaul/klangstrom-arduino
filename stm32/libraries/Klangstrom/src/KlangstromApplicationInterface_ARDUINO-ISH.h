/*
 * Klangstrom
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2022 Dennis P Paul.
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

#ifndef KlangstromApplicationInterface_ARDUINO_ISH_h
#define KlangstromApplicationInterface_ARDUINO_ISH_h

#ifdef __cplusplus
#include "KlangstromDefines.hpp"
#include "KlangstromEvents.h"
#endif
#include "KlangstromDefinesArduino.h"

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------------------------------------------------------- */
/* --- internal shared (ISH) functions --------------------------------------------------------- */
/* --------------------------------------------------------------------------------------------- */

void KLST_ISH_jump_to_bootloader();
void KLST_ISH_loop();
void KLST_ISH_post_setup();
void KLST_ISH_pre_setup();
void KLST_ISH_fill_buffer(uint32_t* pTXBuffer, uint32_t* pRXBuffer, uint16_t pBufferLength);
void KLST_ISH_shutdown();
void KLST_ISH_handle_encoders();
void KLST_ISH_handleSerialPorts();

uint32_t KLST_ISH_OPT_audio_line();
bool     KLST_ISH_OPT_audio_input_enabled();
float    KLST_ISH_OPT_headphone_output_volume();

/* --------------------------------------------------------------------------------------------- */
/* --- board-specific (BSP) functions ---------------------------------------------------------- */
/* --------------------------------------------------------------------------------------------- */

uint32_t KLST_BSP_boot_address();
uint32_t KLST_BSP_U_ID_address();
void     KLST_BSP_configure_audio_codec();
void     KLST_BSP_init_encoders();
void     KLST_BSP_deinit_encoders();
void     KLST_BSP_start_audio_codec();
void     KLST_BSP_shutdown();
void     KLST_BSP_init_peripherals();
void     KLST_BSP_init_LEDs();
void     KLST_BSP_configure_TinyUSB();
uint8_t  KLST_BSP_error_code();

#ifdef __cplusplus
}
#endif

#endif /* KlangstromApplicationInterface_ARDUINO_ISH_h */
