/*----------------------------------------------------------------------------
 *      Arquivo:    QUEUE.c
 *      Proposito: 	Gerenciamento das filas de atendimento
 *----------------------------------------------------------------------------
 *      Vers�o: 	1.01 	Prog: Ricardo T.   Data: 06/09/2017
 * 		Descri��o:  Vers�o inicial
 *----------------------------------------------------------------------------
 *      Este c�digo pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#include	"UART_CONFIG.h"
#include	"RTC_CONFIG.h"
#include	"IAP_CONFIG.h"
#include	"COMM_CONFIG.h"
#include	"DataFlash.h"
#include	"APP.h"
#include	"QUEUE.h"

/*----------------------------------------------------------------------------
 *      App.c   variables
 *---------------------------------------------------------------------------*/
extern unsigned char	STATPROG;
extern ATD	atend;

/*----------------------------------------------------------------------------
 *      REPORT.c   variables
 *---------------------------------------------------------------------------*/
extern unsigned char STATFILE;

/*----------------------------------------------------------------------------
 *      Local variables
 *---------------------------------------------------------------------------*/
unsigned int 			tktNumber [MAXDEP] = 		{ 0,0,0,0,0,0,0,0,0,0,0,0,0 };
unsigned char   		contSchPri [MAXDEP] =		{ 0,0,0,0,0,0,0,0,0,0,0,0,0 };
unsigned short			queue [MAXDEP] =			{ 0,0,0,0,0,0,0,0,0,0,0,0,0 };
unsigned int			lastCallDpto [MAXDEP]= 		{ 0,0,0,0,0,0,0,0,0,0,0,0,0	};
unsigned int			lastPrioDpto [MAXDEP]= 		{ 0,0,0,0,0,0,0,0,0,0,0,0,0 };
unsigned int			waitingTime [MAXDEP][5] = 	{
													{ 0,0,0,0,0 },
													{ 0,0,0,0,0 },
													{ 0,0,0,0,0 },
													{ 0,0,0,0,0 },
													{ 0,0,0,0,0 },
													{ 0,0,0,0,0 },
													{ 0,0,0,0,0 },
													{ 0,0,0,0,0 },
													{ 0,0,0,0,0 },
													{ 0,0,0,0,0 },
													{ 0,0,0,0,0 },
													{ 0,0,0,0,0 },
													{ 0,0,0,0,0 }
													};
unsigned char   		contSchHlp [MAXUSER] =		{ 
													0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
													0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
													0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
													0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 
													};
unsigned int			lastCallUser [MAXUSER]= 	{ 
													0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
													0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
													0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
													0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
													};
unsigned int			lastPrioUser [MAXUSER]= 	{ 
													0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
													0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
													0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
													0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
													};
CFG 					cfg;
USERCALL				userCall [MAXUSER];

/*----------------------------------------------------------------------------
 *      Local functions
 *---------------------------------------------------------------------------*/
void TicketInit (void);
void SaveBeforeReport (void);
char Set_Opinion (unsigned short ticketNum, unsigned char opinion, unsigned char userNum);
char Get_Opinion (unsigned short ticketNum, unsigned char userNum);
char Set_ticketSta (unsigned short ticketNum, unsigned char status, unsigned char userNum, unsigned char *priority, unsigned char depNum);
unsigned char Get_ticketSta (unsigned short ticketNum, unsigned char userNum, unsigned char depNum);
unsigned char Find_GreaterQueue (void);
unsigned short CalcForecast (unsigned char depNum);
unsigned int LiveTime (ATD *atd);
char Check_keepLiveTicket (ATD *atd);
void Close_TicketPrevious (unsigned char userNum);
void NewWaitingTime (unsigned char depNum, unsigned int time);
unsigned char Find_NextTicket (unsigned char depNum, unsigned char priority, ATD *atd);
TKTINFO Modify_NextTicket (unsigned char userNum, unsigned char depNum, unsigned char priority);
unsigned char isPriorityTurn (unsigned char depNum);
TKTINFO Ver_TypeTicket (unsigned char userNum, unsigned char depNum);
unsigned char Comp_WaitingTime (unsigned char depNum, unsigned char findLocal);
unsigned char isHelperLesser (unsigned char depNum);
unsigned char isHelperTurn (unsigned char userNum, unsigned char depNum);
TKTINFO Get_ticketNum (unsigned char userNum, unsigned char depNum);
TKTINFO Get_ticketNumLast (unsigned char userNum);
ATD New_ticketNumber (unsigned char depNum, unsigned char priority);
unsigned int New_ticketNumber_MIGCli (unsigned char depNum);
unsigned char New_ticketNumber_TDCli (unsigned int ticketNum, unsigned char depNum, unsigned char priority);

extern void Write_Flash (void);

/*----------------------------------------------------------------------------
 * Fun��es:		TicketInit
 * Descri��o: 	Inicializa vari�veis do sistema de senhas.
 * Par�metros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void TicketInit (void)
{
	unsigned char 	done[MAXDEP];
	unsigned char 	change=0;
	unsigned char 	depNum;
	unsigned char 	userNum;
	ATD 			atd;
	unsigned char 	*pAtd = (unsigned char *)&atd;

	for (depNum=0 ; depNum<MAXDEP ; depNum++)			// Faz varredura em todos os dptos.
		done[depNum]=0;									// Zera flag de pronto..

	for (depNum=0; depNum<MAXDEP ; depNum++)			// Verifica todos departamentos
		if (cfg.lastBeforeReport[depNum]==0)			// Zero = ligando a primeira vez..
		{												// Reinicia n�mero do ticket
		 	cfg.lastBeforeReport[depNum]=cfg.depRange[depNum][0];
			change=1;									// Indica que gravou na estrutura config
		}

	SetReader_End ();
	while (ReaderFlash_fromEnd (pAtd)) 					// Posiciona ponteiro de leitura no �ltimo ticket
	{	 												// para analisar os registros
		depNum=atd.depNum-1;  							// index para array de dptos.
		userNum=atd.userNum-1;							// index para array de usu�rios.
														
														// Verifica fila:
		if ((atd.staEnd==0xFF) && 						// Procura com status "N�o finalizada" e
			(atd.staAtend==0xFF) &&						// Status "N�o atendida" e
			(atd.staIni==STA_WAITING || 				// Status de "Aguardando" ou
			 atd.staIni==STA_REDIRWAIT))				// Status de "Encaminhamento Aguardando"
		{
			queue [depNum]++;							// Incrementa fila do departamento.
		}
														// Verifica �ltimo sendo atendido:
		else if (atd.staAtend==STA_WORKING)				// Procura com status "Em atendimento"
		{
			if (lastPrioUser[userNum]==0 &&				// Se ainda n�o encontrou �ltimo do usuario
				(atd.priority==TKTPREF))				// com status prefer�ncial:
				lastPrioUser[userNum]=GetReader();		// Inicia acelerador de tickets prefer.

			if (lastCallUser[userNum]==0 &&				// Se ainda n�o encontrou �ltimo do usuario
				(atd.priority==TKTCOMUM))				// com status comum:
				lastCallUser[userNum]=GetReader();		// Inicia acelerador de tickets comuns

			if (lastPrioDpto[depNum]==0 &&				// Se ainda n�o encontrou �ltimo do dpto
				(atd.priority==TKTPREF))				// com status prefer�ncial:
				lastPrioDpto[depNum]=GetReader();		// Inicia acelerador de tickets prefer.

			if (lastCallDpto[depNum]==0 &&				// Se ainda n�o encontrou �ltimo dpto
				(atd.priority==TKTCOMUM))				// com status comum:
				lastCallDpto[depNum]=GetReader();		// Inicia acelerador de tickets comuns

			if (atd.staEnd==0xFF)						// Desligou atendendo?
			{
				atd.staEnd=STA_CLOSEDINIT;				// Grava status de "finalizado ao ligar"				
				atd.utcEnd=RTC_To_UTC(1970);			// Grava o hor�rio UTC do encerramento
				ReWrite_DataFlash (	pAtd, 
								GetReader(), 
								_STAEND,5);				// Grava informa��es na Flash
			}
		}
		
		if (done[depNum]==0 &&							// Ainda n�o encontrou �ltimo n�mero?
		   ((atd.number>=cfg.depRange[depNum][0])&&		// Valor encontrado esta dentro da
		    (atd.number<=cfg.depRange[depNum][1])))		// faixa de n�meros do departamento?
		{
			tktNumber[depNum]=atd.number+1;				// Inicia com �ltimo n�mero + 1			
			done[depNum]=1;
		}
	}

	for (userNum=0; userNum<MAXUSER ; userNum++)		// Verifica todos os usu�rios
	{
		// Desabilita usu�rio para come�ar
		userCall[userNum].sta = 0;
		// Usu�rio n�o esta atendendo ningu�m..
		userCall[userNum].run = 0;
		// Reinicia hor�rio do �ltimo encerramento
		userCall[userNum].time = 0;
		//----------------------------------------
	}
	
	for (depNum=0; depNum<MAXDEP ; depNum++)			// Verifica todos os departamentos
	{
		if (done[depNum]==0)
		 	tktNumber[depNum]=cfg.lastBeforeReport[depNum];
	}

	if (change)											// Caso houver mudan�as:
	{
		Write_Flash ();
	}
}

/*----------------------------------------------------------------------------
 * Fun��es:		SaveBeforeReport
 * Descri��o: 	Salva o �ltimo ticket cadastrado em cada departamento 
 *				para manter a sequ�ncia da numera��o do departamento, na busca 
 *				ignora os cadastros feitos por encaminhamentos, devido a estes 
 *				alterarem a	ordem da numera��o do deapartamento.
 * Par�metros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void SaveBeforeReport (void)
{
	unsigned char 	done[MAXDEP];
	unsigned char 	change=0;
	unsigned char 	depNum;
	ATD 			atd;
	unsigned char 	*pAtd = (unsigned char *)&atd;

	for (depNum=0 ; depNum<MAXDEP ; depNum++)			// Faz varredura em todos os dptos.
		done[depNum]=0;									// Zera flag de pronto..

	SetReader_End ();									// Posiciona ponteiro de leitura no �ltimo 
	while (ReaderFlash_fromEnd (pAtd)) 					// ticket para analisar os registros
	{
		if (atd.staIni==STA_WAITING)					// Procura com status de "Aguardando",
		{												// n�o importa se ja foi atendido ou n�o.
			
			depNum=atd.depNum-1;  						// Verifica o departamento do registro.
			if (done[depNum]==0 &&						// Ainda n�o encontrou �ltimo n�mero?
			   ((atd.number>=cfg.depRange[depNum][0])&&	// Valor encontrado esta dentro da
			    (atd.number<=cfg.depRange[depNum][1])))	// faixa de n�meros do departamento?
			{
				cfg.lastBeforeReport[depNum]=atd.number+1;// Guarda �ltimo n�mero cadastrado.
				done[depNum]=1;							// Flag indica que gravou �ltimo n�mero.
				change=1;								// Indica que houve mudan�as
			}
		}
	}

	if (change)											// Caso houver mudan�as:
	{
		Write_Flash ();
	}
}

/*----------------------------------------------------------------------------
 * Fun��es:		Set_Opinion
 * Descri��o: 	Procura o atendimento atrav�s do n�mero do ticket, 
 *				e grava a opini�o do cliente sobre o atendimento.
 * Par�metros: 	ticketNum - N�mero do ticket
 *				opinion - Opini�o do cliente sobre o atendimento
 *				userNum - N�mero do usu�rio que atendeu
 * Retorno: 	(0)=n�o foi poss�vel gravar (1) opini�o gravada
 -----------------------------------------------------------------------------*/
