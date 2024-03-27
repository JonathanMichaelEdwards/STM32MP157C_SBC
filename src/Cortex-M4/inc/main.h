/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    UART/UART_Receive_Transmit_Console/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "stm32mp1xx_hal.h"
#include "lock_resource.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32mp15xx_disco.h"


/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);


/* Private defines -----------------------------------------------------------*/
#define DEFAULT_IRQ_PRIO      1U
#define   I2C_ADDRESS     (0x40 << 1)  // LSB used for read or write

static ADC_HandleTypeDef hadc1;


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
