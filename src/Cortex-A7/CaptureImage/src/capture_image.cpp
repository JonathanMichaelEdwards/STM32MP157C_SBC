/***************************************************************************************************
 * 
 * FILE: capture_image.cpp
 * DATE: 01/08/2022
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


// C++ standard libraries
#include <cstdio>
#include <cstring>

// Vimba libraries
#include <vimbaC.h>
#include <vmb_common_types.h>

// Application libraries
#include "load_settings.h"
#include "capture_process_image.hpp"



/***************************************************************************************************
 *
 * FUNCTION NAME: main
 *
 * DESCRIPTION: 
 *               Runs the main CaptureImage application.
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
signed int
main (int argc, char *argv[])
{
    VmbError_t error = VmbErrorSuccess;

    /* 
     * If an input has been given, the host has control.
     */
    if (argc > 1)
    {
        /*
         * Loads settings.
         */
        error = LoadSettings(argv[1]);
        if (error != VmbErrorSuccess)   
        {
            printf("Exiting application, could not load settings. Error code: %d\n", error);

            return error;
        }
                        
        /* 
         * Capture, process, and store image.
         */
        error = CaptureProcessImage(argv[1]);
        if (error != VmbErrorSuccess)   
        {
            printf("Exiting application, could not capture and store image. Error code: %d\n", error);

            return error;
        }


        return error;
    }


    return error;
}

