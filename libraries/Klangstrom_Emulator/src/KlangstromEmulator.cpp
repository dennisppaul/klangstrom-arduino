/*
 * Klangstrom
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2025 Dennis P Paul.
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

#include <iostream>

#include "Arduino.h"
#include "KlangstromEnvironment.h"
#include "KlangstromEmulator.h"

#include "stm32_hal_callbacks.h"
#include "stm32_hal.h"
#include "KLST_EMU_PANDA_Peripheral_Defines.h"

#ifndef KLST_ENV
#warning "no environment defined in KLST_ENV"
#endif

#ifndef KLST_EMU_SCREEN_WIDTH
#define KLST_EMU_SCREEN_WIDTH 1024
#endif
#ifndef KLST_EMU_SCREEN_HEIGHT
#define KLST_EMU_SCREEN_HEIGHT 768
#endif

static void sketch_setup() {
    ::setup();
}

static void sketch_loop() {
    ::loop();
}

umfeld::KlangstromEmulator* umfeld::KlangstromEmulator::fInstance = nullptr;

void umfeld::KlangstromEmulator::arguments(const std::vector<std::string> args) {
    for (auto& s: args) {
        println("> ", s);
        if (begins_with(s, "--fontpath=")) {
            println("found fontpath: ", get_string_from_argument(s));
            mFontPath = get_string_from_argument(s);
        }
    }
}

int umfeld::KlangstromEmulator::get_width() {
    return umfeld::width;
}

int umfeld::KlangstromEmulator::get_height() {
    return umfeld::height;
}

void umfeld::KlangstromEmulator::setup() {
    println("sketchpath: ", sketchPath());
    println("width     : ", umfeld::width);
    println("height    : ", umfeld::height);

    std::string font_file;
#ifdef KLST_EMULATOR_FONT_PATH
    println("font file : ", KLST_EMULATOR_FONT_PATH);
    if (exists(KLST_EMULATOR_FONT_PATH)) {
        font_file = KLST_EMULATOR_FONT_PATH;
    } else {
        println("fontfile in '", KLST_EMULATOR_FONT_PATH, "' does not exist");
    }
#endif

    if (font_file.empty()) {
        if (file_exists(mFontPath + "/" + mFontName)) {
            font_file = mFontPath + "/" + mFontName;
        } else if (file_exists(mFontPath + "/../" + mFontName)) {
            font_file = mFontPath + "/../" + mFontName;
        } else if (file_exists(mFontPath + "/data/" + mFontName)) {
            font_file = mFontPath + "/data/" + mFontName;
        }
        println("font file : ", font_file);
    }

    if (!font_file.empty()) {
        mFont = loadFont(font_file, DEFAULT_FONT_SIZE);
        textFont(mFont);
    } else {
        println("could not load font: ",
                (mFontPath + "/" + mFontName),
                " or ",
                (mFontPath + "/../" + mFontName),
                " or ",
                (mFontPath + "/data/" + mFontName)
#ifdef KLST_EMULATOR_FONT_PATH
                    ,
                " or ",
                font_file
#endif
        );
    }

    osc_setup();
    sketch_setup();

    task.set_callback(sketch_loop);
    task.set_frequency(1000);
    task.start();
}

void umfeld::KlangstromEmulator::settings() {
    run_update_in_thread = true;

    // TODO this needs to be handled BETTER:
    size(KLST_EMU_SCREEN_WIDTH, KLST_EMU_SCREEN_HEIGHT);
    // TODO use KLST_EMU_AUDIO_BLOCK to configure audio
    // TODO use KLST_EMU_SAMPLE_RATE to configure audio
    // audio(2, 2);
    subsystem_audio = umfeld_create_subsystem_audio_portaudio();
    audio(KLST_EMU_AUDIO_INPUT_CHANNELS,
          KLST_EMU_AUDIO_OUTPUT_CHANNELS,
          KLST_EMU_SAMPLE_RATE,
          KLST_EMU_AUDIO_BLOCK,
          KLST_EMU_AUDIO_INPUT_DEVICE,
          KLST_EMU_AUDIO_OUTPUT_DEVICE,
          KLST_EMU_AUDIO_THREADED);

    mOutputBuffers = new float*[audio_output_channels];
    for (int i = 0; i < audio_output_channels; i++) {
        mOutputBuffers[i] = new float[KLST_EMU_AUDIO_BLOCK];
    }
    mInputBuffers = new float*[audio_input_channels];
    for (int i = 0; i < audio_input_channels; i++) {
        mInputBuffers[i] = new float[KLST_EMU_AUDIO_BLOCK];
    }
}

void umfeld::KlangstromEmulator::draw() {
    background(0.2);

    // fill(0);
    // text(get_emulator_name(), 10, 10 + 32);

    // stroke(0);
    // noFill();
    // rect(10, 10, width / 2 - 20, height / 2 - 20);

    // noStroke();
    // fill(random(0, 0.2));
    // rect(20, 20, width / 2 - 40, height / 2 - 40);

    fill(1.0f);
    noStroke();
    textSize(DEFAULT_FONT_SIZE);
    text(get_emulator_name(), 25, 10 + DEFAULT_FONT_SIZE);

    for (const auto& drawable: drawables) {
        drawable->draw(g);
    }
}

void umfeld::KlangstromEmulator::update() {
    // TODO only call loop if not delayed
    loop();
}

// void KlangstromEmulator::process_device(KlangstromEmulatorAudioDevice* device) {
//     client->process_device(device);
// }

static void copy_float_array_2D(float** src, float** dest, const int rows, const int cols) {
    for (int i = 0; i < rows; ++i) {
        memcpy(dest[i], src[i], cols * sizeof(float));
    }
}

bool umfeld::KlangstromEmulator::handle_audiodevice(float** input, float** output, const int length, KlangstromEmulatorAudioDevice* device) {
    if (client == nullptr) {
        error_in_function("client not initialized");
        return false;
    }
    return client->handle_audiodevice(input, output, length, device);
}

/**
 * called from umfeld to request and process audio data
 * @param input
 * @param output
 * @param length
 */
