/***************************************************************************************************
 * 
 * FILE: load_settings.c
 * DATE: 01/08/2022
 * AUTHORS:  Jonathan Edwards, 
 *              Copyright (C) 2014 - 2016 Allied Vision Technologies.  All Rights Reserved.
 *              Redistribution of this file, in original or modified form, without
 *              prior written consent of Allied Vision Technologies is prohibited.
 * 
 * DESCRIPTION:
 *              Loads a settings '.xml' file into the Camera. 
 * 
***************************************************************************************************/


// C standard libraries
#include <stdio.h>
#include <stdlib.h>

// Vimba libraries
#include <vimbaC.h>
#include <vmb_common_types.h>

// Application libraries
#include "load_settings.h"



/***************************************************************************************************
 *
 * FUNCTION NAME: LoadSettings
 *
 * DESCRIPTION: 
 *              Loads camera settings file.
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
LoadSettings(const char *file_name)
{
    VmbError_t error = VmbErrorSuccess;
    VmbUint32_t nCount = 0;
    VmbCameraInfo_t *pCameras = NULL;
    VmbHandle_t handle = NULL;
    const char *cameraId = NULL;

    /* 
     * Prepare settings struct to determine behaviour during loading or saving operation.
     *  - (This is optional. Passing NULL triggers default settings)
     */
    VmbFeaturePersistSettings_t settings;
    settings.loggingLevel = 4;      //  full loggin details (trace, debug, warning, error and info messages)
    settings.maxIterations = 5;     //  how many iterations during loading shall be performed at max 
                                    //  (comparing desired with device value. in case of difference new iteration will be executed)
    settings.persistType = VmbFeaturePersistNoLUT;      //  determines which features shall be considered
                                                        //  NoLUT means: all features except for lookUp table features


    printf( "\n" );
    printf( "///////////////////////////////\n" );
    printf( "/// Vimba API Load Settings ///\n" );
    printf( "///////////////////////////////\n" );
    printf( "\n" );


    /*
     * Start VimbaC API.
     */
    error = VmbStartup();
    if (error != VmbErrorSuccess)
    {
        printf("Could not start Vimba [error code: %i]\n", error);
        VmbShutdown();

        return error;
    }

    printf("--> VimbaC has been started\n");
    
    /* 
     * Get the amount of connected cameras.
     */
    error = VmbCamerasList(NULL, 0, &nCount, sizeof(*pCameras));
    if (error != VmbErrorSuccess)
    {
        printf("Could not retrieve number of cameras [error code: %i]\n", error);
        VmbShutdown();
        
        return error;
    }

    /* 
     * No camera connected.
     */
    if (nCount == 0)
    {
        printf("No Camera found [error code: %i]\n", VmbErrorNotFound);
        VmbShutdown();
        
        return VmbErrorNotFound;
    }

    /*
     * Allocate space for camera list.
     */
    pCameras = (VmbCameraInfo_t *)calloc(nCount, sizeof(*pCameras));
    if (pCameras == NULL)
    {
        printf("No Camera found [error code: %i]\n", VmbErrorNotFound);
        VmbShutdown();
        free(pCameras);

        return VmbErrorNotFound;
    }
    
    /*
     * Retrieve camera list from Vimba.
     */
    error = VmbCamerasList(pCameras, nCount, &nCount, sizeof(*pCameras));
    if (error != VmbErrorSuccess)
    {
        printf("Could not retrieve info pointers for connected cameras [error code: %i]\n", error);
        VmbShutdown();
        free(pCameras);

        return error;
    }

    /* 
     * Get camera id.
     */
    cameraId = pCameras[0].cameraIdString;
    if (cameraId == NULL)
    {
        printf("Could not retrieve camera id [error code: %i]\n", error);
        VmbShutdown();
        free(pCameras);

        return error;
    }
    
    /* 
     * Open camera (first in list).
     */
    error = VmbCameraOpen(cameraId, VmbAccessModeFull, &handle);
    if (error != VmbErrorSuccess)
    {
        printf("Could not open camera in Full Access mode [error code: %i]\n", error);
        VmbShutdown();
        free(pCameras);

        return error;
    }

    printf("--> Camera with id '%s' has been opened\n", cameraId);

    /*
     * Load feature values to selected camera from xml file.
     */
    error = VmbCameraSettingsLoad(handle, file_name, &settings, sizeof(settings));
    if (error != VmbErrorSuccess)
    {
        printf("Could not load feature values from given XML file '%s' [error code: %i]\n", file_name, error);
        VmbShutdown();
        free(pCameras);

        return error;
    }

    printf("--> Feature values have been loaded from given XML file\n");
    
    /*
     * Close camera.
     */
    error = VmbCameraClose(handle);
    if (error != VmbErrorSuccess)
    {
        printf("Could not close camera [error code: %i]\n", error);
        VmbShutdown();
        free(pCameras);

        return error;
    }

    /* 
     * Shutdown VimbaC API.
     */
    printf("--> Camera has been closed\n");
    printf("--> VimbaC has been stopped\n");
    VmbShutdown();
    free(pCameras);
    

    return VmbErrorSuccess;
}

