//
//  Controller.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

#ifndef Controller_hpp
#define Controller_hpp

#include "Connection.hpp"
#include "ConnectionPool.hpp"
#include "KlangCommands.hpp"
#include "Node.hpp"

//#define ENABLE_RESET_CMD

namespace klang {
    class Controller {
    public:
        std::vector<Node*> nodes;

        // @TODO("add a sequential reset sequence ( e.g 6× `0x00` )")

        void command(KLANG_CMD_TYPE pData) {
            // @TODO (this might not necessary)
#ifdef ENABLE_RESET_CMD
            if (pData == KLANG_CMD_RESET_CMD) {
                mCommandResetCounter++;
                /* reset */
                if (mCommandResetCounter >= CMD_RESET_COUNTER) {
                    reset_cmd_collection();
                    return;
                }
            }
#endif
            /* -- AUTO GENERATED CODE (BEGIN) -- */
            if (mCommandCollectorCounter == NO_CMD) {
                mCommandPayloadID = 0;
                switch (pData) {
                    case KLANG_CMD_SYNTH_START:
                    case KLANG_CMD_RESERVED_01:
                    case KLANG_CMD_RESET_CMD:
                    case KLANG_CMD_RESET:
                    case KLANG_CMD_SYNTH_STOP:
                        mCommandCollectorCounter = 0;
                        break;
                    case KLANG_CMD_OUTPUT_NODE_I8:
                    case KLANG_CMD_CREATE_NODE_I8:
                    case KLANG_SET_START:
                    case KLANG_SET_STOP:
                    case KLANG_CMD_INPUT_NODE_I8:
                    case KLANG_CMD_DELETE_NODE_I8:
                    case KLANG_SET_CLEAR_STAGES:
                        mCommandCollectorCounter = 1;
                        break;
                    case KLANG_SET_TYPE_I8:
                    case KLANG_SET_MUTE_I8:
                    case KLANG_SET_WAVEFORM_I8:
                    case KLANG_SET_STEREO_I8:
                        mCommandCollectorCounter = 2;
                        break;
                    case KLANG_CMD_CONNECT_NODES_I8_I8_I8_I8:
                        mCommandCollectorCounter = 4;
                        break;
                    case KLANG_SET_CLIP_F32:
                    case KLANG_SET_RANGE_MAX_F32:
                    case KLANG_SET_DECAY_F32:
                    case KLANG_SET_RELEASE_F32:
                    case KLANG_SET_AMPLIFICATION_F32:
                    case KLANG_SET_SPEED_F32:
                    case KLANG_SET_ATTACK_F32:
                    case KLANG_SET_FEEDBACK_F32:
                    case KLANG_SET_FILTER_F32:
                    case KLANG_SET_RATE_F32:
                    case KLANG_SET_STRIDE_F32:
                    case KLANG_SET_MIN_F32:
                    case KLANG_SET_SWEEPRATE_F32:
                    case KLANG_SET_RESONANCE_F32:
                    case KLANG_SET_AMPLITUDE_F32:
                    case KLANG_SET_IN_MIN_F32:
                    case KLANG_SET_CUTOFF_F32:
                    case KLANG_SET_VALUE_F32:
                    case KLANG_SET_DELAY_F32:
                    case KLANG_SET_FREQUENCY_F32:
                    case KLANG_SET_OUT_MAX_F32:
                    case KLANG_SET_SUSTAIN_F32:
                    case KLANG_SET_SWEEP_F32:
                    case KLANG_SET_DURATION_F32:
                    case KLANG_SET_OUT_MIN_F32:
                    case KLANG_SET_IN_MAX_F32:
                    case KLANG_SET_MAX_F32:
                    case KLANG_SET_RANGE_MIN_F32:
                    case KLANG_SET_TIME_SCALE_F32:
                    case KLANG_SET_OFFSET_F32:
                    case KLANG_SET_WET_F32:
                    case KLANG_SET_MIX_F32:
                        mCommandCollectorCounter = 5;
                        break;
                    case KLANG_SET_PAN_I8_F32:
                    case KLANG_SET_MIX_I8_F32:
                        mCommandCollectorCounter = 6;
                        break;
                    case KLANG_SET_ADD_STAGE_F32_F32:
                        mCommandCollectorCounter = 9;
                        break;
                }
            }
            /* -- AUTO GENERATED CODE (END) -- */

            //            KLANG_LOG("data: 0x%02X (payload_counter: %i)", pData, mCommandCollectorCounter);
            mCommandPayloadCollector[mCommandPayloadID++] = pData;
            if (mCommandCollectorCounter <= 0) {
                //                KLANG_LOG("cmd_data");
                command_package(mCommandPayloadCollector);
                reset_cmd_collection();
            } else {
                mCommandCollectorCounter--;
            }
        }

