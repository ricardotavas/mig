/*----------------------------------------------------------------------------
 *      Biblioteca:	LPC176x_IMP.lib
 *      Proposito:	Biblioteca com fun��es para impress�o
 *----------------------------------------------------------------------------
 *      Arquivo:    IMP_CONFIG.c
 *      Proposito: 	Arquivo de defini��es do usu�rio para impressoras
 *----------------------------------------------------------------------------
 *      Vers�o: 	1.00 	Prog: Ricardo T.   Data: 25/04/2013
 * 		Descri��o:  Vers�o inicial
 *----------------------------------------------------------------------------
 *      Este c�digo pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#include "IMP_CONFIG.h"

/*----------------------------------------------------------------------------
 *      User configuration part
 *---------------------------------------------------------------------------*/

//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
//
// <h>Driver de impressora
// =======================
//   <o>Defini��o da impressora <1=> EngWorks  <2=> Diebold  <2=> Bematech
//   <i> Define o driver de acordo com a impressora.
//   <i> Default: 1
// </h>
#define DEF_IMP		1

//------------- <<< end of configuration section >>> -----------------------

/* Global variables -------------------------------------------------------- */

unsigned char IMP_userOptions = (DEF_IMP<<1);

/* Bit Configuration --------------------------------------------------------
					Bit0=Habilita LCD Gr�fico
 -----------------------------------------------------------------------------*/

 /*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
