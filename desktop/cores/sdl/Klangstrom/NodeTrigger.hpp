//
//  NodeTrigger.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_TRIGGER        ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL              |
 *       |                     |
 *       +---------------------+
 */

#ifndef NodeTrigger_hpp
#define NodeTrigger_hpp

#include "KlangEventDistributor.hpp"
#include "KlangNode.hpp"

namespace klang {
    class NodeTrigger : public NodeKernel, public KlangEventDistributor {
    public:
        static constexpr float FALLING_EDGE = -1.0;
        static constexpr float RISING_EDGE  = 1.0;

        SIGNAL_TYPE kernel(SIGNAL_TYPE s) {
            if (mPreviousSample > mThreshold && s < mThreshold) {
                fire_event(KLANG_EVENT::TRIGGER, std::vector<float>{FALLING_EDGE});
            } else if (mPreviousSample < mThreshold && s > mThreshold) {
                fire_event(KLANG_EVENT::TRIGGER, std::vector<float>{RISING_EDGE});
            }
            mPreviousSample = s;
            return s;
        }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {}

    private:
        Connection*            mConnection_CH_IN = nullptr;
        SIGNAL_TYPE            mPreviousSample   = 0.0;
        static constexpr float mThreshold        = 0.0;
    };
}  // namespace klang

#endif /* NodeTrigger_hpp */