char Set_Opinion (unsigned short ticketNum, unsigned char opinion, unsigned char userNum)
{
	ATD atd;
	unsigned char *pAtd = (unsigned char *)&atd;
	unsigned int callAddr=0;

	callAddr = SetReader_Begin ();						// Posiciona o ponteiro de leitura no in�cio dos 
														// dados ja gravados. Guarda o endere�o antes de ler.
	while (ReaderFlash_fromBegin (pAtd))				// L� dados e incrementa o endere�o de leitura.
	{
		if (atd.number==ticketNum)						// Verifica se � o mesmo n�mero do ticket
		{
			if (atd.opinion==0xFF)						// com opini�o ainda sem grava��o
			{
				atd.opinion=opinion;					// Grava opini�o na estrutura do atendimento
				ReWrite_DataFlash ( pAtd, 				// Grava informa��es de atendimento na FLASH
									callAddr,
									_OPINION,
									1);
				return 1;
			}
		}
		callAddr = GetReader ();	 					// Guarda o endere�o antes de ler o pr�ximo.
	}
	return 0;
}

/*----------------------------------------------------------------------------
 * Fun��es:		Get_Opinion
 * Descri��o: 	Procura o atendimento atrav�s do n�mero do ticket, 
 *				e devolve a opini�o do cliente sobre o atendimento.
 * Par�metros: 	ticketNum - N�mero do ticket
 *				userNum - N�mero do usu�rio que atendeu
 * Retorno: 	Opini�o do cliente
 -----------------------------------------------------------------------------*/
char Get_Opinion (unsigned short ticketNum, unsigned char userNum)
{
	ATD atd;
	unsigned char *pAtd = (unsigned char *)&atd;
	unsigned char depNum=0;

	depNum = cfg.depUsers [userNum-1];					// Verifica o dpto. do usu�rio

	SetReader_Begin ();									// Posiciona o ponteiro de leitura no in�cio dos dados ja gravados.
	while (ReaderFlash_fromBegin (pAtd))
	{
		if (atd.depNum==depNum)							// Verifica se � o mesmo dpto
		{
			if (atd.number==ticketNum)					// Verifica se � o mesmo n�mero do ticket
			{
				if (atd.opinion!=0xFF) 					// Verifica se ja foi gravada a opini�o,
					return atd.opinion;					// Devolve opini�o do cliente para o atendimento
				else 
					return 'E';	   						// Valor = Esperando opini�o
			}
		}
	}
	return 0;
}

/*----------------------------------------------------------------------------
 * Fun��es:		Set_ticketSta
 * Descri��o: 	Procura o ticket atrav�s do n�mero, guarda o endere�o onde o ticket
 *				foi encontrado e encerra a senha para realizar a mudan�a do status.
 * Par�metros: 	ticketNum - N�mero do regisro do ticket para mudar status
 *				status - Novo Status de atendimento
 *				userNum - N�mero do usu�rio (usado na mudan�a para status "atendendo")
 *				priority - Endere�o de vari�vel para devolver o tipo do ticket chamado
 *				depNum - N�mero do departamento para pesquisa
 * Retorno: 	(0)=n�o foi poss�vel alterar (1) status alterado
 -----------------------------------------------------------------------------*/
char Set_ticketSta (unsigned short ticketNum, unsigned char status, unsigned char userNum, unsigned char *priority, unsigned char depNum)
{
	ATD atd;
	unsigned char *pAtd = (unsigned char *)&atd;
	unsigned int callAddr=0;

	if (depNum==0)										// Se o dpto n�o foi informado:
		depNum = cfg.depUsers [userNum-1];				// Verifica o dpto. padr�o do usu�rio

	if (status==STA_WORKING)	 						// Verifica se o usu�rio quer mudar para "atendendo"
	{
		if (cfg.depCloseNext[depNum-1])					// Verifica se a op��o fechar autom�tico esta habilitada 
			Close_TicketPrevious (userNum);				// Fecha atendimento anterior
	}

	callAddr = SetReader_Begin ();						// Posiciona o ponteiro de leitura no in�cio dos 
														// dados ja gravados. Guarda o endere�o antes de ler.
	while (ReaderFlash_fromBegin (pAtd))				// L� dados e incrementa o endere�o de leitura.
	{
		if (atd.depNum==depNum)							// Verifica se � o mesmo dpto
		{
			if (atd.number==ticketNum)					// Verifica se � o mesmo n�mero do ticket
			{
				if (status==STA_WORKING)	 			// Usu�rio quer mudar para "atendendo"
				{
					if (atd.staAtend==0xFF)				// com status de atendimento sem grava��o
					{
						atd.staAtend=STA_WORKING;		// Muda status para "Em atendimento"
						atd.userNum=userNum;			// Grava usu�rio que atendeu o ticket
						atd.utcIni=RTC_To_UTC(1970);	// Grava o hor�rio UTC do atendimento
						ReWrite_DataFlash ( pAtd, 		// Grava informa��es de atendimento na FLASH
											callAddr,
											_STAATEND, 
											6);			

						queue[depNum-1]--;	   			// Decrementa fila do departamento atendido
					
						NewWaitingTime (depNum-1, 
										LiveTime(&atd));// Guarda novo tempo de espra
					
						if (atd.priority==TKTPREF)		// Usa acelerador para ticket preferencial
							lastPrioUser[userNum-1]=callAddr;
						else if (atd.priority==TKTCOMUM)// Usa acelerador para ticket comum
							lastCallUser[userNum-1]=callAddr;

						// Esta atendendo...
						userCall[userNum-1].run=1;
						//userCall[userNum-1].time=0xFFFFFFFF;	
						//-----------------------------------------

						*priority = atd.priority;		// Indica se o ticket chamado � prefer.

						return 1;
					}
				}
				else if (	status==STA_CLOSED || 	   	// Usu�rio quer mudar para "encerrado"
							status==STA_CANCELED ||
							status==STA_CLOSEDREP ||
							status==STA_CLOSEDAUTO ||
							status==STA_REDIRECT ||
							status==STA_CLOSEDINIT ||
							status==STA_CLOSEDOVER)
				{
					if ((atd.staAtend==STA_WORKING) &&	// ja tem status atendimento "working"
						(atd.staEnd==0xFF))				// com status Final sem grava��o
					{
						atd.staEnd=status;				// Grava status de "finalizado"				
						atd.utcEnd=RTC_To_UTC(1970);	// Grava o hor�rio UTC do encerramento
						ReWrite_DataFlash ( pAtd, 		// Grava novo status do ticket no endere�o encontrado.
											callAddr, 
											_STAEND, 
											5);

						// grava hora de encerramento
						userCall[userNum-1].run=0;
						userCall[userNum-1].time=atd.utcEnd;
						//---------------------------

						return 1;
					}
					else if ((atd.staAtend==0xFF) &&	// sem status de atendimento
							 (atd.staEnd==0xFF))		// sem status de encerramento
					{
						atd.userNum=userNum;			// Grava usu�rio que atendeu o ticket
						atd.staAtend=STA_WORKING;		// Muda status para "Em atendimento"
						atd.utcIni=atd.utcImp;			// Grava o hor�rio UTC do atendimento
						atd.staEnd=status;				// Grava status de "finalizado"				
						atd.utcEnd=RTC_To_UTC(1970);	// Grava o hor�rio UTC do encerramento
						ReWrite_DataFlash ( pAtd, 		// Grava informa��es de atendimento na FLASH
											callAddr,
											_STAATEND, 
											14);		

						// grava hora de encerramento
						userCall[userNum-1].run=0;
						userCall[userNum-1].time=atd.utcEnd;
						//--------------------------

						queue[depNum-1]--;	   			// Decrementa fila do departamento atendido

						*priority = atd.priority;		// Indica se o ticket chamado � prefer.

						return 1;
					}
					else
					{
						// grava hora de encerramento
						userCall[userNum-1].run=0;
						userCall[userNum-1].time=atd.utcEnd;
						//--------------------------
						return 0;
					}
				}
			}
		}
		callAddr = GetReader ();	 					// Guarda o endere�o antes de ler o pr�ximo.
	}
	return 0;
}

