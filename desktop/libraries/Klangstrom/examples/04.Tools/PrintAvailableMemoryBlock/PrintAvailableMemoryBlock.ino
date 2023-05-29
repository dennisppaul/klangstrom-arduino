#include "Klangstrom.h"

void setup() {
    Serial.begin(115200);
    Serial.println("-------------------------");
    Serial.println("PrintAvailableMemoryBlock");
    Serial.println("-------------------------");
}

void print_available_memory_block(uint32_t size_available_memory_block) {
    Serial.print("largest available memory block: ");
    Serial.print(size_available_memory_block);
    Serial.print(" bytes");
    Serial.println();
}

uint32_t test_memory(uint8_t* mem, uint32_t size_available_memory_block) {
    for (size_t i = 0; i < size_available_memory_block; i++) {
        mem[i] = random();
    }
    uint32_t sum = 0;
    for (size_t i = 0; i < size_available_memory_block; i++) {
        sum += mem[i];
    }
    return sum / size_available_memory_block;
}

void loop() {
    uint32_t size_available_memory_block = klangstrom::query_available_memory_block();
    print_available_memory_block(size_available_memory_block);

    /*
     * the function returns the *guaranteed* largest available memory block in heap memory at
     * the time of the call. the actual largest available memory block may be larger likely
     * twice the size of the returned value. heap memory is allocated dynamically with the `new`
     * keyword.
     *
     * on KLST_SHEEP the largest available heap memory block is 262532 bytes in this particular
     * application ( found by trial and error ).
     *
     * note, that there is also stack memory ( or global dynamic memory ). when allocating memory
     * on KLST_SHEEP the largest available memory block is 517076 bytes. stack memory is allocated
     * with `uint8_t mem[517076];`. to change the location of the allocated memory use the following
     * attribute e.g for `sample_buffer` `__attribute__((section(".sample_buffer")))`.
     */

    uint8_t* mem    = new uint8_t[size_available_memory_block];
    uint32_t result = test_memory(mem, size_available_memory_block);
    Serial.print("result: ");
    Serial.println(result);
    delete[] mem;

    Serial.println("---");
    delay(1000);
}
