/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdbool.h>
#include "liquidcrystal_i2c.h"

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);


int main(void)
{
	HAL_Init();


	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_I2C1_Init();

	if (HAL_I2C_IsDeviceReady(&hi2c1, DEVICE_ADDR, 10, HAL_MAX_DELAY) != HAL_OK)
	{
		// Device is not ready
		Error_Handler();
	}


	/* Initialize */
	HD44780_Init(2);

	/* Clear buffer */
	//HD44780_Clear();

	/* Hide characters */
	//HD44780_NoDisplay();
	HD44780_Cursor();
	HD44780_SetCursor(0,0);
	HD44780_PrintStr("HEaaa STM32!!!");
	HD44780_PrintSpecialChar(0);

	/* Show characters */
	HD44780_Display();

	/* Move position */
	HD44780_SetCursor(0, 1);
	HD44780_PrintStr("BYE STM32!!!");
	HD44780_PrintSpecialChar(1);

	/* Blink cursor */
	HD44780_Blink();

	HAL_Delay(2000);
	HD44780_Clear();
	HAL_Delay(2000);
	HD44780_SetCursor(0, 2);
	HAL_Delay(2000);
	HD44780_SetCursor(5, 2);
	HAL_Delay(2000);
	HD44780_SetCursor(5, 1);
	HD44780_PrintStr("B");

	/* Infinite loop */
	while (1)
	{

	}
}


static void MX_I2C1_Init(void)
{
    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 100000;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        Error_Handler();
    }
}

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    __HAL_RCC_I2C1_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}


static void Error_Handler(void)
{
	/* Turn LED2 on */
	BSP_LED_On(LED2);
	while (1)
	{
	}
}


void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}


