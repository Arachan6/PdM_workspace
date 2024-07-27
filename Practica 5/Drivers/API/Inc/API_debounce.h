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

void debounceFSM_init();		// debe cargar el estado inicial
void debounceFSM_update();	    // debe leer las entradas, resolver la lógica de
					            // transición de estados y actualizar las salidas
void buttonPressed();			// debe encender el LED
void buttonReleased();		    // debe apagar el LED

bool_t readKey();

#endif /* API_INC_API_DEBOUNCE_H_ */