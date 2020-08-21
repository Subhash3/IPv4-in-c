#ifndef __uInt32BitManipulations_H__
#define __uInt32BitManipulations_H__

#include <stdlib.h>
#include <stdbool.h>

#define SIZE_OF_UINT32_IN_BITS 32

void setBit(u_int32_t *number, int k);
void clearBit(u_int32_t *number, int k);
bool isBitSet(u_int32_t *number, int k);
void toggleBit(u_int32_t *number, int k);
void printBitString(u_int32_t *number);
char whatIsIthBit(u_int32_t *address, int k);

#endif