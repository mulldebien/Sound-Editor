/*
 * Copyright (c) 2026 Mark Ruccius
 * Licensed under the MIT License.
 */

#ifndef TOOLS_H
#define TOOLS_H

#define WAV_HEADER_LENGTH 80

#include "wav_file_format.h"

void print_usage();

void bit_shift(uint8_t *buffer, uint64_t buffer_size);

void invert(uint8_t *buffer, uint64_t buffer_size);

void reverse(WAV_File* wav_file);

#endif