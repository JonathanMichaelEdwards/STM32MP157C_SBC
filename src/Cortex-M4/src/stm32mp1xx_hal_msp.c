/**
  ******************************************************************************
  * @file    stm32mp1xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   This file provides code for the MSP Initialization 
  *                      and de-Initialization codes.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
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


// extern DMA_HandleTypeDef hdma_adc1;


/**
  * Initializes the Global MSP.
  */
void 
HAL_MspInit(void)
{
  /*HW semaphore Clock enable*/
  __HAL_RCC_HSEM_CLK_ENABLE();

/* System interrupt init*/
  /* MemoryManagement_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(MemoryManagement_IRQn, 1, 0);
  /* BusFault_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(BusFault_IRQn, 1, 0);
  /* UsageFault_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(UsageFault_IRQn, 1, 0);
  /* SVCall_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SVCall_IRQn, 1, 0);
  /* DebugMonitor_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DebugMonitor_IRQn, 1, 0);
  /* PendSV_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(PendSV_IRQn, 1, 0);

  /* Peripheral interrupt init */
  /* RCC_WAKEUP_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(RCC_WAKEUP_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(RCC_WAKEUP_IRQn);
}



static uint32_t HAL_RCC_ADC12_CLK_ENABLED = 0;


/**
* @brief ADC MSP Initialization
* This function configures the hardware resources used in this example
* @param hadc: ADC handle pointer
* @retval None
*/
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    if (hadc->Instance == ADC1)
    {
        /* Peripheral clock enable */
        // HAL_RCC_ADC12_CLK_ENABLED++;
        // if (HAL_RCC_ADC12_CLK_ENABLED == 1)
        __HAL_RCC_ADC12_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOF_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        /** ADC1 GPIO Configuration
            ANA0     ------> ADC1_INP0 (ADC_HALL1)
            ANA1     ------> ADC1_INP1 (ADC_IN1)
            PF12     ------> ADC1_INP6 (ADC_IN2)
            PC0      ------> ADC1_INP10 (ADC_HALL3)
            PA6      ------> ADC1_INP3 (ADC_HALL4)
            PF11     ------> ADC1_INP2 (ADC_HALL5)
        */
        HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PA0, SYSCFG_SWITCH_PA0_OPEN);

        HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PA1, SYSCFG_SWITCH_PA1_OPEN);

        GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        PERIPH_LOCK(GPIOF);
        HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
        PERIPH_UNLOCK(GPIOF);

        GPIO_InitStruct.Pin = GPIO_PIN_0;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        PERIPH_LOCK(GPIOC);
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
        PERIPH_UNLOCK(GPIOC);

        GPIO_InitStruct.Pin = GPIO_PIN_6;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        PERIPH_LOCK(GPIOA);
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        PERIPH_UNLOCK(GPIOA);

        /* ADC1 interrupt Init */
        // HAL_NVIC_SetPriority(ADC1_IRQn, DEFAULT_IRQ_PRIO, 0);
        // HAL_NVIC_EnableIRQ(ADC1_IRQn);
    }
    else if (hadc->Instance == ADC2)
    {    /* Peripheral clock enable */
        // HAL_RCC_ADC12_CLK_ENABLED++;
        // if (HAL_RCC_ADC12_CLK_ENABLED == 1)
        __HAL_RCC_ADC12_CLK_ENABLE();
    

        __HAL_RCC_GPIOF_CLK_ENABLE();
        /** ADC2 GPIO Configuration
            PF13     ------> ADC2_INP2 (ADC_HALL2)
        */
        GPIO_InitStruct.Pin = GPIO_PIN_13;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        PERIPH_LOCK(GPIOF);
        HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
        PERIPH_UNLOCK(GPIOF);
    }
}


/**
* @brief ADC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hadc: ADC handle pointer
* @retval None
*/
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC1)
    {
        /* Peripheral clock disable */
        // HAL_RCC_ADC12_CLK_ENABLED--;
        // if (HAL_RCC_ADC12_CLK_ENABLED == 0)
        __HAL_RCC_ADC12_CLK_DISABLE();

        /** ADC1 GPIO Configuration
            ANA0     ------> ADC1_INP0 (ADC_HALL1)
            ANA1     ------> ADC1_INP1 (ADC_IN1)
            PF12     ------> ADC1_INP6 (ADC_IN2)
            PC0      ------> ADC1_INP10 (ADC_HALL3)
            PA6      ------> ADC1_INP3 (ADC_HALL4)
            PF11     ------> ADC1_INP2 (ADC_HALL5)
        */
        PERIPH_LOCK(GPIOF);
        HAL_GPIO_DeInit(GPIOF, GPIO_PIN_11|GPIO_PIN_12);
        PERIPH_UNLOCK(GPIOF);

        PERIPH_LOCK(GPIOC);
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0);
        PERIPH_UNLOCK(GPIOC);

        PERIPH_LOCK(GPIOA);
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6);
        PERIPH_UNLOCK(GPIOA);

        /* ADC1 interrupt DeInit */
        // HAL_NVIC_DisableIRQ(ADC1_IRQn);
    }
    else if (hadc->Instance == ADC2)
    {
        /* Peripheral clock disable */
        // HAL_RCC_ADC12_CLK_ENABLED--;
        // if (HAL_RCC_ADC12_CLK_ENABLED == 0)
        __HAL_RCC_ADC12_CLK_DISABLE();

        /** ADC2 GPIO Configuration
            PF13     ------> ADC2_INP2 (ADC_HALL2)
        */
        PERIPH_LOCK(GPIOF);
        HAL_GPIO_DeInit(GPIOF, GPIO_PIN_13);
        PERIPH_UNLOCK(GPIOF);
    }
}


