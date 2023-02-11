//
//  klangstrom_arduino_proxy.cpp
//  Klang – a node+text-based synthesizer library
//
//
//

#include "klangstrom_arduino_proxy_serial.h"
extern void klangstrom_arduino_emu_transmit_serial(int pPort, int pData);

#include <stdarg.h>

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
        printf("> "); \
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

#define SERIAL_PORT_ID 0

using namespace std;

// @TODO(implement `print` functions with `sprintf(finalarray, "%i", value);` )

size_t SerialProxy::printf(const char *format, ...) {
    char    _buffer[SERIAL_PROXY_BUFFER_SIZE];
    va_list args;
    va_start(args, format);
    int r = vsnprintf(_buffer, SERIAL_PROXY_BUFFER_SIZE, format, args);
    cout << _buffer << std::flush;
    va_end(args);
    return r;
}

void SerialProxy::begin(unsigned long pBaud) {
    KLST_LOG_APS("<begin>");
    mIsInitialized = true;
}

size_t SerialProxy::write(uint8_t c) {
    KLST_LOG_APS("<write>");
    KLST_LOG_APS("char");
    // KLST_CONSOLE("%c", c);
    klangstrom_arduino_emu_transmit_serial(SERIAL_PORT_ID, c);
    return 0;
}

size_t SerialProxy::write(const uint8_t *buffer, size_t size) {
    KLST_LOG_APS("<write>");
    KLST_LOG_APS("char*");
    for (size_t i = 0; i < size; i++) {
        write(buffer[i]);
    }
    return 0;
}

size_t SerialProxy::write(const char *str) {
    if (str == NULL) {
        return 0;
    }
    return write((const uint8_t *)str, strlen(str));
}

size_t SerialProxy::write(const char *buffer, size_t size) {
    return write((const uint8_t *)buffer, size);
}

// int printf(const char *format, ...);
// int printf(const __FlashStringHelper *format, ...);

#define F_CHAR_BUFFER_SIZE 128
const char *F_NEWLINE = "\n\r";

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
    char mBuffer[F_CHAR_BUFFER_SIZE];
    int  mNumChars = snprintf(mBuffer, F_CHAR_BUFFER_SIZE, "%s", s.c_str());
    write(mBuffer, mNumChars);
    return mNumChars;
}

size_t SerialProxy::print(const char c[]) {
    KLST_LOG_APS("<print>");
    KLST_LOG_APS("<char[]>");
    KLST_CONSOLE("%s", c);
    char mBuffer[F_CHAR_BUFFER_SIZE];
    int  mNumChars = snprintf(mBuffer, F_CHAR_BUFFER_SIZE, "%s", c);
    write(mBuffer, mNumChars);
    return mNumChars;
}

size_t SerialProxy::print(char c) {
    KLST_LOG_APS("<print>");
    KLST_LOG_APS("<char>");
    KLST_CONSOLE("%c", c);
    write(c);
    return 1;
}

size_t SerialProxy::print(unsigned char c, int) {
    KLST_LOG_APS("<print>");
    KLST_LOG_APS("<unsigned char, int>");
    KLST_CONSOLE("%c", c);
    char mBuffer[F_CHAR_BUFFER_SIZE];
    int  mNumChars = snprintf(mBuffer, F_CHAR_BUFFER_SIZE, "%c", c);
    write(mBuffer, mNumChars);
    return mNumChars;
}

size_t SerialProxy::print(int i, int) {
    KLST_LOG_APS("<print>");
    KLST_LOG_APS("<int>");
    KLST_CONSOLE("%i", i);
    char mBuffer[F_CHAR_BUFFER_SIZE];
    int  mNumChars = snprintf(mBuffer, F_CHAR_BUFFER_SIZE, "%i", i);
    write(mBuffer, mNumChars);
    return mNumChars;
}

size_t SerialProxy::print(unsigned int i, int) {
    KLST_LOG_APS("<print>");
    KLST_LOG_APS("<unsigned int>");
    KLST_CONSOLE("%i", i);
    char mBuffer[F_CHAR_BUFFER_SIZE];
    int  mNumChars = snprintf(mBuffer, F_CHAR_BUFFER_SIZE, "%i", i);
    write(mBuffer, mNumChars);
    return mNumChars;
}

size_t SerialProxy::print(long l, int) {
    KLST_LOG_APS("<print>");
    KLST_LOG_APS("<long>");
    KLST_CONSOLE("%ld", l);
    char mBuffer[F_CHAR_BUFFER_SIZE];
    int  mNumChars = snprintf(mBuffer, F_CHAR_BUFFER_SIZE, "%ld", l);
    write(mBuffer, mNumChars);
    return mNumChars;
}

