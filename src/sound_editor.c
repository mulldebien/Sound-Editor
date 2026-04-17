#include "sound_editor.h"

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
    uint64_t file_size = ftell(audio);
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

    // Create buffer and copy file content into buffer
    uint8_t *file_buffer = malloc(file_size);
    if(file_buffer == NULL) {
        fprintf(stderr, "malloc error: %s\n", strerror(errno));
        fclose(audio);
        return EXIT_FAILURE;
    }
    uint64_t read_bytes = fread(file_buffer, 1, file_size, audio);
    if(read_bytes != file_size) {
        fprintf(stderr, "fread error: %s\n", strerror(errno));
        fclose(audio);
        return EXIT_FAILURE;
    }

    // Change buffer accordingly
    //bit_shift(file_buffer, file_size);
    invert(file_buffer, file_size);

    // Create new audiofile and write buffer
    FILE *new_audio = fopen("new_file.wav", "wb");
    if(new_audio == NULL) {
        fprintf(stderr, "fopen error: %s\n", strerror(errno));
        fclose(audio);
        return EXIT_FAILURE;
    }
    uint64_t wrote_bytes = fwrite(file_buffer, 1, file_size, new_audio);
    if(wrote_bytes != file_size) {
        fprintf(stderr, "fwrite error: %s\n", strerror(errno));
        fclose(audio);
        fclose(new_audio);
        return EXIT_FAILURE;
    }

    // Cleanup
    fclose(audio);
    fclose(new_audio);
    free(file_buffer);
    return EXIT_SUCCESS;
}