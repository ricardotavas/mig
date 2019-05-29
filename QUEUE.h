/*----------------------------------------------------------------------------
 *      Arquivo:    QUEUE.h
 *      Proposito: 	Gerenciamento da filas de atendimenrto
 *----------------------------------------------------------------------------
 *      Versão: 	1.00 	Prog: Ricardo T.   Data: 25/04/2013
 * 		Descrição:  Versão inicial
 *----------------------------------------------------------------------------
 *      Este código pertence a Wyma Tecnologia e Sistemas LTDA.
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
#define		STA_CLOSED		4		// Status de encerramento (usuário fechou)
#define		STA_CANCELED	5		// Status de encerramento (usuário cancelou)
#define		STA_CLOSEDREP 	6		// Status de encerramento (fechou para gerar relatório)
#define		STA_CLOSEDAUTO 	7		// Status de encerramento (fechou ao chamar o próximo)
#define		STA_REDIRECT	8		// Status de encerramento (fechou ao redirecionar o ticket)
#define		STA_CLOSEDINIT	9		// Status de encerramento (fechou ao ligar)
#define		STA_CLOSEDOVER	10		// Status de encerramento (fechou para liberar ticket em uso)

/*----------------------------------------------------------------------------
 *      Position struct atend definition
 *---------------------------------------------------------------------------*/
#define		_STAINI			0		// 1ª WORD - Primeira gravação na impressão do ticket
#define		_DEPNUM			1		// 1ª WORD - Primeira gravação na impressão do ticket
#define		_PRIORITY		2		// 1ª WORD - Primeira gravação na impressão do ticket
#define		_ALPHANUM		3		// 1ª WORD - Primeira gravação na impressão do ticket
#define		_NUMBER			4		// 2ª WORD - Primeira gravação na impressão do ticket
#define		_UTCIMP			8		// 3ª WORD - Primeira gravação na impressão do ticket
#define		_QUEUE			12		// 4ª WORD - Primeira gravação na impressão do ticket
#define		_STAATEND		14		// 4ª WORD - Segunda gravação na chamada
#define		_USERNUM		15		// 4ª WORD - Segunda gravação na chamada
#define		_UTCINI			16		// 5ª WORD - Segunda gravação na chamada
#define		_NEWINF1		20		// 6ª WORD
#define		_NEWINF2		21		// 6ª WORD
#define		_NEWINF3		22		// 6ª WORD
#define		_STAEND			23		// 6ª WORD - Terceira gravação no encerramento
#define		_UTCEND			24		// 7ª WORD - Terceira gravação no encerramento
#define		_OPINION		28		// 8ª WORD - Quarta gravação após encerramento
#define		_NEWINF4		29		// 8ª WORD
#define		_NEWINF5		30		// 8ª WORD
#define		_NEWINF6		31		// 8ª WORD

/*----------------------------------------------------------------------------
 *      User type definitions
 *---------------------------------------------------------------------------*/

typedef struct atend		  /* comprimento desta struct deve ser = LEN_VALID_DAT */
{
	unsigned char	staIni;					// Status inicial
	unsigned char 	depNum;					// Número do Departamento
	unsigned char	priority;				// Nível de prioridade
	char 			alphaNum;				// Letra do Ticket
	unsigned int 	number;					// Número do ticket
	unsigned int 	utcImp;					// Utc da impressão do ticket
	unsigned short	queue;					// Fila de espera no departamento no cadastro do ticket
	unsigned char	staAtend;				// Status de atendimento
	unsigned char 	userNum;				// Número do usuário do Departamento que atendeu o ticket
	unsigned int	utcIni;					// Horário UTC da chamada para atendimento
	unsigned char	newInfo1;				// futuras implementações
	unsigned char	newInfo2;				// futuras implementações
	unsigned char	newInfo3;				// futuras implementações
	unsigned char 	staEnd;	 				// Status encerramento
	unsigned int	utcEnd;					// Horário UTC do término do atendimento
	unsigned char	opinion;				// Opinião do cliente
	unsigned char	newInfo4;				// futuras implementações
	unsigned char	newInfo5;				// futuras implementações
	unsigned char	newInfo6;				// futuras implementações
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
