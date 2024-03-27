/***************************************************************************************************
 * 
 * FILE: capture_process_image.cpp
 * DATE: 01/08/2022
 * AUTHORS:  Jonathan Edwards, 
 *              Copyright (C) 2014 - 2016 Allied Vision Technologies.  All Rights Reserved.
 *              Redistribution of this file, in original or modified form, without
 *              prior written consent of Allied Vision Technologies is prohibited.
 * 
 * DESCRIPTION:
 *              Grab a single image synchronously, process and store as a '.bmp'
 * 
***************************************************************************************************/


// C++ standard libraries
#include <cstdio>
#include <cstdlib>
#include <cstring>

// Vimba libraries
#include <vimbaC.h>
#include <vmb_common_types.h>

// Application libraries
#include "capture_process_image.hpp"
#include "process_image.hpp"
#include "print_vimba_version.h"



/***************************************************************************************************
 *
 * FUNCTION NAME: CaptureProcessImage
 *
 * DESCRIPTION: 
 *              Starts Vimba API
 *              Opens the given camera, if no camera ID was provided, the first camera found will be used
 *              Sets the maximum possible Ethernet packet size
 *              Adjusts the image format
 *              Acquires one image synchronously
 *              Writes the image as bitmap to file
 *              Closes the camera in case of failure
 * 
 * ACCESS SPECIFIERS:
 *                    + Public
 *
 * ARGUMENT LIST:
 *
 *    Argument        Type         IO              Description
 * -------------   -------------   --   ---------------------------------
 *  file_name      const char *     I   - Camera settings file
 * 
 * RETURN VALUE: VmbError_t - 0 for Success otherwise an Error has occured
 *
 **************************************************************************************************/
