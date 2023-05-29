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

uint32_t test_memory_allocation(uint8_t* mem, uint32_t size_available_memory_block) {
    for (size_t i = 0; i < size_available_memory_block; i++) {
        mem[i] = random();
    }
    uint32_t sum = 0;
    for (size_t i = 0; i < size_available_memory_block; i++) {
        sum += mem[i];
    }
    return sum / size_available_memory_block;
}

uint8_t memory_RAM_D1[517076] __attribute__((section(".data")))              = {0};  // RAM_D1 :: 512K > 524288 bytes - 7212 bytes ( HEAP + STACK ) = 517076 bytes
uint8_t memory_RAM_D2[294912] __attribute__((section(".sample_buffer")))     = {0};  // RAM_D2 :: 288K > 294912 bytes
uint8_t memory_RAM_D3[59392] __attribute__((section(".audio_block_buffer"))) = {0};  // RAM_D3 :: 64K > 65536 bytes - 6144 bytes ( DMA BUFFER ) = 59392 bytes
/* note that this allocation scheme results into a total of 871380 bytes ( 850K ) separated into 3
 * memory blocks. also note that the `.data` section is the default location for global variables.
 * the section attributes `.sample_buffer` and `.audio_block_buffer` are required to use other 
 * memory locations.
 */

void loop() {
    uint32_t size_available_memory_block = klangstrom::query_available_memory_block();
    print_available_memory_block(size_available_memory_block);

    size_available_memory_block /= 4; /* the algorithm seems to be a bit too optimistic regarding the available size */
    uint8_t* mem                = new uint8_t[size_available_memory_block];
    uint32_t memory_heap_result = test_memory_allocation(mem, size_available_memory_block);
    Serial.print("memory_heap_result  : ");
    Serial.println(memory_heap_result);
    delete[] mem;

    uint32_t memory_RAM_D1_result = test_memory_allocation(memory_RAM_D1, 517076);
    Serial.print("memory_RAM_D1_result: ");
    Serial.println(memory_RAM_D1_result);
    uint32_t memory_RAM_D2_result = test_memory_allocation(memory_RAM_D2, 294912);
    Serial.print("memory_RAM_D2_result: ");
    Serial.println(memory_RAM_D2_result);
    uint32_t memory_RAM_D3_result = test_memory_allocation(memory_RAM_D3, 59392);
    Serial.print("memory_RAM_D3_result: ");
    Serial.println(memory_RAM_D3_result);

    Serial.println("---");
    delay(1000);
}
