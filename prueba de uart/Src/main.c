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
    HAL_Delay(500);


    UART5_Send_String((uint8_t*)"$PMTK220,2000*1C\r\n");
    char dest[100] = "$PMTK220,";
	const char *src = "1000*1F\r\n";

	// Concatenate src to dest
	String_Concat(dest, src);
    UART5_Send_String((uint8_t*)dest);



    while (1){
    	if (nmea_sentence_received() == true){
    		char* nmea_sentence = get_nmea_sentence();
			#if DEBUG
    			USART2_Send_String((uint8_t*)"(DEBUG) NMEA Received: ");
				USART2_Send_String((uint8_t*)nmea_sentence);
			#endif
			if (Parse_NMEA_Sentence(nmea_sentence)){
				nmeaData = Get_NMEA_Data();
				USART2_Send_String((uint8_t*)"(DEBUG) NMEA Processed: Fix Quality: ");
				USART2_Send_String((uint8_t*)nmeaData->fixQuality);
				USART2_Send_String((uint8_t*)"\r\n");
			}
    	}
    }
}

void SystemClock_Config(void) {
}

void Error_Handler(void){
    while (1){}
}


