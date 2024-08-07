/*
 * API_delay.c
 *
 *  Created on: Jul 4, 2024
 *      Author: felipe
 */

#include "API_delay.h"
#include "main.h"

/**
 * @brief Initialize the delay structure.
 *
 * This function sets up the delay structure with the specified duration.
 * It also resets the running state and start time.
 *
 * @param delay Pointer to the delay structure to initialize.
 * @param duration Duration of the delay in ticks.
 */
void delayInit(delay_t* delay, tick_t duration){

    if(delay == NULL){
        Error_Handler();
    }

    delay->running = false;      /**< Indicates whether the delay is currently running. */
    delay->startTime = 0;        /**< The start time of the delay in ticks. */
    delay->duration = duration;  /**< The duration of the delay in ticks. */
}

/**
 * @brief Check if the delay has elapsed.
 *
 * This function checks if the specified delay has elapsed. If the delay
 * is not running, it will start it and return false. If the delay is
 * already running, it will check if the elapsed time has reached the
 * duration and return true if it has.
 *
 * @param delay Pointer to the delay structure.
 * @return True if the delay has elapsed, false otherwise.
 */
bool_t delayRead(delay_t* delay){

    if(delay == NULL){
        Error_Handler();
    }

    if (delay->running == false){
        delay->startTime = HAL_GetTick();
        delay->running = true;
        return false;
    }

    if ((HAL_GetTick() - delay->startTime) >= delay->duration){
        delay->running = false;
        return true;
    }

    return false;
}

/**
 * @brief Set the duration of the delay.
 *
 * This function updates the duration of the specified delay structure.
 *
 * @param delay Pointer to the delay structure.
 * @param duration New duration for the delay in ticks.
 */
void delayWrite(delay_t* delay, tick_t  duration){

    if(delay == NULL){
        Error_Handler();
    }

    delay->duration = duration;
}
