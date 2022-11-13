/*
    this example demonstrates how to print the header of a WAV file.
*/

#include "Klangstrom.h"
#include "KlangstromCard.h"

using namespace klangstrom;

void print_file_list(vector<String>& pFiles) {
    if (pFiles.size() > 0) {
        for (uint16_t i = 0; i < pFiles.size(); i++) {
            Serial.print(i);
            Serial.print("\t");
            Serial.println(pFiles[i]);
        }
        Serial.println();
    } else {
        Serial.println("--- no files found");
    }
}

bool open_file(String& pFileName) {
    Serial.print("--- opening file: ");
    Serial.print(pFileName);
    return Card.open(pFileName);
}

void read_WAV_header() {
    WaveHeader_t* mHeader = new WaveHeader_t();
    int           mError  = Card.load_WAV_header(mHeader);
    if (!mError) {
        Serial.println();
        Serial.println("WAV file header: ");
        Card.print_WAV_header(mHeader);
    } else {
        Serial.print("--- loading WAV header failed : ");
        Serial.println(mError);
    }
    delete mHeader;
}

void setup() {
    Serial.begin(115200);
    Serial.println("--- Example WAV File Info");
    Serial.println();

    bool           mOpenCardError = Card.begin();
    vector<String> mFiles;
    if (!mOpenCardError) {
        Serial.println("--- reading file list");
        Card.get_file_list(mFiles);
        print_file_list(mFiles);
    } else {
        Serial.println("--- opening card failed");
        return;
    }

    bool mOpenFileError = open_file(mFiles[0]); /* open first file on card */
    if (Card.is_open() && !mOpenFileError) {
        read_WAV_header();
    } else {
        Serial.println("--- opening file failed");
        return;
    }
    Card.close();
}

void loop() {}
