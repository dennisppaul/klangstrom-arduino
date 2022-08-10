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

#ifndef KlangstromApplicationArduino_h
#define KlangstromApplicationArduino_h

/* --- application-related callback functions --- 
 *
 * implemented in applicaton
 *
 * see also `KlangstromApplication.hpp`
 *
 */

#ifdef __cplusplus

#include "KlangstromDefines.hpp"
#include "KlangstromDefinesArduino.h"

WEAK void setup();
WEAK void loop();
WEAK void beat(uint32_t pBeat);
WEAK void event_receive(const EVENT_TYPE event, const float* data); // @todo(as cool as it is to use *raw* arrays, consider using `std::array<float, ?>`)
WEAK void data_receive(const uint8_t receiver, uint8_t* data, uint8_t length);

WEAK void audioblock(SIGNAL_TYPE* pLeftTX, SIGNAL_TYPE* pRightTX, SIGNAL_TYPE* pLeftRX, SIGNAL_TYPE* pRightRX);

#endif // __cplusplus

#endif /* KlangstromApplicationArduino_h */
