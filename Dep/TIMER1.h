/*----------------------------------------------------------------------------
 *      Arquivo:    TIMER1.h
 *      Proposito: 	Header do arquivo TIMER1.c
 *----------------------------------------------------------------------------
 *      Versão: 	1.00 	Prog: David D.   Data: 24/06/2013
 * 		Descrição:  Versão inicial
 *----------------------------------------------------------------------------
 *      Este código pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#ifndef __SYSTICK
#define __SYSTICK

/* Definitions for application  ------------------------------------------------------- */

#define TIME100ms		1
#define TIME200ms		2
#define TIME300ms		3
#define TIME400ms		4
#define TIME500ms		5
#define TIME600ms		6
#define TIME700ms		7
#define TIME800ms		8
#define TIME900ms		9
#define TIME1s			10
#define TIME2s			20
#define TIME3s			30
#define TIME4s			40
#define TIME5s			50
#define TIME10s			100
#define TIME20s			200
#define TIME30s			300

/* Exported Variables ------------------------------------------------------------- */

extern unsigned char TIM1_flag_overflow;						/* Flag que indica que indica fim de contagem no modo overflow */

/* Exported functions ------------------------------------------------------- */

extern void TIMER1_init (void); 						/* Iniciliza SysTick Timer para gerar interrupção a cada 1ms */ 
extern void TIM1_delay (unsigned long time_ms); 		/* Delay (trava programa para esperar) em mili segundos utilizando SysTick Timer como base */
														/* time_ms = valor de tempo em mili segundos */
extern void TIM1_disable_overflow (void);														
extern void TIM1_overflow (unsigned long time_ms); 		/* Flag overflow é setada quando a contagem de time_ms é alcançada */

/* Example Overflow */
/*

		TIM1_overflow (TIME5s);
		while (flag_overflow==0);
		{
			 Oque deve ficar fazendo, durante 5 segundos
			 
		}	
		flag_overflow=0;
							*/
							
/* Example Delay */
/*
		TIM1_delay (TIME5s);			Programa espera contagem de 5 segundos
							*/							
#endif

/*----------------------------------------------------------------------------
 * Fim do arquivo
 *---------------------------------------------------------------------------*/

