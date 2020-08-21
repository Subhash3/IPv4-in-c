#include <stdio.h>
#include "uInt32BitManipulations.h"

char whatIsIthBit(u_int32_t *address, int k)
{
    return (isBitSet(&(*address), k)) ? '1' : '0';
}

void setBit(u_int32_t *address, int k)
{
    *address |= (1 << (k % SIZE_OF_UINT32_IN_BITS));

    return;
}

void clearBit(u_int32_t *address, int k)
{
    *address &= ~(1 << (k % SIZE_OF_UINT32_IN_BITS));

    return;
}

bool isBitSet(u_int32_t *address, int k)
{
    return (*address & (1 << (k % SIZE_OF_UINT32_IN_BITS))) != 0;
}

void toggleBit(u_int32_t *address, int k)
{
    *address ^= (1 << (k % SIZE_OF_UINT32_IN_BITS));
    return;
}

void printBitString(u_int32_t *address)
{
    int i;

    // LITTLE ENDIAN... printing in reverse order
    for (i = SIZE_OF_UINT32_IN_BITS - 1; i >= 0; i--)
    {
        printf((isBitSet(&(*address), i)) ? "\x1b[33m1\x1b[0m" : "0");
        if ((SIZE_OF_UINT32_IN_BITS - i) % 8 == 0)
        {
            printf(" ");
        }
        fflush(NULL); // flushing must be done 'cuz we're not using \n
    }
    printf("\n");

    return;
}