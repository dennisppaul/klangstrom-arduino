/*
  rakarrack - a guitar efects software

  global.h  -  Variable Definitions and functions
  Copyright (C) 2008-2010 Josep Andreu
  Author: Josep Andreu & Ryan Billing

 This program is free software; you can redistribute it and/or modify
 it under the terms of version 2 of the GNU General Public License
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License (version 2) for more details.

 You should have received a copy of the GNU General Public License
 (version2)  along with this program; if not, write to the Free Software Foundation,
 Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/

#ifndef DXEMU_H
#define DXEMU_H

#define D_PI                                 6.283185f
#define PI                                   3.141598f
#define LOG_10                               2.302585f
#define LOG_2                                0.693147f
#define LN2R                                 1.442695041f
#define CNST_E                               2.71828182845905f
#define AMPLITUDE_INTERPOLATION_THRESHOLD    0.0001f
#define FF_MAX_VOWELS                        6
#define FF_MAX_FORMANTS                      12
#define FF_MAX_SEQUENCE                      8
#define MAX_FILTER_STAGES                    5
#define RND                                  (rand() / (RAND_MAX + 1.0))
#define RND1                                 (((float)rand()) / (((float)RAND_MAX) + 1.0f))
#define F2I(f, i)                            (i) = ((f > 0) ? ((int)(f)) : ((int)(f - 1.0f)))
#define dB2rap(dB)                           (float)((expf((dB)*LOG_10 / 20.0f)))
#define rap2dB(rap)                          (float)((20 * log(rap) / LOG_10))
#define CLAMP(x, low, high)                  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
#define INTERPOLATE_AMPLITUDE(a, b, x, size) ((a) + ((b) - (a)) * (float)(x) / (float)(size))
#define ABOVE_AMPLITUDE_THRESHOLD(a, b)      ((2.0f * fabs((b) - (a)) / (fabs((b) + (a) + 0.0000000001f))) > AMPLITUDE_INTERPOLATION_THRESHOLD)
#define POLY                                 8
#define DENORMAL_GUARD                       1e-18f  // Make it smaller until CPU problem re-appears
#define SwapFourBytes(data)                  ((((data) >> 24) & 0x000000ff) | (((data) >> 8) & 0x0000ff00) | (((data) << 8) & 0x00ff0000) | (((data) << 24) & 0xff000000))
#define D_NOTE                               1.059463f
#define LOG_D_NOTE                           0.057762f
#define D_NOTE_SQRT                          1.029302f
#define MAX_PEAKS                            8
#define MAX_ALIENWAH_DELAY                   100
#define ATTACK                               0.175f  // crossover time for reverse delay
#define MAX_DELAY                            2       // Number of Seconds
#define MAXHARMS                             8       // max number of harmonics available
#define MAX_PHASER_STAGES                    12
#define MAX_CHORUS_DELAY                     250.0f  // ms
#define LN2                                  (1.0f)  // Uncomment for att/rel to behave more like a capacitor.
#define MUG_CORR_FACT                        0.4f
#define Thi                                  0.67f         // High threshold for limiting onset // Crunch waveshaping constants
#define Tlo                                  -0.65f        // Low threshold for limiting onset
#define Tlc                                  -0.6139445f   // Tlo + sqrt(Tlo/500)
#define Thc                                  0.6365834f    // Thi - sqrt(Thi/600)
#define CRUNCH_GAIN                          100.0f        // Typical voltage gain for most OD stompboxes
#define DIV_TLC_CONST                        0.002f        // 1/300
#define DIV_THC_CONST                        0.0016666f    // 1/600 (approximately) // End waveshaping constants
#define D_FLANGE_MAX_DELAY                   0.055f        // Number of Seconds  - 50ms corresponds to fdepth = 20 (Hz). Added some extra for padding
#define LFO_CONSTANT                         9.765625e-04  // 1/(2^LOG_FMAX - 1)
#define LOG_FMAX                             10.0f         //  -- This optimizes LFO sweep for useful range.
#define MINDEPTH                             20.0f         // won't allow filter lower than 20Hz
#define MAXDEPTH                             15000.0f      // Keeps delay greater than 2 samples at 44kHz SR
#define MAX_EQ_BANDS                         16
#define CLOSED                               1
#define OPENING                              2
#define OPEN                                 3
#define CLOSING                              4
#define ENV_TR                               0.0001f
#define HARMONICS                            11
#define REV_COMBS                            8
#define REV_APS                              4
#define MAX_SFILTER_STAGES                   12

#include <dirent.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>

#include "config.h"
// #include <alsa/asoundlib.h>
// #include <X11/xpm.h>
// #include <jack/jack.h>
// #include <jack/midiport.h>
// #include <Fl/Fl_Preferences.H>
// #include "FPreset.h"
// #include "Reverb.h"
#include "Chorus.h"
#include "Echo.h"
// #include "Phaser.h"
// #include "Distorsion.h"
// #include "EQ.h"
// #include "Compressor.h"
// #include "Alienwah.h"
// #include "DynamicFilter.h"
// #include "Pan.h"
// #include "Harmonizer.h"
// #include "MusicDelay.h"
// #include "Gate.h"
// #include "Tuner.h"
// #include "MIDIConverter.h"
// #include "RecognizeNote.h"
// #include "RecChord.h"
// #include "NewDist.h"
// #include "APhaser.h"
// #include "Valve.h"
// #include "Dual_Flange.h"
// #include "Ring.h"
// #include "Exciter.h"
// #include "MBDist.h"
// #include "Arpie.h"
// #include "Expander.h"
// #include "Shuffle.h"
// #include "Synthfilter.h"
// #include "MBVvol.h"
// #include "Convolotron.h"
// #include "Resample.h"
// #include "Looper.h"
// #include "RyanWah.h"
// #include "RBEcho.h"
// #include "CoilCrafter.h"
// #include "ShelfBoost.h"
// #include "Vocoder.h"
// #include "Sustainer.h"
// #include "Sequence.h"
// #include "Shifter.h"
// #include "StompBox.h"
// #include "Reverbtron.h"
// #include "Echotron.h"
// #include "StereoHarm.h"
// #include "CompBand.h"
// #include "Opticaltrem.h"
// #include "Vibe.h"

extern int          Pexitprogram, preset;
extern int          commandline, gui;
extern int          exitwithhelp, nojack;
extern int          maxx_len;
extern int          error_num;
extern int          PERIOD;
extern int          reconota;
extern int          stecla;
extern int          looper_lqua;
extern unsigned int SAMPLE_RATE;
extern int          note_active[POLY];
extern int          rnote[POLY];
extern int          gate[POLY];
extern int          pdata[50];
extern float        val_sum;
extern float        fPERIOD;
extern unsigned int SAMPLE_RATE;
extern float        fSAMPLE_RATE;
extern float        cSAMPLE_RATE;
extern float        r__ratio[12];
extern int          Wave_res_amount;
extern int          Wave_up_q;
extern int          Wave_down_q;
// extern Pixmap       p, mask;
// extern XWMHints    *hints;
extern float freqs[12];
extern float lfreqs[12];
extern float aFreq;

#endif
