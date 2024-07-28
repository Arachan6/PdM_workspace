/*
 * API_gps.c
 *
 *  Created on: Jul 28, 2024
 *      Author: felipe
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "API_uart.h"
#include "API_gps.h"

NMEAData nmeaData;

// Function to split a string by a delimiter
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


bool_t Parse_NMEA_Sentence(const char* nmea) {
	bool_t rtrn = true;
    char fields[MAX_FIELDS][MAX_FIELDS_LENGTH];
    int fieldCount = 0;
    String_Split(nmea, ',', fields, &fieldCount);

    if (fieldCount < 1) {rtrn=false;} // Not enough fields

    strncpy(nmeaData.sentenceType, fields[0], sizeof(nmeaData.sentenceType) - 1);

    if (strcmp(nmeaData.sentenceType, "$GPGSA") == 0) {
        nmeaData.mode = fields[GPGSA_MODE][0];            						// Mode (e.g., 'A' for automatic, 'M' for manual)
        nmeaData.fixType = atoi(fields[GPGSA_FIX_TYPE]);  						// Fix type (1 = no fix, 2 = 2D fix, 3 = 3D fix)
        nmeaData.pdop = atof(fields[GPGSA_PDOP]);        	 					// Position dilution of precision
        nmeaData.hdop = atof(fields[GPGSA_HDOP]);         						// Horizontal dilution of precision
        nmeaData.vdop = atof(fields[GPGSA_VDOP]);         						// Vertical dilution of precision

    } else if (strcmp(nmeaData.sentenceType, "$GPGSV") == 0) {
        nmeaData.numOfMessages = atoi(fields[GPGSV_NUM_MESSAGES]);   			// Total number of GSV messages to be transmitted
        nmeaData.messageNumber = atoi(fields[GPGSV_MESSAGE_NUMBER]); 			// Message number
        nmeaData.satellitesInView = atoi(fields[GPGSV_SATELLITES_IN_VIEW]); 	// Total number of satellites in view

    } else if (strcmp(nmeaData.sentenceType, "$GPRMC") == 0) {
        strncpy(nmeaData.time, fields[GPRMC_TIME], sizeof(nmeaData.time) - 1); 	// UTC time in hhmmss.ss format
        nmeaData.status = fields[GPRMC_STATUS][0];                            	// Status (A = active, V = void)
        nmeaData.latitude = atof(fields[GPRMC_LATITUDE]);                     	// Latitude in ddmm.mmmm format
        nmeaData.longitude = atof(fields[GPRMC_LONGITUDE]);                   	// Longitude in dddmm.mmmm format
        nmeaData.speed = atof(fields[GPRMC_SPEED]);                           	// Speed over ground in knots
        nmeaData.trackAngle = atof(fields[GPRMC_TRACK_ANGLE]);                	// Track angle in degrees True
        strncpy(nmeaData.date, fields[GPRMC_DATE], sizeof(nmeaData.date) - 1); 	// Date in ddmmyy format

    } else if (strcmp(nmeaData.sentenceType, "$GPVTG") == 0) {
        nmeaData.trueTrack = atof(fields[GPVTG_TRUE_TRACK]);                  	// True track made good (degrees)
        nmeaData.magneticTrack = atof(fields[GPVTG_MAGNETIC_TRACK]);          	// Magnetic track made good (degrees)
        nmeaData.groundSpeedKnots = atof(fields[GPVTG_SPEED_KNOTS]);          	// Ground speed in knots
        nmeaData.groundSpeedKph = atof(fields[GPVTG_SPEED_KPH]);              	// Ground speed in kilometers per hour

    } else if (strcmp(nmeaData.sentenceType, "$GPGGA") == 0) {
        strncpy(nmeaData.time, fields[GPGGA_TIME], sizeof(nmeaData.time) - 1); 	// UTC time in hhmmss.ss format
        nmeaData.latitude = atof(fields[GPGGA_LATITUDE]);                     	// Latitude in ddmm.mmmm format
        nmeaData.longitude = atof(fields[GPGGA_LONGITUDE]);                   	// Longitude in dddmm.mmmm format
        nmeaData.fixQuality = atoi(fields[GPGGA_FIX_QUALITY]);                	// Fix quality (0 = invalid, 1 = GPS fix, 2 = DGPS fix)
        nmeaData.numSatellites = atoi(fields[GPGGA_NUM_SATELLITES]);          	// Number of satellites being tracked
        nmeaData.hdop = atof(fields[GPGGA_HDOP]);                             	// Horizontal dilution of precision
        nmeaData.altitude = atof(fields[GPGGA_ALTITUDE]);                     	// Altitude above mean sea level in meters
    }
    return rtrn;
}

const NMEAData* Get_NMEA_Data() {
    return &nmeaData;
}
