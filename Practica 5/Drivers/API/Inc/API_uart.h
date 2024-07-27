/*
 * API_uart.h
 *
 *  Created on: Jul 22, 2024
 *      Author: felipe
 */

#ifndef API_INC_API_UART_H_
#define API_INC_API_UART_H_

bool_t uartInit();
void uartSendString(uint8_t * pstring);
void uartSendStringSize(uint8_t * pstring, uint16_t size);
void uartReceiveStringSize(uint8_t * pstring, uint16_t size);

#endif /* API_INC_API_UART_H_ */