void umfeld::KlangstromEmulator::audioblock(float** input, float** output, const int length) {
    if (fAudioDevices.size() > 1) {
        warning_in_function_once("multiple audio devices detected. currently only one device supported. only the last audio device will be audible ...");
    }

    for (int ch = 0; ch < audio_output_channels; ++ch) {
        memset(output[ch], 0, length * sizeof(float));
    }

    for (auto* device: fAudioDevices) {
        if (handle_audiodevice(input, output, length, device)) {
            continue;
        }
    }

    /* sanitze and clamp output samples */
    for (int ch = 0; ch < audio_output_channels; ++ch) {
        for (int i = 0; i < length; i++) {
            constexpr float audio_sample_clamp_range = 0.95f;
            // NOTE clamp to [-1.0, 1.0]
            float s       = output[ch][i];
            s             = std::isnan(s) ? 0.0f : s;
            s             = std::max(-audio_sample_clamp_range, std::min(audio_sample_clamp_range, s));
            output[ch][i] = s;
        }
    }

    /* fill buffers for oscilloscope visualization */
    if (mOutputBuffers == nullptr || mInputBuffers == nullptr) {
        return;
    }
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < audio_output_channels; j++) {
            mOutputBuffers[j][i] = constrain(output[j][i], -1.0f, 1.0f);
        }
    }
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < audio_input_channels; j++) {
            mInputBuffers[j][i] = constrain(input[j][i], -1.0f, 1.0f);
        }
    }
}

void umfeld::KlangstromEmulator::keyPressed() {
    // TODO this is a bit hacky maybe there is a better way to handle this
#if defined(KLST_PANDA_EMU)
    if (key == '1') {
        if (HAL_GPIO_ReadPin(_MECH_BUTTON_00_GPIO_Port, _MECH_BUTTON_00_Pin) == GPIO_PIN_RESET) {
            HAL_GPIO_WritePin(_MECH_BUTTON_00_GPIO_Port, _MECH_BUTTON_00_Pin, GPIO_PIN_SET);
            HAL_GPIO_EXTI_Callback(_MECH_BUTTON_00_Pin);
        }
    }
    if (key == '2') {
        if (HAL_GPIO_ReadPin(_MECH_BUTTON_01_GPIO_Port, _MECH_BUTTON_01_Pin) == GPIO_PIN_RESET) {
            HAL_GPIO_WritePin(_MECH_BUTTON_01_GPIO_Port, _MECH_BUTTON_01_Pin, GPIO_PIN_SET);
            HAL_GPIO_EXTI_Callback(_MECH_BUTTON_01_Pin);
        }
    }
#endif // KLST_PANDA_EMU
#ifdef KLST_ENCODER_WIP
    if (key == '3') {
        TIM_HandleTypeDef htim;
        htim.counter      = 23;
        htim.timer_handle = 1;
        HAL_TIM_IC_CaptureCallback(&htim);
    }
#endif
}

