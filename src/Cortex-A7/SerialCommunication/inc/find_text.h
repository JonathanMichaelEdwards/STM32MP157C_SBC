/***************************************************************************************************
 * 
 * FILE: find_text.h
 * DATE: 01/11/2022
 * AUTHORS:  Jonathan Edwards
 * 
 * DESCRIPTION:
 *              Finds a string of text.
 * 
***************************************************************************************************/


#ifndef FIND_TEXT_H_
#define FIND_TEXT_H_

// Application libraries
#include "serial_interface.h"


/*
 * Finds a string of text.
 */
int
FindText(FD_TTY *fd_tty, const char *str_text, int *max_timeout, char read_buff);


#endif  /* FIND_TEXT_H_ */

