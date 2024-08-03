/*
 * utils.c
 *
 *  Created on: Aug 1, 2024
 *      Author: felipe
 */

#include "API_delay.h"

bool_t String_Compare(const char *str1, const char *str2) {
	// Loops until null terminator in str1 "\0"
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *str1 == *str2;
}

// Function to concatenate src to the end of dest
char *String_Concat(char *dest, const char *src) {
    char *original_dest = dest;

    // Move dest pointer to the end of the current string
    while (*dest) {dest++;}

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
