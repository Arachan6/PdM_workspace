/*
 * API_debounce.c
 *
 *  Created on: Jul 11, 2024
 *      Author: felipe
 */

#include "API_debounce.h"
#include "API_delay.h"

typedef enum{
	BUTTON_UP,
	BUTTON_FALLING,
	BUTTON_DOWN,
	BUTTON_RAISING,
} debounceState_t;

static bool_t buttonPress;
static debounceState_t buttonState;
static delay_t d1;

/* Establece el estado inicial de la máquina de estados */
void debounceFSM_init(){
	buttonState = BUTTON_UP;
	delayInit(&d1, VALID_BUTTON_TIME);
}

/* Verifica el estado del botón y actualiza el estado de la maquina de estados */
void debounceFSM_update(){
	switch(buttonState){
		case BUTTON_UP:
			if (BSP_PB_GetState(BUTTON_USER)){
				buttonState = BUTTON_FALLING;
				delayRead(&d1);
			}
			break;
		case BUTTON_DOWN:
			if (!BSP_PB_GetState(BUTTON_USER)){
				buttonState = BUTTON_RAISING;
				delayRead(&d1);
			}
			break;
		case BUTTON_RAISING:
			if (delayRead(&d1)){
				if (!BSP_PB_GetState(BUTTON_USER)){
					buttonState = BUTTON_UP;
					buttonReleased();
				} else {
					buttonState = BUTTON_DOWN;
				}
			}
			break;
		case BUTTON_FALLING:
			if (delayRead(&d1)){
				if (BSP_PB_GetState(BUTTON_USER)){
					buttonState = BUTTON_DOWN;
					buttonPressed();
				} else {
					buttonState = BUTTON_UP;
				}
			}
			break;
		default:
			Error_Handler();
			break;
	}
}

/* Prende el LED1 */
void buttonPressed(){
	BSP_LED_On(LED1);
	buttonPress=true;
}

/* Apaga el LED1 */
void buttonReleased(){
	BSP_LED_Off(LED1);
}

bool_t readKey(){
	bool_t rtrn = false;

	if (buttonPress==true){
		rtrn=true;
		buttonPress=false;
	}

	return rtrn;
}
