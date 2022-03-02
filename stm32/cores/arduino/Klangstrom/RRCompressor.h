/*
  rakarrack - a guitar effects software

 Compressor.h  -  Compressor Effect definitions
 Based on artscompressor.cc by Matthias Kretz <kretz@kde.org>
 Stefan Westerfeld <stefan@space.twc.de>

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
 (version2)  along with this program; if not, write to the Free Software
 Foundation,
 Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/

#ifndef RR_COMPRESSOR_H
#define RR_COMPRESSOR_H

// #include "FPreset.h"
#include "RRGlobal.h"

class RRCompressor {
public:
    static const uint8_t PRESET_2_TO_1            = 0;
    static const uint8_t PRESET_4_TO_1            = 1;
    static const uint8_t PRESET_8_TO_1            = 2;
    static const uint8_t PRESET_FINAL_LIMITER     = 3;
    static const uint8_t PRESET_HARMONIC_ENHANCER = 4;
    static const uint8_t PRESET_BAND_COMP_BAND    = 5;
    static const uint8_t PRESET_END_COMP_BAND     = 6;
    static const uint8_t NUM_PRESETS              = 7;
    static const uint8_t PARAM_THRESHOLD          = 0;
    static const uint8_t PARAM_RATIO              = 1;
    static const uint8_t PARAM_OUTPUT             = 2;
    static const uint8_t PARAM_ATT                = 3;
    static const uint8_t PARAM_REL                = 4;
    static const uint8_t PARAM_A_OUT              = 5;
    static const uint8_t PARAM_KNEE               = 6;
    static const uint8_t PARAM_STEREO             = 7;
    static const uint8_t PARAM_PEAK               = 8;
    static const uint8_t NUM_PARAMS               = 9;

    RRCompressor();
    // RRCompressor(float *efxoutl_, float *efxoutr_);
    ~RRCompressor();

    void out(float *smps_l, float *smps_r);

    void Compressor_Change(int np, int value);
    void Compressor_Change_Preset(int npreset);
    // void Compressor_Change_Preset(int dgui, int npreset);
    int  getpar(int npar);
    void cleanup();

    // float *efxoutl;
    // float *efxoutr;

    int tatt;  // attack time  (ms)
    int trel;  // release time (ms)
    int tratio;
    int toutput;
    int tthreshold;
    int a_out;
    int stereo;
    int tknee;
    int peak;
    int clipping;
    int limit;

private:
    float rvolume;
    float lvolume;
    float rvolume_db;
    float lvolume_db;
    float thres_db;  // threshold
    float knee;
    float thres_mx;
    float kpct;
    float ratio;   // ratio
    float kratio;  // ratio maximum for knee region
    float eratio;  // dynamic ratio
    float makeup;  // make-up gain
    float makeuplin;

    float outlevel;
    float att, attr, attl;
    float rel, relr, rell;
    // float relcnst, attconst;
    int ltimer, rtimer, hold;

    float rgain;
    float rgain_old;

    float lgain;
    float lgain_old;

    float lgain_t;
    float rgain_t;

    float coeff_kratio;
    float coeff_ratio;
    float coeff_knee;
    float coeff_kk;
    float lpeak;
    float rpeak;

    // class FPreset *Fpre;
};

#endif
