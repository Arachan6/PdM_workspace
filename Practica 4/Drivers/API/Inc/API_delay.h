/*
 * API_delay.h
 *
 *  Created on: Jul 4, 2024
 *      Author: felipe
 */

#ifndef API_INC_API_DELAY_H_
#define API_INC_API_DELAY_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"  		/* <- HAL include */
#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */


typedef uint32_t tick_t;
typedef bool bool_t;
typedef struct{
	tick_t startTime;
	tick_t duration;
	bool_t running;
} delay_t;

/**
 * @brief Initialize delay structure.
 * @param delay Pointer to delay structure to be initialized.
 * @param duration Duration of the delay in ticks.
 */
void delayInit(delay_t* delay, tick_t duration);

/**
 * @brief Checks if the delay is running and starts the timer if not.
 * @param delay Pointer to the delay structure.
 * @retval TRUE if time elapsed, FALSE if not.
 */
bool_t delayRead(delay_t* delay);

/**
 * @brief Sets the duration of the delay.
 * @param delay Pointer to the delay structure.
 * @param duration Duration of the delay in ticks.
 */
void delayWrite(delay_t* delay, tick_t  duration);

#endif /* API_INC_API_DELAY_H_ */
