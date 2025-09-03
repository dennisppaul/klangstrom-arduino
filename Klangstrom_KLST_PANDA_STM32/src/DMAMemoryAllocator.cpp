/*
* Klangstrom
*
* This file is part of the *Klangstrom* library (https://github.com/dennisppaul/klangstrom-libraries).
* Copyright (c) 2024 Dennis P Paul.
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

#include "DMAMemoryAllocator.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef KLST_DMA_POOL_SIZE
#define KLST_MAX_DMA_POOL_SIZE (16384) // 16KB as defined in linker script for RAM_D3 : ORIGIN = 0x38000000
#define KLST_DMA_POOL_SIZE KLST_MAX_DMA_POOL_SIZE // (2048) // 128 blocksize * 2 double buffer * 2 channels * 2 bytes per sample
#endif

#ifndef KLST_DMA_SECTION_NAME
#define KLST_DMA_SECTION_NAME ".dma_buffer"
#endif

uint8_t dma_memory_pool[KLST_DMA_POOL_SIZE]
    __attribute__((section(KLST_DMA_SECTION_NAME)))
    __attribute__((aligned(32)));

typedef struct {
    uint8_t* base;
    uint8_t* next;
    size_t   size;
} MemoryPool;

static MemoryPool dma_pool = {
    .base = dma_memory_pool,
    .next = dma_memory_pool,
    .size = KLST_DMA_POOL_SIZE};

void* dma_malloc(const size_t size) {
    /* align the next pointer to the nearest 32-byte boundary */
    dma_pool.next = reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(dma_pool.next + 31) & ~static_cast<uintptr_t>(31));

    if ((dma_pool.next + size) > (dma_pool.base + dma_pool.size)) {
        /* Not enough memory in the pool */
        return nullptr;
    }

    void* ptr = dma_pool.next;
    dma_pool.next += size;
    return ptr;
}

void dma_free_all() {
    dma_pool.next = dma_pool.base;
}

size_t dma_pool_size() {
    return dma_pool.size;
}

size_t dma_pool_used() {
    return dma_pool.next - dma_pool.base;
}

size_t dma_pool_available() {
    return dma_pool.size - dma_pool_used();
}

#ifdef __cplusplus
}
#endif

//// EXAMPLE:
//
//#define I2S_BUFFER_SIZE 256 // Example size
//
//uint32_t* dma_TX_buffer;
//
//void init_dma_buffer() {
//    dma_TX_buffer = (uint32_t*) dma_malloc(I2S_BUFFER_SIZE * sizeof(uint32_t));
//    if (dma_TX_buffer == NULL) {
//        // Handle allocation failure
//    }
//}
//
//int main() {
//    init_dma_buffer();
//    // Use dma_TX_buffer as needed
//
//    // reset the memory pool if needed
//    dma_free_all();
//
//    return 0;
//}
