/*----------------------------------------------------------------------------
 *      Software:	MIG
 *      Proposito:	Projeto do dispensador de ticket - SISTEMA DE SENHAS 
 *----------------------------------------------------------------------------
 *      Arquivo:    App.h
 *      Proposito: 	Gerencia impress�es senhas e solicita��es dos clientes
 *----------------------------------------------------------------------------
 *      Vers�o: 	2.01 	Prog: Ricardo T.   Data: 06/12/2017
 * 		Descri��o:  Vers�o inicial
 *----------------------------------------------------------------------------
 *      Este c�digo pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#ifndef	__APP__
#define	__APP__

/*----------------------------------------------------------------------------
 *      Definitions
 *---------------------------------------------------------------------------*/
#define	MAXPREV			3			// Tickets anteriores (de 1 a 5) usadas para calcular a previs�o espera 
#define	MAXDEP			13			// M�ximo n�mero de departamentos
#define	MAXUSER			99			// M�ximo n�mero de usu�rios

/*----------------------------------------------------------------------------
 *      Device Model definitions
 *---------------------------------------------------------------------------*/
#define _DISPLAYCALL	1
#define	_DISPLAYMETER	2
#define	_DISPLAYPRICE	3
#define	_PT3D			4
#define	_PT4D			5
#define	_PT5D			6
#define	_TD12			7
#define	_TD16			8
#define	_MIG6			9
#define	_MIG12			10
#define _MCIE			11
#define _IE				12
#define _JM				13
#define _JMTELV			14
#define _JMCOMP			15
#define _JMERI			16
#define	_JMTERM			17
#define _TOPLIGHT		20
#define _TOPMID			21
#define _TOPPLUS		22
#define _WYNET			23
#define _CASCATA		24

/*----------------------------------------------------------------------------
 *      Prog stat machine definitions
 *---------------------------------------------------------------------------*/
#define	_INISTART  		0
#define _INIFILESYSTEM	1
#define	_INIWRITEFS	 	2
#define	_INIREADFS 		3
#define	_INIDATA		4
#define	_INISCR1		5
#define	_INISCR2		6
#define	_INILOGO		7
#define	_INIFILE		8
#define	_INIREPORT		9
#define	_INICOMPLETE	10

/*----------------------------------------------------------------------------
 *      rf codes functions definitions
 *---------------------------------------------------------------------------*/
#define	F_NONE			0
#define	F_NEXT			1
#define	F_REPEAT		2
#define	F_CLOSE			3
#define F_AUX			4

/*----------------------------------------------------------------------------
 *     Remote Control Definitions 
 *---------------------------------------------------------------------------*/
#define	NEXT			0						/* Defini��o do bot�o pr�ximo */
#define	REPEAT			1						/* Defini��o do bot�o repete */
#define	CLOSE			2						/* Defini��o do bot�o encerra */
#define	CANCEL			3						/* Defini��o do bot�o cancela */

/*----------------------------------------------------------------------------
 *      Drive definitions
 *---------------------------------------------------------------------------*/
#define	_FLASHFS		2 
#define	_SDCARD			4

/*----------------------------------------------------------------------------
 *      Update definitions
 *---------------------------------------------------------------------------*/
#define _NOACTION		0
#define _FROMAPP		1
#define _FROMBOOT		2
#define	_UPDERROR		3

/*----------------------------------------------------------------------------
 *		WIFI Mode definitions
 *---------------------------------------------------------------------------*/
#define WIFI_MODE_STA	0
#define WIFI_MODE_AP	1

/*----------------------------------------------------------------------------
 *      Prog stat machine definitions
 *---------------------------------------------------------------------------*/
#define _STATINIT		1
#define	_GETDEV			2
#define	_RECDEV			3
#define _WAITING		4
#define _GETTICKET		5
#define	_RECTICKET		6
#define	_PRINTING		7
#define	_QUEUING		8
#define	_UPDATING		9
#define	_UPDATINGREP	10
#define	_NOTICKET		11
#define _DPTODISABLE	12
#define _POWEROFF		13
#define _UDPPROG		14
#define	_GETMAC			15
#define	_SETIPWIFI		16
#define	_SETDNS1WIFI	17
#define	_SETDNS2WIFI	18
#define _SETMACWIFI		19
#define _SETMODEWIFI	20
#define	_RESUMEWIFI		21
#define _SETCALL		22
#define _RECCALL		23
#define	_SETCARD		24
#define	_RECCARD		25

