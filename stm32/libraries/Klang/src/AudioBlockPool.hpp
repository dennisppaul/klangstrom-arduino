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

#ifndef AudioBlockPool_hpp
#define AudioBlockPool_hpp

#include "Klang.hpp"

#ifndef KLANG_AUDIO_BLOCKS
#error KLANG_AUDIO_BLOCKS not defined.
#endif

#ifdef KLANG_USE_HEAP_ALLOCATION
#warning KLANG_USE_HEAP_ALLOCATION is enabled.
#endif

namespace klang {
    class AudioBlockPool {
    public:
        static const AUDIO_BLOCK_ID NO_ID = -1;

        static AudioBlockPool& instance() {
            static AudioBlockPool instance;
            return instance;
        }

        ~AudioBlockPool() {
#ifdef KLANG_USE_HEAP_ALLOCATION
            for (AUDIO_BLOCK_ID i = 0; i < KLANG_AUDIO_BLOCKS; i++) {
                delete[] mAudioBlocks[i];
            }
            delete[] mAudioBlocks;
#endif
        }

        AUDIO_BLOCK_ID request() {
            for (AUDIO_BLOCK_ID i = 0; i < KLANG_AUDIO_BLOCKS; i++) {
                if (mAudioBlockAvailableIDs[i]) {
                    mAudioBlockAvailableIDs[i] = false;
                    mAudioBlocksNeeded++;
                    if (mAudioBlocksNeeded > mAudioBlocksNeededMax) {
                        mAudioBlocksNeededMax = mAudioBlocksNeeded;
                    }
                    return i;
                }
            }
#ifdef DEBUG_AUDIO_POOL
            KLANG_LOG_ERR("+++ @AudioBlockPool no more audio blocks available");
#endif
            return NO_ID;
        }

        void release(AUDIO_BLOCK_ID pAudioBlockID) {
            if (pAudioBlockID > NO_ID && pAudioBlockID < KLANG_AUDIO_BLOCKS) {
                mAudioBlockAvailableIDs[pAudioBlockID] = false;
                mAudioBlocksNeeded--;
            }
        }

        SIGNAL_TYPE* data(AUDIO_BLOCK_ID pAudioBlockID) {
            if (pAudioBlockID > NO_ID && pAudioBlockID < KLANG_AUDIO_BLOCKS) {
                return mAudioBlocks[pAudioBlockID];
            } else {
                return nullptr;
            }
        }

        void reset() {
            for (AUDIO_BLOCK_ID i = 0; i < KLANG_AUDIO_BLOCKS; i++) {
                mAudioBlockAvailableIDs[i] = true;
            }
            mAudioBlocksNeeded    = 0;
            mAudioBlocksNeededMax = 0;
        }

        uint8_t blocks_available() {
            return KLANG_AUDIO_BLOCKS - mAudioBlocksNeeded;
        }

        uint8_t blocks_used() {
            return mAudioBlocksNeeded;
        }

        uint8_t blocks_used_max() {
            return mAudioBlocksNeededMax;
        }

    private:
        AudioBlockPool(AudioBlockPool const&);
        void operator=(AudioBlockPool const&);

        bool mAudioBlockAvailableIDs[KLANG_AUDIO_BLOCKS];
#ifdef KLANG_USE_HEAP_ALLOCATION
        SIGNAL_TYPE** mAudioBlocks;
#else
        // @TODO(see if there is a way to move audioblock buffer to specific memory location in class ( with `static`? ) + test this on desktop version too)
        // static SIGNAL_TYPE mAudioBlocks[KLANG_AUDIO_BLOCKS][KLANG_SAMPLES_PER_AUDIO_BLOCK] __attribute__ ((section(".audio_block_buffer"))); // or make this global
        // SIGNAL_TYPE mAudioBlocks[KLANG_AUDIO_BLOCKS][KLANG_SAMPLES_PER_AUDIO_BLOCK];
        // @TODO(removed `inline` not sure why it was there in the first place.)
        inline static SIGNAL_TYPE mAudioBlocks[KLANG_AUDIO_BLOCKS][KLANG_SAMPLES_PER_AUDIO_BLOCK]
#if (KLST_ARCH == KLST_ARCH_MCU)
            __attribute__((section(".audio_block_buffer")))
        // @TODO(moved memory to RAM_D3 (0x38000000). not really happy about this plattform specificness at this point)
            // @NOTE
            // on KLST_SHEEP audio block buffer is located at 0x38000000 which has 64K of memory.
            // 2048 bytes are used by the DMA buffer. this leaves 63488 bytes for the audio blocks.
            // e.g at an audio block size of 256 bytes this would allow 248 audio blocks.
#endif
            ;
#endif

        uint8_t mAudioBlocksNeeded    = 0;
        uint8_t mAudioBlocksNeededMax = 0;

        AudioBlockPool() {
#ifdef KLANG_USE_HEAP_ALLOCATION
            mAudioBlocks = new SIGNAL_TYPE*[KLANG_AUDIO_BLOCKS];
            for (AUDIO_BLOCK_ID i = 0; i < KLANG_AUDIO_BLOCKS; i++) {
                SIGNAL_TYPE* mBlock = new SIGNAL_TYPE[KLANG_SAMPLES_PER_AUDIO_BLOCK];
                mAudioBlocks[i]     = mBlock;
            }
#endif
            for (AUDIO_BLOCK_ID i = 0; i < KLANG_AUDIO_BLOCKS; i++) {
                mAudioBlockAvailableIDs[i] = true;
            }
            reset();
        }
    };
}  // namespace klang

#endif /* AudioBlockPool_hpp */
