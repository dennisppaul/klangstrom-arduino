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
 *       [ NODE_KERNEL_BLOCK_MULTI  ]
 *       +--------------------------+
 *       |                          |
 * IN00--| SIGNAL_00      SIGNAL_00 |--OUT00
 *   ... |                          | ...
 * INFF--| SIGNAL_FF      SIGNAL_FF |--OUTFF
 *       |                          |
 *       +--------------------------+
 *
 *       @description(
 *          abstract node that allows the easy creation of new signal processing nodes by
 *          overriding the `kernel` method. this kernel node allows to process mulitple input
 *          and output signals.
 *       )
 */

#ifndef NodeKernelBlockMulti_hpp
#define NodeKernelBlockMulti_hpp

#include <vector>

#include "KlangNode.hpp"

using namespace std;

namespace klang {
    class NodeKernelBlockMulti : public Node {
    public:
        static const CHANNEL_ID NUM_CH_IN  = 0xFF;
        static const CHANNEL_ID NUM_CH_OUT = 0xFF;

        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            add_channel(pInChannel, pConnection);
            return true;
        }

        bool disconnect(CHANNEL_ID pInChannel) {
            if (pInChannel < get_num_input_channels()) {
                mInputConnections[pInChannel].connection = nullptr;
                return true;
            }
            return false;
        }

        void set_num_output_channels(uint8_t pNumOutputChannels) {
            fNumOutputChannels = pNumOutputChannels;
        }

        uint8_t get_num_output_channels() {
            return fNumOutputChannels;
        }

        uint8_t get_num_input_channels() {
            return mInputConnections.size();
        }

        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            const uint8_t mNumInputChannels = get_num_input_channels();
            bool          m_has_SIGNAL[mNumInputChannels];
            for (uint8_t i = 0; i < mNumInputChannels; ++i) {
                m_has_SIGNAL[i] = (mInputConnections[i].connection != nullptr);
            }
            if (is_not_updated()) {
                vector<SIGNAL_TYPE*> mInputSignals(mNumInputChannels);
                for (uint8_t i = 0; i < mNumInputChannels; ++i) {
                    if (m_has_SIGNAL[i]) {
                        mInputConnections[i].id = AudioBlockPool::instance().request();
                        if (mInputConnections[i].id == AudioBlockPool::NO_ID) {
                            // @note(probably ran out of memory blocks @maybe(implement some better error handling))
                            KLANG_FILL_AUDIO_BUFFER(pAudioBlock, 0.0);
                            return;
                        }
                        mInputConnections[i].data = AudioBlockPool::instance().data(mInputConnections[i].id);
                        mInputConnections[i].connection->update(mInputConnections[i].data);
                        mInputSignals[i] = mInputConnections[i].data;
                    } else {
                        mInputSignals[i] = nullptr;
                    }
                }

                const uint8_t mNumOutputChannels = get_num_output_channels();
                mOutputSignals.resize(mNumOutputChannels); // @todo maybe move this to `get_num_output_channels()`
                for (uint8_t i = 0; i < mNumOutputChannels; ++i) {
                    AUDIO_BLOCK_ID mBlockID = AudioBlockPool::instance().request();
                    mOutputSignals[i]       = AudioBlockPool::instance().data(mBlockID);
                }

                kernel(mOutputSignals, mInputSignals);

                for (uint8_t i = 0; i < mNumInputChannels; ++i) {
                    if (m_has_SIGNAL[i]) {
                        AudioBlockPool::instance().release(mInputConnections[i].id);
                    }
                }

                flag_updated();
            }

            if (pChannel < get_num_output_channels()) {
                if (mOutputSignals[pChannel] != nullptr) {
                    KLANG_COPY_AUDIO_BUFFER(pAudioBlock, mOutputSignals[pChannel]);
                } else {
                    KLANG_FILL_AUDIO_BUFFER(pAudioBlock, 0.0);
                }
            }
        }

        virtual void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad){};

    protected:
        /* override kernel method to implement custom kernels. */
        virtual void kernel(vector<SIGNAL_TYPE*>& pOutputSignals, vector<SIGNAL_TYPE*>& pInputSignals) = 0;

    private:
        typedef struct KernelConnectionStruct {
            Connection*    connection = nullptr;
            AUDIO_BLOCK_ID id         = AudioBlockPool::NO_ID;
            SIGNAL_TYPE*   data       = nullptr;
        } KernelConnection;

        vector<KernelConnection> mInputConnections;
        uint8_t                  fNumOutputChannels = 1;
        vector<SIGNAL_TYPE*>     mOutputSignals;

        void add_channel(uint32_t pChannel, Connection* pConnection) {
            if (pChannel >= mInputConnections.size()) {
                mInputConnections.resize(pChannel + 1);
            }
            // KernelConnection c;
            // c.connection                = pConnection;
            // mInputConnections[pChannel] = c;
            mInputConnections[pChannel].connection = pConnection;
        }
    };
}  // namespace klang

#endif /* NodeKernelBlockMulti_hpp */
