//
// KLST Terminal
//
// @note make sure to select `Tools > USB support (if available) > USB Host Mouse+Keyboard+MIDI`
//

#include "Klangstrom.h"
#include "KlangNodes.hpp"
#include "KlangstromDisplay.h"
#include "KlangstromDisplayFont_7x10.h"
#include "KlangstromDisplayTerminal.h"

#define EMBEDDED_CLI_IMPL
#include "embedded_cli.h"

#include "USBHost.h"

using namespace klang;
using namespace klangstrom;

NodeVCOFunction mVCO;
NodeADSR        mADSR;
NodeDAC         mDAC;

KlangstromDisplay         *Display;
KlangstromDisplayTerminal *mTerminal;

EmbeddedCli *cli;

void setup() {
  Serial.begin(115200);

  usb_host_init();

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
  mTerminal->background_color.set(0, 0, 0);
  mTerminal->foreground_color.set(255, 255, 255);

  EmbeddedCliConfig *config  = embeddedCliDefaultConfig();
  config->enableAutoComplete = false;
  cli                        = embeddedCliNew(config);
  if (cli != NULL) {
    mTerminal->println("--- KLST CLI ---");
  }

  embeddedCliAddBinding(cli, {"hello", "Print hello message", true, (void *)"world", onHello});
  embeddedCliAddBinding(cli, {
    "bing",              // command name (spaces are not allowed)
    "Plays a bing tone", // Optional help for a command (NULL for no help)
    false,               // flag whether to tokenize arguments
    nullptr,             // optional pointer to any application context
    onBing               // binding function
  });
  cli->onCommand = onCommand;
  cli->writeChar = writeChar;

  Klang::lock();

  Klang::connect(mVCO, mADSR);
  Klang::connect(mADSR, mDAC);

  mVCO.set_frequency(220);
  mVCO.set_waveform(NodeVCOFunction::WAVEFORM::SINE);
  mVCO.set_amplitude(0.5);

  mADSR.set_attack(0.01);
  mADSR.set_decay(0.1);
  mADSR.set_sustain(0.01);
  mADSR.set_release(0.1);

  Klang::unlock();
}

void loop() {
  usb_host_process();

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

const static uint8_t KEYCODE_RETURN = 40;
const static uint8_t KEYCODE_ESCAPE = 41;
const static uint8_t KEYCODE_BACKSPACE = 42;
const static uint8_t KEYCODE_TAB = 43;
const static uint8_t KEYCODE_SPACE = 44;
const static uint8_t KEYCODE_DOWN = 81;
const static uint8_t KEYCODE_UP = 82;

void receive_keyboard(uint8_t *keys) {
  if (cli != NULL) {
    // @TODO(improve USB keyboard implementation!)
    if (keys[0] > 0) {
      Serial.println((int)keys[0]);
      if (keys[0] == KEYCODE_RETURN) {
        embeddedCliReceiveChar(cli, '\n');
        embeddedCliReceiveChar(cli, '\r');
      } else if (keys[0] == KEYCODE_ESCAPE) {
        mTerminal->clear();
        embeddedCliPrint(cli, "");
      } else if (keys[0] == KEYCODE_BACKSPACE) {
        embeddedCliReceiveChar(cli, '\b');
      } else if (keys[0] == KEYCODE_TAB) {
        embeddedCliReceiveChar(cli, '\t');
      } else if (keys[0] == KEYCODE_SPACE) {
        embeddedCliReceiveChar(cli, ' ');
      } else if (keys[0] == KEYCODE_DOWN) {
        embeddedCliReceiveChar(cli, '\e');
        embeddedCliReceiveChar(cli, '[');
        embeddedCliReceiveChar(cli, 'B');
      } else if (keys[0] == KEYCODE_UP) {
        embeddedCliReceiveChar(cli, '\e');
        embeddedCliReceiveChar(cli, '[');
        embeddedCliReceiveChar(cli, 'A');
      } else if (keys[0] >= 4 && keys[0] <= 29) {
        char c = (char)(keys[0] + 93);
        embeddedCliReceiveChar(cli, c);
      } else if (keys[0] == 39) {
        embeddedCliReceiveChar(cli, '0');
      } else if (keys[0] >= 30 && keys[0] <= 38) {
        char c = (char)(keys[0] + 19);
        embeddedCliReceiveChar(cli, c);
      }
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

void onBing(EmbeddedCli *cli, char *args, void *context) {
  if (embeddedCliGetTokenCount(args) == 1) {
    const char *arg = embeddedCliGetToken(args, 1);
    float mFreq = strtof(arg, NULL);
    mVCO.set_frequency(mFreq);
  }

  mTerminal->println("bing!");
  mADSR.start();
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

void audioblock(SIGNAL_TYPE* pOutputLeft, SIGNAL_TYPE* pOutputRight, SIGNAL_TYPE* pInputLeft, SIGNAL_TYPE* pInputRight) {
  mDAC.process_frame(pOutputLeft, pOutputRight);
}
