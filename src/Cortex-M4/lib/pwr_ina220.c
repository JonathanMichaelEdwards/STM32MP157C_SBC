/***************************************************************************************************
 * 
 * FILE: pwr_ina220.c
 * DATE: 19/07/2023
 * AUTHOR:  Jonathan Edwards
 * 
 * DESCRIPTION:
 *              I2C firmware for INA220 PWR Monitor.
 * 
***************************************************************************************************/

#include "pwr_ina220.h"
#include "i2c_linker.h"



/**
 * \brief Writes a Byte of data to the 24LC32A EEPROM
 *
 * BYTE WRITE:
 * 				| Control Byte | - | CMD reg | - | Data MSB | - | Data LSB |
 * 
 * \return bool : status of eeprom
 *       - 0 : Device is present
 *       - 1 : Device is not acknowledging I2C address
  */
bool 
pwr_ina220_init (uint8_t addr)
{
	enum status_code i2c_status;

	uint8_t data[3];
	const uint16_t cali = INA220_CALIBRATE;
	data[0] = INA_CALIBRATION_REGISTER;
	data[1] = (uint8_t)(cali >> 8);
	data[2] = (uint8_t)(cali);
	
	struct i2c_master_packet transfer = {
		.address     = addr,
		.data_length = 3,
		.data        = data,
	};

	// PWR Monitor calibrate transfer
	i2c_status = i2c_master_write_packet_wait(I2C_2, &transfer);
	if (i2c_status != STATUS_OK)
		return STATUS_ERR_OVERFLOW;


	const uint16_t config = INA220_CONFIG_DEFAULT;
	transfer.data[0] = INA_CONFIGURATION_REGISTER;
	transfer.data[1] = (uint8_t)(config >> 8);
	transfer.data[2] = (uint8_t)(config);

	// PWR Monitor configure transfer
	i2c_status = i2c_master_write_packet_wait(I2C_2, &transfer);
	if (i2c_status != STATUS_OK)
		return STATUS_ERR_OVERFLOW;
	

	return STATUS_OK;
}


/**
 * \brief Writes a Byte of data to the 24LC32A EEPROM
 *
 * BYTE WRITE:
 * 				| Control Byte | - | CMD reg | - | Data MSB | - | Data LSB |
 * 
 * \return bool : status of eeprom
 *       - 0 : Device is present
 *       - 1 : Device is not acknowledging I2C address
  */
bool 
pwr_ina220_reset (uint8_t addr)
{
	enum status_code i2c_status;

	uint8_t data[3];
	const uint16_t reset = INA_RESET_DEVICE;
	data[0] = INA_CONFIGURATION_REGISTER;
	data[1] = (uint8_t)(reset >> 8);
	data[2] = (uint8_t)(reset);
	
	struct i2c_master_packet transfer = {
		.address     = addr,
		.data_length = 3,
		.data        = data,
	};

	// PWR Reset transfer
	i2c_status = i2c_master_write_packet_wait(I2C_2, &transfer);
	if (i2c_status != STATUS_OK)
		return STATUS_ERR_OVERFLOW;
	

	// Reset Reg request read
	transfer.data_length = 1;
	transfer.data[0] = INA_CONFIGURATION_REGISTER;

	i2c_status = i2c_master_write_packet_wait(I2C_2, &transfer);
	if (i2c_status != STATUS_OK)
		return STATUS_ERR_OVERFLOW;
	
	
	// Read reset message log
	uint8_t buffer[2] = { 0 };

	transfer.data_length = 2;
	transfer.data = buffer;

	i2c_status = i2c_master_read_packet_wait(I2C_2, &transfer);
	if (i2c_status != STATUS_OK)
		return STATUS_ERR_OVERFLOW;


	uint16_t rst_check = ((buffer[0] << 8) & 0xFF00) | (buffer[1] & 0xFF);   // Decode uint16_t <- uint16_t | uint8_t
	if (rst_check != INA220_CONFIG_DEFAULT) 
		return STATUS_ERR_OVERFLOW;


	return STATUS_OK;
}


/**
 * \brief Check whether INA220 PWR Monitor ICs are connected
 *
 * BYTE WRITE:
 * 				| Control Byte |
 * 
 * \return bool : status of eeprom
 *       - 0 : Device is present
 *       - 1 : Device is not acknowledging I2C address
  */
bool 
pwr_ina220_is_connected (void)
{
	enum status_code i2c_status;
	
	struct i2c_master_packet transfer = {
		.address     = PWR_MON_ADDR_1,
		.data_length = 0,
		.data        = NULL,
	};

	// PWR Monitor 1 transfer
	i2c_status = i2c_master_write_packet_wait(I2C_2, &transfer);
	if (i2c_status != STATUS_OK)
		return STATUS_ERR_OVERFLOW;

	
	// PWR Monitor 2 transfer
	transfer.address = PWR_MON_ADDR_2;
	i2c_status = i2c_master_write_packet_wait(I2C_2, &transfer);
	if (i2c_status != STATUS_OK)
		return STATUS_ERR_OVERFLOW;
	

	return STATUS_OK;
}



