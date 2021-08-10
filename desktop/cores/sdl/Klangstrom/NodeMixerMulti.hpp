//
//  NodeMixerMulti.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_MIXER_MULTI    ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL_00    SIGNAL |--OUT00
 *   ... | ...                 |
 * INFF--| SIGNAL_FF           |
 *       |                     |
 *       +---------------------+
 */

#ifndef NodeMixerMulti_hpp
#define NodeMixerMulti_hpp

#include <vector>

#include "Node.hpp"

using namespace std;

namespace klang {
    class NodeMixerMulti : public Node {
    public:
        static const CHANNEL_ID NUM_CH_IN           = 4;
        static const CHANNEL_ID CH_OUT_SIGNAL_RIGHT = 1;
        static const CHANNEL_ID NUM_CH_OUT          = 2;

        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            add_channel(pInChannel, pConnection);
            return true;
        }

        bool disconnect(CHANNEL_ID pInChannel) {
            if (pInChannel < get_number_of_channels()) {
                mConnection_CH_IN_SIGNAL_and_mix[pInChannel].connection = nullptr;
                return true;
            }
            return false;
        }

        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            const uint16_t mNumberOfChannels = get_number_of_channels();
            bool           m_has_SIGNAL[mNumberOfChannels];
            uint8_t        mSignalInputCounter = 0;
            for (uint8_t i = 0; i < mNumberOfChannels; ++i) {
                m_has_SIGNAL[i] = (mConnection_CH_IN_SIGNAL_and_mix[i].connection != nullptr);
                mSignalInputCounter += m_has_SIGNAL[i];
            }
            if (is_not_updated() && pChannel == CH_OUT_SIGNAL && mSignalInputCounter > 0) {
                AUDIO_BLOCK_ID mBlock_SIGNAL[mNumberOfChannels];
                SIGNAL_TYPE*   mBlockData_SIGNAL[mNumberOfChannels];

                for (uint8_t i = 0; i < mNumberOfChannels; ++i) {
                    if (m_has_SIGNAL[i]) {
                        mBlock_SIGNAL[i] = AudioBlockPool::instance().request();
                        if (mBlock_SIGNAL[i] == AudioBlockPool::NO_ID) {
                            // @note(probably ran out of memory blocks @maybe(implement some better error handling))
                            memset(pAudioBlock, 0.0, KLANG_SAMPLES_PER_AUDIO_BLOCK * sizeof(SIGNAL_TYPE));
                            return;
                        }
                        mBlockData_SIGNAL[i] = AudioBlockPool::instance().data(mBlock_SIGNAL[i]);
                        mConnection_CH_IN_SIGNAL_and_mix[i].connection->update(mBlock_SIGNAL[i]);
                    }
                }

                const float mInverseSigCounter = 1.0 / mSignalInputCounter;
                for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; ++i) {
                    float sum = 0.0;
                    for (uint8_t j = 0; j < mNumberOfChannels; ++j) {
                        if (m_has_SIGNAL[j]) {
                            const float s = mBlockData_SIGNAL[j][i] * mConnection_CH_IN_SIGNAL_and_mix[j].mix;
                            sum += s;
                        }
                    }
                    pAudioBlock[i] = sum * mInverseSigCounter;
                }

                for (uint8_t i = 0; i < mNumberOfChannels; ++i) {
                    if (m_has_SIGNAL[i]) {
                        AudioBlockPool::instance().release(mBlock_SIGNAL[i]);
                    }
                }

                flag_updated();
            } else {
                memset(pAudioBlock, 0.0, KLANG_SAMPLES_PER_AUDIO_BLOCK * sizeof(SIGNAL_TYPE));
            }
        }

        void set_mix(uint8_t pChannel, float pValue) {
            mConnection_CH_IN_SIGNAL_and_mix[pChannel].mix = pValue;
        }

        SIGNAL_TYPE get_mix(uint8_t pChannel) {
            return mConnection_CH_IN_SIGNAL_and_mix[pChannel].mix;
        }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_MIX_F32:
                    set_mix(static_cast<uint8_t>(pPayLoad[0]), KlangMath::FLOAT_32(pPayLoad, 1));
                    break;
            }
        }

    private:
        struct MixConnectionStruct {
            Connection* connection;
            float       mix;
        };
        vector<MixConnectionStruct> mConnection_CH_IN_SIGNAL_and_mix;

        void add_channel(uint32_t pChannel, Connection* pConnection) {
            if (pChannel >= mConnection_CH_IN_SIGNAL_and_mix.size()) {
                mConnection_CH_IN_SIGNAL_and_mix.resize(pChannel + 1);
            }
            MixConnectionStruct mc;
            mc.connection                              = pConnection;
            mc.mix                                     = 1.0;
            mConnection_CH_IN_SIGNAL_and_mix[pChannel] = mc;
        }

        uint16_t get_number_of_channels() {
            return mConnection_CH_IN_SIGNAL_and_mix.size();
        }
    };
}  // namespace klang

#endif /* NodeMixerMulti_hpp */
