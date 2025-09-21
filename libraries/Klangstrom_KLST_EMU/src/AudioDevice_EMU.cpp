/*
* Klangstrom
*
* This file is part of the *Klangstrom* library (https://github.com/dennisppaul/klangstrom-libraries).
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

#include "KlangstromEnvironment.h"
#ifdef KLST_ARCH_IS_EMU

#include "AudioDevice_EMU.h"
#include "Console.h"
#include "KlangstromEmulator.h"

#include "PGraphics.h"
using namespace umfeld;

class DrawableAudioDevice final : public Drawable {
public:
    explicit DrawableAudioDevice(AudioDevice* audiodevice) : fAudioDevice(audiodevice) {}

    void draw(PGraphics* g_ptr) override {
        if (g_ptr == nullptr) {
            error_in_function("g_ptr is nullptr");
            return;
        }
        if (fAudioDevice == nullptr) {
            error_in_function("audio device is nullptr");
            return;
        }
        if (fAudioDevice->peripherals == nullptr) {
            error_in_function("audio device peripherals is nullptr");
            return;
        }

        PGraphics& graphics = *g_ptr;

        graphics.stroke(1.0f);
        graphics.noFill();
        graphics.pushMatrix();
        graphics.translate(20, 200);
        constexpr int      mStrafe                    = 16;
        constexpr float    mHeight                    = 150;
        constexpr float    mWidth                     = 512 - 20;
        constexpr uint16_t sample_frame_buffer_length = KLST_EMU_AUDIO_BLOCK;

        graphics.fill(1.0f);
        graphics.noStroke();
        graphics.textSize(KlangstromEmulator::DEFAULT_FONT_SIZE * 0.5f);
        graphics.text("AUDIO", -1, -2 - mHeight / 2);
        graphics.noFill();

        if (fAudioDevice->peripherals->is_paused) {
            graphics.fill(1.0f);
            graphics.noStroke();
            graphics.textSize(KlangstromEmulator::DEFAULT_FONT_SIZE * 0.5f);
            graphics.text("PAUSED", 3, -mHeight / 2 + KlangstromEmulator::DEFAULT_FONT_SIZE * 0.5f + 2);
            graphics.noFill();
        }

        float** mBuffers = KlangstromEmulator::instance()->get_audio_output_buffers(); // TODO get buffer from each device
        for (int i = 0; i < audio_output_channels; i++) {
            const auto ii = static_cast<float>(i);
            graphics.stroke(1, 0.5f);
            graphics.rect(0, ii * mHeight - mHeight * 0.5f, mWidth, mHeight);
            graphics.line(0, ii * mHeight, mWidth, ii * mHeight);
            graphics.stroke(1.0f);
            for (int j = mStrafe; j < sample_frame_buffer_length; j += mStrafe) {
                const float mSample0 = mBuffers[i][j - mStrafe] * 0.5f;
                const float mSample1 = mBuffers[i][j] * 0.5f;
                const float x0       = mWidth * static_cast<float>(j - mStrafe) / sample_frame_buffer_length;
                const float y0       = ii * mHeight + mSample0 * mHeight;
                const float x1       = mWidth * static_cast<float>(j) / sample_frame_buffer_length;
                const float y1       = ii * mHeight + mSample1 * mHeight;
                graphics.line(x0, y0, x1, y1);
                graphics.line(x0, y0, x0, ii * mHeight);
            }
        }

        mBuffers = KlangstromEmulator::instance()->get_audio_input_buffers();
        graphics.translate(0, mHeight * 2 + 10);
        for (int i = 0; i < audio_input_channels; i++) {
            const auto ii = static_cast<float>(i);
            graphics.stroke(1, 0.5f);
            graphics.rect(0, ii * mHeight - mHeight * 0.5f, mWidth, mHeight);
            graphics.line(0, ii * mHeight, mWidth, ii * mHeight);
            graphics.stroke(1.0f);
            for (int j = mStrafe; j < sample_frame_buffer_length; j += mStrafe) {
                const float mSample0 = mBuffers[i][j - mStrafe] * 0.5f;
                const float mSample1 = mBuffers[i][j] * 0.5f;
                const float x0       = mWidth * static_cast<float>(j - mStrafe) / sample_frame_buffer_length;
                const float y0       = ii * mHeight + mSample0 * mHeight;
                const float x1       = mWidth * static_cast<float>(j) / sample_frame_buffer_length;
                const float y1       = ii * mHeight + mSample1 * mHeight;
                graphics.line(x0, y0, x1, y1);
                graphics.line(x0, y0, x0, ii * mHeight);
            }
        }
        graphics.popMatrix();
    }

private:
    AudioDevice* fAudioDevice;
};

#ifdef __cplusplus
extern "C" {
#endif

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void audiodevice_resume(AudioDevice* audiodevice) {
    if (audiodevice == nullptr) {
        error_in_function("audiodevice is nullptr");
        return;
    }
    audiodevice->peripherals->is_paused = false;
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void audiodevice_pause(AudioDevice* audiodevice) {
    if (audiodevice == nullptr) {
        error_in_function("audiodevice is nullptr");
        return;
    }
    audiodevice->peripherals->is_paused = true;
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void audiodevice_deinit_BSP(AudioDevice* audiodevice) {
    (void) audiodevice;
}

void audiodevice_init_peripherals_BSP(AudioDevice* audiodevice) {
    if (audiodevice == nullptr) {
        error_in_function("audiodevice is nullptr");
        return;
    }
    audiodevice->peripherals = new AudioDevicePeripherals();
}

void audiodevice_deinit_peripherals_BSP(AudioDevice* audiodevice) {
    if (audiodevice == nullptr) {
        error_in_function("audiodevice is nullptr");
        return;
    }
    delete audiodevice->peripherals;
    audiodevice->peripherals = nullptr;
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
static void rx_input_callback(AudioDevice* audiodevice, const uint8_t callback_type) {
    if (audiodevice == nullptr) {
        error_in_function("audiodevice is nullptr");
        return;
    }
    if (callback_type == CALLBACK_FULL_COMPLETE) {
    } else if (callback_type == CALLBACK_HALF_COMPLETE) {
    }
    (void) audiodevice; // TODO maybe copy input data to audioblock?
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
static void tx_output_callback(AudioDevice* audiodevice, const uint8_t callback_type) {
    if (audiodevice == nullptr) {
        error_in_function("audiodevice is nullptr");
        return;
    }
    if (callback_type == CALLBACK_FULL_COMPLETE) {
    } else if (callback_type == CALLBACK_HALF_COMPLETE) {
    }
    if (audiodevice->callback_audioblock != nullptr) {
        audiodevice->callback_audioblock(audiodevice->audioblock);
    }
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
static void error_callback(AudioDevice* audiodevice, const uint8_t callback_type) {
    (void) audiodevice;
    (void) callback_type;
    console_error("error in audio device");
}

void audiodevice_init_device_BSP(AudioDevice* audiodevice) {
    static bool initialized = false;
    if (audiodevice == nullptr) {
        error_in_function("audiodevice is nullptr");
        return;
    }
    if (audiodevice->peripherals == nullptr) {
        error_in_function("audiodevice peripherals is nullptr");
        return;
    }
    if (!initialized) {
        initialized                              = true;
        audiodevice->peripherals->callback_rx    = rx_input_callback;
        audiodevice->peripherals->callback_tx    = tx_output_callback;
        audiodevice->peripherals->callback_error = error_callback;
        KlangstromEmulator::instance()->register_audio_device(audiodevice);
        KlangstromEmulator::instance()->register_drawable(new DrawableAudioDevice(audiodevice));
        audiodevice_pause(audiodevice);
    }
}

#ifdef __cplusplus
}
#endif

#endif // KLST_ARCH_IS_EMU