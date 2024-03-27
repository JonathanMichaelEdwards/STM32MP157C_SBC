/***************************************************************************************************
 * 
 * FILE: crystal_detection.cpp
 * DATE: 24/11/2022
 * AUTHORS:  Jonathan Edwards
 * 
 * DESCRIPTION:
 *             We desire to be able to detect the light/white region.
 *                  - This will indicate where the crystals are located on the image.
 *             The demo code below uses the Canny edge detection algorithm to determine the edges
 *              that outline the light and dark regions of the mono image.
 *                  - The protein crystals are fluorescent under UV light in a dark environment.
 *                  - The mono image will be taken with only the UV light source on.
 *             A mapped outline of the light regions of the mono image will be masked on the 
 *              colored image to show the crystals
 *                  - The colored image will be taken with only the White light source on.  
 *                  - This has not been implemented yet but stands as a good foundation for the
 *                      next person incharge of this aera.
 * 
 **************************************************************************************************/


// C++ libraries
#include <opencv2/opencv.hpp>
#include <iostream>




/***************************************************************************************************
 *
 * FUNCTION NAME: CrystalEdgeDetection
 *
 * DESCRIPTION: 
 *                  
 *                              UV light source
 *            
 *             We desire to be able to detect the light/white.
 *            
 *             Use the Canny edge detection algorithm determine edges
 *             that outline the light and dark regions of the image.
 *              - Use the Mono8 pixel format instead of BayerRG8.
 *                - Reduces noise
 *                - Using BayerRG8 may result in lose of image quiality.
 *                - Do not use cvtColor() with BayerRG8 to convert to grey scale,
 *                  this looses image quality. 
 *              - Adjust the threshold values based on the intensity of light.
 *              - Aperture size 5 or 7 should be used
 *                  - Light regions of the image are displayed as black pixels.
 *                  - Dark and light regions of the image are displayed as white to noisy pixels.
 *                  - Dark regions of the image are displayed as noisy to white pixels.
 *
 *
 * ACCESS SPECIFIERS:
 *                    - Private
 *
 * ARGUMENT LIST:
 *
 *    Argument        Type    IO              Description
 * -------------   --------   --   ---------------------------------
 *  void            -          -   -
 * 
 * RETURN VALUE: int - 0 for Success otherwise an Error has occured
 *
 **************************************************************************************************/
int 
CrystalEdgeDetection (void)
{
    int error = 0;

    cv::Mat src_mono  = cv::imread("/usr/etc/images/tmp/result_mono.bmp", CV_8U);

    if (src_mono.data == NULL)
    {
        printf("Error %i from imread: %s\n", errno, strerror(errno));

        return errno;
    }

    // 5496 - width max (cols)
    // 3972 - height max (rows)
    int cols = src_mono.cols; 
    int rows = src_mono.rows;

    int cols_h = (cols / 2);
    int rows_h = (rows / 2);

    // Split
    // Copy images into the correct position - quarters
    // - Because of RAM usage using the canny edge detection alg `Canny()'
    // Rect(x, y, width, height)
    //  ----------
    // | q1 | q2 |
    // | --- --- |
    // | q3 | q4 |
    // ----------
    cv::Mat src_mono_q1 = src_mono(cv::Rect(0, 0, cols_h, rows_h));
    cv::Mat src_mono_q2 = src_mono(cv::Rect(cols_h, 0, cols_h, rows_h));
    cv::Mat src_mono_q3 = src_mono(cv::Rect(0, rows_h, cols_h, rows_h));
    cv::Mat src_mono_q4 = src_mono(cv::Rect(cols_h, rows_h, cols_h, rows_h));

    // both the height and width have been halved - reducing mem usage
    cv::Mat result_mono_q1(rows_h, cols_h, CV_8U);
    cv::Mat result_mono_q2(rows_h, cols_h, CV_8U);
    cv::Mat result_mono_q3(rows_h, cols_h, CV_8U);
    cv::Mat result_mono_q4(rows_h, cols_h, CV_8U);
    
    /*
     * Outline the lightest regions on the image.
     *  - This alg uses alot of memory (i.e., reason for splitting in quarters).
     */
    cv::Canny(src_mono_q1, result_mono_q1, (double)10, (double)20, 7);
    cv::Canny(src_mono_q2, result_mono_q2, (double)10, (double)20, 7);
    cv::Canny(src_mono_q3, result_mono_q3, (double)10, (double)20, 7);
    cv::Canny(src_mono_q4, result_mono_q4, (double)10, (double)20, 7);

    // Stich the two images together ----------------------
    cv::Mat result_mono(rows, cols, CV_8U);

    // Copy images in correct position
    result_mono_q1.copyTo(result_mono(cv::Rect(0, 0, cols_h, rows_h)));
    result_mono_q2.copyTo(result_mono(cv::Rect(cols_h, 0, cols_h, rows_h)));
    result_mono_q3.copyTo(result_mono(cv::Rect(0, rows_h, cols_h, rows_h)));
    result_mono_q4.copyTo(result_mono(cv::Rect(cols_h, rows_h, cols_h, rows_h)));

    // write image
    cv::imwrite("/usr/etc/images/tmp/result_edge.bmp", result_mono);


    return error;
}


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
    int error = 0;
    
    // 1. Detect and outline mask the Crystals on the Mono Image.
    //   - Which uses the UV light source
    error = CrystalEdgeDetection();
    // 2. Copy the outline mask to the Colour Image.
    //   - Which uses the White light source
    // MaskColourImage();  -- not implemented


    return error;
}

