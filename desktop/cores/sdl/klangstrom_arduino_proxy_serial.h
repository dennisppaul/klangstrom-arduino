//
//  klangstrom_arduino_proxy_serial.h
//  Klangstrom
//
//
//

#ifndef klangstrom_arduino_proxy_serial_h
#define klangstrom_arduino_proxy_serial_h

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <iostream>

#include "WString.h"

using namespace std;

class SerialProxy {
#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

#define SERIAL_PROXY_BUFFER_SIZE 64

public:
    void begin(unsigned long pBaud);

    size_t write(const char *str);
    size_t write(const char *buffer, size_t size);

    size_t print(const __FlashStringHelper *);
    size_t print(const String &c);
    size_t print(const char[]);
    size_t print(char);
    size_t print(unsigned char, int = DEC);
    size_t print(int, int = DEC);
    size_t print(unsigned int, int = DEC);
    size_t print(long, int = DEC);
    size_t print(unsigned long, int = DEC);
    size_t print(double, int = 2);
    //     size_t print(const Printable &);

    size_t println(const __FlashStringHelper *);
    size_t println(const String &s);
    size_t println(const char[]);
    size_t println(char);
    size_t println(unsigned char, int = DEC);
    size_t println(int, int = DEC);
    size_t println(unsigned int, int = DEC);
    size_t println(long, int = DEC);
    size_t println(unsigned long, int = DEC);
    size_t println(double, int = 2);
    //     size_t println(const Printable &);
    size_t println(void);
    // #ifdef SUPPORT_LONGLONG
    //     void println(int64_t, uint8_t = DEC) {}
    //     void print(int64_t, uint8_t = DEC) {}
    //     void println(uint64_t, uint8_t = DEC) {}
    //     void print(uint64_t, uint8_t = DEC) {}
    // #endif
    //
    size_t printf(const char *format, ...);
    //     int printf(const __FlashStringHelper *format, ...);
private:
    bool mIsInitialized = false;
};

#endif /* klangstrom_arduino_proxy_serial_h */