/*----------------------------------------------------------------------------
 *      Data Prog stat machine definitions
 *---------------------------------------------------------------------------*/
#define	_FLWAIT			0
#define	_FLBUFFER		1
#define	_FLANALYZE		2
#define _FLOPEN			3
#define _FLREAD			4
#define _FLSAVE			5
#define _FLCLOSE		6
#define _FLERROR		7
#define _FLIGNORE		8

/*----------------------------------------------------------------------------
 *      Web requests definitions
 *---------------------------------------------------------------------------*/
#define	REQ_NONE		0
#define	UPD_SCR1		1
#define	UPD_SCR2		2
#define	UPD_LOGO		3
#define	UPD_FIRM		4
#define UPD_NETWORK		5
#define NEW_CALL		6

/*----------------------------------------------------------------------------
 *      Run Mode definitions
 *---------------------------------------------------------------------------*/
#define	_MODESERVER		1
#define	_MODECLIENT		2
#define	_MODEOFFLINE	3

/*----------------------------------------------------------------------------
 *      Call functions definitions
 *---------------------------------------------------------------------------*/
#define	F_NONE			0
#define	F_NEXT			1
#define	F_REPEAT		2
#define	F_CLOSE			3
#define F_AUX			4
#define	F_TICKET		5

/*----------------------------------------------------------------------------
 *      USB Product definitions
 *---------------------------------------------------------------------------*/
#define	USB_TOPL		1
#define	USB_IE			2
#define	USB_MCIE		3
#define	USB_CWU			4
#define	USB_TOPP		5
#define	USB_WYSA		6
#define	USB_SM			7
#define	USB_IMP			8
#define	USB_RCT			9
#define	USB_PMV			10
#define	USB_JM			11
#define	USB_TOPP3		12
#define	USB_TD16		13
#define	USB_TOPM		14
#define	USB_TOPLV2		15
				 
/*----------------------------------------------------------------------------
 *      MIG Key definitions
 *---------------------------------------------------------------------------*/
#define		MIGKEY1				(1<<0)
#define		MIGKEY2				(1<<1)
#define		MIGKEY3				(1<<4)
#define		MIGKEY4				(1<<5)
#define		MIGKEY5				(1<<16)
#define		MIGKEY6				(1<<19)
#define		MIGKEY7				(1<<20)
#define		MIGKEY8				(1<<21)
#define		MIGKEY9				(1<<22)
#define		MIGKEY10			(1<<23)
#define		MIGKEY11			(1<<24)
#define		MIGKEY12			(1<<25)

/*----------------------------------------------------------------------------
 *      Network/Print definitions
 *---------------------------------------------------------------------------*/
#define		TH1		"WYMA"
#define		TH2		"TECNOLOGIA"
#define		TDP		"DEPARTAMENTO "
#define		TF2		"Aguarde sua chamada"
#define		TF3		"no painel eletr�nico"

#define		CLI1	"CAIXAS PREFERENCIAL"
#define		LENCLI1	19
#define		CLI2	"CAIXAS"
#define		LENCLI2	6
#define		CLI3	"CREDI�RIO PREFERENCIAL"
#define		LENCLI3	22
#define		CLI4	"CREDI�RIO"
#define		LENCLI4	9

#define		TIP		"\xC0\xA8\x0\xA0"
#define		TMAS	"\xFF\xFF\xFF\x0"
#define		TDN1	"\x0\x0\x0\x0"
#define		TDN2	"\x0\x0\x0\x0"
#define		TGAT	"\x0\x0\x0\x0"
#define		TMAC	"\x74\xD6\x75\x01\x01\x02"
#define		THNAME	""

#define		WIP		0xA000A8C0
#define		WMAS	0x00FFFFFF
#define		WDN1	0x00000000
#define		WDN2	0x00000000
#define		WGAT	0x00000000
#define		WRAN	0xAA00A8C0

/*----------------------------------------------------------------------------
 *      User type definitions for App config
 *---------------------------------------------------------------------------*/
