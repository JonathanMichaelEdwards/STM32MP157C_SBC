/***************************************************************************************************
 * 
 * FILE: main.c
 * DATE: 12/05/2023
 * AUTHOR:  Jonathan Edwards
 * 
 * DESCRIPTION:
 *              STM32MP157C Cortex M4 Demo Application
 * 
***************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c_linker.h"
#include "ms8607.h"
#include "drv8889.h"
#include "eeprom_24lc32a.h"
#include "pwr_ina220.h"
#include "hall_als31313.h"


#define MAX_BUFFER_SIZE RPMSG_BUFFER_SIZE

#define RCC_WAKEUP_IRQ_PRIO 0U
#define ADC1_BUFFER_SIZE 5
#define ADC2_BUFFER_SIZE 2
#define TPS92602_DIAG_SCALE		12.5f
#define AMPS_MILLI_AMPS 		1000  //  [A->mA]

static ADC_HandleTypeDef hadc2;
static DAC_HandleTypeDef hdac1;
static UART_HandleTypeDef huart4;
static UART_HandleTypeDef huart2;

__IO bool adc1_conver_flag = false;
uint32_t adc_hall_raw[ADC1_BUFFER_SIZE] = { 0 };
uint32_t adc_light_raw[ADC2_BUFFER_SIZE] = { 0 };





/* USER CODE BEGIN PFP */
#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
	 set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#ifdef __GNUC__
#define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */



/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
void
MX_SPI2_Init (void)
{
	hspi2.Instance = SPI2;
	hspi2.Init.Mode = SPI_MODE_MASTER;
	hspi2.Init.Direction = SPI_DIRECTION_2LINES;
	hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi2.Init.NSS = SPI_NSS_SOFT;
	hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
	hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi2.Init.CRCPolynomial = 7;
	hspi2.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
	hspi2.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
	hspi2.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
	hspi2.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
	hspi2.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
	hspi2.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
	hspi2.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
	hspi2.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
	hspi2.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
	hspi2.Init.IOSwap = SPI_IO_SWAP_DISABLE;

	if (HAL_SPI_Init(&hspi2) != HAL_OK)
	{
		Error_Handler();
	}

	// Force Reset SPI2
	HAL_SPI_DeInit(&hspi2);
	HAL_SPI_MspDeInit(&hspi2);
	HAL_NVIC_DisableIRQ(SPI2_IRQn);
	__HAL_RCC_SPI2_FORCE_RESET();
	__HAL_RCC_SPI2_RELEASE_RESET();
}


/**
 * @brief UART4 Initialization Function
 * @param None
 * @retval None
 */
