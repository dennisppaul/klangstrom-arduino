---
layout: libdoc
title: Klangstrom ( Application )
permalink: /klangstrom-application/
index: 2
---

@todo(this page needs improvement!)

*Klangstrom* supplies an application structure which to facilitate the development of applications. 

it extends the arduino *idiom* ( i.e `setup()` + `loop()` ) with sound, music + event related functions ( e.g `audioblock()` + `beat()` ). *Klangstrom* also implements an simple abstraction layer to unify access to peripherals ( e.g `data_receive()` + `data_transmit()` ).

## functions ( implemented in application )

- events 
- can but do no have to be implemented in application
- detailed description of each function
- `setup` :: some thing happen right before and after this is called
- `loop` :: also processes *serial* events ( at least in MCU )

```c
void setup();
void loop();
void beat(uint32_t pBeat);
void event_receive(const EVENT_TYPE event, const float* data);
void data_receive(const uint8_t sender, uint8_t* data, uint8_t length);
void audioblock(SIGNAL_TYPE* pLeftTX, SIGNAL_TYPE* pRightTX, SIGNAL_TYPE* pLeftRX, SIGNAL_TYPE* pRightRX);
```

## functions ( interfacing with low-level )

- are used to communicated with peripherals ( UART(serial), SPI, I2C, SD Card, LEDs, GPIOs )

```c
void option(uint8_t pOption, uint8_t pValue);
void beats_per_minute(float pBPM);
void beats_per_minute_ms(uint32_t pMicroSeconds);
void led(uint8_t pLED, bool pState);
void led_toggle(uint8_t pLED);
bool button_state(uint8_t pButton);
bool pin_state(uint8_t pButton);
void event_transmit(EVENT_TYPE pEvent, float* pPayload);
void data_transmit(const uint8_t pSender, uint8_t* pData, uint8_t pDataLength);
```

- `KLST_LOG` 
