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
#ifdef KLST_ARCH_IS_EMU

#include <string>

#include "Console.h"
#include "SerialDevice.h"
#include "KlangstromEmulator.h"

class DrawableSerialDevice : public Drawable {
public:
    DrawableSerialDevice(SerialDevice* serialdevice) : fSerialDevice(serialdevice) {}

    void draw(PGraphics* g_ptr) override {
        PGraphics& g = *g_ptr;
        g.pushMatrix();
        g.translate(200, 30 + 10 * fSerialDevice->device_id);
        g.fill(0, 255, 0);
        g.textSize(12);
        std::string text = "SerialDevice: " + std::to_string(fSerialDevice->device_id);
        g.text(text, 0, 0);
        g.popMatrix();
    }

private:
    SerialDevice* fSerialDevice;
};

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SerialPeripherals {
    std::string IP;
    uint32_t    port;
} SerialPeripherals;

void serialdevice_send(const SerialDevice* serialdevice, const uint8_t* data, const uint16_t length) {
    // TODO could interprete IP + port
    OscMessage msg(KLST_EMU_SERIAL_ADDRESS_PATTERN);
    msg.add(serialdevice->device_type);
    msg.add(serialdevice->device_id);
    msg.add(std::string{data, data + length}); // NOTE does not work with C-style string
    msg.add(length);
    umfeld::KlangstromEmulator::instance()->osc_send(msg);
}

bool serialdevice_init_BSP(SerialDevice* serialdevice) {
    if (serialdevice->peripherals == nullptr) { println("ERROR: peripherals not initialized"); }

    println("serialdevice_init_BSP");
    println("buffer_size: ", serialdevice->data_buffer_size);
    println("device_type: ", serialdevice->device_type);
    println("device_id  : ", serialdevice->device_id);
    println("baud_rate  : ", serialdevice->baud_rate);
    println("port       : ", serialdevice->peripherals->port);
    println("IP         : ", serialdevice->peripherals->IP);

    serialdevice->data = new uint8_t[serialdevice->data_buffer_size];

    bool mDeviceInitialized = false;

    if (serialdevice->device_type < SERIAL_DEVICE_MAX_NUMBER_OF_DEVICE_TYPES) {
        println("device type: ", serialdevice->device_type);
        if (serialdevice->device_type == SERIAL_DEVICE_TYPE_IDC_00 ||
            serialdevice->device_type == SERIAL_DEVICE_TYPE_IDC_01) {
            console_status("device type: is IDC_00");
            // TODO maybe check configuratiom ( e.g baud rate )
            // TODO and see if the board ( e.g KLST_PANDA or KLST_CATERPILLAR ) is compatible
            // TODO in hardware implementation this would also set handlers and initialize hardware ( e.g `UART_HandleTypeDef huart8` + `DMA_HandleTypeDef  hdma_uart8_rx` + `DMA_HandleTypeDef  hdma_uart8_tx` )
            //            typedef struct SerialPeripherals {
            //                UART_HandleTypeDef* uart_handle;
            //                DMA_HandleTypeDef*  dma_handle;
            //                uint8_t*            rx_buffer; // from pool with `__attribute__((section(".dma_buffer")))`
            //                uint8_t*            tx_buffer; // from pool with `__attribute__((section(".dma_buffer")))`
            //            } SerialPeripherals;
            mDeviceInitialized = true;
        } else if (serialdevice->device_type == SERIAL_DEVICE_TYPE_MIDI) {
            console_status("device type: is MIDI");
            mDeviceInitialized = true;
        }
    }

    console_status("device type: custom(%i)", serialdevice->device_type);
    // TODO in hardware implementation this would require the client to intialize hardware manually
    // NOTE client needs to set peripherals manually

    umfeld::KlangstromEmulator::instance()->register_serial_device(serialdevice);
    umfeld::KlangstromEmulator::instance()->register_drawable(new DrawableSerialDevice(serialdevice));
    return mDeviceInitialized;
}

void serialdevice_deinit_BSP(SerialDevice* serialdevice) {
    delete[] serialdevice->data;
    // TODO unregister serial device
    // TODO unregister drawable
}

bool serialdevice_init_peripherals_BSP(SerialDevice* serialdevice) {
    SerialPeripherals* peripherals = new SerialPeripherals();
    peripherals->port              = 7000;
    serialdevice->peripherals      = peripherals;
    return true;
}

void serialdevice_deinit_peripherals_BSP(SerialDevice* serialdevice) {
    delete serialdevice->peripherals;
    serialdevice->peripherals = nullptr;
}

#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_EMU