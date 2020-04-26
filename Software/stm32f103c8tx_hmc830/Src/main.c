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

void HMC830_Write_265M(void)
{
	HMC830_HMC_Write(HMC830_REG01H_ENABLE, HMC830_REG01H_CHIPEN_FROM_SPI_MASK);
	
	HMC830_HMC_Write(HMC830_REG02H_REFDIV, 1);  // Rdiv = 1
    
	HMC830_HMC_Write(HMC830_REG05H_VCO_SPI, 0x6510);
//4	reg05h 6510h	[2:0] = 000 HMC830LP6GE
//								[6:3] = 0010 VCO_Reg02h
//								[11:7] = 001000 vco output devider = 8, max gain
//								[15:12] = 011
	
	HMC830_HMC_Write(HMC830_REG05H_VCO_SPI, 0x898);
//5	reg05h 898h		single output RF_N
	
	HMC830_HMC_Write(HMC830_REG05H_VCO_SPI, HMC830_REG05H_VCO_SUBSYSTEM_ID_WB);
//6 reg05h 0h			enable all bands
	
	HMC830_HMC_Write(HMC830_REG06H_SD_CFG,0x307ca);
//7 reg06h 307CAh	PFD:int ,mode B		0011 0000 0111 1100 1010
////7 reg06h F4Ah           1111 0100 1010

	HMC830_HMC_Write(HMC830_REG07H_LOCK_DETECT,0xccd);
//8 reg07h CCDh 	lock detect window
	
	HMC830_HMC_Write(HMC830_REG09H_CHARGE_PUMP,0x376E);
//a	reg09h 376Eh	PFD gain
	
	HMC830_HMC_Write(HMC830_REG0AH_VCO_AUTOCAL_CFG,0x2046);
//b reg0Ah 2046h	
	
	HMC830_HMC_Write(HMC830_REG0BH_PD,0x7c061);
//c reg0Bh 7C061h
	
	HMC830_HMC_Write(HMC830_REG0FH_GPO_SPI_RDIV,0x81);
//d reg0Fh 81h		locked_sign output

	HMC830_HMC_Write(HMC830_REG03H_INTEGER_PART,50);
//e reg03h 35h			Ndevider=50
//
}


void HMC830_Test_25M(void)
{
    HMC830_HMC_Write(HMC830_REG02H_REFDIV, 0x1);
    HMC830_HMC_Write(HMC830_REG06H_SD_CFG, 0x2003CA);
    HMC830_HMC_Write(HMC830_REG07H_LOCK_DETECT, 0xCCD);
    HMC830_HMC_Write(HMC830_REG08H_ANALOG_EN, 0xC1BEFF);
    HMC830_HMC_Write(HMC830_REG09H_CHARGE_PUMP, 0x5B3FFF);
    HMC830_HMC_Write(HMC830_REG0AH_VCO_AUTOCAL_CFG, 0x2046);
    HMC830_HMC_Write(HMC830_REG0BH_PD, 0x7C061);
    HMC830_HMC_Write(HMC830_REG0FH_GPO_SPI_RDIV, 0x81);
    HMC830_HMC_Write(HMC830_REG05H_VCO_SPI, 0x1628);
    HMC830_HMC_Write(HMC830_REG05H_VCO_SPI, 0x60A0);
    //---
    HMC830_HMC_Write(HMC830_REG05H_VCO_SPI, 0xFF10);
    HMC830_HMC_Write(HMC830_REG05H_VCO_SPI, 0x0);
    HMC830_HMC_Write(HMC830_REG03H_INTEGER_PART, 0x1F);
    HMC830_HMC_Write(HMC830_REG04H_FACTIONAL_PART, 0x0);
}

void HMC830_Test_35M(void)
{
    HMC830_HMC_Write(HMC830_REG02H_REFDIV, 0x1);
    HMC830_HMC_Write(HMC830_REG06H_SD_CFG, 0x200B4A);
    HMC830_HMC_Write(HMC830_REG07H_LOCK_DETECT, 0xCCD);
    HMC830_HMC_Write(HMC830_REG08H_ANALOG_EN, 0xC1BEFF);
    HMC830_HMC_Write(HMC830_REG09H_CHARGE_PUMP, 0x5B3FFF);
    HMC830_HMC_Write(HMC830_REG0AH_VCO_AUTOCAL_CFG, 0x2046);
    HMC830_HMC_Write(HMC830_REG0BH_PD, 0x7C021);
    HMC830_HMC_Write(HMC830_REG0FH_GPO_SPI_RDIV, 0x81);
    HMC830_HMC_Write(HMC830_REG05H_VCO_SPI, 0x1628);
    HMC830_HMC_Write(HMC830_REG05H_VCO_SPI, 0x60A0);
    //---
    HMC830_HMC_Write(HMC830_REG05H_VCO_SPI, 0xFF10);
    HMC830_HMC_Write(HMC830_REG05H_VCO_SPI, 0x0);
    HMC830_HMC_Write(HMC830_REG03H_INTEGER_PART, 0x2B);
    HMC830_HMC_Write(HMC830_REG04H_FACTIONAL_PART, 0x666666);
}


