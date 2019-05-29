/*----------------------------------------------------------------------------
 *      Biblioteca:	LPC176x_UART.lib
 *      Proposito:	Biblioteca com funções para comunicação através das UARTs 
 *----------------------------------------------------------------------------
 *      Arquivo:    UART_CONFIG.h
 *      Proposito: 	Arquivo de definições do usuário para UART
 *----------------------------------------------------------------------------
 *      Versão: 	1.00 	Prog: Ricardo T.   Data: 25/04/2013
 * 		Descrição:  Versão inicial
 *----------------------------------------------------------------------------
 *      Este código pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#ifndef		__UART_CONFIG__
#define		__UART_CONFIG__

/*----------------------------------------------------------------------------
 *      Mask for user
 *---------------------------------------------------------------------------*/

#define 	MASKUART_USE	0x00000001
#define 	MASKUART_LENTX	0x0000FFFE
#define 	MASKUART_LENRX	0xFFFF0000

#define 	USE_UART0		(UART0_userOptions & MASKUART_LENTX)
#define 	LENTX_UART0 	((UART0_userOptions & MASKUART_LENTX)>>1)
#define 	LENRX_UART0		((UART0_userOptions & MASKUART_LENRX)>>16)

#define 	USE_UART1		(UART1_userOptions & MASKUART_LENTX)
#define 	LENTX_UART1 	((UART1_userOptions & MASKUART_LENTX)>>1)
#define 	LENRX_UART1		((UART1_userOptions & MASKUART_LENRX)>>16)

/*----------------------------------------------------------------------------
 *      Definitions
 *---------------------------------------------------------------------------*/

#define 	UART0			0
#define 	UART1			1

/*----------------------------------------------------------------------------
 *      Exported variables
 *---------------------------------------------------------------------------*/

extern unsigned int UART0_userOptions;
extern unsigned int UART1_userOptions;

extern unsigned char bufTxUart0 []; /* Work FIFO buffers for UARTs 0 and 1 (RX-TX)*/
extern unsigned char bufRxUart0 [];
extern unsigned char bufTxUart1 [];
extern unsigned char bufRxUart1 [];

/*----------------------------------------------------------------------------
 *      Exported funcitons
 *---------------------------------------------------------------------------*/

extern unsigned char init_uart (unsigned char uartPort, unsigned int baudrate, unsigned char dataLen);
extern unsigned char CheckToSend (unsigned char uartPort);
extern unsigned char UART_PutString (unsigned char uartPort, unsigned char *bytes);
extern unsigned char UART_PutChar (unsigned char uartPort, unsigned char byte);
extern int UART_GetChar (unsigned char uartPort);

#endif

 /*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
