/* Decompression.c */
#include "Decompression.h"
#include <math.h>

/**
 * Decompresses a variable-byte encoded integer
 * Each byte uses 7 bits for data and 1 bit for continuation
 *
 * @param byteBuffer Pointer to current position in byte buffer
 * @return Decompressed integer value
 *
 * Format:
 * - High bit 1: More bytes follow
 * - High bit 0: Last byte
 * - Lower 7 bits: Data bits
 */
uint32_t varByteDecompressInt(uint8_t **byteBuffer) {
    uint32_t docId = 0;
    size_t shift = 0;
    while (1) {
        uint8_t currentByteValue = **byteBuffer;
        // Extract 7 data bits and shift into position
        docId |= (currentByteValue & 0x7F) << shift;
        shift += 7;
        (*byteBuffer)++;
        // Check if this is the last byte
        if ((currentByteValue & 0x80) == 0) {
            return docId;
        }
    }
}

/**
 * Decompresses a logarithmically compressed impact score
 * Reverses the log2(x+1) * 36.06 compression
 *
 * @param byteBuffer Pointer to current position in byte buffer
 * @return Decompressed impact score
 */
double logDecompressToDouble(uint8_t **byteBuffer) {
    uint8_t compressed = **byteBuffer;
    if (compressed == 0) {
        return 0.0;
    }
    // Reverse scaling and log compression
    double logImpactScore = (double)compressed / 36.06;
    double impactScore = exp2(logImpactScore) - 1;
    (*byteBuffer)++;
    return impactScore;
}

/**
 * Decompresses all postings in current loaded chunk of inverted list
 * Handles both document IDs (VByte) and impact scores (log)
 *
 * @param invertedList List containing compressed postings
 */
void decompressPostings(InvertedList *invertedList) {
    uint8_t *currentByte = invertedList->postings;
    int lastDocId = invertedList->lastDocIds[invertedList->currentChunkIndex];
    int prevDocId = -1;
    int postingIndex1 = 0;
    // Decompress delta-gap encoded document IDs
    while (1) {
        int docId = (int)varByteDecompressInt(&currentByte);
        if (prevDocId != -1) {
            docId += prevDocId; // Add delta to previous ID
        }
        invertedList->docIds[postingIndex1] = docId;
        prevDocId = docId;
        postingIndex1++;
        if (docId == lastDocId) {
            break;
        }
    }
    invertedList->postingCount = postingIndex1;
    // Decompress impact scores
    for (int postingIndex2 = 0; postingIndex2 < invertedList->postingCount; postingIndex2++) {
        invertedList->impactScores[postingIndex2] = logDecompressToDouble(&currentByte);
    }
}
