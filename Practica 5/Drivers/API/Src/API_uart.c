/*
 * API_uart.c
 *
 *  Created on: Jul 22, 2024
 *      Author: felipe
 */

#include "API_delay.h"
#include "API_uart.h"
#include "main.h"

#define HAL_MAX_TIMEOUT 1000

UART_HandleTypeDef UartHandle;

/**
 * @brief Initialize uart peripheral
 * @return true if successful, false otherwise.
 */
bool_t uartInit(void){
	bool_t rtrn=true;
	char baudRate[10]={};
	char wordLength[10]={};
	char stopBits[10]={};
	char parityBits[10]={};

	UartHandle.Instance        = USARTx;
	UartHandle.Init.BaudRate   = 9600;
	UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits   = UART_STOPBITS_1;
	UartHandle.Init.Parity     = UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode       = UART_MODE_TX_RX;
	UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&UartHandle) != HAL_OK){rtrn=false;}

	// Con sprintf convierto integer a string
	// Quiero por ej baudRate = ['9','6','0','0',...]
	sprintf(baudRate, "%d", (int)UartHandle.Init.BaudRate);
	sprintf(wordLength, "%d",(int)UartHandle.Init.WordLength);
	sprintf(stopBits, "%d",(int)UartHandle.Init.StopBits);
	sprintf(parityBits, "%d",(int)UartHandle.Init.Parity);

	// La funcion uartSendString recibe un puntero a uint
	// Si le paso un string directamente es un puntero a int porque un char es un int entre -127 y 127
	// Sin el casteo tira warning porque la conversion de signed a unsigned da cualquier cosa si convierte un negativo
	uartSendString((uint8_t*)"Uart Inicializada Correctamente\n\r");
	uartSendString((uint8_t*)"Parametros Configurados:\n\r");
	uartSendString((uint8_t*)"Baud rate: ");
	uartSendString((uint8_t*)baudRate);
	uartSendString((uint8_t*)"\n\r");
	uartSendString((uint8_t*)"Word Length: ");
	uartSendString((uint8_t*)wordLength);
	uartSendString((uint8_t*)"\n\r");
	uartSendString((uint8_t*)"Stop Bits: ");
	uartSendString((uint8_t*)stopBits);
	uartSendString((uint8_t*)"\n\r");
	uartSendString((uint8_t*)"Parity: ");
	uartSendString((uint8_t*)parityBits);
	uartSendString((uint8_t*)"\n\r");

	return rtrn;
}

/**
 * @brief send a string through initialized uart peripheral.
 * @param pointer to bytes sequence.
 */
void uartSendString(uint8_t * pstring){
	HAL_UART_Transmit(&UartHandle,(uint8_t*) pstring, strlen(pstring), HAL_MAX_TIMEOUT);
}

/**
 * @brief send a string through initialized uart peripheral.
 * @param pointer to bytes sequence.
 * @param size of bytes sequence.
 */
void uartSendStringSize(uint8_t * pstring, uint16_t size){
	HAL_UART_Transmit(&UartHandle,(uint8_t*) pstring, size, HAL_MAX_TIMEOUT);
}
