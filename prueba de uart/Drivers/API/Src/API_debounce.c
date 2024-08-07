/*
 * API_debounce.c
 *
 *  Created on: Jul 11, 2024
 *      Author: felipe
 */

#include "API_debounce.h"
#include "API_delay.h"

/**
 * @brief Enum for the debounce state machine states.
 */
typedef enum {
    BUTTON_UP,       /**< Button is not pressed. */
    BUTTON_FALLING,  /**< Button press is being debounced. */
    BUTTON_DOWN,     /**< Button is pressed. */
    BUTTON_RAISING   /**< Button release is being debounced. */
} debounceState_t;

static bool_t buttonPress; /**< Flag indicating if the button has been pressed. */
static debounceState_t buttonState; /**< Current state of the debounce state machine. */
static delay_t d1; /**< Delay structure used for debouncing timing. */

/**
 * @brief Initializes the debounce state machine.
 *
 * This function sets the initial state of the debounce state machine to BUTTON_UP
 * and initializes the debounce delay.
 */
void debounceFSM_init() {
    buttonState = BUTTON_UP;
    delayInit(&d1, VALID_BUTTON_TIME);
}

/**
 * @brief Updates the debounce state machine based on the button state.
 *
 * This function should be called periodically to check the button state and
 * update the state machine accordingly. It handles debouncing logic for both
 * button press and release events.
 */
void debounceFSM_update() {
    switch (buttonState) {
        case BUTTON_UP:
            if (BSP_PB_GetState(BUTTON_USER)) {
                buttonState = BUTTON_FALLING;
                delayRead(&d1);
            }
            break;
        case BUTTON_DOWN:
            if (!BSP_PB_GetState(BUTTON_USER)) {
                buttonState = BUTTON_RAISING;
                delayRead(&d1);
            }
            break;
        case BUTTON_RAISING:
            if (delayRead(&d1)) {
                if (!BSP_PB_GetState(BUTTON_USER)) {
                    buttonState = BUTTON_UP;
                    buttonReleased();
                } else {
                    buttonState = BUTTON_DOWN;
                }
            }
            break;
        case BUTTON_FALLING:
            if (delayRead(&d1)) {
                if (BSP_PB_GetState(BUTTON_USER)) {
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

/**
 * @brief Handles the button pressed event.
 *
 * This function is called when the button is confirmed to be pressed after
 * debouncing. It turns on LED1 and sets the button press flag.
 */
void buttonPressed() {
    BSP_LED_On(LED1);
    buttonPress = true;
}

/**
 * @brief Handles the button released event.
 *
 * This function is called when the button is confirmed to be released after
 * debouncing. It turns off LED1.
 */
void buttonReleased() {
    BSP_LED_Off(LED1);
}

/**
 * @brief Reads the button press status.
 *
 * This function returns whether the button was pressed since the last time
 * this function was called. It resets the button press flag after reading.
 *
 * @return `true` if the button was pressed, `false` otherwise.
 */
bool_t readKey() {
    bool_t rtrn = false;

    if (buttonPress == true) {
        rtrn = true;
        buttonPress = false;
    }

    return rtrn;
}
