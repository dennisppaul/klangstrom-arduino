//
//  Node.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

#ifndef Node_hpp
#define Node_hpp

#include "AudioBlockPool.hpp"
#include "Connection.hpp"
#include "Klang.hpp"
#include "KlangCommands.hpp"

namespace klang {
    class Connection;
    class Communincation;
    class Node {
    public:
        static const CHANNEL_ID CH_IN_SIGNAL  = 0;
        static const CHANNEL_ID CH_OUT_SIGNAL = 0;

        Node();
        virtual ~Node() {}

        NODE_ID ID() {
            return mID;
        }

        bool is_not_updated() {
            return Klang::instance().frame_index() != mFrame;
        }

        void flag_updated() {
            mFrame = Klang::instance().frame_index();
        }

        // virtual CHANNEL_ID get_num_input_channels() = 0;
        // virtual CHANNEL_ID get_num_output_channels() = 0;
        virtual void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock)          = 0;
        virtual bool connect(Connection* pConnection, CHANNEL_ID pInChannel)        = 0;
        virtual bool disconnect(CHANNEL_ID pInChannel)                              = 0;
        virtual void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) = 0;
        virtual bool ouput_allow_multiple_connections() { return false; }

        static void reset();

    private:
        FRAME_TYPE     mFrame = -1;
        NODE_ID        mID;
        static NODE_ID oID;
    };
}  // namespace klang

#endif /* Node_hpp */
