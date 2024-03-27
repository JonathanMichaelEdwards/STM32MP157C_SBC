/***************************************************************************************************
 * 
 * FILE: process_image.cpp
 * DATE: 01/08/2022 - 01/09/2022
 * AUTHORS:  Jonathan Edwards
 * 
 * DESCRIPTION:
 *               Process and save the image as a '.bmp' given the pixel format.
 * 
***************************************************************************************************/


// C++ libraries
#include <opencv2/opencv.hpp>
#include <iostream>

// Vimba libraries
#include <vimbaC.h>
#include <vmb_common_types.h>

// Application libraries
#include "process_image.hpp"



/***************************************************************************************************
 *
 * FUNCTION NAME: ProcessingImage
 *
 * DESCRIPTION: 
 *                 - If BayerRG8 pixel format
 *                      - Use the demosaicing algorithm to reconstruct a full RGB image.
 *                      - This is done instead of using RGB pixel format 
 *                  - Else, just write image data
 *                  - Store the image as a '.bmp'
 *
 * ACCESS SPECIFIERS:
 *                    + Public
 *
 * ARGUMENT LIST:
 *
 *    Argument        Type       IO              Description
 * -------------   -----------   --   ---------------------------------
 * file_name       const char*    I   - Camera settings file
 * frame            VmbFrame_t    I   - Frame delivered by the camera
 *
 * RETURN VALUE: -
 *
***************************************************************************************************/
void 
ProcessingImage (const char *file_name, VmbFrame_t frame)
{   
    /*
     *  Create a matrix out of the current image.
     */
    cv::Mat src(frame.height, frame.width, CV_8U, frame.buffer);

    /*
     *  Save the image as a '.bmp' given 
     *  the color pixel format used.
     */
    if (strcmp(file_name, "/usr/etc/settings/camera_settings_color.xml") == 0)
    {
        puts("Processing Color Image");
        
        cv::Mat dst(src.size(), CV_8U);
        cv::demosaicing(src, dst, cv::COLOR_BayerRG2RGB);  /* Demosaicing process - specifically used for converting from Bayer pattern to RGB, BGR or greyScale */
        
        cv::imwrite("/usr/etc/images/tmp/result_color.bmp", dst);  /* RGB image */
    }
    else
    {
        puts("Processing Mono Image");

        cv::imwrite("/usr/etc/images/tmp/result_mono.bmp", src);  /* Mono image */
    }
    puts("Image was written successfully...");
}