/**
* @brief DAC MSP Initialization
* This function configures the hardware resources used in this example
* @param hdac: DAC handle pointer
* @retval None
*/
void 
HAL_DAC_MspInit(DAC_HandleTypeDef *hdac)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    if (hdac->Instance == DAC1)
    {
        __HAL_RCC_DAC12_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /** DAC1 GPIO Configuration
        PA5     ------> DAC1_OUT2
        PA4     ------> DAC1_OUT1
        */
        GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_4;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        PERIPH_LOCK(GPIOA);
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        PERIPH_UNLOCK(GPIOA);
    }
}


/**
* @brief DAC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hdac: DAC handle pointer
* @retval None
*/
void
HAL_DAC_MspDeInit(DAC_HandleTypeDef *hdac)
{
    if (hdac->Instance == DAC1)
    {
        __HAL_RCC_DAC12_CLK_DISABLE();

        /** DAC1 GPIO Configuration
        PA5     ------> DAC1_OUT2
        PA4     ------> DAC1_OUT1
        */
        PERIPH_LOCK(GPIOA);
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5|GPIO_PIN_4);
        PERIPH_UNLOCK(GPIOA);
    }
}


void
HAL_I2C_MspInit (I2C_HandleTypeDef *hi2c)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    if (hi2c->Instance == I2C1)
    {
        /** I2C1 GPIO Configuration
            PF14     ------> I2C1_SDA
            PF15     ------> I2C1_SCL
        */
        __HAL_RCC_GPIOF_CLK_ENABLE();
        GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF5_I2C1;
        PERIPH_LOCK(GPIOF);
        HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
        PERIPH_UNLOCK(GPIOF);

        /* Peripheral I2C clock enable */
        __HAL_RCC_I2C1_CLK_ENABLE();

        /* I2C1 interrupt Init */
        HAL_NVIC_SetPriority(I2C1_EV_IRQn, DEFAULT_IRQ_PRIO, 0);
        HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
        HAL_NVIC_SetPriority(I2C1_ER_IRQn, DEFAULT_IRQ_PRIO, 0);
        HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
    }
    else if (hi2c->Instance == I2C2)
    {
        /** I2C2 GPIO Configuration
            PH5     ------> I2C2_SDA
            PH4     ------> I2C2_SCL
        */
        __HAL_RCC_GPIOH_CLK_ENABLE();
        GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
        PERIPH_LOCK(GPIOH);
        HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
        PERIPH_UNLOCK(GPIOH);

        /* Peripheral I2C clock enable */
        __HAL_RCC_I2C2_CLK_ENABLE();

        /* I2C2 interrupt Init */
        HAL_NVIC_SetPriority(I2C2_EV_IRQn, DEFAULT_IRQ_PRIO, 0);
        HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
        HAL_NVIC_SetPriority(I2C2_ER_IRQn, DEFAULT_IRQ_PRIO, 0);
        HAL_NVIC_EnableIRQ(I2C2_ER_IRQn);
    }
    else if (hi2c->Instance == I2C3)
    {
        /** I2C3 GPIO Configuration
            PC9     ------> I2C3_SDA
            PH7     ------> I2C3_SCL
        */
        __HAL_RCC_GPIOC_CLK_ENABLE();
        GPIO_InitStruct.Pin = GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
        PERIPH_LOCK(GPIOC);
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
        PERIPH_UNLOCK(GPIOC);

        __HAL_RCC_GPIOH_CLK_ENABLE();
        GPIO_InitStruct.Pin = GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
        PERIPH_LOCK(GPIOH);
        HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
        PERIPH_UNLOCK(GPIOH);

        /* Peripheral I2C clock enable */
        __HAL_RCC_I2C3_CLK_ENABLE();

        /* I2C3 interrupt Init */
        HAL_NVIC_SetPriority(I2C3_EV_IRQn, DEFAULT_IRQ_PRIO, 0);
        HAL_NVIC_EnableIRQ(I2C3_EV_IRQn);
        HAL_NVIC_SetPriority(I2C3_ER_IRQn, DEFAULT_IRQ_PRIO, 0);
        HAL_NVIC_EnableIRQ(I2C3_ER_IRQn);
    }
    else if (hi2c->Instance == I2C5)
    {
        /** I2C5 GPIO Configuration
            PD0     ------> I2C5_SDA
            PD1     ------> I2C5_SCL
        */
        __HAL_RCC_GPIOD_CLK_ENABLE();
        GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C5;
        PERIPH_LOCK(GPIOD);
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
        PERIPH_UNLOCK(GPIOD);

        /* Peripheral I2C clock enable */
        __HAL_RCC_I2C5_CLK_ENABLE();

        /* I2C5 interrupt Init */
        HAL_NVIC_SetPriority(I2C5_EV_IRQn, DEFAULT_IRQ_PRIO, 0);
        HAL_NVIC_EnableIRQ(I2C5_EV_IRQn);
        HAL_NVIC_SetPriority(I2C5_ER_IRQn, DEFAULT_IRQ_PRIO, 0);
        HAL_NVIC_EnableIRQ(I2C5_ER_IRQn);
    }
}