void umfeld::KlangstromEmulator::keyReleased() {
    // TODO this is a bit hacky maybe there is a better way to handle this
#if defined(KLST_PANDA_EMU)
    if (key == '1') {
        HAL_GPIO_WritePin(_MECH_BUTTON_00_GPIO_Port, _MECH_BUTTON_00_Pin, GPIO_PIN_RESET);
        HAL_GPIO_EXTI_Callback(_MECH_BUTTON_00_Pin);
    }
    if (key == '2') {
        HAL_GPIO_WritePin(_MECH_BUTTON_01_GPIO_Port, _MECH_BUTTON_01_Pin, GPIO_PIN_RESET);
        HAL_GPIO_EXTI_Callback(_MECH_BUTTON_01_Pin);
    }
#endif // KLST_PANDA_EMU
}

std::string umfeld::KlangstromEmulator::get_emulator_name() {
#if defined(GENERIC_EMU)
    return "GENERIC";
#elif defined(KLST_CORE_EMU)
    return "KLST_CORE";
#elif defined(KLST_TINY_EMU)
    return "KLST_TINY";
#elif defined(KLST_SHEEP_EMU)
    return "KLST_SHEEP";
#elif defined(KLST_PANDA_EMU)
    return "KLST_PANDA";
#elif defined(KLST_CATERPILLAR_EMU)
    return "KLST_CATERPILLAR";
#else
    return "(UNDEFINED)";
#endif
}

umfeld::KlangstromEmulator* umfeld::KlangstromEmulator::instance() {
    if (fInstance == nullptr) {
        fInstance = new KlangstromEmulator();
    }
    return fInstance;
}

// PApplet* umfeld::instance() {
//     return KlangstromEmulator::instance();
// }

void umfeld::KlangstromEmulator::register_drawable(Drawable* drawable) {
    drawables.push_back(drawable);
}

void umfeld::KlangstromEmulator::unregister_drawable(Drawable* drawable) {
    const auto it = std::remove(drawables.begin(), drawables.end(), drawable);
    drawables.erase(it, drawables.end());
}

uint8_t umfeld::KlangstromEmulator::unregister_audio_device(AudioDevice* audiodevice) {
    warning("TODO unregister audio device at emulator");
    return 0;
}

uint8_t umfeld::KlangstromEmulator::register_audio_device(AudioDevice* audiodevice) {
    auto* mAudioDevice = new KlangstromEmulatorAudioDevice(audiodevice, audio_device_id);
    fAudioDevices.push_back(mAudioDevice);
    audio_device_id++;
    return mAudioDevice->get_id();
}

uint8_t umfeld::KlangstromEmulator::register_serial_device(SerialDevice* serialdevice) {
    (void) serialdevice;
    //    auto* mAudioDevice = new KlangstromEmulatorAudioDevice(audiodevice, audio_device_id);
    //    fAudioDevices.push_back(mAudioDevice);
    //    audio_device_id++;
    //    return mAudioDevice->get_id();
    warning("TODO register serial device at emulator");
    return 0;
}

uint8_t umfeld::KlangstromEmulator::unregister_serial_device(SerialDevice* serialdevice) {
    warning("TODO unregister serial device at emulator");
    return 0;
}

