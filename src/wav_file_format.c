/** 
 * Copyright (c) 2026 Mark Ruccius
 * Licensed under the MIT License.
 */

#include "wav_file_format.h"

void print_wav_file(WAV_File *wav_file) {
    printf("RIFF HEADER.\n");
    printf("%d, %.4s\n\n", wav_file->riff_header.chunkSize, wav_file->riff_header.riffType);
    printf("FORMAT HEADER:\n");
    printf("%d, %d, %d, %d, %d, %d, %d\n\n", wav_file->format_chunk.chunkSize, wav_file->format_chunk.wFormatTag, wav_file->format_chunk.wChannels, wav_file->format_chunk.dwSamplesPerSec, wav_file->format_chunk.dwAvgBytesPerSec, wav_file->format_chunk.wBlockAlign, wav_file->format_chunk.wBitsPerSample);
    printf("DATA HEADER:\n");
    printf("%d\n\n", wav_file->data_chunk.chunkSize);
}

size_t safe_fread(void *__restrict __ptr, size_t __size, size_t __n, FILE *__restrict __stream) {
    size_t ret = fread(__ptr, __size, __n, __stream);
    if(ret != __n) {
        fprintf(stderr, "fread error: %s\n", strerror(errno));
    }
    return ret;
}

size_t safe_fwrite(const void *__restrict __ptr, size_t __size, size_t __n, FILE *__restrict __s) {
    size_t ret = fwrite(__ptr, __size, __n, __s);
    if(ret != __n) {
        fprintf(stderr, "fwrite error: %s\n", strerror(errno));
    }
    return ret;
}

int find_chunk(FILE *file, char find_id[5]) {
    char id[4] = {0};
    if(safe_fread(id, 1, 4, file) != 4) return -1;
    while(strncmp(id, find_id, 4) != 0) {
        // read size
        uint32_t size = 0;
        if(safe_fread(&size, 4, 1, file) != 1) return -1;
        // go to end of chunk
        if(fseek(file, size, SEEK_CUR) != 0) {
            fprintf(stderr, "fseek error: %s\n", strerror(errno));
            return -1;
        }
        // read new chunk id
        if(safe_fread(id, 1, 4, file) != 4) return -1;
    }
    return 0;
}

WAV_File *parse_data_to_wav(FILE *file, long file_size) {
    if(file_size < 36) return NULL;
    WAV_File *wav_file = (WAV_File*)malloc(sizeof(WAV_File));
    if(wav_file == NULL) {
        fprintf(stderr, "malloc error: %s\n", strerror(errno));
        return NULL;
    }

    // Parse RIFF header
    if(find_chunk(file, "RIFF") != 0) {
        fprintf(stderr, "find_chunk error: RIFF\n");
        return NULL;
    }
    if(safe_fread(&wav_file->riff_header.chunkSize, 4, 1, file) != 1) return NULL;
    if(safe_fread(wav_file->riff_header.riffType, 1, 4, file) != 4) return NULL;

    // Parse format chunk
    if(find_chunk(file, "fmt ") != 0) {
        fprintf(stderr, "find_chunk error: fmt \n");
        return NULL;
    }
    if(safe_fread(&wav_file->format_chunk.chunkSize, 4, 1, file) != 1) return NULL;
    if(safe_fread(&wav_file->format_chunk.wFormatTag, 2, 1, file) != 1) return NULL;
    if(safe_fread(&wav_file->format_chunk.wChannels, 2, 1, file) != 1) return NULL;
    if(safe_fread(&wav_file->format_chunk.dwSamplesPerSec, 4, 1, file) != 1) return NULL;
    if(safe_fread(&wav_file->format_chunk.dwAvgBytesPerSec, 4, 1, file) != 1) return NULL;
    if(safe_fread(&wav_file->format_chunk.wBlockAlign, 2, 1, file) != 1) return NULL;
    if(safe_fread(&wav_file->format_chunk.wBitsPerSample, 2, 1, file) != 1) return NULL;

    // Parse data chunk
    if(find_chunk(file, "data") != 0) {
        fprintf(stderr, "find_chunk error: data\n");
        return NULL;
    }
    if(safe_fread(&wav_file->data_chunk.chunkSize, 4, 1, file) != 1) return NULL;
    wav_file->data_chunk.data = malloc(wav_file->data_chunk.chunkSize);
    if(wav_file->data_chunk.data == NULL) {
        fprintf(stderr, "malloc error: %s\n", strerror(errno));
        return NULL;
    }
    if(safe_fread(wav_file->data_chunk.data, 1, wav_file->data_chunk.chunkSize, file) != wav_file->data_chunk.chunkSize) return NULL;
    return wav_file;
}

void free_wav_file(WAV_File *file) {
    free(file->data_chunk.data);
    free(file);
}

FILE *parse_wav_to_data(FILE *file, WAV_File *wav_file) {
    if(file == NULL || wav_file == NULL) return NULL;

    // Write RIFF header
    if(safe_fwrite("RIFF", 1, 4, file) != 4) return NULL;
    uint32_t new_riff_header_chunkSize = wav_file->data_chunk.chunkSize+36;
    if(safe_fwrite(&new_riff_header_chunkSize, 4, 1, file) != 1) return NULL;
    if(safe_fwrite(wav_file->riff_header.riffType, 1, 4, file) != 4) return NULL;

    // Write format chunk
    if(safe_fwrite("fmt ", 1, 4, file) != 4) return NULL;
    if(safe_fwrite(&wav_file->format_chunk.chunkSize, 4, 1, file) != 1) return NULL;
    if(safe_fwrite(&wav_file->format_chunk.wFormatTag, 2, 1, file) != 1) return NULL;
    if(safe_fwrite(&wav_file->format_chunk.wChannels, 2, 1, file) != 1) return NULL;
    if(safe_fwrite(&wav_file->format_chunk.dwSamplesPerSec, 4, 1, file) != 1) return NULL;
    if(safe_fwrite(&wav_file->format_chunk.dwAvgBytesPerSec, 4, 1, file) != 1) return NULL;
    if(safe_fwrite(&wav_file->format_chunk.wBlockAlign, 2, 1, file) != 1) return NULL;
    if(safe_fwrite(&wav_file->format_chunk.wBitsPerSample, 2, 1, file) != 1) return NULL;

    // Write data chunk
    if(safe_fwrite("data", 1, 4, file) != 4) return NULL;
    if(safe_fwrite(&wav_file->data_chunk.chunkSize, 4, 1, file) != 1) return NULL;
    if(safe_fwrite(wav_file->data_chunk.data, 1, wav_file->data_chunk.chunkSize, file) != wav_file->data_chunk.chunkSize) return NULL;

    free_wav_file(wav_file);

    return file;
}