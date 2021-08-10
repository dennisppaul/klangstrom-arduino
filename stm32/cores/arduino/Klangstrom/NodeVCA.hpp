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
 *       |                     |
 *       +---------------------+
 */

#ifndef NodeVCA_hpp
#define NodeVCA_hpp

#include "NodeVCA.hpp"

namespace klang {
    class NodeVCA : public NodeKernel {
    public:
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
        SIGNAL_TYPE kernel(const SIGNAL_TYPE s) {
            return s * mAmplification;
        }

    private:
        SIGNAL_TYPE mAmplification = 1.0;
    };
}  // namespace klang

#endif /* NodeVCA_hpp */