void umfeld::KlangstromEmulator::delay_loop(const uint32_t microseconds) {
    //    task.sleep_for(microseconds); // TODO check what s the problem with this
    // std::this_thread::sleep_for(std::chrono::microseconds(microseconds)); // NOTE this stalls the entire system

    // Cooperative delay: break into small slices to keep SDL responsive
    using clock    = std::chrono::steady_clock;
    const auto end = clock::now() + std::chrono::microseconds(microseconds);

    // Slice length ~0.1 ms; adjust as needed
    // constexpr auto slice = std::chrono::milliseconds(1);
    constexpr auto slice = std::chrono::microseconds(100);

    while (clock::now() < end) {
        // Let SDL process events and render between slices
        // ... existing code ...
        // If you have a centralized place that already pumps SDL each frame,
        // you can call a lightweight tick here, e.g.:
        // this->update(); // if appropriate
        // this->redraw(); // if appropriate
        // ... existing code ...
        std::this_thread::sleep_for(slice);
    }
}

// bool KlangstromEmulator::update_serial_data(SerialDevice* device, const char* msg_data, const int msg_data_length) {
//     device->length = msg_data_length;
//     for (uint16_t i = 0; i < msg_data_length; ++i) {
//         if (i >= device->data_buffer_size) {
//             device->length = device->data_buffer_size;
//             println("ERROR: data buffer overflow");
//             return false;
//         }
//         device->data[i] = msg_data[i];
//     }
//     return true;
// }

// bool KlangstromEmulator::evaluate_serial_msg(const OscMessage& msg, SerialDevice* device) {
//     //    println("app: evaluate_serial_msg");
//     if (begins_with(msg.addrPattern(), KLST_EMU_SERIAL_ADDRESS_PATTERN)) {
//         if (begins_with(msg.typetag(), KLST_EMU_SERIAL_TYPETAG)) {
//             const int         msg_device_type = msg.get(KLST_EMU_SERIAL_DEVICE_MSG_POSITION_TYPE).intValue();
//             const int         msg_device_id   = msg.get(KLST_EMU_SERIAL_DEVICE_MSG_POSITION_ID).intValue();
//             const std::string msg_data_str    = msg.get(KLST_EMU_SERIAL_DEVICE_MSG_POSITION_DATA).stringValue();
//             const char*       msg_data        = msg_data_str.c_str();
//             const int         msg_data_length = msg_data_str.length();
//             //            const int         msg_data_length = msg.get(KLST_EMU_SERIAL_DEVICE_MSG_POSITION_LENGTH).intValue();
//
//             /* copy data to serial device buffer */
//             if (msg_data_str.length() != msg.get(KLST_EMU_SERIAL_DEVICE_MSG_POSITION_LENGTH).intValue()) {
//                 println("ERROR: data length mismatch. setting `length` to actual length of `data` buffer.");
//             }
//
//             /* trigger callback */
//             if (msg_device_type == SERIAL_DEVICE_TYPE_UNDEFINED) {
//                 /* device by id, ignoring type */
//                 if (device->device_id == msg_device_id) {
//                     update_serial_data(device, msg_data, msg_data_length);
//                     device->callback_serial(device);
//                     return true;
//                 }
//             } else if (msg_device_id == SERIAL_DEVICE_ID_UNDEFINED) {
//                 /* device by type, ignoring ID */
//                 if (device->device_type == msg_device_type) {
//                     update_serial_data(device, msg_data, msg_data_length);
//                     device->callback_serial(device);
//                     return true;
//                 }
//             } else {
//                 /* device by id + type */
//                 if (device->device_type == msg_device_type &&
//                     device->device_id == msg_device_id) {
//                     update_serial_data(device, msg_data, msg_data_length);
//                     device->callback_serial(device);
//                     return true;
//                 }
//             }
//         }
//     }
//     return false;
// }

void umfeld::KlangstromEmulator::receive(const OscMessage& msg) {
    error("emulator not handling OSC messages … WIP");

    //     if (client == nullptr) {
    //         println("ERROR: client not initialized");
    //         return;
    //     }
    //     client->receive(msg);

    // //    println("app: received OSC message: ", msg.typetag());
    // ArrayList_SerialDevicePtr* sd = system_get_registered_serialdevices();
    // for (int i = 0; i < sd->size; i++) {
    //     SerialDevice* device = arraylist_SerialDevicePtr_get(sd, i);
    //     evaluate_serial_msg(msg, device); // TODO consider skipping other devices with `continue;` on success
    // }
}
