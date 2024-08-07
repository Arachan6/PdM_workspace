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
#define NMEA_BUFFER_SIZE 255
#define PC_BUFFER_SIZE 128

#define NMEA_QUEUE_SIZE 10 // Maximum number of NMEA messages to store

typedef struct {
    uint8_t buffer[NMEA_QUEUE_SIZE][NMEA_BUFFER_SIZE]; /**< Buffer to store NMEA messages */
    uint8_t head; /**< Index of the next message to be dequeued */
    uint8_t tail; /**< Index of the next message to be enqueued */
    uint8_t count; /**< Number of messages currently in the queue */
} NMEA_Queue;

NMEA_Queue nmea_queue = { .head = 0, .tail = 0, .count = 0 };

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart5;

uint8_t rx_char_gps;
uint8_t rx_char_pc;
bool_t nmea_available;

/**
 * @brief Initialize GPIO for UART5.
 */
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

/**
 * @brief Initialize GPIO for USART2.
 */
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

/**
 * @brief Initialize UART5 with default parameters.
 * @retval true if initialization is successful, false otherwise.
 */
bool_t UART5_Init(void){
    UART5_GPIO_Init();

    bool_t rtrn = true;

    huart5.Instance = UART5;
    huart5.Init.BaudRate = 9600;
    huart5.Init.WordLength = UART_WORDLENGTH_8B;
    huart5.Init.StopBits = UART_STOPBITS_1;
    huart5.Init.Parity = UART_PARITY_NONE;
    huart5.Init.Mode = UART_MODE_TX_RX;
    huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart5.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart5) != HAL_OK) { rtrn = false; }

    // Enable UART5 interrupts
    HAL_NVIC_SetPriority(UART5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(UART5_IRQn);
    HAL_UART_Receive_IT(&huart5, (uint8_t *)&rx_char_gps, 1);

    return rtrn;
}

/**
 * @brief Initialize USART2 with default parameters.
 * @retval true if initialization is successful, false otherwise.
 */
bool_t USART2_Init(void){
    USART2_GPIO_Init();

    bool_t rtrn = true;
    char baudRate[10] = {};
    char wordLength[10] = {};
    char stopBits[10] = {};
    char parityBits[10] = {};

    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart2) != HAL_OK) { rtrn = false; }

    // Enable USART2 interrupts
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
    HAL_UART_Receive_IT(&huart2, (uint8_t *)&rx_char_pc, 1);

    sprintf(baudRate, "%d", (int)huart2.Init.BaudRate);
    sprintf(wordLength, "%d", (int)huart2.Init.WordLength);
    sprintf(stopBits, "%d", (int)huart2.Init.StopBits);
    sprintf(parityBits, "%d", (int)huart2.Init.Parity);

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

/**
 * @brief Transmit a string via UART5.
 * @param pstring Pointer to the string to transmit.
 */
void UART5_Send_String(uint8_t * pstring){
    HAL_UART_Transmit(&huart5, (uint8_t*)pstring, strlen((const char*)pstring), UART_MAX_TIMEOUT);
}

/**
 * @brief Transmit a string via USART2.
 * @param pstring Pointer to the string to transmit.
 */
void USART2_Send_String(uint8_t * pstring){
    HAL_UART_Transmit(&huart2, (uint8_t*)pstring, strlen((const char*)pstring), UART_MAX_TIMEOUT);
}

/**
 * @brief Receive a character via UART5.
 * @param pchar Pointer to store the received character.
 * @retval HAL status.
 */
HAL_StatusTypeDef UART5_Receive_Char(uint8_t *pchar) {
    HAL_StatusTypeDef stts = HAL_UART_Receive(&huart5, pchar, 1, UART_MAX_TIMEOUT);
    return stts;
}

/**
 * @brief Receive a character via USART2.
 * @param pchar Pointer to store the received character.
 * @retval HAL status.
 */
HAL_StatusTypeDef USART2_Receive_Char(uint8_t *pchar) {
    HAL_StatusTypeDef stts = HAL_UART_Receive(&huart2, pchar, 1, UART_MAX_TIMEOUT);
    return stts;
}

/**
 * @brief UART5 IRQ handler.
 */
void UART5_IRQHandler(void) {
    HAL_UART_IRQHandler(&huart5);
}

/**
 * @brief USART2 IRQ handler.
 */
void USART2_IRQHandler(void) {
    HAL_UART_IRQHandler(&huart2);
}

uint8_t nmea_incoming_buffer[NMEA_BUFFER_SIZE];
uint8_t nmea_index = 0;