        void commands(uint8_t* data, uint8_t length) {
            for (uint8_t i = 0; i < length; i++) {
                command(data[i]);
            }
        }

        /**
         *
         * @deprecated("use serial cmd collector only")
         */
        void command_sequence(std::initializer_list<KLANG_CMD_TYPE> pData) {
            for (KLANG_CMD_TYPE mDatum : pData) {
                command(mDatum);
            }
        }

        // @TODO("find a better way to parse the payload. maybe inscribe it in the constant name e.g `KLANG_SET_AMPLITUDE_1F32`, `KLANG_CMD_DELETE_NODE_1I8`, or `KLANG_CMD_CONNECT_NODES_4I8` ")
        void command_package(KLANG_CMD_TYPE* pData) {
            try {
                const KLANG_CMD_TYPE mCommand = pData[CMD_LOCATION];
                switch (mCommand) {
                    case KLANG_CMD_RESET_CMD:
                        reset_cmd_collection();
                        break;
                    case KLANG_CMD_RESET:
                        KLANG_LOG("@Controller :: KLANG_CMD_RESET");
                        cmd_reset();
                        break;
                    case KLANG_CMD_RESERVED_01:
                        break;
                    case KLANG_CMD_SYNTH_START:
                        KLANG_LOG("@Controller :: KLANG_CMD_SYNTH_START");
                        Klang::unlock();
                        break;
                    case KLANG_CMD_SYNTH_STOP:
                        KLANG_LOG("@Controller :: KLANG_CMD_SYNTH_STOP");
                        Klang::lock();
                        break;
                    case KLANG_CMD_CREATE_NODE_I8:
                        KLANG_LOG("@Controller :: KLANG_CMD_CREATE_NODE_I8");
                        cmd_create_node(pData[CMD_NODE_TYPE_LOCATION]);
                        break;
                    case KLANG_CMD_DELETE_NODE_I8:
                        KLANG_LOG("@Controller :: KLANG_CMD_DELETE_NODE_I8 @TODO");
                        cmd_delete_node(pData[CMD_NODE_TYPE_LOCATION]);
                        break;
                    case KLANG_CMD_CONNECT_NODES_I8_I8_I8_I8:
                        KLANG_LOG("@Controller :: KLANG_CMD_CONNECT_NODES_I8_I8_I8_I8");
                        cmd_connect_nodes(pData);
                        break;
                    case KLANG_CMD_OUTPUT_NODE_I8:
                        KLANG_LOG("@Controller :: KLANG_CMD_OUTPUT_NODE_I8");
                        cmd_output_node(pData);
                        break;
                    case KLANG_CMD_INPUT_NODE_I8:
                        KLANG_LOG("@Controller :: KLANG_CMD_INPUT_NODE_I8");
                        cmd_input_node(pData);
                        break;
                        // @TODO("automate the set branch")
                    case KLANG_SET_CLIP_F32:
                    case KLANG_SET_RANGE_MAX_F32:
                    case KLANG_SET_DECAY_F32:
                    case KLANG_SET_RELEASE_F32:
                    case KLANG_SET_AMPLIFICATION_F32:
                    case KLANG_SET_SPEED_F32:
                    case KLANG_SET_ATTACK_F32:
                    case KLANG_SET_FEEDBACK_F32:
                    case KLANG_SET_FILTER_F32:
                    case KLANG_SET_RATE_F32:
                    case KLANG_SET_STRIDE_F32:
                    case KLANG_SET_MIN_F32:
                    case KLANG_SET_SWEEPRATE_F32:
                    case KLANG_SET_RESONANCE_F32:
                    case KLANG_SET_AMPLITUDE_F32:
                    case KLANG_SET_IN_MIN_F32:
                    case KLANG_SET_CUTOFF_F32:
                    case KLANG_SET_VALUE_F32:
                    case KLANG_SET_DELAY_F32:
                    case KLANG_SET_FREQUENCY_F32:
                    case KLANG_SET_OUT_MAX_F32:
                    case KLANG_SET_SUSTAIN_F32:
                    case KLANG_SET_SWEEP_F32:
                    case KLANG_SET_DURATION_F32:
                    case KLANG_SET_OUT_MIN_F32:
                    case KLANG_SET_IN_MAX_F32:
                    case KLANG_SET_MAX_F32:
                    case KLANG_SET_RANGE_MIN_F32:
                    case KLANG_SET_TIME_SCALE_F32:
                    case KLANG_SET_OFFSET_F32:
                    case KLANG_SET_WET_F32:
                    case KLANG_SET_MIX_F32:
                        KLANG_LOG("@Controller :: KLANG_SET_*_F32");
                        cmd_set_value_f32(pData);
                        break;
                    case KLANG_SET_TYPE_I8:
                    case KLANG_SET_MUTE_I8:
                    case KLANG_SET_WAVEFORM_I8:
                    case KLANG_SET_STEREO_I8:
                        KLANG_LOG("@Controller :: KLANG_SET_*_I8");
                        cmd_set_value_i8(pData);
                        break;
                    case KLANG_SET_START:
                    case KLANG_SET_STOP:
                    case KLANG_SET_CLEAR_STAGES:
                    case KLANG_SET_PAN_I8_F32:
                    case KLANG_SET_MIX_I8_F32:
                    case KLANG_SET_ADD_STAGE_F32_F32:
                        KLANG_LOG("@Controller :: KLANG_SET_*_* @TODO(might crash)");
                        cmd_set_value(pData);
                        break;
                    default:
                        KLANG_LOG_ERR("@Controller :: could not parse command (0x%02X)", pData[CMD_LOCATION]);
                }
            } catch (const std::exception& e) {
                KLANG_LOG_ERR("@Controller :: command failed with exception: %s", e.what());
            }
        }

