//
//  NodeVCO.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_VCO            ]
 *       +---------------------+
 *       |                     |
 * IN00--| FREQ         SIGNAL |--OUT00
 * IN01--| AMP                 |
 *       |                     |
 *       +---------------------+
 */

#ifndef NodeVCO_hpp
#define NodeVCO_hpp

#include "Node.hpp"

namespace klang {
    class NodeVCO : public Node {
    public:
        static const CHANNEL_ID CH_IN_FREQ  = 0;
        static const CHANNEL_ID CH_IN_AMP   = 1;

        virtual void set_frequency(SIGNAL_TYPE pFrequency) = 0;
        virtual SIGNAL_TYPE get_frequency() = 0;
        virtual void set_amplitude(SIGNAL_TYPE pAmplitude) = 0;
        virtual SIGNAL_TYPE get_amplitude() = 0;
    };
}

#endif /* NodeVCO_hpp */
