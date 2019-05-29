/*--------------------------------------------------------------------------------------------
 *      Arquivo:    TIMER1.c
 *      Proposito: 	TIMER1 base de tempo para firmware
 *--------------------------------------------------------------------------------------------
 *      Versão: 	1.00 	Prog: David D.   Data: 24/06/2013
 * 		Descrição:  Versão inicial
 *--------------------------------------------------------------------------------------------
 *      Este código pertence a Wyma Tecnologia e Sistemas LTDA.
 *------------------------------------------------------------------------------------------*/

#include 		<LPC17xx.H>        		/* LPC17xx definições     			*/

/* Global Variables ------------------------------------------------------ */

unsigned char TIM1_flag_delay = 0;				/* Flag que indica fim de contagem de delay, uso interno */
unsigned char TIM1_flag_overflow=0;				/* Flag que indica que indica fim de contagem overflow uso interno e externo */


/*-------------------------------------------------------------------------------------------
 * Função: 		TIMER1_init
 * Descrição: 	Inicializa TIMER1
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
-------------------------------------------------------------------------------------------*/

void TIMER1_init (void) 
{
	/* Initialize and enable the Timer1 Interface		*/
	LPC_SC->PCONP		|= (1<<2);           				/* Enable power to  Timer1 block   	*/	
	LPC_SC->PCLKSEL0	&= ~(3<<14);           				/* PCLKTIM1 = CCLK/4 ( 25MHz)   	*/
											  
	/* Enable and setup timer interrupt, start timer	*/	
	LPC_TIM1->PR	= 2499999;								/* Set prescaler 24999999, TC running 25MHZ / (2499999 + 1) = 10HZ */
	LPC_TIM1->TCR  	= 2;									/* Reset timer */
	LPC_TIM1->MCR  	= 0x09;       	   	               		/* Interrupt on MR0 and MR1 */
	LPC_TIM1->MR0	= 0;									/* Time interrupt on MR0 */
	LPC_TIM1->MR1	= 0;									/* Time interrupt on MR1 */	
	NVIC_EnableIRQ(TIMER1_IRQn);							/* Enable vector timer 3 */
	LPC_TIM1->IR	= 3;									/* Clear timer MR0 and MR1 flag */
}

/*-------------------------------------------------------------------------------------------
 * Função: 		TIMER3_IRQHandler
 * Descrição: 	Vetor de interrupção do Timer 1
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
-------------------------------------------------------------------------------------------*/

void TIMER1_IRQHandler (void) 
{   
	if ((LPC_TIM1->IR & 1)==1)								/* Interrupção em MR0 é delay */
	{	
		LPC_TIM1->TCR   = 2;								/* Reseta e para o timer */	
		TIM1_flag_delay = 1;								/* Seta flag do delay */
		LPC_TIM1->IR    = 1;								/* Limpa flag de interupção do MR0 */
	}
	if ((LPC_TIM1->IR & 2)==2)								/* Interrupção em MR1 é overflow */
	{
		LPC_TIM1->TCR   = 2;								/* Reseta e para o timer */
		TIM1_flag_overflow = 1;								/* Seta flag do overflow */
		LPC_TIM1->IR    = 2;								/* Limpa flag de interupção do MR1 */		
	}
}

/*-------------------------------------------------------------------------------------------
 * Função: 		TIM1_delay
 * Descrição: 	Delay em mili segundos 
 * Parâmetros: 	time_ms = valor de tempo em mili segundos
 * Retorno: 	Nenhum
-------------------------------------------------------------------------------------------*/

void TIM1_delay (unsigned long time_ms) 
{      
	LPC_TIM1->MR0	= time_ms;									/* Time interrupt on MR0 */
	TIM1_flag_delay = 0;										/* Zera a flag do delay */
  	LPC_TIM1->TCR   = 2;										/* Reseta e para o timer */	
  	LPC_TIM1->TCR   = 1;										/* Liga o timer */	
	while (TIM1_flag_delay==0);									/* Espera a contagem do tempo */
	TIM1_flag_delay = 0;										/* Zera a flag que foi setada na interrupção */
}

/*-------------------------------------------------------------------------------------------
 * Função: 		TIM1_overflow
 * Descrição: 	Delay em mili segundos utilizando Timer1 como base com sinalização de flag
 * Parâmetros: 	time_ms = valor de tempo em centésimos de mili segundos
 * Retorno: 	Nenhum
-------------------------------------------------------------------------------------------*/

void TIM1_overflow (unsigned long time_ms) 
{      
	LPC_TIM1->MR0 	= 0;									/* Zera o registro do delay */
	LPC_TIM1->MR1 	= time_ms;								/* Recebe o tempo de contagem */
	TIM1_flag_overflow =0;									/* Zera a flag de overflow */
	LPC_TIM1->TCR   = 2;									/* Reseta e para o timer */		
	LPC_TIM1->TCR   = 1;									/* Liga o timer */
}

/*-------------------------------------------------------------------------------------------
 * Função: 		TIM1_overflow
 * Descrição: 	Desabilita Delay em mili segundos utilizando Timer1 como base com sinalização de flag
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
-------------------------------------------------------------------------------------------*/

void TIM1_disable_overflow (void) 
{      
	LPC_TIM1->TCR   = 2;								/* Reseta e para o timer */			
	LPC_TIM1->MR1 	= 0;								/* Zera o registro do overflow */
	TIM1_flag_overflow =1;								/* Seta a flag para sair do loop da aplicação */
}
/*----------------------------------------------------------------------------
 * Final do arquivo
 *---------------------------------------------------------------------------*/
