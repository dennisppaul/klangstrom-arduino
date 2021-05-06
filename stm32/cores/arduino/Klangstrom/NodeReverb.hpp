//
//  NodeReverb.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_REVERB         ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 *       |                     |
 *       |                     |
 *       |                     |
 *       |                     |
 *       +---------------------+
 */

#ifndef NodeReverb_hpp
#define NodeReverb_hpp

#include "Node.hpp"
#include "revmodel.h"

namespace klang {
    // @TODO(add second input for stereo reverb)
    /**
     *       [ NODE_REVERB         ]
     *       +---------------------+
     *       |                     |
     * IN00--| SIGNAL_R   SIGNAL_R |--OUT00
     * IN01--| SIGNAL_L   SIGNAL_L |--OUT01
     *       |                     |
     *       |                     |
     *       |                     |
     *       +---------------------+
     */
    
    class NodeReverb : public Node {
    public:
        static const CHANNEL_ID NUM_CH_IN         = 1;
        static const CHANNEL_ID NUM_CH_OUT        = 1;
        
        NodeReverb() {
            mFreeVerb.mute();
        }
        
        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL) {
                mInput_SIGNAL = pConnection;
                return true;
            }
            return false;
        }
        
        bool disconnect(CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL) {
                mInput_SIGNAL = nullptr;
                return true;
            }
            return false;
        }
        
        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            if (is_not_updated()) {
                if (mInput_SIGNAL != nullptr) {
                    mInput_SIGNAL->output_node.update(mInput_SIGNAL->output_channel_id, pAudioBlock);
                }
                flag_updated();
            }
            
            if (pChannel == CH_OUT_SIGNAL) {
                mFreeVerb.processreplace_mono(pAudioBlock, pAudioBlock, KLANG_SAMPLES_PER_AUDIO_BLOCK, 1);
            }
        }
        
        /* reverb adapter */
        void    mute() { mFreeVerb.mute(); }
        void    setroomsize(float value) { mFreeVerb.setroomsize(value); }  // default: 0.5
        float   getroomsize() { return mFreeVerb.getroomsize(); }
        void    setdamp(float value) { mFreeVerb.setdamp(value); }          // default: 0.5
        float   getdamp() { return mFreeVerb.getdamp(); }
        void    setwet(float value) { mFreeVerb.setwet(value); }            // default: 0.33
        float   getwet() { return mFreeVerb.getwet(); }
        void    setdry(float value) { mFreeVerb.setdry(value); }            // default: 0
        float   getdry() { return mFreeVerb.getdry(); }
        void    setwidth(float value) { mFreeVerb.setwidth(value); }        // default: 1
        float   getwidth() { return mFreeVerb.getwidth(); }
        void    setmode(float value) { mFreeVerb.setmode(value); }          // default: 0
        float   getmode() { return mFreeVerb.getmode(); }
        
        void set_command(const KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {}

    private:
        revmodel    mFreeVerb;
        Connection* mInput_SIGNAL   = nullptr;
    };
}

#endif /* NodeReverb_hpp */
