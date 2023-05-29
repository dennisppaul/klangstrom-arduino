/*
 * Klangstrom
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2023 Dennis P Paul.
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _klangstrom_display_CLI_
#define _klangstrom_display_CLI_

#include "KlangstromDisplay.h"
#include "KlangstromDisplayFont.h"
#include "KlangstromDisplayTerminal.h"

#define EMBEDDED_CLI_IMPL
#include "embedded_cli.h"

namespace klangstrom {

    class KlangstromDisplayCLI {
    public:
        static KlangstromDisplayCLI &create(KlangstromDisplay     *pDisplay,
                                            KlangstromDisplayFont *pFont) {
            return *(new KlangstromDisplayCLI(pDisplay, pFont));
        }
        KlangstromDisplayCLI(KlangstromDisplay     *pDisplay,
                             KlangstromDisplayFont *pFont) {
            mTerminal         = new KlangstromDisplayTerminal(pDisplay, pFont);
            mTerminal->width  = pDisplay->width() / pFont->width;
            mTerminal->height = pDisplay->height() / pFont->height;
            mTerminal->x      = (pDisplay->width() - mTerminal->width * pFont->width) / 2;
            mTerminal->y      = (pDisplay->height() - mTerminal->height * pFont->height) / 2;
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
        }

        ~KlangstromDisplayCLI() {
            embeddedCliFree(cli);
        }

        void background(uint8_t r, uint8_t g, uint8_t b) {
            mTerminal->background_color.set(r, g, b);
        }

        void color(uint8_t r, uint8_t g, uint8_t b) {
            mTerminal->foreground_color.set(r, g, b);
        }

        /**
         * navigates through history: older commands
         */
        void send_history_back() {
            receive_char('\e');
            receive_char('[');
            receive_char('A');
        }

        /**
         * navigates through history: newer commands
         */
        void send_history_forward() {
            receive_char('\e');
            receive_char('[');
            receive_char('B');
        }

        /**
         * removes last typed character
         */
        void send_delete_character() {
            receive_char('\b');
        }

        /**
         * complete command
         */
        void send_complete_command() {
            receive_char('\t');
        }

        /**
         * removes last typed character
         */
        void send_return() {
            receive_char('\n');
        }

        bool add_binding(CliCommandBinding binding) {
            return embeddedCliAddBinding(cli, binding);
        }

        void process() {
            if (cli != NULL) {
                embeddedCliProcess(cli);
            }
        }

        void println() {
            mTerminal->println();
        }

        void println(const char *s) {
            mTerminal->println(s);
        }

        void print(const char *s) {
            mTerminal->print(s);
        }

        void print(const char c) {
            mTerminal->print(c);
        }

        void print_to(uint8_t pCursorX, uint8_t pCursorY, char c) {
            mTerminal->print_to(pCursorX, pCursorY, c);
        }

        void clear() {
            mTerminal->clear();
        }

        void set_position_x(uint16_t x) {
            mTerminal->x = x;
        }

        void set_position_y(uint16_t y) {
            mTerminal->y = y;
        }

        void set_position(uint16_t x, uint16_t y) {
            mTerminal->x = x;
            mTerminal->y = y;
        }

        void set_width(uint8_t width) {
            mTerminal->width = width;
        }

        void set_height(uint8_t height) {
            mTerminal->height = height;
        }

        void set_dimensions(uint8_t width, uint8_t height) {
            mTerminal->width  = width;
            mTerminal->height = height;
        }

        uint8_t get_width() {
            return mTerminal->width;
        }

        uint8_t get_height() {
            return mTerminal->height;
        }

        uint16_t get_position_x() {
            return mTerminal->x;
        }

        uint16_t get_position_y() {
            return mTerminal->y;
        }

        void receive_char(char c) {
            if (cli != NULL) {
                embeddedCliReceiveChar(cli, c);
            }
        }

        void print_help() {
            if (cli != NULL) {
                onHelp(cli, nullptr, nullptr);
            }
        }

        void set_write_char_callback(void (*writeChar)(EmbeddedCli *, char)) {
            cli->writeChar = writeChar;
        }

        void set_on_command_callback(void (*onCommand)(EmbeddedCli *, CliCommand *)) {
            cli->onCommand = onCommand;
        }

        void tokenize_args(char *args) {
            embeddedCliTokenizeArgs(args);
        }

        const char *get_token(const char *tokenizedStr, uint16_t pos) {
            return embeddedCliGetToken(tokenizedStr, pos);
        }

        uint16_t get_token_count(const char *tokenizedStr) {
            return embeddedCliGetTokenCount(tokenizedStr);
        }

        uint16_t find_token(const char *tokenizedStr, const char *token) {
            return embeddedCliFindToken(tokenizedStr, token);
        }

        char *get_token_variable(char *tokenizedStr, uint16_t pos) {
            return embeddedCliGetTokenVariable(tokenizedStr, pos);
        }

    private:
        KlangstromDisplayTerminal *mTerminal;
        EmbeddedCli               *cli;
    };
}  // namespace klangstrom

#endif  // _klangstrom_display_CLI_