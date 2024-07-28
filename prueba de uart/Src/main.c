#include "stm32f4xx_hal.h"
#include "API_uart.h"
#include "API_gps.h"
#include "API_i2c.h"
#include "API_hd44780.h"

/* Function prototypes */
void SystemClock_Config(void);

#define DEBUG true

int main(void){
    HAL_Init();
    SystemClock_Config();
    UART5_Init();
    USART2_Init();
    I2C1_Init();
	HD44780_Init(2);
	//HD44780_Clear(); // Clear buffer

	//HD44780_Set_Display(true);
	//HD44780_Set_Cursor(true);
	//HD44780_Set_Blink(true);

	//HD44780_Cursor_Position(0, 0);

	//HD44780_PrintStr("Ooooh!!!");











    while (1){
    	if (nmea_sentence_received() == true){
    		char* nmea_sentence = get_nmea_sentence();
			#if DEBUG
    			USART2_Send_String((uint8_t*)"(DEBUG) NMEA Recibido: ");
				USART2_Send_String((uint8_t*)nmea_sentence);
			#endif
			Parse_NMEA_Sentence(nmea_sentence);
    	}
    }
}

void SystemClock_Config(void){
}

void Error_Handler(void){
    while (1){}
}

