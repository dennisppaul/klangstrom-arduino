/*
 * Klangstrom
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2025 Dennis P Paul.
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

#include <iostream>
#include <cstdint>
#include <cctype>
#include <cmath>

#include "Arduino.h"
#include "KlangstromEmulator.h"

template<typename T>
T abs(T value);

template<typename T>
T constrain(T value, T min, T max) {
    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }
    return value;
}
long map(long value, long fromLow, long fromHigh, long toLow, long toHigh);

template<typename T>
T max(T a, T b) { return (a > b) ? a : b; }

template<typename T>
T      min(T a, T b) { return (a < b) ? a : b; }
double pow(double base, double exponent); // from cmath

template<typename T>
T      sq(T value) { return value * value; }
double sqrt(double value); // from cmath

void delay(const uint32_t milliseconds) {
    umfeld::KlangstromEmulator::instance()->delay_loop(milliseconds * 1000);
}

void delayMicroseconds(const uint32_t microseconds) {
    umfeld::KlangstromEmulator::instance()->delay_loop(microseconds);
}

uint32_t micros() {
    static auto startTime   = std::chrono::steady_clock::now();
    auto        currentTime = std::chrono::steady_clock::now();
    auto        duration    = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - startTime).count();
    return static_cast<uint32_t>(duration);
}

uint32_t millis() {
    return micros() / 1000;
}

template<typename T>
T abs(T value) {
    return (value < 0) ? -value : value;
}

template<typename T>
T mapT(const T value,
       const T start0,
       const T stop0,
       const T start1,
       const T stop1) {
    const T a = value - start0;
    const T b = stop0 - start0;
    const T c = stop1 - start1;
    const T d = a / b;
    const T e = d * c;
    return e + start1;
}

float mapf(const float value, const float start0, const float stop0, const float start1, const float stop1) {
    return mapT<float>(value, start0, stop0, start1, stop1);
}

long map(const long value, const long fromLow, const long fromHigh, const long toLow, const long toHigh) {
    return (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
}

bool isAlpha(const char c) {
    return std::isalpha(static_cast<unsigned char>(c));
}

bool isAlphaNumeric(const char c) {
    return std::isalnum(static_cast<unsigned char>(c));
}

bool isAscii(const char c) {
    return static_cast<unsigned char>(c) <= 127;
}

bool isControl(const char c) {
    return std::iscntrl(static_cast<unsigned char>(c));
}

bool isDigit(const char c) {
    return std::isdigit(static_cast<unsigned char>(c));
}

bool isGraph(const char c) {
    return std::isgraph(static_cast<unsigned char>(c));
}

bool isHexadecimalDigit(const char c) {
    return std::isxdigit(static_cast<unsigned char>(c));
}

bool isLowerCase(const char c) {
    return std::islower(static_cast<unsigned char>(c));
}

bool isPrintable(const char c) {
    return std::isprint(static_cast<unsigned char>(c));
}

bool isPunct(const char c) {
    return std::ispunct(static_cast<unsigned char>(c));
}

bool isSpace(const char c) {
    return std::isspace(static_cast<unsigned char>(c));
}

bool isUpperCase(const char c) {
    return std::isupper(static_cast<unsigned char>(c));
}

bool isWhitespace(const char c) {
    return std::isspace(static_cast<unsigned char>(c));
}

long random(const long max) {
    if (max == 0) {
        return 0;
    }
    return std::rand() % max;
}

long random(const long min, const long max) {
    if (min >= max) {
        return min;
    }
    long range = max - min;
    return min + std::rand() % range;
}

void randomSeed(const uint32_t seed) {
    std::srand(seed);
}

uint8_t bit(const uint8_t n) {
    return 1 << n;
}

uint8_t bitClear(uint8_t value, const uint8_t bit) {
    value &= ~(1 << bit);
    return value;
}

bool bitRead(const uint8_t value, const uint8_t bit) {
    return (value >> bit) & 1;
}

uint8_t bitSet(uint8_t value, const uint8_t bit) {
    value |= (1 << bit);
    return value;
}

uint8_t bitWrite(uint8_t value, const uint8_t bit, const bool bitValue) {
    if (bitValue) {
        value = bitSet(value, bit);
    } else {
        value = bitClear(value, bit);
    }
    return value;
}

uint8_t highByte(const uint16_t value) {
    return value >> 8;
}

uint8_t lowByte(const uint16_t value) {
    return value & 0xFF;
}

void attachInterrupt() {}

void detachInterrupt() {}

void digitalPinToInterrupt() {}

void interrupts() {}

void noInterrupts() {}
