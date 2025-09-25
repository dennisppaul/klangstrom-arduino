#include <iostream>

#include "Arduino.h"
#include "Klangstrom.h"
#include "KlangstromSerialDebug.h"
#include "KlangstromEnvironment.h"

SerialDebug console;
Klangstrom  klangstrom;

void setup() {
    console.init();
}

void loop() {
#ifdef KLST_ARCH_IS_EMU
    std::cout << "abs(-5) = " << abs(-5) << std::endl;
    std::cout << "constrain(10, 0, 5) = " << constrain(10, 0, 5) << std::endl;
    std::cout << "map(5, 0, 10, 0, 100) = " << map(5, 0, 10, 0, 100) << std::endl;
    std::cout << "max(10, 20) = " << max(10, 20) << std::endl;
    std::cout << "min(10, 20) = " << min(10, 20) << std::endl;
    std::cout << "pow(2, 3) = " << pow(2, 3) << std::endl;
    std::cout << "sq(5) = " << sq(5) << std::endl;
    std::cout << "sqrt(25) = " << sqrt(25) << std::endl;
    std::cout << "cos(0) = " << cos(0) << std::endl;
    std::cout << "sin(0) = " << sin(0) << std::endl;
    std::cout << "tan(0) = " << tan(0) << std::endl;

    std::cout << "Random number between 0 and 99: " << random(100) << std::endl;
    std::cout << "Random number between 10 and 20: " << random(10, 21) << std::endl;

    char testChar = 'A';
    std::cout << "isAlpha('A') = " << isAlpha(testChar) << std::endl;
    std::cout << "isAlphaNumeric('A') = " << isAlphaNumeric(testChar) << std::endl;
    std::cout << "isAscii('A') = " << isAscii(testChar) << std::endl;
    std::cout << "isControl('A') = " << isControl(testChar) << std::endl;
    std::cout << "isDigit('A') = " << isDigit(testChar) << std::endl;
    std::cout << "isGraph('A') = " << isGraph(testChar) << std::endl;
    std::cout << "isHexadecimalDigit('A') = " << isHexadecimalDigit(testChar) << std::endl;
    std::cout << "isLowerCase('A') = " << isLowerCase(testChar) << std::endl;
    std::cout << "isPrintable('A') = " << isPrintable(testChar) << std::endl;
    std::cout << "isPunct('A') = " << isPunct(testChar) << std::endl;
    std::cout << "isSpace('A') = " << isSpace(testChar) << std::endl;
    std::cout << "isUpperCase('A') = " << isUpperCase(testChar) << std::endl;
    std::cout << "isWhitespace('A') = " << isWhitespace(testChar) << std::endl;

    uint8_t value = 0b10101010;
    std::cout << "After bitClear(value, 1): " << std::bitset<8>(bitClear(value, 1)) << std::endl;
    std::cout << "bitRead(value, 2) = " << std::bitset<8>(bitRead(value, 2)) << std::endl;
    std::cout << "After bitSet(value, 1): " << std::bitset<8>(bitSet(value, 1)) << std::endl;
    std::cout << "After bitWrite(value, 2, false): " << std::bitset<8>(bitWrite(value, 2, false)) << std::endl;
    std::cout << "After bitWrite(value, 2, true): " << std::bitset<8>(bitWrite(value, 2, true)) << std::endl;
    uint16_t number = 0xABCD;
    std::cout << "highByte(0xABCD) = " << std::bitset<8>(highByte(number)) << std::endl;
    std::cout << "lowByte(0xABCD) = " << std::bitset<8>(lowByte(number)) << std::endl;

    std::cout << std::endl
              << std::flush;
#else
    console.println("this sketch is KLST_EMU only");
#endif // KLST_ARCH_IS_EMU

    delay(1000);
}