void 
MX_UART4_Init (void)
{
	huart4.Instance = UART4;
	huart4.Init.BaudRate = 115200;
	huart4.Init.WordLength = UART_WORDLENGTH_8B;
	huart4.Init.StopBits = UART_STOPBITS_1;
	huart4.Init.Parity = UART_PARITY_NONE;
	huart4.Init.Mode = UART_MODE_TX_RX;
	huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart4.Init.OverSampling = UART_OVERSAMPLING_8;
	huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart4.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

	if (HAL_UART_Init(&huart4) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_UARTEx_SetTxFifoThreshold(&huart4, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_UARTEx_SetRxFifoThreshold(&huart4, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_UARTEx_DisableFifoMode(&huart4) != HAL_OK)
	{
		Error_Handler();
	}
}


/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
void
MX_USART2_Init (void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
	{
		Error_Handler();
	}
}


/** 
 * Configure clks and pins
 * - note: 
 * 		Enable peripheral port clocks so M4 still runs when A7 is sleeping
*/
void 
MX_GPIO_Init (void)
{
	// PB8 - Power Up Motor Switch
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef  gpio_init_structure;
    gpio_init_structure.Pin = GPIO_PIN_8;
    gpio_init_structure.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_structure.Pull = GPIO_PULLUP;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    BSP_ENTER_CRITICAL_SECTION(GPIOB);
    HAL_GPIO_Init(GPIOB, &gpio_init_structure);
    BSP_EXIT_CRITICAL_SECTION(GPIOB);

	// PC10 - CS_M1
	// PC11 - STEP_M1
	// PC12 - EM_M1
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef  gpio_init_structure1;
	gpio_init_structure1.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
	gpio_init_structure1.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init_structure1.Pull = GPIO_PULLUP;
	gpio_init_structure1.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	BSP_ENTER_CRITICAL_SECTION(GPIOC);
	HAL_GPIO_Init(GPIOC, &gpio_init_structure1);
	BSP_EXIT_CRITICAL_SECTION(GPIOC);

	// PD11 Blue - Application Status
	// PD2  - DIR_M1
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef  gpio_init_structure4;
	gpio_init_structure4.Pin = GPIO_PIN_2|GPIO_PIN_11;
	gpio_init_structure4.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init_structure4.Pull = GPIO_PULLUP;
	gpio_init_structure4.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	BSP_ENTER_CRITICAL_SECTION(GPIOD);
	HAL_GPIO_Init(GPIOD, &gpio_init_structure4);
	BSP_EXIT_CRITICAL_SECTION(GPIOD);

	// PF6 Yellow - Fail LED Status
	__HAL_RCC_GPIOF_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init_structure_2;
	gpio_init_structure_2.Pin = GPIO_PIN_6;
	gpio_init_structure_2.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init_structure_2.Pull = GPIO_PULLUP;
	gpio_init_structure_2.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	BSP_ENTER_CRITICAL_SECTION(GPIOF);
	HAL_GPIO_Init(GPIOF, &gpio_init_structure_2);
	BSP_EXIT_CRITICAL_SECTION(GPIOF);

	// PI5 (White), PI6 (UV) GPIO Lighting Enable pins
	// PI3 - nSleep_M1 
	__HAL_RCC_GPIOI_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init_structure_3;
	gpio_init_structure_3.Pin = GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_6;
	gpio_init_structure_3.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init_structure_3.Pull = GPIO_PULLUP;
	gpio_init_structure_3.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	BSP_ENTER_CRITICAL_SECTION(GPIOI);
	HAL_GPIO_Init(GPIOI, &gpio_init_structure_3);
	BSP_EXIT_CRITICAL_SECTION(GPIOI);
}


/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
void
MX_ADC1_Init (void)
{
	ADC_ChannelConfTypeDef sConfig = {0};

	/** Common config
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
	hadc1.Init.Resolution = ADC_RESOLUTION_16B;
	hadc1.Init.ScanConvMode = ENABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc1.Init.LowPowerAutoWait = DISABLE;
	hadc1.Init.ContinuousConvMode = ENABLE;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
	hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	hadc1.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
	hadc1.Init.OversamplingMode = DISABLE;

	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		Error_Handler();
	}
}


/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
void 
MX_ADC2_Init (void)
{
	ADC_ChannelConfTypeDef sConfig = { 0 };

	/** Common config
	 */
	hadc2.Instance = ADC2;
	hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
	hadc2.Init.Resolution = ADC_RESOLUTION_16B;
	hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc2.Init.LowPowerAutoWait = DISABLE;
	hadc2.Init.ContinuousConvMode = ENABLE;
	hadc2.Init.NbrOfConversion = 1;
	hadc2.Init.DiscontinuousConvMode = DISABLE;
	hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc2.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
	hadc2.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	hadc2.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
	hadc2.Init.OversamplingMode = DISABLE;

	if (HAL_ADC_Init(&hadc2) != HAL_OK)
	{
		Error_Handler();
	}
}


/**
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
void 
MX_DAC_Init(void)
{
	DAC_ChannelConfTypeDef sConfig = { 0 };

	/** DAC Initialization
	 */
	hdac1.Instance = DAC1;
	if (HAL_DAC_Init(&hdac1) != HAL_OK)
	{
		Error_Handler();
	}

	/** DAC channel OUT1 & OUT2 config
	 */
	sConfig.DAC_HighFrequency = DAC_HIGH_FREQUENCY_INTERFACE_MODE_DISABLE;
	sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
	sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
	sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;

	if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_2) != HAL_OK)
	{
		Error_Handler();
	}

	// Set and Enable DAC Channel 1
	if (HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0) != HAL_OK)
	{
		/* Start Error */
		Error_Handler();
	}

	if (HAL_DAC_Start(&hdac1, DAC_CHANNEL_1) != HAL_OK)
	{
		/* Start Error */
		Error_Handler();
	}

	// Set and Enable DAC Channel 2
	if (HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 0) != HAL_OK)
	{
		/* Start Error */
		Error_Handler();
	}

	if (HAL_DAC_Start(&hdac1, DAC_CHANNEL_2) != HAL_OK)
	{
		/* Start Error */
		Error_Handler();
	}	
}


//// -- Uncomment this if the external Vref pin is not connected
// void 
// MX_VREF_Init (void)
// {
// 	/* USER CODE BEGIN 2 */
// 	// Enable the VREF clock
// 	__HAL_RCC_VREF_CLK_ENABLE();
// 	// Disable the high impedance mode which is the default one read page 1694 of refman
// 	HAL_SYSCFG_VREFBUF_HighImpedanceConfig(SYSCFG_VREFBUF_HIGH_IMPEDANCE_DISABLE);
// 	HAL_SYSCFG_VREFBUF_VoltageScalingConfig(SYSCFG_VREFBUF_VOLTAGE_SCALE2);
// 	// To enable VREFBUF
// 	HAL_SYSCFG_EnableVREFBUF();
// }


