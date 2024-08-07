/*
 * API_debounce.h
 *
 *  Created on: Jul 11, 2024
 *      Author: felipe
 */

#include "API_delay.h"

#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

#define VALID_BUTTON_TIME 40

void debounceFSM_init();
void debounceFSM_update();

void buttonPressed();
void buttonReleased();

bool_t readKey();

#endif /* API_INC_API_DEBOUNCE_H_ */
