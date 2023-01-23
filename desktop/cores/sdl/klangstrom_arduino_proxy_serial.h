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

extern void serialEvent() __attribute__((weak));

class SerialProxy {
#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

#define SERIAL_PROXY_BUFFER_SIZE 256
#define _SS_MAX_RX_BUFF          64  // RX buffer size

public:
    void begin(unsigned long pBaud);

    size_t write(uint8_t c);
    size_t write(const uint8_t *buffer, size_t size);
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

    void flush() {
        _receive_buffer_head = _receive_buffer_tail = 0;
    }

    int peek() {
        // Empty buffer?
        if (_receive_buffer_head == _receive_buffer_tail) {
            return -1;
        }

        // Read from "head"
        return _receive_buffer[_receive_buffer_head];
    }

    int available() {
        return (_receive_buffer_tail + _SS_MAX_RX_BUFF - _receive_buffer_head) % _SS_MAX_RX_BUFF;
    }

    int read() {
        // Empty buffer?
        if (_receive_buffer_head == _receive_buffer_tail) {
            return -1;
        }

        // Read from "head"
        uint8_t d            = _receive_buffer[_receive_buffer_head];  // grab next byte
        _receive_buffer_head = (_receive_buffer_head + 1) % _SS_MAX_RX_BUFF;
        return d;
    }

    //
    // The receive routine called by the interrupt handler
    //
    void recv(uint8_t rx_buffer) {
        // stop bit read complete add to buffer
        uint8_t next = (_receive_buffer_tail + 1) % _SS_MAX_RX_BUFF;
        if (next != _receive_buffer_head) {
            // save new data in buffer: tail points to where byte goes
            _receive_buffer[_receive_buffer_tail] = rx_buffer;  // save new byte
            _receive_buffer_tail                  = next;
        } else {  // rx_bit_cnt = x  with x = [0..7] correspond to new bit x received
            _buffer_overflow = true;
        }
    }

private:
    uint8_t          _receive_buffer[_SS_MAX_RX_BUFF];
    volatile uint8_t _receive_buffer_tail = 0;
    volatile uint8_t _receive_buffer_head = 0;
    uint16_t         _buffer_overflow     = false;
    bool             mIsInitialized       = false;
};

#endif /* klangstrom_arduino_proxy_serial_h */