/**
 * @brief  Retargets the C library printf function to the USART.
 * @param  None
 * @retval None
 */

PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART3 and Loop until the end of transmission */
	HAL_UART_Transmit(&huart4, (uint8_t *)&ch, 1, 0xFFFF);

	return ch;
}

GETCHAR_PROTOTYPE
{
	uint8_t ch = 0;
	/* Clear the Overrun flag just before receiving the first character */
	__HAL_UART_CLEAR_OREFLAG(&huart4);

	HAL_UART_Receive(&huart4, (uint8_t *)&ch, 1, 0xFFFF);
	HAL_UART_Transmit(&huart4, (uint8_t *)&ch, 1, 0xFFFF);
	
	return ch;
}


/**
 * @brief  This function is executed in case of error occurrence.
 * @param  file: The file name as string.
 * @param  line: The line in file as a number.
 * @retval None
 */
void 
Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	while (1)
	{
		/* Toggle LED7 for error */
		HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_6);
		HAL_Delay(100);
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void 
assert_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
		 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif /* USE_FULL_ASSERT */


// select adc2 channel
void 
ADC2_Select_CH (uint32_t channel)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	/** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = channel;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;

	if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
}


// select adc1 channel
void 
ADC1_Select_CH (uint32_t channel)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	/** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = channel;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;

	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
}


void
LIGHT_ADC_Current (void)
{
	// Lighting ADC reads
	// ADC_IN1 - channel 1
	ADC1_Select_CH(ADC_CHANNEL_1);
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	adc_light_raw[0] = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);

	// ADC_IN2 - channel 6
	ADC1_Select_CH(ADC_CHANNEL_6);
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	adc_light_raw[1] = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);

	// Convert to volts
	float light_uv		= (3.3f * (float)adc_light_raw[0]) / 65536.f; 
	float light_white 	= (3.3f * (float)adc_light_raw[1]) / 65536.f; 

	printf("\r-- UV Light Voltage: %.3f [V], UV LEDs load Current: %.3f [mA]\n", light_uv, (light_uv/TPS92602_DIAG_SCALE)*AMPS_MILLI_AMPS);
	printf("\r-- White Light Voltage: %.3f [V], White LEDs load Current: %.3f [mA]\n\n", light_white, (light_white/TPS92602_DIAG_SCALE)*AMPS_MILLI_AMPS);
}


void 
Monitor_PWR (void)
{	
	if (pwr_ina220_is_connected() != HAL_OK)
		puts("\rI2C2 PWR Monitor 0x40 & 0x41 are Not Connected");
	else
		puts("\rI2C2 PWR Monitor 0x40 & 0x41 are Connected");

	// +5 V source
	uint16_t v1_data = 0;
	uint32_t i1_data = 0;
	uint32_t p1_data = 0;

	pwr_ina220_read_volts(PWR_MON_ADDR_1, &v1_data);
	pwr_ina220_read_current(PWR_MON_ADDR_1, &i1_data);
	pwr_ina220_read_watts(PWR_MON_ADDR_1, &p1_data);
	printf("\rMeter 1 -- Voltage: %.3f [V], Current: %.3f [mA], Power: %.3f [mW]\n", v1_data/1000.0f, i1_data/1555.0f, ((float)i1_data/1555.0f) * ((float)v1_data/1000.0f));

	// if (pwr_ina220_reset(PWR_MON_ADDR_1) == HAL_OK)
	// 	puts("\rReseted fine\n");
	// else
	// 	puts("\rReset error\n");
	pwr_ina220_reset(PWR_MON_ADDR_1);
	pwr_ina220_init(PWR_MON_ADDR_1);


	// +12 V source
	uint16_t v2_data = 0;
	uint32_t i2_data = 0;
	uint32_t p2_data = 0;
	
	pwr_ina220_read_volts(PWR_MON_ADDR_2, &v2_data);
	pwr_ina220_read_current(PWR_MON_ADDR_2, &i2_data);
	pwr_ina220_read_watts(PWR_MON_ADDR_2, &p2_data);
	printf("\rMeter 2 -- Voltage: %.3f [V], Current: %.3f [mA], Power: %.3f [mW]\n", v2_data/955.0f, i2_data/1555.0f,(i2_data/1555.0f) * ((float)v2_data/955.0f));

	// if (pwr_ina220_reset(PWR_MON_ADDR_2) == HAL_OK)
	// 	puts("\rReseted fine\n");
	// else
	// 	puts("\rReset error\n");
	pwr_ina220_reset(PWR_MON_ADDR_2); 
	pwr_ina220_init(PWR_MON_ADDR_2);
}


void
Poll_PHT_Sensors(uint8_t hi2c)
{
	volatile float temperature = 0.f;
	volatile float pressure = 0.f;
	volatile float humidity = 0.f;


	if (ms8607_is_connected(hi2c) != HAL_OK)
	{
		ms8607_init();

		switch (hi2c)
		{
			case I2C_1:
				puts("\rI2C1 PHT Sensor 0x76 & 0x40 is Not Connected");
				i2c1_master_init();
				break;
			case I2C_3:
				puts("\rI2C3 PHT Sensor 0x76 & 0x40 is Not Connected");
				i2c3_master_init();
				break;
			case I2C_5:
				puts("\rI2C5 PHT Sensor 0x76 & 0x40 is Not Connected");
				i2c5_master_init();
				break;
		}
	}
	else
	{
		switch (hi2c)
		{
			case I2C_1:
				puts("\rI2C1 PHT Sensor 0x76 & 0x40 is Connected");
				break;
			case I2C_3:
				puts("\rI2C3 PHT Sensor 0x76 & 0x40 is Connected");
				break;
			case I2C_5:
				puts("\rI2C5 PHT Sensor 0x76 & 0x40 is Connected");
				break;
		}

		ms8607_read_temperature_pressure_humidity(hi2c, &temperature, &pressure, &humidity);
		printf("\rTemperature = %.2f \u2103\n", temperature);
		printf("\rPressure = %.2f hPa\n", pressure);
		printf("\rHumidity = %.2f %%RH\n\n", humidity);
	}
}


void
Poll_Micro_Stepper_Motors (uint8_t cs_spi)
{
	uint8_t ctrl1 = 0x00;
    uint8_t ctrl2 = 0x0F;
    uint8_t ctrl3 = 0x06;
    uint8_t ctrl4 = 0x30;
    uint8_t ctrl5 = 0x08;
    uint8_t ctrl6 = 0x03;
    uint8_t ctrl7 = 0x20;

	Set_CS_Pin(cs_spi);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);

	uint8_t data[1];
	data[0] = (ctrl1 & 0b11111) << 1;

    if (HAL_SPI_Transmit(&hspi2, data, 1, 1000) != HAL_OK)
		Error_Handler();
    // if (HAL_SPI_Transmit(&hspi2, data, 1, 100) != HAL_OK)
	// 	Error_Handler();
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);

	// Reset_Settings(ctrl1, ctrl2, ctrl3, ctrl4, ctrl5, ctrl6, ctrl7);
	// clearFaults(ctrl4);

	Monitor_PWR();

	// if (ms8607_is_connected(cs_spi) != HAL_OK)
	// {
	// 	ms8607_init();

	// 	switch (cs_spi)
	// 	{
	// 		case CS_1:
	// 			puts("\rI2C1 PHT Sensor 0x76 & 0x40 is Not Connected");
	// 			i2c1_master_init();
	// 			break;
	// 		case CS_2:
	// 			puts("\rI2C3 PHT Sensor 0x76 & 0x40 is Not Connected");
	// 			i2c3_master_init();
	// 			break;
	// 	}
	// }
	// else
	// {

	// }
}


void 
MCU_PWR_STOP (void)
{
	// MCU Stop example - Turns OFF the MCU while the A7 is OFF
	// 	-- Use LPTimer if we want to wakeup
	/* Insert 5 second delay */
	/* Turn on the LED */
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, 1);
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, 1);
	HAL_Delay(2000);

	/* CRITICAL SECTION STARTS HERE!
		* IRQs will be masked (Only RCC IRQ allowed).
		* Eg. SysTick IRQ won't be able to increment uwTick HAL variable, use a
		* polling method if delays or timeouts are required.
		* /

	/* (C)STOP protection mechanism
		* Only the IT with the highest priority (0 value) can interrupt.
		* RCC_WAKEUP_IRQn IT is intended to have the highest priority and to be the
		* only one IT having this value
		* RCC_WAKEUP_IRQn is generated only when RCC is completely resumed from
		* CSTOP (protection mechanism)
		*/

	__set_BASEPRI((RCC_WAKEUP_IRQ_PRIO + 1) << (8 - __NVIC_PRIO_BITS));

	/* Back up clock context */
	// RCC_backupClocks();

	/* Disable IO Compensation */
	HAL_SYSCFG_DisableIOCompensation();

	/* Turn off LED */
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, 0);

	/* Clear the Low Power MCU flags before going into CSTOP */
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_STOP);

	/* Enter STOP mode */
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

	/* ... STOP mode ... */

	/* At  STOP mode exit, enable and select PLL3 as system clock source
	(PLL3 & PLL4 are disabled in STOP mode) */

	/* Test if system was on STOP mode */
	if (__HAL_PWR_GET_FLAG(PWR_FLAG_STOP) == 1U)
	{
		/* Clear the Low Power MCU flags */
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_STOP);
	}

	/* Enable IO Compensation */
	if (HAL_SYSCFG_EnableIOCompensation() != HAL_OK)
	{
		Error_Handler();
	}

	/* All level of ITs can interrupt */
	__set_BASEPRI(RCC_WAKEUP_IRQ_PRIO);
}


