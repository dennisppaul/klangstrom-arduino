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

#include "Klangstrom.h"
#if defined(KLST_PERIPHERAL_ENABLE_EXTERNAL_MEMORY)
#ifdef KLST_ARCH_IS_STM32

#include <stdio.h>

#include "main.h"
#include "SystemExternalMemory_KLST_PANDA.h"
#include "Console.h"

#ifdef __cplusplus
extern "C" {
#endif
extern OSPI_HandleTypeDef hospi1;

/* allocate external memory */
//#define EXT_MEM __attribute__((section(".external_memory")))
//#define NUM_SAMPLES (48000 * 20)
//EXT_MEM float m_array[NUM_SAMPLES];
void externalmemory_init() {
    OSPI_HyperbusCmdTypeDef  sCommand;
    OSPI_MemoryMappedTypeDef sMemMappedCfg;

    /* Memory-mapped mode configuration */
    sCommand.AddressSpace = HAL_OSPI_MEMORY_ADDRESS_SPACE;
    sCommand.AddressSize  = HAL_OSPI_ADDRESS_32_BITS;
    sCommand.DQSMode      = HAL_OSPI_DQS_ENABLE;
    sCommand.Address      = 0;
    sCommand.NbData       = 1;

    if (HAL_OSPI_HyperbusCmd(&hospi1, &sCommand,
                             HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        Error_Handler();
    }

    sMemMappedCfg.TimeOutActivation = HAL_OSPI_TIMEOUT_COUNTER_DISABLE;

    if (HAL_OSPI_MemoryMapped(&hospi1, &sMemMappedCfg) != HAL_OK) {
        Error_Handler();
    }
}

void externalmemory_deinit() {}

/* S70KL1281 memory */

#define SDRAM_ADDRESS_START 0x90000000
#define SDRAM_SIZE 0x1000000

void externalmemory_test() {
    uint32_t counter;
    uint32_t start_time;
    uint32_t stop_time;

    start_time = HAL_GetTick();
    for (counter = 0x00; counter < SDRAM_SIZE; counter++) {
        *(__IO uint8_t*) (SDRAM_ADDRESS_START + counter) = (uint8_t) 0x0;
    }
    stop_time = HAL_GetTick() - start_time;
    console_println("             writing 0s   : %li\n\r", stop_time);

    HAL_Delay(50);

    uint8_t testByte = 0x00;
    start_time       = HAL_GetTick();
    for (counter = 0x00; counter < SDRAM_SIZE; counter++) {
        *(__IO uint8_t*) (SDRAM_ADDRESS_START + counter) = (uint8_t) (testByte + counter);
    }
    stop_time = HAL_GetTick() - start_time;
    console_println("             writing byte : %li\n\r", stop_time);

    HAL_Delay(50);

    uint32_t error_counter = 0;
    start_time             = HAL_GetTick();
    for (counter = 0x00; counter < SDRAM_SIZE; counter++) {
        uint8_t testValue = counter;
        uint8_t readValue = *(__IO uint8_t*) (SDRAM_ADDRESS_START + counter);

        if (testValue != readValue) {
            error_counter++;
        }
    }
    stop_time = HAL_GetTick() - start_time;
    console_println("             reading byte : %li\n\r", stop_time);
    console_println("                   errors : %li\n\r", error_counter);

    HAL_Delay(50);

    //	// fill with visual pattern
    //	const uint32_t M_FRAMEBUFFER_SIZE = 480 * 272 * 4;
    //	for (counter = 0x00; counter < M_FRAMEBUFFER_SIZE; counter++) {
    //		*(__IO uint8_t*) (SDRAM_ADDRESS_START + (counter + 0)) = 0;
    //	}
    //	for (counter = 0x00; counter < M_FRAMEBUFFER_SIZE; counter += 4) {
    //		const uint8_t rgb = (uint8_t) (counter / (480 * 4));
    //		*(__IO uint8_t*) (SDRAM_ADDRESS_START + M_FRAMEBUFFER_SIZE + (counter + 0)) = 255;
    //		*(__IO uint8_t*) (SDRAM_ADDRESS_START + M_FRAMEBUFFER_SIZE + (counter + 1)) = rgb;
    //		*(__IO uint8_t*) (SDRAM_ADDRESS_START + M_FRAMEBUFFER_SIZE + (counter + 2)) = rgb;
    //		*(__IO uint8_t*) (SDRAM_ADDRESS_START + M_FRAMEBUFFER_SIZE + (counter + 3)) = 255;
    //	}
}

#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_STM32
#endif //