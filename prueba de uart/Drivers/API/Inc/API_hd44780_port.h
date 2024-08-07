/*
 * API_hd44780_port.h
 *
 *  Created on: Aug 4, 2024
 *      Author: felipe
 */

#ifndef API_INC_API_HD44780_PORT_H_
#define API_INC_API_HD44780_PORT_H_

#include "main.h"

HAL_StatusTypeDef I2C_Master_Transmit_Wrapper(I2C_HandleTypeDef *hi2c, uint16_t deviceAddr, uint8_t *data, uint16_t size, uint32_t timeout);
void HAL_Delay_Wrapper(uint32_t ms);

#endif /* API_INC_API_HD44780_PORT_H_ */
