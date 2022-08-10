/*
 * Klangstrom
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2022 Dennis P Paul.
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

#ifndef klangstrom_wave_file_h
#define klangstrom_wave_file_h

namespace klangstrom {
    // __attribute__((packed))
    typedef struct KlangstromWaveHeader {
        /* RIFF Chunk Descriptor */
        uint8_t  RIFF[4];    // RIFF Header Magic header
        uint32_t ChunkSize;  // RIFF Chunk Size
        uint8_t  WAVE[4];    // WAVE Header
        /* "fmt" sub-chunk */
        uint8_t  fmt[4];              // FMT header
        uint32_t Subchunk1Size;       // Size of the fmt chunk
        uint16_t AudioFormat;         // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
        uint16_t NumOfChan;           // Number of channels 1=Mono 2=Sterio
        uint32_t SamplesPerSec;       // Sampling Frequency in Hz
        uint32_t bytesPerSec;         // bytes per second
        uint16_t bytePerSampleFrame;  // 2=16-bit mono, 4=16-bit stereo
        uint16_t bitsPerSample;       // Number of bits per sample
        /* "data" sub-chunk */
        uint8_t  Subchunk2ID[4];  // "data"  string
        uint32_t Subchunk2Size;   // Sampled data length
    } WaveHeader_t;

    class KlangstromWaveFile {
    public:
        static const uint8_t  CHANNEL_LEFT              = 0;
        static const uint8_t  CHANNEL_RIGHT             = 1;
        static const uint16_t AUDIO_FORMAT_PCM          = 1;
        static const uint16_t AUDIO_FORMAT_FLOAT        = 3;
        static const uint16_t AUDIO_FORMAT_MULAW        = 6;
        static const uint16_t AUDIO_FORMAT_ALAW         = 7;
        static const uint16_t AUDIO_FORMAT_IBM_MULAW    = 257;
        static const uint16_t AUDIO_FORMAT_ADPCM        = 259;
        static const uint16_t BLOCK_ALIGN_16_BIT_MONO   = 2;
        static const uint16_t BLOCK_ALIGN_16_BIT_STEREO = 4;

        WaveHeader_t* header            = nullptr;
        uint8_t*      sample_data       = nullptr;
        uint32_t      number_of_samples = 0;

        int16_t* get_sample_data(const uint8_t pChannel = 0) {
            return (int16_t*)(sample_data + (number_of_samples * pChannel * (header->bitsPerSample / 8)));
        }
        void purge() {
            if (header != nullptr) {
                delete header;
            }
            if (sample_data != nullptr) {
                delete[] sample_data;
            }
        }
    };
}  // namespace klangstrom

#endif
