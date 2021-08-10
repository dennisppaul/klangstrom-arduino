//
//  NodeSplitter.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_SPLITTER       ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUTFF
 *       |                     |
 *       +---------------------+
 */

#ifndef NodeSplitter_hpp
#define NodeSplitter_hpp

#include "NodeBuffer.hpp"

namespace klang {
    class NodeSplitter : public NodeBuffer {
    public:
        static const CHANNEL_ID NUM_CH_IN  = 1;
        static const CHANNEL_ID NUM_CH_OUT = 1;

        bool ouput_allow_multiple_connections() { return true; }
    };
}  // namespace klang

#endif /* NodeSplitter_hpp */
