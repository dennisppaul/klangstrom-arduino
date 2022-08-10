/*
 * this example demonstrates how to list all files on an SD card
 * and play the first file as a WAV file.
 */

#include "Klangstrom.h"
#include "KlangNodes.hpp"
#include "KlangstromCard.h"

using namespace klang;
using namespace klangstrom;

NodeDAC        mDAC;
NodeSamplerI16 mSampler;
NodeSamplerI16 mSamplerRight;

void setup() {
    Serial.begin(115200);
    Serial.println("--- Example Play WAV File");

    bool mCardOpenError = Card.begin();
    if (mCardOpenError) {
        Serial.println("--- opening card failed");
        return;
    }

    Serial.println("--- reading file list");
    vector<String> mFiles;
    Card.get_file_list(mFiles);

    for (uint16_t i = 0; i < mFiles.size(); i++) {
        Serial.print(i);
        Serial.print("\t");
        Serial.println(mFiles[i]);
    }
    Serial.println();

    bool mFileOpenError = Card.open(mFiles[0]); /* open first file on card */
    if (mFileOpenError) {
        Serial.println("--- opening file failed");
        return;
    } else {
        Serial.print("--- opening file: ");
        Serial.println(mFiles[0]);
    }

    KlangstromWaveFile mWAV;
    int                mWAVOpenError = Card.load_WAV(&mWAV);
    if (!mWAVOpenError) {
        Serial.println("--- WAV file header: ");
        Card.print_WAV_header(mWAV.header);

        Klang::connect(mSampler, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL_LEFT);
        mSampler.set_buffer(mWAV.get_sample_data(KlangstromWaveFile::CHANNEL_LEFT));
        mSampler.set_buffer_size(mWAV.number_of_samples);
        mSampler.loop(true);
        mSampler.start();

        if (mWAV.header->NumOfChan == 2) {
            mDAC.set_stereo(true);
            Klang::connect(mSamplerRight, Node::CH_OUT_SIGNAL, mDAC, NodeDAC::CH_IN_SIGNAL_RIGHT);
            mSamplerRight.set_buffer(mWAV.get_sample_data(KlangstromWaveFile::CHANNEL_RIGHT));
            mSamplerRight.set_buffer_size(mWAV.number_of_samples);
            mSamplerRight.loop(true);
            mSamplerRight.start();
        } else {
            mDAC.set_stereo(false);
        }
    } else {
        Serial.print("--- loading WAV failed : ");
        Serial.println(mWAVOpenError);
    }
}

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight,
                SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
    mDAC.process_frame(pOutputLeft, pOutputRight);
}