typedef struct config
{
	unsigned char authEnable;				// Habilita autentica��o
	unsigned char authPassword[11];			// Senha para autentica��o
	unsigned char hostName[16];				// Nome do host (para uso com dhcp)
	unsigned char dhcpStatus;				// Habilita dhcp
	unsigned char localIp[4];				// IP local
	unsigned char ipMask [4];				// Mascara de rede
	unsigned char gateway [4];				// IP de gateway
	unsigned char dns1 [4];					// IP do DNS1
	unsigned char dns2 [4];					// IP do DNS2
	unsigned char macAddress[6];			// Endere�o MAC
	unsigned char driver;					// Driver ativo (FLASH SPI ou SDCARD)
	unsigned char update;					// Dados de atualiza��o de firmware
	unsigned short pagesUpdate;				// Quantidade de p�ginas do firmware.
	unsigned short crcUpdate;				// Calculo CRC dos dados do firmware
	unsigned char id;						// Identifica��o do dispositivo na rede RS485
	unsigned int idBoard;					// Identifica��o da placa (HARDWARE)

	// A partir daqui, s�o dados especificos da aplica��o

											// DADOS DO SISTEMA:
	unsigned char devModel;					// Modelo: 80= 6teclas ; 81= 12 teclas
	unsigned char runMode;	 				// Modos: servidor, cliente ou off-line
	unsigned char ipServer[4];				// Ip do servidor quando conf em cliente
	unsigned char srvItf;					// Interf do servidor: (WIFI UDP ou LAN UDP)
	unsigned char autoTest;					// Controla o auto-teste
	unsigned int  keepLiveTicket;			// Tempo de vida do ticket 
	unsigned char enTimeAuto;				// Habilita configura��o de hor�rio de funcionamento
	unsigned char timeHourOn;				// Configura hora de ligar
	unsigned char timeMinOn;				// Configura minuto de ligar
	unsigned char timeHourOff;				// Configura hora de desligar
	unsigned char timeMinOff;				// COnfigura minuto de desligar
	unsigned char weekDay;					// Configura dia da semana para ligar
	unsigned char timeHourRep;				// Configura hora de gerar relat�rio
	unsigned char timeMinRep;				// Configura minuto de gerar o relat�rio
	unsigned char numTicketForecast;		// N�mero de tickets de 1 a 5 para fazer m�dia de espera
	unsigned char energyStatus;				// Status de energia (0) desligado (1) ligado
											// DADOS DO TICKET:
	unsigned char logoEnable;				// habilita o logo no ticket (default WYMA)
	unsigned char lettEnable;				// Habilita letra no ticket
	unsigned char lenNumTicket;				// Configura comprimento do n�mero no ticket
	unsigned char typeNumTicket;			// Configura o tipo de n�mero no ticket
	unsigned char cut;						// habilita corte total ou parcial
	unsigned char copy;						// Controla o n�mero de c�pias(vias) do ticket
	unsigned char optHead1;					
	char header1 [49];
	unsigned char optHead2;
	char header2 [49];
	unsigned char optHead3;
	char header3 [49];
	unsigned char optFoot1;
	char footer1 [49];
	unsigned char optFoot2;
	char footer2 [49];
	unsigned char optFoot3;
	char footer3 [49];
	unsigned char optFoot4;
	char footer4 [49];
											// DADOS DO DEPARTAMENTO:
	char depAlpha [MAXDEP];
	unsigned char depEnable [MAXDEP];
	unsigned char depUsers [MAXUSER];
	char depName [MAXDEP][49];
	unsigned int depRange [MAXDEP][2];
	unsigned char depCloseNext [MAXDEP];
	unsigned char depCloseRep [MAXDEP];		// Guarda a op��o para fechar atendimentos no relat�rio
	unsigned char depSchPri [MAXDEP];		// Guarda o esquema de chamada dos prefer�nciais.
	unsigned char depHelp  [MAXDEP];		// Guarda o n� dos dpto que recebem ajuda deste.
	unsigned char depSchHlp [MAXDEP];		// Guarda o esquema de ajuda
	unsigned char depIpDisp [MAXDEP][4];	// Indica o IP do display do dpto.
	unsigned char depIpTop [MAXDEP][4];		// Indica o IP do teclado de opini�o do dpto.
	unsigned char depRowDisp [MAXDEP]; 		// Indica qual a linha do display atende o dpto.
	unsigned char depIsBtPref [MAXDEP];		// Emite tickets preferenciais para outro dpto.
	unsigned char userBox [MAXUSER];		// Indica quais s�o os usu�rios do dpto.
	unsigned char depItfDisp [MAXDEP];		// Interf dos displays de dpts: (WIFI UDP ou LAN UDP)
	unsigned char depItfTop [MAXDEP];		// Interf dos Tops de dpts: (WIFI UDP ou LAN UDP)
	unsigned char wifiMode;	 				// Indica o modo de opera��o do wifi: AP ou STA
	unsigned char wifiUser;					// Indica se foi informado um SSID pelo usu�rio
	char wifiSsid[32];						// SSID da rede wifi
	char wifiPw[32];						// Senha da rede wifi
	unsigned char eTop [MAXDEP];			// Habilita envio do IP de pesquisa para o cliente
	unsigned int lastBeforeReport[MAXDEP];	// Guarda ultimo ticket na gera��o do relat�rio
	unsigned char saveReport;				// Indica se deve gravar o arquivo do relat�rio
	unsigned char broadcastInfo;			// Habilita envio de informa��es de filas
	unsigned char wifiChannel;				// Canal do r�dio Wifi (1-13 � default)
	unsigned char wifiSecurity;				// Seguran�a com senha Wifi (0-Open, 1-WPA, 2-WPA2, 3-WEP)
	unsigned char wifiDhcp;					// Habilita dhcp
	unsigned int  wifiIp;					// IP local
	unsigned int  wifiMask;					// Mascara de rede
	unsigned int  wifiGateway;				// IP de gateway
	unsigned int  wifiDns1;					// IP do DNS1
	unsigned int  wifiDns2;					// IP do DNS2
	unsigned char wifiSdhcp;				// Habilita servidor Dhcp
	unsigned int  wifiSdhcpIp;				// Start IP para servidor Dhcp
	unsigned char printProtocol;			// Op��o para imprimir o n�mero do protocolo no ticket
	unsigned char ipSrvProt [4];			// Ip do servidor de protocolo 
	unsigned char depOptionHlp [MAXDEP];	// Op��es do sistema de ajuda
	unsigned char checkTime [MAXDEP];		// Op��o para verificar tempo de espera do usu�rio
	unsigned char optSchHlp [MAXDEP];		// Op��o de esquema de ajuda: Dpto(0) ou Box(1)
 	unsigned char zero;						// Op��o para zerar tickets ao emitir o relat�rio
	unsigned char idRf;						// C�digo para recep��o do RF
	unsigned char ticketOrientation;		// Orienta��o do ticket 0=normal 1=invertido
	unsigned char depCopy [MAXDEP];			// N�mero de vias por departamento
	unsigned char depPref [MAXDEP];			// Atende sempre o outro departamento primeiro
	// V2.10
	unsigned short logoImpWidth;
	unsigned short logoImpHeight;
	unsigned char logoImp [1024];			// Logo da impressora
	//
	unsigned char salesDpto; // N�mero do departamento de vendas
}CFG;

/*----------------------------------------------------------------------------
 *      User type definitions for App Call
 *---------------------------------------------------------------------------*/
typedef struct ticketCall
{
	unsigned char row;
	char alpha;
	unsigned short number;
	unsigned char user;
	unsigned char dep;
	unsigned char func;
}CALL;

/*----------------------------------------------------------------------------
 *      User type definitions for last App Call
 *---------------------------------------------------------------------------*/
typedef struct usercall {
	unsigned char sta; 	// O usu�rio esta habilitado = 1 // desabilitado = 0
	unsigned char run; 	// Esta atendendo = 1  // Encerrou atendimento = 0
	unsigned int time; 	// Hor�rio do �ltimo encerramento
}USERCALL;

/*----------------------------------------------------------------------------
 *      Extern functions
 *---------------------------------------------------------------------------*/
extern void InitClient (void);
extern void InitAppConfig (void);
extern void InitNetConfig (void);

#endif

 /*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
