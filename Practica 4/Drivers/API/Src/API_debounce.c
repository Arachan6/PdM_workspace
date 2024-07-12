/*
 * API_debounce.c
 *
 *  Created on: Jul 11, 2024
 *      Author: felipe
 */

#include "API_debounce.h"

static typedef enum{
	BUTTON_UP,
	BUTTON_FALLING,
	BUTTON_DOWN,
	BUTTON_RAISING,
} debounceState_t;

static debounceState_t buttonState;

void debounceFSM_init(){
	buttonState = BUTTON_UP;
}
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

void buttonPressed(){
	BSP_LED_On(LED1);
}

void buttonReleased(){
	BSP_LED_Off(LED1);
}

bool_t readKey(){

}
