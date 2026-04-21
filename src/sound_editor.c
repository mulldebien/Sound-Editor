/*
 * Copyright (c) 2026 Mark Ruccius
 * Licensed under the MIT License.
 */

#include "tools.h"

int main(int argc, char **argv) {
    // Check for correct arguments
    if(argc != 2) {
        printf("Error: Not enough arguments!\n");
        print_usage();
        return EXIT_FAILURE;
    }

    // Open audiofile
    FILE *audio = fopen(argv[1], "rb");
    if(audio == NULL) {
        fprintf(stderr, "fopen error: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    // Determine file size
    if(fseek(audio, 0, SEEK_END) != 0) {
        fprintf(stderr, "fseek error: %s\n", strerror(errno));
        fclose(audio);
        return EXIT_FAILURE;
    }
    long file_size = ftell(audio);
    if(file_size < 0) {
        fprintf(stderr, "ftell error: %s\n", strerror(errno));
        fclose(audio);
        return EXIT_FAILURE;
    }
    if(fseek(audio, 0, SEEK_SET) != 0) {
        fprintf(stderr, "fseek error: %s\n", strerror(errno));
        fclose(audio);
        return EXIT_FAILURE;
    }

    // Parse file
    WAV_File *wav_file = parse_data_to_wav(audio, file_size);
    if(wav_file == NULL) {
        fprintf(stderr, "parse_data_to_wav error\n");
        fclose(audio);
        return EXIT_FAILURE;
    }
    print_wav_file(wav_file);

    // Change buffer accordingly
    //bit_shift(file_buffer, file_size);
    //invert_bitwise(wav_file);
    if(reverse(wav_file) != 0) {
        fprintf(stderr, "reverse error: %s\n", strerror(errno));
        fclose(audio);
        return EXIT_FAILURE;
    }

    // Create new audiofile and write buffer
    FILE *new_audio = fopen("new_file.wav", "wb");
    if(new_audio == NULL) {
        fprintf(stderr, "fopen error: %s\n", strerror(errno));
        fclose(audio);
        return EXIT_FAILURE;
    }
    if(parse_wav_to_data(new_audio, wav_file) == NULL) {
        fprintf(stderr, "parse_wav_to_data error\n");
        fclose(audio);
        fclose(new_audio);
        return EXIT_FAILURE;
    }

    // Cleanup
    fclose(audio);
    fclose(new_audio);
    return EXIT_SUCCESS;
}