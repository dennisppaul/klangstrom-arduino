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

#pragma once

#include <cstring>
#include <cstdint>
#include <memory>
#include "SDCard.h"
#include "Console.h"

class AudioStreamFloat32 {
public:
    AudioStreamFloat32(uint32_t buffer_size)
        : fBufferSize(buffer_size), fReadBuffer(new float[buffer_size]()) {
        clear_buffer(fReadBuffer.get(), buffer_size);
    }

    ~AudioStreamFloat32() = default; // The unique_ptr will automatically clean up

    bool update() {
        if (sdcard_file_eof() && fLoop) {
            if (!sdcard_file_seek(0)) {
                return false;
            }
        }

        uint32_t mBytesRead   = sdcard_file_read(reinterpret_cast<uint8_t*>(fReadBuffer.get()), fBufferSize * sizeof(float));
        uint32_t elementsRead = mBytesRead / sizeof(float);

        if (mBytesRead < fBufferSize * sizeof(float)) {
            clear_buffer(fReadBuffer.get() + elementsRead, fBufferSize - elementsRead);
        }

        // if (mBytesRead == 0) {}
        return true;
    }

    float* buffer() const {
        return fReadBuffer.get();
    }

    void set_loop(bool loop) {
        fLoop = loop;
    }

private:
    std::unique_ptr<float[]> fReadBuffer;
    const uint32_t           fBufferSize;
    bool                     fLoop = true;

    static void clear_buffer(float* buffer, uint32_t size) {
        std::memset(buffer, 0, sizeof(float) * size);
    }
};