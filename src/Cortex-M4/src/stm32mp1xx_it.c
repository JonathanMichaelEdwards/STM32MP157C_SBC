/**
  ******************************************************************************
  * @file    stm32mp1xx_it.c
  * @author  MCD Application Team
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32mp1xx_it.h"
#include "i2c_linker.h"
#include "drv8889.h"



/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */ 
/******************************************************************************/

/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
  /* USER CODE BEGIN HardFault_IRQn 1 */

  /* USER CODE END HardFault_IRQn 1 */
}

/**
* @brief This function handles Memory management fault.
*/
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
  /* USER CODE BEGIN MemoryManagement_IRQn 1 */

  /* USER CODE END MemoryManagement_IRQn 1 */
}

/**
* @brief This function handles Pre-fetch fault, memory access fault.
*/
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
  /* USER CODE BEGIN BusFault_IRQn 1 */

  /* USER CODE END BusFault_IRQn 1 */
}

/**
* @brief This function handles Undefined instruction or illegal state.
*/
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
  /* USER CODE BEGIN UsageFault_IRQn 1 */

  /* USER CODE END UsageFault_IRQn 1 */
}

/**
* @brief This function handles System service call via SWI instruction.
*/
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
* @brief This function handles Debug monitor.
*/
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
* @brief This function handles Pendable request for system service.
*/
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}


/******************************************************************************/
/* STM32MP1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32mp1xx.s).                    */
/******************************************************************************/



// /**
//   * @brief This function handles ADC1 global interrupt.
//   */
// void ADC1_IRQHandler(void)
// {
//   /* USER CODE BEGIN ADC1_IRQn 0 */

//   /* USER CODE END ADC1_IRQn 0 */
//   HAL_ADC_IRQHandler(&hadc1);
//   /* USER CODE BEGIN ADC1_IRQn 1 */

//   /* USER CODE END ADC1_IRQn 1 */
// }


/**
* @brief This function handles SPI2 event interrupt.
*/
void SPI2_IRQHandler(void)
{
  HAL_SPI_IRQHandler(&hspi2);
}


/**
* @brief This function handles I2C1 event interrupt.
*/
void I2C1_EV_IRQHandler(void)
{
  HAL_I2C_EV_IRQHandler(&hi2c1);
}


/**
* @brief This function handles I2C1 error interrupt.
*/
void I2C1_ER_IRQHandler(void)
{
  HAL_I2C_ER_IRQHandler(&hi2c1);
}


/**
* @brief This function handles I2C2 event interrupt.
*/
void I2C2_EV_IRQHandler(void)
{
  HAL_I2C_EV_IRQHandler(&hi2c2);
}


/**
* @brief This function handles I2C2 error interrupt.
*/
void I2C2_ER_IRQHandler(void)
{
  HAL_I2C_ER_IRQHandler(&hi2c2);
}


/**
* @brief This function handles I2C3 event interrupt.
*/
void I2C3_EV_IRQHandler(void)
{
  HAL_I2C_EV_IRQHandler(&hi2c3);
}


/**
* @brief This function handles I2C3 error interrupt.
*/
void I2C3_ER_IRQHandler(void)
{
  HAL_I2C_ER_IRQHandler(&hi2c3);
}


/**
* @brief This function handles I2C5 event interrupt.
*/
void I2C5_EV_IRQHandler(void)
{
  HAL_I2C_EV_IRQHandler(&hi2c5);
}


/**
* @brief This function handles I2C5 error interrupt.
*/
void I2C5_ER_IRQHandler(void)
{
  HAL_I2C_ER_IRQHandler(&hi2c5);
}