        void process_frame_output(SIGNAL_TYPE* pLeft, SIGNAL_TYPE* pRight) {
            if (!Klang::islocked() && mOutputNode != nullptr) {
                mOutputNode->process_frame(pLeft, pRight);
            } else {
                for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    pLeft[i]  = 0.0;
                    pRight[i] = 0.0;
                }
            }
        }

        void process_frame_input(SIGNAL_TYPE* pLeft, SIGNAL_TYPE* pRight) {
            //            KLANG_LOG("Klang::islocked(): %i", Klang::islocked());
            if (!Klang::islocked() && mInputNode != nullptr) {
                mInputNode->process_frame(pLeft, pRight);
            }
        }

    private:
        static const KLANG_CMD_TYPE CMD_LOCATION                = 0x00;
        static const KLANG_CMD_TYPE CMD_CONNECT_DATA_LOCATION   = 0x01;
        static const KLANG_CMD_TYPE CMD_OUTPUT_NODE_LOCATION    = 0x01;
        static const KLANG_CMD_TYPE CMD_SET_NODE_ID_LOCATION    = 0x01;
        static const KLANG_CMD_TYPE CMD_SET_NODE_VALUE_LOCATION = 0x02;
        static const KLANG_CMD_TYPE CMD_NODE_TYPE_LOCATION      = 0x01;

