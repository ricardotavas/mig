/*----------------------------------------------------------------------------
 *      RL-ARM - FlashFS
 *----------------------------------------------------------------------------
 *      Name:    FS_TIME.C 
 *      Purpose: File System Time Support Functions
 *      Rev.:    V4.11
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2010 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include 		<RTL.h>
#include 		<LPC17xx.h>                    	// LPC17xx definitions

/*----------------------------------------------------------------------------
 *      FS Time Functions
 *----------------------------------------------------------------------------
 *  Required functions for File System Time Support
 *   - U32 fs_get_time ()
 *   - U32 fs_get_date ()
 *---------------------------------------------------------------------------*/

/*--------------------------- fs_get_time -----------------------------------*/

U32 fs_get_time (void) 
{
  /* Return Current Time for FAT File Time stamp. */

  	return (LPC_RTC->CTIME0);
}


/*--------------------------- fs_get_date -----------------------------------*/

U32 fs_get_date (void) 
{
  /* Return Current Date for FAT File Time stamp. */

  	return (LPC_RTC->CTIME1 & 0x00FFFFFF);
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

