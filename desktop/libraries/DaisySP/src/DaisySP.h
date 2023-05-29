//    DaisySP is a DSP Library targeted at the Electrosmith Daisy Product Line.
//    Author: Stephen Hensley, 2019
//
//    However, this is decoupled from the hardware in such a way that it
//        should be useful outside of the ARM context with different build configurations.
//
//    A few general notes about the contents of the library:
//        - all memory usage is static.
//        - in cases of potentially large memory usage: the user will either supply a buffer and a size, or the class will be a template that can have size set at compile time.
//        - all modules will have an Init() function, and a Process() function.
//        - all modules, unless otherwise noted, will process a single sample at a time.
//        - all processing will be done with 'float' type unless otherwise noted.
//
#pragma once
#ifndef DSYSP_H
#define DSYSP_H

/** Control Modules */
#include "adenv.h"
#include "adsr.h"
#include "line.h"
#include "phasor.h"

/** Drum Modules */
#include "analogbassdrum.h"
#include "analogsnaredrum.h"
#include "hihat.h"
#include "synthbassdrum.h"
#include "synthsnaredrum.h"

/** Dynamics Modules */
#include "balance.h"
#include "compressor.h"
#include "crossfade.h"
#include "limiter.h"

/** Effects Modules */
#include "autowah.h"
#include "bitcrush.h"
#include "chorus.h"
#include "decimator.h"
#include "flanger.h"
#include "fold.h"
#include "overdrive.h"
#include "reverbsc.h"
#include "phaser.h"
#include "pitchshifter.h"
#include "sampleratereducer.h"
#include "tremolo.h"

/** Filter Modules */
#include "allpass.h"
#include "atone.h"
#include "biquad.h"
#include "comb.h"
#include "mode.h"
#include "moogladder.h"
#include "nlfilt.h"
#include "svf.h"
#include "tone.h"
#include "fir.h"

/** Noise Modules */
#include "clockednoise.h"
#include "dust.h"
#include "fractal_noise.h"
#include "grainlet.h"
#include "particle.h"
#include "whitenoise.h"

/** Physical Modeling Modules */
#include "drip.h"
#include "modalvoice.h"
#include "pluck.h"
#include "PolyPluck.h"
#include "resonator.h"
#include "KarplusString.h"
#include "stringvoice.h"

/** Synthesis Modules */
#include "blosc.h"
#include "fm2.h"
#include "formantosc.h"
#include "harmonic_osc.h"
#include "oscillator.h"
#include "oscillatorbank.h"
#include "variablesawosc.h"
#include "variableshapeosc.h"
#include "vosim.h"
#include "zoscillator.h"

/** modules Modules */
#include "dcblock.h"
#include "delayline.h"
#include "dsp.h"
#include "jitter.h"
#include "looper.h"
#include "maytrig.h"
#include "metro.h"
#include "port.h"
#include "samplehold.h"
#include "smooth_random.h"

#endif