void HMC830_Test_100M(void)
{
    HMC830_HMC_Write(HMC830_REG02H_REFDIV, 0x1);
    HMC830_HMC_Write(HMC830_REG06H_SD_CFG, 0x2003CA);
    HMC830_HMC_Write(HMC830_REG07H_LOCK_DETECT, 0xCCD);
    HMC830_HMC_Write(HMC830_REG08H_ANALOG_EN, 0xC1BEFF);
    HMC830_HMC_Write(HMC830_REG09H_CHARGE_PUMP, 0x5B3FFF);
    HMC830_HMC_Write(HMC830_REG0AH_VCO_AUTOCAL_CFG, 0x2046);
    HMC830_HMC_Write(HMC830_REG0BH_PD, 0x7C061);
    HMC830_HMC_Write(HMC830_REG0FH_GPO_SPI_RDIV, 0x81);
    HMC830_HMC_Write(HMC830_REG05H_VCO_SPI, 0x1628);
    HMC830_HMC_Write(HMC830_REG05H_VCO_SPI, 0x60A0);
    //---
    HMC830_HMC_Write(HMC830_REG05H_VCO_SPI, 0xEF10);
    HMC830_HMC_Write(HMC830_REG05H_VCO_SPI, 0x0);
    HMC830_HMC_Write(HMC830_REG03H_INTEGER_PART, 0x3C);
    HMC830_HMC_Write(HMC830_REG04H_FACTIONAL_PART, 0x0);
}

void HMC830_Test_425M(void)
{
    HMC830_HMC_Write(HMC830_REG02H_REFDIV, 0x1);
    HMC830_HMC_Write(HMC830_REG06H_SD_CFG, 0x2003CA);
    HMC830_HMC_Write(HMC830_REG07H_LOCK_DETECT, 0xCCD);
    HMC830_HMC_Write(HMC830_REG08H_ANALOG_EN, 0xC1BEFF);
    HMC830_HMC_Write(HMC830_REG09H_CHARGE_PUMP, 0x5B3FFF);
    HMC830_HMC_Write(HMC830_REG0AH_VCO_AUTOCAL_CFG, 0x2046);
    HMC830_HMC_Write(HMC830_REG0BH_PD, 0x7C061);
    HMC830_HMC_Write(HMC830_REG0FH_GPO_SPI_RDIV, 0x81);
    HMC830_HMC_Write(HMC830_REG05H_VCO_SPI, 0x1628);
    HMC830_HMC_Write(HMC830_REG05H_VCO_SPI, 0x60A0);
    //---
    HMC830_HMC_Write(HMC830_REG05H_VCO_SPI, 0xE310);
    HMC830_HMC_Write(HMC830_REG05H_VCO_SPI, 0x0);
    HMC830_HMC_Write(HMC830_REG03H_INTEGER_PART, 0x33);
    HMC830_HMC_Write(HMC830_REG04H_FACTIONAL_PART, 0x0);
}

void HMC830_Test_650M(void)
{
    HMC830_HMC_Write(HMC830_REG02H_REFDIV, 0x1);
    HMC830_HMC_Write(HMC830_REG06H_SD_CFG, 0x2003CA);
    HMC830_HMC_Write(HMC830_REG07H_LOCK_DETECT, 0xCCD);
    HMC830_HMC_Write(HMC830_REG08H_ANALOG_EN, 0xC1BEFF);
    HMC830_HMC_Write(HMC830_REG09H_CHARGE_PUMP, 0x5B3FFF);
    HMC830_HMC_Write(HMC830_REG0AH_VCO_AUTOCAL_CFG, 0x2046);
    HMC830_HMC_Write(HMC830_REG0BH_PD, 0x7C061);
    HMC830_HMC_Write(HMC830_REG0FH_GPO_SPI_RDIV, 0x81);
    HMC830_HMC_Write(HMC830_REG05H_VCO_SPI, 0x1628);
    HMC830_HMC_Write(HMC830_REG05H_VCO_SPI, 0x60A0);
    //---
    HMC830_HMC_Write(HMC830_REG05H_VCO_SPI, 0xE210);
    HMC830_HMC_Write(HMC830_REG05H_VCO_SPI, 0x0);
    HMC830_HMC_Write(HMC830_REG03H_INTEGER_PART, 0x34);
    HMC830_HMC_Write(HMC830_REG04H_FACTIONAL_PART, 0x0);
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
    //HMC830_Test_425M();
    HMC830_Test_650M();
    //HMC830_Write_265M();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      
    uint32_t id = HMC830_HMC_Read(HMC830_REG00H_ID);
    uint32_t gpo2 = HMC830_HMC_Read(HMC830_REG12H_GPO2);
      
    char UserTxBuffer[32];
    memset(UserTxBuffer, 0, sizeof(UserTxBuffer));
    sprintf(UserTxBuffer,"hmc830 id:0x%X\r\nLD:%x\r\n",id,gpo2);
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
