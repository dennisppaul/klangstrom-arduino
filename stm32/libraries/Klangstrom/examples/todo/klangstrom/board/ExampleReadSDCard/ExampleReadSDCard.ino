//
// ExampleReadSDCard
//

/*

    pin connection map for KLST_TINYv0.1:

    | SD_CARD | KLST     |
    |---------|----------|
    | MOSI    | SPI_MOSI |
    | MISO    | SPI_MISO |
    | SCK     | SPI_SCK  |
    | CS      | GPIO_00  |

    pin connection map for KLST_SHEEPv0.1 on :

    | SD_CARD | KLST         |
    |---------|--------------|
    | MOSI    | SPI_USR_MOSI |
    | MISO    | SPI_USR_MISO |
    | SCK     | SPI_USR_SCK  |
    | CS      | GPIO_00   |

*/

#include "Klangstrom.h"
#include <SPI.h>
#include <SD.h>

Sd2Card card;
SdVolume volume;
SdFile root;

void setup() {
    klangstrom::begin_serial_debug(true);

    Serial.print("\nInitializing SD card...");

    if (!card.init(SPI_HALF_SPEED, GPIO_00)) {
        Serial.println("initialization failed. Things to check:");
        Serial.println("* is a card inserted?");
        Serial.println("* is your wiring correct?");
        Serial.println("* did you change the chipSelect pin to match your shield or module?");
        while (1);
    } else {
        Serial.println("Wiring is correct and a card is present.");
    }

    // print  type of card
    Serial.println();
    Serial.print("Card type:         ");
    switch (card.type()) {
        case SD_CARD_TYPE_SD1:
            Serial.println("SD1");
            break;
        case SD_CARD_TYPE_SD2:
            Serial.println("SD2");
            break;
        case SD_CARD_TYPE_SDHC:
            Serial.println("SDHC");
            break;
        default:
            Serial.println("Unknown");
    }

    // open  'volume'/'partition' - should be FAT16 or FAT32
    if (!volume.init(card)) {
        Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
        while (1);
    }

    Serial.print("Clusters:          ");
    Serial.println(volume.clusterCount());
    Serial.print("Blocks x Cluster:  ");
    Serial.println(volume.blocksPerCluster());

    Serial.print("Total Blocks:      ");
    Serial.println(volume.blocksPerCluster() * volume.clusterCount());
    Serial.println();

    // print  type and size of first FAT-type volume
    uint32_t volumesize;
    Serial.print("Volume type is:    FAT");
    Serial.println(volume.fatType(), DEC);

    volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
    volumesize *= volume.clusterCount();       // we'll have a lot of clusters
    volumesize /= 2;                           // SD card blocks are always 512 bytes (2 blocks are 1KB)
    Serial.print("Volume size (Kb):  ");
    Serial.println(volumesize);
    Serial.print("Volume size (Mb):  ");
    volumesize /= 1024;
    Serial.println(volumesize);
    Serial.print("Volume size (Gb):  ");
    Serial.println((float)volumesize / 1024.0);

    Serial.println("\nFiles found on the card (name, date and size in bytes): ");
    root.openRoot(volume);

    // list all files on card with date and size
    root.ls(LS_R | LS_DATE | LS_SIZE);
}
