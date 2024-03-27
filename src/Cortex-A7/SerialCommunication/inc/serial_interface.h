/***************************************************************************************************
 * 
 * FILE: serial_interface.h
 * DATE: 20/11/2022
 * AUTHORS:  Jonathan Edwards
 * 
 * DESCRIPTION:
 *              
 * 
 **************************************************************************************************/


#ifndef SERIAL_INTERFACE_H_
#define SERIAL_INTERFACE_H_

#include <termios.h>
#include <unistd.h>
#include <errno.h>


/* 
 *  FD file structure.
 */
typedef struct fd_tty 
{
    int fd_stdin;
    int fd_stdout;
    int fd_serial_port;
    int fd_dst;
} FD_TTY;


// Camera config
#define     CAM_DONE            ">> ["

#define     LOGIN               "root@stm32mp1:"  // If this is not found, the board must be booting
#define     BOOT_FINISHED       "vdda: disabling"  // Finished booting
#define     BOOT_ERROR          "STM32MP>"

// Ethernet configs
#define     CONFIG_ETHERNET     "ifconfig eth0 169.254.128.28 broadcast 169.254.255.255 netmask 255.255.0.0"
#define     ETH0_UP             "eth0: Link is Up"
#define     TIMEOUT   100e3


/*
 * Termios init configs.
 */
int
Init_Termios(FD_TTY *fd_tty);


#endif  /* SERIAL_INTERFACE_H_ */