/**
 * \brief Reads INA220 PWR Monitor (write then read)
 *
 * BYTE READ:
 * 				| Control Byte | - | CMD reg | -- | Control Byte | - | Data MSB | - | Data LSB |
 * 
 * \return bool : status INA220 PWR Monitor
 *       - 0 : Device is present
 *       - 1 : Device is not acknowledging I2C address
 */
bool
pwr_ina220_read_volts (uint8_t addr, uint16_t *data_read)
{
	enum status_code i2c_status;

	uint8_t data[1];
	data[0] = INA_BUS_VOLTAGE_REGISTER;
	
	struct i2c_master_packet transfer = {
		.address     = addr,
		.data_length = 1,
		.data        = data,
	};

	// Voltage Reg request read
	i2c_status = i2c_master_write_packet_wait(I2C_2, &transfer);
	if (i2c_status != STATUS_OK)
		return STATUS_ERR_OVERFLOW;
	
	
	// Read voltage bus raw
	uint8_t buffer[2] = { 0 };

	transfer.data_length = 2;
	transfer.data = buffer;

	i2c_status = i2c_master_read_packet_wait(I2C_2, &transfer);
	if (i2c_status != STATUS_OK)
		return STATUS_ERR_OVERFLOW;


	uint16_t v_bus_raw = ((buffer[0] << 8) & 0xFF00) | (buffer[1] & 0xFF);   // Decode uint16_t <- uint16_t | uint8_t

	v_bus_raw = v_bus_raw >> 3;
	*data_read = (v_bus_raw*INA220_BUS_VOLTAGE_LSB) / 100.0f;  // conversion to get milliVolts
	

	return STATUS_OK;
}

/**
 * \brief Reads INA220 PWR Monitor (write then read)
 *
 * BYTE READ:
 * 				| Control Byte | - | CMD reg | -- | Control Byte | - | Data MSB | - | Data LSB |
 * 
 * \return bool : status INA220 PWR Monitor
 *       - 0 : Device is present
 *       - 1 : Device is not acknowledging I2C address
  */
bool
pwr_ina220_read_current (uint8_t addr, uint32_t *data_read)
{
	enum status_code i2c_status;

	uint8_t data[1];
	data[0] = INA220_CURRENT_REGISTER;
	
	struct i2c_master_packet transfer = {
		.address     = addr,
		.data_length = 1,
		.data        = data,
	};

	// Current Reg request read
	i2c_status = i2c_master_write_packet_wait(I2C_2, &transfer);
	if (i2c_status != STATUS_OK)
		return STATUS_ERR_OVERFLOW;
	

	// Read current bus raw
	uint8_t buffer[2] = { 0 };

	transfer.data_length = 2;
	transfer.data = buffer;

	i2c_status = i2c_master_read_packet_wait(I2C_2, &transfer);
	if (i2c_status != STATUS_OK)
		return STATUS_ERR_OVERFLOW;

	uint16_t i_bus_raw = ((buffer[0] << 8) & 0xFF00) | (buffer[1] & 0xFF);   // Decode uint16_t <- uint16_t | uint8_t
	*data_read = (uint32_t)i_bus_raw * LSB_CURRENT / 1000.0f;  // conversion to get microamps
	

	return STATUS_OK;
}



/**
 * \brief Reads INA220 PWR Monitor (write then read)
 *
 * BYTE READ:
 * 				| Control Byte | - | CMD reg | -- | Control Byte | - | Data MSB | - | Data LSB |
 * 
 * \return bool : status INA220 PWR Monitor
 *       - 0 : Device is present
 *       - 1 : Device is not acknowledging I2C address
  */
bool
pwr_ina220_read_watts (uint8_t addr, uint32_t *data_read)
{
	enum status_code i2c_status;

	uint8_t data[1];
	data[0] = INA_POWER_REGISTER;
	
	struct i2c_master_packet transfer = {
		.address     = addr,
		.data_length = 1,
		.data        = data,
	};

	// Current Reg request read
	i2c_status = i2c_master_write_packet_wait(I2C_2, &transfer);
	if (i2c_status != STATUS_OK)
		return STATUS_ERR_OVERFLOW;
	

	// Read current bus raw
	uint8_t buffer[2] = { 0 };

	transfer.data_length = 2;
	transfer.data = buffer;

	i2c_status = i2c_master_read_packet_wait(I2C_2, &transfer);
	if (i2c_status != STATUS_OK)
		return STATUS_ERR_OVERFLOW;

	uint16_t p_bus_raw = ((buffer[0] << 8) & 0xFF00) | (buffer[1] & 0xFF);   // Decode uint16_t <- uint16_t | uint8_t
	*data_read = (uint32_t)p_bus_raw * LSB_PWR / 1000.0f;  // conversion to get microamps
	

	return STATUS_OK;
}

