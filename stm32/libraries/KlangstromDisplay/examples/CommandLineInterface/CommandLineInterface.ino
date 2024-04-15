#include "Klangstrom.h"
#include "KlangstromDisplay.h"
#include "KlangstromDisplayCLI.h"
#include "KlangstromDisplayFont_7x10.h"

using namespace klangstrom;

KlangstromDisplay    &Display = KlangstromDisplay::create();
KlangstromDisplayCLI &CLI     = KlangstromDisplayCLI::create(&Display, &Font_7x10);

void setup() {
    Serial.begin(115200);
    Serial.println("--------------------");
    Serial.println("CommandLineInterface");
    Serial.println("--------------------");

    Serial.begin(115200);

    Display.begin();
    Display.background(255, 255, 255);
    Display.clear();
    Display.color(0, 0, 0);

    CLI.set_write_char_callback(writeChar);
    CLI.set_on_command_callback(onCommand);
    CLI.add_binding({"hello", "print hello message", true, (void *)"world", onHello});
    CLI.add_binding({"clear", "clear screen", true, (void *)"", onClear});
    CLI.color(0, 0, 0);
    CLI.background(255, 255, 255);
    CLI.clear();
}

void loop() {
    CLI.process();
}

void serialEvent() {
    while (Serial.available()) {
        CLI.receive_char(Serial.read());
    }
}

void onCommand(EmbeddedCli *embeddedCli, CliCommand *command) {
    CLI.print("Received command:\n\r");
    CLI.println(command->name);
    CLI.tokenize_args(command->args);
    for (int i = 1; i <= CLI.get_token_count(command->args); ++i) {
        CLI.print("arg ");
        CLI.print((char)('0' + i));
        CLI.print(": ");
        CLI.println(CLI.get_token(command->args, i));
    }
}

void onHello(EmbeddedCli *cli, char *args, void *context) {
    if (CLI.get_token_count(args) == 0) {
        CLI.print((const char *)context);
    } else {
        CLI.print(CLI.get_token(args, 1));
    }
    CLI.println();
}

void onClear(EmbeddedCli *cli, char *args, void *context) {
    CLI.clear();
}

void writeChar(EmbeddedCli *embeddedCli, char c) {
    CLI.print(c);
}