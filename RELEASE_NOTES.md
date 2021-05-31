# Klangstrom for Arduino / Release Notes

## 0.0.1

- added mechanism to measure performance of code blocks ( in clock cycles or μs )
- cleaned up and improved examples
- fixed intial encoder button states
- set MCU speed to 180MHz ( from 90MHz )
- added tool to generate sample files 
- improved `NodeWavetable` speed by 4.5× 
- boards have now unique identifiers `ID()` ( based on their hardware serial number )
- added application to examples ( drum machine + sequencer )
- added documentation ( e.g node library )
- imporved + fixed mixer nodes ( `NodeMixer4Stereo` is still not perfect )
- fixed DFU mode ( USB peripherals had to be deinitialized before reset )
- added desktop simulator (SDL)
- added extensions ( i.e reverb, vocoder + TTS )

## 0.0.0

- initial release
