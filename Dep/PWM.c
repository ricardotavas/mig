/*----------------------------------------------------------------------------
 *      Arquivo:    PWM.c
 *      Proposito: 	PWM para Beep, Brilho do display e Volume do Buzzer
 *----------------------------------------------------------------------------
 *      Versão: 	1.00 	Prog: David D.   Data: 21/05/2013
 * 		Descrição:  Versão inicial
 *----------------------------------------------------------------------------
 *      Este código pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#include 		<LPC17xx.H>        		/* LPC17xx definições     			*/

/*----------------------------------------------------------------------------
 *     Global Definitions
 *---------------------------------------------------------------------------*/
 
#define		BEEP		1

/*-------------------------------------------------------------------------------------------
 * Função: 		PWM_init
 * Descrição: 	Inicializa pinos e configura PWM1.1 PWM1.3 e PWM1.6 .  
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
-------------------------------------------------------------------------------------------*/

void PWM_init (void) 
{
	//LPC_PINCON->PINSEL3 &= ~(0x03 << 4);		/* Pin P1.18 reset value I/O							*/
	//LPC_PINCON->PINSEL3 |= (0x02 << 4);  		/* Pin P1.18 used as PWM1.1 for Bright Display			*/
	
	LPC_PINCON->PINSEL3 &= ~(0x03 << 10);		/* Pin P1.21 reset value I/O							*/
	LPC_PINCON->PINSEL3 |= (0x02 << 10);  		/* Pin P1.21 used as PWM1.3 for Sound Beep				*/		
	
	//LPC_PINCON->PINSEL4 &= ~(0x03 << 10);		/* Pin P2.5 reset value I/O								*/
	//LPC_PINCON->PINSEL4 |= (0x01 << 10);  	/* Pin P2.5 used as PWM1.6 for Volume Buzzer			*/			

	LPC_PWM1->PR = 25; 							/* Prescale Register em 25, timer runs at 25 MHz / 25 = 1 MHz*/
	LPC_PWM1->PC = 0; 							/* Prescale Counter em 0*/
	LPC_PWM1->TC = 0; 							/* Reseta Timer Counter em 0*/

	LPC_PWM1->MR0 = 100;						/* Registro do valor de comparação principal: 			*/
												/* Frequência base = 1 MHz / 100 = 10 KHz				*/ 
												/* 10KHZ PARA VOLUME, 4KHZ PARA BEEP */
	
	LPC_PWM1->MR1 = 0; 							/* Registro do valor de comparação do PWM1.1 Bright Display	*/
	LPC_PWM1->MR3 = 0; 							/* Registro do valor de comparação do PWM1.3 Sound Beep	 	*/	
	LPC_PWM1->MR6 = 0; 							/* Registro do valor de comparação do PWM1.6 Volume Buzzer 	*/	

	LPC_PWM1->MCR = 0x00000002; 				/* Reseta Timer Counter em MR0		   					*/

	LPC_PWM1->PCR = (1<<9)|(1<<11)|(1<<14);		/* Modo controle por unica borda para PWM1.1,PWM1.3 e PWM1.6 */
							
											

	LPC_PWM1->LER = (1<<0)|(1<<1)|(1<<3)|(1<<6);/* Habilita Pino de Saída do  PWM1.1,PWM1.3 e PWM1.6 */
					
											
	LPC_PWM1->TCR = 0x09; 						/* Habilita modo PWM e inicia timer						*/
}

/*-------------------------------------------------------------------------------------------
 * Função: 		PWM_Change_Bright
 * Descrição: 	Recebe um valor de 0 a 10 inverte e multiplica por 10 e carrega em MR1
 *				que é o registrador do PWM do brilho.
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
-------------------------------------------------------------------------------------------*/

void PWM_Change_Bright (unsigned char value)
{
	switch (value)
	{
		case 0:			LPC_PWM1->MR1 =100;		break;
		
		case 1:			LPC_PWM1->MR1 =90;		break;
		
		case 2:			LPC_PWM1->MR1 =80;		break;
		
		case 3:			LPC_PWM1->MR1 =70;		break;
		
		case 4:			LPC_PWM1->MR1 =60;		break;

		case 5:			LPC_PWM1->MR1 =50;		break;
		
		case 6:			LPC_PWM1->MR1 =40;		break;
		
		case 7:			LPC_PWM1->MR1 =30;		break;
		
		case 8:			LPC_PWM1->MR1 =20;		break;		

		case 9:			LPC_PWM1->MR1 =10;		break;
		
		case 10:		LPC_PWM1->MR1 =0;		break;		
	}
	LPC_PWM1->LER |= (1<<1);
}

/*-------------------------------------------------------------------------------------------
 * Função: 		PWM_Change_Volume
 * Descrição: 	Recebe um valor de 0 a 10 multiplica por 10 e carrega em MR6 = VOLUME
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
-------------------------------------------------------------------------------------------*/

void PWM_Change_Volume (unsigned char value)
{
	LPC_PWM1->MR6 = (value*10);
	LPC_PWM1->LER |= (1<<6);
}

/*-------------------------------------------------------------------------------------------
 * Função: 		PWM_Beep
 * Descrição: 	Recebe um valor 0 ou 1, onde 1=ACIONA BEEP e 0=DESACIONA BEEP
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
-------------------------------------------------------------------------------------------*/

void PWM_Beep (unsigned char value)
{
	unsigned int scale;
	
	if (value==BEEP)
	{
		LPC_PWM1->MR0 = 250;								/* Ajusta para frequência de 4KHZ para BEEP */
		LPC_PWM1->MR3 = 50;									/* Duty Cycle 50% */		
		scale = LPC_PWM1->MR1;
		LPC_PWM1->MR1 = (scale/0.4);
		scale = LPC_PWM1->MR6;
		LPC_PWM1->MR6 = (scale/0.4);	
		LPC_PWM1->LER |= (1<<0)|(1<<3)|(1<<1)|(1<<6);			
	}
	else
	{
		LPC_PWM1->MR0 = 100;								/* Devolve a frequência para 10KHZ Volume */
		LPC_PWM1->MR3 = 0;									/* Duty Cycle 0% */		
		scale = LPC_PWM1->MR1;
		LPC_PWM1->MR1 = (scale*0.4);
		scale = LPC_PWM1->MR6;
		LPC_PWM1->MR6 = (scale*0.4);	
		LPC_PWM1->LER |= (1<<0)|(1<<3)|(1<<1)|(1<<6);			
	}
}


/*----------------------------------------------------------------------------
 * Final do arquivo
 *---------------------------------------------------------------------------*/
