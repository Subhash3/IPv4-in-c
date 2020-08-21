#ifndef __IPV4_H__
#define __IPV4_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define IPv4_PARTS_LENGTH 4
#define EACH_IPv4_PART_SIZE_IN_BITS 8

#define SIZE_OF_UINT32_IN_BITS 32

struct IPv4
{
    u_int32_t address;
    u_int8_t ipArray[IPv4_PARTS_LENGTH];
    u_int8_t networkBitsLength;
    u_int8_t hostBitsLength;
    u_int8_t CIDR;
    char ipClass;
};

typedef struct IPv4 IPv4;

// Function prototypes
IPv4 *initIPv4(IPv4 **ipAddress, u_int8_t decimalArray[], u_int8_t *CIDR);
void printIPv4(IPv4 **ipAddress);
int convertDecimalIPtoUnsignedInt(u_int8_t decimalArray[], int size);
char determineIPClass(IPv4 **ipAddress);
int sizeOfNetwork(char ipClass);
u_int32_t calculateNetmask(IPv4 **ipAddress);
u_int32_t calculateNetworkID(IPv4 **ipAddress);
u_int32_t calculateBroadcastID(IPv4 **ipAddress);
void dottedDecimal(u_int32_t address, u_int8_t decimalRep[]);
void printDottedDecimalRepresentation(u_int32_t address);
void printHostsRange(IPv4 **ipAddress);

// Helper Functions
/*
I should've made an another file for them.
Let them be here for now!
*/

int bitwiseConcatIntergers(unsigned int a, unsigned int b);
int bitCountAnInteger(int num);

#endif