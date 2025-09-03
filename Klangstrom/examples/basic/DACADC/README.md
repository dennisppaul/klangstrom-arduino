# DAC and ADC

this example demonstrates how to use the analog-digital-converter (ADC) and the digital-analog-converter (DAC).

the DAC is a 12-bit DAC with a resolution of 4096 steps. the DAC can be used to generate a voltage between 0V and 3.3V.

note that on KLST_PANDA GPIO 04 and GPIO 05 pins can also be used as ADCs.

## NERD SECTION

the DAC is configured to be used in manually. the ADC however runs continously in direct-memory-access (DMA) mode and is 
therefore the ADC clock is relevant and set to 16 MHz and configured with a prescaler of 256 i.e it samples at a frequency 
of 62.5 kHz or 16 µsec per sample. by default, it samples in blocks of 128 samples i.e the buffer is updated every 2048 µsec
or approx 488 Hz. the buffer size can be changed by setting the macro `KLST_ADC_BUFFER_SIZE` to the desired size.
