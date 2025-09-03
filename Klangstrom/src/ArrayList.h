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

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DEFINE_ARRAYLIST(type, ext)                                                                  \
    typedef struct ArrayList_##ext {                                                                 \
        type*  data;                                                                                 \
        size_t size;                                                                                 \
        size_t capacity;                                                                             \
    } ArrayList_##ext;                                                                               \
                                                                                                     \
    static inline void arraylist_##ext##_init(ArrayList_##ext* arraylist, size_t initialCapacity) {  \
        arraylist->data = (type*) malloc(initialCapacity * sizeof(type));                            \
        if (arraylist->data == NULL) {                                                               \
            fprintf(stderr, "Memory allocation failed\n");                                           \
            exit(EXIT_FAILURE);                                                                      \
        }                                                                                            \
        arraylist->size     = 0;                                                                     \
        arraylist->capacity = initialCapacity;                                                       \
    }                                                                                                \
                                                                                                     \
    static inline void arraylist_##ext##_resize(ArrayList_##ext* arraylist, size_t newCapacity) {    \
        type* temp = (type*) realloc(arraylist->data, newCapacity * sizeof(type));                   \
        if (temp == NULL) {                                                                          \
            fprintf(stderr, "Memory reallocation failed\n");                                         \
            free(arraylist->data);                                                                   \
            exit(EXIT_FAILURE);                                                                      \
        }                                                                                            \
        arraylist->data     = temp;                                                                  \
        arraylist->capacity = newCapacity;                                                           \
    }                                                                                                \
                                                                                                     \
    static inline void arraylist_##ext##_add(ArrayList_##ext* arraylist, type element) {             \
        if (arraylist->size == arraylist->capacity) {                                                \
            arraylist_##ext##_resize(arraylist, arraylist->capacity * 2);                            \
        }                                                                                            \
        arraylist->data[arraylist->size++] = element;                                                \
    }                                                                                                \
                                                                                                     \
    static inline type arraylist_##ext##_get(ArrayList_##ext* arraylist, size_t index) {             \
        if (index >= arraylist->size) {                                                              \
            fprintf(stderr, "Index out of bounds\n");                                                \
            exit(EXIT_FAILURE);                                                                      \
        }                                                                                            \
        return arraylist->data[index];                                                               \
    }                                                                                                \
                                                                                                     \
    static inline void arraylist_##ext##_set(ArrayList_##ext* arraylist, size_t index, type value) { \
        if (index >= arraylist->size) {                                                              \
            fprintf(stderr, "Index out of bounds\n");                                                \
            exit(EXIT_FAILURE);                                                                      \
        }                                                                                            \
        arraylist->data[index] = value;                                                              \
    }                                                                                                \
                                                                                                     \
    static inline void arraylist_##ext##_free(ArrayList_##ext* arraylist) {                          \
        free(arraylist->data);                                                                       \
        arraylist->data     = NULL;                                                                  \
        arraylist->size     = 0;                                                                     \
        arraylist->capacity = 0;                                                                     \
    }

#ifdef __cplusplus
}
#endif

DEFINE_ARRAYLIST(int, i)
DEFINE_ARRAYLIST(float, f)
DEFINE_ARRAYLIST(uint8_t, ui8)

//// Integer arraylist
//ArrayList_int intArrayList;
//arraylist_int_init(&intArrayList, 4);
//arraylist_int_add(&intArrayList, 10);
//arraylist_int_add(&intArrayList, 20);
//arraylist_int_add(&intArrayList, 30);
//arraylist_int_add(&intArrayList, 40);
//arraylist_int_add(&intArrayList, 50);
//
//for (size_t i = 0; i < intArrayList.size; i++) {
//    printf("%d ", arraylist_int_get(&intArrayList, i));
//}
//printf("\n");
//
//arraylist_int_free(&intArrayList);
//
//// Float arraylist
//ArrayList_float floatArrayList;
//arraylist_float_init(&floatArrayList, 4);
//arraylist_float_add(&floatArrayList, 10.5f);
//arraylist_float_add(&floatArrayList, 20.5f);
//arraylist_float_add(&floatArrayList, 30.5f);
//arraylist_float_add(&floatArrayList, 40.5f);
//arraylist_float_add(&floatArrayList, 50.5f);
//
//for (size_t i = 0; i < floatArrayList.size; i++) {
//    printf("%f ", arraylist_float_get(&floatArrayList, i));
//}
//printf("\n");
//
//arraylist_float_free(&floatArrayList);