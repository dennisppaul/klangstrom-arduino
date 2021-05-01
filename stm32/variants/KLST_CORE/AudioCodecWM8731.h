#ifndef _KLANGSTROM_AUDIOCODECWM8731_
#define _KLANGSTROM_AUDIOCODECWM8731_

#include <stdbool.h>
#include <stdint.h>
#include "Arduino.h"

#define WM8731_I2C_ADDR 0x34

#define WM8731_REG_LLINEIN  0
#define WM8731_REG_RLINEIN  1
#define WM8731_REG_LHEADOUT 2
#define WM8731_REG_RHEADOUT 3
#define WM8731_REG_ANALOG   4
#define WM8731_REG_DIGITAL  5
#define WM8731_REG_POWERDOWN    6
#define WM8731_REG_INTERFACE    7
#define WM8731_REG_SAMPLING 8
#define WM8731_REG_ACTIVE   9
#define WM8731_REG_RESET    15

#define AUDIO_INPUT_LINEIN  0
#define AUDIO_INPUT_MIC     1

#ifdef __cplusplus
extern "C" {
#endif

bool WM8731_write(uint8_t reg, uint16_t val);
bool WM8731_volumeInteger(unsigned int n);
bool WM8731_inputLevel(float n);
bool WM8731_inputSelect(int n);
bool WM8731_volume(float n);
bool WM8731_init();

#ifdef __cplusplus
}
#endif

#endif /* _KLANGSTROM_AUDIOCODECWM8731_ */
