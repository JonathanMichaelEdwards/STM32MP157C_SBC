/***************************************************************************************************
 * 
 * FILE: print_vimba_version.c
 * DATE: 01/08/2022
 * AUTHORS:  Jonathan Edwards, 
 *              Copyright (C) 2014 - 2016 Allied Vision Technologies.  All Rights Reserved.
 *              Redistribution of this file, in original or modified form, without
 *              prior written consent of Allied Vision Technologies is prohibited.
 * 
 * DESCRIPTION:
 *              Print Vimba Version.
 * 
***************************************************************************************************/


// C standard libraries
#include <stdio.h>

// Vimba libraries
#include <vimbaC.h>

// Application libraries
#include "print_vimba_version.h"



/***************************************************************************************************
 *
 * FUNCTION NAME: PrintVimbaVersion
 *
 * DESCRIPTION: 
 *               Prints out the version of the Vimba API.
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
 * RETURN VALUE: void
 *
***************************************************************************************************/
void 
PrintVimbaVersion(void)
{
    VmbVersionInfo_t    version_info;
    VmbError_t          result = VmbVersionQuery(&version_info, sizeof(version_info));

    if( VmbErrorSuccess == result)
        printf("Vimba Version Major: %u Minor: %u Patch: %u\n", version_info.major, version_info.minor, version_info.patch);
    else
        printf("VmbVersionQuery failed with Reason: %x\n", result);
}

