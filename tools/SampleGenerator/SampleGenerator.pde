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
