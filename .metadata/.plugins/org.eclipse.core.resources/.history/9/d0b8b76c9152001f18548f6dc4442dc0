#include "stm32f4xx_hal.h"
#include "API_uart.h"
#include "API_gps.h"
#include "API_i2c.h"
#include "API_hd44780.h"
#include "API_pc.h"
#include "utils.h"

/* Function prototypes */
void SystemClock_Config(void);
void Error_Handler(void);

#define DEBUG true

int main(void){

	NMEAData* nmeaData;

    HAL_Init();
    SystemClock_Config();
    I2C1_Init();
    gpsFSM_init();
	HD44780_Init(2);
	HD44780_Clear();

	HD44780_Set_Display(true);
	HD44780_Set_Cursor(true);
	HD44780_Set_Blink(true);
	HD44780_Cursor_Position(0, 0);
	HD44780_PrintStr("LCD Initialized!");

	HAL_Delay(500);
	HD44780_Clear();
	HD44780_Cursor_Position(0, 0);
	if (UART5_Init()){
		HD44780_PrintStr("GPS UART");
		HD44780_Cursor_Position(0, 1);
		HD44780_PrintStr("Initialized!");
	} else {
		HD44780_PrintStr("GPS UART Error!");
		Error_Handler();
	}

	HAL_Delay(500);
	HD44780_Clear();
	HD44780_Cursor_Position(0, 0);
    if (USART2_Init()){
    	HD44780_PrintStr("PC UART");
		HD44780_Cursor_Position(0, 1);
		HD44780_PrintStr("Initialized!");
    } else {
    	HD44780_PrintStr("PC UART Error!");
    	Error_Handler();
    }

    HAL_Delay(500);
	HD44780_Clear();
	HD44780_Cursor_Position(0, 0);
	HD44780_PrintStr("Configuring GPS");
	GPS_Configure_Init();
	HD44780_Clear();
	HD44780_Cursor_Position(0, 0);
	HD44780_PrintStr("GPS Configured!");

	HAL_Delay(500);
	HD44780_Clear();
	HD44780_Cursor_Position(0, 0);
	HD44780_PrintStr("Processing Data");
	HD44780_Cursor_Position(0, 1);
	HD44780_PrintStr("State: IDLE");
	HAL_Delay(500);

    while (1){
    	gpsFSM_update();
    	if (nmea_sentence_received() == true){
    		char* nmea_sentence = get_nmea_sentence();
			#if DEBUG
    			USART2_Send_String((uint8_t*)"(DEBUG) NMEA Received: ");
				USART2_Send_String((uint8_t*)nmea_sentence);
			#endif
			if (Parse_NMEA_Sentence(nmea_sentence)){
				nmeaData = Get_NMEA_Data();
			#if DEBUG
				USART2_Send_String((uint8_t*)"(DEBUG) NMEA Processed: Fix Quality: ");
				USART2_Send_String((uint8_t*)nmeaData->fixQuality);
				USART2_Send_String((uint8_t*)"\r\n");
			#endif
			}
    	} if (pc_command_received() == true){
    		char* pc_command = get_pc_command();
			#if DEBUG
    			USART2_Send_String((uint8_t*)"(DEBUG) PC Command Received: ");
				USART2_Send_String((uint8_t*)pc_command);
			#endif
			Process_PC_Command(pc_command);
    	}
    }
}

void SystemClock_Config(void) {
}

void Error_Handler(void){
    while (1){}
}


