#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IPv4.h"

#define MAX_IP_STRING_LENGTH 17
#define MAX_TOKEN_LENGTH 3

// Function prototypes
void printArray(u_int8_t arr[], int size);
u_int8_t *ipStringToDecimalArray(char *ipString, u_int8_t *decimalArray);
int validateIPPart(char *token);
// u_int8_t validateCIDR(char *cidr);

int main(int argc, char **argv)
{
    char ipString[MAX_IP_STRING_LENGTH + 1];
    IPv4 *ipAddress;
    u_int8_t decimalArray[IPv4_PARTS_LENGTH], *CIDR = NULL, num;

    if (argc < 2 || argc > 3)
    {
        printf("Usage: %s <IP address> [CIDR]\n", argv[0]);
        exit(1);
    }
    else
    {
        if (argc == 2)
        {
            // printf("%s\n", argv[1]);
            // Only IP is provided
        }
        else
        {
            // printf("%s, %s\n", argv[1], argv[2]);
            // Both IP and CIDR are provided.
            num = atoi(argv[2]);
            CIDR = &num;
        }
        strcpy(ipString, argv[1]);
        if (ipStringToDecimalArray(ipString, decimalArray) == NULL)
        {
            printf("Invalid IP address\n");
            printf("IP address' format must be: a.b.c.d\n");
            exit(1);
        }
    }

    // printArray(decimalArray, IPv4_PARTS_LENGTH);

    // printf("Create a new IPv4 address Object\n");
    ipAddress = initIPv4(&ipAddress, decimalArray, CIDR);

    printIPv4(&ipAddress);

    exit(0);
}

void printArray(u_int8_t arr[], int size)
{
    int i;

    for (i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return;
}

u_int8_t *ipStringToDecimalArray(char *ipString, u_int8_t *decimalArray)
{
    int dots = 0, i, j = 0, ipPart, ipPartIndex = 0;
    char token[MAX_TOKEN_LENGTH + 1];
    bool SEEN_DOT = false;

    for (i = 0;; i++)
    {
        if (!((ipString[i] >= 48 && ipString[i] <= 57) || ipString[i] == 46 || ipString[i] == '\0'))
        {
            // printf("Invalid Char: %c\n", ipString[i]);
            return NULL;
        }
        if (ipString[i] == '.')
        {
            // printf("%c a dot.\n", ipString[i]);
            if (SEEN_DOT)
            {
                // printf("THe previous one is also dot.\n");
                return NULL;
            }
            dots++;
            SEEN_DOT = true;
            token[j] = '\0';
            j = 0;
            ipPart = validateIPPart(token);
            // printf("TOekn: %s, ipPart: %d\n", token, ipPart);
            decimalArray[ipPartIndex++] = ipPart;
            if (ipPart == -1)
                return NULL;
            continue;
        }
        if (ipString[i] == '\0')
        {
            // printf("Last char\n");
            if (SEEN_DOT)
            {
                // printf("THe previous one is a dot.\n");
                return NULL;
            }
            token[j] = '\0';
            j = 0;
            ipPart = validateIPPart(token);
            // printf("TOekn: %s, ipPart: %d\n", token, ipPart);
            decimalArray[ipPartIndex++] = ipPart;
            if (ipPart == -1)
                return NULL;
            break;
        }
        SEEN_DOT = false;
        token[j] = ipString[i];
        j++;
    }
    if (dots != 3)
        return NULL;

    return decimalArray;
}

int validateIPPart(char *token)
{
    int ipPart = atoi(token);
    if (ipPart < 0 || ipPart > 255)
        return -1;
    return ipPart;
}