        static const int8_t  NO_CMD                                           = -1;
        int8_t               mCommandCollectorCounter                         = NO_CMD;
        uint8_t              mCommandPayloadID                                = 0;
        uint8_t              mCommandResetCounter                             = 0;
        static const uint8_t CMD_RESET_COUNTER                                = 5;
        static const uint8_t PAYLOAD_COLLECTOR_SIZE                           = 8;
        KLANG_CMD_TYPE       mCommandPayloadCollector[PAYLOAD_COLLECTOR_SIZE] = {0};

        uint8_t  mOutputNodeID = 255;
        uint8_t  mInputNodeID  = 255;
        NodeDAC* mOutputNode   = nullptr;
        NodeADC* mInputNode    = nullptr;

        void reset_payload_collector() {
            for (uint8_t i = 0; i < PAYLOAD_COLLECTOR_SIZE; i++) {
                mCommandPayloadCollector[i] = 0;
            }
        }

        void reset_cmd_collection() {
            mCommandPayloadID        = 0;
            mCommandResetCounter     = 0;
            mCommandCollectorCounter = NO_CMD;
            reset_payload_collector();
        }

        void cmd_set_value_i8(KLANG_CMD_TYPE* pData) {
            const uint8_t mID = pData[CMD_SET_NODE_ID_LOCATION];
            if (mID >= nodes.size()) {
                return;
            }
            Node* mNode = nodes[mID];
            if (mNode == nullptr) {
                return;
            }
            uint8_t pPayload[] = {
                pData[CMD_SET_NODE_ID_LOCATION + 1]};
            KLANG_LOG("@Controller :: NODE_%02i > set_value_i8 (0x%02X) > %i ", mID, pData[CMD_LOCATION], pPayload[0]);
            mNode->set_command(pData[CMD_LOCATION], pPayload);
        }

        void cmd_set_value_f32(KLANG_CMD_TYPE* pData) {
            const uint8_t mID = pData[CMD_SET_NODE_ID_LOCATION];
            if (mID >= nodes.size()) {
                return;
            }
            Node* mNode = nodes[mID];
            if (mNode == nullptr) {
                return;
            }
            uint8_t pPayload[] = {
                pData[CMD_SET_NODE_VALUE_LOCATION + 0],
                pData[CMD_SET_NODE_VALUE_LOCATION + 1],
                pData[CMD_SET_NODE_VALUE_LOCATION + 2],
                pData[CMD_SET_NODE_VALUE_LOCATION + 3]};
            KLANG_LOG("@Controller :: NODE_%02i > set_value_f32 (0x%02X) > %f ", mID, pData[CMD_LOCATION], KlangMath::FLOAT_32(pPayload));
            mNode->set_command(pData[CMD_LOCATION], pPayload);
        }

        void cmd_set_value(KLANG_CMD_TYPE* pData) {
            // @TODO("`cmd_set_value` could be unified")
            const uint8_t mID = pData[CMD_SET_NODE_ID_LOCATION];
            if (mID >= nodes.size()) {
                return;
            }
            Node* mNode = nodes[mID];
            if (mNode == nullptr) {
                return;
            }
            uint8_t pPayload[] = {
                pData[CMD_SET_NODE_VALUE_LOCATION + 0],
                pData[CMD_SET_NODE_VALUE_LOCATION + 1],
                pData[CMD_SET_NODE_VALUE_LOCATION + 2],
                pData[CMD_SET_NODE_VALUE_LOCATION + 3],
                pData[CMD_SET_NODE_VALUE_LOCATION + 4],
                pData[CMD_SET_NODE_VALUE_LOCATION + 5],
                pData[CMD_SET_NODE_VALUE_LOCATION + 6],
                pData[CMD_SET_NODE_VALUE_LOCATION + 7]};
            KLANG_LOG("@Controller :: NODE_%02i > set_value_* (0x%02X)", mID, pData[CMD_LOCATION]);
            mNode->set_command(pData[CMD_LOCATION], pPayload);
        }

