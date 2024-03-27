/***************************************************************************************************
 * 
 * FILE: pwr_ina220.h
 * DATE: 19/07/2023
 * AUTHOR:  Jonathan Edwards
 * 
 * DESCRIPTION:
 *              I2C firmware for INA220 PWR Monitor.
 * 
***************************************************************************************************/

#ifndef _EEPROM_INA220_H_
#define _EEPROM_INA220_H_

#include "main.h"


// INA220 PWR device address
#define PWR_MON_ADDR_1										0x40  // 64
#define PWR_MON_ADDR_2										0x41  // 65

#define INA_CONFIGURATION_REGISTER         0x00 // Configuration Register address
#define INA_BUS_VOLTAGE_REGISTER           0x02 // Bus Voltage Register address
#define INA_POWER_REGISTER                 0x03 // Power Register address
#define INA_CALIBRATION_REGISTER           0x05 // Calibration Register address
#define INA_RESET_DEVICE                 0x8000 // Write to configuration to reset device
#define INA_CONVERSION_READY_MASK        0x0080 // Bit 4
#define INA_CONFIG_MODE_MASK             0x0007 // Bits 0-3
#define INA_DEFAULT_OPERATING_MODE         0x07 // Default continuous mode
#define INA220_SHUNT_VOLTAGE_REGISTER      0x01 // INA220 Shunt Voltage Register
#define INA220_CURRENT_REGISTER            0x04 // INA220 Current Register
#define INA220_CONFIG_DEFAULT            0x399F // Default register value after reset - 14751

#define INA220_BUS_VOLTAGE_LSB       400  // 4.00mV typical
#define INA220_SHUNT_VOLTAGE_LSB     100; // 10.0uV typical

#define MAX_CUR                     3 // 3 Amps
#define SHUNT_R                 100e3 // 100 mOhm

#define LSB_CURRENT         91552   //(uint64_t)MAX_CUR * 1000000000 / 32768          // Get the best possible LSB in nA - 91552
#define LSB_PWR             1831040 //(uint32_t)20 * LSB_CURRENT         // Fixed multiplier per device - 1831040
#define INA220_CALIBRATE    6826    //(uint64_t)409600000 / ((uint64_t)LSB_CURRENT * (uint64_t)SHUNT_R / (uint64_t)100000) // Compute - 6826



/*
 * Check whether 24LC32A EEPROM is connected
 */
bool 
pwr_ina220_init (uint8_t addr);

/*
 * Check whether 24LC32A EEPROM is connected
 */
bool 
pwr_ina220_reset (uint8_t addr);

/*
 * Check whether 24LC32A EEPROM is connected
 */
bool 
pwr_ina220_is_connected (void);

/*
 * Check whether 24LC32A EEPROM is connected
 */
bool
pwr_ina220_read_volts (uint8_t addr, uint16_t *data_read);

/*
 * Check whether 24LC32A EEPROM is connected
 */
bool 
pwr_ina220_read_current (uint8_t addr, uint32_t *data_read);

/*
 * Check whether 24LC32A EEPROM is connected
 */
bool
pwr_ina220_read_watts (uint8_t addr, uint32_t *data_read);


#endif  // _EEPROM_INA220_H_

