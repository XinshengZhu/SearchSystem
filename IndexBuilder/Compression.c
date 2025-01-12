/* Compression.c */
#include "Compression.h"
#include <math.h>

/**
 * Computes the number of bytes needed to encode a document ID using variable-byte encoding
 *
 * Variable-byte encoding uses 7 bits per byte for data and 1 bit as continuation flag.
 * Each byte except the last has its highest bit set to 1, indicating more bytes follow.
 * The last byte has its highest bit set to 0, indicating the end of the encoded number.
 *
 * @param docId The document ID to be encoded
 * @return Number of bytes needed for encoding
 */
int computeVarByteLength(int docId) {
    int byteCount = 0;
    // Keep shifting right by 7 bits until we've processed all significant bits
    while (docId >= 128) {  // 128 = 2^7, need another byte if value >= 128
        docId >>= 7;
        byteCount++;
    }
    return byteCount + 1;   // Add 1 for the last byte
}

/**
 * Compresses an integer using variable-byte encoding
 *
 * Uses 7 bits per byte for data and 1 bit as continuation flag.
 * Each byte except the last has its highest bit set to 1.
 * The last byte has its highest bit set to 0.
 *
 * @param docId Document ID to compress
 * @param byteBuffer Buffer to store compressed bytes
 * @return Number of bytes written to buffer
 *
 * Example:
 *   docId = 130 (10000010 in binary)
 *   Encoded as: [10000010, 00000001]
 *   Where:
 *     First byte: 1|0000010 (continuation bit|7 data bits)
 *     Second byte: 0|0000001 (end bit|7 data bits)
 *   When decoded: 0000001|0000010 = 130
 */
size_t varByteCompressInt(uint32_t docId, uint8_t *byteBuffer) {
    size_t byteCount = 0;
    // Process 7 bits at a time, setting continuation bit
    while (docId >= 128) {
        byteBuffer[byteCount] = (docId & 127) | 128;    // Keep 7 bits and set high bit to 1
        docId >>= 7;    // Move to next 7 bits
        byteCount++;
    }
    // Write final byte (high bit is 0)
    byteBuffer[byteCount] = docId;
    return byteCount + 1;
}

/**
 * Compresses a double value using logarithmic compression
 *
 * Uses log2(x+1) transformation followed by scaling to fit in a byte.
 * The scaling factor 36.06 is chosen to maximize precision while fitting
 * typical impact scores in a single byte.
 *
 * @param impactScore Score value to compress (must be non-negative)
 * @return Compressed value as a single byte
 *
 * Compression process:
 * 1. Add 1 to handle values between 0 and 1
 * 2. Take log base 2 to compress dynamic range
 * 3. Scale by 36.06 to maximize use of byte range
 * 4. Cast to byte (0-255)
 *
 * Example ranges:
 * - Input 0.0 -> Output 0
 * - Input 1.0 -> Output ~36
 * - Input 7.0 -> Output ~108
 */
uint8_t logCompressDouble(double impactScore) {
    if (impactScore <= 0.0) {
        return 0;   // Handle zero or negative values
    }
    double logImpactScore = log2(impactScore + 1);  // Add 1 to handle values between 0 and 1
    uint8_t compressed = (uint8_t)(logImpactScore * 36.06); // Scale by 36.06 to maximize use of byte range
    return compressed;
}
