#include "API_hd44780.h"
#include "API_hd44780_port.h"
#include "API_uart.h"

extern I2C_HandleTypeDef hi2c1;

uint8_t dpFunction;    /**< Display function setting */
uint8_t dpControl;     /**< Display control setting */
uint8_t dpMode;        /**< Display mode setting */
uint8_t dpRows;        /**< Number of display rows */
uint8_t dpBacklight;   /**< Display backlight setting */

static void SendCommand(uint8_t cmd);
static void SendData(uint8_t data);
static void ExpanderWrite(uint8_t value);
static void DelayUS(uint32_t us);
static void SetCursor(uint8_t col, uint8_t row);

/**
 * @brief Initialize the HD44780 display.
 * @param rows Number of rows on the display.
 */
void HD44780_Init(uint8_t rows){
    dpRows = rows;
    dpBacklight = LCD_BACKLIGHT;

    dpFunction = LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS;

    /* Wait for initialization */
    HAL_Delay_Wrapper(1000);

    /* 4bit Mode */
    ExpanderWrite(0x03 << 4);
    HAL_Delay_Wrapper(5);

    ExpanderWrite(0x03 << 4);
    HAL_Delay_Wrapper(5);

    ExpanderWrite(0x03 << 4);
    HAL_Delay_Wrapper(5);

    ExpanderWrite(0x02 << 4);
    HAL_Delay_Wrapper(1);

    /* Display Control */
    SendCommand(LCD_FUNCTIONSET | dpFunction);

    dpControl = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKON;
    HD44780_Set_Display(true);
    HD44780_Clear();

    /* Display Mode */
    dpMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    SendCommand(LCD_ENTRYMODESET | dpMode);
    HAL_Delay_Wrapper(5);

    HD44780_Home();
}

/**
 * @brief Set the display to home position.
 */
void HD44780_Home(){
    SendCommand(LCD_RETURNHOME);
    HAL_Delay_Wrapper(2);
}

/**
 * @brief Print a string on the display.
 * @param c String to be printed.
 */
void HD44780_PrintStr(const char c[]){
    while(*c) SendData(*c++);
}

/**
 * @brief Set the display on or off.
 * @param displayOn Boolean indicating display on (true) or off (false).
 */
void HD44780_Set_Display(bool_t displayOn) {
    if (displayOn) {
        dpControl |= LCD_DISPLAYON;  // Set the display on flag
    } else {
        dpControl &= ~LCD_DISPLAYON; // Clear the display on flag
    }
    SendCommand(LCD_DISPLAYCONTROL | dpControl); // Send the command to the display
}

/**
 * @brief Set the blink mode on or off.
 * @param blinkOn Boolean indicating blink on (true) or off (false).
 */
void HD44780_Set_Blink(bool_t blinkOn) {
    if (blinkOn) {
        dpControl |= LCD_BLINKON;  // Set the blink on flag
    } else {
        dpControl &= ~LCD_BLINKON; // Clear the blink on flag
    }
    SendCommand(LCD_DISPLAYCONTROL | dpControl); // Send the command to the display
}

/**
 * @brief Set the cursor mode on or off.
 * @param cursorOn Boolean indicating cursor on (true) or off (false).
 */
void HD44780_Set_Cursor(bool_t cursorOn) {
    if (cursorOn) {
        dpControl |= LCD_CURSORON;  // Set the cursor on flag
    } else {
        dpControl &= ~LCD_CURSORON; // Clear the cursor on flag
    }
    SendCommand(LCD_DISPLAYCONTROL | dpControl); // Send the command to the display
}

/**
 * @brief Set the cursor position on the display.
 * @param col Column position.
 * @param row Row position.
 */
void HD44780_Cursor_Position(uint8_t col, uint8_t row){
    int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    if (row >= dpRows){
        row = dpRows-1;
    }
    SendCommand(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

/**
 * @brief Clear the display.
 */
void HD44780_Clear(){
    SendCommand(LCD_CLEARDISPLAY);
    HAL_Delay_Wrapper(2);
}

/**
 * @brief Send data to the display.
 * @param data Data to be sent.
 */
static void SendData(uint8_t data){
    uint8_t highnib = data & 0xF0;
    uint8_t lownib = (data << 4) & 0xF0;
    ExpanderWrite(highnib | RS);
    ExpanderWrite(lownib | RS);
}

/**
 * @brief Send a command to the display.
 * @param cmd Command to be sent.
 */
static void SendCommand(uint8_t cmd){
    uint8_t highnib = cmd & 0xF0;
    uint8_t lownib = (cmd << 4) & 0xF0;
    ExpanderWrite(highnib);
    ExpanderWrite(lownib);
}

/**
 * @brief Write a value to the I2C expander.
 * @param value Value to be written.
 */
static void ExpanderWrite(uint8_t value)
{
    uint8_t data = value | dpBacklight;
    I2C_Master_Transmit_Wrapper(&hi2c1, DEVICE_ADDR, (uint8_t*)&data, 1, 10);

    data = (value | ENABLE) | dpBacklight;
    I2C_Master_Transmit_Wrapper(&hi2c1, DEVICE_ADDR, (uint8_t*)&data, 1, 10);
    HAL_Delay_Wrapper(1);

    data = (value & ~ENABLE) | dpBacklight;
    I2C_Master_Transmit_Wrapper(&hi2c1, DEVICE_ADDR, (uint8_t*)&data, 1, 10);
    HAL_Delay_Wrapper(1);
}
