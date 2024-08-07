/*
 * utils.c
 *
 *  Created on: Aug 1, 2024
 *      Author: felipe
 */

#include "API_delay.h"
#include "utils.h"

/**
 * @brief Compare two strings.
 *
 * This function compares two strings and returns true if they are equal, false otherwise.
 *
 * @param str1 Pointer to the first string.
 * @param str2 Pointer to the second string.
 * @retval bool_t True if the strings are equal, false otherwise.
 */
bool_t String_Compare(const char *str1, const char *str2) {
    // Loops until null terminator in str1 "\0"
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *str1 == *str2;
}

/**
 * @brief Concatenate src to the end of dest.
 *
 * This function appends the source string (src) to the end of the destination string (dest).
 *
 * @param dest Pointer to the destination string. The source string will be appended to this.
 * @param src Pointer to the source string to be appended.
 * @retval char* Pointer to the destination string (original dest).
 */
char *String_Concat(char *dest, const char *src) {
    char *original_dest = dest;

    // Move dest pointer to the end of the current string
    while (*dest) { dest++; }

    // Copy characters from src to the end of dest
    while (*src) {
        *dest = *src;
        dest++;
        src++;
    }

    // Add null terminator
    *dest = '\0';

    // Return the original destination pointer
    return original_dest;
}

/**
 * @brief Split a string by a delimiter.
 *
 * This function splits a string into fields based on a delimiter and stores the fields in an array.
 *
 * @param str Pointer to the string to be split.
 * @param delimiter The delimiter character used to split the string.
 * @param fields Array to store the split fields. Each field is a string.
 * @param fieldCount Pointer to an integer to store the number of fields.
 */
void String_Split(const char* str, char delimiter, char fields[MAX_FIELDS][MAX_FIELDS_LENGTH], int* fieldCount) {
    *fieldCount = 0;
    int length = strlen(str);
    int fieldIndex = 0;
    int charIndex = 0;

    for (int i = 0; i < length; i++) {
        if (str[i] == delimiter || str[i] == '\n') {
            fields[fieldIndex][charIndex] = '\0'; // Null-terminate current field
            fieldIndex++;
            charIndex = 0;
            if (fieldIndex >= MAX_FIELDS) break;
        } else {
            fields[fieldIndex][charIndex++] = str[i];
            if (charIndex >= MAX_FIELDS_LENGTH - 1) {
                fields[fieldIndex][charIndex] = '\0'; // Null-terminate and prevent overflow
                charIndex = 0;
            }
        }
    }
    *fieldCount = fieldIndex + 1; // Account for the last field
}

/**
 * @brief Calculate the XOR checksum of a string.
 *
 * This function calculates the XOR checksum of the given string.
 *
 * @param str Pointer to the string to be checksummed.
 * @retval unsigned char The XOR checksum of the string.
 */
unsigned char String_XOR_Checksum(const char* str) {
    unsigned char checksum = 0;
    while (*str) {
        checksum ^= *str;
        str++;
    }
    return checksum;
}

/**
 * @brief Convert an unsigned char to a two-digit hexadecimal string.
 *
 * This function converts an unsigned char value to a two-character hexadecimal string.
 *
 * @param value The unsigned char value to convert.
 * @param buffer Pointer to the buffer to store the hexadecimal string. Must be at least 3 bytes long.
 */
void CHAR_To_HEX_String(unsigned char value, char* buffer) {
    const char hexDigits[] = "0123456789ABCDEF";
    buffer[0] = hexDigits[(value >> 4) & 0x0F]; // High nibble
    buffer[1] = hexDigits[value & 0x0F];        // Low nibble
    buffer[2] = '\0'; // Null-terminate the string
}
