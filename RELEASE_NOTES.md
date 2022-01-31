# Klangstrom for Arduino / Release Notes

## 0.0.2

- added `NodeKernelBlockMulti` with ( theoretically ) up to 256 input and output channels. ( `NodeKernel*` classes assist in the development of custom nodes )
- added option to select target board in emulator
- LEDs can now be turned on, off or toggled from one function (`LED(ID,STATE)` where state can be `LED_ON`, `LED_OFF` or `LED_TOGGLE` )
- added `TaskScheduler` to easily created time-based callbacks
- added encoder support to emulator (SDL)
- added WAV file parser ( e.g for SD Card )
- added SD Card support to emulator (SDL)
- added native SD Card support for KLST_SHEEP
- added display support to emulator (SDL)
- added native display support for KLST_SHEEP ( incl. drawing primitives, fonts and bitmaps )
- added KLST_SHEEP board
- added option to select sampling rate and audio block size in Arduino IDE
- added `NodeFFT` to perform *Fast Fourier Transfom* on audio signals
- added *Strom* to repository ( still WIP )
- added *USB Host* support for Mouse + Keyboard + MIDI ( i.e boards can now host USB devices like e.g MIDI keyboards )
- added *USB Device* support for Mouse + Keyboard + MIDI ( via TinyUSB ) ( i.e boards can now act as USB devices )
- unique board ID is now represented as single string ( `klangstrom::U_ID()` )
- added KLST_CORE board
- added USB-MIDI device support ( via TinyUSB library )
- `NodeDelay` maximum delay ( line ) length can now be globally configured ( `#define NODE_DELAY_BUFFER_SIZE BUFFER_SIZE_IN_SAMPLES` )
- added *Klang* source code to documentation
- added support and example for SD Cards

## 0.0.1 (20210614)

- `NodeADSR`+ `NodeRamp` + `NodeEnvelope` can now be triggered by an input channel signal
- `NodeADSR`+ `NodeRamp` + `NodeEnvelope` now also accept microsecond values ( and use them internally as well )
- `NodeSampler` now works with different data types ( `float`, `uint8_t` and `uint16_t` )
- speed of serial port ( `SERIAL_00` + `SERIAL_01` ) is now an option
- `NodeTextToSpeechSAM` now has a customizable buffer size
- `NodeVCOWavetable` can now share wavetable data
- added `NodeRamp` for linear value interpolation
- improved + fixed mixer nodes ( there are now static mixers for 4–32 channels and one node with multiple channels plus 2 stereo versions )
- added mechanism to measure performance of code blocks ( in clock cycles or μs )
- fixed initial encoder button states
- set MCU speed to 180MHz ( from 90MHz )
- added tool to generate sample files
- improved `NodeWavetable` speed by 4.5×
- boards have now unique identifiers `ID()` ( based on their hardware serial number )
- added application to examples ( drum machine + sequencer )
- added documentation ( e.g node library )
- fixed DFU mode ( USB peripherals had to be deinitialized before reset )
- added desktop emulator (SDL)
- added extensions ( i.e reverb, vocoder + TTS )
- cleaned up and improved examples

## 0.0.0 (20210501)

- initial release
