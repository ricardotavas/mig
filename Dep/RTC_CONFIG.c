/*----------------------------------------------------------------------------
 *      Biblioteca:	LPC176x_RTC.lib
 *      Proposito:	Biblioteca com fun��es RTC
 *----------------------------------------------------------------------------
 *      Arquivo:    RTC_CONFIG.c
 *      Proposito: 	Arquivo de defini��es do usu�rio
 *----------------------------------------------------------------------------
 *      Vers�o: 	1.00 	Prog: Ricardo T.   Data: 25/04/2013
 * 		Descri��o:  Vers�o inicial
 *----------------------------------------------------------------------------
 *      Este c�digo pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

 #include				"RTC_CONFIG.h"

/*----------------------------------------------------------------------------
 *      User configuration part
 *---------------------------------------------------------------------------*/

//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------
//
// <e>Calibragem
// ==============
// <i> Habilita calibragem do RTC
#define CFG_USE_CAL			0
// </e>

//------------- <<< end of configuration section >>> -----------------------

/* Global variables -------------------------------------------------------- */

unsigned char RTC_userOptions =(CFG_USE_CAL<<0);

/* Bit Configuration --------------------------------------------------------
					Bit0=Ativa Calibragem

 -----------------------------------------------------------------------------*/

 /*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
