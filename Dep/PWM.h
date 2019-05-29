/*----------------------------------------------------------------------------
 *      Arquivo:    PWM.h
 *      Proposito: 	Header do arquivo PWM.c
 *----------------------------------------------------------------------------
 *      Versão: 	1.00 	Prog: David D.   Data: 21/05/2013
 * 		Descrição:  Versão inicial
 *----------------------------------------------------------------------------
 *      Este código pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#ifndef __PWM
#define __PWM

/*----------------------------------------------------------------------------
 *      Exported functions
 *---------------------------------------------------------------------------*/

extern void PWM_init(void);
extern void PWM_Change_Bright (unsigned char value);
extern void PWM_Change_Volume (unsigned char value);
extern void PWM_Beep (unsigned char value);

#endif

/*----------------------------------------------------------------------------
 * Fim do arquivo
 *---------------------------------------------------------------------------*/

