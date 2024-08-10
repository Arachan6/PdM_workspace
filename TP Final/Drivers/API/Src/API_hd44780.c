#include "API_hd44780.h"
#include "API_hd44780_port.h"
#include "API_uart.h"

extern I2C_HandleTypeDef hi2c1;

static uint8_t dpFunction;    /**< Display function setting */
static uint8_t dpControl;     /**< Display control setting */
static uint8_t dpMode;        /**< Display mode setting */
static uint8_t dpRows;        /**< Number of display rows */
static uint8_t dpBacklight;   /**< Display backlight setting */

static void Send_Command(uint8_t cmd);
static void Send_Data(uint8_t data);
static void Expander_Write(uint8_t value);

/**
 * @brief Initialize the HD44780 display.
 * @param rows Number of rows on the display.
 */
void HD44780_Init(uint8_t rows) {
    // Set up global variables
    dpRows = rows;
    dpBacklight = LCD_BACKLIGHT;

    // Configuration for 4-bit mode, 2-line display, and 5x8 dots
    const uint8_t initSequence[] = { 0x03, 0x03, 0x03, 0x02 };
    dpFunction = LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS;
    dpMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

    // Initialization delay
    HAL_Delay_Wrapper(1000);

    // Execute the initialization sequence
    for (uint8_t i = 0; i < sizeof(initSequence) / sizeof(initSequence[0]); ++i) {
        Expander_Write(initSequence[i] << 4);
        HAL_Delay_Wrapper(i < 3 ? 5 : 1);  // Different delay for last step
    }

    // Set the display function
    Send_Command(LCD_FUNCTIONSET | dpFunction);

    // Set the entry mode
    Send_Command(LCD_ENTRYMODESET | dpMode);
    HAL_Delay_Wrapper(5);

    // Return cursor to home position
    HD44780_Home();
}


/**
 * @brief Set the display to home position.
 */
void HD44780_Home(){
    Send_Command(LCD_RETURNHOME);
    HAL_Delay_Wrapper(2);
}

/**
 * @brief Print a string on the display.
 * @param c String to be printed.
 */
void HD44780_Print_String(const char str[]) {
    int i = 0;
    while (str[i] != '\0') {
        Send_Data(str[i]);
        i++;
    }
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
    Send_Command(LCD_DISPLAYCONTROL | dpControl); // Send the command to the display
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
    Send_Command(LCD_DISPLAYCONTROL | dpControl); // Send the command to the display
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
    Send_Command(LCD_DISPLAYCONTROL | dpControl); // Send the command to the display
}

/**
 * @brief Set the cursor position on the display.
 * @param column Column position.
 * @param line line position.
 */
void HD44780_Move_Cursor(uint8_t column, uint8_t line) {
	// The HD44780 LCD controller has a character display RAM (DDRAM) where it stores the characters to be displayed.
	// The DDRAM is organized into rows, and each row has a base address. This supports up to 4 rows.
    const int rowAddressMap[] = { 0x00, 0x40, 0x14, 0x54 };
    uint8_t maxLine = dpRows > 0 ? dpRows - 1 : 0;

    if (line > maxLine) {
        line = maxLine;
    }

    uint8_t ddramAddress = column + rowAddressMap[line];
    Send_Command(LCD_SETDDRAMADDR | ddramAddress);
}

/**
 * @brief Clear the display.
 */
void HD44780_Clear(){
    Send_Command(LCD_CLEARDISPLAY);
    HAL_Delay_Wrapper(2);
}

/**
 * @brief Send data to the display.
 * @param data Data to be sent.
 */
static void Send_Data(uint8_t data){
    uint8_t highnib = data & 0xF0;
    uint8_t lownib = (data << 4) & 0xF0;
    Expander_Write(highnib | RS);
    Expander_Write(lownib | RS);
}

/**
 * @brief Send a command to the display.
 * @param cmd Command to be sent.
 */
static void Send_Command(uint8_t cmd){
    uint8_t highnib = cmd & 0xF0;
    uint8_t lownib = (cmd << 4) & 0xF0;
    Expander_Write(highnib);
    Expander_Write(lownib);
}

/**
 * @brief Write a value to the I2C expander.
 * @param value Value to be written.
 */
static void Expander_Write(uint8_t value)
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
