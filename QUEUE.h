/*----------------------------------------------------------------------------
 *      Arquivo:    QUEUE.h
 *      Proposito: 	Gerenciamento da filas de atendimenrto
 *----------------------------------------------------------------------------
 *      Vers�o: 	1.00 	Prog: Ricardo T.   Data: 25/04/2013
 * 		Descri��o:  Vers�o inicial
 *----------------------------------------------------------------------------
 *      Este c�digo pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#ifndef		__QUEUE__
#define		__QUEUE__

#include	"APP.h"

/*----------------------------------------------------------------------------
 *      Definitions
 *---------------------------------------------------------------------------*/
#define		TKTCOMUM		0
#define		TKTPREF			1

#define		MSGSTANDARD		0
#define		MSGEXPAND		1
#define		MSGFORECAST		2
#define		MSGDATETIME		3
#define		MSGDATE			4
#define		MSGTIME			5

#define		ENABLE			1
#define		DISABLE			0

#define		CUTTOTAL		0
#define		CUTPARTIAL		1

/*----------------------------------------------------------------------------
 *      Ticket status definitions
 *---------------------------------------------------------------------------*/
#define		STA_NONE		0 		// nenhum status
#define		STA_WAITING		1		// Status em espera (ticket cadastrado local)
#define		STA_REDIRWAIT	2		// Status em espera (ticket cadastrado por encaminhamento)
#define		STA_WORKING		3		// Status em atendimento
#define		STA_CLOSED		4		// Status de encerramento (usu�rio fechou)
#define		STA_CANCELED	5		// Status de encerramento (usu�rio cancelou)
#define		STA_CLOSEDREP 	6		// Status de encerramento (fechou para gerar relat�rio)
#define		STA_CLOSEDAUTO 	7		// Status de encerramento (fechou ao chamar o pr�ximo)
#define		STA_REDIRECT	8		// Status de encerramento (fechou ao redirecionar o ticket)
#define		STA_CLOSEDINIT	9		// Status de encerramento (fechou ao ligar)
#define		STA_CLOSEDOVER	10		// Status de encerramento (fechou para liberar ticket em uso)

/*----------------------------------------------------------------------------
 *      Position struct atend definition
 *---------------------------------------------------------------------------*/
#define		_STAINI			0		// 1� WORD - Primeira grava��o na impress�o do ticket
#define		_DEPNUM			1		// 1� WORD - Primeira grava��o na impress�o do ticket
#define		_PRIORITY		2		// 1� WORD - Primeira grava��o na impress�o do ticket
#define		_ALPHANUM		3		// 1� WORD - Primeira grava��o na impress�o do ticket
#define		_NUMBER			4		// 2� WORD - Primeira grava��o na impress�o do ticket
#define		_UTCIMP			8		// 3� WORD - Primeira grava��o na impress�o do ticket
#define		_QUEUE			12		// 4� WORD - Primeira grava��o na impress�o do ticket
#define		_STAATEND		14		// 4� WORD - Segunda grava��o na chamada
#define		_USERNUM		15		// 4� WORD - Segunda grava��o na chamada
#define		_UTCINI			16		// 5� WORD - Segunda grava��o na chamada
#define		_NEWINF1		20		// 6� WORD
#define		_NEWINF2		21		// 6� WORD
#define		_NEWINF3		22		// 6� WORD
#define		_STAEND			23		// 6� WORD - Terceira grava��o no encerramento
#define		_UTCEND			24		// 7� WORD - Terceira grava��o no encerramento
#define		_OPINION		28		// 8� WORD - Quarta grava��o ap�s encerramento
#define		_NEWINF4		29		// 8� WORD
#define		_NEWINF5		30		// 8� WORD
#define		_NEWINF6		31		// 8� WORD

/*----------------------------------------------------------------------------
 *      User type definitions
 *---------------------------------------------------------------------------*/

typedef struct atend		  /* comprimento desta struct deve ser = LEN_VALID_DAT */
{
	unsigned char	staIni;					// Status inicial
	unsigned char 	depNum;					// N�mero do Departamento
	unsigned char	priority;				// N�vel de prioridade
	char 			alphaNum;				// Letra do Ticket
	unsigned int 	number;					// N�mero do ticket
	unsigned int 	utcImp;					// Utc da impress�o do ticket
	unsigned short	queue;					// Fila de espera no departamento no cadastro do ticket
	unsigned char	staAtend;				// Status de atendimento
	unsigned char 	userNum;				// N�mero do usu�rio do Departamento que atendeu o ticket
	unsigned int	utcIni;					// Hor�rio UTC da chamada para atendimento
	unsigned char	newInfo1;				// futuras implementa��es
	unsigned char	newInfo2;				// futuras implementa��es
	unsigned char	newInfo3;				// futuras implementa��es
	unsigned char 	staEnd;	 				// Status encerramento
	unsigned int	utcEnd;					// Hor�rio UTC do t�rmino do atendimento
	unsigned char	opinion;				// Opini�o do cliente
	unsigned char	newInfo4;				// futuras implementa��es
	unsigned char	newInfo5;				// futuras implementa��es
	unsigned char	newInfo6;				// futuras implementa��es
}ATD;

typedef struct ticketinfo
{
	unsigned int	number;
	unsigned char	alpha;
	unsigned char	type;
	unsigned char	dep;
}TKTINFO;

/*----------------------------------------------------------------------------
 *      Extern variables
 *---------------------------------------------------------------------------*/
extern unsigned int 	tktNumber [MAXDEP];
extern unsigned char   	contSchPri [MAXDEP];
extern unsigned char   	contSchHlp [MAXUSER];
extern CFG cfg;

/*----------------------------------------------------------------------------
 *      Extern functions
 *---------------------------------------------------------------------------*/
extern void TicketInit (void);
extern void SaveBeforeReport (void);
extern ATD New_ticketNumber (unsigned char depNum, unsigned char priority);
extern unsigned int New_ticketNumber_MIGCli (unsigned char depNum);
extern unsigned char Route_ticketNumber (unsigned int ticketNum, unsigned char alpha, unsigned char priority, unsigned char depNum);
extern unsigned char New_ticketNumber_TDCli (unsigned int ticketNum, unsigned char depNum, unsigned char priority);
extern unsigned short CalcForecast (unsigned char depNum);
extern char Check_liveTicket (ATD *pAtd);
extern char Close_ticket (ATD *pAtd);
extern void DEBUG_Tickets (void);
extern TKTINFO Get_ticketNum (unsigned char userNum, unsigned char depNum);
extern TKTINFO Get_ticketNumLast (unsigned char userNum);
extern char Set_ticketSta (unsigned short ticketNum, unsigned char status, unsigned char userNum, unsigned char *priority, unsigned char depNum);
extern unsigned char Get_ticketSta (unsigned short ticketNum, unsigned char userNum, unsigned char depNum);
extern char Set_Opinion (unsigned short ticketNum, unsigned char opinion, unsigned char userNum);
extern char Get_Opinion (unsigned short ticketNum, unsigned char userNum);

#endif

 /*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
