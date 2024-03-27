/***************************************************************************************************
 * 
 * FILE: serial_com.c
 * DATE: 01/11/2022
 * AUTHORS:  Jonathan Edwards
 * 
 * DESCRIPTION:
 *              Main application file contains 3 main tasks and does error checking. 
 *              This application has been tested on a x86_64 Linux machine and the stm32mp157.
 *
 *              1. Loads Alvium Camera settings stored in a '.xml' file. The '.xml' file can be 
 *                  created using VimbaViewer.  
 *                  - Greyscale (Mono8) - 20 MB
 *                  - Colorscale (BayerRG8) - 60 MB
 *              2. If a Alvium Camera is avaliable and settings have loaded successfully,
 *                  capture the image.
 *              3. Process and store the image if an image was taken
 *                  - If BayerRG8 pixel format is used, use the demosaicing algorithm to 
 *                     reconstruct the image.
 *                  - Else, just write image data
 *                  - Store the image as a '.bmp'
 * 
 **************************************************************************************************/


// C standard libraries
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>

// Application libraries
#include "serial_interface.h"
#include "find_text.h"




void Close_FD(FD_TTY *fd_tty)
{
    close(fd_tty->fd_stdin);
    close(fd_tty->fd_stdout);
    close(fd_tty->fd_serial_port);
    close(fd_tty->fd_dst);
}



/***************************************************************************************************
 *
 * FUNCTION NAME: main
 *
 * DESCRIPTION: 
 *               Runs the main Imaging application.
 *                  1. Loads settings
 *                  2. Capture the image
 *                  3. Process and store the image
 *
 * ACCESS SPECIFIERS:
 *                    - Private
 *
 * ARGUMENT LIST:
 *
 *    Argument        Type    IO              Description
 * -------------   --------   --   ---------------------------------
 *  argc              int      I   - Number of inputs specified
 *  argv            char**     I   - Type of input
 *  argv[1]          char*     I   - Camera load settings file
 * 
 * RETURN VALUE: int - 0 for Success otherwise an Error has occured
 *
 **************************************************************************************************/
