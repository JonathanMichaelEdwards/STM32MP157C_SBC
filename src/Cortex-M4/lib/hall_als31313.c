/***************************************************************************************************
 * 
 * FILE: hall_als31313.c
 * DATE: 26/07/2023
 * AUTHOR:  Jonathan Edwards
 * 
 * DESCRIPTION:
 *              I2C firmware for ALS31313 Hall Effect Sensor.
 * 
***************************************************************************************************/

#include "hall_als31313.h"
#include "i2c_linker.h"



/**
 * \brief Check whether ALS31313 Hall is connected
 *
 * BYTE WRITE:
 * 				| Control Byte |
 * 
 * \return bool : status of eeprom
 *       - 0 : Device is present
 *       - 1 : Device is not acknowledging I2C address
  */
bool 
hall_als31313_is_connected (uint8_t addr)
{
	enum status_code i2c_status;
	
	struct i2c_master_packet transfer = {
		.address     = addr,
		.data_length = 0,
		.data        = NULL,
	};

	// Hall transfer
	i2c_status = i2c_master_write_packet_wait(I2C_1, &transfer);
	if (i2c_status != STATUS_OK)
		return STATUS_ERR_OVERFLOW;
	

	return STATUS_OK;
}


/**
 * \brief Writes 4 Bytes of data to the ALS31313 Hall - Enter customer access mode
 *
 * BYTE WRITE:
 * 				| Control Byte | - | CMD reg | - | Data 0 (MSB) | - | Data 1 | - | Data 2 | - | Data 3 (LSB) |
 * 
 * \return bool : status of eeprom
 *       - 0 : Device is present
 *       - 1 : Device is not acknowledging I2C address
  */
bool 
hall_als31313_init (uint8_t addr)
{
	enum status_code i2c_status;

	uint8_t data[5];
	const uint32_t customer_code = 0x2C413534;

	data[0] = CUSTOMER_ACCESS;
	data[1] = (uint8_t)(customer_code >> 24);
	data[2] = (uint8_t)(customer_code >> 16);
	data[3] = (uint8_t)(customer_code >> 8);
	data[4] = (uint8_t)(customer_code);
	
	struct i2c_master_packet transfer = {
		.address     = addr,
		.data_length = 5,
		.data        = data,
	};

	// Write data to cmd reg
	i2c_status = i2c_master_write_packet_wait(I2C_1, &transfer);
	if (i2c_status != STATUS_OK)
		return STATUS_ERR_OVERFLOW;
	

	return STATUS_OK;
}


/**
 * \brief Reads ALS31313 Hall loop mode (Write then read)
 *
 * BYTE READ:
 * 				| Control Byte | - | CMD reg | -- | Control Byte | - | Data 0 (MSB) | - | Data 1 | - | Data 2 | - | Data 3 (LSB) |
 * 
 * \return bool
 *       - 0 : Device is present
 *       - 1 : Device is not acknowledging I2C address
 */
bool
hall_als31313_read (uint8_t addr, uint8_t cmd, uint32_t *data_read)
{
	enum status_code i2c_status;

	uint8_t data[1];
	data[0] = cmd;
	
	struct i2c_master_packet transfer = {
		.address     = addr,
		.data_length = 1,
		.data        = data,
	};

	// Voltage Reg request read
	i2c_status = i2c_master_write_packet_wait(I2C_1, &transfer);
	if (i2c_status != STATUS_OK)
		return STATUS_ERR_OVERFLOW;
	
	
	// Read voltage bus raw
	uint8_t buffer[4] = { 0 };

	transfer.data_length = 4;
	transfer.data = buffer;

	i2c_status = i2c_master_read_packet_wait(I2C_1, &transfer);
	if (i2c_status != STATUS_OK)
		return STATUS_ERR_OVERFLOW;


	*data_read  = ((buffer[0] << 24) & 0xFF000000) | ((buffer[1] << 16) & 0xFF0000) | ((buffer[2] << 8) & 0xFF00) | (buffer[3] & 0xFF);  // read 32 bits (4 bytes)
	

	return STATUS_OK;
}


/**
 * \brief Write ALS31313 Hall loop mode
 *
 * BYTE WRITE:
 * 				| Control Byte | - | CMD reg | - | Data 0 (MSB) | - | Data 1 | - | Data 2 | - | Data 3 (LSB) |
 * 
 * \return bool
 *       - 0 : Device is present
 *       - 1 : Device is not acknowledging I2C address
 */
bool
hall_als31313_write (uint8_t addr, uint8_t cmd, uint8_t data_mode)
{
	enum status_code i2c_status;

	uint8_t data[5];
	data[0] = cmd;
	data[1] = (uint8_t)(data_mode >> 24);
	data[2] = (uint8_t)(data_mode >> 16);
	data[3] = (uint8_t)(data_mode >> 8);
	data[4] = (uint8_t)(data_mode);
	
	struct i2c_master_packet transfer = {
		.address     = addr,
		.data_length = 5,
		.data        = data,
	};

	// Write data to cmd reg
	i2c_status = i2c_master_write_packet_wait(I2C_1, &transfer);
	if (i2c_status != STATUS_OK)
		return STATUS_ERR_OVERFLOW;
	

	return STATUS_OK;
}


//
// sign_extend_bitfield
//
// Sign extend a right justified value
//
int64_t 
sign_extend_bitfield (uint32_t data, uint8_t width)
{
    int64_t x = (int64_t)data;
    int64_t mask = 1L << (width - 1);

    if (width < 32)
    {
        x = x & ((1 << width) - 1);  // make sure the upper bits are zero
    }

    return (int64_t)((x ^ mask) - mask);
}