/*----------------------------------------------------------------------------
 * Fun��es:		Get_ticketSta
 * Descri��o: 	Devolve o status do ticket em resposta a solicita��o "GET"
 * Par�metros: 	ticketNum - N�mero do ticket para devolver o status
 *				userNum - N�mero do usu�rio
 *				depNum - N�mero do departamento
 * Retorno: 	Status do ticket
 -----------------------------------------------------------------------------*/
unsigned char Get_ticketSta (unsigned short ticketNum, unsigned char userNum, unsigned char depNum)
{
	ATD atd;
	unsigned char *pAtd = (unsigned char *)&atd;

	if (depNum==0)										// Se o dpto n�o foi informado:
		depNum = cfg.depUsers [userNum-1];				// Verifica o dpto. padr�o do usu�rio

	SetReader_Begin ();									// Posiciona o ponteiro de leitura no in�cio dos dados ja gravados.
	while (ReaderFlash_fromBegin (pAtd))
	{
		if (atd.depNum==depNum)							// Verifica se � o mesmo dpto
		{
			if (atd.number==ticketNum)					// Verifica se � o mesmo n�mero do ticket
			{
				if (atd.staEnd!=0xFF) 					// Status de final de ticket
					return atd.staEnd;
				else if (atd.staAtend!=0xFF) 			// Status de atendimento do ticket
					return atd.staAtend;	
				else if (atd.staIni!=0xFF) 				// Status de impress�o do ticket
					return atd.staIni;	
				else 
					return 0x30;
			}
		}
	}
	return 0;
}

/*----------------------------------------------------------------------------
 * Fun��es:		Find_GreaterQueue
 * Descri��o: 	Procura maior fila entre os departamentos
 * Par�metros: 	Nenhum 
 * Retorno: 	N�mero do departamento
 -----------------------------------------------------------------------------*/
unsigned char Find_GreaterQueue (void)
{
	unsigned char cnt;
	unsigned short lastQueue=0;
	unsigned char depNum=0;
	
	for (cnt=0 ; cnt<MAXDEP ; cnt++)					// Varredura em todos os departamentos
		if (queue[cnt]>lastQueue)						// Procura maior fila
		{
			lastQueue=queue[cnt];						// Guarda maior fila
			depNum=cnt+1;								// Guarda department da maior fila.
		}
	return depNum;										// Retorna departamento de maior fila
}

/*----------------------------------------------------------------------------
 * Fun��es:		CalcForecast
 * Descri��o: 	Calcula previs�o de espera em um departamento
 * Par�metros: 	depNum - N�mero do departamento 
 * Retorno: 	Previs�o de espera em minutos
 -----------------------------------------------------------------------------*/
unsigned short CalcForecast (unsigned char depNum)
{	
	unsigned int tForecast=0;
	unsigned char  contAtd=0;

	for (contAtd=0 ; contAtd<MAXPREV ; contAtd++)
	{
		if (waitingTime[depNum-1][contAtd]>0)
			tForecast+=waitingTime[depNum-1][contAtd];
		else
			break;
	}

	if (contAtd>0)
		tForecast = tForecast/contAtd;					// Calcula a m�dia de espera em segundos

	return (unsigned short) tForecast/60;				// Devolve a m�dia de espera em minutos
}

/*----------------------------------------------------------------------------
 * Fun��es:		LiveTime
 * Descri��o: 	Verifica o tempo de vida de um ticket desde a sua impress�o.
 * Par�metros: 	Estrutura de atendimento 
 * Retorno: 	Tempo em segundos
 -----------------------------------------------------------------------------*/
unsigned int LiveTime (ATD *atd)
{
	unsigned int secNow=0;
	unsigned int secPrint=0;

	secNow	=RTC_To_UTC(1970);
	secPrint=atd->utcImp;

	return (secNow-secPrint);
}

/*----------------------------------------------------------------------------
 * Fun��es:		Check_liveTicket
 * Descri��o: 	Procura tickets com hor�rio excedendo o tempo de vida �til, para
 *				ver se tem alguma informa��o para gerar o relat�rio.
 * Par�metros: 	Estrutura de atendimento para gravar os dados lidos em RAM
 * Retorno: 	(0) N�o encontrou ticket vencido (1) Encontrou ticket
 -----------------------------------------------------------------------------*/
char Check_liveTicket (ATD *atd)
{
	unsigned char *pAtd=(unsigned char*)atd;

	while (ReaderFlash_fromBegin (pAtd))				// L� dados e incrementa o endere�o de leitura.
	{
		if (LiveTime(atd)>cfg.keepLiveTicket)			// Verifica se tempo do ticket excedeu limite
			return 1;
	}

	return 0;
}

/*----------------------------------------------------------------------------
 * Fun��es:		Close_ticket
 * Descri��o: 	Procura tickets com hor�rio excedendo o tempo de vida �til, para
 *				permitir fechar relat�rio e gravar em arquivo, se encontrar um 
 *				ticket em atendimento finaliza ele com status "STA_CLOSEDAUTO".
 * Par�metros: 	Estrutura de atendimento para gravar os dados lidos em RAM
 * Retorno: 	(0) N�o encontrou ticket vencido (1) Encontrou ticket
 -----------------------------------------------------------------------------*/
char Close_ticket (ATD *atd)
{
	unsigned char *pAtd=(unsigned char*)atd;
	unsigned int callAddr=0;

	callAddr=SetReader_Begin ();						// Posiciona o ponteiro de leitura no in�cio dos 
														// dados ja gravados. Guarda o endere�o antes de ler.
	while (ReaderFlash_fromBegin (pAtd))				// L� dados e incrementa o endere�o de leitura.
	{
		if (LiveTime(atd)>cfg.keepLiveTicket)			// Verifica se tempo do ticket excedeu limite
		{
			if ((atd->staEnd==0xFF) && 					// Verifica se ainda n�o foi finalizado
				(atd->staAtend==0xFF) &&				// Verifica se ainda n�o foi atendido
				(atd->staIni==STA_WAITING))				// Procura com status de "Aguardando"
			{
				queue [atd->depNum-1]--;				// Senha n�o atendida vai ser eliminada, atualiza fila
			}

			if (cfg.depCloseRep[atd->depNum-1])			// Verifica se esta configurado para fechar o ticket
			{
				if ((atd->staEnd==0xFF) && 				// Verifica se ainda n�o foi finalizado
					(atd->staAtend==STA_WORKING))		// status = "Atendendo"	
				{
					atd->staEnd=STA_CLOSEDREP;			// Grava status de "finalizado"
					atd->utcEnd=RTC_To_UTC(1970);		// Grava o hor�rio UTC do atendimento
					ReWrite_DataFlash (pAtd, callAddr,
										_STAEND, 5);	// Grava informa��es na Flash
				}
			}
			return 1;
		}
		callAddr = GetReader ();	 					// Guarda o endere�o antes de ler o pr�ximo.
	}

	return 0;
}

/*----------------------------------------------------------------------------
 * Fun��es:		Close_TicketPrevious
 * Descri��o: 	Procura n�mero anterior de ticket do usu�rio e fecha atendimento
 * Par�metros: 	userNum - N�mero do usu�rio
 * Retorno: 	nenhum
 -----------------------------------------------------------------------------*/
