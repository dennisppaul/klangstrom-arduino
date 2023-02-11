/*
 * Klang – a node+text-based synthesizer library
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

/**
 *       [ NODE_TTS_SAM        ]
 *       +---------------------+
 *       |                     |
 *       |              SIGNAL |--OUT00
 *       |                     |
 *       +---------------------+
 */

#ifndef NodeTextToSpeechSAM_hpp
#define NodeTextToSpeechSAM_hpp

#include <string.h>

#include <sstream>
#include <string>

#include "reciter.h"
#include "sam.h"

using namespace std;

extern char*    SAM_buffer;
extern uint32_t SAM_buffer_max_length;

namespace klang {
    class NodeTextToSpeechSAM : public Node {
    public:
        static const CHANNEL_ID NUM_CH_IN  = 0;
        static const CHANNEL_ID NUM_CH_OUT = 1;

        NodeTextToSpeechSAM() : NodeTextToSpeechSAM(65536) {}

        NodeTextToSpeechSAM(uint32_t pBufferLength) {
            SAM_buffer            = new char[pBufferLength];
            SAM_buffer_max_length = pBufferLength;
            fAllocatedBuffer      = true;
            setDefaults();
        }

        NodeTextToSpeechSAM(char* pBuffer, uint32_t pBufferLength) {
            SAM_buffer            = pBuffer;
            SAM_buffer_max_length = pBufferLength;
            fAllocatedBuffer      = false;
            setDefaults();
        }

        ~NodeTextToSpeechSAM() {
            if (fAllocatedBuffer) {
                delete[] SAM_buffer;
            }
        }

        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) { return false; }

        bool disconnect(CHANNEL_ID pInChannel) { return false; }

        void set_pitch(uint8_t pPitch) {
            mPitch = pPitch;
            SetPitch(pPitch);  // default: pitch = 64
        }

        void set_throat(uint8_t pThroat) {
            mThroat = pThroat;
            SetThroat(pThroat);  // default: throat = 128
        }

        void set_speed(uint8_t pSpeed) {
            mSpeed = pSpeed;
            SetSpeed(pSpeed);
        }

        void set_mouth(uint8_t pMouth) {
            mMouth = pMouth;
            SetMouth(pMouth);
        }

        void set_sing_mode(bool pMode) {
            if (pMode) {
                EnableSingmode();
            } else {
                DisableSingmode();
            }
        }

        void speak(string pText, bool pUsePhonemes = false) {
            char input[256];
            if (pUsePhonemes) {
                strcpy(input, pText.c_str());
            } else {
                char mText[256];
                strcpy(mText, pText.c_str());
                for (uint8_t i = 0; i < 255; i++) input[i] = 0;
                strncat(input, mText, 255);
                strncat(input, "[", 255);
                TextToPhonemes(input);
                //    std::cout << "TextToPhonemes: " << input << std::endl;
            }
            SetInput(input);
            SAMMain();
            mDoneSpeaking = false;
            mCounter      = 0;
        }

        void speak_ascii(int pASCIIValue) {
            stringstream ss;
            ss << (char)pASCIIValue;
            string s;
            ss >> s;
            speak(s);
        }

        uint32_t get_used_buffer_length() {
            return GetBufferLength() / 50;
        }

        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            if (pChannel == CH_OUT_SIGNAL) {
                uint8_t*       mBuffer       = (uint8_t*)GetBuffer();
                const uint32_t mBufferLength = GetBufferLength() / 50;
                for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i += 2) {
                    if (!mDoneSpeaking && mBufferLength > 0) {
                        pAudioBlock[i] = mBuffer[mCounter] / 255.0 * 2.0 - 1.0;
                        mCounter++;
                        if (mCounter >= mBufferLength) {
                            mDoneSpeaking = true;
                        }
                        mCounter %= mBufferLength;
                    } else {
                        pAudioBlock[i] = 0.0;
                    }
                    pAudioBlock[i + 1] = pAudioBlock[i];
                }
            }
        }

        void set_command(const KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {}

    private:
        uint8_t mPitch;
        uint8_t mThroat;
        uint8_t mMouth;
        uint8_t mSpeed;

        uint32_t mCounter         = 0;
        bool     mDoneSpeaking    = false;
        bool     fAllocatedBuffer = false;

        void setDefaults() {
            set_pitch(64);
            set_throat(128);
            set_speed(72);
            set_mouth(128);
        }
    };
}  // namespace klang

#endif /* NodeTextToSpeechSAM_hpp */
