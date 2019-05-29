/*----------------------------------------------------------------------------
 *      Biblioteca:	LPC176x_LCD.lib
 *      Proposito:	Biblioteca com fun��es para LCD
 *----------------------------------------------------------------------------
 *      Arquivo:    LCD_CONFIG.c
 *      Proposito: 	Arquivo de defini��es do usu�rio para LCD
 *----------------------------------------------------------------------------
 *      Vers�o: 	1.00 	Prog: Ricardo T.   Data: 25/04/2013
 * 		Descri��o:  Vers�o inicial
 *----------------------------------------------------------------------------
 *      Este c�digo pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#include				"LCD_CONFIG.h"

/*----------------------------------------------------------------------------
 *      User configuration part
 *---------------------------------------------------------------------------*/

//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------
//
// <e>LCD Gr�fico
// ===============
// <i> Habilitado fornece suporte para LCD gr�fico
#define CFG_USE_LCDG		1

//   <o>Defini��o do LCD gr�fico <0=> 128x64 
//   <i> Configura a defini��o da tela do LCD Gr�fico.
//   <i> Default: 128x64
#define CFG_DEF_LCDG		0

// </e>
// <e>LCD de Caractere
// ====================
// <i> Habilitado fornece suporte para LCD de caractere
#define CFG_USE_LCDC		0

//   <o>Configura��o do LCD de caractere <0=> 16x2 
//   <i> Define a quantidade de caracteres e linhas do LCD
//   <i> Default: 16x2
#define CFG_DEF_LCDC   		0
						 
//------------- <<< end of configuration section >>> -----------------------

/* Global variables -------------------------------------------------------- */

unsigned char LCD_userOptions = (CFG_USE_LCDG<<0)|
								(CFG_DEF_LCDG<<1)|
								(CFG_USE_LCDC<<4)|
								(CFG_DEF_LCDC<<5);

/* Bit Configuration --------------------------------------------------------
					Bit0=Habilita LCD Gr�fico
					Bit1=Especifica LCD Gr�fico
					Bit2=Especifica LCD Gr�fico
					Bit3=Especifica LCD Gr�fico
					Bit4=Habilita LCD caractere
					Bit5=Especifica LCD caractere
					Bit6=Especifica LCD caractere
 -----------------------------------------------------------------------------*/

 /*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
