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

#if (KLST_ARCH==KLST_ARCH_MCU) // @TODO( probably better test against specific board KLST_PANDA might has a different codec )

#include "AudioCodecWM8731.h"

// #ifdef __cplusplus
// extern "C" {
// #endif

#define WM8731_I2C_ADDR 0x34

// moved to `KLST_*-BSP.cpp`
// bool WM8731_init() { 
//     /* I2S configuration */
//     WM8731_delay(5);
//     WM8731_write(WM8731_REG_RESET, 0);

//     WM8731_write(WM8731_REG_INTERFACE, 0b00000010);  // 0x02=0b00000010 // I2S, 16 bit, MCLK slave
//     WM8731_write(WM8731_REG_SAMPLING, 0b00100000);   // 0x20=0b00100000  // 256*Fs, 44.1 kHz, MCLK/1

//     WM8731_write(WM8731_REG_DIGITAL, 0x08);  // DAC soft mute
//     WM8731_write(WM8731_REG_ANALOG, 0x00);   // disable all

//     WM8731_write(WM8731_REG_POWERDOWN, 0x00);

//     WM8731_write(WM8731_REG_LHEADOUT, 0x80);
//     WM8731_write(WM8731_REG_RHEADOUT, 0x80);

//     WM8731_delay(5);
//     WM8731_write(WM8731_REG_ACTIVE, 1);
//     WM8731_delay(5);

//     WM8731_write(WM8731_REG_DIGITAL, 0b00100);    // DAC unmuted
//     WM8731_write(WM8731_REG_ANALOG, 0b00010000);  // DAC selected

//     WM8731_volume(0.5);
//     return true;
// }

bool WM8731_write(uint8_t reg, uint16_t val) {
    /*
     sub procedure WM8731_CMD(dim address as byte, dim cmd as word)
     dim addr as byte
     ' B[15:9] Are Control Address Bits
     ' B[8:0]  Are Control Data Bits
     addr = address << 1                 ' Shift left for one positions
     addr = addr or (hi(cmd) and 1)

     I2C2_Start()                           ' Start I2C2 module
     I2C2_wm8731_write(WM8731_ADDRESS)             ' Write Adress of WM8731 chip
     I2C2_wm8731_write(addr)                       ' Write register address
     I2C2_wm8731_write(lo(cmd))                    ' Write command
     I2C2_Stop()                            ' Stop I2C2 module
     end sub
     */

    const static uint8_t TRANSMIT_LENGTH = 2;
    uint8_t              data[TRANSMIT_LENGTH];

    data[0] = (reg << 1) | ((val >> 8) & 1);
    data[1] = val & 0xFF;

    bool mResult = WM8731_I2C_write(WM8731_I2C_ADDR, data, TRANSMIT_LENGTH);
    return mResult;
}

bool WM8731_headphone_output_volume_integer(unsigned int n) {
    // n = 127 for max volume (+6 dB)
    // n = 48 for min volume (-73 dB)
    // n = 0 to 47 for mute
    if (n > 127)
        n = 127;
    WM8731_write(WM8731_HEADPHONE_OUT_LEFT, n | 0x80);
    WM8731_write(WM8731_HEADPHONE_OUT_RIGHT, n | 0x80);
    return true;
}

bool WM8731_input_level(float n) {
    // range is 0x00 (min) - 0x1F (max)

    int _level = (int)(n * 31.f);

    _level = _level > 0x1F ? 0x1F : _level;
    WM8731_write(WM8731_LINE_IN_LEFT, _level);
    WM8731_write(WM8731_LINE_IN_RIGHT, _level);
    return true;
}

bool WM8731_input_select(int n) {
    if (n == AUDIO_INPUT_LINEIN) {
        WM8731_write(WM8731_ANALOG_AUDIO_PATH_CONTROL, 0x12); // 0b000010010
    } else if (n == AUDIO_INPUT_MIC) {
        WM8731_write(WM8731_ANALOG_AUDIO_PATH_CONTROL, 0x15); // 0b000010101
    } else {
        return false;
    }
    return true;
}

bool WM8731_headphone_output_volume(float n) {
    return WM8731_headphone_output_volume_integer(n * 80.0 + 47.499);
}

// #ifdef __cplusplus
// }
// #endif

#endif // (KLST_ARCH==KLST_ARCH_MCU)

