/*
* Klangstrom
*
* This file is part of the *Klangstrom* library (https://github.com/dennisppaul/klangstrom-libraries).
* Copyright (c) 2024 Dennis P Paul.
*
* This library is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3.
*
* This library is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "KlangstromEnvironment.h"
#if defined(KLST_PANDA_STM32) || defined(KLST_CATERPILLAR_STM32)

#include <stdint.h>

#include "main.h"
#include "SystemBoot.h"

#ifdef __cplusplus
extern "C" {
#endif

uint32_t system_boot_address() {
    //    AN2606 Rev 51 STM32 microcontroller system memory boot mode, p261
    return 0x1FF09800; // boot address for STM32H74xxx/75xxx
}

static void KLST_BSP_shutdown() {
    //    /* stop encoders */
    //    KLST_BSP_deinit_encoders();
    //    /* stop audio */
    //    HAL_SAI_DeInit(&hsai_BlockB1);
    //    HAL_SAI_DeInit(&hsai_BlockA1);
    //    HAL_SAI_DMAStop(&hsai_BlockB1);
    //    HAL_SAI_DMAStop(&hsai_BlockA1);
    //    HAL_SAI_MspDeInit(&hsai_BlockB1);
    //    HAL_SAI_MspDeInit(&hsai_BlockA1);
    //    /* stop I2C */
    //    HAL_I2C_DeInit(&hi2c1);
    //    HAL_I2C_MspDeInit(&hi2c1);
    //    /* stop SPI */
    //    HAL_SPI_DeInit(&hspi3);
    //    HAL_SPI_MspDeInit(&hspi3);
    //    /* stop USB */
    //    __HAL_RCC_USB_OTG_FS_CLK_DISABLE();
    //    __HAL_RCC_USB_OTG_HS_CLK_DISABLE();
}

static void KLST_ISH_shutdown() {
    KLST_BSP_shutdown();
    //    //@todo(check if other peripherals also need to be reset e.g GPIOs, SPI, I2C. see https://github.com/viktorvano/STM32-Bootloader/blob/master/README.md )
    //    /* stop beat */
    //    mKLSTBeatTimer->pause();
    ///* stop USB */
    //#if defined(USBCON) && defined(USBD_USE_CDC)
    //    SerialUSB.end();
    //#endif
    //    /* flash LEDs */
    //    digitalWrite(LED_00, LOW);
    //    digitalWrite(LED_01, HIGH);
    //    digitalWrite(LED_02, LOW);
    //    for (uint8_t i = 16; i > 0; i--) {
    //        KLST_shutdown_toggle_leds(24 + 12 * i);
    //    }
}

void system_jump_to_bootloader() {
    // - [x] Disable all peripheral clocks
    // - [ ] Disable used PLL
    // - [ ] Disable interrupts
    // - [ ] Clear pending interrupts
    KLST_ISH_shutdown();

    void (*SysMemBootJump)(void);

    /* Set the address of the entry point to bootloader */
    volatile uint32_t BootAddr = system_boot_address();

    /* Disable all interrupts */
    __disable_irq();

    /* Disable Systick timer */
    SysTick->CTRL = 0;

    /* Set the clock to the default state */
    HAL_RCC_DeInit();

    /* Clear Interrupt Enable Register & Interrupt Pending Register */
    for (uint8_t i = 0; i < 5; i++) {
        NVIC->ICER[i] = 0xFFFFFFFF;
        NVIC->ICPR[i] = 0xFFFFFFFF;
    }

    /* Re-enable all interrupts */
    __enable_irq();

    /* Set up the jump to booloader address + 4 */
    SysMemBootJump = (void (*)(void))(*((uint32_t*) ((BootAddr + 4))));

    /* Set the main stack pointer to the bootloader stack */
    __set_MSP(*(uint32_t*) BootAddr);

    /* Call the function to jump to bootloader location */
    SysMemBootJump();
}


#ifdef __cplusplus
}
#endif

#endif // KLST_PANDA_STM32 || KLST_CATERPILLAR_STM32
