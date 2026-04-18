#include "tools.h"

void print_usage() {
    // TODO: print, how to use the programm
}

void bit_shift(uint8_t *buffer, uint64_t buffer_size) {
    if(buffer == NULL || buffer_size <= WAV_HEADER_LENGTH) return;
    for(uint64_t i = WAV_HEADER_LENGTH; i < buffer_size; i++) {
        buffer[i] >>= 1;
    }
}

void invert(uint8_t *buffer, uint64_t buffer_size) {
    if(buffer == NULL || buffer_size <= WAV_HEADER_LENGTH) return;
    for(uint64_t i = WAV_HEADER_LENGTH; i < buffer_size; i++) {
        buffer[i] = !buffer[i];
        //buffer[i] ^= 0xF0;
    }
}

void reverse(uint8_t *buffer, uint64_t buffer_size) {
    if(buffer == NULL || buffer_size <= WAV_HEADER_LENGTH) return;
    for(uint64_t i = WAV_HEADER_LENGTH; i < (buffer_size-WAV_HEADER_LENGTH)/2; i++) {
        uint8_t temp = buffer[i];
        buffer[i] = buffer[buffer_size + WAV_HEADER_LENGTH - i];
        buffer[buffer_size + WAV_HEADER_LENGTH - i] = temp;
    }
}