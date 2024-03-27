/***************************************************************************************************
 * 
 * FILE: eeprom_24lc32a.h
 * DATE: 18/07/2023
 * AUTHOR:  Jonathan Edwards
 * 
 * DESCRIPTION:
 *              I2C firmware for Michrochips 24lc32a eeprom.
 * 
***************************************************************************************************/

#ifndef _EEPROM_24LC32A_H_
#define _EEPROM_24LC32A_H_

#include "main.h"


// 24LC32A EEPROM device address
#define EEPOROM_ADDR_1										0x50  // 80
#define EEPOROM_ADDR_2										0x51  // 81


/*
 * Check whether 24LC32A EEPROM is connected
 */
bool 
eeprom_24lc32a_is_connected (void);

/*
 * Check whether 24LC32A EEPROM is connected
 */
bool 
eeprom_24lc32a_write (uint8_t addr, uint16_t index, char *data);

/*
 * Check whether 24LC32A EEPROM is connected
 */
bool 
eeprom_24lc32a_read (uint8_t addr, uint16_t index, uint8_t max_len, char *data);


#endif  // _EEPROM_24LC32A_H_

