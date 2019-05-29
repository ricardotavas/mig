/*----------------------------------------------------------------------------
 *      Arquivo:    MENU.h
 *      Proposito: 	Menu de configuração para o usuário
 *----------------------------------------------------------------------------
 *      Versão: 	1.00 	Prog: Ricardo T.   Data: 25/04/2013
 * 		Descrição:  Versão inicial
 *----------------------------------------------------------------------------
 *      Este código pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#ifndef __MENU
#define __MENU

/*----------------------------------------------------------------------------
 *      Definitions
 *---------------------------------------------------------------------------*/

#define			_MENUNONE		0
#define			_MAINMENU		1
#define			_TIMEMENU		2
#define			_AJTIMEMENU		3
#define			_AJDATEMENU		4
#define			_DRIVERMENU		5
#define			_FTDRIVERMENU	6
#define			_TIMERUNMENU	7
#define			_TIMEREPMENU	8
#define			_SAVEREPMENU	9
#define			_INITTICKETMENU	10
#define			_TCPIP			11
#define			_FINDWIFI		12
#define			_MODEWIFI		13
#define			_AJDHCP			14
#define			_AJIP			15
#define			_AJGATE			16
#define			_AJDNS1			17
#define			_AJDNS2			18
#define			_AJMASCARA		19
#define			_AJMAC			20
#define			_AJPASSWORD		21
#define			_AJSSID			22

/*----------------------------------------------------------------------------
 *      Extern functions
 *---------------------------------------------------------------------------*/

extern unsigned char CheckMenu (void);

#endif

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

