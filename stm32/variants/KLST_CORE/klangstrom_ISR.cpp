#if !I2S_USE_DMA

#include "Arduino.h"
#include "HardwareTimer.h"
#include "klangstrom_ISR.h"
#include "klangstrom_peripherals.h"

void KLST_startISR() {
    TIM_TypeDef *Instance2 = TIM2;
    HardwareTimer *MyTim2 = new HardwareTimer(Instance2);
    MyTim2->setOverflow(46876, HERTZ_FORMAT); // 44100 Hz
    MyTim2->attachInterrupt(KLST_audio_interrupt);
    MyTim2->resume();
}
#endif
