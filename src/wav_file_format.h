/** 
 * Copyright (c) 2026 Mark Ruccius
 * Licensed under the MIT License.
 */

#ifndef WAV_FILE_FORMAT_H
#define WAV_FILE_FORMAT_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct {
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
    uint8_t *data;
} Data_Chunk;

typedef struct {
    RIFF_Header riff_header;
    Format_Chunk format_chunk;
    Data_Chunk data_chunk;
} WAV_File;

/**
 * DEBUGGING FUNCTION:
 * Print the contents of the struct WAV_File
 * 
 * @param wav_file pointer to struct WAV_File
 */
void print_wav_file(WAV_File *wav_file);

/**
 * WRAPPER FUNCTION:
 * Performs fread.
 * Print error message in case of failure to stderr
 */
size_t safe_fread(void *__restrict __ptr, size_t __size, size_t __n, FILE *__restrict __stream);

/**
 * WRAPPER FUNCTION:
 * Performs fwrite.
 * Print error message in case of failure to stderr
 */
size_t safe_fwrite(const void *__restrict __ptr, size_t __size, size_t __n, FILE *__restrict __s);


/**
 * Find a certain chunk in an RIFF file with the given id
 * @param file Filepointer to RIFF file
 * @param id ID if the chunk, containing 4 ASCII cahracters and a nullbyte
 * @return non zero value for error or if chunk is not found
 */
int find_chunk(FILE *file, char id[5]);

WAV_File *parse_data_to_wav(FILE *file, long file_size);

void free_wav_file(WAV_File *file);

FILE *parse_wav_to_data(FILE *file, WAV_File *wav_file);

#endif