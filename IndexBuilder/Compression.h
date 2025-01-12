/* Compression.h */
#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <stdlib.h>
#include <stdint.h>

/* Function prototypes */
int computeVarByteLength(int docId);    // Compute the number of bytes required to store docId using VByte encoding
size_t varByteCompressInt(uint32_t docId, uint8_t *byteBuffer);    // Compress docId using VByte encoding
uint8_t logCompressDouble(double impactScore);  // Compress impactScore using logarithmic encoding (1 byte)

#endif
