/*
 * Klangstrom
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2023 Dennis P Paul.
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

#ifndef klangstrom_arduino_sdl_h
#define klangstrom_arduino_sdl_h

#include <vector>

#include "KLST_Emulator.h"
#include "KlangstromApplicationInterface.h"
#include "KlangstromCallback.h"
#include "KlangstromDefines.hpp"
#include "KlangstromDefinesArduino.h"
#include "SDL_render.h"

const uint16_t SCREEN_WIDTH                    = 1024;
const uint16_t SCREEN_HEIGHT                   = 768;
const uint16_t OSC_TRANSMIT_OUTPUT_BUFFER_SIZE = 1024;

//@todo maybe merge this into `KLST_SDL-adapter.h`
void klangstrom_arduino_beats_per_minute(float BPM);
void klangstrom_arduino_beats_per_minute_ms(uint32_t micro_seconds);
bool get_encoder_button_state(uint8_t encoder);

void init_main();

void init_audio_output();

void init_audio_input();

void init_SDL();

void init_renderer();

void init_card();

void init_app();

void loop_renderer();

void loop_event();

void start_audio();

void list_audio_devices();

void update_audiobuffer();

void shutdown_main();

/* klangstrom::KlangstromDisplay */

void registerKlangstromDisplayCallback(klangstrom::KlangstromCallback* pDisplayEventListener);

SDL_Renderer* getSDLRenderer();

uint16_t getSDLRendererWidth();

uint16_t getSDLRendererHeight();

uint16_t getKlangstromDisplayPosX();

uint16_t getKlangstromDisplayPosY();

/* klang::KlangApplicationInterface */

void klangstrom_arduino_event_transmit(EVENT_TYPE pEvent, float* data);
void klangstrom_arduino_data_transmit(const uint8_t sender, uint8_t* data, uint16_t length);
void klangstrom_arduino_sim_transmit(std::vector<float>& pData);

/* serial */
void klangstrom_arduino_emu_transmit_serial(int pPort, int pData);

#endif /* klangstrom_arduino_sdl_h */
