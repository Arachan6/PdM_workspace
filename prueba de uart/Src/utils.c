/*
 * utils.c
 *
 *  Created on: Jul 28, 2024
 *      Author: felipe
 */
#include <stdio.h>

void DoubleToString(char* buffer, size_t bufferSize, double value) {
    // Format the double into the buffer with a specified number of decimal places
    snprintf(buffer, bufferSize, "%.6f", value);  // Use %.6f for 6 decimal places, adjust as needed
}
