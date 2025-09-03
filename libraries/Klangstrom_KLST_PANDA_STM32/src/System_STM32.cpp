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
#ifdef KLST_ARCH_IS_STM32

#include "ArrayList.h"
#include "System.h"
#include "stm32h7xx_hal.h"
#include "AudioDevice_STM32.h"
#include "SerialDevice_STM32.h"
#include "Timer_STM32.h"
#include "Console.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(KLST_PERIPHERAL_ENABLE_SERIAL_DEBUG) || \
    defined(KLST_PERIPHERAL_ENABLE_IDC_SERIAL) ||   \
    defined(KLST_PERIPHERAL_ENABLE_MIDI)
#define KLST_PERIPHERAL_ENABLE_UART
#endif

uint32_t system_get_ticks_BSP() {
    return HAL_GetTick();
}

static bool cycle_counter_enabled = false;

#ifdef KLST_USE_ARM_REGISTERS_FOR_CYCLE_COUNTER
#define ARM_CM_DEMCR (*(uint32_t*) 0xE000EDFC)
#define ARM_CM_DWT_CTRL (*(uint32_t*) 0xE0001000)
#define ARM_CM_DWT_CYCCNT (*(uint32_t*) 0xE0001004)

void system_enable_cycle_counter(bool enable) {
    if (enable) {
        if (ARM_CM_DWT_CTRL != 0) {  // See if DWT is available
            ARM_CM_DEMCR |= 1 << 24; // Set bit 24
            ARM_CM_DWT_CYCCNT = 0;
            ARM_CM_DWT_CTRL |= 1 << 0; // Set bit 0
            cycle_counter_enabled = true;
        }
    } else {
        if (ARM_CM_DWT_CTRL != 0) {       // See if DWT is available
            ARM_CM_DWT_CTRL &= ~(1 << 0); // Clear bit 0 to disable the cycle counter
            cycle_counter_enabled = false;
        }
    }
}

void system_reset_cycles() {
    ARM_CM_DWT_CYCCNT = 0;
}

uint32_t system_get_cycles_BSP() {
    if (!cycle_counter_enabled) {
        system_enable_cycle_counter(true);
    }
    return ARM_CM_DWT_CYCCNT;
}

#else  // KLST_USE_ARM_REGISTERS_FOR_CYCLE_COUNTER
void system_enable_cycle_counter(bool enable) {
    if (enable) {
        if (DWT->CTRL != 0) {                               // See if DWT is available
            CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // Set bit 24
            DWT->CYCCNT = 0;
            DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; // Set bit 0
            cycle_counter_enabled = true;
        }
    } else {
        if (DWT->CTRL != 0) {                     // See if DWT is available
            DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; // Clear bit 0 to disable the cycle counter
            cycle_counter_enabled = false;
        }
    }
}

void system_reset_cycles() {
    DWT->CYCCNT = 0;
}

uint32_t system_get_cycles() {
    if (!cycle_counter_enabled) {
        system_enable_cycle_counter(true);
    }
    return DWT->CYCCNT;
}
#endif // KLST_USE_ARM_REGISTERS_FOR_CYCLE_COUNTER

uint32_t system_clock_frequency() {
    return HAL_RCC_GetSysClockFreq();
}

/* ----------------- SAI ---------------- */

static void SAI_TX_event(SAI_HandleTypeDef* hsai, uint8_t callback_event) {
    ArrayList_AudioDevicePtr* fAudioDeviceListeners = system_get_registered_audiodevices();
    for (size_t i = 0; i < fAudioDeviceListeners->size; i++) {
        AudioDevice* ad = arraylist_AudioDevicePtr_get(fAudioDeviceListeners, i);
        if (ad != nullptr) {
#ifdef KLST_PERIPHERAL_ENABLE_AUDIODEVICE
            if (ad->peripherals->audiodevice_sai_tx->Instance == hsai->Instance) {
                if (ad->peripherals->callback_tx != nullptr) {
                    ad->peripherals->callback_tx(ad, callback_event);
                }
            }
#endif // KLST_PERIPHERAL_ENABLE_AUDIODEVICE
        }
    }
}

void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef* hsai) {
    SAI_TX_event(hsai, CALLBACK_FULL_COMPLETE);
}

void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef* hsai) {
    SAI_TX_event(hsai, CALLBACK_HALF_COMPLETE);
}

static void SAI_RX_event(SAI_HandleTypeDef* hsai, uint8_t callback_event) {
    ArrayList_AudioDevicePtr* fAudioDeviceListeners = system_get_registered_audiodevices();
    for (size_t i = 0; i < fAudioDeviceListeners->size; i++) {
        AudioDevice* ad = arraylist_AudioDevicePtr_get(fAudioDeviceListeners, i);
        if (ad != nullptr) {
#ifdef KLST_PERIPHERAL_ENABLE_AUDIODEVICE
            if (ad->peripherals->audiodevice_sai_rx->Instance == hsai->Instance) {
                if (ad->peripherals->callback_rx != nullptr) {
                    ad->peripherals->callback_rx(ad, callback_event);
                }
            }
#endif // KLST_PERIPHERAL_ENABLE_AUDIODEVICE
        }
    }
}

void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef* hsai) {
    SAI_RX_event(hsai, CALLBACK_FULL_COMPLETE);
}

void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef* hsai) {
    SAI_RX_event(hsai, CALLBACK_HALF_COMPLETE);
}

