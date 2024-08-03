#include "stm32f4xx_hal.h"
#include "API_uart.h"
#include "API_gps.h"
#include "API_i2c.h"
#include "API_hd44780.h"
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




    //HAL_Delay(500);
    GPS_Set_Update_Rate(2000); // Devuelve $PMTK001,220,3*30
    //HAL_Delay(500);
    GPS_Start_Logging(); // Devuelve $PMTK001,185,3*3C

    void GPS_Dump_Flash_Data();
    //HAL_Delay(5000);
    while (1){
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
    	}
    }
}

void SystemClock_Config(void) {
}

void Error_Handler(void){
    while (1){}
}


