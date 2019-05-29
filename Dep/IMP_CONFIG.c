/*----------------------------------------------------------------------------
 *      Biblioteca:	LPC176x_IMP.lib
 *      Proposito:	Biblioteca com funções para impressão
 *----------------------------------------------------------------------------
 *      Arquivo:    IMP_CONFIG.c
 *      Proposito: 	Arquivo de definições do usuário para impressoras
 *----------------------------------------------------------------------------
 *      Versão: 	1.00 	Prog: Ricardo T.   Data: 25/04/2013
 * 		Descrição:  Versão inicial
 *----------------------------------------------------------------------------
 *      Este código pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#include "IMP_CONFIG.h"

/*----------------------------------------------------------------------------
 *      User configuration part
 *---------------------------------------------------------------------------*/

//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
//
// <h>Driver de impressora
// =======================
//   <o>Definição da impressora <1=> EngWorks  <2=> Diebold  <2=> Bematech
//   <i> Define o driver de acordo com a impressora.
//   <i> Default: 1
// </h>
#define DEF_IMP		1

//------------- <<< end of configuration section >>> -----------------------

/* Global variables -------------------------------------------------------- */

unsigned char IMP_userOptions = (DEF_IMP<<1);

/* Bit Configuration --------------------------------------------------------
					Bit0=Habilita LCD Gráfico
 -----------------------------------------------------------------------------*/

 /*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
