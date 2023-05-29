//
//  KLST_SDL-adapter.cpp
//  Klangstrom – a node+text-based synthesizer library
//
//
//

#include "KLST_SDL-adapter.h"

#include <chrono>

#include "Arduino.h"
#include "KLST_Emulator.h"
#include "KLST_SDL-includes.h"

using namespace std;

#ifdef DEBUG_ARDUINO_PROXY
#define KLST_LOG_AP(...) (cout << __VA_ARGS__ << endl);
#else
#define KLST_LOG_AP(...)
#endif

KLST_Emulator mEmulator;

uint32_t analogRead(uint32_t pPin) {
    return mEmulator.analogRead(pPin);
}

int digitalRead(uint32_t pPin) {
    return mEmulator.digitalRead(pPin);
}

void digitalWrite(uint32_t pPin, uint32_t pValue) {
    KLST_LOG_AP("<digitalWrite(%i << %i)> " << pPin << ", " << pValue);
    mEmulator.digitalWrite(pPin, pValue);
}

void pinMode(uint32_t pPin, uint32_t pMode) {
    KLST_LOG_AP("<pinMode(%i << %i)> " << pPin << ", " << pMode);
}

void interrupts() {}

void noInterrupts() {}

auto mAppStartTime = std::chrono::steady_clock::now();

uint32_t micros() {
    const auto mCurrentTime = std::chrono::steady_clock::now();
    const auto mDelta       = mCurrentTime - mAppStartTime;
    return std::chrono::duration_cast<std::chrono::microseconds>(mDelta).count();
}

uint32_t millis() {
    return micros() / 1000;
}
