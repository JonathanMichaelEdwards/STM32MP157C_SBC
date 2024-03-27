/************************************************************************************************** *
 * 
 * FILE: drv8434s.h
 * DATE: 31/07/2023
 * AUTHOR:  Jonathan Edwards
 * 
 * DESCRIPTION:
 *              SPI firmware for DRV8434S Micro Stepper Motor driver.
 *                  - Write to a single stepper motor at a time
 * 
***************************************************************************************************/

#ifndef _DRV8434S_H_
#define _DRV8434S_H_

#include "main.h"


static SPI_HandleTypeDef hspi2;

/*
 * SPI CS (chip select) line
 *	- Micro stepper motor to write to
 */
typedef enum spi_cs_line 
{
    CS_1 = 0,
    CS_2,
    CS_3,
    CS_4,
    CS_5,
} SPI_CS_Line;


// Addresses of control and status registers.
enum DRV8434SRegAddr
{
    FAULT = 0x00,
    DIAG1 = 0x01,
    DIAG2 = 0x02,
    CTRL1 = 0x03,
    CTRL2 = 0x04,
    CTRL3 = 0x05,
    CTRL4 = 0x06,
    CTRL5 = 0x07,
    CTRL6 = 0x08,
    CTRL7 = 0x09,
    CTRL8 = 0x0A,
    CTRL9 = 0x0B,
};

/*
 * (chip select) Select cs pin
 */
void
Set_CS_Pin (uint8_t pin);

/*
 * Changes all of the driver's settings back to their default values.
 */
void
Reset_Settings (uint8_t ctrl1, uint8_t ctrl2, uint8_t ctrl3, uint8_t ctrl4, uint8_t ctrl5, uint8_t ctrl6, uint8_t ctrl7);

// Clears any fault conditions that are currently latched in the driver
/// (CLR_FLT = 1).
///
/// WARNING: Calling this function clears latched faults, which might allow
/// the motor driver outputs to reactivate.  If you do this repeatedly without
/// fixing an abnormal condition (like a short circuit), you might damage the
/// driver.
///
/// The driver automatically clears the CLR_FLT bit after it is written.
void
clearFaults (uint8_t data);


#endif  // _DRV8434S_H_

