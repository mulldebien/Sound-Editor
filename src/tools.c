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

int invert_bitwise(WAV_File *wav_file) {
    for(uint32_t i = 0; i < wav_file->data_chunk.chunkSize; i++) {
        wav_file->data_chunk.data[i] = (0x00)+(i/4);
        // wav_file->data_chunk.data[i] = ~wav_file->data_chunk.data[i];
    }

    return 0;
}

int reverse(WAV_File* wav_file) {
    // Deklare and Initialize common variables
    uint32_t data_chunk_size = wav_file->data_chunk.chunkSize;
    uint16_t block_align = wav_file->format_chunk.wBlockAlign;
    uint8_t *data = wav_file->data_chunk.data;
    size_t num_sample_frames = data_chunk_size/block_align;
    uint8_t *temp = malloc(block_align*sizeof(uint8_t*));
    if(temp == NULL) {
        fprintf(stderr, "malloc error: %s\n", strerror(errno));
        return -1;
    }

    // Reverse sample frames
    for(size_t i = 0; i < num_sample_frames/2; i++) {
        memcpy(temp, &data[i*block_align], block_align);
        memcpy(&data[i*block_align], &data[(num_sample_frames-i-1)*block_align], block_align);
        memcpy(&data[(num_sample_frames-i-1)*block_align], temp, block_align);
    }
    
    // Cleanup
    free(temp);
    return 0;
}