VmbError_t
CaptureProcessImage(const char *file_name)
{
    const char          *pCameraID          = NULL;             // ID of Camera
    VmbError_t          error               = VmbStartup();     // Initialize the Vimba API
    VmbCameraInfo_t     *pCameras           = NULL;             // A list of camera details
    VmbUint32_t         nCount              = 0;                // Number of found cameras
    VmbUint32_t         nFoundCount         = 0;                // Change of found cameras
    const VmbUint32_t   nTimeout            = 20000;            // Timeout for Grab
    VmbAccessMode_t     cameraAccessMode    = VmbAccessModeFull;// We open the camera with full access
    VmbHandle_t         cameraHandle        = NULL;             // A handle to our camera
    VmbFrame_t          frame;                                  // The frame we capture
    const char*         pPixelFormat        = NULL;             // The pixel format we use for acquisition
    VmbInt64_t          nPayloadSize        = 0;                // The size of one frame
    VmbErrorType        img_process_state   = VmbErrorImgStore; // Image processing and storing state
    
    
    puts("\n");
    printf( "///////////////////////////////////////////////////\n" );
    printf( "/// Vimba API Capture, Process, and Store Image ///\n" );
    printf( "///////////////////////////////////////////////////\n\n" );


    PrintVimbaVersion();

    /***************************************************************************************************
     *                                        Camera Setup
     ***************************************************************************************************/
    if (error == VmbErrorSuccess)
    {       
        /* 
         * If no camera ID was provided use the first camera found.
         */
        if (pCameraID == NULL)
        {
            /* 
             * Get the amount of known cameras.
             */
            error = VmbCamerasList(NULL, 0, &nCount, sizeof *pCameras);
            if ((error == VmbErrorSuccess) && (0 < nCount))
            {
                pCameras = (VmbCameraInfo_t*)malloc(sizeof(*pCameras)*nCount);
                if ( NULL != pCameras )
                {
                    /*
                     * Actually query all static details of all known cameras without having to open the cameras.
                     * If a new camera was connected since we queried the amount of cameras (nFoundCount > nCount)
                     * we can ignore that one.
                     */
                    error = VmbCamerasList(pCameras, nCount, &nFoundCount, sizeof(*pCameras));
                    if ((error != VmbErrorSuccess) && (error != VmbErrorMoreData))
                        printf("Could not list cameras. Error code: %d\n", error);
                    else
                    {
                        // Use the first camera
                        if(nFoundCount != 0)
                            pCameraID = pCameras[0].cameraIdString;
                        else
                        {
                            pCameraID = NULL;
                            error = VmbErrorNotFound;
                            printf("Camera lost.\n");
                        }
                    }

                    free(pCameras);
                    pCameras = NULL;
                }
                else
                    printf("Could not allocate camera list.\n");
            }
            else
                printf("Could not list cameras or no cameras present. Error code: %d\n", error);
        }

        /***************************************************************************************************
         *                                        Capture Image
         ***************************************************************************************************/
        if (NULL != pCameraID)
        {
            /*
             * Open camera.
             */
            error = VmbCameraOpen(pCameraID, cameraAccessMode, &cameraHandle);
            if (error == VmbErrorSuccess)
            {
                printf("Camera ID: %s\n\n", pCameraID);

                if (error == VmbErrorSuccess)
                {
                    /*
                     *  Choose pixel format.
                     */
                    if (strcmp(file_name, "/usr/etc/settings/camera_settings_color.xml") == 0)
                        error = VmbFeatureEnumSet(cameraHandle, "PixelFormat", "BayerRG8");
                    else
                        error = VmbFeatureEnumSet(cameraHandle, "PixelFormat", "Mono8");
                    
                    /* 
                     * Read back pixel format.
                     */
                    VmbFeatureEnumGet(cameraHandle, "PixelFormat", &pPixelFormat);
                    printf("Pixel format %s\n", pPixelFormat);

                    if (error == VmbErrorSuccess)
                    {
                        /* 
                         * Evaluate frame size.
                         */
                        error = VmbFeatureIntGet(cameraHandle, "PayloadSize", &nPayloadSize);
                        if (error == VmbErrorSuccess)
                        {       
                            frame.bufferSize    = (VmbUint32_t)nPayloadSize;                     
                            frame.buffer        = (unsigned char*)malloc(frame.bufferSize);

                            // Announce Frame
                            error = VmbFrameAnnounce(cameraHandle, &frame, (VmbUint32_t)sizeof(VmbFrame_t));
                            if (error == VmbErrorSuccess)
                            {
                                // Start Capture Engine
                                error = VmbCaptureStart(cameraHandle);
                                if (error == VmbErrorSuccess)
                                {
                                    // Queue Frame
                                    error = VmbCaptureFrameQueue( cameraHandle, &frame, NULL);
                                    if (error == VmbErrorSuccess)
                                    {
                                        // Start Acquisition
                                        error = VmbFeatureCommandRun(cameraHandle,"AcquisitionStart");
                                        if (error == VmbErrorSuccess)
                                        {
                                            /*
                                             * Capture one frame synchronously.
                                             */
                                            error = VmbCaptureFrameWait(cameraHandle, &frame, nTimeout);
                                            if (error == VmbErrorSuccess)
                                                img_process_state = VmbErrorSuccess;
                                            else
                                                printf("Could not capture frame. Error code: %d\n", error);

                                            // Stop Acquisition
                                            error = VmbFeatureCommandRun(cameraHandle,"AcquisitionStop");
                                            if (error != VmbErrorSuccess)
                                                printf("Could not stop acquisition. Error code: %d\n", error);
                                        }
                                        else
                                            printf("Could not start acquisition. Error code: %d\n", error);
                                    }
                                    else
                                        printf("Could not queue frame. Error code: %d\n", error);

                                    // Stop Capture Engine
                                    error = VmbCaptureEnd( cameraHandle );
                                    if (error != VmbErrorSuccess)
                                        printf( "Could not end capture. Error code: %d\n", error);
                                }
                                else
                                    printf("Could not start capture. Error code: %d\n", error);

                                // Revoke frame
                                error = VmbFrameRevoke( cameraHandle, &frame );
                                if (error != VmbErrorSuccess)
                                    printf("Could not revoke frame. Error code: %d\n", error);
                            }
                            else
                                printf("Could not announce frame. Error code: %d\n", error);
                        }
                    }
                    else
                        printf("Could not set pixel format to either RGB or Mono. Error code: %d\n", error);
                }
                else
                    printf("Could not adjust packet size. Error code: %d\n", error);

                error = VmbCameraClose ( cameraHandle );
                if (error != VmbErrorSuccess)
                    printf("Could not close camera. Error code: %d\n", error);
            }
            else
                printf("Could not open camera. Error code: %d\n", error);
        }
        VmbShutdown();
    }
    else
        printf("Could not start system. Error code: %d\n", error);

    /* 
     * Process and store image.
     */
    if ((error == VmbErrorSuccess) && (img_process_state == VmbErrorSuccess)) 
        ProcessingImage(file_name, frame);

    free(frame.buffer);


    return error;
}

