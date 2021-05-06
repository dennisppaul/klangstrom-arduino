//
//  AudioBlockPool.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

#ifndef AudioBlockPool_hpp
#define AudioBlockPool_hpp

#include "Klang.hpp"

#ifndef KLANG_AUDIO_BLOCKS
#error KLANG_AUDIO_BLOCKS not defined.
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
            for (AUDIO_BLOCK_ID i=0; i<KLANG_AUDIO_BLOCKS; i++) {
                delete [] mAudioBlocks[i];
            }
            delete [] mAudioBlocks;
        }
        
        AUDIO_BLOCK_ID request() {
            for (AUDIO_BLOCK_ID i=0; i<KLANG_AUDIO_BLOCKS; i++) {
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
        
        inline SIGNAL_TYPE* data(AUDIO_BLOCK_ID pAudioBlockID) {
            if (pAudioBlockID > NO_ID && pAudioBlockID < KLANG_AUDIO_BLOCKS) {
                return mAudioBlocks[pAudioBlockID];
            } else {
                return nullptr;
            }
        }
        
        void reset() {
            for (AUDIO_BLOCK_ID i=0; i<KLANG_AUDIO_BLOCKS; i++) {
                mAudioBlockAvailableIDs[i] = true;
            }
            mAudioBlocksNeeded = 0;
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
        
        SIGNAL_TYPE**           mAudioBlocks;
        bool*                   mAudioBlockAvailableIDs;
        uint8_t                 mAudioBlocksNeeded = 0;
        uint8_t                 mAudioBlocksNeededMax = 0;
        
        AudioBlockPool() {
            mAudioBlocks = new SIGNAL_TYPE*[KLANG_AUDIO_BLOCKS];
            mAudioBlockAvailableIDs = new bool[KLANG_AUDIO_BLOCKS];
            for (AUDIO_BLOCK_ID i=0; i<KLANG_AUDIO_BLOCKS; i++) {
                SIGNAL_TYPE* mBlock = new SIGNAL_TYPE[KLANG_SAMPLES_PER_AUDIO_BLOCK];
                mAudioBlocks[i] = mBlock;
            }
            reset();
        }
    };
}

#endif /* AudioBlockPool_hpp */