        void cmd_connect_nodes(KLANG_CMD_TYPE* pData) {
            KLANG_LOG("@Controller :: connecting NODE_%02i(OUT%02i) with NODE_%02i(IN%02i)",
                      pData[CMD_CONNECT_DATA_LOCATION + 0],
                      pData[CMD_CONNECT_DATA_LOCATION + 1],
                      pData[CMD_CONNECT_DATA_LOCATION + 2],
                      pData[CMD_CONNECT_DATA_LOCATION + 3]);
            Klang::connect(*nodes[pData[CMD_CONNECT_DATA_LOCATION + 0]],
                           pData[CMD_CONNECT_DATA_LOCATION + 1],
                           *nodes[pData[CMD_CONNECT_DATA_LOCATION + 2]],
                           pData[CMD_CONNECT_DATA_LOCATION + 3]);
        }

        void cmd_output_node(KLANG_CMD_TYPE* pData) {
            mOutputNodeID = pData[CMD_OUTPUT_NODE_LOCATION];
            KLANG_LOG("@Controller :: set NODE_%02i as output_node", mOutputNodeID);
            if (mOutputNodeID < nodes.size() &&
                nodes[mOutputNodeID] != nullptr) {
                mOutputNode = dynamic_cast<NodeDAC*>(nodes[mOutputNodeID]);
                if (!mOutputNode) {
                    KLANG_LOG("@Controller :: setting output node failed! node id or node is not valid ( make sure node is of type `NodeDAC` )");
                }
            }
        }

        void cmd_input_node(KLANG_CMD_TYPE* pData) {
            mInputNodeID = pData[CMD_OUTPUT_NODE_LOCATION];
            KLANG_LOG("@Controller :: set NODE_%02i as input_node", mInputNodeID);
            if (mInputNodeID < nodes.size() &&
                nodes[mInputNodeID] != nullptr) {
                mInputNode = dynamic_cast<NodeADC*>(nodes[mInputNodeID]);
                if (!mInputNode) {
                    KLANG_LOG("@Controller :: setting input node failed! node id or node is not valid ( make sure node is of type `NodeADC` )");
                }
            }
        }

        void cmd_delete_node(const uint8_t pNodeID) {
            // @TODO("this corrupts all subsequent IDs")
            KLANG_LOG("@Controller :: delete NODE_%02lu", nodes.size());
            KLANG_LOG("@TODO(\"this corrupts all subsequent IDs\")");
            nodes.erase(nodes.begin() + pNodeID);
        }

        void cmd_reset() {
            // @TODO("delete all nodes and connections, reset synth, reset controller")
            reset_cmd_collection();
            Klang::instance().reset_frame_index();
            ConnectionPool::instance().reset();
            reset();
        }

        void reset() {
            mInputNode  = nullptr;
            mOutputNode = nullptr;
            for (uint16_t i = 0; i < nodes.size(); i++) {
                delete nodes[i];
            }
            nodes.clear();
            Node::reset();
        }

