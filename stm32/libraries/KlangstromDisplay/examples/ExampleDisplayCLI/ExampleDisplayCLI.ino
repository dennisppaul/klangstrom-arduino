#include "Klangstrom.h"
#include "KlangstromDisplay.h"
#include "KlangstromDisplayFont_7x10.h"
#include "KlangstromDisplayTerminal.h"

#define EMBEDDED_CLI_IMPL
#include "embedded_cli.h"

using namespace klangstrom;

KlangstromDisplay         *Display;
KlangstromDisplayTerminal *mTerminal;

EmbeddedCli *cli;

void setup() {
    Serial.begin(115200);

    Display = KlangstromDisplay::create();
    Display->begin();
    Display->background(0, 0, 0);
    Display->clear();
    Display->color(255, 255, 255);

    mTerminal         = new KlangstromDisplayTerminal(Display, &Font_7x10);
    mTerminal->width  = 31;
    mTerminal->height = 29;
    mTerminal->x      = 11;
    mTerminal->y      = 15;
    mTerminal->foreground_color.set(0, 0, 0);
    mTerminal->background_color.set(255, 255, 255);

    EmbeddedCliConfig *config  = embeddedCliDefaultConfig();
    config->maxBindingCount    = 16;
    config->enableAutoComplete = false;
    cli                        = embeddedCliNew(config);
    if (cli == NULL) {
        mTerminal->print("Cli was not created. Check sizes!");
        return;
    }
    mTerminal->print("Cli has started. Enter your commands.");

    embeddedCliAddBinding(
        cli, {"hello", "Print hello message", true, (void *)"world", onHello});

    cli->onCommand = onCommand;
    cli->writeChar = writeChar;
}

void loop() {
    if (cli != NULL) {
        embeddedCliProcess(cli);
    }
}

void serialEvent() {
    while (Serial.available()) {
        int mRead = Serial.read();
        if (mRead == 'x') {
            mTerminal->clear();
        }
        if (cli != NULL) {
            embeddedCliReceiveChar(cli, (char)mRead);
        }
    }
}

void onCommand(EmbeddedCli *embeddedCli, CliCommand *command) {
    mTerminal->print("Received command:\n\r");
    mTerminal->println(command->name);
    embeddedCliTokenizeArgs(command->args);
    for (int i = 1; i <= embeddedCliGetTokenCount(command->args); ++i) {
        mTerminal->print("arg ");
        mTerminal->print((char)('0' + i));
        mTerminal->print(": ");
        mTerminal->println(embeddedCliGetToken(command->args, i));
    }
}

void onHello(EmbeddedCli *cli, char *args, void *context) {
    if (embeddedCliGetTokenCount(args) == 0)
        mTerminal->print((const char *)context);
    else
        mTerminal->print(embeddedCliGetToken(args, 1));
    mTerminal->println();
}

void writeChar(EmbeddedCli *embeddedCli, char c) {
    mTerminal->print(c);
}