void HAL_SAI_ErrorCallback(SAI_HandleTypeDef* hsai) {
    ArrayList_AudioDevicePtr* fAudioDeviceListeners = system_get_registered_audiodevices();
    for (size_t i = 0; i < fAudioDeviceListeners->size; i++) {
        AudioDevice* ad = arraylist_AudioDevicePtr_get(fAudioDeviceListeners, i);
        if (ad != nullptr) {
#ifdef KLST_PERIPHERAL_ENABLE_AUDIODEVICE
            if (ad->peripherals->audiodevice_sai_tx->Instance == hsai->Instance) {
                if (ad->peripherals->callback_error != nullptr) {
                    ad->peripherals->callback_error(ad, CALLBACK_TX_ERROR);
                }
            }
            if (ad->peripherals->audiodevice_sai_rx->Instance == hsai->Instance) {
                if (ad->peripherals->callback_error != nullptr) {
                    ad->peripherals->callback_error(ad, CALLBACK_RX_ERROR);
                }
            }
#endif // KLST_PERIPHERAL_ENABLE_AUDIODEVICE
        }
    }
}

/* ----------------- GPIO --------------- */

void HAL_GPIO_EXTI_Callback(const uint16_t GPIO_Pin) {
    ArrayList_GPIOListenerPtr* fGPIOListeners = system_get_registered_gpio_listener();
    for (size_t i = 0; i < fGPIOListeners->size; i++) {
        const GPIOListener* gpio = arraylist_GPIOListenerPtr_get(fGPIOListeners, i);
        if (gpio != nullptr) {
            gpio->callback(GPIO_Pin);
        }
    }
    // NOTE: currenlty the following EXTI ( external interrupts ) IDs are used:
    // - EXTI13 for touch display
    // - EXTI14 for mechanical key 00
    // - EXTI15 for mechanical key 01
    // maybe use
    // - EXTI12 for programmer button
}

/* --------------- U(S)ART -------------- */

#ifdef KLST_PERIPHERAL_ENABLE_UART
static void start_receive(SerialDevice* serialdevice) {
    HAL_UARTEx_ReceiveToIdle_DMA(serialdevice->peripherals->uart_handle,
                                 serialdevice->peripherals->buffer_rx,
                                 serialdevice->data_buffer_size);
    //    __HAL_DMA_DISABLE_IT(serialdevice->peripherals->dma_handle_rx, DMA_IT_HT);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size) {
    //    console_status("HAL_UARTEx_RxEventCallback");
    ArrayList_SerialDevicePtr* mSerialDevices = system_get_registered_serialdevices();
    for (size_t i = 0; i < mSerialDevices->size; i++) {
        SerialDevice* sd = arraylist_SerialDevicePtr_get(mSerialDevices, i);
        if (sd != nullptr) {
            if (sd->peripherals->uart_handle->Instance == huart->Instance) {
                if (sd->callback_serial != nullptr) {
                    sd->callback_serial(sd);
                    if (Size > sd->data_buffer_size) {
                        Size = sd->data_buffer_size;
                        console_error("serial RX buffer overflow");
                    }
                    /* copy data to buffer */
                    for (int j = 0; j < Size; ++j) {
                        sd->data[j] = sd->peripherals->buffer_rx[j];
                    }
                    sd->length = Size;
                    start_receive(sd);
                }
            }
        }
    }
    // void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef* huart) {
    console_error("HAL_UART_ErrorCallback");
    ArrayList_SerialDevicePtr* mSerialDevices = system_get_registered_serialdevices();
    for (size_t i = 0; i < mSerialDevices->size; i++) {
        SerialDevice* sd = arraylist_SerialDevicePtr_get(mSerialDevices, i);
        if (sd != nullptr) {
            if (sd->peripherals->uart_handle->Instance == huart->Instance) {
                console_error("@ SerialDevices ID: %i", sd->device_id);
                start_receive(sd);
            }
        }
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart) {
    ArrayList_SerialDevicePtr* mSerialDevices = system_get_registered_serialdevices();
    for (size_t i = 0; i < mSerialDevices->size; i++) {
        SerialDevice* sd = arraylist_SerialDevicePtr_get(mSerialDevices, i);
        if (sd != nullptr) {
            if (sd->peripherals->uart_handle->Instance == huart->Instance) {
                sd->peripherals->is_transmitting = false;
                start_receive(sd);
            }
        }
    }
}
#endif // KLST_PERIPHERAL_ENABLE_UART

/* ---------------- TIMER --------------- */

#ifdef KLST_PERIPHERAL_ENABLE_TIMERS
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
    ArrayList_TimerPtr* mTimers = system_get_registered_timer();
    for (size_t i = 0; i < mTimers->size; i++) {
        Timer* t = arraylist_TimerPtr_get(mTimers, i);
        if (t != nullptr) {
            if (t->peripherals->timer_handle->Instance == htim->Instance) {
                t->callback(t);
            }
        }
    }
}
#endif // KLST_PERIPHERAL_ENABLE_TIMERS

#ifdef SYSTEM_HANDLE_TIMEOFDAY_WARNING
#include <sys/time.h>
[[maybe_unused]] int _gettimeofday(struct timeval* tv, void* tzvp) {
    (void) tv;
    (void) tzvp;
    return 0;
}
#endif // SYSTEM_HANDLE_TIMEOFDAY_WARNING

#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_STM32
