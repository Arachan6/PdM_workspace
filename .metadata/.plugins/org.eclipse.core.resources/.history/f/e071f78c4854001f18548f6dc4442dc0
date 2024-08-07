#include "API_hd44780.h"
#include "API_hd44780_port.h"
#include "API_uart.h"

extern I2C_HandleTypeDef hi2c1;

uint8_t dpFunction;
uint8_t dpControl;
uint8_t dpMode;
uint8_t dpRows;
uint8_t dpBacklight;

static void SendCommand(uint8_t);
static void SendData(uint8_t);
static void ExpanderWrite(uint8_t);
static void DelayUS(uint32_t);
static void SetCursor(uint8_t, uint8_t);

void HD44780_Init(uint8_t rows){
	dpRows = rows;
	dpBacklight = LCD_BACKLIGHT;

	dpFunction = LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS;

	/* Wait for initialization */
	HAL_Delay(1000);

	/* 4bit Mode */
	ExpanderWrite(0x03 << 4);
	HAL_Delay(5);
	DelayUS(4500);

	ExpanderWrite(0x03 << 4);
	DelayUS(4500);

	ExpanderWrite(0x03 << 4);
	DelayUS(4500);

	ExpanderWrite(0x02 << 4);
	DelayUS(100);

	/* Display Control */
	SendCommand(LCD_FUNCTIONSET | dpFunction);

	dpControl = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKON;
	HD44780_Set_Display(true);
	HD44780_Clear();

	/* Display Mode */
	dpMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	SendCommand(LCD_ENTRYMODESET | dpMode);
	DelayUS(4500);

	HD44780_Home();
}

void HD44780_Home(){
	SendCommand(LCD_RETURNHOME);
	DelayUS(2000);
}

void HD44780_PrintStr(const char c[]){
	while(*c) SendData(*c++);
}

void HD44780_Set_Display(bool_t displayOn) {
    if (displayOn) {
        dpControl |= LCD_DISPLAYON;  // Set the display on flag
    } else {
        dpControl &= ~LCD_DISPLAYON; // Clear the display on flag
    }
    SendCommand(LCD_DISPLAYCONTROL | dpControl); // Send the command to the display
}

void HD44780_Set_Blink(bool_t blinkOn) {
    if (blinkOn) {
        dpControl |= LCD_BLINKON;  // Set the blink on flag
    } else {
        dpControl &= ~LCD_BLINKON; // Clear the blink on flag
    }
    SendCommand(LCD_DISPLAYCONTROL | dpControl); // Send the command to the display
}

void HD44780_Set_Cursor(bool_t cursorOn) {
    if (cursorOn) {
        dpControl |= LCD_CURSORON;  // Set the cursor on flag
    } else {
        dpControl &= ~LCD_CURSORON; // Clear the cursor on flag
    }
    SendCommand(LCD_DISPLAYCONTROL | dpControl); // Send the command to the display
}

void HD44780_Cursor_Position(uint8_t col, uint8_t row){
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if (row >= dpRows){
		row = dpRows-1;
	}
	SendCommand(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void HD44780_Clear(){
	SendCommand(LCD_CLEARDISPLAY);
	DelayUS(2000);
}

static void SendData(uint8_t cmd){
	uint8_t highnib = cmd & 0xF0;
	uint8_t lownib = (cmd<<4) & 0xF0;
	ExpanderWrite(highnib | RS);
	ExpanderWrite(lownib | RS);
}

static void SendCommand(uint8_t cmd){
	uint8_t highnib = cmd & 0xF0;
	uint8_t lownib = (cmd<<4) & 0xF0;
	ExpanderWrite(highnib);
	ExpanderWrite(lownib);
}

static void ExpanderWrite(uint8_t value)
{
	uint8_t data = value | dpBacklight;
	I2C_Master_Transmit_Wrapper(&hi2c1, DEVICE_ADDR, (uint8_t*)&data, 1, 10);

	data = (value | ENABLE) | dpBacklight;
	I2C_Master_Transmit_Wrapper(&hi2c1, DEVICE_ADDR, (uint8_t*)&data, 1, 10);
	DelayUS(20);

	data = (value & ~ENABLE) | dpBacklight;
	I2C_Master_Transmit_Wrapper(&hi2c1, DEVICE_ADDR, (uint8_t*)&data, 1, 10);
	DelayUS(20);
}
/*
static void DelayUS(uint32_t us) {
	uint32_t cycles = (SystemCoreClock/1000000L)*us;
	uint32_t start = DWT->CYCCNT;
	volatile uint32_t cnt;

	do{
		cnt = DWT->CYCCNT - start;
	} while(cnt < cycles);
}

*/


void DWT_Init(void) {
    // Enable the DWT (Data Watchpoint and Trace) clock
    if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk)) {
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // Enable Trace and Debug
    }
    DWT->CYCCNT = 0; // Reset the cycle counter
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; // Enable the cycle counter
}

uint32_t DWT_GetMicroseconds(void) {
    // Get the current count from the cycle counter
    uint32_t cycles = DWT->CYCCNT;

    // Convert cycles to microseconds
    return (cycles / (SystemCoreClock / 1000000)); // SystemCoreClock is the system clock frequency in Hz
}

void delayMicroseconds(uint32_t us) {
    uint32_t start = DWT_GetMicroseconds();
    while (DWT_GetMicroseconds() - start < us) {
        // Busy-wait for the desired number of microseconds
    }
}

static void DelayUS(uint32_t us) {
	DWT_Init();
	delayMicroseconds(us);
}
