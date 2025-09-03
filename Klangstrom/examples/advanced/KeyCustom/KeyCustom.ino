/**
 * this example demonstrates how to use the programmer button as a custom key with KLST_PANDA on STM32.
*/

#include "main.h" // this must be first include

#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "Key.h"
#include "Key_STM32.h"

Key* key_CUSTOM;

void setup() {
    system_init();

    key_init();

    // NOTE enable `#define KLST_ENABLE_FIRE_EVENT_FOR_ALL_KEYS` in `KlangstromConfigurationCustom.h`
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin  = _BUTTON_PROGRAMMER_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(_BUTTON_PROGRAMMER_GPIO_Port, &GPIO_InitStruct);

    key_CUSTOM                         = key_create(KEY_TYPE_CUSTOM);
    key_CUSTOM->peripherals->gpio_port = _BUTTON_PROGRAMMER_GPIO_Port;
    key_CUSTOM->peripherals->gpio_pin  = _BUTTON_PROGRAMMER_Pin;
}

void loop() {}

void key_event(const Key* key) {
    if (key->device_id == key_CUSTOM->device_id) {
        console_println("Key CUSTOM: %i", key->pressed);
    }
}
