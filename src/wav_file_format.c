#include "wav_file_format.h"

int find_chunk(FILE *file, char find_id[5]) {
    char id[4] = {0};
    if(fread(id, 1, 4, file) != 4) {
        fprintf(stderr, "fread error: %s\n", strerror(errno));
        return -1;
    }
    while(strncmp(id, find_id, 4) != 0) {
        // read size
        uint32_t size = 0;
        if(fread(&size, 4, 1, file) != 4) {
            fprintf(stderr, "fread error: %s\n", strerror(errno));
            return -1;
        }
        // go to end of chunk
        if(fseek(file, size, SEEK_CUR) != 0) {
            fprintf(stderr, "fseek error: %s\n", strerror(errno));
            return -1;
        }
        // read new chunk id
        if(fread(id, 1, 4, file) != 4) {
            fprintf(stderr, "fread error: %s\n", strerror(errno));
            return -1;
        }
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
    if(fread(wav_file->riff_header.chunkID, 4, 1, file) != 4) {
        fprintf(stderr, "fread error: %s\n", strerror(errno));
        return NULL;
    }
    if(fread(&wav_file->riff_header.chunkSize, 4, 1, file) != 4) {
        fprintf(stderr, "fread error: %s\n", strerror(errno));
        return NULL;
    }
    if(fread(wav_file->riff_header.riffType, 4, 1, file) != 4) {
        fprintf(stderr, "fread error: %s\n", strerror(errno));
        return NULL;
    }

    // Parse format chunk
    if(find_chunk(file, "fmt ") != 0) {
        fprintf(stderr, "find_chunk error: fmt\n");
        return NULL;
    }
    if(fread(&wav_file->format_chunk.chunkSize, 4, 1, file) != 4) {
        fprintf(stderr, "fread error: %s\n", strerror(errno));
        return NULL;
    }
    if(fread(&wav_file->format_chunk.wFormatTag, 2, 1, file) != 2) {
        fprintf(stderr, "fread error: %s\n", strerror(errno));
        return NULL;
    }
    if(fread(&wav_file->format_chunk.wChannels, 2, 1, file) != 2) {
        fprintf(stderr, "fread error: %s\n", strerror(errno));
        return NULL;
    }
    if(fread(&wav_file->format_chunk.dwSamplesPerSec, 4, 1, file) != 4) {
        fprintf(stderr, "fread error: %s\n", strerror(errno));
        return NULL;
    }
    if(fread(&wav_file->format_chunk.dwAvgBytesPerSec, 4, 1, file) != 4) {
        fprintf(stderr, "fread error: %s\n", strerror(errno));
        return NULL;
    }
    if(fread(&wav_file->format_chunk.wBlockAlign, 2, 1, file) != 2) {
        fprintf(stderr, "fread error: %s\n", strerror(errno));
        return NULL;
    }
    if(fread(&wav_file->format_chunk.wBitsPerSample, 2, 1, file) != 2) {
        fprintf(stderr, "fread error: %s\n", strerror(errno));
        return NULL;
    }

    // Parse data chunk
    if(find_chunk(file, "data") != 0) {
        fprintf(stderr, "find_chunk error: data\n");
        return NULL;
    }
    if(fread(&wav_file->data_chunk.chunkSize, 4, 1, file) != 4) {
        fprintf(stderr, "fread error: %s\n", strerror(errno));
        return NULL;
    }
    wav_file->data_chunk.data = malloc(wav_file->data_chunk.chunkSize);
    if(wav_file->data_chunk.data == NULL) {
        fprintf(stderr, "malloc error: %s\n", strerror(errno));
        return NULL;
    }
    if(fread(wav_file->data_chunk.data, 1, wav_file->data_chunk.chunkSize, file) != wav_file->data_chunk.chunkSize) {
        fprintf(stderr, "fread error: %s\n", strerror(errno));
        return NULL;
    }

    return wav_file;
}

void free_wav_file(WAV_File *file) {
    free(file->data_chunk.data);
    free(file);
}

/*
uint8_t *parse_wav_to_data(WAV_File *file) {
    return 0;
}
*/