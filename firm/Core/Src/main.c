/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"
#include <string.h>
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

OSPI_HandleTypeDef hospi1;
MDMA_HandleTypeDef hmdma_octospi1_fifo_th;

/* USER CODE BEGIN PV */
uint8_t aTxBuffer[BUFFERSIZE]  __attribute__((section(".DATA_RAM_SRD")));
uint8_t aRxBuffer[2][BUFFERSIZE]  __attribute__((section(".DATA_RAM_SRD")));
uint32_t aRxLen[2];
uint32_t aRxPtr;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_MDMA_Init(void);
static void MX_OCTOSPI1_Init(void);
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
	char str[256];
  /* USER CODE END 1 */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

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
  MX_MDMA_Init();
  MX_OCTOSPI1_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  OSPI_RegularCmdTypeDef sCommand;
  sCommand.OperationType = HAL_OSPI_OPTYPE_COMMON_CFG;
  sCommand.FlashId = HAL_OSPI_FLASH_ID_1;
  sCommand.InstructionMode = HAL_OSPI_INSTRUCTION_8_LINES;
  sCommand.InstructionSize = HAL_OSPI_INSTRUCTION_16_BITS;
  sCommand.AddressMode = HAL_OSPI_ADDRESS_8_LINES;
  sCommand.AddressSize = HAL_OSPI_ADDRESS_24_BITS;
  sCommand.Address = 0x00012345;
  sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode = HAL_OSPI_DATA_8_LINES;
  sCommand.SIOOMode = HAL_OSPI_SIOO_INST_EVERY_CMD;
  sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  sCommand.AddressDtrMode = HAL_OSPI_ADDRESS_DTR_DISABLE;
  sCommand.DataDtrMode = HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DQSMode = HAL_OSPI_DQS_DISABLE;

  aRxLen[0] = 0;
  aRxLen[1] = 0;
  aRxPtr = 0;

  while (1)
  {
/*
	  HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_3);
      //CDC_Transmit_HS((uint8_t *)buffer, strlen(buffer));

	  sCommand.Instruction = LINEAR_BURST_WRITE*0x100 + BUFFERSIZE-1;
	  sCommand.DummyCycles = DUMMY_CLOCK_CYCLES_SRAM_WRITE;

	  //memcpy(aRamBuffer, aTxBuffer, BUFFERSIZE);
	  for(int i = 0; i < BUFFERSIZE; i++){
		  //aRamBuffer[i] = i;
		  aTxBuffer[i] = rand();
	  }
	  SCB_CleanDCache_by_Addr((uint32_t*)aTxBuffer, BUFFERSIZE);

	  if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
      {
    	  Error_Handler();
      }

      //if (HAL_OSPI_Transmit(&hospi1, aRamBuffer, HAL_OSPI_TIMEOUT_DEFAULT_VALUE)!= HAL_OK)
      if (HAL_OSPI_Transmit_DMA(&hospi1, aTxBuffer)!= HAL_OK)
      {
    	  Error_Handler();
      }

      while(HAL_OSPI_GetState(&hospi1) == HAL_OSPI_STATE_BUSY_TX){}

	  sCommand.Instruction = LINEAR_BURST_READ*0x100 + BUFFERSIZE-1;
	  sCommand.DummyCycles = DUMMY_CLOCK_CYCLES_SRAM_READ;

      SCB_InvalidateDCache_by_Addr((uint32_t*)aRxBuffer, BUFFERSIZE);

      if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
      {
    	  Error_Handler();
      }

      if (HAL_OSPI_Receive_DMA(&hospi1, aRxBuffer)!= HAL_OK)
      {
    	  Error_Handler();
      }

      while(HAL_OSPI_GetState(&hospi1) == HAL_OSPI_STATE_BUSY_RX){}

      for(int i = 0; i < BUFFERSIZE/16; i++){
    	  for(int j = 0; j < 16; j++){
    		  if(aTxBuffer[i*16+j] != aRxBuffer[i*16+j]){
        	      sprintf(str,"Error %i, %02x, %02x", i*16+j, aTxBuffer[i*16+j], aRxBuffer[i*16+j]);
        	      CDC_Transmit_HS((uint8_t *)str, strlen(str));
        	      HAL_Delay(1);
    		  }
    	      sprintf(str,"%02x ",aTxBuffer[i*16+j]);
    	      CDC_Transmit_HS((uint8_t *)str, strlen(str));
    	      HAL_Delay(1);
    	  }
	      sprintf(str,"\n\r");
	      CDC_Transmit_HS((uint8_t *)str, strlen(str));
	      HAL_Delay(1);
      }
      sprintf(str,"\n\r");
      CDC_Transmit_HS((uint8_t *)str, strlen(str));

      //HAL_Delay(1000);
*/
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  //HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_3);
      //HAL_Delay(1000);

      HAL_Delay(1);
	  if(aRxLen[(aRxPtr+1)%2] > 0){
		  /**/
		  sCommand.Instruction = LINEAR_BURST_WRITE*0x100 + aRxLen[(aRxPtr+1)%2]-1;
		  sCommand.DummyCycles = DUMMY_CLOCK_CYCLES_SRAM_WRITE;
		  sCommand.NbData = aRxLen[(aRxPtr+1)%2];

		  SCB_CleanDCache_by_Addr((uint32_t*)(uint32_t*)aRxBuffer[(aRxPtr+1)%2], aRxLen[(aRxPtr+1)%2]);

		  if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	      {
	    	  Error_Handler();
	      }

	      if (HAL_OSPI_Transmit_DMA(&hospi1, aRxBuffer[(aRxPtr+1)%2])!= HAL_OK)
	      {
	    	  Error_Handler();
	      }

	      while(HAL_OSPI_GetState(&hospi1) == HAL_OSPI_STATE_BUSY_TX){}


		  sCommand.Instruction = LINEAR_BURST_READ*0x100 + aRxLen[(aRxPtr+1)%2]-1;
		  sCommand.DummyCycles = DUMMY_CLOCK_CYCLES_SRAM_READ;
		  sCommand.NbData = aRxLen[(aRxPtr+1)%2];

	      SCB_InvalidateDCache_by_Addr((uint32_t*)aTxBuffer, aRxLen[(aRxPtr+1)%2]);

	      if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	      {
	    	  Error_Handler();
	      }

	      if (HAL_OSPI_Receive_DMA(&hospi1, aTxBuffer)!= HAL_OK)
	      {
	    	  Error_Handler();
	      }

	      while(HAL_OSPI_GetState(&hospi1) == HAL_OSPI_STATE_BUSY_RX){}

		  CDC_Transmit_HS((uint8_t *)aTxBuffer, aRxLen[(aRxPtr+1)%2]);
		  /**/
		  //CDC_Transmit_HS((uint8_t *)aRxBuffer[(aRxPtr+1)%2], aRxLen[(aRxPtr+1)%2]);

		  aRxLen[(aRxPtr+1)%2] = 0;

	  }
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

  /*AXI clock gating */
  RCC->CKGAENR = 0xFFFFFFFF;

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Macro to configure the PLL clock source
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 112;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief OCTOSPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_OCTOSPI1_Init(void)
{

  /* USER CODE BEGIN OCTOSPI1_Init 0 */

  /* USER CODE END OCTOSPI1_Init 0 */

  OSPIM_CfgTypeDef sOspiManagerCfg = {0};

  /* USER CODE BEGIN OCTOSPI1_Init 1 */
  //sOspiManagerCfg.WriteZeroLatency = HAL_OSPI_NO_LATENCY_ON_WRITE;

  /* USER CODE END OCTOSPI1_Init 1 */
  /* OCTOSPI1 parameter configuration*/
  hospi1.Instance = OCTOSPI1;
  hospi1.Init.FifoThreshold = 1;
  hospi1.Init.DualQuad = HAL_OSPI_DUALQUAD_DISABLE;
  hospi1.Init.MemoryType = HAL_OSPI_MEMTYPE_APMEMORY;
  hospi1.Init.DeviceSize = 24;
  hospi1.Init.ChipSelectHighTime = 1;
  hospi1.Init.FreeRunningClock = HAL_OSPI_FREERUNCLK_DISABLE;
  hospi1.Init.ClockMode = HAL_OSPI_CLOCK_MODE_0;
  hospi1.Init.WrapSize = HAL_OSPI_WRAP_NOT_SUPPORTED;
  hospi1.Init.ClockPrescaler = 2;
  hospi1.Init.SampleShifting = HAL_OSPI_SAMPLE_SHIFTING_HALFCYCLE;
  hospi1.Init.DelayHoldQuarterCycle = HAL_OSPI_DHQC_DISABLE;
  hospi1.Init.ChipSelectBoundary = 0;
  hospi1.Init.DelayBlockBypass = HAL_OSPI_DELAY_BLOCK_BYPASSED;
  hospi1.Init.MaxTran = 0;
  hospi1.Init.Refresh = 0;
  if (HAL_OSPI_Init(&hospi1) != HAL_OK)
  {
    Error_Handler();
  }
  sOspiManagerCfg.ClkPort = 1;
  sOspiManagerCfg.DQSPort = 1;
  sOspiManagerCfg.NCSPort = 1;
  sOspiManagerCfg.IOLowPort = HAL_OSPIM_IOPORT_1_LOW;
  sOspiManagerCfg.IOHighPort = HAL_OSPIM_IOPORT_1_HIGH;
  if (HAL_OSPIM_Config(&hospi1, &sOspiManagerCfg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN OCTOSPI1_Init 2 */

  /* USER CODE END OCTOSPI1_Init 2 */

}

/**
  * Enable MDMA controller clock
  */
static void MX_MDMA_Init(void)
{

  /* MDMA controller clock enable */
  __HAL_RCC_MDMA_CLK_ENABLE();
  /* Local variables */

  /* MDMA interrupt initialization */
  /* MDMA_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(MDMA_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(MDMA_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin : PE3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

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
  __disable_irq();
  while (1)
  {
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
