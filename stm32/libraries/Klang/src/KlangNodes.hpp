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

#ifndef KlangNodes_h
#define KlangNodes_h

#include "NodeADC.hpp"
#include "NodeADSR.hpp"
#include "NodeAdd.hpp"
#include "NodeAlienWah.hpp"
#include "NodeBuffer.hpp"
#include "NodeChorus.hpp"
#include "NodeClip.hpp"
#include "NodeCompressor.hpp"
#include "NodeDAC.hpp"
#include "NodeDelay.hpp"
#include "NodeDistortion.hpp"
#include "NodeEnvelope.hpp"
#include "NodeVowelFormantFilter.hpp"
#include "NodeFFT.hpp"
#include "NodeKernel.hpp"
#include "NodeKernelBlock.hpp"
#include "NodeKernelBlock2.hpp"
#include "NodeKernelBlockMulti.hpp"
#include "NodeKernelBlockStereo.hpp"
#include "NodeMap.hpp"
#include "NodeMixer16.hpp"
#include "NodeMixer2.hpp"
#include "NodeMixer32.hpp"
#include "NodeMixer4.hpp"
#include "NodeMixer4Stereo.hpp"
#include "NodeMixer8.hpp"
#include "NodeMixerMulti.hpp"
#include "NodeMixerMultiStereo.hpp"
#include "NodeMute.hpp"
#include "NodeNoise.hpp"
#include "NodePassthrough.hpp"
#include "NodePatch.hpp"
#include "NodePatch16.hpp"
#include "NodePhaser.hpp"
#include "NodePortamento.hpp"
#include "NodeRamp.hpp"
#include "NodeSampler.hpp"
#include "NodeSoftClip.hpp"
#include "NodeSplitter.hpp"
#include "NodeTrigger.hpp"
#include "NodeVCA.hpp"
#include "NodeVCF.hpp"
#include "NodeVCFLowPass.hpp"
#include "NodeVCFMoogLHBP.hpp"
#include "NodeVCFMoogLP.hpp"
#include "NodeVCOFunction.hpp"
#include "NodeVCOWavetable.hpp"
#include "NodeValue.hpp"

#ifdef KLANG_EXT_NODE_REVERB
#include "NodeReverb.hpp"
#endif
#ifdef KLANG_EXT_NODE_VOCODER
#include "NodeVocoder.hpp"
#endif
#ifdef KLANG_EXT_NODE_TEXT_TO_SPEECH
#include "NodeTextToSpeech.hpp"
#endif
#ifdef KLANG_EXT_NODE_TEXT_TO_SPEECH_SAM
#include "NodeTextToSpeechSAM.hpp"
#endif
#ifdef KLANG_EXT_NODE_PROTOSEG
#include "NodePROTOSEQ.hpp"
#endif

#endif /* KlangNodes_h */
