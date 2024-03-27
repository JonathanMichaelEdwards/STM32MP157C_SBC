/***************************************************************************************************
 * 
 * FILE: find_text.c
 * DATE: 01/11/2022
 * AUTHORS:  Jonathan Edwards, 
 *              Copyright (C) 2014 - 2016 Allied Vision Technologies.  All Rights Reserved.
 *              Redistribution of this file, in original or modified form, without
 *              prior written consent of Allied Vision Technologies is prohibited.
 * 
 * DESCRIPTION:
 *              Print Vimba Version.
 * 
 **************************************************************************************************/


// C standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>

// Application libraries
#include "find_text.h"
#include "serial_interface.h"



/***************************************************************************************************
 *
 * FUNCTION NAME: FindText
 *
 * DESCRIPTION: 
 *               Finds a string of text and returns the error code.
 *
 * ACCESS SPECIFIERS:
 *                    + Public
 *
 * ARGUMENT LIST:
 *
 *    Argument        Type    IO              Description
 * -------------   --------   --   ---------------------------------
 * Void
 * 
 * RETURN VALUE: 0 = Success, 1 = string has been found.
 *
***************************************************************************************************/
int
FindText(FD_TTY *fd_tty, const char *str_text, int *max_timeout, char read_buff)
{
    char *txt_input = (char *)malloc(sizeof(char)*128 + 1);
    memset(txt_input, '\0', strlen(txt_input));
    int error = 1;

    int i = 0;

    txt_input[0] = read_buff;
    while (((txt_input[i]) == str_text[i]) && (i < strlen(str_text)))
    {
        int num_bytes = read(fd_tty->fd_serial_port, &read_buff, sizeof(char));
        if (num_bytes < 0)
            printf("Error %i from read: %s\n", errno, strerror(errno));
        num_bytes = write(fd_tty->fd_dst, &read_buff, sizeof(read_buff));
        txt_input[++i] = read_buff;
        printf("%c", read_buff);
        
        /*
         * Find the whole string to determine 
         * Otherwise 
         */
        if (i == strlen(str_text))
        {
            if (str_text[1] == '>')
            {
                /*
                 * Find ending character ']' to determine the 
                 * error code given by the stm32mp157 task.
                 */
                int j = 0;
                txt_input[strlen(str_text)] = txt_input[i];
                while (txt_input[strlen(str_text)+(j++)] != ']')
                {
                    num_bytes = read(fd_tty->fd_serial_port, &txt_input[strlen(str_text)+j], sizeof(char));
                    num_bytes = write(fd_tty->fd_dst, &txt_input[strlen(str_text)+j], sizeof(char));
                    printf("%c", txt_input[strlen(str_text)+j]);
                }

                char str_error_code[strlen(str_text)];
                for (int k = 0; k < (j-1); k++)
                    sprintf(&str_error_code[k], "%c", txt_input[strlen(str_text)+k]);
                error = atoi(str_error_code);
                *max_timeout = 5;
                free(txt_input);
                
                return error;
            }
            else
                error = 10;
        }
    }
    
    free(txt_input);


    return error;
}

