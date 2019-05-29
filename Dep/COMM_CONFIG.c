/*----------------------------------------------------------------------------
 *      Biblioteca:	LPC176x_COMM.lib
 *      Proposito:	Biblioteca do protocolo WYMA
 *----------------------------------------------------------------------------
 *      Arquivo:    COMM_CONFIG.c
 *      Proposito: 	Arquivo de defini��es do usu�rio para o protocolo
 *----------------------------------------------------------------------------
 *      Vers�o: 	5.00 	Prog: Ricardo T.   Data: 2/3/2016
 * 		Descri��o:  
 *----------------------------------------------------------------------------
 *      Este c�digo pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#include "COMM_CONFIG.h"

/*----------------------------------------------------------------------------
 *      User configuration part
 *---------------------------------------------------------------------------*/

//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
//
// <h>Defini��es do protocolo
// ==========================
// <i> Defini��es globais do protocolo Wyma
//   <o>N�mero da classe do dispositivo <1-255>
//   <i> Classe para Identifica��o do dispositivo na rede.
//   <i> Default: 1
#define CFG_CLASS_COMM   	3
//   <o>Buffer do protocolo WYMA <1-1024>
//   <i> Tamanho do buffer para gravar o frame.
//   <i> Default: 64
#define CFG_LENBUF_COMM   	300
// </h>
//
// <h>Tamanho do buffer WIFI
// ==========================
// <i> Defini��es globais do driver Wifi
//   <o>Buffer de recebimento <1-2048>
//   <i> Tamanho do buffer para recebimento wifi.
//   <i> Default: 2048
#define CFG_LENBUF_RECWIFI	2048
//   <o>Buffer de transmiss�o <1-1024>
//   <i> Tamanho do buffer de transmiss�o wifi.
//   <i> Default: 300
#define CFG_LENBUF_SENDWIFI	300
// </h>

//------------- <<< end of configuration section >>> -----------------------

/* Global variables -------------------------------------------------------- */

unsigned char bufComm [CFG_LENBUF_COMM];
unsigned char rx_wifi [CFG_LENBUF_RECWIFI];
unsigned char tx_wifi [CFG_LENBUF_SENDWIFI];
unsigned int COMM_userOptions =(CFG_CLASS_COMM<<0)|
								(CFG_LENBUF_COMM<<8)|
								(CFG_LENBUF_SENDWIFI<<18);

/* Bit Configuration for userOptions -----------------------------------------
					Bit0-Bit7  	= Classe do dispositivo
					Bit8-Bit17 	= Tamanho do buffer Comm (10 bits m�x 1024)
					Bit18-Bit27 = Tamanho do Buffer wifi Send (10 bits m�x 1024)
 -----------------------------------------------------------------------------*/

 /*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
