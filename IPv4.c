#include "IPv4.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

IPv4 *initIPv4(IPv4 **ipAddress, u_int8_t ipArray[], u_int8_t *CIDR)
{
    IPv4 *temp = malloc(sizeof(IPv4 *));
    int i, networkBitsLength;

    if (!temp)
    {
        return NULL;
    }

    (*ipAddress) = (IPv4 *)temp;
    for (i = 0; i < IPv4_PARTS_LENGTH; i++)
    {
        (*ipAddress)->ipArray[i] = ipArray[i];
    }

    // Allocate memory for IPv4 object
    *ipAddress = (IPv4 *)temp;

    // Convert IP adress to binary string
    (*ipAddress)->address = convertDecimalIPtoBinaryString(ipArray, IPv4_PARTS_LENGTH);

    // Determine IP class
    (*ipAddress)->ipClass = determineIPClass(ipAddress);

    // Calculate the number of net-bits and host-bits
    networkBitsLength = sizeOfNetwork((*ipAddress)->ipClass);
    if (networkBitsLength == -1)
    {
        //printf("Class D network-bits are not defined\n");
    }
    else
    {
        (*ipAddress)->networkBitsLength = networkBitsLength;
        (*ipAddress)->hostBitsLength = SIZE_OF_UINT32_IN_BITS - networkBitsLength;
    }

    // If CIDR is provided
    if (CIDR != NULL)
    {
        printf("CIDR: %hhd\n", *CIDR);
        (*ipAddress)->networkBitsLength = *CIDR;
        (*ipAddress)->hostBitsLength = SIZE_OF_UINT32_IN_BITS - *CIDR;
    }

    printf("Network bits: %d\n", (*ipAddress)->networkBitsLength);
    printf("Host bits: %d\n", (*ipAddress)->hostBitsLength);
    return (*ipAddress);
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

void printIPv4(IPv4 **ipAddress)
{
    u_int8_t netmaskArr[IPv4_PARTS_LENGTH], broadcastIDArr[IPv4_PARTS_LENGTH], networkIDArr[IPv4_PARTS_LENGTH];

    printf("\t\tIP INFO\n");
    printf("IP-Class: \t%c\n", (*ipAddress)->ipClass);
    printf("IP: 32-bit decimal representation: %u\n", (*ipAddress)->address);
    printf("IP: Binary string: ");
    printBitString(&((*ipAddress)->address));
    printf("Dotted decimal: ");
    printDottedDecimalRepresentation((*ipAddress)->ipArray);
    printf("\n");

    printf("\t\tNetmask INFO\n");
    calculateNetmask(ipAddress);
    printf("Netmask 32-bit decimal representation: %u\n", (*ipAddress)->netmask);
    printf("Netmask: Binary string: ");
    printBitString(&((*ipAddress)->netmask));
    printf("Dotted decimal: ");
    dottedDecimal(&((*ipAddress)->netmask), netmaskArr);
    printDottedDecimalRepresentation(netmaskArr);
    printf("\n");

    printf("\t\tNetwork ID INFO\n");
    calculateNetworkID(ipAddress);
    printf("Network-ID 32-bit decimal representation: %u\n", (*ipAddress)->networkID);
    printf("Network-ID: Binary string: ");
    printBitString(&((*ipAddress)->networkID));
    printf("Dotted decimal: ");
    dottedDecimal(&((*ipAddress)->networkID), networkIDArr);
    printDottedDecimalRepresentation(networkIDArr);
    printf("\n");

    printf("\t\tBroadcast ID INFO\n");
    calculateBroadcastID(ipAddress);
    printf("Broadcast-ID 32-bit decimal representation: %u\n", (*ipAddress)->broadcastID);
    printf("Broadcast-ID: Binary string: ");
    printBitString(&((*ipAddress)->broadcastID));
    printf("Dotted decimal: ");
    dottedDecimal(&((*ipAddress)->broadcastID), broadcastIDArr);
    printDottedDecimalRepresentation(broadcastIDArr);

    printf("\n");
    printf("\t\tHost Range INFO\n");
    printHostsRange(ipAddress);
    return;
}

char whatIsIthBit(u_int32_t *address, int k)
{
    return (isBitSet(&(*address), k)) ? '1' : '0';
}

int bitwiseConcatIntergers(unsigned int a, unsigned int b)
{
    return ((a << EACH_IPv4_PART_SIZE_IN_BITS) | b);
}

int bitCountAnInteger(int num)
{
    return num == 0 ? 1 : (int)log2(num) + 1;
}

int convertDecimalIPtoBinaryString(u_int8_t ipArray[], int size)
{
    int i;
    unsigned int result = 0;

    for (i = 0; i < IPv4_PARTS_LENGTH; i++)
    {
        result = bitwiseConcatIntergers(result, (unsigned int)ipArray[i]);
    }

    return result;
}

char determineIPClass(IPv4 **ipAddress)
{
    if ((*ipAddress)->ipArray[0] <= 127)
        return 'A';
    else if ((*ipAddress)->ipArray[0] <= 191)
        return 'B';
    else if ((*ipAddress)->ipArray[0] <= 223)
        return 'C';
    else
        return 'D';
}

int sizeOfNetwork(char ipClass)
{
    switch (ipClass)
    {
    case 'A':
        return 8;
    case 'B':
        return 16;
    case 'C':
        return 24;
    case 'D':
        return -1;
    }

    return -1;
}

u_int32_t calculateNetmask(IPv4 **ipAddress)
{
    int i;
    u_int32_t netmask = (*ipAddress)->address;

    for (i = 0; i < SIZE_OF_UINT32_IN_BITS; i++)
    {
        if (i < (*ipAddress)->networkBitsLength)
        {
            setBit(&netmask, SIZE_OF_UINT32_IN_BITS - i - 1);
        }
        else
        {
            clearBit(&netmask, SIZE_OF_UINT32_IN_BITS - i - 1);
        }
    }

    (*ipAddress)->netmask = netmask;

    return netmask;
}

u_int32_t calculateBroadcastID(IPv4 **ipAddress)
{
    int i;
    u_int32_t broadcast = (*ipAddress)->address;

    for (i = 0; i < SIZE_OF_UINT32_IN_BITS; i++)
    {
        if (i < (*ipAddress)->networkBitsLength)
        {
            // setBit(&broadcast, SIZE_OF_UINT32_IN_BITS - i - 1);
        }
        else
        {
            setBit(&broadcast, SIZE_OF_UINT32_IN_BITS - i - 1);
        }
    }
    (*ipAddress)->broadcastID = broadcast;

    return broadcast;
}

u_int32_t calculateNetworkID(IPv4 **ipAddress)
{
    u_int32_t ip = (*ipAddress)->address;
    u_int32_t netmask = (*ipAddress)->netmask;
    u_int32_t networkID = ip & netmask;

    (*ipAddress)->networkID = networkID;

    return networkID;
}

void dottedDecimal(u_int32_t *address, u_int8_t decimalRep[])
{
    int i;
    unsigned int biggestNumberOfByte = 255;
    int shiftOffset;
    for (i = 0; i < IPv4_PARTS_LENGTH; i++)
    {
        shiftOffset = ((IPv4_PARTS_LENGTH - i - 1) * 8);
        decimalRep[i] = ((biggestNumberOfByte << shiftOffset) & *address) >> shiftOffset;
    }

    return;

    /*
        I know this functions looks damn confusing..
        Let say that num is a 32-bit unsigned integer.
        (let) 10110101 11011001 00010101 00100001
        To get the first byte i.e first 8 bits, we do the following
            OR operation (|)
        num:    10110101 11011001 00010101 00100001
        mask:   11111111 00000000 00000000 00000000

        To get the second byte i.e second 8 bits, we do the following
            OR operation (|)
        num:    10110101 11011001 00010101 00100001
        mask:   00000000 11111111 00000000 00000000

        You get the Idea.

        Here the mask is just a shfted version of 11111111(which is 255, the biggest number with 7-bits).
        
        For 1st byte, left-shift 255 24bits.
        For 2st byte, left-shift 255 16bits.
        For 3rd byte, left-shift 255 8bits.
        For 4th byte, left-shift 255 0bits.

        Generalised:
        For ith byte, shift 255 by a shiftOffest calculated above :D
    */
}

void printDottedDecimalRepresentation(u_int8_t decimalRep[])
{
    int i;
    for (i = 0; i < IPv4_PARTS_LENGTH; i++)
    {
        printf("%d", decimalRep[i]);
        if (i != IPv4_PARTS_LENGTH - 1)
        {
            printf(".");
        }
        fflush(NULL);
    }
    printf("\b\n");
}

void printHostsRange(IPv4 **ipAddress)
{
    unsigned int firstHost, lastHost;

    // Set the last bit of the network ID to get the first host
    firstHost = (*ipAddress)->networkID;
    setBit(&firstHost, 0);

    printf("First host: %u\n", firstHost);
    printBitString(&firstHost);

    // Clear the first bit of broadcastID to get the last host
    lastHost = (*ipAddress)->broadcastID;
    clearBit(&lastHost, 0);

    printf("Last host: %u\n", lastHost);
    printBitString(&lastHost);

    printf("No. of hosts: %u\n", lastHost - firstHost + 1);
    return;
}