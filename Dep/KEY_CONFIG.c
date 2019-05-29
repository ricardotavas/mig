/*----------------------------------------------------------------------------
 *      Biblioteca:	LPC176x_KEY.lib
 *      Proposito:	Biblioteca com fun��es para leitura de teclado
 *----------------------------------------------------------------------------
 *      Arquivo:    KEY_CONFIG.c
 *      Proposito: 	Arquivo de defini��es do usu�rio
 *----------------------------------------------------------------------------
 *      Vers�o: 	1.00 	Prog: Ricardo T.   Data: 25/04/2013
 * 		Descri��o:  Vers�o inicial
 *----------------------------------------------------------------------------
 *      Este c�digo pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

 #include				"KEY_CONFIG.h"

/*----------------------------------------------------------------------------
 *      User configuration part
 *---------------------------------------------------------------------------*/

//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
//
// <h>Driver de teclado
// =======================
//   <o>Defini��o do produto <1=> CPU LED <2=> CPU MIG <3=> CPU TOPL <4=> TOPM <5=> CPU TOPP <6=> CPU TEC12 <7=> CPU TEC16 <8=> MCIE
//   <i> Define o teclado de acordo com o produto.
//   <i> Default: 1
// </h>
#define DEF_KEY		1

//------------- <<< end of configuration section >>> -----------------------

/* Global variables -------------------------------------------------------- */

unsigned char KEY_userOptions = (DEF_KEY<<0);

/* Bit Configuration --------------------------------------------------------
					Bit0-Bit7=Tipo de produto
 -----------------------------------------------------------------------------*/

 /*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
