/*
 * API_hd44780_port.c
 *
 *  Created on: Aug 4, 2024
 *      Author: felipe
 */


#include "API_hd44780_port.h"

HAL_StatusTypeDef I2C_Master_Transmit_Wrapper(I2C_HandleTypeDef *hi2c, uint16_t deviceAddr, uint8_t *data, uint16_t size, uint32_t timeout) {
    HAL_StatusTypeDef stts = HAL_I2C_Master_Transmit(hi2c, deviceAddr, data, size, timeout);
    return stts;
}