/** 
 * ADC2 GPIO Configuration
            PF13     ------> ADC2_INP2 (ADC_HALL2)

 * ADC1 GPIO Configuration
			ANA0     ------> ADC1_INP0 (ADC_HALL1)
			ANA1     ------> ADC1_INP1 (ADC_IN1)
			PF12     ------> ADC1_INP6 (ADC_IN2)
			PC0      ------> ADC1_INP10 (ADC_HALL3)
			PA6      ------> ADC1_INP3 (ADC_HALL4)
			PF11     ------> ADC1_INP2 (ADC_HALL5)
*/
void
ADC_Read_Example (void)
{
	// Lighting ADC reads
	// ADC_IN1 - channel 1
	ADC1_Select_CH(ADC_CHANNEL_1);
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	adc_light_raw[0] = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);

	// ADC_IN2 - channel 6
	ADC1_Select_CH(ADC_CHANNEL_6);
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	adc_light_raw[1] = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);

	// Convert to volts
	float light_uv		= (3.3f * (float)adc_light_raw[0]) / 65536.f; 
	float light_white 	= (3.3f * (float)adc_light_raw[1]) / 65536.f; 

	printf("\r-- ADC_IN1 raw data: %ld, UV Light Voltage: %.3f [V]\n", adc_light_raw[0], light_uv);
	printf("\r-- ADC_IN2 raw data: %ld, White Light Voltage: %.3f [V]\n", adc_light_raw[1], light_white);


	// Hall ADC reads
	// ADC_HALL1 - channel 0
	ADC1_Select_CH(ADC_CHANNEL_0);
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	adc_hall_raw[0] = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);

	// ADC_HALL2 - channel 2
	ADC2_Select_CH(ADC_CHANNEL_2);
	HAL_ADC_Start(&hadc2);
	HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY);
	adc_hall_raw[1] = HAL_ADC_GetValue(&hadc2);
	HAL_ADC_Stop(&hadc2);

	// ADC_HALL3 - channel 10
	ADC1_Select_CH(ADC_CHANNEL_10);
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	adc_hall_raw[2] = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);

	// ADC_HALL4 - channel 3
	ADC1_Select_CH(ADC_CHANNEL_3);
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	adc_hall_raw[3] = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);

	// ADC_HALL5 - channel 2
	ADC1_Select_CH(ADC_CHANNEL_2);
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	adc_hall_raw[4] = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);

	// Convert to volts
	float v1_hall = (3.3f * (float)adc_hall_raw[0]) / 65536.f; 
	float v2_hall = (3.3f * (float)adc_hall_raw[1]) / 65536.f; 
	float v3_hall = (3.3f * (float)adc_hall_raw[2]) / 65536.f; 
	float v4_hall = (3.3f * (float)adc_hall_raw[3]) / 65536.f; 
	float v5_hall = (3.3f * (float)adc_hall_raw[4]) / 65536.f; 

	printf("\r-- ADC_HALL1 raw data: %ld, Hall Voltage: %.3f [V]\n", adc_hall_raw[0], v1_hall);
	printf("\r-- ADC_HALL2 raw data: %ld, Hall Voltage: %.3f [V]\n", adc_hall_raw[1], v2_hall);
	printf("\r-- ADC_HALL3 raw data: %ld, Hall Voltage: %.3f [V]\n", adc_hall_raw[2], v3_hall);
	printf("\r-- ADC_HALL4 raw data: %ld, Hall Voltage: %.3f [V]\n", adc_hall_raw[3], v4_hall);
	printf("\r-- ADC_HALL5 raw data: %ld, Hall Voltage: %.3f [V]\n", adc_hall_raw[4], v5_hall);

	HAL_Delay(1000);
}


