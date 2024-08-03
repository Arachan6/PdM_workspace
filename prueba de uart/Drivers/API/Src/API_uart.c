/*
 * API_uart.c
 *
 *  Created on: Jul 22, 2024
 *      Author: felipe
 */

#include "API_delay.h"
#include "API_uart.h"
#include "main.h"

#define UART_MAX_TIMEOUT 1000
#define NMEA_BUFFER_SIZE 128

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart5;

uint8_t rx_char;
bool_t nmea_ready;

static void UART5_GPIO_Init(void){
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable GPIO Clocks */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /* Enable UART5 Clock */
    __HAL_RCC_UART5_CLK_ENABLE();

    /* Configure PC12 for UART5 TX */
    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* Configure PD2 for UART5 RX */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

static void USART2_GPIO_Init(void){
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable GPIO Clocks */
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /* Enable USART2 Clock */
    __HAL_RCC_USART2_CLK_ENABLE();

    /* Configure PD5 for USART2 TX */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* Configure PD6 for USART2 RX */
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

bool_t UART5_Init(void){
	UART5_GPIO_Init();

	bool_t rtrn=true;

    huart5.Instance = UART5;
    huart5.Init.BaudRate = 9600;
    huart5.Init.WordLength = UART_WORDLENGTH_8B;
    huart5.Init.StopBits = UART_STOPBITS_1;
    huart5.Init.Parity = UART_PARITY_NONE;
    huart5.Init.Mode = UART_MODE_TX_RX;
    huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart5.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart5) != HAL_OK){rtrn=false;}

    // Habilitar interrupciones de UART5
	HAL_NVIC_SetPriority(UART5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(UART5_IRQn);
	HAL_UART_Receive_IT(&huart5, (uint8_t *)&rx_char, 1);

	return rtrn;
}

bool_t USART2_Init(void){
	USART2_GPIO_Init();

	bool_t rtrn=true;
	char baudRate[10]={};
	char wordLength[10]={};
	char stopBits[10]={};
	char parityBits[10]={};

    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart2) != HAL_OK){rtrn=false;}

    sprintf(baudRate, "%d",(int)huart2.Init.BaudRate);
	sprintf(wordLength, "%d",(int)huart2.Init.WordLength);
	sprintf(stopBits, "%d",(int)huart2.Init.StopBits);
	sprintf(parityBits, "%d",(int)huart2.Init.Parity);

	USART2_Send_String((uint8_t*)"Uart Inicializada correctamente\n\r");
	USART2_Send_String((uint8_t*)"-----Parametros Configurados-----\n\r");
	USART2_Send_String((uint8_t*)"\n\rBaud rate: ");
	USART2_Send_String((uint8_t*)baudRate);
	USART2_Send_String((uint8_t*)"\n\rWord Length: ");
	USART2_Send_String((uint8_t*)wordLength);
	USART2_Send_String((uint8_t*)"\n\rStop Bits: ");
	USART2_Send_String((uint8_t*)stopBits);
	USART2_Send_String((uint8_t*)"\n\rParity: ");
	USART2_Send_String((uint8_t*)parityBits);
	USART2_Send_String((uint8_t*)"\n\r");

	return rtrn;
}

void UART5_Send_String(uint8_t * pstring){
	HAL_UART_Transmit(&huart5,(uint8_t*) pstring, strlen((const char*) pstring), UART_MAX_TIMEOUT);
}

void USART2_Send_String(uint8_t * pstring){
	HAL_UART_Transmit(&huart2,(uint8_t*) pstring, strlen((const char*) pstring), UART_MAX_TIMEOUT);
}

HAL_StatusTypeDef UART5_Receive_Char(uint8_t *pchar) {
    HAL_StatusTypeDef stts = HAL_UART_Receive(&huart5, pchar, 1, UART_MAX_TIMEOUT);
    return stts;
}

HAL_StatusTypeDef USART2_Receive_Char(uint8_t *pchar) {
    HAL_StatusTypeDef stts = HAL_UART_Receive(&huart2, pchar, 1, UART_MAX_TIMEOUT);
    return stts;
}

// La función UART5_IRQHandler es la rutina de servicio de interrupción (ISR)
// que se llama automáticamente cuando ocurre una interrupción asociada al UART5.
void UART5_IRQHandler(void) {

	// La función HAL_UART_IRQHandler es parte de la HAL.
	// Se encarga de identificar qué tipo de evento de interrupción ocurrió.
	// Dependiendo del tipo, llamará a la función de callback correspondiente.
    HAL_UART_IRQHandler(&huart5);
}

uint8_t nmea_incoming_buffer[NMEA_BUFFER_SIZE];
uint8_t nmea_ready_buffer[NMEA_BUFFER_SIZE];
uint8_t nmea_index = 0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

    if (huart->Instance == UART5) {
        if (rx_char == '$'){
        	for (uint8_t i = 0; i < NMEA_BUFFER_SIZE; i++) {
        	    nmea_ready_buffer[i] = nmea_incoming_buffer[i];
        	    nmea_incoming_buffer[i] = 0;
        	}
            nmea_index = 0;
            nmea_ready = true;
            //USART2_Send_String((uint8_t*)nmea_ready_buffer);
        }
        nmea_incoming_buffer[nmea_index++] = rx_char;
	}

	// Continuar recibiendo el siguiente carácter
	HAL_UART_Receive_IT(&huart5, (uint8_t *)&rx_char, 1);
}

bool_t nmea_sentence_received(void){
	bool_t rtrn = false;
	if (nmea_ready==true){
		rtrn=true;
		nmea_ready=false;
	}
	return rtrn;
}

char* get_nmea_sentence(void) {
    return (char*) nmea_ready_buffer;
}



















