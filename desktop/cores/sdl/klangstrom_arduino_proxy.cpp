//
//  klangstrom_arduino_proxy.cpp 
//  Klang – a node+text-based synthesizer library
//
//
//

#include "klangstrom_arduino_proxy.h"

// #define DEBUG_ARDUINO_PROXY

// using namespace std;
// 
// #ifdef DEBUG_ARDUINO_PROXY
// #define  KLST_LOG_AP(...)       (cout << __VA_ARGS__ << endl);
// #else
// #define  KLST_LOG_AP(...)
// #endif
// 
// uint16_t mDelayMilliSeconds = 0;
// 
// // void delay(uint32_t pMS) { 
// //     KLST_LOG_AP("<delay(%i)> " << pMS);
// //     mDelayMilliSeconds = pMS; 
// // }
// 
// int digitalRead(uint32_t pPin) {  
//     KLST_LOG_AP("<digitalRead(%i)> " << pPin);
//     uint8_t mID = (pPin >= NUMBER_OF_PINS) ? (NUMBER_OF_PINS-1) : pPin;
//     return mPins[mID];
// }
// 
// void digitalWrite(uint32_t pPin, uint32_t pValue) { 
//     KLST_LOG_AP("<digitalWrite(%i << %i)> " << pPin << ", " << pValue);
//     uint8_t mID = (pPin >= NUMBER_OF_PINS) ? (NUMBER_OF_PINS-1) : pPin;
//     mPins[mID] = pValue;
// }
// 
// void pinMode(uint32_t pPin, uint32_t pMode) { 
//     KLST_LOG_AP("<pinMode(%i << %i)> " << pPin << ", " << pMode);
// }

// uint16_t get_delay() {
//     return mDelayMilliSeconds;
// }