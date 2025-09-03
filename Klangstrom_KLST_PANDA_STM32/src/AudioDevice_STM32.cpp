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

#include "PeripheralConfiguration_STM32.h" // TODO change this to KLST_STM32 aka 'Architecture Specific' (ASP)
#ifdef KLST_PERIPHERAL_ENABLE_AUDIODEVICE
#include "KlangstromEnvironment.h"
#ifdef KLST_ARCH_IS_STM32

#include "AudioDevice.h"
#include "AudioDevice_STM32.h"

#ifdef __cplusplus
extern "C" {
#endif

void audiodevice_resume(AudioDevice* audiodevice) {
    if (audiodevice->peripherals->audiodevice_sai_tx != nullptr) {
        HAL_SAI_DMAResume(audiodevice->peripherals->audiodevice_sai_tx);
    }
    if (audiodevice->peripherals->audiodevice_sai_rx != nullptr) {
        HAL_SAI_DMAResume(audiodevice->peripherals->audiodevice_sai_rx);
    }
}

void audiodevice_pause(AudioDevice* audiodevice) {
    if (audiodevice->peripherals->audiodevice_sai_tx != nullptr) {
        HAL_SAI_DMAPause(audiodevice->peripherals->audiodevice_sai_tx);
    }
    if (audiodevice->peripherals->audiodevice_sai_rx != nullptr) {
        HAL_SAI_DMAPause(audiodevice->peripherals->audiodevice_sai_rx);
    }
}

#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_STM32
#endif // KLST_PERIPHERAL_ENABLE_AUDIODEVICE