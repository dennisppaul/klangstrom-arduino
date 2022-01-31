//
//  klangstrom_arduino_proxy.cpp
//  Klang – a node+text-based synthesizer library
//
//
//

#include "klangstrom_arduino_proxy_serial.h"

#include <fstream>
#include <iostream>

#ifndef KLANG_NL
#if defined(KLANG_TARGET_MCU)
#define KLANG_NL "\n\r"
#else
#define KLANG_NL "\n"
#endif
#endif

#define PRINT_ARDUINO_PROXY_SERIAL

#ifdef PRINT_ARDUINO_PROXY_SERIAL
#define KLST_CONSOLE(...)    \
    if (mIsInitialized) {    \
        printf(__VA_ARGS__); \
    }
#define KLST_CONSOLE_LN(...) \
    if (mIsInitialized) {    \
        printf(__VA_ARGS__); \
        printf(KLANG_NL);    \
    }
#else
#define KLST_CONSOLE(...)
#define KLST_CONSOLE_LN(...)
#endif

// #define DEBUG_ARDUINO_PROXY_SERIAL

#ifdef DEBUG_ARDUINO_PROXY_SERIAL
#define KLST_LOG_APS(...) \
    printf(__VA_ARGS__);  \
    printf(" :: ");
#else
#define KLST_LOG_APS(...)
#endif

using namespace std;

size_t SerialProxy::printf(const char *format, ...) {
    char    _buffer[SERIAL_PROXY_BUFFER_SIZE];
    va_list args;
    va_start(args, format);
    int r = vsprintf(_buffer, format, args);
    cout << _buffer << flush;
    va_end(args);
    return r;
}

void SerialProxy::begin(unsigned long pBaud) {
    KLST_LOG_APS("<begin>");
    mIsInitialized = true;
}

size_t SerialProxy::write(const char *str) {
    KLST_LOG_APS("<write>");
    KLST_LOG_APS("char*");
    KLST_CONSOLE("%s", str);
    return 0;
}

size_t SerialProxy::write(const char *c, size_t size) {
    KLST_LOG_APS("<write>");
    KLST_LOG_APS("__FlashStringHelper");
    KLST_CONSOLE("%s", c);
    return 0;
}

size_t SerialProxy::print(const __FlashStringHelper *) {
    KLST_LOG_APS("<print>");
    KLST_LOG_APS("<FlashString>");
    KLST_LOG_APS("__FlashStringHelper");
    return 0;
}

size_t SerialProxy::print(const String &s) {
    KLST_LOG_APS("<print>");
    KLST_LOG_APS("<String>");
    KLST_CONSOLE("%s", s.c_str());
    return 0;
}

size_t SerialProxy::print(const char c[]) {
    KLST_LOG_APS("<print>");
    KLST_LOG_APS("<char[]>");
    KLST_CONSOLE("%s", c);
    return 0;
}

size_t SerialProxy::print(char c) {
    KLST_LOG_APS("<print>");
    KLST_LOG_APS("<char>");
    KLST_CONSOLE("%c", c);
    return 0;
}

size_t SerialProxy::print(unsigned char c, int) {
    KLST_LOG_APS("<print>");
    KLST_LOG_APS("<unsigned char, int>");
    KLST_CONSOLE("%c", c);
    return 0;
}

size_t SerialProxy::print(int i, int) {
    KLST_LOG_APS("<print>");
    KLST_LOG_APS("<int>");
    KLST_CONSOLE("%i", i);
    return 0;
}

size_t SerialProxy::print(unsigned int i, int) {
    KLST_LOG_APS("<print>");
    KLST_LOG_APS("<unsigned int>");
    KLST_CONSOLE("%i", i);
    return 0;
}

size_t SerialProxy::print(long l, int) {
    KLST_LOG_APS("<print>");
    KLST_LOG_APS("<long>");
    KLST_CONSOLE("%ld", l);
    return 0;
}

size_t SerialProxy::print(unsigned long l, int) {
    KLST_LOG_APS("<print>");
    KLST_LOG_APS("<long, int>");
    KLST_CONSOLE("%lu", l);
    return 0;
}

size_t SerialProxy::print(double d, int) {
    KLST_LOG_APS("<print>");
    KLST_LOG_APS("<double>");
    KLST_CONSOLE("%lf", d);
    return 0;
}

size_t SerialProxy::println(const __FlashStringHelper *) {
    KLST_LOG_APS("<println>");
    KLST_LOG_APS("<FlashString>");
    KLST_CONSOLE_LN("__FlashStringHelper");
    return 0;
}

size_t SerialProxy::println(const String &s) {
    KLST_LOG_APS("<println>");
    KLST_LOG_APS("string");
    println(s.c_str());
    return 0;
}

size_t SerialProxy::println(const char c[]) {
    KLST_LOG_APS("<println>");
    KLST_LOG_APS("char*");
    KLST_CONSOLE_LN("%s", c);
    return 0;
}

size_t SerialProxy::println(char c) {
    KLST_LOG_APS("<println>");
    KLST_LOG_APS("char");
    KLST_CONSOLE_LN("%c", c);
    return 0;
}

size_t SerialProxy::println(unsigned char c, int i) {
    KLST_LOG_APS("<println>");
    KLST_LOG_APS("unsigned char");
    KLST_CONSOLE_LN("%c", c);
    return 0;
}

size_t SerialProxy::println(int i, int) {
    KLST_LOG_APS("<println>");
    KLST_LOG_APS("int");
    KLST_CONSOLE_LN("%i", i);
    return 0;
}

size_t SerialProxy::println(unsigned int i, int) {
    KLST_LOG_APS("<println>");
    KLST_LOG_APS("unsigned int");
    KLST_CONSOLE_LN("%i", i);
    return 0;
}

size_t SerialProxy::println(long l, int) {
    KLST_LOG_APS("<println>");
    KLST_LOG_APS("long");
    KLST_CONSOLE_LN("%ld", l);
    return 0;
}

size_t SerialProxy::println(unsigned long l, int) {
    KLST_LOG_APS("<println>");
    KLST_LOG_APS("unsigned long");
    KLST_CONSOLE_LN("%lu", l);
    return 0;
}

size_t SerialProxy::println(double d, int) {
    KLST_LOG_APS("<println>");
    KLST_LOG_APS("double");
    KLST_CONSOLE_LN("%lf", d);
    return 0;
}

size_t SerialProxy::println(void) {
    KLST_LOG_APS("<println>");
    KLST_CONSOLE_LN("");
    return 0;
}

SerialProxy Serial;