// Concaternate floating number to string then write and read from EEPROM
void
EEPROM_READ_WRITE_EXAMPLE (uint8_t addr)
{
	if (eeprom_24lc32a_is_connected() == HAL_OK)
		puts("\rI2C3 EEPROM 0x50 & 0x51 are Connected");
	else
		puts("\rI2C3 EEPROM 0x50 & 0x51 are Not Connected");

	const char *message = "Hello World!";
	char msg_read[100];

	eeprom_24lc32a_write(addr, 0, message);
	eeprom_24lc32a_read(addr, 0, strlen(message), msg_read);

	printf("\rEEPROM Wrote: %s\n", message);
	puts("\rEEPROM Read Data: ");
	for (uint8_t i = 0; i < strlen(message); i++)
	{
		printf("%c", msg_read[i]);
	}
	puts("\r");


	// float -> str
	float f_num = 100.31f;
	char str_buff[100];
	sprintf(str_buff, " %.2f", f_num);

	eeprom_24lc32a_write(addr, strlen(message), str_buff);
	eeprom_24lc32a_read(addr, strlen(message), strlen(str_buff), msg_read);

	printf("\rEEPROM Wrote: %s\n", str_buff);
	puts("\rEEPROM Read Data: ");
	for (uint8_t i = 0; i < strlen(message)+strlen(str_buff); i++)
	{
		printf("%c", msg_read[i]);
	}
	puts("\r");
	
	HAL_Delay(1000);
}


