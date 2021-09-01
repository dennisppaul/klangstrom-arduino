//
//  Nodes.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

#ifndef Nodes_h
#define Nodes_h

#include "NodeADC.hpp"
#include "NodeADSR.hpp"
#include "NodeAdd.hpp"
#include "NodeAlienWah.hpp"
#include "NodeBuffer.hpp"
#include "NodeChorus.hpp"
#include "NodeClip.hpp"
#include "NodeDAC.hpp"
#include "NodeDelay.hpp"
#include "NodeDistortion.hpp"
#include "NodeEnvelope.hpp"
#include "NodeFFT.hpp"
#include "NodeKernel.hpp"
#include "NodeKernelBlock.hpp"
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

#endif /* Nodes_h */
