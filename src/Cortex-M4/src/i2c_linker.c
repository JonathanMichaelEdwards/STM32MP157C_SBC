/***************************************************************************************************
 * 
 * FILE: i2c_linker.c
 * DATE: 05/05/2022
 * AUTHOR:  Jonathan Edwards
 * 
 * DESCRIPTION:
 *              STM32MP1 I2C linker with the TE. ms8607 sensor.
 * 
***************************************************************************************************/

#include "i2c_linker.h"



/***************************************************************************************************
 *
 * FUNCTION NAME: i2c_master_inits (I2C1, I2C3, I2C5)
 *
 * DESCRIPTION: 
 *               Init master I2C software module.
 * 
 *
 *
 * ARGUMENT LIST:
 *
 *    Argument        Type    IO              Description
 * -------------   --------   --   ---------------------------------
 * 		-       	-         -    -
 *
 * RETURN VALUE: void
 *
***************************************************************************************************/
void
i2c1_master_init (void)
{
	hi2c1.Instance = I2C1;
	hi2c1.Init.Timing = 0x10707DBC;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	{
		Error_Handler();
	}

	/**Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		Error_Handler();
	}

	/**Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
	{
		Error_Handler();
	}
}


void
i2c2_master_init (void)
{
	hi2c2.Instance = I2C2;
	hi2c2.Init.Timing = 0x10707DBC;
	hi2c2.Init.OwnAddress1 = 0;
	hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c2.Init.OwnAddress2 = 0;
	hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	if (HAL_I2C_Init(&hi2c2) != HAL_OK)
	{
		Error_Handler();
	}

	/**Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		Error_Handler();
	}

	/**Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
	{
		Error_Handler();
	}
}


void
i2c3_master_init (void)
{
	hi2c3.Instance = I2C3;
	hi2c3.Init.Timing = 0x10707DBC;
	hi2c3.Init.OwnAddress1 = 0;
	hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c3.Init.OwnAddress2 = 0;
	hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	if (HAL_I2C_Init(&hi2c3) != HAL_OK)
	{
		Error_Handler();
	}

	/**Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		Error_Handler();
	}

	/**Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
	{
		Error_Handler();
	}
}


void
i2c5_master_init (void)
{
	hi2c5.Instance = I2C5;
	hi2c5.Init.Timing = 0x10707DBC;
	hi2c5.Init.OwnAddress1 = 0;
	hi2c5.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c5.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c5.Init.OwnAddress2 = 0;
	hi2c5.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c5.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c5.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	if (HAL_I2C_Init(&hi2c5) != HAL_OK)
	{
		Error_Handler();
	}

	/**Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c5, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		Error_Handler();
	}

	/**Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c5, 0) != HAL_OK)
	{
		Error_Handler();
	}
}


/***************************************************************************************************
 *
 * FUNCTION NAME: i2c_master_write_packet_wait
 *
 * DESCRIPTION: 
 *            Write I2C packet data from master to slave.
 *				- Packet Info - stored in a struct
 *
 *
 * ARGUMENT LIST:
 *
 *    Argument	   Type							IO   Description
 * -------------   ------------------------		--   ---------------------------------
 * packet          I2C_Master_Packet *const     IO   Sending packet - master -> slave 
 *
 * RETURN VALUE: Status_Code - If status code does not equal 'STATUS_OK' - Fail
 *
***************************************************************************************************/
Status_Code
i2c_master_write_packet_wait (uint8_t hi2c, I2C_Master_Packet *const packet)
{
	I2C_HandleTypeDef _hi2c;

	// switch to the i2c line being used
	switch (hi2c)
	{
		case I2C_1:
			_hi2c = hi2c1;
			break;
		case I2C_2:
			_hi2c = hi2c2;
			break;
		case I2C_3:
			_hi2c = hi2c3;
			break;
		case I2C_5:
			_hi2c = hi2c5;
			break;
	}

	if (HAL_I2C_Master_Transmit(&_hi2c, (packet->address << 1), packet->data, packet->data_length, HAL_MAX_DELAY) != STATUS_OK)
		return HAL_ERROR;
	
	
	return STATUS_OK;
}


/***************************************************************************************************
 *
 * FUNCTION NAME: i2c_master_read_packet_wait
 *
 * DESCRIPTION: 
 *            Reading I2C packet data from the slave to the master.
 *				- Packet Info - stored in a struct
 *
 *
 * ARGUMENT LIST:
 *
 *    Argument	   Type							IO   Description
 * -------------   ------------------------		--   ---------------------------------
 * packet          I2C_Master_Packet *const     IO   Sending packet - master -> slave 
 *
 * RETURN VALUE: Status_Code - If status code does not equal 'STATUS_OK' - Fail
 *
***************************************************************************************************/
Status_Code
i2c_master_read_packet_wait (uint8_t hi2c, I2C_Master_Packet *const packet)
{
	I2C_HandleTypeDef _hi2c;

	// switch to the i2c line being used
	switch (hi2c)
	{
		case I2C_1:
			_hi2c = hi2c1;
			break;
		case I2C_2:
			_hi2c = hi2c2;
			break;
		case I2C_3:
			_hi2c = hi2c3;
			break;
		case I2C_5:
			_hi2c = hi2c5;
			break;
	}

	if (HAL_I2C_Master_Receive(&_hi2c, (packet->address << 1), packet->data, packet->data_length, HAL_MAX_DELAY) != STATUS_OK)
		return HAL_ERROR;
	

	return STATUS_OK;
}


/***************************************************************************************************
 *								No currently used in software lib.
***************************************************************************************************/
Status_Code
i2c_master_write_packet_wait_no_stop (uint8_t hi2c, I2C_Master_Packet *const packet)
{	
	return STATUS_OK;
}