// --- Dimming example
// GPIO:
// 		I5 - White channel
// 		I6 - UV channel
// DAC (ICTRL):
// 		Resistor divider divides by 2
// 		12-bit res dac - ICTRL max is 1.5V (150mA)
// 		each channel rated for 150mA
// ADC (DIAG):
// 		16-bit res adc - DIAG scale factor 12.5 (pg.14 datasheet)
// 			- Divide by the DIAG voltage to get the current on that channel
// 				- showen in 'LIGHT_ADC_Current()'
void
Lighting_UV_WHITE (void)
{
	// Set i_max each channel to 2.5 V -> 1.25 ~ 83 % max (~ 125mA)
	uint32_t i_max = (uint32_t)((2.5f * 4096.f) / 3.3f);  // Voltage is half due to voltage divider

	// Enable Lighting IC
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_5, 1);  // White
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_6, 1);  // UV


	for (int i = 0; i < 500; i++)
	{
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, i);
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, i);
		HAL_Delay(5);
	}

	Monitor_PWR();
	LIGHT_ADC_Current();

	for (int i = 500; i < 1100; i++)
	{
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, i);
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, i);
		HAL_Delay(5);
	}

	Monitor_PWR();
	LIGHT_ADC_Current();

	for (int i = 1100; i < 1600; i++)
	{
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, i);
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, i);
		HAL_Delay(5);
	}

	Monitor_PWR();
	LIGHT_ADC_Current();

	for (int i = 1600; i < i_max; i++)
	{
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, i);
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, i);
		HAL_Delay(5);
	}

	Monitor_PWR();
	LIGHT_ADC_Current();

	for (int i = i_max; i > 1300; i--)
	{
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, i);
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, i);
		HAL_Delay(5);
	}

	Monitor_PWR();
	LIGHT_ADC_Current();

	for (int i = 1300; i > 700; i--)
	{
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, i);
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, i);
		HAL_Delay(5);
	}

	Monitor_PWR();
	LIGHT_ADC_Current();

	for (int i = 700; i > 0; i--)
	{
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, i);
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, i);
		HAL_Delay(5);
	}

	Monitor_PWR();
	LIGHT_ADC_Current();

	// Disable Lighting IC
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_5, 0);
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_6, 0);
}


void
Lighting_UV (void)
{
	// 12-bit resolution dac
	// uint32_t v1_dac = (uint32_t)((1.5f * 4096.f) / 3.3f);
	// uint32_t v2_dac = (uint32_t)((1.5f * 4096.f) / 3.3f);

	// Enable Lighting IC
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_5, 0);
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_6, 1);


	for (int i = 0; i < 500; i++)
	{
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, i);
		HAL_Delay(5);
	}

	Monitor_PWR();

	for (int i = 500; i < 1100; i++)
	{
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, i);
		HAL_Delay(5);
	}

	Monitor_PWR();

	for (int i = 1100; i < 2000; i++)
	{
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, i);
		HAL_Delay(5);
	}

	Monitor_PWR();

	for (int i = 2000; i < 3000; i++)
	{
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, i);
		HAL_Delay(5);
	}

	Monitor_PWR();

	for (int i = 3000; i > 1500; i--)
	{
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, i);
		HAL_Delay(5);
	}

	Monitor_PWR();

	for (int i = 1500; i > 700; i--)
	{
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, i);
		HAL_Delay(5);
	}

	Monitor_PWR();

	for (int i = 700; i > 0; i--)
	{
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, i);
		HAL_Delay(5);
	}

	Monitor_PWR();

	// Disable Lighting IC
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_6, 0);
}