void Close_TicketPrevious (unsigned char userNum)
{
	ATD atd;
	unsigned char *pAtd = (unsigned char *)&atd;
	unsigned int utc=0;
	unsigned char isLast=0;								// Indica qual foi o ultimo atendido..
														// (0)=fila normal (1)=fila preferencial

	if (lastPrioUser[userNum-1]!=0)						// Foi chamado um ticket preferencial anterior
		if (SetReader (lastPrioUser[userNum-1]))		// Ajusta endere�o do �ltimo ticket gravado
			if (ReaderFlash (pAtd))						// Faz leitura dos dados no endere�o espec�ficado
			{
				if ((atd.userNum==userNum) &&			// Procura n�mero do usu�rio 
					(atd.staEnd==0xFF) && 				// Procura tickets ainda n�o finalizados
					(atd.staAtend==STA_WORKING)&&		// Procura com status de "Atendendo"	
					(atd.priority==TKTPREF))			// Ticket Preferencial
				{
					utc = atd.utcIni;					// Guarda utc do atendimento preferencial
					isLast=TKTPREF;						// �ltimo: Ticket preferencial (por enquanto)
				}
			}

	if (lastCallUser[userNum-1]!=0)						// Foi chamado um ticket comum anterior
		if (SetReader (lastCallUser[userNum-1]))		// Ajusta endere�o do �ltimo ticket gravado
			if (ReaderFlash (pAtd))						// Faz leitura dos dados no endere�o espec�ficado
			{
				if ((atd.userNum==userNum) &&			// Procura n�mero do usu�rio 
					(atd.staEnd==0xFF) && 				// Procura tickets ainda n�o finalizados
					(atd.staAtend==STA_WORKING) &&		// Procura com status de "Atendendo"	
					(atd.priority==TKTCOMUM))			// Ticket Comum
				{
					if (atd.utcIni>utc)					// Verifica se cadastro � mais recente..
						isLast=TKTCOMUM;				// Se for mais recente �ltimo = comum
				}
			}

	if (isLast==TKTPREF && 								// �ltimo ticket foi um preferencial e
		lastPrioUser[userNum-1]!=0)						// confirma cadastro do ticket preferencial..
	{
		if (SetReader (lastPrioUser[userNum-1]))		// Ajusta endere�o do �ltimo ticket gravado
		{
			if (ReaderFlash (pAtd))						// Faz leitura dos dados no endere�o espec�ficado
			{
				if ((atd.userNum==userNum) &&			// Procura n�mero do usu�rio 
					(atd.staEnd==0xFF) && 				// Procura tickets ainda n�o finalizados
					(atd.staAtend==STA_WORKING)&&		// Procura com status de "Atendendo"	
					(atd.priority==TKTPREF))			// Ticket Preferencial
				{
					atd.staEnd=STA_CLOSEDAUTO;			// Grava status de "finalizado"				
					atd.utcEnd=RTC_To_UTC(1970);		// Grava o hor�rio UTC do encerramento
					ReWrite_DataFlash (	pAtd, 
								lastPrioUser[userNum-1], 
								_STAEND,5);				// Grava informa��es na Flash

					// grava hora de encerramento
					userCall[userNum-1].run=0;
					userCall[userNum-1].time=atd.utcEnd;
					//---------------------------
				}
			}
		}
	}
	else if (	isLast==TKTCOMUM && 					// �ltimo ticket foi comum e
				lastCallUser[userNum-1]!=0)				// Confirma cadastro do ticket comum..
	{
		if (SetReader (lastCallUser[userNum-1]))		// Ajusta endere�o do �ltimo ticket gravado
		{
			if (ReaderFlash (pAtd))						// Faz leitura dos dados no endere�o espec�ficado
			{
				if ((atd.userNum==userNum) &&			// Procura n�mero do usu�rio 
					(atd.staEnd==0xFF) && 				// Procura tickets ainda n�o finalizados
					(atd.staAtend==STA_WORKING) &&		// Procura com status de "Atendendo"	
					(atd.priority==TKTCOMUM))			// Ticket Comum
				{
					atd.staEnd=STA_CLOSEDAUTO;			// Grava status de "finalizado"				
					atd.utcEnd=RTC_To_UTC(1970);		// Grava o hor�rio UTC do encerramento
					ReWrite_DataFlash (	pAtd, 
								lastCallUser[userNum-1], 
								_STAEND,5);				// Grava informa��es na Flash

					// grava hora de encerramento
					userCall[userNum-1].run=0;
					userCall[userNum-1].time=atd.utcEnd;
					//---------------------------
				}
			}
		}
	}
}

/*----------------------------------------------------------------------------
 * Fun��es:		NewWaitingTime
 * Descri��o: 	Atualiza lista dos �ltimos 5 tempos de espera
 * Par�metros: 	depNum - N�mero do departamento 
 *				time - tempo de espera
 * Retorno: 	nenhum
 -----------------------------------------------------------------------------*/
void NewWaitingTime (unsigned char depNum, unsigned int time)
{
	unsigned char i;

	for (i=4 ; i>0 ; i--)
		waitingTime [depNum][i] = waitingTime [depNum][i-1];
	waitingTime [depNum][0] = time;
}

/*----------------------------------------------------------------------------
 * Fun��es:		Find_NextTicket
 * Descri��o: 	Procura pr�ximo n�mero de ticket n�o atendido no departamento
 * Par�metros: 	depNum - N�mero do departamento 
 *				priority - N�vel de prioridade
 *				atd - Estrutura de atendimento para guardar o resultado
 * Retorno: 	(0)=N�o encontrou (1)=Encontrou ticket
 -----------------------------------------------------------------------------*/
unsigned char Find_NextTicket (unsigned char depNum, unsigned char priority, ATD *atd)
{
	unsigned char *pAtd = (unsigned char *)atd;

	if (priority==TKTPREF)								// Busca � por ticket preferencial ?
	{
		if (lastPrioDpto[depNum-1]!=0)					// Foi chamado um ticket preferencial anterior ? 
		{
			if (SetReader (lastPrioDpto[depNum-1])==0)	// Ajusta endere�o do �ltimo ticket grav do dpto.
				SetReader_Begin ();						// Posiciona o ponteiro de leitura no in�cio dos 
		}												// dados ja gravados. Guarda o endere�o antes de ler.
		else
			SetReader_Begin ();							// Posiciona o ponteiro de leitura no in�cio dos 
														// dados ja gravados. Guarda o endere�o antes de ler.
	}
	else if (priority==TKTCOMUM)						// Busca � por ticket comum ?
	{
		if (lastCallDpto[depNum-1]!=0)					// Foi chamado um ticket comum anterior ? 
		{
			if (SetReader (lastCallDpto[depNum-1])==0)	// Ajusta endere�o do �ltimo ticket grav do dpto.
				SetReader_Begin ();						// Posiciona o ponteiro de leitura no in�cio dos 
		}												// dados ja gravados. Guarda o endere�o antes de ler.
		else
			SetReader_Begin ();							// Posiciona o ponteiro de leitura no in�cio dos 
														// dados ja gravados. Guarda o endere�o antes de ler.
	}

	while (ReaderFlash_fromBegin (pAtd))				// L� dados e incrementa o endere�o de leitura.
	{
		if ((atd->depNum==depNum) &&					// Procura ticket do departamento escolhido
			(atd->staEnd==0xFF) && 						// Verifica se ainda n�o foi finalizado
			(atd->staAtend==0xFF) &&					// Verifica se ainda n�o foi atendido
			((atd->staIni==STA_WAITING) ||				// Procura com status de "Aguardando"	
			(atd->staIni==STA_REDIRWAIT)) &&			// ou com status de "Encam. aguardando"
			(atd->priority==priority))					// Procura o n�vel de prioridade escolhido
		{
			return 1;
		}
	}

	return 0;
}

/*----------------------------------------------------------------------------
 * Fun��es:		Modify_NextTicket
 * Descri��o: 	Procura pr�ximo n�mero de ticket n�o atendido no departamento para
 * 				gravar as informa��es da chamada do ticket.
 * Par�metros: 	userNum - N�mero do usu�rio
 *				depNum - N�mero do departamento 
 *				priority - N�vel de prioridade
 * Retorno: 	N�mero do ticket
 -----------------------------------------------------------------------------*/
