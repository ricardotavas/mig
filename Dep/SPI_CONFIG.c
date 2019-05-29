/*----------------------------------------------------------------------------
 *      Biblioteca:	LPC176x_SPI.lib
 *      Proposito:	Biblioteca com fun��es SPI para todos os dispositivos
 *----------------------------------------------------------------------------
 *      Arquivo:    SPI_CONFIG.c
 *      Proposito: 	Arquivo de defini��es do usu�rio
 *----------------------------------------------------------------------------
 *      Vers�o: 	2.00 	Prog: Ricardo T.   Data: 16/10/2014
 * 		Descri��o:  Vers�o inicial
 *----------------------------------------------------------------------------
 *      Este c�digo pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

 #include				"SPI_CONFIG.h"

/*----------------------------------------------------------------------------
 *      User configuration part
 *---------------------------------------------------------------------------*/

//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------
//
// <e>SSP0
// ========
// <i> Habilita suporte para interface SSP0
#define CFG_USE_SSP0		1
//  <e>Mem�ria EEPROM
//  <i> Habilita suporte para mem�ria EEPROM SPI.
#define CFG_USE_EEPROM		1
// 	</e>
//  <e>Mem�ria FLASH
//  <i> Habilita suporte para mem�ria FLASH SPI.
#define CFG_USE_FLASH  		1
// 	</e>
//  <e>Mem�ria SRAM
//  <i> Habilita suporte para mem�ria SRAM SPI.
#define CFG_USE_SRAM   		0
// 	</e>
//  <e>LCD
//  <i> Habilita suporte para LCD SPI.
#define CFG_USE_LCD   		1
// 	</e>
//  <e>INTERFACE RF
//  <i> Habilita suporte para placa de RF.
#define CFG_USE_RF 			1
// 	</e>
// </e>

// <e>SSP1
// ========
// <i> Habilita suporte para interface SSP1
#define CFG_USE_SSP1		1

//  <e>Mem�ria EEPROM FS
//  <i> Habilita suporte para mem�ria EEPROM SPI com sistema de arquivos.
#define CFG_USE_FLASHFS    	1
// 	</e>
//  <e>SDCARD
//  <i> Habilita suporte para SDCARD.
#define CFG_USE_SDCARD 		1
// 	</e>
// </e>

// <e>SPI
// ========
// <i> Habilita suporte para interface SPI
#define CFG_USE_SPI			0

//  <e>DISPLAY LED
//  <i> Habilita suporte para interface com driver de LED SPI.
#define CFG_USE_LED	     	0

//	<e>SELECIONAR BITS SPI
//	<i> Permite selecionar quantos bits ser�o enviados ou recebidos no controlador SPI.
#define	CFG_USE_SELBITS		0

//	<o>N�mero de bits:  <8=> 8 bits <11=> 11 bits <13=> 13 bits <16=> 16 bits
//	<i> Estas s�o as op��es de n�meros de bits que podem ser controlados pelo SPI.
#define CFG_USE_NUMBITS		8

//	</e>
// 	</e>
// 	</e>

//------------- <<< end of configuration section >>> -----------------------

/* Global variables -------------------------------------------------------- */

unsigned int SPI_userOptions =	(CFG_USE_SSP0<<0)|
								(CFG_USE_EEPROM<<1)|(CFG_USE_FLASH<<2)|(CFG_USE_SRAM<<3)|
								(CFG_USE_LCD<<4)|(CFG_USE_RF<<5)|
								(CFG_USE_SSP1<<8)| 
								(CFG_USE_FLASHFS<<9) | (CFG_USE_SDCARD<<10)|
								(CFG_USE_SPI<<13)|
								(CFG_USE_LED<<14)|
								(CFG_USE_SELBITS<<15)|
								(CFG_USE_NUMBITS<<16);

/* Bit Configuration --------------------------------------------------------
					Bit0=Ativa SSP0
					Bit1=EEPROM
					Bit2=FLASH
					Bit3=SRAM
					Bit4=LCD
					Bit5=POTDIG
					Bit6=
					Bit7=
					Bit8=Ativa SSP1
					Bit9=FLASHFS
					Bit10=SDCARD
					Bit11=
					Bit12=
					Bit13=Ativa SPI
					Bit14=LED
					Bit15=Ativar sele��o de n�mero de bits no SPI
					Bit16 a bit20 = N�mero de bits controlados pelo SPI
 -----------------------------------------------------------------------------*/

 /*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
