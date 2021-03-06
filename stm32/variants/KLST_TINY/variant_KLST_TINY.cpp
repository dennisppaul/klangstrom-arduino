/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "pins_arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

// Digital PinName array
const PinName digitalPin[] = {
    PA_0,   // D0/A0
    PA_1,   // D1/A1
    PA_2,   // D2/A2
    PA_3,   // D3/A3
    PA_4,   // D4/A4
    PA_5,   // D5/A5
    PA_6,   // D6/A6
    PA_7,   // D7/A7
    PA_8,   // D8
    PA_9,   // D9
    PA_10,  // D10
    PA_11,  // D11
    PA_12,  // D12
    PA_13,  // D13
    PA_14,  // D14
    PA_15,  // D15
    PB_0,   // D16/A8
    PB_1,   // D17/A9
    PB_2,   // D18
    PB_3,   // D19
    PB_4,   // D20
    PB_5,   // D21
    PB_6,   // D22
    PB_7,   // D23
    PB_8,   // D24
    PB_9,   // D25
    PB_10,  // D26
    PB_12,  // D27
    PB_13,  // D28
    PB_14,  // D29
    PB_15,  // D30
    PC_0,   // D31/A10
    PC_1,   // D32/A11
    PC_2,   // D33/A12
    PC_3,   // D34/A13
    PC_4,   // D35/A14
    PC_5,   // D36/A15
    PC_6,   // D37
    PC_7,   // D38
    PC_8,   // D39
    PC_9,   // D40
    PC_10,  // D41
    PC_11,  // D42
    PC_12,  // D43
    PC_13,  // D44
    PC_14,  // D45
    PC_15,  // D46
    PD_2,   // D47
    PH_0,   // D48
    PH_1    // D49
};

// Analog (Ax) pin number array
const uint32_t analogInputPin[] = {
    0,   // A0,  PA0
    1,   // A1,  PA1
    2,   // A2,  PA2
    3,   // A3,  PA3
    4,   // A4,  PA4
    5,   // A5,  PA5
    6,   // A6,  PA6
    7,   // A7,  PA7
    16,  // A8,  PB0
    17,  // A9,  PB1
    31,  // A10, PC0
    32,  // A11, PC1
    33,  // A12, PC2
    34,  // A13, PC3
    35,  // A14, PC4
    36   // A15, PC5
};

#ifdef __cplusplus
}
#endif