void
Lighting_WHITE (void)
{
	// 12-bit resolution dac
	// uint32_t v1_dac = (uint32_t)((1.5f * 4096.f) / 3.3f);
	// uint32_t v2_dac = (uint32_t)((1.5f * 4096.f) / 3.3f);

	// Enable Lighting IC
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_5, 1);


	for (int i = 0; i < 500; i++)
	{
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, i);
		HAL_Delay(5);
	}

	Monitor_PWR();

	for (int i = 500; i < 1100; i++)
	{
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, i);
		HAL_Delay(5);
	}

	Monitor_PWR();

	for (int i = 1100; i < 2000; i++)
	{
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, i);
		HAL_Delay(5);
	}

	Monitor_PWR();

	for (int i = 2000; i < 3000; i++)
	{
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, i);
		HAL_Delay(5);
	}

	Monitor_PWR();

	for (int i = 3000; i > 1500; i--)
	{
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, i);
		HAL_Delay(5);
	}

	Monitor_PWR();

	for (int i = 1500; i > 700; i--)
	{
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, i);
		HAL_Delay(5);
	}

	Monitor_PWR();

	for (int i = 700; i > 0; i--)
	{
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, i);
		HAL_Delay(5);
	}

	Monitor_PWR();

	// Disable Lighting IC
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_5, 0);
}


void 
Hall_PROG_Sensor (uint8_t addr)
{
	if (hall_als31313_is_connected(addr) != STATUS_OK)
	{
		printf("\rI2C1 0x%x Hall Sensor Not Connected\n", addr);
	}
	else
	{
		printf("\rI2C1 0x%x Hall Sensor Connected\n", addr);
		hall_als31313_write(addr, SINGLE_LOOP_MODE, HALL_ACTIVE);
		if (hall_als31313_init(addr) != STATUS_OK)
		{
			puts("\r     - Hall Sensor customer access error\n");
		}
		else
		{
			puts("\r  Customer Access Mode Enabled");
			uint32_t hall_loop_data = 0;

			if (hall_als31313_read(addr, SINGLE_LOOP_MODE, &hall_loop_data) != STATUS_OK)
			{
				puts("\r     - Unable to read loop mode error\n");
			}
			else
			{
				// No loop mode masking and write loop mode 
				hall_loop_data = (hall_loop_data & 0xFFFFFFF3) | (0x0 << 2);
				if (hall_als31313_write(addr, SINGLE_LOOP_MODE, hall_loop_data) != STATUS_OK)
				{
					puts("\r     - Unable to read loop mode error\n");
				}
				else
				{
					uint32_t hall_data_a = 0;  // data read from 0x28
					uint32_t hall_data_b = 0;  // data read from 0x29
					int16_t x, y, z = 0;

					for (uint8_t count = 0; count < 1; count++)
					{
						// puts("\r  Reading 4 bytes from 0x28");
						if (hall_als31313_read(addr, HALL_REG_A_READ, &hall_data_a) != STATUS_OK)
						{
							puts("\r     - Unable to read loop mode error\n");
							break;
						}
						else
						{
							if (hall_als31313_read(addr, HALL_REG_B_READ, &hall_data_b) != STATUS_OK)
							{
								puts("\r     - Unable to read loop mode error\n");
								break;
							}
							else
							{
								// Take the most significant byte of each axis from register 0x28 and combine it with the least
								// significant 4 bits of each axis from register 0x29, then sign extend the 12th bit.
								// Raw data!
								x = sign_extend_bitfield(((hall_data_a >> 20) & 0x0FF0) | ((hall_data_b >> 16) & 0x0F), 12);
								y = sign_extend_bitfield(((hall_data_a >> 12) & 0x0FF0) | ((hall_data_b >> 12) & 0x0F), 12);
								z = sign_extend_bitfield(((hall_data_a >> 4) & 0x0FF0) | ((hall_data_b >> 8) & 0x0F), 12);

								// If no magnetic field present - no angle present
								// Otherwise print angle aswell
								if ((x > -20)  && (x < 20) && (y > -20)  && (y < 20) && (z > -20)  && (z < 20))
								{
									printf("\r\nx: %d  	  y: %d  	z: %d  	  angleXY: NA  	angleXZ: NA  	angleYZ: NA\n", x, y, z);
								} 
								else
								{
									// Convert the X, Y and Z values into radians
									float rx = (float)x / 4096.0 * M_TWOPI;
									float ry = (float)y / 4096.0 * M_TWOPI;
									float rz = (float)z / 4096.0 * M_TWOPI;

									// Use a four quadrant Arc Tan to convert 2
									// axis to an angle (which is in radians) then
									// convert the angle from radians to degrees
									// for display.
									float angleXY = atan2f(ry, rx) * 180.0 / M_PI;
									float angleXZ = atan2f(rz, rx) * 180.0 / M_PI;
									float angleYZ = atan2f(rz, ry) * 180.0 / M_PI;

									printf("\r\nx: %d  	  y: %d  	z: %d  	  angleXY: %.3f  	angleXZ: %.3f  	angleYZ: %.3f\n", x, y, z, angleXY, angleXZ, angleYZ);
								}

								// Sleep
								hall_als31313_write(addr, SINGLE_LOOP_MODE, HALL_SLEEP);
							}	
						}	
					}
				}				
			}
		}
	}

	HAL_Delay(1000);
}


