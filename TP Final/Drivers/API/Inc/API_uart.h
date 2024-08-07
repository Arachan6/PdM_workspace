/*
 * API_uart.h
 *
 *  Created on: Jul 22, 2024
 *      Author: felipe
 */

#ifndef API_INC_API_UART_H_
#define API_INC_API_UART_H_

#include "API_delay.h"

bool_t UART5_Init();
bool_t USART2_Init();
void UART5_Send_String(uint8_t * pstring);
void USART2_Send_String(uint8_t * pstring);
HAL_StatusTypeDef UART5_Receive_Char(uint8_t *pchar);
HAL_StatusTypeDef USART2_Receive_Char(uint8_t *pchar);
bool_t nmea_sentence_received();
char* get_nmea_sentence();
bool_t pc_command_received();
char* get_pc_command();

#endif /* API_INC_API_UART_H_ */