int 
main (int argc, char **argv)
{
    FD_TTY fd_tty;

    // Close_FD(&fd_tty);
    /*
     * Open file descriptors.
     */
    fd_tty.fd_stdout = fileno(stdout);
    fd_tty.fd_stdin = fileno(stdin);
    fd_tty.fd_serial_port = open("/dev/ttyACM0", O_RDWR, S_IRUSR);
    fd_tty.fd_dst = open("scripts/log/monitor.log", O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);
    if ((fd_tty.fd_stdin < 0) || (fd_tty.fd_serial_port < 0) || (fd_tty.fd_dst < 0) || (fd_tty.fd_stdout < 0))
    {
        printf("Error %d from open: %s\n", errno, strerror(errno));

        return errno;
    }
    

    /* 
     * Terminal read timeout settings - for select()
     */
    fd_set set_key, set_read;
    struct timeval timeout_read, timeout_key;

    int num_bytes = 0;
    bool host_text = false;
    bool host_input_text = false;

    timeout_read.tv_sec = 0;
    timeout_read.tv_usec = TIMEOUT;

    FD_ZERO(&set_read);              /* Clear the set */
    FD_SET(fd_tty.fd_serial_port, &set_read);  /* Add our file descriptor to the set */

    timeout_key.tv_sec = 0;
    timeout_key.tv_usec = TIMEOUT;

    FD_ZERO(&set_key);
    FD_SET(fd_tty.fd_stdin, &set_key);

    char read_buff = '\0';
    char write_buff = '\0';
    char *cmd = (char *)malloc(sizeof(char)*128 + 1);  

    bool exit_flag = false;
    int read_count = 0;
    int max_timeout = 1000;
    int cmd_wait_timeout = 2;


    /*
     * Initialise a new shell using termios.
     */
    int error = Init_Termios(&fd_tty);
    if (error != 0)
        printf("Error %i from Init_Termios\n", error);

    /*
     * Parse the command line arguments
     */
    for (int i = 0; i < argc; ++i)
    {
        if (strcmp (argv[i], "-Q") == 0)
            exit_flag = true;
        else if (strcmp(argv[i], "-t") == 0) 
        {
            if (i == argc - 1)
            {
                fprintf(stderr, "Error: expected timeout integer -t!\n");
                return EXIT_FAILURE;
            }
            max_timeout = atoi(argv[++i]);
        }
        else if (strcmp (argv[i], "-E") == 0)
        {
            strcpy(cmd, CONFIG_ETHERNET);
            for (int i = 0; i < strlen(cmd); i++)
                num_bytes = write(fd_tty.fd_serial_port, &cmd[i], sizeof(char));  // num_bytes = write a byte at a time
        }
        else if (strcmp (argv[i], "-B") == 0)
        {
            // Booting process
        }
        else if (strcmp (argv[i], "-M") == 0)
        {
            strcpy(cmd, "cd /usr/bin/\n");
            for (int i = 0; i < strlen(cmd); i++)
                num_bytes = write(fd_tty.fd_serial_port, &cmd[i], sizeof(char));
            strcpy(cmd, "sh ./capture_image.sh -M\n");
            for (int i = 0; i < strlen(cmd); i++)
                num_bytes = write(fd_tty.fd_serial_port, &cmd[i], sizeof(char));
        }
        else if (strcmp (argv[i], "-C") == 0)
        {
            strcpy(cmd, "cd /usr/bin/\n");
            for (int i = 0; i < strlen(cmd); i++)
                num_bytes = write(fd_tty.fd_serial_port, &cmd[i], sizeof(char));
            strcpy(cmd, "sh ./capture_image.sh -C\n");
            for (int i = 0; i < strlen(cmd); i++)
                num_bytes = write(fd_tty.fd_serial_port, &cmd[i], sizeof(char));
        }
        else if (strcmp (argv[i], "-D") == 0)
        {
            strcpy(cmd, "ifconfig eth0 down");
            for (int i = 0; i < strlen(cmd); i++)
                num_bytes = write(fd_tty.fd_serial_port, &cmd[i], sizeof(char));  // num_bytes = write a byte at a time
        }
        else if (strcmp (argv[i], "-I") == 0)
        {
            strcpy(cmd, "ifconfig");
            for (int i = 0; i < strlen(cmd); i++)
                num_bytes = write(fd_tty.fd_serial_port, &cmd[i], sizeof(char));  // num_bytes = write a byte at a time
        }
        else if (strcmp (argv[i], "-L") == 0)
        {
            strcpy(cmd, "cd /usr/bin/\n");
            for (int i = 0; i < strlen(cmd); i++)
                num_bytes = write(fd_tty.fd_serial_port, &cmd[i], sizeof(char));
            strcpy(cmd, "sh ./controller_A7.sh\n");
            for (int i = 0; i < strlen(cmd); i++)
                num_bytes = write(fd_tty.fd_serial_port, &cmd[i], sizeof(char));
        }
    }

    // Terminate
    write_buff = '\n';
    num_bytes = write(fd_tty.fd_serial_port, &write_buff, sizeof(char));
               

    while (exit_flag != true)
    {
        int rv_read = select(fd_tty.fd_serial_port + 1, &set_read, NULL, NULL, &timeout_read);
        int rv_key = select(fd_tty.fd_stdin + 1, &set_key, NULL, NULL, &timeout_key);

        if((rv_read == -1) || (rv_key == -1))
        {
            printf("Error %i from select: %s\n", errno, strerror(errno));

            return -1;
        }
        else if (rv_read == 1)
        {            
            num_bytes = read(fd_tty.fd_serial_port, &read_buff, sizeof(char));
            if (num_bytes < 0)
                printf("Error %i from read: %s\n", errno, strerror(errno));
            num_bytes = write(fd_tty.fd_dst, &read_buff, sizeof(char));
            printf("%c", read_buff);


            /*
             * Check to see if Imaging.sh has completed, write
             * the Error code to the log and exit application if applicable.
             *  - If an error is detected - timeout application.
             */


            // 'STM32MP>' - If found, the client board will reset.
            int str_code = FindText(&fd_tty, BOOT_ERROR, &max_timeout, read_buff);
            if (str_code == 10)
            {
                strcpy(cmd, "reset\n");
                for (int i = 0; i < strlen(cmd); i++)
                    num_bytes = write(fd_tty.fd_serial_port, &cmd[i], sizeof(char));
            }

            // // 'vdda: disabling' - If found, the client board will reset.
            // str_code = FindText(&fd_tty, BOOT_FINISHED, &max_timeout, read_buff);
            // if (str_code == 10)
            // {
            //     host_text = true;
            //     strcpy(cmd, "\r\n");
            //     for (int i = 0; i < strlen(cmd); i++)
            //         num_bytes = write(fd_tty.fd_serial_port, &cmd[i], sizeof(char));
            // }

            // 'root@stm32mp1:' - If found, the Host can now enter an input.
            str_code = FindText(&fd_tty, LOGIN, &max_timeout, read_buff);
            if (str_code == 10)
                host_text = true;


            // '>> [' - get the error code.
            str_code = FindText(&fd_tty, CAM_DONE, &max_timeout, read_buff);
            if ((str_code >= 200) || (str_code == 0))
            {
                error = str_code;
                cmd_wait_timeout = max_timeout;
            }

            read_count = 0; 
        }
        else if ((rv_key == 1) && (host_input_text == true))
        {
            /* 
            *  Exit loop when a 'New line' is entered and terminate
            *  Characters are read from fd_tty.fd_stdin and written when the Enter is pressed 
            */
            exit_flag = false;
            while ((write_buff=getchar()) != '\n')
            {
                /*
                *  Check action commands, otherwise directly write from the input.
                */
                if (write_buff == 'Q')
                    exit_flag = true;
                else
                    num_bytes = write(fd_tty.fd_serial_port, &write_buff, sizeof(char));
            }

            // Termiante
            write_buff = '\n';
            num_bytes = write(fd_tty.fd_serial_port, &write_buff, sizeof(char));

            read_count = 0;
            host_input_text = false;
        }
        else
        {
            if (rv_read == 0)
            {
                read_count++;

                timeout_read.tv_sec = 0;
                timeout_read.tv_usec = TIMEOUT;

                FD_ZERO(&set_read);
                FD_SET(fd_tty.fd_serial_port, &set_read);
            }
            if (rv_key == 0)
            {
                timeout_key.tv_sec = 0;
                timeout_key.tv_usec = TIMEOUT;

                FD_ZERO(&set_key);
                FD_SET(fd_tty.fd_stdin, &set_key);
            }
        }

        /*
         * The Host can enter an input command.
         */
        if ((read_count > cmd_wait_timeout) && (host_text == true) && (host_input_text == false))
        {   
            char *host_enter = "Host>> ";
            num_bytes = write(fd_tty.fd_stdout, host_enter, sizeof(host_enter));
            host_input_text = true;
        }

        // if (read_count > max_timeout)
        // {           
        //     write_buff = '\n';
        //     num_bytes = write(fd_tty.fd_serial_port, &write_buff, sizeof(char));
        //     exit_flag = true;
        // }
    }

    Close_FD(&fd_tty);
    free(cmd);


    return error;
}

