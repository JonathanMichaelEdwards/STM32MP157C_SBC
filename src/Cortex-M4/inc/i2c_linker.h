/***************************************************************************************************
 * 
 * FILE: i2c_linker.h
 * DATE: 06/08/2021, 28-31/08/2021, 06/09/2021, 18/07/2023
 * AUTHOR:  Jonathan Edwards
 * 
 * DESCRIPTION:
 *              STM32MP1 I2C linker
 * 
***************************************************************************************************/

#ifndef _I2C_MS8607_H_
#define _I2C_LINKER_H_

#include "main.h"


/* I2C Macros */
#define    F_SCL				100000UL        /* 100kHz SCL frequency */
#define    TW_READ_ACK			1
#define    TW_READ_NACK		    0

/* Converting Factors */
#define		TEMP_FACTOR          100
#define		PRESSURE_FACTOR      10
#define		HUMIDITY_FACTOR      100


static I2C_HandleTypeDef hi2c1;
static I2C_HandleTypeDef hi2c2;
static I2C_HandleTypeDef hi2c3;
static I2C_HandleTypeDef hi2c5;


/***************************************************************************************************
 *                                         Enums & Structs
***************************************************************************************************/

/*
 * I2C 'master' transfer direction.
 *	- read or write
 */
typedef enum i2c_transfer_direction 
{
    I2C_TRANSFER_WRITE = 0,
    I2C_TRANSFER_READ  = 1,
} I2C_Transfer_Direction;

/*
 * I2C line (1, 3, 5)
 *	- read or write
 */
typedef enum i2c_line 
{
    I2C_1 = 0,
    I2C_2,
    I2C_3,
    I2C_5,
} I2C_Line;

/*
 * I2C 'master' status code error code logic
 * with the ms8607 sensor.
 */
typedef enum status_code 
{
    STATUS_OK             = 0x00,
    STATUS_ERR_OVERFLOW	  = 0x01,
    STATUS_ERR_TIMEOUT    = 0x02,
} Status_Code;

/*
 * I2C 'master' <-> 'slave' transfer struct.
 */
typedef struct i2c_master_packet 
{
    uint16_t address;		/* Address to slave device */
    uint16_t data_length;   /* Length of data array */
    uint8_t *data;			/* Data array containing all data to be transferred */
} I2C_Master_Packet;


/*
 * Init master I2C software module.
 */
void 
i2c1_master_init (void);
void 
i2c2_master_init (void);
void 
i2c3_master_init (void);
void 
i2c5_master_init (void);

/*
 * Reading I2C packet data from the slave to the master.
 */
Status_Code
i2c_master_read_packet_wait (uint8_t _hi2c, I2C_Master_Packet *const packet);

/*
 * Reading I2C data from the slave to the master.
 *  - used with the TE. ms8607 sensor.
 */
Status_Code 
i2c_master_write_packet_wait (uint8_t _hi2c, I2C_Master_Packet *const packet);

/*
 * Not currently used with the ms8607 software lib.
 *	- Continuously running with no stop condition
 */
Status_Code
i2c_master_write_packet_wait_no_stop (uint8_t _hi2c, I2C_Master_Packet *const packet);


#endif  // _I2C_LINKER_H_

