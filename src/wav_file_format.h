#ifndef WAV_FILE_FORMAT_H
#define WAV_FILE_FORMAT_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct {
    char chunkID[4];
    uint32_t chunkSize;
    char riffType[4];
} RIFF_Header;

typedef struct {
    uint32_t chunkSize;
    uint16_t wFormatTag;
    uint16_t wChannels;
    uint32_t dwSamplesPerSec;
    uint32_t dwAvgBytesPerSec;
    uint16_t wBlockAlign;
    uint16_t wBitsPerSample;
} Format_Chunk;

typedef struct {
    uint32_t chunkSize;
    void *data;
} Data_Chunk;

typedef struct {
    RIFF_Header riff_header;
    Format_Chunk format_chunk;
    Data_Chunk data_chunk;
} WAV_File;

int find_chunk(FILE *file, char id[5]);

WAV_File *parse_data_to_wav(FILE *file, long file_size);

void free_wav_file(WAV_File *file);

//uint8_t *parse_wav_to_data(WAV_File *file);

#endif