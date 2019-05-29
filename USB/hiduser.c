/*----------------------------------------------------------------------------
 * U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 * Name:    hiduser.c
 * Purpose: HID Custom User Module
 * Version: V1.20
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------
 * History:
 *          V1.20 
 *          V1.00 Initial Version
 *----------------------------------------------------------------------------*/

#include "type.h"

#include "usb.h"
#include "hid.h"
#include "usbcfg.h"
#include "usbcore.h"
#include "hiduser.h"

#include "COMM_CONFIG.h"


U8 HID_Protocol;
U8 HID_IdleTime[HID_REPORT_NUM];


/*
 *  HID Get Report Request Callback
 *   Called automatically on HID Get Report Request
 *    Parameters:      None (global SetupPacket and EP0Buf)
 *    Return Value:    TRUE - Success, FALSE - Error
 */

BOOL HID_GetReport (void) 
{
  	/* ReportID = SetupPacket.wValue.WB.L; */
  	switch (SetupPacket.wValue.WB.H) 
  	{
    	case HID_REPORT_INPUT:
			usbtx_callback ();
      		EP0Buf[0] = InReport[0];
			EP0Buf[1] = InReport[1];
			EP0Buf[2] = InReport[2];
			EP0Buf[3] = InReport[3];
			EP0Buf[4] = InReport[4];
			EP0Buf[5] = InReport[5];
			EP0Buf[6] = InReport[6];
			EP0Buf[7] = InReport[7];
			EP0Buf[8] = InReport[8];
			EP0Buf[9] = InReport[9];
			EP0Buf[10] = InReport[10];
			EP0Buf[11] = InReport[11];
			EP0Buf[12] = InReport[12];
			EP0Buf[13] = InReport[13];
			EP0Buf[14] = InReport[14];
			EP0Buf[15] = InReport[15];
			EP0Buf[16] = InReport[16];
			EP0Buf[17] = InReport[17];
			EP0Buf[18] = InReport[18];
			EP0Buf[19] = InReport[19];
			EP0Buf[20] = InReport[20];
			EP0Buf[21] = InReport[21];
			EP0Buf[22] = InReport[22];
			EP0Buf[23] = InReport[23];
			EP0Buf[24] = InReport[24];
			EP0Buf[25] = InReport[25];
			EP0Buf[26] = InReport[26];
			EP0Buf[27] = InReport[27];
			EP0Buf[28] = InReport[28];
			EP0Buf[29] = InReport[29];
			EP0Buf[30] = InReport[30];
			EP0Buf[31] = InReport[31];
			EP0Buf[32] = InReport[32];
			EP0Buf[33] = InReport[33];
			EP0Buf[34] = InReport[34];
			EP0Buf[35] = InReport[35];
			EP0Buf[36] = InReport[36];
			EP0Buf[37] = InReport[37];
			EP0Buf[38] = InReport[38];
			EP0Buf[39] = InReport[39];
			EP0Buf[40] = InReport[40];
			EP0Buf[41] = InReport[41];
			EP0Buf[42] = InReport[42];
			EP0Buf[43] = InReport[43];
			EP0Buf[44] = InReport[44];
			EP0Buf[45] = InReport[45];
			EP0Buf[46] = InReport[46];
			EP0Buf[47] = InReport[47];
			EP0Buf[48] = InReport[48];
			EP0Buf[49] = InReport[49];
			EP0Buf[50] = InReport[50];
			EP0Buf[51] = InReport[51];
			EP0Buf[52] = InReport[52];
			EP0Buf[53] = InReport[53];
			EP0Buf[54] = InReport[54];
			EP0Buf[55] = InReport[55];
			EP0Buf[56] = InReport[56];
			EP0Buf[57] = InReport[57];
			EP0Buf[58] = InReport[58];
			EP0Buf[59] = InReport[59];
			EP0Buf[60] = InReport[60];
			EP0Buf[61] = InReport[61];
			EP0Buf[62] = InReport[62];
			EP0Buf[63] = InReport[63];
      		break;
    	case HID_REPORT_OUTPUT:
      		return (__FALSE);        /* Not Supported */
    	case HID_REPORT_FEATURE:
      		/* EP0Buf[] = ...; */
      		/* break; */
      		return (__FALSE);        /* Not Supported */
  	}
  	return (__TRUE);
}


/*
 *  HID Set Report Request Callback
 *   Called automatically on HID Set Report Request
 *    Parameters:      None (global SetupPacket and EP0Buf)
 *    Return Value:    TRUE - Success, FALSE - Error
 */

