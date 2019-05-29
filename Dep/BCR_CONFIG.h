/*----------------------------------------------------------------------------
 *      Biblioteca:	LPC176x_BCR.lib
 *      Proposito:	Biblioteca com funções para leitor de código de barras
 *----------------------------------------------------------------------------
 *      Arquivo:    BCR_CONFIG.h
 *      Proposito: 	Arquivo de definições do usuário para leitor
 *----------------------------------------------------------------------------
 *      Versão: 	1.00 	Prog: Ricardo T.   Data: 22/11/2017
 * 		Descrição:  Versão inicial
 *----------------------------------------------------------------------------
 *      Este código pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#ifndef		__BCR_CONFIG__
#define		__BCR_CONFIG__

/*----------------------------------------------------------------------------
 *      Mask for user defitions
 *---------------------------------------------------------------------------*/

#define 	MASKBCR_MOD		0x01

#define 	USE_IDTECH		(BCR_userOptions & MASKBCR_MOD)

/* Exported variables ----------------------------------------------------- */

extern unsigned char BCR_userOptions;

/*----------------------------------------------------------------------------
 *      Exported functions
 *---------------------------------------------------------------------------*/

extern void bcr_init (void);
extern void bcr_reader_on (void);
extern void bcr_reader_off (void);
extern char bcr_status (void);
extern char bcr_uartSend (unsigned char byte);
extern int bcr_uartRead (void);

#endif

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
