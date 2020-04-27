/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"
#include "hmc830.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
void HMC830_Test_25M(void)
{
    HMC830_HMC_Write_REFDIV(1);
    HMC830_HMC_Write_PFD_General_Setting(HMC830_INTEGER_MODE);
    HMC830_HMC_Write_Charge_Pump_Current(2.54, HMC830_INTEGER_MODE, 1550, 50);
    HMC830_HMC_Write_VCO_General_Setting(62, HMC830_VCO_REG02H_GAIN_3);
    HMC830_HMC_Write_NDIV(31.0);
}

void HMC830_Test_35M(void)
{
    HMC830_HMC_Write_REFDIV(1);
    HMC830_HMC_Write_PFD_General_Setting(HMC830_FRACTIONAL_MODE);
    HMC830_HMC_Write_Charge_Pump_Current(2.54, HMC830_FRACTIONAL_MODE, 2170, 50);
    HMC830_HMC_Write_VCO_General_Setting(62, HMC830_VCO_REG02H_GAIN_3);
    HMC830_HMC_Write_NDIV(43.4);
}


void HMC830_Test_100M(void)
{
    HMC830_HMC_Write_REFDIV(1);
    HMC830_HMC_Write_PFD_General_Setting(HMC830_INTEGER_MODE);
    HMC830_HMC_Write_Charge_Pump_Current(2.54, HMC830_INTEGER_MODE, 3000, 50);
    HMC830_HMC_Write_VCO_General_Setting(30, HMC830_VCO_REG02H_GAIN_3);
    HMC830_HMC_Write_NDIV(60.0);
}

void HMC830_Test_425M(void)
{
    HMC830_HMC_Write_REFDIV(1);
    HMC830_HMC_Write_PFD_General_Setting(HMC830_INTEGER_MODE);
    HMC830_HMC_Write_Charge_Pump_Current(2.54, HMC830_INTEGER_MODE, 2550, 50);
    HMC830_HMC_Write_VCO_General_Setting(6, HMC830_VCO_REG02H_GAIN_3);
    HMC830_HMC_Write_NDIV(51.0);
}

void HMC830_Test_650M(void)
{
    HMC830_HMC_Write_REFDIV(1);
    HMC830_HMC_Write_PFD_General_Setting(HMC830_INTEGER_MODE);
    HMC830_HMC_Write_Charge_Pump_Current(2.54, HMC830_INTEGER_MODE, 2600, 50);
    HMC830_HMC_Write_VCO_General_Setting(4, HMC830_VCO_REG02H_GAIN_3);
    HMC830_HMC_Write_NDIV(52.0);
}
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
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
    USB_RePlug();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */
  
    HMC830_Init(HMC830_HMC_MODE);
    //HMC830_Test_25M();
    //HMC830_Test_35M();
    //HMC830_Test_100M();
    HMC830_Test_425M();
    //HMC830_Test_650M();
    
    //HMC830_HMC_Write_Output_Mode(HMC830_OUTPUT_DIFFERENTIAL_MODE);
    HMC830_HMC_Write_Output_Mode(HMC830_OUTPUT_SINGLE_ENDED_MODE);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    uint32_t id = HMC830_HMC_Read_Chip_ID();
    uint32_t lock = HMC830_HMC_Read_Lock_Detect();
      
    char UserTxBuffer[32];
    memset(UserTxBuffer, 0, sizeof(UserTxBuffer));
      sprintf(UserTxBuffer,"HMC830 ID:0x%X\r\nLD:%s\r\n", id, lock == HMC830_LOCKED ? "LOCKED" : "UNLOCKED");
    CDC_Transmit_FS((uint8_t*)UserTxBuffer,sizeof(UserTxBuffer));
      
    HAL_Delay(1000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
