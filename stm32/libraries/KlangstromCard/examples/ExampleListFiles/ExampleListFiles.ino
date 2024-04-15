/*
 * this example demonstrates how to list all files on an SD card.
 */

#include "Klangstrom.h"
#include "KlangstromCard.h"

using namespace klangstrom;

void setup() {
    Serial.begin(115200);
    Serial.println("----------------");
    Serial.println("ExampleListFiles");
    Serial.println("----------------");

    Serial.println("--- ExampleList Files");
    Serial.println();

    Card.begin();

    Serial.println("--- reading file list");
    vector<String> mFiles;
    Card.get_file_list(mFiles);

    for (uint16_t i = 0; i < mFiles.size(); i++) {
        Serial.print(i);
        Serial.print("\t");
        Serial.println(mFiles[i]);
    }
}

void loop() {}