void 
HAL_I2C_MspDeInit (I2C_HandleTypeDef* hi2c)
{
    if (hi2c->Instance == I2C1)
    {
        /* Peripheral clock disable */
        __HAL_RCC_I2C1_CLK_DISABLE();

        /** I2C1 GPIO Configuration
            PF14     ------> I2C1_SDA
            PF15     ------> I2C1_SCL
        */
        PERIPH_LOCK(GPIOF);
        HAL_GPIO_DeInit(GPIOF, GPIO_PIN_14|GPIO_PIN_15);
        PERIPH_UNLOCK(GPIOF);

        /* I2C1 interrupt DeInit */
        HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);
        HAL_NVIC_DisableIRQ(I2C1_ER_IRQn);
    }
    else if (hi2c->Instance == I2C2)
    {
        /* Peripheral clock disable */
        __HAL_RCC_I2C2_CLK_DISABLE();

        /** I2C2 GPIO Configuration
            PH5     ------> I2C2_SDA
            PH4     ------> I2C2_SCL
        */
        PERIPH_LOCK(GPIOH);
        HAL_GPIO_DeInit(GPIOH, GPIO_PIN_4|GPIO_PIN_5);
        PERIPH_UNLOCK(GPIOH);

        /* I2C2 interrupt DeInit */
        HAL_NVIC_DisableIRQ(I2C2_EV_IRQn);
        HAL_NVIC_DisableIRQ(I2C2_ER_IRQn);
    }
    else if (hi2c->Instance == I2C3)
    {
        /* Peripheral clock disable */
        __HAL_RCC_I2C3_CLK_DISABLE();

        /** I2C3 GPIO Configuration
            PC9     ------> I2C3_SDA
            PH7     ------> I2C3_SCL
        */
        PERIPH_LOCK(GPIOC);
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_9);
        PERIPH_UNLOCK(GPIOC);

        PERIPH_LOCK(GPIOH);
        HAL_GPIO_DeInit(GPIOH, GPIO_PIN_7);
        PERIPH_UNLOCK(GPIOH);

        /* I2C3 interrupt DeInit */
        HAL_NVIC_DisableIRQ(I2C3_EV_IRQn);
        HAL_NVIC_DisableIRQ(I2C3_ER_IRQn);
    }
    else if (hi2c->Instance == I2C5)
    {
        /* Peripheral clock disable */
        __HAL_RCC_I2C5_CLK_DISABLE();

        /** I2C5 GPIO Configuration    
            PD0     ------> I2C5_SDA
            PD1     ------> I2C5_SCL
        */
        PERIPH_LOCK(GPIOD);
        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_0|GPIO_PIN_1);
        PERIPH_UNLOCK(GPIOD);

        /* I2C5 interrupt DeInit */
        HAL_NVIC_DisableIRQ(I2C5_EV_IRQn);
        HAL_NVIC_DisableIRQ(I2C5_ER_IRQn);
    }
}



