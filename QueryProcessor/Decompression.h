/* Decompression.h */
#ifndef DECOMPRESSION_H
#define DECOMPRESSION_H

#include "InvertedList.h"
#include <stdint.h>

/* Function prototypes */
void decompressPostings(InvertedList *invertedList);    // Decompress all postings in the current loaded chunk
uint32_t varByteDecompressInt(uint8_t **byteBuffer);    // Decompress a VByte encoded integer
double logDecompressToDouble(uint8_t **byteBuffer);    // Decompress a log-encoded double

#endif
