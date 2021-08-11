//
//  NodeMute.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_MUTE           ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 *       |                     |
 *       +---------------------+
 *
 *       @description(
 *          allows to mute an entire branch of nodes connected to this node’s input.
 *          note that muting a branch will also result into halting the processing
 *          of all connected note ( e.g envelopes not being advanced ). optionally
 *          the incoming audio block can be filled with zeros.
 *       )
 */

#ifndef NodeMute_hpp
#define NodeMute_hpp

#include "KlangNode.hpp"

namespace klang {
    class NodeMute : public Node {
    public:
        static const CHANNEL_ID NUM_CH_IN  = 1;
        static const CHANNEL_ID NUM_CH_OUT = 1;

        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL) {
                mConnection_CH_IN_SIGNAL = pConnection;
                return true;
            }
            return false;
        }

        // @TODO(maybe just use a single method to either MUTE, MUTE_WITH_ZEROS, UNMUTE )
        void set_mute(bool pIsMuted) {
            mIsMuted = pIsMuted;
        }

        void fill_muted_audioblock_with_zeros(bool pFillMutedAudioblockWithZeros) {
            mFillMutedAudioblockWithZeros = pFillMutedAudioblockWithZeros;
        }

        bool disconnect(CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL) {
                mConnection_CH_IN_SIGNAL = nullptr;
                return true;
            }
            return false;
        }

        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            if (is_not_updated()) {
                if (mConnection_CH_IN_SIGNAL != nullptr) {
                    if (!mIsMuted) {
                        mConnection_CH_IN_SIGNAL->update(pAudioBlock);
                    }
                }
                flag_updated();
            }
            if (pChannel == CH_OUT_SIGNAL) {
                if (mIsMuted && mFillMutedAudioblockWithZeros) {
                    for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                        if (mIsMuted) {
                            pAudioBlock[i] = 0.0;
                        }
                    }
                }
            }
        }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_MUTE_I8:
                    set_mute(pPayLoad[0]);
                    break;
            }
        }

    private:
        bool        mIsMuted                      = true;
        bool        mFillMutedAudioblockWithZeros = true;
        Connection* mConnection_CH_IN_SIGNAL      = nullptr;
    };
}  // namespace klang

#endif /* NodeMute_hpp */
