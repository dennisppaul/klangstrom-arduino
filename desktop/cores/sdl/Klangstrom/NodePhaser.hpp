//
//  NodePhaser.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

/**
 *       [ NODE_PHASER         ]
 *       +---------------------+
 *       |                     |
 * IN00--| SIGNAL       SIGNAL |--OUT00
 * IN01--| FEEDBACK            |
 * IN02--| WET                 |
 * IN03--| SWEEPRATE           |
 * IN04--| RANGE_MIN           |
 * IN05--| RANGE_MAX           |
 *       |                     |
 *       +---------------------+
 */
// @TODO(implement `FEEDBACK` + `WET` + `SWEEPRATE` + `RANGE_MIN` + `RANGE_MAX`)

#ifndef NodePhaser_hpp
#define NodePhaser_hpp

#include "LUTSine.hpp"
#include "KlangNode.hpp"

namespace klang {
    //    static const uint16_t WAVETABLE_LENGTH = 2048;
    //    SIGNAL_TYPE oTableSine[WAVETABLE_LENGTH+1]; // @TODO(should be one wavetable ( aka `static` ) for all phasersaudio)

    class NodePhaser : public Node {
    public:
        static const CHANNEL_ID NUM_CH_IN = 1;
        //        static const CHANNEL_ID CH_IN_FEEDBACK    = 1;
        //        static const CHANNEL_ID CH_IN_WET         = 2;
        //        static const CHANNEL_ID CH_IN_SWEEPRATE   = 3;
        //        static const CHANNEL_ID CH_IN_RANGE_MIN   = 4;
        //        static const CHANNEL_ID CH_IN_RANGE_MAX   = 5;
        //        static const CHANNEL_ID NUM_CH_IN         = 6;
        static const CHANNEL_ID NUM_CH_OUT = 1;

        NodePhaser() {
            set_sweeprate(0.1);
            set_feedback(0.7);
            set_wet(0.5);
            set_range_min(200);
            set_range_max(1700);
            //            if (!oWavetableFilled) {
            //            NodeWavetable::fill_wavetable(NodeWavetable::WAVEFORM::SINE, oTableSine, WAVETABLE_LENGTH+1); // @TODO(check the `+1`)
            //                oWavetableFilled = true;
            //            }
        }

        bool connect(Connection* pConnection, CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL) {
                mConnection_CH_IN_SIGNAL = pConnection;
                return true;
            }
            return false;
        }

        bool disconnect(CHANNEL_ID pInChannel) {
            if (pInChannel == CH_IN_SIGNAL) {
                mConnection_CH_IN_SIGNAL = nullptr;
                return true;
            }
            return false;
        }

        void update(CHANNEL_ID pChannel, SIGNAL_TYPE* pAudioBlock) {
            if (is_not_updated()) {
                if (mConnection_CH_IN_SIGNAL != nullptr) {
                    mConnection_CH_IN_SIGNAL->update(pAudioBlock);
                }
                flag_updated();
            }
            if (pChannel == CH_OUT_SIGNAL) {
                for (uint16_t i = 0; i < KLANG_SAMPLES_PER_AUDIO_BLOCK; i++) {
                    pAudioBlock[i] = kernel(pAudioBlock[i]);
                }
            }
        }

        void set_wet(float pWet) {
            mWet = pWet;
        }

        void set_sweeprate(float pRate) {
            mSweepRate   = pRate;
            mLFOIncrease = TWO_PI * mSweepRate / KLANG_AUDIO_RATE_UINT16;
        }

        void set_feedback(float pFeedback) {
            mFeedback = KlangMath::clamp(pFeedback, 0.0, 0.99);
        }

        void set_range_min(float pRangeMin) {
            f_min = pRangeMin;
            dmin  = 2 * f_min / KLANG_AUDIO_RATE_UINT16;
        }

        void set_range_max(float pRangeMax) {
            f_max = pRangeMax;
            dmax  = 2 * f_max / KLANG_AUDIO_RATE_UINT16;
        }

        float get_sweeprate() { return mSweepRate; }
        float get_wet() { return mWet; }
        float get_feedback() { return mFeedback; }
        float get_range_min() { return f_min; }
        float get_range_max() { return f_max; }

        void set_command(KLANG_CMD_TYPE pCommand, KLANG_CMD_TYPE* pPayLoad) {
            switch (pCommand) {
                case KLANG_SET_FEEDBACK_F32:
                    set_feedback(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_WET_F32:
                    set_wet(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_SWEEPRATE_F32:
                    set_sweeprate(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_RANGE_MIN_F32:
                    set_range_min(KlangMath::FLOAT_32(pPayLoad));
                    break;
                case KLANG_SET_RANGE_MAX_F32:
                    set_range_max(KlangMath::FLOAT_32(pPayLoad));
                    break;
            }
        }

    private:
        Connection* mConnection_CH_IN_SIGNAL = nullptr;
        //        Connection* mConnection_CH_IN_FEEDBACK;
        //        Connection* mConnection_CH_IN_WET;
        //        Connection* mConnection_CH_IN_SWEEPRATE;
        //        Connection* mConnection_CH_IN_RANGE_MIN;
        //        Connection* mConnection_CH_IN_RANGE_MAX;

        //        constexpr static const float ALPHA      = (WAVETABLE_LENGTH/TWO_PI);

        static const uint8_t FILTER_STAGES = 6;

        float mOld[FILTER_STAGES];
        float mSweepRate;
        float mWet;
        float mFeedback;
        float mLFOPhase;
        float mLFOIncrease;
        float f_min, f_max;
        float dmin, dmax;
        float a1;
        float zm1;

        //        bool oWavetableFilled = false;
        //
        //        SIGNAL_TYPE sin_fast(float pRad) {
        //            const uint16_t mIndex = ((uint32_t)(WAVETABLE_LENGTH * pRad / TWO_PI)) % WAVETABLE_LENGTH;
        //            return oTableSine[mIndex];
        //        }
        //
        //        SIGNAL_TYPE cos_fast(float pRad) {
        //            return sin_fast(pRad + HALF_PI);
        //        }

        float allpass(float yin, int ind) {
            float yout;
            yout      = -yin * a1 + mOld[ind];
            mOld[ind] = yout * a1 + yin;
            return yout;
        }

        float kernel(float pInput) {
            float yout;
            int   i;
            float d;

            // calculate and update phaser sweep lfo...
            d = dmin + (dmax - dmin) * ((LUTSine::WAVETABLE[lrintf(LUTSine::ALPHA * mLFOPhase)] + 1.0) * 0.5);

            mLFOPhase += mLFOIncrease;
            if (mLFOPhase >= TWO_PI) {
                mLFOPhase -= TWO_PI;
            }

            // update filter coeffs
            a1 = (1.f - d) / (1.f + d);

            // calculate output
            yout = allpass(pInput + zm1 * mFeedback, 0);

            for (i = 1; i < FILTER_STAGES; i++) {
                yout = allpass(yout, i);
            }
            zm1 = yout;

            yout = (1 - mWet) * pInput + mWet * yout;

            return yout;
        }
    };
}  // namespace klang

#endif /* NodePhaser_hpp */