TKTINFO Modify_NextTicket (unsigned char userNum, unsigned char depNum, unsigned char priority)
{
  	TKTINFO 		tkt={0,0,0,0};
	ATD 			atd;
	unsigned char 	*pAtd = (unsigned char *)&atd;
	unsigned int 	callAddr = 0;

	if (priority==TKTPREF)								// Busca � por ticket preferencial ?
	{
		if (lastPrioDpto[depNum-1]!=0)					// Foi chamado um ticket preferencial anterior ? 
		{
			if (SetReader (lastPrioDpto[depNum-1]))		// Ajusta endere�o do �ltimo ticket grav do dpto.
				callAddr = lastPrioDpto[depNum-1];		// Endere�o do �ltimo ticket pref. do dpto.
			else
				callAddr = SetReader_Begin ();			// Posiciona o ponteiro de leitura no in�cio dos 
		}												// dados ja gravados. Guarda o endere�o antes de ler.
		else
			callAddr = SetReader_Begin ();				// Posiciona o ponteiro de leitura no in�cio dos 
														// dados ja gravados. Guarda o endere�o antes de ler.
	}
	else if (priority==TKTCOMUM)						// Busca � por ticket comum ?
	{
		if (lastCallDpto[depNum-1]!=0)					// Foi chamado um ticket comum anterior ? 
		{
			if (SetReader (lastCallDpto[depNum-1]))		// Ajusta endere�o do �ltimo ticket grav do dpto.
				callAddr = lastCallDpto[depNum-1];		// Endere�o do �ltimo ticket comum do dpto.
			else
				callAddr = SetReader_Begin ();			// Posiciona o ponteiro de leitura no in�cio dos 
		}												// dados ja gravados. Guarda o endere�o antes de ler.
		else
			callAddr = SetReader_Begin ();				// Posiciona o ponteiro de leitura no in�cio dos 
														// dados ja gravados. Guarda o endere�o antes de ler.
	}

	while (ReaderFlash_fromBegin (pAtd))				// L� dados e incrementa o endere�o de leitura.
	{
	  if ((atd.depNum==depNum) &&						// Procura ticket do departamento escolhido
		(atd.staEnd==0xFF) && 							// Verifica se ainda n�o foi finalizado
		(atd.staAtend==0xFF) &&							// Verifica se ainda n�o foi atendido
		((atd.staIni==STA_WAITING) ||					// Procura com status de "Aguardando"	
		(atd.staIni==STA_REDIRWAIT)) &&					// ou com status de "Encam. aguardando"
		(atd.priority==priority))						// Procura o n�vel de prioridade escolhido
	  {
		atd.staAtend=STA_WORKING;						// Muda status para "Em atendimento"
		atd.userNum=userNum;							// Grava usu�rio que atendeu o ticket
		atd.utcIni=RTC_To_UTC(1970);					// Grava o hor�rio UTC do atendimento

		ReWrite_DataFlash (pAtd,callAddr,_STAATEND,6);	// Grava informa��es de atendimento na FLASH

		NewWaitingTime (depNum-1,LiveTime(&atd));		// Guarda novo tempo de espra


		// Esta atendendo...
		userCall[userNum-1].run=1;
		//userCall[userNum-1].time=0xFFFFFFFF;	
		//-----------------------------------------
									 
					
		if (atd.priority==TKTPREF)						// Usa acelerador para ticket preferencial
		{
			lastPrioUser[userNum-1]=callAddr;
			lastPrioDpto[depNum-1]=callAddr;
		}
		else if (atd.priority==TKTCOMUM)				// Usa acelerador para ticket comum
		{
			lastCallUser[userNum-1]=callAddr;
			lastCallDpto[depNum-1]=callAddr;
		}

		tkt.number=atd.number;
		tkt.alpha=atd.alphaNum;
		tkt.dep=atd.depNum;
		tkt.type=atd.priority;
		
		return tkt;
	  }
	  callAddr = GetReader ();	 						// Guarda o endere�o antes de ler o pr�ximo.
	}

	return tkt;
}

/*----------------------------------------------------------------------------
 * Fun��es:		isPriorityTurn
 * Descri��o: 	Verifica de quem � a vez: Prefer�ncial ou Normal
 * Par�metros: 	depNum - N�mero do departamento 
 * Retorno: 	(0)=NORMAL (1)=PREFERENCIAL
 -----------------------------------------------------------------------------*/
unsigned char isPriorityTurn (unsigned char depNum)
{
	unsigned char result;

	if (((cfg.depSchPri[depNum-1]&0xF0)>>4)>=			// Verifica esquema: comum >= prefer�ncial ?
		 (cfg.depSchPri[depNum-1]&0x0F))
	{
		if (contSchPri[depNum-1]<						// Contador ainda menor que o esquema maior (comum)?
			((cfg.depSchPri[depNum-1]&0xF0)>>4))
			result=TKTCOMUM;							// � a vez do ticket comum	
		else
			result=TKTPREF;								// � a vez do ticket prefer�ncial
	}
	else												// Verifica esquema: prefer�ncial > normal
	{
		if (contSchPri[depNum-1]<						// Contador ainda menor que o esquema maior (prefer�ncial)?
			(cfg.depSchPri[depNum-1]&0x0F))
			result=TKTPREF;								// � a vez do ticket prefer�ncial
		else
			result=TKTCOMUM;							// � a vez do ticket comum	
	}

	return result;
}

/*----------------------------------------------------------------------------
 * Fun��es:		Ver_TypeTicket
 * Descri��o: 	Verifica esquema de chamada para procurar novo n�mero de ticket
 * Par�metros: 	userNum - N�mero do usu�rio
 *				depNum - N�mero do departamento 
 * Retorno: 	Estrutura simples de informa��o do ticket
 -----------------------------------------------------------------------------*/
TKTINFO Ver_TypeTicket (unsigned char userNum, unsigned char depNum)
{
  TKTINFO tkt={0,0,0,0};

  if (userNum==0 || depNum==0)  						// Retorna se n�o foi escolhido um usu�rio ou departamento
	return tkt;

  if (cfg.depCloseNext[depNum-1])						// Verifica se a op��o fechar autom�tico esta habilitada 
	Close_TicketPrevious (userNum);						// Fecha atendimento anterior

  if (isPriorityTurn(depNum))							// � a vez do prefer�ncial ?
  {
    tkt=Modify_NextTicket(userNum,depNum,TKTPREF);		// Busca cadastro de ticket prefer�ncial
									
    if (tkt.number)
      contSchPri[depNum-1]=0;							// Encontrou um ticket inicializa o esquema
    else
    {
      tkt=Modify_NextTicket(userNum,depNum,TKTCOMUM);	// N�o encontrou nenhum ticket pref. ent�o busca comum
    }
  }
  else													// � a vez do ticket comum :
  {
    tkt=Modify_NextTicket (userNum,depNum,TKTCOMUM);	// Busca cadastro de ticket comum
									
    if (tkt.number)			
	  contSchPri[depNum-1]++;							// Encontrou um ticket incrementa o esquema
	else
	{
	  tkt=Modify_NextTicket(userNum,depNum,TKTPREF);	// N�o encontrou nenhum ticket comum ent�o busca pref.
	}
  }

  if (tkt.number!=0)									// Verifica se encontrou algum ticket..
  {
 	queue[depNum-1]--;	   								// Decrementa fila do departamento atendido
  }

  return tkt;											// Rertorna informa��es do ticket
}

/*----------------------------------------------------------------------------
 * Fun��es:		Check_DptoHelp
 * Descri��o: 	Verifica configura��o de ajuda, 1�Dpto bits 7-4  2�Dpto bits 3-0.
 *				Para ajudar a maior fila 4 bits com valor=15. 
 * Par�metros: 	userNum - N�mero do usu�rio
 *				depNum - N�mero do departamento 
 * Retorno: 	Estrutura simples de informa��o do ticket
 -----------------------------------------------------------------------------*/
TKTINFO Check_DptoHelp (unsigned char userNum, unsigned char depNum)
{
  TKTINFO tkt={0,0,0,0};
			
  if (((cfg.depHelp[depNum-1]&0xF0)>>4)!=0)				// Verifica 1� op��o de ajuda
  {
	tkt=Ver_TypeTicket (userNum,						// Procura ticket no primeiro dpto. de ajuda
			((cfg.depHelp[depNum-1]&0xF0)>>4));
	
	if (tkt.number==0)
	{
	  if ((cfg.depHelp[depNum-1]&0x0F)!=0)				// N�o tinha ticket na 1� op��o, verifica 2� op��o de ajuda
	  {
		tkt=Ver_TypeTicket (userNum,					// Procura ticket no segundo atdo. de ajuda
						(cfg.depHelp[depNum-1]&0x0F));
	  }
	}
  }
  else if ((cfg.depHelp[depNum-1]&0x0F)!=0)				// 1� op��o n�o configurada, verifica 2� op��o de ajuda
  {
	tkt=Ver_TypeTicket (userNum,						// Procura ticket no segundo atdo. de ajuda
					(cfg.depHelp[depNum-1]&0x0F));
  }

  return tkt;											// Rertorna informa��es do ticket
}

/*----------------------------------------------------------------------------
 * Fun��es:		Comp_WaitingTime
 * Descri��o: 	Compara tempos de espera dos departamentos e verifica qual 
 *				tem o maior tempo de espera
 * Par�metros: 	depNum - N�mero do departamento
 *				findLocal - (0) N�o procura em dpto local (1) Procura dpto local
 * Retorno: 	N�mero do departamento com maior tempo de espera
 -----------------------------------------------------------------------------*/
