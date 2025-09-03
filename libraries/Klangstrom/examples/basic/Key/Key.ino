/**
 * this example demonstrates how to use mechanical keys.
*/

#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "Key.h"

Key* key_left;
Key* key_right;

void setup() {
    system_init();

    key_left  = key_create(KEY_LEFT);
    key_right = key_create(KEY_RIGHT);
}

void loop() {}

void key_event(const Key* key) {
    if (key->device_id == key_left->device_id) {
        console_println("left key  : %i", key->pressed);
    }
    if (key->device_id == key_right->device_id) {
        console_println("right key : %i", key->pressed);
    }
}
