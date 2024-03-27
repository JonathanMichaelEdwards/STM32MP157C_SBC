/***************************************************************************************************
 * 
 * FILE: load_settings.h
 * DATE: 01/08/2022
 * AUTHORS:  Jonathan Edwards, 
 *              Copyright (C) 2014 - 2016 Allied Vision Technologies.  All Rights Reserved.
 *              Redistribution of this file, in original or modified form, without
 *              prior written consent of Allied Vision Technologies is prohibited.
 * 
 * DESCRIPTION:
 *              The capture_process_image example will grab a single image
 *              synchronously and save it to a file using VimbaC.
 * 
***************************************************************************************************/


#ifndef LOAD_SETTINGS_H
#define LOAD_SETTINGS_H

#ifdef __cplusplus
extern "C" {
#endif


/* 
 * Loads camera settings.
 */
VmbError_t 
LoadSettings(const char *file_name);


#ifdef __cplusplus
}
#endif

#endif  /* LOAD_SETTINGS_H */

