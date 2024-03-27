/***************************************************************************************************
 * 
 * FILE: autofocus.c
 * DATE: 25/11/2022 - 28/07/2023
 * AUTHORS:  Jonathan Edwards
 * 
 * DESCRIPTION:
 *              Main application file contains 2 sub applications, which detects for crystals 
 *              or autofocuses the Camera. 
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
 * Run Example:
 *              cd /usr/bin/ && gpioset gpiochip3 4=1 && ./autofocus 100
 * 
 **************************************************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <assert.h>


#define DEVICE_ADDR             0x76  // 0b11101100

#define POWER_MODE              0x00  // active cmd
#define LENS_SLEEP              0
#define LENS_ACTIVE             1

#define HIGH_VOLTAGE_OUTPUT     0x01  // output cmd



/***************************************************************************************************
 *
 * FUNCTION NAME: main
 *
 * DESCRIPTION: 
 *               Runs the main CrystalDetection application that contains two sub applications.
 *                  1. crystal_detection.cpp
 *                      * This requires both a BayerRG8 and Mono8 image to be taken
 *                          * Mono8 uses the UV light source 
 *                          * BayerRG8 uses the White light source 
 *                      1. The Mono8 image is used to outline the lightest regions on the image.
 *                          - showing the crystals fluorescence - illuminate bright white
 *                      2. The Mono8 outlined fluorescence regions will then be masked over the 
 *                         BayerRG8 image.
 *                  2. autofocus.cpp
 *                      * Autofocuses the camera on the current sample to the next sample.
 *                          * Detect if all the wells can  
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
 * 
 * RETURN VALUE: int - 0 for Success otherwise an Error has occured
 *
 **************************************************************************************************/
int
main (int argc, char *argv[])
{
    uint8_t focus_val = 0x00;

    /*
     * Determine if the input is a valid focus value.
     */
    assert(argc == 2);
    assert((atoi(argv[1]) >= 0) && (atoi(argv[1]) <= 255));
    focus_val = atoi(argv[1]);

    /* 
     * Open up i2c4.
     *  - use "ls -l /sys/bus/i2c/devices" to check if found
     *  - `/dev/i2c-1` is linked to i2c 4 (address 0x5c002000)
     */ 
    int fd_i2c_lens_fd = open("/dev/i2c-1", O_RDWR);
    if (fd_i2c_lens_fd < 0)
    {
        printf("Error %i from open: %s\n", errno, strerror(errno));
        return errno;
    }
    
    /* 
     * Specify the device address we want to communicate with.
     */
    if (ioctl(fd_i2c_lens_fd, I2C_SLAVE, DEVICE_ADDR) < 0)
    {
        printf("Error %i from ioctl: %s\n", errno, strerror(errno));
        return errno;
    }
    
    /* 
     * Check connection
     */
    __s32 res = write(fd_i2c_lens_fd, NULL, 0);
    if (res < 0) 
    {
        printf("Error %i from write: %s\n", errno, strerror(errno));
        return errno;
    }

    /*
     * Turn on the crystal
     *      -  0 = sleep
     *      -  1 = Active
     * 
     *  BYTE WRITE: 
     *          | Control Byte | - | CMD reg | - | Data |
     */
    uint8_t data[2];
    data[0] = POWER_MODE;
    data[1] = LENS_ACTIVE;

    res =  write(fd_i2c_lens_fd, data, 2);
    if (res < 0) 
    {
        printf("Error %i from write: %s\n", errno, strerror(errno));
        return errno;
    }

    /*
     * Writing the focus value to the Liquid Lens driver.
     *  - focus_val of 0 to 255 = output controlled
     * 
     * Read the MAX14515 datasheet for more details.
     */
    for (uint8_t i = 0; i < focus_val; i++)
    {
        data[0] = HIGH_VOLTAGE_OUTPUT;
        data[1] = i;

        res =  write(fd_i2c_lens_fd, data, 2);
        if (res < 0) 
        {
            printf("Error %i from write: %s\n", errno, strerror(errno));
            return errno;
        }
        
        // Extra prevention damage delay - > 2 ms
        usleep(20000);
    }

    return EXIT_SUCCESS;
}




///////////////////////// HV892 Example
// int
// main(int argc, char *argv[])
// {
    // const char DEVICE_ADDR = 0x23;  // 0b0100011
//     uint8_t focus_val = 0x00;

//     /*
//      * Determine if the input is a valid focus value.
//      */
//     assert(argc == 2);
//     assert((atoi(argv[1]) >= 0) && (atoi(argv[1]) <= 255));
//     focus_val = atoi(argv[1]);

//     /* 
//      * Open up i2c4.
//      *  - use "ls -l /sys/bus/i2c/devices" to check if found
//      *  - `/dev/i2c-1` is linked to i2c 4 (address 0x5c002000)
//      */ 
//     int fd_i2c_lens = open("/dev/i2c-1", O_RDWR);
//     if (fd_i2c_lens < 0)
//     {
//         printf("Error %i from open: %s\n", errno, strerror(errno));

//         return errno;
//     }
    
//     /* 
//      * Specify the device address we want to communicate with.
//      */
//     if (ioctl(fd_i2c_lens, I2C_SLAVE, DEVICE_ADDR) < 0)
//     {
//         printf("Error %i from ioctl: %s\n", errno, strerror(errno));

//         return errno;
//     }
    

//     /*
//      * Writing the focus value to the Liquid Lens driver.
//      *  - focus_val of 0 = sleep mode (starting point)
//      *  - focus_val of 1 t0 255 = output controlled
//      * 
//      * Read the HV892 datasheet for more details.
//      */
//     for (uint16_t i = 0; i < focus_val; i++)
//     {
//         __s32 res =  write(fd_i2c_lens, &i, 1);
//         if (res != 1) 
//         {
//             printf("Error %i from write: %s\n", errno, strerror(errno));

//             return errno;
//         }
        
//         // Extra prevention damage delay - > 2 ms
//         if (usleep(10000) !=0)
//         {
//             printf("Error %i from usleep: %s\n", errno, strerror(errno)); 
            
//             return errno;
//         }
//     }


//     return EXIT_SUCCESS;
// }

