/***************************************************************************************************
 * 
 * FILE: serial_interface.c
 * DATE: 20/11/2022
 * AUTHORS:  Jonathan Edwards, 
 * 
 * DESCRIPTION:
 *              Print Vimba Version.
 * 
 **************************************************************************************************/


// C standard libraries
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Application libraries
#include "serial_interface.h"



/***************************************************************************************************
 *
 * FUNCTION NAME: Setup_Termios
 *
 * DESCRIPTION: 
 *              Configs a new terminal interface 
 *
 * ACCESS SPECIFIERS:
 *                    - Private
 *
 * ARGUMENT LIST:
 *
 *    Argument        Type    IO              Description
 * -------------   ----------------   --   ---------------------------------
 *  tty            struct termios *    I   - termios struct
 * 
 * RETURN VALUE: int - 0 for Success otherwise an Error has occured
 *
 **************************************************************************************************/
int
Setup_Termios(struct termios *tty)
{
    /* 
     * Set in/out baud rate.
     */
    int error = cfsetispeed(tty, (speed_t)B115200);
    error = cfsetospeed(tty, (speed_t)B115200);  
    if (error != 0)  
        printf("Error %i from cfsetospeed or cfsetispeed: %s\n", errno, strerror(errno));

    tty->c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
    tty->c_cflag &= ~CSIZE;
    tty->c_cflag |= CS8;         /* 8-bit characters */
    tty->c_cflag &= ~PARENB;     /* no parity bit */
    tty->c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
    tty->c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */

    /* setup for non-canonical mode */
    tty->c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty->c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty->c_oflag &= ~OPOST;

    /* fetch bytes as they become available */
    tty->c_cc[VMIN] = 1;
    tty->c_cc[VTIME] = 0;


    return error;
}


/***************************************************************************************************
 *
 * FUNCTION NAME: Init_Termios
 *
 * DESCRIPTION: 
 *              Setups a terminal interface 
 *
 * ACCESS SPECIFIERS:
 *                    - Private
 *
 * ARGUMENT LIST:
 *
 *    Argument        Type    IO              Description
 * -------------   ----------------   --   ---------------------------------
 *  tty            struct termios *    I   - termios struct
 * 
 * RETURN VALUE: int - 0 for Success otherwise an Error has occured
 *
 **************************************************************************************************/
int
Init_Termios(FD_TTY *fd_tty)
{
    struct termios tty;

    /*
     * Read in existing settings.
     */
    int error = tcgetattr(fd_tty->fd_serial_port, &tty);
    if (error != 0) 
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));

    /*
     * Termios setup shell settings.
     */
    error = Setup_Termios(&tty);
    if (error != 0) 
        printf("Error %i from Setup_Termios: %s\n", errno, strerror(errno));

    /* 
     * Save tty settings.
     */
    error = tcsetattr(fd_tty->fd_serial_port, TCSANOW, &tty);
    if (error != 0)
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));

    /*
     * Flush the Input FD ports.
     */
    error = tcflush(fd_tty->fd_serial_port, TCIOFLUSH);
    if (error != 0)
        printf("Error %i from tcflush: %s\n", errno, strerror(errno));
        
    for (int i = 0; i < 1024; i++)
    {
        tcflush(fd_tty->fd_stdin, TCIOFLUSH);
        tcflush(fd_tty->fd_serial_port, TCIOFLUSH);
    }


    return error;
}

