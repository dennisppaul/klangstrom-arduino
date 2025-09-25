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

#pragma once

#include <vector>

#include "Umfeld.h"
#include "Drawable.h"
#include "PeriodicalTask.h"
#include "PApplet.h"
#include "PVector.h"

#include "KlangstromEmulatorClient.h"
#include "KlangstromEmulatorAudioDevice.h"
#include "../../Klangstrom/src/Console.h"

#ifndef KLST_EMU_AUDIO_BLOCK
#define KLST_EMU_AUDIO_BLOCK 128
#endif
#ifndef KLST_EMU_AUDIO_INPUT_CHANNELS
#define KLST_EMU_AUDIO_INPUT_CHANNELS 2
#endif
#ifndef KLST_EMU_AUDIO_OUTPUT_CHANNELS
#define KLST_EMU_AUDIO_OUTPUT_CHANNELS 2
#endif
#ifndef KLST_EMU_SAMPLE_RATE
#define KLST_EMU_SAMPLE_RATE 48000
#endif
#ifndef KLST_EMU_AUDIO_INPUT_DEVICE
#define KLST_EMU_AUDIO_INPUT_DEVICE DEFAULT_AUDIO_DEVICE
#endif
#ifndef KLST_EMU_AUDIO_OUTPUT_DEVICE
#define KLST_EMU_AUDIO_OUTPUT_DEVICE DEFAULT_AUDIO_DEVICE
#endif
#ifndef KLST_EMU_AUDIO_THREADED
#define KLST_EMU_AUDIO_THREADED true
#endif

#define KLST_EMULATE_SERIAL_VIA_OSC

#ifdef KLST_EMULATE_SERIAL_VIA_OSC
#include "OSC.h"
#endif // KLST_EMULATE_SERIAL_VIA_OSC
#ifndef KLST_EMULATE_SERIAL_IP
#define KLST_EMULATE_SERIAL_IP "127.0.0.1"
#endif // KLST_EMULATE_SERIAL_IP
#ifndef KLST_EMULATE_SERIAL_TX
#define KLST_EMULATE_SERIAL_TX 7000
#endif // KLST_EMULATE_SERIAL_TX
#ifndef KLST_EMULATE_SERIAL_RX
#define KLST_EMULATE_SERIAL_RX 7001
#endif // KLST_EMULATE_SERIAL_RX

static auto              KLST_EMU_SERIAL_ADDRESS_PATTERN            = "/klst/serial";
static auto              KLST_EMU_SERIAL_TYPETAG                    = "iisi"; // type, id, data(byte), length
static constexpr uint8_t KLST_EMU_SERIAL_DEVICE_MSG_POSITION_TYPE   = 0;
static constexpr uint8_t KLST_EMU_SERIAL_DEVICE_MSG_POSITION_ID     = 1;
static constexpr uint8_t KLST_EMU_SERIAL_DEVICE_MSG_POSITION_DATA   = 2;
static constexpr uint8_t KLST_EMU_SERIAL_DEVICE_MSG_POSITION_LENGTH = 3;

struct SerialDevice;

namespace umfeld {
    class KlangstromEmulator : public umfeld::PApplet, OSCListener {
        PVector           mVector{16, 16};
        std::string       mFontPath = sketchPath();
        const std::string mFontName = "JetBrainsMonoNL-Light.ttf";
        PFont*            mFont     = nullptr;

        // TODO clean up!!! e.g move to implementation
#ifdef KLST_EMULATE_SERIAL_VIA_OSC
        OSC mOSC{KLST_EMULATE_SERIAL_IP, KLST_EMULATE_SERIAL_TX, KLST_EMULATE_SERIAL_RX};

        static bool evaluate_serial_msg(const OscMessage& msg, SerialDevice* device);
        void        receive(const OscMessage& msg) override;

        void osc_setup() {
            mOSC.callback(this);
        }

    public:
        template<typename... Args>
        void osc_send(const std::string& addr_pattern, Args... args) {
            mOSC.send(addr_pattern, args...);
        }

        void osc_send(OscMessage msg) {
            mOSC.send(msg);
        }

#endif // KLST_EMULATE_SERIAL_VIA_OSC

    public:
        ~KlangstromEmulator() override {
            for (const auto* device: fAudioDevices) {
                delete device;
            }
            task.stop();
        }

        static float mouseX() {
            return umfeld::mouseX;
        }

        static float mouseY() {
            return umfeld::mouseY;
        }

        static KlangstromEmulator* instance();
        void                       arguments(std::vector<std::string> args) override;
        void                       settings() override;
        void                       setup() override;
        void                       draw() override;
        static void                update();
        bool                       handle_audiodevice(float** input, float** output, int length, KlangstromEmulatorAudioDevice* device);
        void                       audioblock(float** input, float** output, int length) override;
        void                       keyPressed() override;
        void                       keyReleased() override;
        static std::string         get_emulator_name();
        void                       register_drawable(Drawable* drawable);
        void                       unregister_drawable(Drawable* drawable);
        static void                delay_loop(uint32_t microseconds);
        void                       set_emulator_speed(float loop_frequency_hz) { task.set_frequency(loop_frequency_hz); }
        float**                    get_audio_output_buffers() const { return mOutputBuffers; }
        float**                    get_audio_input_buffers() const { return mInputBuffers; }
        uint8_t                    register_audio_device(AudioDevice* audiodevice);
        static uint8_t             unregister_audio_device(AudioDevice* audiodevice);
        static uint8_t             register_serial_device(SerialDevice* serialdevice);
        static uint8_t             unregister_serial_device(SerialDevice* serialdevice);
        void                       register_client(KlangstromEmulatorClient* client) { this->client = client; }
        int                        get_width() override;
        int                        get_height() override;

        static constexpr float DEFAULT_FONT_SIZE = 24;

    private:
        static KlangstromEmulator*                  fInstance;
        PeriodicalTask                              task;
        std::vector<Drawable*>                      drawables;
        float**                                     mOutputBuffers  = nullptr;
        float**                                     mInputBuffers   = nullptr;
        uint8_t                                     audio_device_id = 0;
        std::vector<KlangstromEmulatorAudioDevice*> fAudioDevices;
        KlangstromEmulatorClient*                   client = nullptr;

        // static bool update_serial_data(SerialDevice* device, const char* msg_data, const int msg_data_length);
    };
} // namespace umfeld