//
//  NodeAdd.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_ADD            ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 *       |                     |
 *       +---------------------+
 */

#ifndef NodeAdd_hpp
#define NodeAdd_hpp

#include "KlangMath.hpp"
#include "NodeKernel.hpp"

namespace klang {
    class NodeAdd : public NodeKernel {
    public:
        void set_value(SIGNAL_TYPE pValue) {
            mValue = pValue;
        }

        SIGNAL_TYPE get_value() {
            return mValue;
        }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_VALUE_F32:
                    set_value(KlangMath::FLOAT_32(pPayLoad));
                    break;
            }
        }

    protected:
        SIGNAL_TYPE kernel(const SIGNAL_TYPE s) {
            return s + mValue;
        }

    private:
        SIGNAL_TYPE mValue = 0.0;
    };
}  // namespace klang

#endif /* NodeAdd_hpp */
