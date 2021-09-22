#ifndef _AUDIOCODECWM8731_
#define _AUDIOCODECWM8731_

#include <stdbool.h>
#include <stdint.h>

/* new driver implementation */

#define WM8731_LINE_IN_LEFT                   0b0000000
#define WM8731_LINE_IN_RIGHT                  0b0000001
#define WM8731_HEADPHONE_OUT_LEFT             0b0000010
#define WM8731_HEADPHONE_OUT_RIGHT            0b0000011
#define WM8731_ANALOG_AUDIO_PATH_CONTROL      0b0000100
#define WM8731_DIGITAL_AUDIO_PATH_CONTROL     0b0000101
#define WM8731_POWER_DOWN_CONTROL             0b0000110
#define WM8731_DIGITAL_AUDIO_INTERFACE_FORMAT 0b0000111
#define WM8731_SAMPLING_CONTROL               0b0001000
#define WM8731_ACTIVE_CONTROL                 0b0001001
#define WM8731_RESET_REGISTER                 0b0001111

#define AUDIO_INPUT_LINEIN 0
#define AUDIO_INPUT_MIC    1

#ifdef __cplusplus
extern "C" {
#endif

bool WM8731_write(uint8_t reg, uint16_t val);
bool WM8731_input_level(float n);            // range: 0.0f to 1.0f
bool WM8731_input_select(int n);
bool WM8731_headphone_output_volume(float n);
bool WM8731_init();

/* I2C */

extern bool WM8731_I2C_write(uint8_t device_address, uint8_t* data, uint8_t length);
extern void WM8731_delay(uint32_t pDelay);

#ifdef __cplusplus
}
#endif

#endif  // _AUDIOCODECWM8731_