size_t SerialProxy::print(unsigned long l, int) {
    KLST_LOG_APS("<print>");
    KLST_LOG_APS("<long, int>");
    KLST_CONSOLE("%lu", l);
    char mBuffer[F_CHAR_BUFFER_SIZE];
    int  mNumChars = snprintf(mBuffer, F_CHAR_BUFFER_SIZE, "%lu", l);
    write(mBuffer, mNumChars);
    return mNumChars;
}

size_t SerialProxy::print(double d, int) {
    KLST_LOG_APS("<print>");
    KLST_LOG_APS("<double>");
    KLST_CONSOLE("%lf", d);
    char mBuffer[F_CHAR_BUFFER_SIZE];
    int  mNumChars = snprintf(mBuffer, F_CHAR_BUFFER_SIZE, "%lf", d);
    write(mBuffer, mNumChars);
    return mNumChars;
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
    return println(s.c_str());
}

size_t SerialProxy::println(const char c[]) {
    KLST_LOG_APS("<println>");
    KLST_LOG_APS("char*");
    KLST_CONSOLE_LN("%s", c);
    char mBuffer[F_CHAR_BUFFER_SIZE];
    int  mNumChars = snprintf(mBuffer, F_CHAR_BUFFER_SIZE, "%s%s", c, F_NEWLINE);
    write(mBuffer, mNumChars);
    return mNumChars;
}

size_t SerialProxy::println(char c) {
    KLST_LOG_APS("<println>");
    KLST_LOG_APS("char");
    KLST_CONSOLE_LN("%c", c);
    char mBuffer[F_CHAR_BUFFER_SIZE];
    int  mNumChars = snprintf(mBuffer, F_CHAR_BUFFER_SIZE, "%c%s", c, F_NEWLINE);
    write(mBuffer, mNumChars);
    return mNumChars;
}

size_t SerialProxy::println(unsigned char c, int i) {
    KLST_LOG_APS("<println>");
    KLST_LOG_APS("unsigned char");
    KLST_CONSOLE_LN("%c", c);
    char mBuffer[F_CHAR_BUFFER_SIZE];
    int  mNumChars = snprintf(mBuffer, F_CHAR_BUFFER_SIZE, "%c%s", c, F_NEWLINE);
    write(mBuffer, mNumChars);
    return mNumChars;
}

size_t SerialProxy::println(int i, int) {
    KLST_LOG_APS("<println>");
    KLST_LOG_APS("int");
    KLST_CONSOLE_LN("%i", i);
    char mBuffer[F_CHAR_BUFFER_SIZE];
    int  mNumChars = snprintf(mBuffer, F_CHAR_BUFFER_SIZE, "%i%s", i, F_NEWLINE);
    write(mBuffer, mNumChars);
    return mNumChars;
}

size_t SerialProxy::println(unsigned int i, int) {
    KLST_LOG_APS("<println>");
    KLST_LOG_APS("unsigned int");
    KLST_CONSOLE_LN("%i", i);
    char mBuffer[F_CHAR_BUFFER_SIZE];
    int  mNumChars = snprintf(mBuffer, F_CHAR_BUFFER_SIZE, "%i%s", i, F_NEWLINE);
    write(mBuffer, mNumChars);
    return mNumChars;
}

size_t SerialProxy::println(long l, int) {
    KLST_LOG_APS("<println>");
    KLST_LOG_APS("long");
    KLST_CONSOLE_LN("%ld", l);
    char mBuffer[F_CHAR_BUFFER_SIZE];
    int  mNumChars = snprintf(mBuffer, F_CHAR_BUFFER_SIZE, "%ld%s", l, F_NEWLINE);
    write(mBuffer, mNumChars);
    return mNumChars;
}

size_t SerialProxy::println(unsigned long l, int) {
    KLST_LOG_APS("<println>");
    KLST_LOG_APS("unsigned long");
    KLST_CONSOLE_LN("%lu", l);
    char mBuffer[F_CHAR_BUFFER_SIZE];
    int  mNumChars = snprintf(mBuffer, F_CHAR_BUFFER_SIZE, "%lu%s", l, F_NEWLINE);
    write(mBuffer, mNumChars);
    return mNumChars;
}

size_t SerialProxy::println(double d, int) {
    KLST_LOG_APS("<println>");
    KLST_LOG_APS("double");
    KLST_CONSOLE_LN("%lf", d);
    char mBuffer[F_CHAR_BUFFER_SIZE];
    int  mNumChars = snprintf(mBuffer, F_CHAR_BUFFER_SIZE, "%lf%s", d, F_NEWLINE);
    write(mBuffer, mNumChars);
    return mNumChars;
}

size_t SerialProxy::println(void) {
    KLST_LOG_APS("<println>");
    KLST_CONSOLE_LN("");
    char mBuffer[F_CHAR_BUFFER_SIZE];
    int  mNumChars = snprintf(mBuffer, F_CHAR_BUFFER_SIZE, "%s", F_NEWLINE);
    write(mBuffer, mNumChars);
    return mNumChars;
}

SerialProxy Serial;
