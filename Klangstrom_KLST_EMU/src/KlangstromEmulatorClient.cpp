/*
* Klangstrom
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
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

#include "KlangstromEmulatorClient.h"
#include "KlangstromEmulator.h"
#include "PApplet.h"

#include "AudioDevice.h"
#include "AudioDevice_EMU.h"
#include "SerialDevice.h"
#include "System.h"
#include "Console.h"

void KlangstromEmulatorClient::process_device(KlangstromEmulatorAudioDevice* device) {
    // TODO check if we should use double buffering here or if we just use normal buffer i.e `CALLBACK_FULL_COMPLETE`
    device->get_audiodevice()->peripherals->callback_tx(device->get_audiodevice(), CALLBACK_FULL_COMPLETE);
}

bool KlangstromEmulatorClient::evaluate_serial_msg(const OscMessage& msg, SerialDevice* device) {
    //    println("app: evaluate_serial_msg");
    if (PApplet::begins_with(msg.addrPattern(), KLST_EMU_SERIAL_ADDRESS_PATTERN)) {
        if (PApplet::begins_with(msg.typetag(), KLST_EMU_SERIAL_TYPETAG)) {
            const int         msg_device_type = msg.get(KLST_EMU_SERIAL_DEVICE_MSG_POSITION_TYPE).intValue();
            const int         msg_device_id   = msg.get(KLST_EMU_SERIAL_DEVICE_MSG_POSITION_ID).intValue();
            const std::string msg_data_str    = msg.get(KLST_EMU_SERIAL_DEVICE_MSG_POSITION_DATA).stringValue();
            const char*       msg_data        = msg_data_str.c_str();
            const int         msg_data_length = msg_data_str.length();
            //            const int         msg_data_length = msg.get(KLST_EMU_SERIAL_DEVICE_MSG_POSITION_LENGTH).intValue();

            /* copy data to serial device buffer */
            if (msg_data_str.length() != msg.get(KLST_EMU_SERIAL_DEVICE_MSG_POSITION_LENGTH).intValue()) {
                console_println("ERROR: data length mismatch. setting `length` to actual length of `data` buffer.");
            }

            /* trigger callback */
            if (msg_device_type == SERIAL_DEVICE_TYPE_UNDEFINED) {
                /* device by id, ignoring type */
                if (device->device_id == msg_device_id) {
                    update_serial_data(device, msg_data, msg_data_length);
                    device->callback_serial(device);
                    return true;
                }
            } else if (msg_device_id == SERIAL_DEVICE_ID_UNDEFINED) {
                /* device by type, ignoring ID */
                if (device->device_type == msg_device_type) {
                    update_serial_data(device, msg_data, msg_data_length);
                    device->callback_serial(device);
                    return true;
                }
            } else {
                /* device by id + type */
                if (device->device_type == msg_device_type &&
                    device->device_id == msg_device_id) {
                    update_serial_data(device, msg_data, msg_data_length);
                    device->callback_serial(device);
                    return true;
                }
            }
        }
    }
    return false;
}

void KlangstromEmulatorClient::receive(const OscMessage& msg) {
    //    println("app: received OSC message: ", msg.typetag());
    ArrayList_SerialDevicePtr* sd = system_get_registered_serialdevices();
    for (int i = 0; i < sd->size; i++) {
        SerialDevice* device = arraylist_SerialDevicePtr_get(sd, i);
        evaluate_serial_msg(msg, device); // TODO consider skipping other devices with `continue;` on success
    }
}

bool KlangstromEmulatorClient::update_serial_data(SerialDevice* device, const char* msg_data, const int msg_data_length) {
    device->length = msg_data_length;
    for (uint16_t i = 0; i < msg_data_length; ++i) {
        if (i >= device->data_buffer_size) {
            device->length = device->data_buffer_size;
            console_println("ERROR: data buffer overflow");
            return false;
        }
        device->data[i] = msg_data[i];
    }
    return true;
}

bool KlangstromEmulatorClient::handle_audiodevice(float** input, float** output, int length, KlangstromEmulatorAudioDevice* device) {
    AudioBlock& audioblock = *device->get_audiodevice()->audioblock;
    const int   block_size = audioblock.block_size;

    if (block_size > length) {
        println("block size mismatch: reduce device block size (", block_size, ") to either equal or smaller multiple of ", length);
        return true;
    }

    if (length % block_size != 0) {
        println("block size mismatch: device block size (", block_size, ") must be multiple of ", length);
        return true;
    }

    // TODO assuming that the underlying audio system always has 2 output and 1 or 2 input channels how would this be mapped. e.g:
    //     - mono output is mapped to both LEFT and RIGHT, stereo output is mapped to channel LEFT and RIGHT each, 3 channels are mapped to LEFT, CENTER;, RIGHT, etcetera
    // TODO handle cases where number of output and input channels do not match.
    // TODO especially when device has or expects more channels than audio system
    if (audioblock.output_channels > audio_output_channels) {
        println("output channels mismatch: device output channels (", static_cast<int>(audioblock.output_channels), ") must match audio system output channels (", audio_output_channels, ")");
        return true;
    }

    if (audioblock.input_channels > audio_input_channels && audioblock.input_channels > 2) {
        println("input channels mismatch: device input channels (", static_cast<int>(audioblock.input_channels), ") must match audio system input channels (", audio_input_channels, ")");
        return true;
    }

    const bool mIsPaused = device->get_audiodevice()->peripherals->is_paused;
    if (mIsPaused) {
        return true;
    }

    /* process audio data ( if need be in multiple passes ) */
    const uint8_t mPasses = length / block_size;
    for (uint8_t i = 0; i < mPasses; ++i) {
        for (int ch = 0; ch < audioblock.input_channels; ++ch) {
            // TODO if audio system ( i.e SDL ) provides only mono input,
            //  then we map all input channels to the same channel
            int    actual_channel = audio_input_channels == 1 ? 0 : ch;
            float* input_ptr      = input[actual_channel] + i * block_size;
            memcpy(audioblock.input[ch], input_ptr, block_size * sizeof(float));
        }

        process_device(device);

        for (int ch = 0; ch < audioblock.output_channels; ++ch) {
            float* output_ptr = output[ch] + i * block_size;
            for (int j = 0; j < block_size; ++j) {
                output_ptr[j] += audioblock.output[ch][j];
            }
        }
    }
    return false;
}