BOOL HID_SetReport (void) 
{
  	/* ReportID = SetupPacket.wValue.WB.L; */
  	switch (SetupPacket.wValue.WB.H) 
	{
    	case HID_REPORT_INPUT:
      		return (__FALSE);        /* Not Supported */
    	case HID_REPORT_OUTPUT:
      		OutReport[0] = EP0Buf[0];
			OutReport[1] = EP0Buf[1];
			OutReport[2] = EP0Buf[2];
			OutReport[3] = EP0Buf[3];
			OutReport[4] = EP0Buf[4];
			OutReport[5] = EP0Buf[5];
			OutReport[6] = EP0Buf[6];
			OutReport[7] = EP0Buf[7];
			OutReport[8] = EP0Buf[8];
			OutReport[9] = EP0Buf[9];
			OutReport[10] = EP0Buf[10];
			OutReport[11] = EP0Buf[11];
			OutReport[12] = EP0Buf[12];
			OutReport[13] = EP0Buf[13];
			OutReport[14] = EP0Buf[14];
			OutReport[15] = EP0Buf[15];
			OutReport[16] = EP0Buf[16];
			OutReport[17] = EP0Buf[17];
			OutReport[18] = EP0Buf[18];
			OutReport[19] = EP0Buf[19];
			OutReport[20] = EP0Buf[20];
			OutReport[21] = EP0Buf[21];
			OutReport[22] = EP0Buf[22];
			OutReport[23] = EP0Buf[23];
			OutReport[24] = EP0Buf[24];
			OutReport[25] = EP0Buf[25];
			OutReport[26] = EP0Buf[26];
			OutReport[27] = EP0Buf[27];
			OutReport[28] = EP0Buf[28];
			OutReport[29] = EP0Buf[29];
			OutReport[30] = EP0Buf[30];
			OutReport[31] = EP0Buf[31];
			OutReport[32] = EP0Buf[32];
			OutReport[33] = EP0Buf[33];
			OutReport[34] = EP0Buf[34];
			OutReport[35] = EP0Buf[35];
			OutReport[36] = EP0Buf[36];
			OutReport[37] = EP0Buf[37];
			OutReport[38] = EP0Buf[38];
			OutReport[39] = EP0Buf[39];
			OutReport[40] = EP0Buf[40];
			OutReport[41] = EP0Buf[41];
			OutReport[42] = EP0Buf[42];
			OutReport[43] = EP0Buf[43];
			OutReport[44] = EP0Buf[44];
			OutReport[45] = EP0Buf[45];
			OutReport[46] = EP0Buf[46];
			OutReport[47] = EP0Buf[47];
			OutReport[48] = EP0Buf[48];
			OutReport[49] = EP0Buf[49];
			OutReport[50] = EP0Buf[50];
			OutReport[51] = EP0Buf[51];
			OutReport[52] = EP0Buf[52];
			OutReport[53] = EP0Buf[53];
			OutReport[54] = EP0Buf[54];
			OutReport[55] = EP0Buf[55];
			OutReport[56] = EP0Buf[56];
			OutReport[57] = EP0Buf[57];
			OutReport[58] = EP0Buf[58];
			OutReport[59] = EP0Buf[59];
			OutReport[60] = EP0Buf[60];
			OutReport[61] = EP0Buf[61];
			OutReport[62] = EP0Buf[62];
			OutReport[63] = EP0Buf[63];
      		usbrx_callback();
      		break;
    	case HID_REPORT_FEATURE:
      		return (__FALSE);        /* Not Supported */
  	}
  	return (__TRUE);
}



/*
 *  HID Get Idle Request Callback
 *   Called automatically on HID Get Idle Request
 *    Parameters:      None (global SetupPacket and EP0Buf)
 *    Return Value:    TRUE - Success, FALSE - Error
 */

BOOL HID_GetIdle (void) {

  EP0Buf[0] = HID_IdleTime[SetupPacket.wValue.WB.L];
  return (__TRUE);
}


/*
 *  HID Set Idle Request Callback
 *   Called automatically on HID Set Idle Request
 *    Parameters:      None (global SetupPacket)
 *    Return Value:    TRUE - Success, FALSE - Error
 */

BOOL HID_SetIdle (void) {

  HID_IdleTime[SetupPacket.wValue.WB.L] = SetupPacket.wValue.WB.H;

  /* Idle Handling if needed */
  /* ... */

  return (__TRUE);
}


/*
 *  HID Get Protocol Request Callback
 *   Called automatically on HID Get Protocol Request
 *    Parameters:      None (global SetupPacket)
 *    Return Value:    TRUE - Success, FALSE - Error
 */

BOOL HID_GetProtocol (void) {

  EP0Buf[0] = HID_Protocol;
  return (__TRUE);
}


/*
 *  HID Set Protocol Request Callback
 *   Called automatically on HID Set Protocol Request
 *    Parameters:      None (global SetupPacket)
 *    Return Value:    TRUE - Success, FALSE - Error
 */

BOOL HID_SetProtocol (void) {

  HID_Protocol = SetupPacket.wValue.WB.L;

  /* Protocol Handling if needed */
  /* ... */

  return (__TRUE);
}

