/***************************************************************************************************
 * 
 * FILE: hall_als31313.h
 * DATE: 26/07/2023
 * AUTHOR:  Jonathan Edwards
 * 
 * DESCRIPTION:
 *              I2C firmware for ALS31313 Hall Effect Sensor.
 * 
***************************************************************************************************/

#ifndef _HALL_ALS31313_H_
#define _HALL_ALS31313_H_

#include "main.h"


// ALS31313 Hall device address
#define HALL_ADDR_1										0x60  // 96
#define HALL_ADDR_2										0x61  // 97
#define HALL_ADDR_3										0x62  // 98
#define HALL_ADDR_4										0x63  // 99
#define HALL_ADDR_5										0x64  // 100

#define SINGLE_LOOP_MODE	    0x27  // 39
#define HALL_REG_A_READ 	    0x28  // 40
#define HALL_REG_B_READ 	    0x29  // 41
#define CUSTOMER_ACCESS		    0x35

// Power Modes
#define HALL_ACTIVE     0
#define HALL_SLEEP      1



/*
 * Check whether ALS31313 Hall is connected
 */
bool 
hall_als31313_is_connected (uint8_t addr);

/*
 * Writes 4 Bytes of data to the ALS31313 Hall - Enter customer access mode
 */
bool 
hall_als31313_init (uint8_t addr);

/*
 *  Read ALS31313 Hall loop mode
 */
bool
hall_als31313_read (uint8_t addr, uint8_t cmd, uint32_t *data_read);

/*
 * Write ALS31313 Hall loop mode
 */
bool
hall_als31313_write (uint8_t addr, uint8_t cmd, uint8_t data_mode);

int64_t 
sign_extend_bitfield (uint32_t data, uint8_t width);


#endif  // _HALL_ALS31313_H_