        /* -- AUTO GENERATED CODE (BEGIN) -- */
        void cmd_create_node(const KLANG_CMD_TYPE pNodeType) {
            switch (pNodeType) {
                case KLANG_NODE_ADC:
                    nodes.push_back(new NodeADC());
                    break;
                case KLANG_NODE_ADD:
                    nodes.push_back(new NodeAdd());
                    break;
                case KLANG_NODE_ADSR:
                    nodes.push_back(new NodeADSR());
                    break;
                case KLANG_NODE_ALIEN_WAH:
                    nodes.push_back(new NodeAlienWah());
                    break;
                case KLANG_NODE_BUFFER:
                    nodes.push_back(new NodeBuffer());
                    break;
                case KLANG_NODE_CHORUS:
                    nodes.push_back(new NodeChorus());
                    break;
                case KLANG_NODE_CLIP:
                    nodes.push_back(new NodeClip());
                    break;
                case KLANG_NODE_DAC:
                    nodes.push_back(new NodeDAC());
                    break;
                case KLANG_NODE_DELAY:
                    nodes.push_back(new NodeDelay());
                    break;
                case KLANG_NODE_DISTORTION:
                    nodes.push_back(new NodeDistortion());
                    break;
                case KLANG_NODE_ENVELOPE:
                    nodes.push_back(new NodeEnvelope());
                    break;
                case KLANG_NODE_MAP:
                    nodes.push_back(new NodeMap());
                    break;
                case KLANG_NODE_MIXER2:
                    nodes.push_back(new NodeMixer2());
                    break;
                case KLANG_NODE_MIXER4:
                    nodes.push_back(new NodeMixer4());
                    break;
                case KLANG_NODE_MIXER4_STEREO:
                    nodes.push_back(new NodeMixer4Stereo());
                    break;
                case KLANG_NODE_MUTE:
                    nodes.push_back(new NodeMute());
                    break;
                case KLANG_NODE_NOISE:
                    nodes.push_back(new NodeNoise());
                    break;
                case KLANG_NODE_PASSTHROUGH:
                    nodes.push_back(new NodePassthrough());
                    break;
                case KLANG_NODE_PATCH:
                    nodes.push_back(new NodePatch());
                    break;
                case KLANG_NODE_PATCH16:
                    nodes.push_back(new NodePatch16());
                    break;
                case KLANG_NODE_PHASER:
                    nodes.push_back(new NodePhaser());
                    break;
                case KLANG_NODE_PORTAMENTO:
                    nodes.push_back(new NodePortamento());
                    break;
                case KLANG_NODE_SAMPLER:
                    nodes.push_back(new NodeSampler());
                    break;
                case KLANG_NODE_SPLITTER:
                    nodes.push_back(new NodeSplitter());
                    break;
                case KLANG_NODE_VALUE:
                    nodes.push_back(new NodeValue());
                    break;
                case KLANG_NODE_VCA:
                    nodes.push_back(new NodeVCA());
                    break;
                case KLANG_NODE_VCF_LP:
                    nodes.push_back(new NodeVCFLowPass());
                    break;
                case KLANG_NODE_VCF_MOOG_LHBP:
                    nodes.push_back(new NodeVCFMoogLHBP());
                    break;
                case KLANG_NODE_VCF_MOOG_LP:
                    nodes.push_back(new NodeVCFMoogLP());
                    break;
                case KLANG_NODE_VCO_FUNC:
                    nodes.push_back(new NodeVCOFunction());
                    break;
                case KLANG_NODE_VCO_WAVETABLE:
                    nodes.push_back(new NodeVCOWavetable());
                    break;
            }
        }
        /* -- AUTO GENERATED CODE (END) -- */
    };
}  // namespace klang

/*
 * a set of exemplary commands that create and connect a sinewave oscillator to a DAC:
 *
 * | COMMANDS                                | HEX REPRESENTATION                     | EXPLANATION                                                             |
 * |-----------------------------------------|----------------------------------------|-------------------------------------------------------------------------|
 * | KLANG_CMD_CREATE_NODE, KLANG_NODE_DAC       | ( 0x05, 0x03 )                         | create NODE_DAC with ID(0)                                              |
 * | KLANG_CMD_CREATE_NODE, KLANG_NODE_WAVETABLE | ( 0x05, 0x04 )                         | create NODE_WAVETABLE with ID(1)                                        |
 * | KLANG_CMD_CONNECT_NODES, 1, 0, 0, 0       | ( 0x07, 0x01, 0x00, 0x00, 0x00 )       | connect OUPUT(0) of NODE_WAVETABLE ID(1) to INPUT(1)) of NODE_DAC ID(0) |
 * | KLANG_SET_WAVEFORM, 1, 1                  | ( 0x83, 0x01, 0x01 )                   | set WAVEFORM of NODE_WAVETABLE ID(1) to SINE                            |
 * | KLANG_SET_FREQUENCY_F32, 1, 110.0         | ( 0x81, 0x01, 0x00, 0x00, 0xDC, 0x36 ) | set FREQUENCY of NODE_WAVETABLE ID(1) to 110.0                          |
 *
 */

#endif /* Controller_hpp */
