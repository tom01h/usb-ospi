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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tusb.h"

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

PCD_HandleTypeDef hpcd_USB_OTG_HS;

/* USER CODE BEGIN PV */
buffer_info buffer_info_axm;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_MDMA_Init(void);
static void MX_OCTOSPI1_Init(void);
static void MX_USB_OTG_HS_PCD_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void from_host_task(OSPI_RegularCmdTypeDef *sCommand)
{
	if ((buffer_info_axm.busy == false)) {
		buffer_info_axm.busy = true;
		uint com_size = 0;
		if (tud_vendor_available()) {
			com_size = tud_vendor_read(buffer_info_axm.buffer, 512);
			tud_vendor_read_flush();
		}else{
			tud_task();
		}
		if (com_size != 0) {
			sCommand->Instruction = *(uint32_t *)&buffer_info_axm.buffer[0];
			buffer_info_axm.count = (sCommand->Instruction & 0x00ffffff)+1;

			sCommand->Address     = *(uint32_t *)&buffer_info_axm.buffer[4];

			uint count = buffer_info_axm.count;

			if((buffer_info_axm.buffer[3]&0xf0)==(LINEAR_BURST_WRITE<<4)) {     // LINEAR_BURST_WRITE
				sCommand->DummyCycles = DUMMY_CLOCK_CYCLES_SRAM_WRITE;
				do{
					while(tud_vendor_read(buffer_info_axm.buffer, 512) == 0){tud_task();}

					if(count > 256){
						sCommand->NbData  = 256;
						count -= 256;
						sCommand->Instruction &= 0xff0000ff;/////////////|0xff
					} else {
						sCommand->NbData  = count;
						count = 0;
						sCommand->Instruction &= 0xff0000ff;
					}
					SCB_CleanDCache_by_Addr((uint32_t*)buffer_info_axm.buffer, 512);

					if (HAL_OSPI_Command(&hospi1, sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {Error_Handler();}
					if (HAL_OSPI_Transmit_DMA(&hospi1, buffer_info_axm.buffer)!= HAL_OK)               {Error_Handler();}
					while(HAL_OSPI_GetState(&hospi1) == HAL_OSPI_STATE_BUSY_TX){}
					sCommand->Address += 256;

					if(count > 0){
						if(count > 256){
							sCommand->NbData  = 256;
							count -= 256;
							sCommand->Instruction &= 0xff0000ff;/////////////|0xff
						} else {
							sCommand->NbData  = count;
							count = 0;
							sCommand->Instruction &= 0xff0000ff;
						}
						SCB_CleanDCache_by_Addr((uint32_t*)buffer_info_axm.buffer, 512);

						if (HAL_OSPI_Command(&hospi1, sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {Error_Handler();}
						if (HAL_OSPI_Transmit_DMA(&hospi1, &buffer_info_axm.buffer[256])!= HAL_OK)         {Error_Handler();}
						while(HAL_OSPI_GetState(&hospi1) == HAL_OSPI_STATE_BUSY_TX){}
						sCommand->Address += 256;
					}
				} while(count > 0);
			} else {                                                             // LINEAR_BURST_READ
				sCommand->DummyCycles = DUMMY_CLOCK_CYCLES_SRAM_READ;
				do{
					SCB_InvalidateDCache_by_Addr((uint32_t*)buffer_info_axm.buffer, 512);

					if(count > 256){
						sCommand->NbData  = 256;
						count -= 256;
						sCommand->Instruction &= 0xff0000ff;/////////////|0xff
					} else {
						sCommand->NbData  = count;
						count = 0;
						sCommand->Instruction &= 0xff0000ff;
					}

					if (HAL_OSPI_Command(&hospi1, sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {Error_Handler();}
					if (HAL_OSPI_Receive_DMA(&hospi1, buffer_info_axm.buffer)!= HAL_OK)                {Error_Handler();}
					while(HAL_OSPI_GetState(&hospi1) == HAL_OSPI_STATE_BUSY_RX){}
					sCommand->Address += 256;

					if(count > 0){
						if(count > 256){
							sCommand->NbData  = 256;
							count -= 256;
							sCommand->Instruction &= 0xff0000ff;/////////////|0xff
						} else {
							sCommand->NbData  = count;
							count = 0;
							sCommand->Instruction &= 0xff0000ff;
						}

						if (HAL_OSPI_Command(&hospi1, sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {Error_Handler();}
						if (HAL_OSPI_Receive_DMA(&hospi1, &buffer_info_axm.buffer[256])!= HAL_OK)          {Error_Handler();}
						while(HAL_OSPI_GetState(&hospi1) == HAL_OSPI_STATE_BUSY_RX){}
						sCommand->Address += 256;
					}

					while(tud_vendor_write(buffer_info_axm.buffer, 512) == 0){tud_task();}///////////////////////////////512
					tud_vendor_flush();
				} while(count > 0);
			}
		}
		buffer_info_axm.busy = false;
	}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_USB_OTG_HS_PCD_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  tusb_init();

  OSPI_RegularCmdTypeDef sCommand;
  sCommand.OperationType = HAL_OSPI_OPTYPE_COMMON_CFG;
  sCommand.FlashId = HAL_OSPI_FLASH_ID_1;
  sCommand.InstructionMode = HAL_OSPI_INSTRUCTION_8_LINES;
  sCommand.InstructionSize = HAL_OSPI_INSTRUCTION_32_BITS;
  sCommand.AddressMode = HAL_OSPI_ADDRESS_8_LINES;
  sCommand.AddressSize = HAL_OSPI_ADDRESS_32_BITS;
  sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode = HAL_OSPI_DATA_8_LINES;
  sCommand.SIOOMode = HAL_OSPI_SIOO_INST_EVERY_CMD;
  sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  sCommand.AddressDtrMode = HAL_OSPI_ADDRESS_DTR_DISABLE;
  sCommand.DataDtrMode = HAL_OSPI_DATA_DTR_DISABLE;
  sCommand.DQSMode = HAL_OSPI_DQS_DISABLE;

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
      //HAL_Delay(1);
	  tud_task();
	  from_host_task(&sCommand);
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

  /* USER CODE END OCTOSPI1_Init 1 */
  /* OCTOSPI1 parameter configuration*/
  hospi1.Instance = OCTOSPI1;
  hospi1.Init.FifoThreshold = 1;
  hospi1.Init.DualQuad = HAL_OSPI_DUALQUAD_DISABLE;
  hospi1.Init.MemoryType = HAL_OSPI_MEMTYPE_APMEMORY;
  hospi1.Init.DeviceSize = 32;
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
  * @brief USB_OTG_HS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_HS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_HS_Init 0 */

  /* USER CODE END USB_OTG_HS_Init 0 */

  /* USER CODE BEGIN USB_OTG_HS_Init 1 */

  /* USER CODE END USB_OTG_HS_Init 1 */
  hpcd_USB_OTG_HS.Instance = USB_OTG_HS;
  hpcd_USB_OTG_HS.Init.dev_endpoints = 9;
  hpcd_USB_OTG_HS.Init.speed = PCD_SPEED_HIGH;
  hpcd_USB_OTG_HS.Init.dma_enable = DISABLE;
  hpcd_USB_OTG_HS.Init.phy_itface = USB_OTG_ULPI_PHY;
  hpcd_USB_OTG_HS.Init.Sof_enable = DISABLE;
  hpcd_USB_OTG_HS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_HS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_HS.Init.vbus_sensing_enable = DISABLE;
  hpcd_USB_OTG_HS.Init.use_dedicated_ep1 = DISABLE;
  hpcd_USB_OTG_HS.Init.use_external_vbus = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_HS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_HS_Init 2 */

  /* USER CODE END USB_OTG_HS_Init 2 */

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

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

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
