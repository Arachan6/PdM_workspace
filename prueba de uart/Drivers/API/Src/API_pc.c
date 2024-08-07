/*
 * API_pc.c
 *
 *  Created on: Aug 3, 2024
 *      Author: felipe
 */

#include "API_pc.h"
#include "API_gps.h"
#include "API_delay.h"

/**
 * @brief Processes a PC command and executes corresponding GPS actions.
 *
 * This function compares the given PC command with predefined command strings
 * and triggers the respective GPS functions.
 *
 * @param pc_command Pointer to the command string received from the PC.
 * @return true if the command is recognized and processed, false otherwise.
 */
bool_t Process_PC_Command(const char* pc_command) {
    bool_t rtrn = true;

    if (String_Compare(pc_command, "$DUMP\n\r") == 1) {
        GPS_Dump_Partial_Flash_Data();
        rtrn = true;
    } else if (String_Compare(pc_command, "$ERASE\n\r") == 1) {
        GPS_Erase_Flash_Data();
        rtrn = true;
    } else if (String_Compare(pc_command, "$LOGNOW\n\r") == 1) {
        GPS_Log_Now();
        rtrn = true;
    } else {
        rtrn = false;
    }
    return rtrn;
}