unsigned char Comp_WaitingTime (unsigned char depNum, unsigned char findLocal)
{
  ATD atd;
  unsigned char dHelp1=0, dHelp2=0, result=0;
  unsigned int timeLocal=0, timeHelp1=0, timeHelp2=0;

  if (findLocal!=0)										// � para buscar na banda principal tamb�m?
  {
    if (isPriorityTurn(depNum))							// � a vez do prefer�ncial ?
    {
      if (Find_NextTicket(depNum,TKTPREF,&atd))			// Existe algum prefer. ? Sim..
	    timeLocal = atd.utcImp;
	  else if (Find_NextTicket(depNum,TKTCOMUM,&atd))	// N�o, procura comum..
	    timeLocal = atd.utcImp;
    }
    else												// � a vez do ticket comum :
    {
      if (Find_NextTicket(depNum,TKTCOMUM,&atd))		// Existe algum comum ? Sim..
	    timeLocal = atd.utcImp;
	  else if (Find_NextTicket(depNum,TKTPREF,&atd))	// N�o, procura prefer�ncial..
	    timeLocal = atd.utcImp;
    }
  }

  dHelp1 = ((cfg.depHelp[depNum-1]&0xF0)>>4);			// Verifica dpto de ajuda 1
  if (dHelp1!=0)					  					// Esta configurado ?
  {
    if (isPriorityTurn(dHelp1))							// � a vez do prefer�ncial ?
	{
	  if (Find_NextTicket(dHelp1,TKTPREF,&atd))			// Existe algum pref. ? Sim..
		timeHelp1 = atd.utcImp;
	  else if (Find_NextTicket(dHelp1,TKTCOMUM,&atd))	// N�o, procura comum..
	    timeHelp1 = atd.utcImp;
    }
	else		  										// � a vez do ticket prefer :
	{
	  if (Find_NextTicket(dHelp1,TKTCOMUM,&atd))		// Existe algum comum ? Sim..
	    timeHelp1 = atd.utcImp;
	  else if (Find_NextTicket(dHelp1,TKTPREF,&atd))	// N�o, procura prefer�ncial..
	    timeHelp1 = atd.utcImp;
	}
  }

  dHelp2 = (cfg.depHelp[depNum-1]&0x0F);				// Verifica dpto de ajuda 2
  if (dHelp2!=0)					  					// Esta configurado ?
  {
    if (isPriorityTurn(dHelp2))							// � a vez do prefer�ncial ?
    {
	  if (Find_NextTicket(dHelp2,TKTPREF,&atd))			// Existe algum pref. ? Sim..
	    timeHelp1 = atd.utcImp;
	  else if (Find_NextTicket(dHelp2,TKTCOMUM,&atd))	// N�o, procura comum..
	    timeHelp1 = atd.utcImp;
	}
	else		  										// � a vez do ticket prefer :
	{
	  if (Find_NextTicket(dHelp2,TKTCOMUM,&atd))		// Existe algum comum ? Sim..
	    timeHelp1 = atd.utcImp;
	  else if (Find_NextTicket(dHelp2,TKTPREF,&atd))	// N�o, procura prefer�ncial..
	    timeHelp1 = atd.utcImp;
	}
  }

  if (timeLocal!=0 && timeHelp1!=0 && timeHelp2!=0)
  {
    if (timeLocal<timeHelp1)							// Verifica��o de tempo de espera..
    {
      if (timeLocal<timeHelp2)				  			
	    result=depNum;									// Dpto Local esta na fila a mais tempo
	  else												
	    result=dHelp2;									// Dpto Help2 esta na fila a mais tempo
    }
    else if (timeHelp1<timeHelp2)							
      result=dHelp1;									// Deto Help1 esta na fila a mais tempo
    else													
 	  result=dHelp2;									// Dpto Help2 esta na fila a mais tempo
  }
  else if (timeLocal!=0 && timeHelp1!=0)
  {
    if (timeLocal<timeHelp1)
		result=depNum;
	else
		result=dHelp1;
  }
  else if (timeLocal!=0 && timeHelp2!=0)
  {
  	if (timeLocal < timeHelp2)
		result=depNum;
	else
		result=dHelp2;
  }
  else if (timeHelp1!=0 && timeHelp2!=0)
  {
  	if (timeHelp1 < timeHelp2)
		result=dHelp1;
	else
		result=dHelp2;
  } 
  else if (timeLocal!=0)
  	result=depNum;
  else if (timeHelp1!=0)
	result=dHelp1;
  else if (timeHelp2!=0)
	result=dHelp2;

  return result;										// Retorna informa��es do ticket
}

/*----------------------------------------------------------------------------
 * Fun��es:		isHelperLesser
 * Descri��o: 	Verifica se a ajuda � menor no esquema
 * Par�metros: 	depNum - N�mero do departamento 
 * Retorno: 	(0)=� MAIOR OU IGUAL (1)=� MENOR
 -----------------------------------------------------------------------------*/
unsigned char isHelperLesser (unsigned char depNum)
{
	if (((cfg.depSchHlp[depNum-1]&0xF0)>>4)>=			// Verifica esquema: dpto.local >= dpto.ajuda ?
	    (cfg.depSchHlp[depNum-1]&0x0F))
		return 1;
	return 0;
}

/*----------------------------------------------------------------------------
 * Fun��es:		isHelperTurn
 * Descri��o: 	Verifica de quem � a vez: Local ou Ajuda
 * Par�metros: 	userNum - N�mero do usu�rio
 *				depNum - N�mero do departamento 
 * Retorno: 	(0)=LOCAL (1)=AJUDA
 -----------------------------------------------------------------------------*/
unsigned char isHelperTurn (unsigned char userNum, unsigned char depNum)
{
	unsigned char result;

	if (((cfg.depSchHlp[depNum-1]&0xF0)>>4)>=			// Verifica esquema: dpto.local >= dpto.ajuda ?
	    (cfg.depSchHlp[depNum-1]&0x0F))
	{
	  if (cfg.optSchHlp[depNum-1])	  					// Op��o para analisar esquema por Usu�rio
	  {
	  	if (contSchHlp[userNum-1]<						// Contador ainda menor que esquema maior (dpto.local)?
		   ((cfg.depSchHlp[depNum-1]&0xF0)>>4))
		  result=0;										// Dpto local
	  	else
		  result=1;										// Dpto ajuda
	  }
	  else												// Op��o para analisar esquema por Depto
	  {
	  	if (contSchHlp[depNum-1]<						// Contador ainda menor que o esquema maior (dpto.local)?
		   ((cfg.depSchHlp[depNum-1]&0xF0)>>4))
		  result=0;										// Dpto local
	  	else
		  result=1;										// Dpto ajuda
	  }
	}
	else												// Verifica esquema: dpto.ajuda > dpto.local 
	{
	  if (cfg.optSchHlp[depNum-1])						// Op��o para analisar esquema por Usu�rio
	  {
	  	if (contSchHlp[userNum-1]<						// Contador ainda menor que esquema maior (dpto.ajuda)?
	       (cfg.depSchHlp[depNum-1]&0x0F))
		  result=1;										// Dpto ajuda
	  	else
		  result=0;										// Dpto local
	  }
	  else												// Op��o para analisar esquema por Dpto
	  {
	  	if (contSchHlp[depNum-1]<						// Contador ainda menor que esquema maior (dpto.ajuda)?
	       (cfg.depSchHlp[depNum-1]&0x0F))
		  result=1;										// Dpto ajuda
	  	else
		  result=0;										// Dpto local
	  }
	}

	return result;
}

/*----------------------------------------------------------------------------
 * Fun��es:		Get_ticketNum
 * Descri��o: 	Busca pr�ximo n�mero do ticket para o usu�rio
 * Par�metros: 	userNum - N�mero do usu�rio
 *				depNum - N�mero do departamento
 * Retorno: 	Estrutura simples de informa��o do ticket
 -----------------------------------------------------------------------------*/
