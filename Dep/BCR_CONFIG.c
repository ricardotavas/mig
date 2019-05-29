/*----------------------------------------------------------------------------
 *      Biblioteca:	LPC176x_BCR.lib
 *      Proposito:	Biblioteca com funções para leitor de código de barras
 *----------------------------------------------------------------------------
 *      Arquivo:    BCR_CONFIG.c
 *      Proposito: 	Arquivo de definições do usuário para leitor
 *----------------------------------------------------------------------------
 *      Versão: 	1.00 	Prog: Ricardo T.   Data: 22/11/2017
 * 		Descrição:  Versão inicial
 *----------------------------------------------------------------------------
 *      Este código pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#include "BCR_CONFIG.h"

/*----------------------------------------------------------------------------
 *      User configuration part
 *---------------------------------------------------------------------------*/

//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
//
// <h>Driver de impressora
// =======================
//   <o>Definição da impressora <1=> IDTECH  <2=> _____  <2=> ______
//   <i> Define o driver de acordo com o leitor.
//   <i> Default: 1
// </h>
#define DEF_BCR		1

//------------- <<< end of configuration section >>> -----------------------

/* Global variables -------------------------------------------------------- */

unsigned char BCR_userOptions = (DEF_BCR<<0);

/* Bit Configuration --------------------------------------------------------
					Bit0=Leitor IDTECH
 -----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
