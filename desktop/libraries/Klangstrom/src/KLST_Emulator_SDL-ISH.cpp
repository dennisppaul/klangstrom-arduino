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

#include "KlangstromDefines.hpp"

#if (KLST_ARCH==KLST_ARCH_DESKTOP)

#include "KLST_Emulator.h"
#include "klangstrom_arduino_sdl.h"
#include "KlangstromEvents.h"
#include "KlangstromApplicationArduino.h"

using namespace klangstrom;

void KLST_Emulator::led(uint8_t pLED, bool pState) {
	const uint8_t mLED = pLED - LED_00; 
	const uint8_t mID = (mLED >= NUMBER_OF_LEDS) ? (NUMBER_OF_LEDS-1) : mLED;
	mLEDs[mID] = pState;

	std::vector<float> mData;
	mData.push_back(KLST_OSC_SIM_LED);
	mData.push_back(0);
	mData.push_back(0);
	klangstrom_arduino_sim_transmit(mData);
}

void KLST_Emulator::led_toggle(uint8_t pLED) {
	const uint8_t mLED = pLED - LED_00; 
	const uint8_t mID = (mLED >= NUMBER_OF_LEDS) ? (NUMBER_OF_LEDS-1) : mLED;
	const bool mState = !mLEDs[mID]; 
	led(pLED, mState);
}

int KLST_Emulator::digitalRead(uint32_t pPin) {  
	uint8_t mID = (pPin >= NUMBER_OF_PINS) ? (NUMBER_OF_PINS-1) : pPin;
	return mPins[mID];
}

void KLST_Emulator::digitalWrite(uint32_t pPin, int8_t pValue) {
	uint8_t mID = (pPin >= NUMBER_OF_PINS) ? (NUMBER_OF_PINS-1) : pPin;
	mPins[mID] = pValue;

	std::vector<float> mData;
	mData.push_back(KLST_OSC_SIM_DIGITAL_WRITE);
	mData.push_back(mID);
	mData.push_back(pValue);
	klangstrom_arduino_sim_transmit(mData);
}

void KLST_Emulator::process_receive(const osc::ReceivedMessage &msg) {
	/**
	 * typetag has to start with a single integer ( message type `KLST_OSC_SIM_` ) 
	 * followed by one or more floats ( date ).
	 * 
	 * the message type defines what kind of event or function will be triggered.
	 */
	if (msg.ArgumentCount() > 0 && str_begins_with("if", msg.TypeTags())) {
		const uint8_t mLength = msg.ArgumentCount() - 1;
		osc::ReceivedMessage::const_iterator arg = msg.ArgumentsBegin();
		const uint8_t mMessageType = (uint8_t)arg->AsInt32();
		float mData[mLength];
		uint8_t i = 0;
		++arg;
		while(arg != msg.ArgumentsEnd()) {
			mData[i] = arg->AsFloat();
			++arg;
			++i;
		}
		switch(mMessageType) {
			case KLST_OSC_SIM_LED:
				if (mLength == 2) {
					led((uint8_t) mData[0], (uint8_t)mData[1]);
				}
				break;
			case KLST_OSC_SIM_DIGITAL_WRITE:
				if (mLength == 2) {
					digitalWrite((uint32_t) mData[0], (int8_t)mData[1]);
				}
				break;
			case EVENT_ENCODER_ROTATED:
				if (mLength == 3 && str_equals(msg.TypeTags(), "ifff")) {
					event_receive(EVENT_ENCODER_ROTATED, mData);
				} else {
					emit_message_error("EVENT_ENCODER_ROTATED", "ifff", msg);
				}
				break;
			case EVENT_ENCODER_BUTTON_PRESSED:
				if (mLength == 1 && str_equals(msg.TypeTags(), "if")) {
					event_receive(EVENT_ENCODER_BUTTON_PRESSED, mData);
				} else {
					emit_message_error("EVENT_ENCODER_BUTTON_PRESSED", "if", msg);
				}
				break;
			case EVENT_ENCODER_BUTTON_RELEASED:
				if (mLength == 1 && str_equals(msg.TypeTags(), "if")) {
					event_receive(EVENT_ENCODER_BUTTON_RELEASED, mData);
				} else {
					emit_message_error("EVENT_ENCODER_BUTTON_RELEASED", "if", msg);
				}
				break;
			default:
			//@todo(consider to pass *unrecognized* message to a callback function )
				break;
		}
	}   
}

bool* KLST_Emulator::getLEDs() {
	return mLEDs;
}

bool KLST_Emulator::str_begins_with(const char *pre, const char *str) {
	size_t lenpre = strlen(pre),
		lenstr = strlen(str);
	return lenstr < lenpre ? false : memcmp(pre, str, lenpre) == 0;
}

bool KLST_Emulator::str_equals(const char *pStringA, const char *pStringB) {
	return (strcmp(pStringA, pStringB) == 0);
}

void KLST_Emulator::emit_message_error(const char *pMessage, const char *pTypeTag, const osc::ReceivedMessage &msg) {
	KLST_LOG.print("+++ ERROR / ");
	KLST_LOG.print("+++ malformed OSC message: ");
	KLST_LOG.print(pMessage);
	KLST_LOG.print(" : ");
	KLST_LOG.print(pTypeTag);
	KLST_LOG.print(" > ");
	KLST_LOG.print(msg.AddressPattern());
	KLST_LOG.print(" : ");
	KLST_LOG.print(msg.TypeTags());
	KLST_LOG.println();
}

#endif // (KLST_ARCH==KLST_ARCH_DESKTOP)
