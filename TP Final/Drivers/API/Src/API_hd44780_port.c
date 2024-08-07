/*
 * API_hd44780_port.c
 *
 *  Created on: Aug 4, 2024
 *      Author: felipe
 */

#include "API_hd44780_port.h"

/**
 * @brief Wrapper function for I2C master transmit operation.
 *
 * This function wraps the HAL_I2C_Master_Transmit function to provide
 * a simplified interface for transmitting data to an I2C device.
 *
 * @param hi2c Pointer to the I2C handle structure.
 * @param deviceAddr The I2C address of the target device.
 * @param data Pointer to the data buffer to be transmitted.
 * @param size Size of the data buffer.
 * @param timeout Timeout duration for the I2C transmission.
 * @return HAL status indicating the result of the transmission.
 */
HAL_StatusTypeDef I2C_Master_Transmit_Wrapper(I2C_HandleTypeDef *hi2c, uint16_t deviceAddr, uint8_t *data, uint16_t size, uint32_t timeout) {
    HAL_StatusTypeDef stts = HAL_I2C_Master_Transmit(hi2c, deviceAddr, data, size, timeout);
    return stts;
}

/**
 * @brief Wrapper function for HAL_Delay.
 *
 * This function wraps the HAL_Delay function to provide
 * a simplified interface for delaying execution.
 *
 * @param ms The number of milliseconds to delay.
 */
void HAL_Delay_Wrapper(uint32_t ms) {
    HAL_Delay(ms);
}
