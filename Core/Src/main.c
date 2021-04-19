/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "usb_host.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern ApplicationTypeDef Appli_state;
extern char USBHPath[4];
FATFS myUSBFatFS;
FIL myFile;
FRESULT myRes;
UINT byteswritten, bytesread;
char rwtext[100];
bool USBTestWrite(void);
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */
void button_check(void);
bool USBTestWrite(void);
bool USBTestRead(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_FATFS_Init();
	MX_USB_HOST_Init();
	/* USER CODE BEGIN 2 */

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */
		MX_USB_HOST_Process();

		/* USER CODE BEGIN 3 */
		switch (Appli_state) {
		case APPLICATION_IDLE:
			break;
		case APPLICATION_START:
			/*Register the file system object to the FATFS module */
			if (f_mount(&myUSBFatFS, (TCHAR const*) USBHPath, 0) != FR_OK) {
				/*FATFS Initialization Error*/
				Error_Handler();
			} else {
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET); //Green led
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET); //Red led
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET); //Blue led
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET); //Orange led
			}
			Appli_state = APPLICATION_IDLE;
			break;
		case APPLICATION_READY:
			if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) {
				if (USBTestWrite()) {
					if(USBTestRead()){
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET); //Red led
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET); //Blue led
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET); //Orange led
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET); //Blue led
					}
					else {
						HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET); //Red led
						HAL_Delay(1000);
					}
				} else {
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET); //Red led
					HAL_Delay(1000);
				}
			}
			break;
		case APPLICATION_DISCONNECT:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
			break;

		}
	}
	/* USER CODE END 3 */
}

//USB test write function
bool USBTestWrite(void) {
	char *str =
			"<!DOCTYPE html>"
					"\n<html>"
					"\n<head>"
					"\n<meta charset=\"utf-8<\">"
					"\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
					"\n<title>Page Not Found</title>"
					"\n<style media=\"screen\">"
					"\nbody { background: #ECEFF1; color: rgba(0,0,0,0.87); font-family: Roboto, Helvetica, Arial, sans-serif; margin: 0; padding: 0; }"
					"\n#message { background: white; max-width: 360px; margin: 100px auto 16px; padding: 32px 24px 16px; border-radius: 3px; }"
					"\n#message h3 { color: #888; font-weight: normal; font-size: 16px; margin: 16px 0 12px; }"
					"\n#message h2 { color: #ffa100; font-weight: bold; font-size: 16px; margin: 0 0 8px; }"
					"\n#message h1 { font-size: 22px; font-weight: 300; color: rgba(0,0,0,0.6); margin: 0 0 16px;}"
					"\n#message p { line-height: 140%; margin: 16px 0 24px; font-size: 14px; }"
					"\n#message a { display: block; text-align: center; background: #039be5; text-transform: uppercase; text-decoration: none; color: white; padding: 16px; border-radius: 4px; }"
					"\n#message, #message a { box-shadow: 0 1px 3px rgba(0,0,0,0.12), 0 1px 2px rgba(0,0,0,0.24); }"
					"\n#load { color: rgba(0,0,0,0.4); text-align: center; font-size: 13px; }"
					"\n@media (max-width: 600px) {"
					"\nbody, #message { margin-top: 0; background: white; box-shadow: none; }"
					"\nbody { border-top: 16px solid #ffa100; }"
					"\n}"
					"\n</style>"
					"\n</head>"
					"\n<body>"
					"\n<div id=\"message\">"
					"\n<h2>404</h2>"
					"\n<h1>Page Not Found</h1>"
					"\n<p>The specified file was not found on this website. Please check the URL for mistakes and try again.</p>"
					"\n<h3>Why am I seeing this?</h3>"
					"\n<p>This page was generated by the Firebase Command-Line Interface. To modify it, edit the <code>404.html</code> file in your project's configured <code>public</code> directory.</p>"
					"\n</div>"
					"\n</body>"
					"\n</html>";
	FRESULT resultado;
	resultado = f_open(&myFile, "test2.txt", FA_CREATE_ALWAYS | FA_WRITE);
	if (resultado != FR_OK) {
		return 0;
	} else {
		sprintf(rwtext, str);
		myRes = f_write(&myFile, (uint8_t*) rwtext, strlen(rwtext),
				&byteswritten);
		if ((byteswritten == 0) && (myRes != FR_OK)) {
			return 0;
		}
	}
	f_close(&myFile);
	return 1;
}

bool USBTestRead(void) {
	FRESULT resultado;
	resultado = f_open(&myFile, "test2.txt", FA_READ);
	if (resultado != FR_OK) {
		return 0;
	} else {
		for (uint8_t i = 0; i < 100; i++) {
			myRes = f_read(&myFile, (uint8_t*) &rwtext[i], 1, &bytesread);
			if (rwtext[i] == 0x00) {
				bytesread = 1;
				break;
			}
		}
		if (bytesread == 0) {
			return 0;
		}
	}

	f_close(&myFile);
	return 1;
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(OTG_PowerSwitch_GPIO_Port, OTG_PowerSwitch_Pin,
			GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOD,
	GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);

	/*Configure GPIO pin : OTG_PowerSwitch_Pin */
	GPIO_InitStruct.Pin = OTG_PowerSwitch_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(OTG_PowerSwitch_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : PA0 */
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : PD12 PD13 PD14 PD15 */
	GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
