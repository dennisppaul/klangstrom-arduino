//
//  Scales.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

#ifndef Scales_hpp
#define Scales_hpp

#include <vector>
#include "Klang.hpp"

namespace klang {
    
typedef uint8_t SCALE_TYPE;
    class Scales {
    public:
        static const std::vector<SCALE_TYPE>    CHROMATIC;
        static const std::vector<SCALE_TYPE>    FIFTH;
        static const std::vector<SCALE_TYPE>    MINOR;
        static const std::vector<SCALE_TYPE>    MAJOR;
        static const std::vector<SCALE_TYPE>    MINOR_CHORD;
        static const std::vector<SCALE_TYPE>    MAJOR_CHORD;
        static const std::vector<SCALE_TYPE>    MINOR_CHORD_7;
        static const std::vector<SCALE_TYPE>    MAJOR_CHORD_7;
        static const std::vector<SCALE_TYPE>    MINOR_PENTATONIC;
        static const std::vector<SCALE_TYPE>    MAJOR_PENTATONIC;
        static const std::vector<SCALE_TYPE>    OCTAVE;
        static const std::vector<SCALE_TYPE>    DIMINISHED;
        static const std::vector<const std::vector<SCALE_TYPE>> SCALES;

        static const uint8_t               ID_CHROMATIC         = 0;
        static const uint8_t               ID_FIFTH             = 1;
        static const uint8_t               ID_MINOR             = 2;
        static const uint8_t               ID_MAJOR             = 3;
        static const uint8_t               ID_MINOR_CHORD       = 4;
        static const uint8_t               ID_MAJOR_CHORD       = 5;
        static const uint8_t               ID_MINOR_CHORD_7     = 6;
        static const uint8_t               ID_MAJOR_CHORD_7     = 7;
        static const uint8_t               ID_MINOR_PENTATONIC  = 8;
        static const uint8_t               ID_MAJOR_PENTATONIC  = 9;
        static const uint8_t               ID_OCTAVE            = 10;
        static const uint8_t               ID_DIMINISHED        = 11;
        static const uint8_t               NUM_OF_SCALES        = 12;
        
        static const std::vector<SCALE_TYPE> get(const uint8_t pScaleID) {
            switch (pScaleID) {
                case ID_CHROMATIC:
                    return CHROMATIC;
                case ID_FIFTH:
                    return FIFTH;
                case ID_MINOR:
                    return MINOR;
                case ID_MAJOR:
                    return MAJOR;
                case ID_MINOR_CHORD:
                    return MINOR_CHORD;
                case ID_MAJOR_CHORD:
                    return MAJOR_CHORD;
                case ID_MINOR_CHORD_7:
                    return MINOR_CHORD_7;
                case ID_MAJOR_CHORD_7:
                    return MAJOR_CHORD_7;
                case ID_MINOR_PENTATONIC:
                    return MINOR_PENTATONIC;
                case ID_MAJOR_PENTATONIC:
                    return MAJOR_PENTATONIC;
                case ID_OCTAVE:
                    return OCTAVE;
                case ID_DIMINISHED:
                    return DIMINISHED;
            }
            return CHROMATIC;
        }
        
        static int note(const std::vector<SCALE_TYPE> pScales, const int pBaseNote, const int pNoteStepOffset) {
            if (pNoteStepOffset > 0) {
                const int mSize = (int)pScales.size();
                const int mOctave = pNoteStepOffset / mSize;
                const int mIndex = pNoteStepOffset % mSize;
                const int mScaleEntry = pScales[mIndex];
                const int mNote = pBaseNote + (mOctave * 12) + mScaleEntry;
                return mNote;
            } else if (pNoteStepOffset < 0) {
                const int mSize = (int)pScales.size();
                const int mNormalizedNoteStepOffset = -pNoteStepOffset + ( mSize - 1);
                const int mOctave = -mNormalizedNoteStepOffset / mSize;
                const int mIndex = ( mSize - 1 ) - ( mNormalizedNoteStepOffset % mSize );
                const int mScaleEntry = pScales[mIndex];
                const int mNote = pBaseNote + (mOctave * 12) + mScaleEntry;
                return mNote;
            } else {
                return pBaseNote;
            }
        }
    };
}

#endif /* Scales_hpp */
