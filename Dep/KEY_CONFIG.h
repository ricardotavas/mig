/*----------------------------------------------------------------------------
 *      Biblioteca:	LPC176x_KEY.lib
 *      Proposito:	Biblioteca com funções para leitura de teclado
 *----------------------------------------------------------------------------
 *      Arquivo:    KEY_CONFIG.h
 *      Proposito: 	Arquivo de definições do usuário
 *----------------------------------------------------------------------------
 *      Versão: 	1.00 	Prog: Ricardo T.   Data: 25/04/2013
 * 		Descrição:  Versão inicial
 *----------------------------------------------------------------------------
 *      Este código pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#ifndef __KEYCONFIG
#define __KEYCONFIG

/*----------------------------------------------------------------------------
 *      Mask for user defitions
 *---------------------------------------------------------------------------*/

#define 	MASKKEY_PRD		0xFF

#define 	KEY_PRD		(KEY_userOptions & MASKKEY_PRD)

/*----------------------------------------------------------------------------
 *      Key definitions
 *---------------------------------------------------------------------------*/
#define		NONEKEY		0

#define		KEY1		(1<<0)
#define		KEY2		(1<<1)
#define		KEY3		(1<<2)
#define		KEY4		(1<<3)
#define		KEY5		(1<<4)
#define		KEY6		(1<<5)
#define		KEY7		(1<<6)
#define		KEY8		(1<<7)
#define		KEY9		(1<<8)
#define		KEY10		(1<<9)
#define		KEY11		(1<<10)
#define		KEY12		(1<<11)
#define		KEY13		(1<<12)
#define		KEY14		(1<<13)
#define		KEY15		(1<<14)
#define		KEY16		(1<<15)

/*----------------------------------------------------------------------------
 *      Multi-Keys definitions
 *---------------------------------------------------------------------------*/

#define		KEYS1_2		((1<<0)|(1<<1))
#define		KEYS5_6		((1<<4)|(1<<5))
#define		KEYS1_12	((1<<0)|(1<<11))
#define		KEYS4_10	((1<<3)|(1<<9))

/*----------------------------------------------------------------------------
 *      Exported functions
 *---------------------------------------------------------------------------*/

extern void init_key (void);
extern void While_KeyPress (unsigned short keyPress);
extern unsigned short Read_Key (void);

#endif

/*----------------------------------------------------------------------------
 * Fim do arquivo
 *---------------------------------------------------------------------------*/

