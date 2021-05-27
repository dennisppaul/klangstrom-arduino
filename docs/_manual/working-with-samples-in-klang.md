---
layout: page
title: Working with Samples in Klang
permalink: /working-with-samples-in-klang/
index: 32
---

samples are chunks of audio data that can be played back at varying speed and volume. *Klang* provides a convenient way to work with samples via a node called [`NodeSampler`]({{ site.baseurl }}{% link _klang/NodeSampler.md %}). it handles the playback by turning the data into an audio signal. the sample data for the node is stored in a float array e.g:

```c
float mSampleData[4] = {0.0, 1.0, 0.0, -1.0};
```

and the passed to the node during intialzation:

```c
NodeSampler mSample(mSampleData, 4);
```

these arrays can be of any length. however, note that the memory on MCUs is rather limited[^1]. the sample data can either be dynamically computed ( e.g mathematical function ) or collected ( e.g sensor data ) at runtime or prepared in a static array ( see below *Tool to Convert Audio Files into Sample Data* ).

the following applications plays a sample of a bass drum. it connects the sampler node ( `mSample` ) to an output node ( `mDAC` ). the sampler node is initialized with a float array of sample data which is then played back every beat.

```c
#include "Nodes.hpp"
#include "606bd1.wav.h"

using namespace klang;
using namespace klangstrom;

NodeDAC     mDAC;
NodeSampler mSample(_606bd1_data, _606bd1_data_length);

void setup() {
    Klang::lock();
    Klang::connect(mSample, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL_LEFT);
    mSample.loop(false);
    Klang::unlock();
    beats_per_minute(120);
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, 
                SIGNAL_TYPE* pInputLeft,  SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}

void beat(uint32_t pBeat) {
    mSample.trigger();
}
```

below is the accompanying C/C++ file containing the sample data ( note that not all data is shown below. see `File > Examples > Klangstrom > applications > DrumMachine` for the full data set ):

```c
/*
 * FILE ............... : 606bd1.wav
 * CHANNELS ........... : 1
 * SAMPLE RATE ........ : 48000
 * NUMBER OF FRAMES ... : 11926
 */

static const uint32_t _606bd1_data_length = 11926;
static const float _606bd1_data[_606bd1_data_length] = {
    6.1035156E-5, -5.79834E-4, -0.0015563965, -0.0020751953, 
    -0.0023498535, -0.0025024414, -0.003112793, -0.0029296875, 
    -0.00289917, -0.0028381348, -0.0032348633, -0.0028076172, 
    -0.0012207031, 0.001953125, 0.024108887, 0.06637573, 

    ...

    0.11767578, 0.16567993, 0.20831299, 0.24707031, 
    0.27905273, 0.30532837, 0.32632446, 0.3435974, 
    0.3585205, 0.37191772, 0.38476562, 0.39694214, 
    0.4091797, 0.4208374, 0.4324646, 0.44158936, 
}
```

## Tool to Convert Audio Files into Sample Data

the following [Processing](https://processing.org) sketch converts an audio file ( e.g AIFF, WAV, MP3[^2] ) into a C/C++ file which can be integrated into a *Klangstrom* application:

```c
/**
 * convert a sound file into an array of mono float sample data
 */

import processing.sound.*;

AudioSample mSample;
static final int AUDIO_CHANNEL = 0;

void settings() {
    size(1024, 768);
}

void setup() {
    selectInput("Select a sound file to convert:", "fileSelected");
}

void draw() {
    background(255);
    noFill();
    stroke(255, 127, 0);
    line(0, height / 2.0f, width, height / 2.0f);
    stroke(0);
    if (mSample != null) {
        beginShape();
        for (int i = 0; i < mSample.frames(); i++) {
            float x = ((float) i / (float) mSample.frames()) * width;
            float y = getSampleAt(mSample, i, AUDIO_CHANNEL) * height / 2.0f + height / 2.0f;
            vertex(x, y);
        }
        endShape(OPEN);
    }
}

void fileSelected(File selection) {
    if (selection != null) {
        try {
            String mPath = selection.getAbsolutePath();
            mSample = new SoundFile(this, mPath);
            mSample.loop();

            if (mSample.channels() > 1) {
                println("### WARNING only channel #"+AUDIO_CHANNEL+" of this multi-channel source file is converted into a sample.");
            }

            final String mSampleName = sanitizeSampleName(selection.getName());
            final String mSampleLengthName = mSampleName + "_data_length";

            StringBuilder sb = new StringBuilder();
            sb.append("/*\n");
            sb.append(" * SOURCE FILE ........ : ").append(selection.getName() + "\n");
            sb.append(" * CHANNELS ........... : ").append(mSample.channels() + "\n");
            sb.append(" * SAMPLE RATE ........ : ").append(mSample.sampleRate() + "\n");
            sb.append(" * NUMBER OF FRAMES ... : ").append(mSample.frames() + "\n");
            sb.append(" */");
            sb.append("\n\n");
            sb.append("static const uint32_t " + mSampleLengthName + " = " + mSample.frames() + ";\n");
            sb.append("static const float " + mSampleName + "_data[" + mSampleLengthName + "] = {\n");
            final int mWrap = 32;
            for (int i = 0; i < mSample.frames(); i++) {
                float s = getSampleAt(mSample, i, AUDIO_CHANNEL);
                sb.append(s);
                if (i != mSample.frames() - 1) {
                    sb.append(", ");
                }
                if (i % mWrap == mWrap - 1) {
                    sb.append("\n");
                }
            }
            sb.append("\n};");
            //println(sb.toString());
            saveStrings(mPath + ".h", new String[]{sb.toString()});
        } 
        catch (Exception e) {
            e.printStackTrace();
        }
    }
}

static float getSampleAt(AudioSample pSample, int i, int pChannel) {
    final float[] sd = new float[pSample.channels()];
    pSample.read(i, sd, 0, 1);
    final float s = sd[pChannel];
    return s;
}

static String sanitizeSampleName(final String pFilename) {
    String mSampleName = pFilename.toLowerCase();
    mSampleName = mSampleName.trim();
    mSampleName = mSampleName.replaceAll("-", "");
    String[] mNameFraction = mSampleName.split("\\.");
    if (mNameFraction.length > 0) {
        mSampleName = mNameFraction[0];
    }
    mSampleName = "_" + mSampleName;
    return mSampleName;
}
```

---

[^1]: e.g the maximum program storage space ( or *Flash Memory* ) on *KLST_TINY* is 524288 bytes ( ≈ 512KB ). each sample is stored as a 32BIT float ( i.e 4 bytes per float ) at a sampling rate of 48000Hz this amounts to a maximum sample length of approx 2.7sec. possible workarounds are to increase the maximum sample duration are to reduce the sample depth from 32BIT to 8BIT and to decrease the sample rate. e.g 8BIT samples at 12000Hz would allow for an approx sample length of 43.2sec. this approach however reduces the sound quality noticably. another strategy would be to also use *dynamic memory* to store sample data. this can be achieved e.g by declaring an array without the `const` ( i.e `float mArraySRAM[]` instead of `const float mArrayFlash[]` ). however *dynamic memory* ( or SRAM ) is even more limited. *KLST_TINY* has a maximum of 131072 bytes ( ≈ 128KB ) of *dynamic memory* which amounts to a maximum of approx 0.68sec sample length.
[^2]: please refer to the Processing Sound Library documentation of the [SoundFile](https://processing.org/reference/libraries/sound/SoundFile.html) class for supported formats.
