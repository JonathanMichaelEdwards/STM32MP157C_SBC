/***************************************************************************************************
 * 
 * FILE: capture_process_image.hpp
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


#ifndef CAPTURE_PROCESS_IMAGE_H_
#define CAPTURE_PROCESS_IMAGE_H_


/*
 * Captures, processes, and stores a image.
 */
VmbError_t 
CaptureProcessImage(const char *file_name);


#endif  /* CAPTURE_PROCESS_IMAGE_H_ */

