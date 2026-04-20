/** 
 * Copyright (c) 2026 Mark Ruccius
 * Licensed under the MIT License.
 */

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

void reverse(WAV_File* wav_file) {
    uint8_t *temp_ptr = (uint8_t*)wav_file->data_chunk.data;
    for(uint32_t i = 0; i < wav_file->data_chunk.chunkSize/2; i++) {
        //swap
        uint8_t temp = temp_ptr[i];
        temp_ptr[i] = temp_ptr[wav_file->data_chunk.chunkSize-i];
        temp_ptr[wav_file->data_chunk.chunkSize] = temp;
    }
}