TKTINFO Get_ticketNum (unsigned char userNum, unsigned char depNum)
{
  TKTINFO tkt={0,0,0,0};

  if (depNum==0)							   			// Se n�o especificou o dpto:
    if (userNum!=0 && userNum<=MAXUSER)					// Verifica range e
      depNum = cfg.depUsers [userNum-1];				// Verifica o dpto. do usu�rio

  if (depNum!=0 && depNum<=MAXDEP)						// Deve estar dentro da quantidade de atdos.
  {
	if (cfg.depPref[depNum-1]!=0)						// Existe um dpto pref.+80 para atender antes deste?
	{
	  tkt=Ver_TypeTicket(userNum,13);					// Busca ticket no dpto pref. +80.
	  if (tkt.number!=0)
	  {	
		return tkt;										// Rertorna informa��es do ticket
	  }
	}
	
	if (cfg.depHelp[depNum-1]!=0)						// Existem dptos configurados para ajudar,
	{													// verifica esquema de ajuda em cada 4bits  
	  if ((cfg.depOptionHlp[depNum-1]&0x0F)==0x01)		// Somente ajudar quando acabar fila local ?
	  {
	    tkt=Ver_TypeTicket (userNum, depNum);	 		// Busca ticket no dpto local.
		if (tkt.number==0)
		{
  		  if ((cfg.depOptionHlp[depNum-1]&0xF0)==0x10)	// Ajudar dpto. de maior fila ?
			tkt=Ver_TypeTicket (userNum,		 		// Busca em dpto. de maior fila
							Find_GreaterQueue());
  		  else 
		  {
		    if (cfg.checkTime[depNum-1]==0)				// N�o verifica tempo de espera ?
		  	  tkt=Check_DptoHelp(userNum, depNum);		// Busca ticket no dpto de ajuda.
		    else
	  		  tkt=Ver_TypeTicket(userNum,			 	// Busca em dpto com maior tempo de espera 
			  			Comp_WaitingTime(depNum,0)); 	// entre dptos (somente dptos de ajuda)
		  }
		}
	  }
	  else
	  {
		if (isHelperTurn(userNum, depNum))				// Verifica contagem do esquema de ajuda
		{
		  if ((cfg.depOptionHlp[depNum-1]&0xF0)==0x10)	// Ajudar dpto. de maior fila ?
			tkt=Ver_TypeTicket (userNum,	 			// Busca em dpto. de maior fila
						Find_GreaterQueue());
  		  else 
		  {
			if (cfg.checkTime[depNum-1]==0)				// N�o verifica tempo de espera ?
			{
		  	  tkt=Check_DptoHelp(userNum, depNum);		// Busca ticket no dpto de ajuda.
			  if (tkt.number==0)						// N�o tem ticket na ajuda
				tkt=Ver_TypeTicket (userNum, depNum);	// Continua buscando no dpto.local
			}
		    else
	  		  tkt=Ver_TypeTicket(userNum,				// Busca em dpto com maior tempo de espera 
			  			Comp_WaitingTime(depNum,1));	// entre dptos (Dptos local e de ajuda)
		  }
			
		  if (tkt.number && tkt.dep!=depNum)			// Se buscou ticket de depto de ajuda
		  {
		  	if (isHelperLesser(depNum))
			{
			  if (cfg.optSchHlp[depNum-1])				// Op��o para analisar esquema por Usu�rio
			    contSchHlp[userNum-1]=0;				// Inicializa esquema de ajuda por Usu�rio
			  else										// Op��o para analisar esquema por Dpto
		        contSchHlp[depNum-1]=0;					// Inicializa esquema de ajuda por Dpto
			}
			else
			{
			  if (cfg.optSchHlp[depNum-1])				// Op��o para analisar esquema por Usu�rio
			    contSchHlp[userNum-1]++;				// Incrementa esquema de ajuda por Usu�rio
			  else										// Op��o para analisar esquema por Dpto
			    contSchHlp[depNum-1]++;					// Incrementa esquema de ajuda por Dpto
			}
		  }
		}
		else
		{
		  tkt=Ver_TypeTicket (userNum, depNum); 		// Continua buscando no dpto.local
		  if (tkt.number)
		  {
		    if (isHelperLesser(depNum))
			{
		      if (cfg.optSchHlp[depNum-1])				// Op��o para analisar esquema por Usu�rio
			    contSchHlp[userNum-1]++;				// Incrementa esquema de ajuda por Usu�rio
			  else										// Op��o para analisar esquema por Dpto
			    contSchHlp[depNum-1]++;					// Incrementa esquema de ajuda por Dpto
			}
			else
			{
			  if (cfg.optSchHlp[depNum-1])				// Op��o para analisar esquema por Usu�rio
			    contSchHlp[userNum-1]=0;				// Inicializa esquema de ajuda por Usu�rio
			  else										// Op��o para analisar esquema por Dpto
			    contSchHlp[depNum-1]=0;					// Inicializa esquema de ajuda por Dpto

			}
		  }
		  else
		  {
  		  	if ((cfg.depOptionHlp[depNum-1]&0xF0)==0x10)// Ajudar dpto. de maior fila ?
			  tkt=Ver_TypeTicket (userNum,				// Busca em dpto. de maior fila
			  				Find_GreaterQueue());		
  		  	else 
			{
			  if (cfg.checkTime[depNum-1]==0)			// N�o verifica tempo de espera ?
		  	    tkt=Check_DptoHelp(userNum, depNum);	// Busca ticket no dpto de ajuda.
		      else
	  		    tkt=Ver_TypeTicket(userNum,			  	// Busca em dpto com maior tempo de espera 
				         Comp_WaitingTime(depNum,0)); 	// entre dptos (somente dptos de ajuda)
			}
		  }
		}
	  }
	}
	else
	{
	  tkt=Ver_TypeTicket (userNum, depNum);	 			// busca ticket no dpto local.
	}
  }

  return tkt;											// Rertorna informa��es do ticket
}

/*----------------------------------------------------------------------------
 * Fun��es:		Get_ticketNumLast
 * Descri��o: 	Procura n�mero anterior de ticket do usu�rio 
 * Par�metros: 	userNum - N�mero do usu�rio
 * Retorno: 	Estrutura simples de informa��o do ticket
 -----------------------------------------------------------------------------*/
TKTINFO Get_ticketNumLast (unsigned char userNum)
{
	TKTINFO tkt={0,0,0,0};
	ATD atd;
	unsigned char *pAtd = (unsigned char *)&atd;
	unsigned int utc=0;
	unsigned char isLast=0;								// Indica qual foi o ultimo atendido..
														// (0)=fila normal (1)=fila preferencial

	if (lastPrioUser[userNum-1]!=0)						// Foi chamado um ticket preferencial anterior
		if (SetReader (lastPrioUser[userNum-1]))		// Ajusta endere�o do �ltimo ticket gravado
			if (ReaderFlash (pAtd))						// Faz leitura dos dados no endere�o espec�ficado
			{
				if ((atd.userNum==userNum) &&			// Procura n�mero do usu�rio 
					(atd.staEnd==0xFF) && 				// Procura tickets ainda n�o finalizados
					(atd.staAtend==STA_WORKING)&&		// Procura com status de "Atendendo"	
					(atd.priority==TKTPREF))			// Ticket Preferencial
				{
					utc = atd.utcIni;					// Guarda utc do atendimento preferencial
					isLast=TKTPREF;						// �ltimo: Ticket preferencial (por enquanto)
				}
			}

	if (lastCallUser[userNum-1]!=0)						// Foi chamado um ticket comum anterior
		if (SetReader (lastCallUser[userNum-1]))		// Ajusta endere�o do �ltimo ticket gravado
			if (ReaderFlash (pAtd))						// Faz leitura dos dados no endere�o espec�ficado
			{
				if ((atd.userNum==userNum) &&			// Procura n�mero do usu�rio 
					(atd.staEnd==0xFF) && 				// Procura tickets ainda n�o finalizados
					(atd.staAtend==STA_WORKING) &&		// Procura com status de "Atendendo"	
					(atd.priority==TKTCOMUM))			// Ticket Comum
				{
					if (atd.utcIni>utc)					// Verifica se cadastro � mais recente..
						isLast=TKTCOMUM;				// Se for mais recente �ltimo = comum
				}
			}

	if (isLast==TKTPREF && 								// �ltimo ticket foi um preferencial e
		lastPrioUser[userNum-1]!=0)						// confirma cadastro do ticket preferencial..
	{
		if (SetReader (lastPrioUser[userNum-1]))		// Ajusta endere�o do �ltimo ticket gravado
		{
			if (ReaderFlash (pAtd))						// Faz leitura dos dados no endere�o espec�ficado
			{
				if ((atd.userNum==userNum) &&			// Procura n�mero do usu�rio 
					(atd.staEnd==0xFF) && 				// Procura tickets ainda n�o finalizados
					(atd.staAtend==STA_WORKING) &&		// Procura com status de "Atendendo"	
					(atd.priority==TKTPREF))			// Verifica se � preferencial mesmo
				{
					tkt.number = atd.number;			// Guarda n�mero do ticket
					tkt.type = atd.priority;			// Guarda tipo de ticket
					tkt.dep = atd.depNum;				// Guarda Dpto do ticket
					tkt.alpha = atd.alphaNum;
				}
			}
		}
	}
	else if (isLast==TKTCOMUM && 						// �ltimo ticket foi comum e
		lastCallUser[userNum-1]!=0)						// confirma cadastro do ticket comum..
	{
		if (SetReader (lastCallUser[userNum-1]))		// Ajusta endere�o do �ltimo ticket gravado
		{
			if (ReaderFlash (pAtd))						// Faz leitura dos dados no endere�o espec�ficado
			{
				if ((atd.userNum==userNum) &&			// Procura n�mero do usu�rio 
					(atd.staEnd==0xFF) && 				// Procura tickets ainda n�o finalizados
					(atd.staAtend==STA_WORKING) &&		// Procura com status de "Atendendo"	
					(atd.priority==TKTCOMUM))			// Verifica se � comum mesmo
				{
					tkt.number = atd.number;			// Guarda n�mero do ticket
					tkt.type = atd.priority;			// Guarda tipo de ticket
					tkt.dep = atd.depNum;				// Dpto do ticket
					tkt.alpha = atd.alphaNum;
				}
			}
		}
	}
	return tkt;											// Rertorna informa��es do ticket
}

/*----------------------------------------------------------------------------
 * Fun��es:		New_ticketNumber_MIGCli
 * Descri��o: 	Inicializa os dados do novo ticket e grava em Flash
 * Par�metros: 	depNum - N�mero do departamento
 * Retorno: 	N�mero do ticket
 -----------------------------------------------------------------------------*/
unsigned int New_ticketNumber_MIGCli (unsigned char depNum)
{
	ATD atd;
	unsigned char *pAtd = (unsigned char *)&atd;

	if (depNum!=0 && depNum<=MAXDEP)						// Deve estar dentro da quantidade de atdos.
	{
		if (cfg.depEnable[depNum-1])
		{
			if (cfg.depIsBtPref[depNum-1]==0)
				atd=New_ticketNumber (depNum, TKTCOMUM);
			else
				atd=New_ticketNumber (cfg.depIsBtPref[depNum-1], TKTPREF);
		}

		if (atd.number!=0)
		{
			Write_DataFlash (pAtd, 14);						// Grava o novo ticket na Flash de dados
			return atd.number;
		}
	}
	return 0;
}