/**
 * @brief  The application entry point.
 *
 * @retval None
 */
int 
main (void)
{
	/* Reset of all peripherals, Initialize the Systick. */
	HAL_Init();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_UART4_Init();
	MX_DAC_Init();
	MX_ADC1_Init();
	MX_ADC2_Init();
	// MX_SPI2_Init();

	// Initialize I2C lines
	ms8607_init();
	pwr_ina220_reset(PWR_MON_ADDR_1);
	pwr_ina220_reset(PWR_MON_ADDR_2);
	pwr_ina220_init(PWR_MON_ADDR_1);
	pwr_ina220_init(PWR_MON_ADDR_2);
	i2c1_master_init();
	i2c2_master_init();
	i2c3_master_init();
	i2c5_master_init();

	hall_als31313_write(HALL_ADDR_1, SINGLE_LOOP_MODE, HALL_SLEEP);

	while (1)
	{
		puts("\r=======================================");
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_11);

		puts("\r---------------------------");

		/////////////////////////////////////// ADC ///////////////////////////////////////	
		puts("\rFive ADC Hall Effect and Two Lighting DIAG Demo:\n");		
		ADC_Read_Example();
		///////////////////////////////////////

		puts("\r---------------------------");

		/////////////////////////////////////// EEPROM ///////////////////////////////////////
		puts("\rTwo EEPROM Write and Read Demo:\n");	
		puts("\r--- 0x50");	
		EEPROM_READ_WRITE_EXAMPLE(EEPOROM_ADDR_1);
		puts("\r--- 0x51");
		EEPROM_READ_WRITE_EXAMPLE(EEPOROM_ADDR_2);
		///////////////////////////////////////

		puts("\r---------------------------");

		/////////////////////////////////////// Lighting ///////////////////////////////////////		
		puts("\rDual Lighting Driver GPIO, DAC (ICTRL), and ADC (DIAG) Demo:\n");	
		Lighting_UV_WHITE();
		///////////////////////////////////////

		puts("\r---------------------------");

		/////////////////////////////////////// Five I2C Hall Sensor ///////////////////////////////////////
		puts("\rFive I2C Hall Effect Sensor Demo:\n");
		puts("\r--- 0x60");	
		Hall_PROG_Sensor(HALL_ADDR_1);
		puts("\r--- 0x61");	
		Hall_PROG_Sensor(HALL_ADDR_2);
		puts("\r--- 0x62");	
		Hall_PROG_Sensor(HALL_ADDR_3);
		puts("\r--- 0x63");	
		Hall_PROG_Sensor(HALL_ADDR_4);
		puts("\r--- 0x64");	
		Hall_PROG_Sensor(HALL_ADDR_5);
		///////////////////////////////////////

		puts("\r---------------------------");

		/////////////////////////////////////// PWR Monitor ///////////////////////////////////////		
		puts("\r+5V and +12V I2C PWR Monitor Demo:\n");
		Monitor_PWR();
		///////////////////////////////////////

		puts("\r---------------------------");

		/////////////////////////////////////// PHT ///////////////////////////////////////
		puts("\rTripple I2C PHT Demo:\n");
		Poll_PHT_Sensors(I2C_1);
  
		HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_6);
		HAL_Delay(1000);

		Poll_PHT_Sensors(I2C_3);

		HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_6);
		HAL_Delay(1000);

		Poll_PHT_Sensors(I2C_5);

		HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_6);
		HAL_Delay(1000);
		///////////////////////////////////////

		puts("\r---------------------------");

		/////////////////////////////////////// Micro Stepper Motor ///////////////////////////////////////
		// puts("\rFive SPI Micro Stepper Motor Demo:\n");
		// puts("\r--- CS_1");	
		// Monitor_PWR();
		// Poll_Micro_Stepper_Motors(CS_1);
		// puts("\r--- CS_2");	
		// Poll_Micro_Stepper_Motors(CS_2);
		// Poll_Micro_Stepper_Motors(CS_3);
		// Poll_Micro_Stepper_Motors(CS_4);
		// Poll_Micro_Stepper_Motors(CS_5);
		///////////////////////////////////////

		// puts("\r---------------------------");

		/////////////////////////////////////// PWR ///////////////////////////////////////
		puts("\rM4 ISR STOP Demo:\n");
		MCU_PWR_STOP();
		///////////////////////////////////////

		puts("\r---------------------------");

		puts("\r=======================================\n");
	}
}

