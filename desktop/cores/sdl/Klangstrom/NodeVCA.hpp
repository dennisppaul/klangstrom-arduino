//
//  NodeVCA.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_VCA            ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 * IN01--| AMPLIFICATION       |
 *       +---------------------+
 */

#ifndef NodeVCA_hpp
#define NodeVCA_hpp

#include "NodeVCA.hpp"

namespace klang {
    class NodeVCA : public NodeKernelBlock2 {
    public:
        static const CHANNEL_ID CH_IN_SIGNAL        = 0;
        static const CHANNEL_ID CH_IN_AMPLIFICATION = 1;

        void set_amplification(SIGNAL_TYPE pValue) {
            mAmplification = pValue;
        }

        SIGNAL_TYPE get_amplification() {
            return mAmplification;
        }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_AMPLIFICATION_F32:
                    set_amplification(KlangMath::FLOAT_32(pPayLoad));
                    break;
            }
        }

    protected:
        void kernel(SIGNAL_TYPE* pOutputSignal,
                    SIGNAL_TYPE* pInputSignal_A,
                    SIGNAL_TYPE* pInputSignal_B) {
            if (pInputSignal_A == nullptr) {
                for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    pOutputSignal[i] = 0.0;
                }
            } else if (pInputSignal_B == nullptr) {
                for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    pOutputSignal[i] = pInputSignal_A[i] * mAmplification;
                }
            } else {
                for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    pOutputSignal[i] = pInputSignal_A[i] * pInputSignal_B[i];
                }
            }
        }

    private:
        SIGNAL_TYPE mAmplification = 1.0;
    };
}  // namespace klang

#endif /* NodeVCA_hpp */