/*----------------------------------------------------------------------------
 * Fun��es:		Route_ticketNumber
 * Descri��o: 	Encaminha um ticket ticket grava em Flash
 * Par�metros: 	ticketNum - N�mero do ticket
 *				alpha - Letra do departamento
 *				priority - Preferencia do ticket
 * 				depNum - N�mero do departamento
  * Retorno: 	0= Erro numero zero ou ja existe  1= OK  
 -----------------------------------------------------------------------------*/
unsigned char Route_ticketNumber (unsigned int ticketNum, unsigned char alpha, unsigned char priority, unsigned char depNum)
{
	if (ticketNum!=0 && depNum!=0)
	{
		ATD atd;
		unsigned char *pAtd = (unsigned char *)&atd;
	
		SetReader_Begin ();										// Posiciona ponteiro no primeiro ticket ja gravado
		while (ReaderFlash_fromBegin (pAtd))					// Busca ticket....
		{
			if (atd.depNum==depNum &&							// Existe mesmo departamento com
				atd.number==ticketNum &&						// este n�mero de ticket na fila?
				atd.staAtend==0xFF)								// Se ainda n�o foi chamado cancela
			{
				return 0;										// Retorna estrutura com n�mero zero (inv�lido)
			}
		}	
		
		atd.staIni=STA_REDIRWAIT;								// Grava status=Aguardando atendimento
		atd.depNum=depNum;										// departamento escolhido pelo cliente
		atd.priority=priority;									// N�vel de prioridade da senha
		atd.alphaNum = alpha;									// Grava letra correspondenete ao departamento
		atd.number=ticketNum;									// Grava atual e incrementa n�mero do ticket
		atd.utcImp = RTC_To_UTC(1970);
		atd.queue = ++queue[depNum-1];							// Incrementa fila do departamento
		atd.staAtend = 0xFF;
		atd.userNum = 0xFF;										// Ningu�m atendeu ainda o ticket
		atd.utcIni = 0xFFFFFFFF;
		atd.utcEnd = 0xFFFFFFFF;
		atd.staEnd = 0xFF;
		atd.opinion = 0xFF;
		atd.newInfo1 = 0xFF;
		atd.newInfo2 = 0xFF;
		atd.newInfo3 = 0xFF;
		atd.newInfo4 = 0xFF;
		atd.newInfo5 = 0xFF;
		atd.newInfo6 = 0xFF;

		Write_DataFlash (pAtd, 14);							// Grava o novo ticket na Flash de dados
		return 1;
	}
	return 0;
}

/*----------------------------------------------------------------------------
 * Fun��es:		New_ticketNumber_TDCli
 * Descri��o: 	Inicializa os dados do novo ticket e grava em Flash
 * Par�metros: 	ticketNum - N�mero do ticket
 * 				depNum - N�mero do departamento
  * Retorno: 	0= Erro numero zero ou ja existe  1= OK  
 -----------------------------------------------------------------------------*/
unsigned char New_ticketNumber_TDCli (unsigned int ticketNum, unsigned char depNum, unsigned char priority)
{
	if (ticketNum!=0 && depNum!=0)
	{
		ATD atd;
		unsigned char *pAtd = (unsigned char *)&atd;
	
		SetReader_Begin ();										// Posiciona ponteiro no primeiro ticket ja gravado
		while (ReaderFlash_fromBegin (pAtd))					// Busca ticket....
		{
			if (atd.depNum==depNum &&							// Existe mesmo departamento com
				atd.number==ticketNum &&						// este n�mero de ticket na fila?
				atd.staAtend==0xFF)								// Se ainda n�o foi chamado cancela
			{
				return 0;										// Retorna estrutura com n�mero zero (inv�lido)
			}
		}	
		
		atd.staIni=STA_REDIRWAIT;								// Grava status=Aguardando atendimento
		atd.depNum=depNum;										// departamento escolhido pelo cliente
		atd.priority=priority;									// N�vel de prioridade da senha
		atd.alphaNum = cfg.depAlpha[depNum-1];					// Grava letra correspondenete ao departamento
		atd.number=ticketNum;									// Grava atual e incrementa n�mero do ticket
		atd.utcImp = RTC_To_UTC(1970);
		atd.queue = ++queue[depNum-1];							// Incrementa fila do departamento
		atd.staAtend = 0xFF;
		atd.userNum = 0xFF;										// Ningu�m atendeu ainda o ticket
		atd.utcIni = 0xFFFFFFFF;
		atd.utcEnd = 0xFFFFFFFF;
		atd.staEnd = 0xFF;
		atd.opinion = 0xFF;
		atd.newInfo1 = 0xFF;
		atd.newInfo2 = 0xFF;
		atd.newInfo3 = 0xFF;
		atd.newInfo4 = 0xFF;
		atd.newInfo5 = 0xFF;
		atd.newInfo6 = 0xFF;

		Write_DataFlash (pAtd, 14);							// Grava o novo ticket na Flash de dados
		return 1;
	}
	return 0;
}

/*----------------------------------------------------------------------------
 * Fun��es:		New_ticketNumber
 * Descri��o: 	Inicializa os dados do novo ticket e grava em Flash
 * Par�metros: 	depNum - N�mero do departamento
 * Retorno: 	Novo registro de atendimento
 -----------------------------------------------------------------------------*/
ATD New_ticketNumber (unsigned char depNum, unsigned char priority)
{
	ATD atd;
	unsigned char *pAtd = (unsigned char *)&atd;
	unsigned int callAddr=0;								

	if (tktNumber[depNum-1]>cfg.depRange[depNum-1][1])		// Se ultrapassou range:
		tktNumber[depNum-1]=cfg.depRange[depNum-1][0];		// Reinicializa n�mero do ticket

	callAddr = SetReader_Begin ();							// Posiciona o ponteiro de leitura no in�cio dos 
															// dados ja gravados. Guarda o endere�o antes de ler.
	while (ReaderFlash_fromBegin (pAtd))					// L� dados e incrementa o endere�o de leitura.
	{
		if (atd.depNum==depNum &&							// Existe mesmo departamento com
			atd.number==tktNumber[depNum-1] &&				// este n�mero de ticket na fila?
			atd.staEnd==0xFF)								// Ainda n�o atendeu?
		{
			if (atd.staAtend==STA_WORKING)
			{
				// Vamos encerrar uma suposta senha em atendimento...
				// para que o sistema n�o trave mais, caso tenha
				// ocorrido um problema deste tipo.
				// Por�m a senha que estava em atendimento,
				// pode ter passado a pertencer a duas pessoas.. caso esta
				// senha for encaminhada poder� ter duas pessoas com 
				// o mesmo n�mero..
				//
				// Ou seja � uma solu��o somente para casos em que n�o seja 
				// usado o encaminhamento.

				atd.staEnd=STA_CLOSEDOVER;					// Grava status de "finalizado para liberar numero"
				atd.utcEnd=RTC_To_UTC(1970);				// Grava o hor�rio UTC do encerramento
				ReWrite_DataFlash (	pAtd, 
									callAddr,
									_STAEND,5);				// Grava informa��es na Flash
			}
			else{								
				atd.number=0;								// Erro: N�o � poss�vel cadastrar
				return atd;									// Retorna estrutura com n�mero zero (inv�lido)
			}
		}
		callAddr = GetReader ();	 						// Guarda o endere�o antes de ler o pr�ximo.
	}	
	atd.staIni=STA_WAITING;	   								// Grava status=Aguardando atendimento
	atd.depNum=depNum;										// departamento escolhido pelo cliente
	atd.priority=priority;									// N�vel de prioridade da senha
	atd.alphaNum = cfg.depAlpha[depNum-1];					// Grava letra correspondenete ao departamento

	if (tktNumber[depNum-1] <= cfg.depRange[depNum-1][1])	// Se n�o ultrapassou range:
		atd.number=tktNumber[depNum-1]++;					// Grava atual e incrementa n�mero do ticket
	else
	{
		tktNumber[depNum-1] = cfg.depRange[depNum-1][0];	// Reinicializa n�mero do ticket
		atd.number=tktNumber[depNum-1]++;					// Grava atual e incrementa n�mero do ticket
	}

	atd.utcImp = RTC_To_UTC(1970);
	atd.queue = ++queue[depNum-1];							// Incrementa fila do departamento
	atd.staAtend = 0xFF;
	atd.userNum = 0xFF;										// Ningu�m atendeu ainda o ticket
	atd.utcIni = 0xFFFFFFFF;
	atd.utcEnd = 0xFFFFFFFF;
	atd.staEnd = 0xFF;
	atd.opinion = 0xFF;
	atd.newInfo1 = 0xFF;
	atd.newInfo2 = 0xFF;
	atd.newInfo3 = 0xFF;
	atd.newInfo4 = 0xFF;
	atd.newInfo5 = 0xFF;
	atd.newInfo6 = 0xFF;

	return atd;
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
