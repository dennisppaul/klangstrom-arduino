#include <stdbool.h>
#include <stdint.h>
#include "AudioCodecWM8731.h"

extern I2C_HandleTypeDef hi2c2;
#define I2C_TX_handle   &hi2c2
#define I2C_RX_handle   &hi2c2

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

bool I2C_TX_write(uint8_t device_address, uint8_t *data, uint8_t length) {
 HAL_StatusTypeDef mResult = HAL_I2C_Master_Transmit(I2C_TX_handle,
                             device_address, data, length, 10);
 if (mResult != HAL_OK) {
   Error_Handler();
   return false;
 }
 return true;
}

bool I2C_RX_write(uint8_t device_address, uint8_t *data, uint8_t length) {
 HAL_StatusTypeDef mResult = HAL_I2C_Master_Transmit(I2C_RX_handle,
                             device_address, data, length, 10);
 if (mResult != HAL_OK) {
   Error_Handler();
   return false;
 }
 return true;
}

void WM8731_delay(uint32_t pDelay) {
	HAL_Delay(pDelay);
}

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
  uint8_t data[TRANSMIT_LENGTH];

  data[0] = (reg << 1) | ((val >> 8) & 1);
  data[1] = val & 0xFF;

  bool mResult = I2C_TX_write(WM8731_I2C_ADDR, data, TRANSMIT_LENGTH);
  return mResult;
}

bool WM8731_volumeInteger(unsigned int n) {
  // n = 127 for max volume (+6 dB)
  // n = 48 for min volume (-73 dB)
  // n = 0 to 47 for mute
  if (n > 127)
    n = 127;
  WM8731_write(WM8731_REG_LHEADOUT, n | 0x180);
  WM8731_write(WM8731_REG_RHEADOUT, n | 0x80);
  return true;
}

bool WM8731_inputLevel(float n) {
  // range is 0x00 (min) - 0x1F (max)

  int _level = (int) (n * 31.f);

  _level = _level > 0x1F ? 0x1F : _level;
  WM8731_write(WM8731_REG_LLINEIN, _level);
  WM8731_write(WM8731_REG_RLINEIN, _level);
  return true;
}

bool WM8731_inputSelect(int n) {
  if (n == AUDIO_INPUT_LINEIN) {
    WM8731_write(WM8731_REG_ANALOG, 0x12);
  } else if (n == AUDIO_INPUT_MIC) {
    WM8731_write(WM8731_REG_ANALOG, 0x15);
  } else {
    return false;
  }
  return true;
}

bool WM8731_volume(float n) {
  return WM8731_volumeInteger(n * 80.0 + 47.499);
}

bool WM8731_init() {
  /* I2S configuration */
  WM8731_delay(5);
  WM8731_write(WM8731_REG_RESET, 0);

  WM8731_write(WM8731_REG_INTERFACE, 0b00000010); // 0x02=0b00000010 // I2S, 16 bit, MCLK slave
  WM8731_write(WM8731_REG_SAMPLING,  0b00100000); // 0x20=0b00100000  // 256*Fs, 44.1 kHz, MCLK/1

  WM8731_write(WM8731_REG_DIGITAL, 0x08);   // DAC soft mute
  WM8731_write(WM8731_REG_ANALOG, 0x00);    // disable all

  WM8731_write(WM8731_REG_POWERDOWN, 0x00);

  WM8731_write(WM8731_REG_LHEADOUT, 0x80);
  WM8731_write(WM8731_REG_RHEADOUT, 0x80);

  WM8731_delay(5);
  WM8731_write(WM8731_REG_ACTIVE, 1);
  WM8731_delay(5);

  WM8731_write(WM8731_REG_DIGITAL, 0b00100);      // DAC unmuted
  WM8731_write(WM8731_REG_ANALOG, 0b00010000);    // DAC selected

  WM8731_volume(0.5);
  return true;
}

