/************************************************************************************************** *
 * 
 * FILE: eeprom_24lc32a.c
 * DATE: 18/07/2023
 * AUTHOR:  Jonathan Edwards
 * 
 * DESCRIPTION:
 *              I2C firmware for Michrochips 24lc32a eeprom.
 * 
***************************************************************************************************/

#include "eeprom_24lc32a.h"
#include "i2c_linker.h"



/**
 * \brief Check whether 24LC32A EEPROM is connected
 *
 * BYTE WRITE:
 * 				| Control Byte |
 * 
 * \return bool : status of eeprom
 *       - 0 : Device is present
 *       - 1 : Device is not acknowledging I2C address
  */
bool 
eeprom_24lc32a_is_connected (void)
{
	enum status_code i2c_status;
	
	struct i2c_master_packet transfer = {
		.address     = EEPOROM_ADDR_1,
		.data_length = 0,
		.data        = NULL,
	};

	// EEPROM 1 transfer
	i2c_status = i2c_master_write_packet_wait(I2C_3, &transfer);
	if (i2c_status != STATUS_OK)
		return STATUS_ERR_OVERFLOW;

	
	// EEPROM 2 transfer
	transfer.address = EEPOROM_ADDR_2;
	i2c_status = i2c_master_write_packet_wait(I2C_3, &transfer);
	if (i2c_status != STATUS_OK)
		return STATUS_ERR_OVERFLOW;
	

	return STATUS_OK;
}


/**
 * \brief Writes a Byte of data to the 24LC32A EEPROM
 *
 * BYTE WRITE:
 * 				| Control Byte | - | Addr High Byte | - | Addr Low Byte | - | Data |
 * 
 * \return bool : status of eeprom
 *       - 0 : Device is present
 *       - 1 : Device is not acknowledging I2C address
  */
bool 
eeprom_24lc32a_write_byte (uint8_t addr, uint16_t index, uint8_t _data)
{
	enum status_code i2c_status;

	uint8_t data[3];
	data[0] = index++;
	data[1] = index;
	data[2] = _data;
	
	struct i2c_master_packet transfer = {
		.address     = addr,
		.data_length = 3,
		.data        = data,
	};

	// EEPROM 1 transfer
	i2c_status = i2c_master_write_packet_wait(I2C_3, &transfer);
	if (i2c_status != STATUS_OK)
		return STATUS_ERR_OVERFLOW;


	return STATUS_OK;
}


/**
 * \brief Writes a Byte of data to the 24LC32A EEPROM
 *
 * Input - pos (index)
 * Input - data (string), finds the length of the string and writes
 * 	that many bytes, 
 * 
 * \return bool : status of eeprom
 *       - 0 : Okay
 *       - 1 : Error
 */
bool 
eeprom_24lc32a_write (uint8_t addr, uint16_t index, char *data)
{
	enum status_code i2c_status;

	for (uint8_t i = 0; i < strlen(data); i++)
	{
		i2c_status = eeprom_24lc32a_write_byte(addr, index, data[i]);
		if (i2c_status != STATUS_OK)
			return STATUS_ERR_OVERFLOW;

		HAL_Delay(10);
		index++;
	}

	


	return STATUS_OK;
}


/**
 * \brief Reads a Byte of data from the 24LC32A EEPROM
 *
 * BYTE READ:
 * 				| Control Byte | - | Addr High Byte | - | Addr Low Byte | - | Control Byte | - | Data |
 * 
 * \return bool : status of eeprom
 *       - 0 : Device is present
 *       - 1 : Device is not acknowledging I2C address
  */
bool
eeprom_24lc32a_read_byte (uint8_t addr, uint16_t index, u_int8_t *data_byte)
{
	enum status_code i2c_status;

	uint8_t data[2];
	data[0] = index++;
	data[1] = index;
	
	struct i2c_master_packet transfer = {
		.address     = addr,
		.data_length = 2,
		.data        = data,
	};

	// EEPROM 1 transfer
	i2c_status = i2c_master_write_packet_wait(I2C_3, &transfer);
	if (i2c_status != STATUS_OK)
		return STATUS_ERR_OVERFLOW;
	
	// Read bytes
	uint8_t buffer[1] = { 0 };

	transfer.data_length = 1;
	transfer.data = buffer;

	i2c_status = i2c_master_read_packet_wait(I2C_3, &transfer);
	if (i2c_status != STATUS_OK)
		return STATUS_ERR_OVERFLOW;

	*data_byte = transfer.data[0];


	return STATUS_OK;
}


/**
 * \brief Writes a Byte of data to the 24LC32A EEPROM
 *
 * Input - pos (index)
 * Input - data (string), finds the length of the string and writes
 * 	that many bytes, 
 * 
 * \return bool : status of eeprom
 *       - 0 : Okay
 *       - 1 : Error
 */
bool 
eeprom_24lc32a_read (uint8_t addr, uint16_t index, uint8_t max_len, char *data)
{
	enum status_code i2c_status;

	uint8_t data_byte = 0;

	for (uint8_t i = index; i < (index+max_len); i++)
	{
		i2c_status = eeprom_24lc32a_read_byte(addr, i, &data_byte);
		if (i2c_status != STATUS_OK)
			return STATUS_ERR_OVERFLOW;

		data[i] = data_byte;
	}


	return STATUS_OK;
}

