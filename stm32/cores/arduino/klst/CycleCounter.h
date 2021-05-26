#ifndef _CycleCounter_h_
#define _CycleCounter_h_

#define  ARM_CM_DEMCR      (*(uint32_t *)0xE000EDFC)
#define  ARM_CM_DWT_CTRL   (*(uint32_t *)0xE0001000)
#define  ARM_CM_DWT_CYCCNT (*(uint32_t *)0xE0001004)

void klst_enable_cycle_counter() {
    if (ARM_CM_DWT_CTRL != 0) {        // See if DWT is available
        ARM_CM_DEMCR      |= 1 << 24;  // Set bit 24
        ARM_CM_DWT_CYCCNT  = 0;
        ARM_CM_DWT_CTRL   |= 1 << 0;   // Set bit 0
    }
}

uint32_t klst_get_cycles() {
    return ARM_CM_DWT_CYCCNT;
}

void klst_reset_cycles() {
    ARM_CM_DWT_CYCCNT  = 0;
}

float klst_cyclesToMicros(uint32_t pCycles) {
  return pCycles / (float)(SystemCoreClock / 1000000);
}

#endif // _CycleCounter_h_
