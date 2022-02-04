/*
 * Klang – a node+text-based synthesizer library
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2022 Dennis P Paul.
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

// @TODO("limit command range to 0x00–0x7F to comply to midi value range")

#ifndef KlangCommands_hpp
#define KlangCommands_hpp

static const KLANG_CMD_TYPE KLANG_CMD_RESET_CMD                 = 0x00;
static const KLANG_CMD_TYPE KLANG_CMD_RESET                     = 0x01;
static const KLANG_CMD_TYPE KLANG_CMD_RESERVED_01               = 0x02;
static const KLANG_CMD_TYPE KLANG_CMD_SYNTH_START               = 0x03;
static const KLANG_CMD_TYPE KLANG_CMD_SYNTH_STOP                = 0x04;
static const KLANG_CMD_TYPE KLANG_CMD_CREATE_NODE_I8            = 0x05;
static const KLANG_CMD_TYPE KLANG_CMD_DELETE_NODE_I8            = 0x06;
static const KLANG_CMD_TYPE KLANG_CMD_OUTPUT_NODE_I8            = 0x07;
static const KLANG_CMD_TYPE KLANG_CMD_INPUT_NODE_I8             = 0x08;
static const KLANG_CMD_TYPE KLANG_CMD_CONNECT_NODES_I8_I8_I8_I8 = 0x09;

static const KLANG_CMD_TYPE KLANG_SET_ADD_STAGE_F32_F32         = 0x20;
static const KLANG_CMD_TYPE KLANG_SET_AMPLIFICATION_F32         = 0x21;
static const KLANG_CMD_TYPE KLANG_SET_AMPLITUDE_F32             = 0x22;
static const KLANG_CMD_TYPE KLANG_SET_ATTACK_F32                = 0x23;
static const KLANG_CMD_TYPE KLANG_SET_CLEAR_STAGES              = 0x24;
static const KLANG_CMD_TYPE KLANG_SET_CLIP_F32                  = 0x25;
static const KLANG_CMD_TYPE KLANG_SET_CUTOFF_F32                = 0x26;
static const KLANG_CMD_TYPE KLANG_SET_DECAY_F32                 = 0x27;
static const KLANG_CMD_TYPE KLANG_SET_DELAY_F32                 = 0x28;
static const KLANG_CMD_TYPE KLANG_SET_DURATION_F32              = 0x29;
static const KLANG_CMD_TYPE KLANG_SET_FEEDBACK_F32              = 0x2A;
static const KLANG_CMD_TYPE KLANG_SET_FILTER_F32                = 0x2B;
static const KLANG_CMD_TYPE KLANG_SET_FREQUENCY_F32             = 0x2C;
static const KLANG_CMD_TYPE KLANG_SET_IN_MAX_F32                = 0x2D;
static const KLANG_CMD_TYPE KLANG_SET_IN_MIN_F32                = 0x2E;
static const KLANG_CMD_TYPE KLANG_SET_MAX_F32                   = 0x2F;
static const KLANG_CMD_TYPE KLANG_SET_MIN_F32                   = 0x30;
static const KLANG_CMD_TYPE KLANG_SET_MIX_F32                   = 0x31;
static const KLANG_CMD_TYPE KLANG_SET_MIX_I8_F32                = 0x32;
static const KLANG_CMD_TYPE KLANG_SET_MUTE_I8                   = 0x33;
static const KLANG_CMD_TYPE KLANG_SET_OFFSET_F32                = 0x34;
static const KLANG_CMD_TYPE KLANG_SET_OUT_MAX_F32               = 0x35;
static const KLANG_CMD_TYPE KLANG_SET_OUT_MIN_F32               = 0x36;
static const KLANG_CMD_TYPE KLANG_SET_PAN_I8_F32                = 0x37;
static const KLANG_CMD_TYPE KLANG_SET_RANGE_MAX_F32             = 0x38;
static const KLANG_CMD_TYPE KLANG_SET_RANGE_MIN_F32             = 0x39;
static const KLANG_CMD_TYPE KLANG_SET_RATE_F32                  = 0x3A;
static const KLANG_CMD_TYPE KLANG_SET_RELEASE_F32               = 0x3B;
static const KLANG_CMD_TYPE KLANG_SET_RESONANCE_F32             = 0x3C;
static const KLANG_CMD_TYPE KLANG_SET_SPEED_F32                 = 0x3D;
static const KLANG_CMD_TYPE KLANG_SET_START                     = 0x3E;
static const KLANG_CMD_TYPE KLANG_SET_STEREO_I8                 = 0x3F;
static const KLANG_CMD_TYPE KLANG_SET_STOP                      = 0x40;
static const KLANG_CMD_TYPE KLANG_SET_STRIDE_F32                = 0x41;
static const KLANG_CMD_TYPE KLANG_SET_SUSTAIN_F32               = 0x42;
static const KLANG_CMD_TYPE KLANG_SET_SWEEPRATE_F32             = 0x43;
static const KLANG_CMD_TYPE KLANG_SET_SWEEP_F32                 = 0x44;
static const KLANG_CMD_TYPE KLANG_SET_TIME_SCALE_F32            = 0x45;
static const KLANG_CMD_TYPE KLANG_SET_TYPE_I8                   = 0x46;
static const KLANG_CMD_TYPE KLANG_SET_VALUE_F32                 = 0x47;
static const KLANG_CMD_TYPE KLANG_SET_WAVEFORM_I8               = 0x48;
static const KLANG_CMD_TYPE KLANG_SET_WET_F32                   = 0x49;

static const KLANG_CMD_TYPE KLANG_NODE_ADC                      = 0x40;
static const KLANG_CMD_TYPE KLANG_NODE_ADD                      = 0x41;
static const KLANG_CMD_TYPE KLANG_NODE_ADSR                     = 0x42;
static const KLANG_CMD_TYPE KLANG_NODE_ALIEN_WAH                = 0x43;
static const KLANG_CMD_TYPE KLANG_NODE_BUFFER                   = 0x44;
static const KLANG_CMD_TYPE KLANG_NODE_CHORUS                   = 0x45;
static const KLANG_CMD_TYPE KLANG_NODE_CLIP                     = 0x46;
static const KLANG_CMD_TYPE KLANG_NODE_DAC                      = 0x47;
static const KLANG_CMD_TYPE KLANG_NODE_DELAY                    = 0x48;
static const KLANG_CMD_TYPE KLANG_NODE_DISTORTION               = 0x49;
static const KLANG_CMD_TYPE KLANG_NODE_ENVELOPE                 = 0x4A;
static const KLANG_CMD_TYPE KLANG_NODE_KERNAL                   = 0x4B;
static const KLANG_CMD_TYPE KLANG_NODE_KERNEL_BLOCK             = 0x4C;
static const KLANG_CMD_TYPE KLANG_NODE_MAP                      = 0x4D;
static const KLANG_CMD_TYPE KLANG_NODE_MIXER2                   = 0x4E;
static const KLANG_CMD_TYPE KLANG_NODE_MIXER4                   = 0x4F;
static const KLANG_CMD_TYPE KLANG_NODE_MIXER4_STEREO            = 0x50;
static const KLANG_CMD_TYPE KLANG_NODE_MUTE                     = 0x51;
static const KLANG_CMD_TYPE KLANG_NODE_NOISE                    = 0x52;
static const KLANG_CMD_TYPE KLANG_NODE_PASSTHROUGH              = 0x53;
static const KLANG_CMD_TYPE KLANG_NODE_PATCH                    = 0x54;
static const KLANG_CMD_TYPE KLANG_NODE_PATCH16                  = 0x55;
static const KLANG_CMD_TYPE KLANG_NODE_PHASER                   = 0x56;
static const KLANG_CMD_TYPE KLANG_NODE_PORTAMENTO               = 0x57;
static const KLANG_CMD_TYPE KLANG_NODE_SAMPLER                  = 0x58;
static const KLANG_CMD_TYPE KLANG_NODE_SPLITTER                 = 0x59;
static const KLANG_CMD_TYPE KLANG_NODE_VALUE                    = 0x5A;
static const KLANG_CMD_TYPE KLANG_NODE_VCA                      = 0x5B;
static const KLANG_CMD_TYPE KLANG_NODE_VCF_LP                   = 0x5C;
static const KLANG_CMD_TYPE KLANG_NODE_VCF_MOOG_LHBP            = 0x5D;
static const KLANG_CMD_TYPE KLANG_NODE_VCF_MOOG_LP              = 0x5E;
static const KLANG_CMD_TYPE KLANG_NODE_VCO_FUNC                 = 0x5F;
static const KLANG_CMD_TYPE KLANG_NODE_VCO_WAVETABLE            = 0x60;

#endif /* KlangCommands_hpp */

/* -- AUTO GENERATED CODE (END) -- */
