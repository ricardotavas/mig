/*----------------------------------------------------------------------------
 *      Biblioteca:	LPC176x_UART.lib
 *      Proposito:	Biblioteca com funções para comunicação através das UARTs 
 *----------------------------------------------------------------------------
 *      Arquivo:    UART_CONFIG.c
 *      Proposito: 	Arquivo de definições do usuário para UART
 *----------------------------------------------------------------------------
 *      Versão: 	1.00 	Prog: Ricardo T.   Data: 25/04/2013
 * 		Descrição:  Versão inicial
 *----------------------------------------------------------------------------
 *      Este código pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#include "UART_CONFIG.h"
/*----------------------------------------------------------------------------
 *      User configuration part
 *---------------------------------------------------------------------------*/

//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
//
// <e>UART 0
// =========
// <i> Habilita suporte para interface UART0
#define CFG_USE_UART0		1
//   <o>Tamanho do buffer TX da UART0 <1-5000>
//   <i> Tamanho do buffer de envio da UART0
//   <i> Default: 64
#define CFG_LENTX_UART0   	100
//   <o>Tamanho do buffer RX da UART0 <1-5000>
//   <i> Tamanho do buffer de recebimento da UART0
//   <i> Default: 64
#define CFG_LENRX_UART0   	100
// </e>

// <e>UART 1
// =========
// <i> Habilita suporte para interface UART1
#define CFG_USE_UART1		1
//   <o>Tamanho do buffer TX da UART1 <1-5000>
//   <i> Tamanho do buffer de envio da UART1
//   <i> Default: 64
#define CFG_LENTX_UART1   	3000
//   <o>Tamanho do buffer RX da UART1 <1-5000>
//   <i> Tamanho do buffer de recebimento da UART1
//   <i> Default: 64
#define CFG_LENRX_UART1   	10
// </e>

//------------- <<< end of configuration section >>> -----------------------

/* Global variables for UART Buffer ---------------------------------------- */

unsigned char bufTxUart0 [CFG_LENTX_UART0];
unsigned char bufRxUart0 [CFG_LENRX_UART0];
unsigned int UART0_userOptions =(CFG_USE_UART0<<0)|
								(CFG_LENTX_UART0<<1)|
								(CFG_LENRX_UART0<<16);

unsigned char bufTxUart1 [CFG_LENTX_UART1];
unsigned char bufRxUart1 [CFG_LENRX_UART1];
unsigned int UART1_userOptions =(CFG_USE_UART1<<0)|
								(CFG_LENTX_UART1<<1)|
								(CFG_LENRX_UART1<<16);

/* Bit Configuration for userOptions -----------------------------------------
					Bit0       	= Ativa UART
					Bit1-Bit15 	= Tamanho do buffer de envio (TX) da UART
					Bit16-Bit31	= Tamanho do buffer de recebimento (RX) da UART
 -----------------------------------------------------------------------------*/

 /*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
