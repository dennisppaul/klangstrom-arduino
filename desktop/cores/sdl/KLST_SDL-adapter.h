#ifndef _VARIANT_KLST_SDL_
#define _VARIANT_KLST_SDL_

/* --------------------------------------------------------------- */
/* KLST                                                            */
/* --------------------------------------------------------------- */

// @todo(clean this up + merge with `klangstrom_arduino_proxy*.*` )

/* PIN LABELS */

#define GPIO_00                   0x00
#define GPIO_01                   0x01
#define GPIO_02                   0x02
#define GPIO_03                   0x03
#define GPIO_04                   0x04
#define GPIO_05                   0x05
#define GPIO_06                   0x06
#define GPIO_07                   0x07
#define GPIO_08                   0x08
#define GPIO_09                   0x09

#define ADC_00                    0x0A
#define ADC_01                    0x0B
#define DAC_00                    0x0C
#define DAC_01                    0x0D

#define LED_00                    0x0E
#define LED_01                    0x0F
#define LED_02                    0x10

#define ENCODER_00_BUTTON         0x11
#define ENCODER_01_BUTTON         0x12
#define ENCODER_02_BUTTON         0x13
#define BUTTON_PROGRAMMER         0x14

#define NUMBER_OF_GPIO_PINS       0x15
#define NUMBER_OF_LEDS            0x03

/* --- */

#define LED_BUILTIN               LED_00
#define KLST_UART_BAUD            115200

#define SERIAL_00                 0x15
#define SERIAL_01                 0x16
#define KLST_LOG                  0x17

#define INPUT                     0x18
#define OUTPUT                    0x19
#define INPUT_PULLUP              0x1A

/* functions */

void delay(uint32_t pMS);
int digitalRead(uint32_t pPin);
void digitalWrite(uint32_t pPin, uint32_t pValue);
void pinMode(uint32_t pPin, uint32_t pMode);
bool* getLEDs();

#endif /* _VARIANT_KLST_SDL_ */