/**
* @brief UART MSP Initialization
* This function configures the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void 
HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    if (huart->Instance == UART4)
    {
        /* Peripheral clock enable */
        __HAL_RCC_UART4_CLK_ENABLE();
        // __HAL_RCC_UART4_FORCE_RESET();
        // __HAL_RCC_UART4_RELEASE_RESET();

        __HAL_RCC_GPIOG_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();

        /** UART4 GPIO Configuration
            PG11     ------> UART4_TX
            PB2      ------> UART4_RX
        */
        // TX
        GPIO_InitStruct.Pin = GPIO_PIN_11;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF6_UART4;
        PERIPH_LOCK(GPIOG);
        HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
        PERIPH_UNLOCK(GPIOG);

        // RX
        GPIO_InitStruct.Pin = GPIO_PIN_2;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
        PERIPH_LOCK(GPIOB);
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        PERIPH_UNLOCK(GPIOB);
    }
    else if (huart->Instance == USART2)
    {
        /* Peripheral clock enable */
        __HAL_RCC_USART2_CLK_ENABLE();

        __HAL_RCC_GPIOD_CLK_ENABLE();
        __HAL_RCC_GPIOF_CLK_ENABLE();
        /** USART2 GPIO Configuration
            PD5     ------> USART2_TX
            PF4     ------> USART2_RX
         */
        GPIO_InitStruct.Pin = GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
        PERIPH_LOCK(GPIOD);
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
        PERIPH_UNLOCK(GPIOD);

        GPIO_InitStruct.Pin = GPIO_PIN_4;
        GPIO_InitStruct.Mode = GPIO_MODE_AF;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
        PERIPH_LOCK(GPIOF);
        HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
        PERIPH_UNLOCK(GPIOF);
    }
}

/**
* @brief USART MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param husart: USART handle pointer
* @retval None
*/
void 
HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
    if (huart->Instance == UART4)
    {
        /* Peripheral clock disable */
        // __HAL_RCC_UART4_FORCE_RESET();
        // __HAL_RCC_UART4_RELEASE_RESET();
        __HAL_RCC_UART4_CLK_DISABLE();

        /** UART4 GPIO Configuration
            PG11     ------> UART4_TX
            PB2      ------> UART4_RX
        */
        // TX
        PERIPH_LOCK(GPIOG);
        HAL_GPIO_DeInit(GPIOG, GPIO_PIN_11);
        PERIPH_UNLOCK(GPIOG);

        // RX
        PERIPH_LOCK(GPIOB);
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_2);
        PERIPH_UNLOCK(GPIOB);
    }
    else if (huart->Instance == USART2)
    {
        /* Peripheral clock disable */
        __HAL_RCC_USART2_CLK_DISABLE();

        /**USART2 GPIO Configuration
            PD5     ------> USART2_TX
            PF4     ------> USART2_RX
        
        */
        PERIPH_LOCK(GPIOD);
        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_5);
        PERIPH_UNLOCK(GPIOD);

        PERIPH_LOCK(GPIOF);
        HAL_GPIO_DeInit(GPIOF, GPIO_PIN_4);
        PERIPH_UNLOCK(GPIOF);
    }   
}


/**
* @brief SPI MSP Initialization
* This function configures the hardware resources used in this example
* @param hspi: SPI handle pointer
* @retval None
*/
void
HAL_SPI_MspInit (SPI_HandleTypeDef* hspi)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    if (hspi->Instance == SPI2)
    {
        /* Peripheral clock enable */
        __HAL_RCC_SPI2_CLK_ENABLE();

        __HAL_RCC_GPIOI_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /** SPI2 GPIO Configuration
            PI2     ------> SPI2_MISO
            PC3     ------> SPI2_MOSI
            PB10    ------> SPI2_SCK
        */
        GPIO_InitStruct.Pin = GPIO_PIN_2;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
        PERIPH_LOCK(GPIOI);
        HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
        PERIPH_UNLOCK(GPIOI);

        GPIO_InitStruct.Pin = GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
        PERIPH_LOCK(GPIOC);
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
        PERIPH_UNLOCK(GPIOC);

        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
        PERIPH_LOCK(GPIOB);
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        PERIPH_UNLOCK(GPIOB);

        HAL_NVIC_SetPriority(SPI2_IRQn, DEFAULT_IRQ_PRIO, 0);
        HAL_NVIC_EnableIRQ(SPI2_IRQn);
    }
}


/**
* @brief SPI MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hspi: SPI handle pointer
* @retval None
*/
void
HAL_SPI_MspDeInit (SPI_HandleTypeDef* hspi)
{
    if (hspi->Instance == SPI2)
    {
        /* Peripheral clock disable */
        __HAL_RCC_SPI2_CLK_DISABLE();

        /** SPI2 GPIO Configuration
            PI2     ------> SPI2_MISO
            PC3     ------> SPI2_MOSI
            PB10    ------> SPI2_SCK
        */
        PERIPH_LOCK(GPIOI);
        HAL_GPIO_DeInit(GPIOI, GPIO_PIN_2);
        PERIPH_UNLOCK(GPIOI);

        PERIPH_LOCK(GPIOC);
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_3);
        PERIPH_UNLOCK(GPIOC);

        PERIPH_LOCK(GPIOB);
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10);
        PERIPH_UNLOCK(GPIOB);

        HAL_NVIC_DisableIRQ(SPI4_IRQn);
    }
}