uint8_t pc_incoming_buffer[PC_BUFFER_SIZE];
uint8_t pc_ready_buffer[PC_BUFFER_SIZE];
uint8_t pc_index = 0;
bool_t pc_ready = false;
bool_t receiving_data = false;

/**
 * @brief UART receive complete callback function.
 * @param huart Pointer to the UART handle.
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == UART5) {
        if (rx_char_gps == '$') {
            if (nmea_queue.count < NMEA_QUEUE_SIZE) {
                // Manually copy the incoming buffer to the queue
                for (uint8_t i = 0; i < NMEA_BUFFER_SIZE; i++) {
                    nmea_queue.buffer[nmea_queue.tail][i] = nmea_incoming_buffer[i];
                }
                nmea_queue.tail = (nmea_queue.tail + 1) % NMEA_QUEUE_SIZE;
                nmea_queue.count++;
            }
            // Clear the incoming buffer
            for (uint8_t i = 0; i < NMEA_BUFFER_SIZE; i++) {
                nmea_incoming_buffer[i] = 0;
            }
            nmea_index = 0;
            nmea_available = true;
        }
        nmea_incoming_buffer[nmea_index++] = rx_char_gps;
        // Continue receiving the next character
        HAL_UART_Receive_IT(&huart5, (uint8_t *)&rx_char_gps, 1);

    } else if (huart->Instance == USART2) {
        if (rx_char_pc == '$') {
            // Start accumulating data
            receiving_data = true;
            pc_index = 0; // Reset index to start storing the new sentence
        }

        if (receiving_data) {
            pc_incoming_buffer[pc_index++] = rx_char_pc;

            if (rx_char_pc == '\r') {
                receiving_data = false; // Stop accumulating
                pc_incoming_buffer[pc_index++] = '\0'; // Null-terminate the buffer

                // Copy to the ready buffer
                for (uint8_t i = 0; i < pc_index; i++) {
                    pc_ready_buffer[i] = pc_incoming_buffer[i];
                }
                pc_ready = true;
            }
        }

        // Continue receiving the next character
        HAL_UART_Receive_IT(&huart2, (uint8_t *)&rx_char_pc, 1);
    }
}

/**
 * @brief Check if a PC command has been received.
 * @retval true if a command is ready, false otherwise.
 */
bool_t pc_command_received(void){
    bool_t rtrn = false;
    if (pc_ready == true){
        rtrn = true;
        pc_ready = false;
    }
    return rtrn;
}

/**
 * @brief Get the last received PC command.
 * @retval Pointer to the PC command string.
 */
char* get_pc_command(void) {
    return (char*) pc_ready_buffer;
}

/**
 * @brief Check if an NMEA sentence is available.
 * @retval true if an NMEA sentence is available, false otherwise.
 */
bool_t nmea_sentence_received(void) {
    return nmea_queue.count > 0;
}

/**
 * @brief Enqueue an NMEA message.
 * @param message Pointer to the message to enqueue.
 * @retval true if the message was successfully enqueued, false if the queue is full.
 */
bool_t enqueue_nmea_message(const uint8_t* message) {
    if (nmea_queue.count < NMEA_QUEUE_SIZE) {
        // Manually copy the message to the queue buffer
        for (uint8_t i = 0; i < NMEA_BUFFER_SIZE; i++) {
            nmea_queue.buffer[nmea_queue.tail][i] = message[i];
        }
        nmea_queue.tail = (nmea_queue.tail + 1) % NMEA_QUEUE_SIZE;
        nmea_queue.count++;
        return true;
    }
    return false; // Queue is full
}

/**
 * @brief Dequeue an NMEA message.
 * @param message Pointer to store the dequeued message.
 * @retval true if the message was successfully dequeued, false if the queue is empty.
 */
bool_t dequeue_nmea_message(uint8_t* message) {
    if (nmea_queue.count > 0) {
        // Manually copy the message from the queue buffer
        for (uint8_t i = 0; i < NMEA_BUFFER_SIZE; i++) {
            message[i] = nmea_queue.buffer[nmea_queue.head][i];
        }
        nmea_queue.head = (nmea_queue.head + 1) % NMEA_QUEUE_SIZE;
        nmea_queue.count--;
        return true;
    }
    return false; // Queue is empty
}

/**
 * @brief Get the next NMEA sentence.
 * @retval Pointer to the NMEA sentence string, or NULL if no message is available.
 */
char* get_nmea_sentence(void) {
    static uint8_t message[NMEA_BUFFER_SIZE];
    if (dequeue_nmea_message(message)) {
        return (char*)message;
    }
    return NULL; // No message available
}
