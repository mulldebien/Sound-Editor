#ifndef SOUND_EDITOR_H
#define SOUND_EDITOR_H

#define WAV_HEADER_LENGTH 100

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

void print_usage() {
    // TODO: print, how to use the programm
}

void bit_shift(uint8_t *buffer, uint64_t buffer_size) {
    if(buffer == NULL || buffer_size <= WAV_HEADER_LENGTH) return;
    for(uint64_t i = WAV_HEADER_LENGTH+1; i < buffer_size; i++) {
        buffer[i] >>= 1;
    }
}

void invert(uint8_t *buffer, uint64_t buffer_size) {
    if(buffer == NULL || buffer_size <= WAV_HEADER_LENGTH) return;
    for(uint64_t i = WAV_HEADER_LENGTH+1; i < buffer_size; i++) {
        buffer[i] = !buffer[i];
        //buffer[i] ^= 0xF0;
    }
}

#endif