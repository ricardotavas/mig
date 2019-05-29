/*----------------------------------------------------------------------------
 *      Software:	MIG
 *      Proposito:	Projeto do dispensador de ticket - SISTEMA DE SENHAS 
 *----------------------------------------------------------------------------
 *      Arquivo:    App.c
 *      Proposito: 	Gerencia impressões senhas e solicitações dos clientes
 *----------------------------------------------------------------------------
 *      Versão: 	2.12 	Prog: Ricardo T.   Data: 09/05/2019
 * 		Descrição:  Versão inicial
 *----------------------------------------------------------------------------
 *      Este código pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/
#define			VERSION		"0212"

#include 		<LPC17xx.h>
#include		<Net_Config.h>
#include		"IMP_CONFIG.h"
#include		"UART_CONFIG.h"
#include		"RTC_CONFIG.h"
#include		"SPI_CONFIG.h"
#include		"COMM_CONFIG.h"
#include		"IAP_CONFIG.h"
#include		"PWM.h"
#include		"APP.h"
#include		"QUEUE.h"
#include		"REPORT.h"
#include		"MENU.h"
#include		"TIMER1.h"
#include		"GRAPHICS.h"
#include 		"usb.h"
#include 		"usbcfg.h"
#include 		"usbhw.h"

/*----------------------------------------------------------------------------
 *      COMM.lib  variable
 *---------------------------------------------------------------------------*/
extern unsigned char 	localId;

/*----------------------------------------------------------------------------
 *      HTTP_CGI.c   variables
 *---------------------------------------------------------------------------*/
extern unsigned char	WEBREQUEST;

/*----------------------------------------------------------------------------
 *      QUEUE.c   variables
 *---------------------------------------------------------------------------*/
extern unsigned short	queue [];

/*----------------------------------------------------------------------------
 *      UserComm.c   variables
 *---------------------------------------------------------------------------*/
extern unsigned char	numFrameServer;				// Número do frame recebido do servidor
extern unsigned int 	numTicketServer;			// Número do ticket recebido do servidor
extern char 			forecastServer[];			// Previsão de espera do ticket recebida do servidor
extern char				alphaServer;				// Letra do ticket recebida do servidor
extern unsigned char  	dptoServer;					// Departamento do ticket recebida do servidor
extern char				devNameServer[];			// Nome do dispositivo recebido do servidor
extern unsigned char 	PROGMODE;					// indica se o dispositivo esta em modo programação
extern char 			atdProtocol[];				// Número do protocolo de atendimento do cliente
extern unsigned short	remoteKey;

/*----------------------------------------------------------------------------
 *      FileConfig.c   variables
 *---------------------------------------------------------------------------*/
extern unsigned short  	_DEF_DRIVE;

/*----------------------------------------------------------------------------
 *      NetConfig.c   variables
 *---------------------------------------------------------------------------*/
extern unsigned char  	http_auth_passw[20];
extern unsigned char   	http_EnAuth;
extern unsigned char	own_hw_adr[ETH_ADRLEN];
extern unsigned char    lhost_name[16];
extern LOCALM 			localm[];   
#define LocM   			localm[NETIF_ETH]   		// Local Machine Settings 
#define 				DHCP_TOUT   500            	// DHCP timeout 5 seconds

/*----------------------------------------------------------------------------
 *      DATAFLASH.c   variables
 *---------------------------------------------------------------------------*/
extern unsigned char STATFILE;

/*----------------------------------------------------------------------------
 *      REPORT.c   variables
 *---------------------------------------------------------------------------*/
extern unsigned char BUFFER_TICKET [];

/*----------------------------------------------------------------------------
 *      Local variables
 *---------------------------------------------------------------------------*/
char 			devName[6] = "MIG6 \x0";
unsigned char 	versionFirm[4] = VERSION;
unsigned char	programMode;
unsigned int	tick;
unsigned int	dhcp_tout;
ATD				atend;
IMGPRN 			pticket = {{0},0,0};
TICKET 			formatTicket;
int 			erroPrint=0;
unsigned char	STATPROG=0;
unsigned char 	ethExist=0;
unsigned char	TIMETESTING=0;
unsigned char	POWERON=0;
unsigned char	POWEROFF=0;
unsigned char	DOREPORT=0;
unsigned char	RESTARTSOFT=0;
unsigned char	PAPERWARNING=0;
unsigned char	PAPEREND=0;
unsigned char 	numFrame=0;

unsigned char 	UserFont[8][8];

unsigned char 	runRow;
char			runAlpha;
unsigned short	runTicket;
unsigned char	runType;
unsigned char	runUser;
unsigned char 	runDep;
unsigned char	runFunc;
CALL			call[10]= 	{	{0,0,0,0,0,0},			// Chamadas HTTP na fila
								{0,0,0,0,0,0},
								{0,0,0,0,0,0},
								{0,0,0,0,0,0},
								{0,0,0,0,0,0},
								{0,0,0,0,0,0},
								{0,0,0,0,0,0},
								{0,0,0,0,0,0},
								{0,0,0,0,0,0},
								{0,0,0,0,0,0}};

unsigned short lastTicket[99] ={0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0};

char lastDep[99] =			   {0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0};

char lastAlpha[99] =		   {0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0};

char lastAlphaDep[12]=		   {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
unsigned short lastTicketDep[12]={0,0,0,0,0,0,0,0,0,0,0,0};
char lastUser[12]=			   {0,0,0,0,0,0,0,0,0,0,0,0};

/*----------------------------------------------------------------------------
 *      Local functions
 *---------------------------------------------------------------------------*/
void InitCasasBahia (void);
void InitNetConfig (void);
void InitAppConfig (void);
void InitAppConfig_V02_R10 (void);
void CheckConfig (void);

void init_socket (void);
void timer_poll (void);
static void dhcp_check (void);
int sendchar (int ch);
int getkey (void);
void spi_init (void);
int fs_spi_Init (void);
int fs_spi_EraseSector (unsigned int iniAddr);
int fs_spi_ProgramPage (unsigned int iniAddr, unsigned int size, unsigned char *pontbuf);
int fs_spi_ReadData (unsigned int iniAddr, unsigned int size, unsigned char *pontbuf);
char GetIPServer (unsigned char itf);
void Send_GetDeviceName (unsigned char itf, unsigned char retry);
void Send_GetNumTicket (unsigned char itf, unsigned char retry, unsigned char index);
void Send_SetInfoAtd (unsigned char itf, unsigned char retry);
void Send_SetProtocolo (unsigned char itf, unsigned char retry);
void Send_SetNumTicket(unsigned char itf, unsigned char retry, unsigned char eTop, 
						unsigned char *ipTop, unsigned char itfTop,	char *depName);
void Beep(void);
void ShowForecast (unsigned char depNum, char *fcast);
char Print_Ticket (unsigned char depNum, char alphaNum, unsigned int number, char *txtPriority, char *strForecast);
char Check_DepTicket (unsigned char depNum, unsigned char type);
char Process_Queue (unsigned short key);
void Write_Flash (void);

/*----------------------------------------------------------------------------
 *    Extern Functions 
 *---------------------------------------------------------------------------*/
extern void New_Call (void);

/*----------------------------------------------------------------------------
 * Funções:		InitClient
 * Descrição: 	Inicializa configurações do cliente
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void InitClient (void)
{
	// Insira aqui a inicialização especial do cliente
	// Se tiver logotipo edite a função "initFilesClient()" em report.c
	// modificando também os arquivos em client.h com as logotipos do cliente.
}

void InitAppConfig_V02_R10 (void)
{
	unsigned int x;
	cfg.logoImpWidth=0;
	cfg.logoImpHeight=0;
	for (x=0 ; x<1024 ; x++)		 						// Inicializa logos
	{
		cfg.logoImp	[x] = 0;
	}
}

/*----------------------------------------------------------------------------
 * Funções:		InitAppConfig
 * Descrição: 	Inicializa configurações do software
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void InitAppConfig (void)
{
	unsigned char i;
	unsigned int x;
	cfg.authEnable=0;									// Habilita autenticação http
	cfg.devModel = _MIG6;								// Modelo 6 teclas								
	cfg.runMode=_MODESERVER;							// Modo de funcionamento SERVER
	cfg.ipServer[0]=0;									// Endereço IP do servidor de tickets,
	cfg.ipServer[1]=0;									// usado no modo cliente
	cfg.ipServer[2]=0;
	cfg.ipServer[3]=0;
	cfg.srvItf=ITF_UDP;									// Interface do servidor de tickets
	cfg.autoTest=0;										// Desliga o auto-teste
	cfg.keepLiveTicket=3600;							// Tempo de vida do ticket = 8 horas
	cfg.enTimeAuto=1;									// Habilita desligamento automático
	cfg.timeHourOn=7;									// Inicializa horário para ligar sistema:
	cfg.timeMinOn=0;									// 7:00
	cfg.timeHourOff=20;									// Inicializa hora para desligar sistema:
	cfg.timeMinOff=0;									// 20:00
	cfg.weekDay=0x3E;									// Dias da semana = Segunda a Sexta (x011-1110)
	cfg.timeHourRep=19;									// Configura hora de gerar relatório:
	cfg.timeMinRep=30;									// 19:30
	cfg.numTicketForecast=3;							// Conf. 3 tickets para fazer média de espera
	cfg.energyStatus=1;									// Status de energia = ligado
	cfg.logoEnable=DISABLE;								// Desabilita logo
	cfg.lettEnable=ENABLE;								// Habilita letra no ticket
	cfg.lenNumTicket=3;									// Número do ticket com 3 dígitos
	cfg.typeNumTicket=2;								// Configura o tipo do número do ticket (0-normal; 1-expandido; 2-gráfico)
	cfg.cut=CUTPARTIAL;									// Corte parcial do papel
	cfg.copy=1;											// Imprime somente 1 via do ticket
	cfg.optHead1=MSGSTANDARD;							// Inicializa tipo de mensagem do header1
	for (i=0; i<49 ;i++) cfg.header1[i]=0;				// Apaga mensagem do header1
	for (i=0; i<4 ;i++) cfg.header1[i]=TH1[i];			// Inicializa nome do header1
	cfg.optHead2=MSGSTANDARD;							// Inicializa tipo de mensagem do header2
	for (i=0; i<49; i++) cfg.header2[i]=0;				// Apaga mensagem do header2
	for (i=0; i<10 ;i++) cfg.header2[i]=TH2[i];			// Inicializa nome do header2
	cfg.optHead3=MSGSTANDARD;							// Inicializa tipo de mensagem do header3
	for (i=0; i<49; i++) cfg.header3[i]=0;				// Apaga mensagem do header3
	cfg.optFoot1=MSGFORECAST;							// Inicializa tipo de mensagem do footer1
	for (i=0; i<49; i++) cfg.footer1[i]=0;				// Apaga mensagem do footer1
	cfg.optFoot2=MSGSTANDARD;							// Inicializa tipo de mensagem do footer2
	for (i=0; i<49; i++) cfg.footer2[i]=0;				// Apaga mensagem do footer2
	for (i=0; i<19 ;i++) cfg.footer2[i]=TF2[i];			// Inicializa nome do footer2
	cfg.optFoot3=MSGSTANDARD;							// Inicializa tipo de mensagem do footer3
	for (i=0; i<49; i++) cfg.footer3[i]=0;				// Apaga mensagem do footer3
	for (i=0; i<20 ;i++) cfg.footer3[i]=TF3[i];			// Inicializa nome do footer3
	cfg.optFoot4=MSGDATETIME;							// Inicializa tipo de mensagem do footer4
	for (i=0; i<49; i++) cfg.footer4[i]=0;				// Apaga mensagem do footer4

	for (i=0 ; i<MAXUSER ; i++)
	{
		cfg.depUsers[i]=0;
		cfg.userBox[i]=i+1;
	}
 
	for (i=0; i<MAXDEP ; i++)
	{
		cfg.depEnable[i]=1;								// Habilita todos departamentos
		for (x=0; x<49 ;x++) cfg.depName[i][x]=0;		// Apaga nome do departamento
		for (x=0; x<13 ;x++) 
			cfg.depName[i][x]=TDP[x];					// Inicializa nome do departamento
		if (i<9)
			cfg.depName[i][x]=i+49;						// Número do departamento
		else
		{		  	   
			cfg.depName[i][x++]='1';					// Número do departamento
			cfg.depName[i][x]=((i+1)%10)+48;			// Número do departamento
		}
				
		cfg.depAlpha[i]=i+65;							// Grava letras iniciando em 'A'
		cfg.depRange[i][0]=(i*1000)+1;					// Inicializa número inicial da faixa de senhas
		cfg.depRange[i][1]=(i+1)*1000;					// inicializa número final da faixa de senhas
		cfg.depCloseNext[i]=1;							// Habilita encerramento auto. na próx. chamada
		cfg.depCloseRep[i]=0;							// Habilita encerramento auto. no relatório
		cfg.depSchPri[i]=0x31;							// Inicializa esquema (Normal X Pref): 3x1
		cfg.depHelp[i]=0x00;							// Inicializa ajuda: ajuda maior fila
		cfg.depSchHlp[i]=0x51;							// Inicializa esquema de ajuda: (localXajuda)5x1
		cfg.depUsers[i]=i+1;							// Inicializa um usuário por departamento
		cfg.depIpDisp[i][0]=0x00;						// IP do display, qdo usado acionador RF 900MHz,
		cfg.depIpDisp[i][1]=0x00;						// TD16 ou outro dispositivo de chamada smart..
		cfg.depIpDisp[i][2]=0x00;						// 
		cfg.depIpDisp[i][3]=0x00;						// 
		cfg.depIpTop[i][0]=0x00;						// IP do TOP, qdo usado acionador RF 900MHz,
		cfg.depIpTop[i][1]=0x00;						// TD16 ou outro dispositivo de chamada smart..
		cfg.depIpTop[i][2]=0x00;						// 
		cfg.depIpTop[i][3]=0x00;						// 
		cfg.depRowDisp[i]=1;							// Linha 1 do display
		cfg.depItfDisp[i]=ITF_UDP;						// Default interface LAN UDP
		cfg.depItfTop[i]=ITF_UDP;						// Default interface LAN UDP
		cfg.eTop[i]=0;									// Default Top desabilitado
		cfg.lastBeforeReport[i]=0;						// Último ticket na geração do relatório.
	}
	
	cfg.saveReport=1;										// Indica se deve salvar o relatório
	cfg.broadcastInfo=0;									// Indica se deve salvar o relatório

	cfg.printProtocol=0;									// Desabilitado protocolo
	cfg.ipSrvProt[0]=0;
	cfg.ipSrvProt[1]=0;
	cfg.ipSrvProt[2]=0;
	cfg.ipSrvProt[3]=0;

	for (i=0; i<MAXDEP ; i++)
		cfg.depOptionHlp[i]=0;								// Opções de ajuda desabilitadas
						  
	for (i=0; i<MAXDEP ; i++)
		cfg.checkTime[i]=0;									// Opções de ajuda desabilitadas

	for (i=0; i<MAXDEP ; i++)
		cfg.optSchHlp[i]=0;									// Opções de ajuda desabilitadas

	cfg.zero=0;
	cfg.idRf=0;
	cfg.ticketOrientation=0;

	for (i=0; i<MAXDEP ; i++)
	{
		cfg.depCopy[i]=1;									// Imprime 1 via 
	}

	InitAppConfig_V02_R10();

	cfg.salesDpto=0;
}

/*----------------------------------------------------------------------------
 * Funções:		InitNetConfig
 * Descrição: 	Inicializa configurações de rede
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void InitNetConfig (void)
{
	unsigned char i;
	cfg.authEnable=0;										// Habilita autenticação http
	for (i=0; i<11 ;i++) cfg.authPassword[i]=0;				// Apaga password http
	for (i=0; i<16 ;i++) cfg.hostName[i]=0;					// Apaga host name
	for (i=0; i<7  ;i++) cfg.hostName[i]=THNAME[i];			// Inicializa host name
	cfg.dhcpStatus=1;										// Liga DHCP
	for (i=0; i<4 ;i++) cfg.localIp[i]=TIP[i];				// IP default
	for (i=0; i<4 ;i++) cfg.ipMask[i]=TMAS[i];				// Mascara default
	for (i=0; i<4 ;i++) cfg.gateway[i]=TGAT[i];				// Gateway default
	for (i=0; i<4 ;i++) cfg.dns1[i]=TDN1[i];				// DNS1 default
	for (i=0; i<4 ;i++) cfg.dns2[i]=TDN2[i];				// DMS2 default
	cfg.driver=2;											// Configura driver na Flash SPI
	cfg.update=_NOACTION;									// Desabilita flag de atualização.
	cfg.pagesUpdate=0;										// Quantidade de páginas gravadas.
	cfg.crcUpdate=0;										// Zera cálculo de CRC do firmware
	cfg.id=3;	  											// id RS485 default de display
}

/*----------------------------------------------------------------------------
 * Funções:		CheckConfig
 * Descrição: 	Verifica integridade da configuração, inicializa no 1º uso
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void CheckConfig (void)
{
	unsigned int i;
	unsigned char save=0;

	if (cfg.authEnable==0xFF)								// Verifica se a FLASH esta sem programação
	{
		for (i=0; i<6 ;i++) cfg.macAddress[i]=TMAC[i];		// Inicializa MacAddress
		cfg.idBoard=0;										// Inicializa identificação da placa (hardware)
		InitNetConfig ();									// Inicializa configurações de rede
		save=1;
	}
	if (cfg.devModel==0xFF || cfg.devModel != _MIG6)		// Verifica se a FLASH esta sem programação
	{
		InitAppConfig ();									// Inicializa configurações da aplicação
		save=1;
	}
	if (cfg.logoImpWidth>128)
	{
		InitAppConfig_V02_R10();							// Inicializa configurações da V1 R13
		save=1;
	}

	if (save)
	{
		Write_Flash ();
	}
}

/*----------------------------------------------------------------------------
 * Funções:		Write_Flash
 * Descrição: 	Salva configuração em Flash
 * Parâmetros: 	nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void Write_Flash (void)
{
	WriteConfig (INI_SECTOR_CONFIG,							// Grava configurações em FLASH
					(unsigned char *) 
					&cfg,sizeof(cfg));
}

/*----------------------------------------------------------------------------
 * Funções:		init_socket
 * Descrição: 	Inicializa o socket UDP
 * Parâmetros: 	nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void init_socket (void)
{
	if (socket_udp == 0)
	{
		socket_udp = udp_get_socket (0, UDP_OPT_SEND_CS | UDP_OPT_CHK_CS, udp_callback);
		if (socket_udp != 0) 
		{
	   		udp_open (socket_udp, 25502); 
		}
	}
}

/*----------------------------------------------------------------------------
 * Funções:		timer_poll
 * Descrição: 	Verifica o timer para setar flag Tick
 * Parâmetros: 	nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void timer_poll (void) 
{
	if (SysTick->CTRL & 0x10000) 
	{
    	timer_tick ();										// Timer tick every 100 ms
    	tick = 1;
  	}
}

/*----------------------------------------------------------------------------
 * Funções:		dhcp_check
 * Descrição: 	Verifica se existe servidor dhcp para fornecer o IP
 * Parâmetros: 	nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
static void dhcp_check (void) 
{
  	if (dhcp_tout == 0) 
  	{
	    return;
  	}

  	if (mem_test (&LocM.IpAdr, 0, IP_ADRLEN) == __FALSE && !(dhcp_tout & 0x80000000)) 
	{
    	dhcp_tout = 0;										// Success, DHCP has already got the IP address.
    	return;
  	}
  	if (--dhcp_tout == 0)
	{
    	dhcp_disable ();									// A timeout, disable DHCP and use static IP address.
    	dhcp_tout = 30 | 0x80000000;
    	return;
  	}
  	if (dhcp_tout == 0x80000000)
	{
	    dhcp_tout = 0;
  	}
}

/*-------------------------------------------------------------------------------------------
 * Função: 		Test_Beep
 * Descrição: 	Faz o teste do beep
 * Parâmetros: 	Nenhum
 * Retorno: 	Resultado (0) falha (1) Sucesso
-------------------------------------------------------------------------------------------*/
char Test_Beep (void)
{
	unsigned char i;
	for (i=0;i<3;i++)
		Beep();
	return 2;
}

/*-------------------------------------------------------------------------------------------
 * Função: 		Test_Flash
 * Descrição: 	Faz o teste do flash
 * Parâmetros: 	Nenhum
 * Retorno: 	Resultado (0) falha (1) Sucesso
-------------------------------------------------------------------------------------------*/
char Test_Flash (void)
{
	unsigned int bytewr=123456789;
	unsigned int byterd=0;									
	spi_EraseSector(1);										// Formata setor 1
	spi_Flash_Write ((unsigned char *)&bytewr, 10000, 4);	// Grava a partir do end. inicial do setor 1
	spi_Flash_Read ((unsigned char *)&byterd, 10000, 4);	// Lê a informação gravada
	if (bytewr==byterd)										// Verifica se gravou corretamente
	{
		spi_EraseSector(1);									// Formata setor 1 antes de sair
		return 1;
	}
	else
		return 0;
}

/*-------------------------------------------------------------------------------------------
 * Função: 		Test_Uart
 * Descrição: 	Faz o teste da UART com loopBack
 * Parâmetros: 	Uart escolhida para o teste 0 ou 1
 * Retorno: 	Resultado (0) falha (1) Sucesso
-------------------------------------------------------------------------------------------*/
char Test_Uart (unsigned char uart)
{
	char result=0;
	UART_PutChar (uart, 'A');
	result=(char)UART_GetChar(uart);
	if (result=='A')
		return 1;
	return 0;
}

/*-------------------------------------------------------------------------------------------
 * Função: 		Test_FlashFs
 * Descrição: 	Faz o teste da flash FS
 * Parâmetros: 	Nenhum
 * Retorno: 	Resultado (0) falha (1) Sucesso
-------------------------------------------------------------------------------------------*/
char Test_FlashFs (void)
{
	unsigned int bytewr=123456789;
	unsigned int byterd=0;
	spi_FlashFs_EraseSector(1);										// Formata setor 1
	spi_FlashFs_ProgramPage ((unsigned char *)&bytewr, 10000, 4);  	// Grava a partir do end. do setor 1
	spi_FlashFs_ReadData ((unsigned char *)&byterd, 10000, 4);		// Lê a informação gravada
	if (bytewr==byterd)												// Verifica se gravou corretamente
	{
		spi_FlashFs_EraseSector(1);									// Formata setor 1 antes de sair
		return 1;
	}
	else
		return 0;
}

/*----------------------------------------------------------------------------
 * Funções:		sendchar
 * Descrição: 	A dummy function for 'retarget.c'
 -----------------------------------------------------------------------------*/
int sendchar (int ch) {	return (ch); }

/*----------------------------------------------------------------------------
 * Funções:		sendchar
 * Descrição: 	A dummy function for 'retarget.c'
 -----------------------------------------------------------------------------*/
int getkey (void) { return 0; }

/*-------------------------------------------------------------------------------------------
 * Função: 		spi_Configuration
 * Descrição: 	Chamado por FIle_Config.h para inicializar pinos do SDCARD,
 *				isto ja foi feito com init_spi da LIB WYMA.
 -------------------------------------------------------------------------------------------*/
void spi_init (void)	{}

/*-------------------------------------------------------------------------------------------
 * Função: 		fs_spi_Init
 * Descrição: 	Chamado por FIle_Config.h para inicializar pinos do FlashFs SPI,
 *				direciona para função da LIB WYMA
 -------------------------------------------------------------------------------------------*/
int fs_spi_Init (void)
{
	return spi_FlashFs_Init ();
}

/*-------------------------------------------------------------------------------------------
 * Função: 		fs_spi_EraseSector
 * Descrição: 	Chamado por FIle_Config.h para apagar setor da FlashFs SPI,
 *				direciona para função da LIB WYMA
 -------------------------------------------------------------------------------------------*/
int fs_spi_EraseSector (unsigned int iniAddr)
{
	return spi_FlashFs_EraseSector (iniAddr);
}

/*-------------------------------------------------------------------------------------------
 * Função: 		fs_spi_Init
 * Descrição: 	Chamado por FIle_Config.h para gravar na FlashFs SPI,
 *				direciona para função da LIB WYMA
 -------------------------------------------------------------------------------------------*/
int fs_spi_ProgramPage (unsigned int iniAddr, unsigned int size, unsigned char *pontbuf)
{
	return spi_FlashFs_ProgramPage (pontbuf, iniAddr, size);
}

/*-------------------------------------------------------------------------------------------
 * Função: 		fs_spi_Init
 * Descrição: 	Chamado por FIle_Config.h para ler a FlashFs SPI,
 *				direciona para função da LIB WYMA
 -------------------------------------------------------------------------------------------*/
int fs_spi_ReadData (unsigned int iniAddr, unsigned int size, unsigned char *pontbuf)
{
	return spi_FlashFs_ReadData (pontbuf, iniAddr, size);
}

/*----------------------------------------------------------------------------
 * Funções:		GetIPServer
 * Descrição: 	Recupera o IP configurado do servidor de tickets
 * Parâmetros: 	itf - Interface de comunicação
 * Retorno: 	(0) = Não configurou IP de destino   (1) = OK
 -----------------------------------------------------------------------------*/
char GetIPServer (unsigned char itf)
{
	if (itf==ITF_UDP)
	{
		remPortUdp = 25502;								// Porta default
		remIpUdp[0] = cfg.ipServer[0];					// IP do servidor de tickets
		remIpUdp[1] = cfg.ipServer[1];
		remIpUdp[2] = cfg.ipServer[2];
		remIpUdp[3] = cfg.ipServer[3];
	}
	else
		return 0;

	return 1;
}

/*----------------------------------------------------------------------------
 * Funções:		GetIPDpto
 * Descrição: 	Recupera o IP configurado do painel do departamento
 * Parâmetros: 	dpto - Número do departamento
 * Retorno: 	(0) = Não configurou IP de destino   (1) = OK
 -----------------------------------------------------------------------------*/
char GetIPDpto (unsigned char dpto)
{
	if (cfg.depItfDisp[dpto-1]==ITF_UDP)
	{
		remPortUdp = 25502;								// Porta default
		remIpUdp[0] = cfg.depIpDisp[dpto-1][0];			// IP do servidor de tickets
		remIpUdp[1] = cfg.depIpDisp[dpto-1][1];
		remIpUdp[2] = cfg.depIpDisp[dpto-1][2];
		remIpUdp[3] = cfg.depIpDisp[dpto-1][3];
	}
	else
		return 0;

	return 1;
}

/*----------------------------------------------------------------------------
 * Funções:		Send_GetDeviceName
 * Descrição: 	Envia comando GET para buscar o nome do dispositivo
 * Parâmetros: 	itf - Interface de comunicação
 *				retry - Flag indicando re-tentativa de envio de pacote
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void Send_GetDeviceName (unsigned char itf, unsigned char retry)
{
	unsigned char remoteClass=CLS_EMISSORSENHA;			// Outro dispositivo é impressora
	unsigned char remoteId=255;							// Não precisa do ID esta usando IP

	if (GetIPServer (itf)==0)
		return;
	numFrame=Send_GetRequest (itf, LOCALCLASS, localId, remoteClass, 
								remoteId, 1, retry, COMM_DEVICE, _DEVNAME, 0, 0);
}

/*----------------------------------------------------------------------------
 * Funções:		Send_GetNumTicket
 * Descrição: 	Envia comando numTicket com "Get Ticket Number"
 * Parâmetros: 	itf - Interface de comunicação
 *				retry - Flag indicando re-tentativa de envio de pacote
 *				index - número do cliente/acionador
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void Send_GetNumTicket (unsigned char itf, unsigned char retry, unsigned char index)
{
	unsigned char remoteClass=CLS_EMISSORSENHA;			// Outro dispositivo é impressora
	unsigned char remoteId=255;							// Não precisa do ID esta usando IP

	if (GetIPServer (itf)==0)
		return;
	numFrame=Send_GetRequest (itf, LOCALCLASS, localId, remoteClass, 
								remoteId, 1, retry, COMM_TICKET, _TICKETNEW, index, 0);
}

/*----------------------------------------------------------------------------
 * Funções:		Send_SetInfoAtd
 * Descrição: 	Envia comando com as informações dos atendimentos
 * Parâmetros:	itf - Interface de comunicação
 *				retry - Flag indicando re-tentativa de envio de pacote
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void Send_SetInfoAtd (unsigned char itf, unsigned char retry)
{
	unsigned char remoteClass=255;
	unsigned char remoteId=255;							// Não precisa do ID esta usando IP
	unsigned char bufSend[48];
	unsigned short temp;

	bufSend[0]=queue[0];
	bufSend[1]=(queue[0]>>8);							// Fila do atendimento = 2 bytes
	temp = CalcForecast(1);								// Calcula previsão do atend.
	bufSend[2]=temp;
	bufSend[3]=temp>>8;									// Previsão do atendimento = 2 bytes

	bufSend[4]=queue[1];
	bufSend[5]=(queue[1]>>8);							// Fila do atendimento = 2 bytes
	temp = CalcForecast(2);								// Calcula previsão do atend.
	bufSend[6]=temp;
	bufSend[7]=temp>>8;									// Previsão do atendimento = 2 bytes

	bufSend[8]=queue[2];
	bufSend[9]=(queue[2]>>8);							// Fila do atendimento = 2 bytes
	temp = CalcForecast(3);								// Calcula previsão do atend.
	bufSend[10]=temp;
	bufSend[11]=temp>>8;								// Previsão do atendimento = 2 bytes

	bufSend[12]=queue[3];
	bufSend[13]=(queue[3]>>8);							// Fila do atendimento = 2 bytes
	temp = CalcForecast(4);								// Calcula previsão do atend.
	bufSend[14]=temp;
	bufSend[15]=temp>>8;								// Previsão do atendimento = 2 bytes

	bufSend[16]=queue[4];
	bufSend[17]=(queue[4]>>8);							// Fila do atendimento = 2 bytes
	temp = CalcForecast(5);								// Calcula previsão do atend.
	bufSend[18]=temp;
	bufSend[19]=temp>>8;								// Previsão do atendimento = 2 bytes

	bufSend[20]=queue[5];
	bufSend[21]=(queue[5]>>8);							// Fila do atendimento = 2 bytes
	temp = CalcForecast(6);								// Calcula previsão do atend.
	bufSend[22]=temp;
	bufSend[23]=temp>>8;								// Previsão do atendimento = 2 bytes

	bufSend[24]=queue[6];
	bufSend[25]=(queue[6]>>8);							// Fila do atendimento = 2 bytes
	temp = CalcForecast(7);								// Calcula previsão do atend.
	bufSend[26]=temp;
	bufSend[27]=temp>>8;								// Previsão do atendimento = 2 bytes

	bufSend[28]=queue[7];
	bufSend[29]=(queue[7]>>8);							// Fila do atendimento = 2 bytes
	temp = CalcForecast(8);								// Calcula previsão do atend.
	bufSend[30]=temp;
	bufSend[31]=temp>>8;								// Previsão do atendimento = 2 bytes

	bufSend[32]=queue[8];
	bufSend[33]=(queue[8]>>8);							// Fila do atendimento = 2 bytes
	temp = CalcForecast(9);								// Calcula previsão do atend.
	bufSend[34]=temp;
	bufSend[35]=temp>>8;								// Previsão do atendimento = 2 bytes

	bufSend[36]=queue[9];
	bufSend[37]=(queue[9]>>8);							// Fila do atendimento = 2 bytes
	temp = CalcForecast(10);							// Calcula previsão do atend.
	bufSend[38]=temp;
	bufSend[39]=temp>>8;								// Previsão do atendimento = 2 bytes

	bufSend[40]=queue[10];
	bufSend[41]=(queue[10]>>8);							// Fila do atendimento = 2 bytes
	temp = CalcForecast(11);							// Calcula previsão do atend.
	bufSend[42]=temp;
	bufSend[43]=temp>>8;								// Previsão do atendimento = 2 bytes

	bufSend[44]=queue[11];
	bufSend[45]=(queue[11]>>8);							// Fila do atendimento = 2 bytes
	temp = CalcForecast(12);							// Calcula previsão do atend.
	bufSend[46]=temp;
	bufSend[47]=temp>>8;								// Previsão do atendimento = 2 bytes

	if (itf==ITF_UDP)
	{
		remPortUdp = 25501;								// Porta default para broadcast
		remIpUdp[0]=localm[NETIF_ETH].IpAdr[0] | ~localm[NETIF_ETH].NetMask[0];
		remIpUdp[1]=localm[NETIF_ETH].IpAdr[1] | ~localm[NETIF_ETH].NetMask[1];
		remIpUdp[2]=localm[NETIF_ETH].IpAdr[2] | ~localm[NETIF_ETH].NetMask[2];
		remIpUdp[3]=localm[NETIF_ETH].IpAdr[3] | ~localm[NETIF_ETH].NetMask[3];
	}

	numFrame=Send_SetRequest (itf, LOCALCLASS, localId, remoteClass, remoteId, 1, retry, COMM_TICKET, _INFOATD, 0, 48, bufSend);
}

/*----------------------------------------------------------------------------
 * Funções:		Send_SetNumTicket
 * Descrição: 	Envia comando "Set Ticket Number" para mostrar ticket no painel
 * Parâmetros:	itf - Interface de comunicação
 *				retry - Flag indicando re-tentativa de envio de pacote
 *				eTop - Indica se a pesquisa esta habilitada
 *				*ipTop - Ip do top
 *				itftop - Interface do Top
 *				*depName - Nome do departamento
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void Send_SetNumTicket(unsigned char itf, unsigned char retry, unsigned char eTop, 
						unsigned char *ipTop, unsigned char itfTop,	char *depName)
{
	unsigned char remoteClass=255;							// Outro dispositivo é display de chamada
	unsigned char remoteId=255;								// Não precisa do ID esta usando IP
	unsigned char bufSend[47];
	unsigned char nBytes=0;
	unsigned char i;

	bufSend[nBytes++]= runRow;								// Número da linha do display
	bufSend[nBytes++]= runAlpha;							// Letra do ticket
	bufSend[nBytes++]= cfg.userBox[runUser-1];				// Número do box
	bufSend[nBytes++]= (unsigned char)(runTicket&0xFF);		// Número do ticket byte 1
	bufSend[nBytes++]= (unsigned char)((runTicket>>8)&0xFF);// Número do ticket byte 2
	bufSend[nBytes++]= runType;								// Tipo de ticket...
	if (eTop)
	{
		bufSend[nBytes++]= *ipTop++;						// IP do TOP byte 1
		bufSend[nBytes++]= *ipTop++;						// IP do TOP byte 2
		bufSend[nBytes++]= *ipTop++;						// IP do TOP byte 3
		bufSend[nBytes++]= *ipTop++;						// IP do TOP byte 4
		bufSend[nBytes++]= itfTop;							// Interface do Top
	}
	else
	{
		bufSend[nBytes++]= 0;								// IP do TOP byte 1
		bufSend[nBytes++]= 0;								// IP do TOP byte 2
		bufSend[nBytes++]= 0;								// IP do TOP byte 3
		bufSend[nBytes++]= 0;								// IP do TOP byte 4
		bufSend[nBytes++]= 0;								// Interface do Top
	}
	for (i=0 ;i<36 && *depName!=0 ;i++)						// Nome do departamento
		bufSend[nBytes++] = *depName++;
			
	if (GetIPDpto (cfg.depUsers[runUser-1]))
		numFrame=Send_SetRequest(cfg.depItfDisp[0], LOCALCLASS, 
							localId, remoteClass, remoteId, 1, 
							retry, COMM_TICKET, _TICKETNUM, 
							runUser, nBytes, bufSend);
}

/*----------------------------------------------------------------------------
 * Funções:		Send_SetProtocolo
 * Descrição: 	Envia comando de requisição de número de protocolo
 * Parâmetros: 	itf - Interface de comunicação
 *				retry - Flag indicando re-tentativa de envio de pacote
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void Send_SetProtocolo (unsigned char itf, unsigned char retry)
{
	unsigned char 	remoteClass=255;
	unsigned char 	remoteId=255;						// Não precisa do ID esta usando IP
	char 			bufSend[76]="ESTAS SAO AS INFORMACOES LIDAS A PARTIR DO "
								"CARTAO MAGNETTICO DO CLIENTE....";

	if (itf==ITF_UDP)
	{
		remPortUdp = 25501;								// Porta default para envio
		remIpUdp[0] = cfg.ipSrvProt[0]; 					
		remIpUdp[1] = cfg.ipSrvProt[1];
		remIpUdp[2] = cfg.ipSrvProt[2];
		remIpUdp[3] = cfg.ipSrvProt[3];
	}

	// Apaga informação de número de protocolo anterior
	atdProtocol[0]='\x0';
	// Solicita novo número de protocolo
	numFrame=Send_SetRequest (itf, LOCALCLASS, localId, remoteClass,
								remoteId, 1, retry, COMM_MIG, _REQPROTOCOL, 
								0, 76, (unsigned char *)bufSend);
}

/*----------------------------------------------------------------------------
 * Funções:		Beep
 * Descrição: 	Emite som de beep
 * Parâmetros: 	nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void Beep(void)
{
	unsigned int i=0;

	PWM_Beep(1);
	for (i=0 ; i<2000000 ; i++);
	PWM_Beep(0);
}

/*----------------------------------------------------------------------------
 * Funções:		ShowForecast
 * Descrição: 	Calcula previsão de espera para mostrar no display
 * Parâmetros: 	depNum - Número do departamento
 *				*fcast - Ponteiro para string de destino
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void ShowForecast (unsigned char depNum, char *fcast)
{
	unsigned short tForecast=0;
	tForecast = CalcForecast (depNum);					// Calcula previsão de espera para a banda
	*fcast++=(char)((((tForecast/60)/10)%10)|0x30);		// Dezena da hora
	*fcast++=(char) (((tForecast/60) 	 %10)|0x30);	// Unidade da hora
	*fcast++=':';
	*fcast++=(char)((((tForecast%60)/10)%10)|0x30);		// Dezena do minuto
	*fcast++=(char) (((tForecast%60)	 %10)|0x30);	// Unidade do minuto
	*fcast++='\x0';
}

/*----------------------------------------------------------------------------
 * Funções:		searchTable
 * Descrição: 	Procura tabela do caractere recebido e devolve ponteiro
 * Parâmetros: 	value - Caractere 
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
unsigned char* searchTable (char value)
{
  	switch (value)
	{
	 	case 'A':	return cfg.ticketOrientation==0?(unsigned char *)print_A.bitmap:(unsigned char *)_print_A.bitmap;
		case 'B':	return cfg.ticketOrientation==0?(unsigned char *)print_B.bitmap:(unsigned char *)_print_B.bitmap;
		case 'C':	return cfg.ticketOrientation==0?(unsigned char *)print_C.bitmap:(unsigned char *)_print_C.bitmap;
		case 'D':	return cfg.ticketOrientation==0?(unsigned char *)print_D.bitmap:(unsigned char *)_print_D.bitmap;
		case 'E':	return cfg.ticketOrientation==0?(unsigned char *)print_E.bitmap:(unsigned char *)_print_E.bitmap;
		case 'F':	return cfg.ticketOrientation==0?(unsigned char *)print_F.bitmap:(unsigned char *)_print_F.bitmap;
		case 'G':	return cfg.ticketOrientation==0?(unsigned char *)print_G.bitmap:(unsigned char *)_print_G.bitmap;
		case 'H':	return cfg.ticketOrientation==0?(unsigned char *)print_H.bitmap:(unsigned char *)_print_H.bitmap;
		case 'I':	return cfg.ticketOrientation==0?(unsigned char *)print_I.bitmap:(unsigned char *)_print_I.bitmap;
		case 'J':	return cfg.ticketOrientation==0?(unsigned char *)print_J.bitmap:(unsigned char *)_print_J.bitmap;
		case 'K':	return cfg.ticketOrientation==0?(unsigned char *)print_K.bitmap:(unsigned char *)_print_K.bitmap;
		case 'L':	return cfg.ticketOrientation==0?(unsigned char *)print_L.bitmap:(unsigned char *)_print_L.bitmap;
		case 'M':	return cfg.ticketOrientation==0?(unsigned char *)print_M.bitmap:(unsigned char *)_print_M.bitmap;
		case 'N':	return cfg.ticketOrientation==0?(unsigned char *)print_N.bitmap:(unsigned char *)_print_N.bitmap;
		case 'O':	return cfg.ticketOrientation==0?(unsigned char *)print_O.bitmap:(unsigned char *)_print_O.bitmap;
		case 'P':	return cfg.ticketOrientation==0?(unsigned char *)print_P.bitmap:(unsigned char *)_print_P.bitmap;
		case 'Q':	return cfg.ticketOrientation==0?(unsigned char *)print_Q.bitmap:(unsigned char *)_print_Q.bitmap;
		case 'R':	return cfg.ticketOrientation==0?(unsigned char *)print_R.bitmap:(unsigned char *)_print_R.bitmap;
		case 'S':	return cfg.ticketOrientation==0?(unsigned char *)print_S.bitmap:(unsigned char *)_print_S.bitmap;
		case 'T':	return cfg.ticketOrientation==0?(unsigned char *)print_T.bitmap:(unsigned char *)_print_T.bitmap;
		case 'U':	return cfg.ticketOrientation==0?(unsigned char *)print_U.bitmap:(unsigned char *)_print_U.bitmap;
		case 'V':	return cfg.ticketOrientation==0?(unsigned char *)print_V.bitmap:(unsigned char *)_print_V.bitmap;
		case 'W':	return cfg.ticketOrientation==0?(unsigned char *)print_W.bitmap:(unsigned char *)_print_W.bitmap;
		case 'X':	return cfg.ticketOrientation==0?(unsigned char *)print_X.bitmap:(unsigned char *)_print_X.bitmap;
		case 'Y':	return cfg.ticketOrientation==0?(unsigned char *)print_Y.bitmap:(unsigned char *)_print_Y.bitmap;
		case 'Z':	return cfg.ticketOrientation==0?(unsigned char *)print_Z.bitmap:(unsigned char *)_print_Z.bitmap;
	 	case 0:		return cfg.ticketOrientation==0?(unsigned char *)print_0.bitmap:(unsigned char *)_print_0.bitmap;
		case 1:		return cfg.ticketOrientation==0?(unsigned char *)print_1.bitmap:(unsigned char *)_print_1.bitmap;	
		case 2:		return cfg.ticketOrientation==0?(unsigned char *)print_2.bitmap:(unsigned char *)_print_2.bitmap;	
	 	case 3:		return cfg.ticketOrientation==0?(unsigned char *)print_3.bitmap:(unsigned char *)_print_3.bitmap;
		case 4:		return cfg.ticketOrientation==0?(unsigned char *)print_4.bitmap:(unsigned char *)_print_4.bitmap;	
		case 5:		return cfg.ticketOrientation==0?(unsigned char *)print_5.bitmap:(unsigned char *)_print_5.bitmap;	
	 	case 6:		return cfg.ticketOrientation==0?(unsigned char *)print_6.bitmap:(unsigned char *)_print_6.bitmap;			
	 	case 7:		return cfg.ticketOrientation==0?(unsigned char *)print_7.bitmap:(unsigned char *)_print_7.bitmap;	
 	 	case 8:		return cfg.ticketOrientation==0?(unsigned char *)print_8.bitmap:(unsigned char *)_print_8.bitmap;			
	 	case 9:		return cfg.ticketOrientation==0?(unsigned char *)print_9.bitmap:(unsigned char *)_print_9.bitmap;
	}
	return (unsigned char *)print_space.bitmap;
}

/*----------------------------------------------------------------------------
 * Funções:		Print_Ticket
 * Descrição: 	Imprime o ticket para o cliente
 * Parâmetros: 	depnum - Número do departamento
 *				alphaNum - letra do departamento
 *				number - Número do ticket
 *				*txtPriority - Ponteiro para texto do ticket preferêncial
 * Retorno: 	(0) Erro (1) OK
 -----------------------------------------------------------------------------*/
char Print_Ticket (unsigned char depNum, char alphaNum, unsigned int number, char *txtPriority, char *strForecast)
{
	unsigned short i=0;
	unsigned short numbyte=0;
	unsigned char  changepont=0;
	unsigned char *pontletter=0;
   	unsigned char *pontdigit1=0;
   	unsigned char *pontdigit2=0;
   	unsigned char *pontdigit3=0;
   	unsigned char *pontdigit4=0;

	if (cfg.ticketOrientation)
		formatTicket.orientation = 1;				// Ticket invertido
	else
		formatTicket.orientation = 0;				// Ticket normal

	if (cfg.logoEnable)
	{
		if (cfg.logoImpWidth==0 || cfg.logoImpHeight==0)
		{
			formatTicket.widthImage=wymaLogo.width;	// Logotipo da WYMA (default)
			formatTicket.heightImage=wymaLogo.height;
			formatTicket.bufImage=(unsigned char *) wymaLogo.bitmap;
		}
		else
		{
			formatTicket.widthImage=cfg.logoImpWidth;		// logotipo da empresa
			formatTicket.heightImage=cfg.logoImpHeight;
			formatTicket.bufImage=cfg.logoImp;
		}
	}
	else
	{
		formatTicket.widthImage=0;
		formatTicket.heightImage=0;
	}

	if (cfg.optHead1==MSGFORECAST) 					// Verifica opções do cabeçalho 1
		formatTicket.headerLine1=strForecast;		// Mostra previsão de espera
	else								 			
		formatTicket.headerLine1=cfg.header1;		// Permanece mensagem configurada
	formatTicket.infoHeader1=cfg.optHead1;
	
	if (cfg.optHead2==MSGFORECAST) 					// Verifica opções do cabeçalho 2
		formatTicket.headerLine2=strForecast;
	else								 			
		formatTicket.headerLine2=cfg.header2;		// Permanece mensagem configurada
	formatTicket.infoHeader2=cfg.optHead2;
	
	if (*txtPriority!=0)							// Verifica se ticket é preferencial
	{
		formatTicket.headerLine3=txtPriority;		// Header 3 é realocada para preferêncial
		formatTicket.infoFooter3=MSGSTANDARD;		// com caractere padrão
	}
	else
	{
		if (cfg.optHead3==MSGFORECAST) 				// Verifica opções do cabeçalho 3
			formatTicket.headerLine3=strForecast;
		else								 		
			formatTicket.headerLine3=cfg.header3;	// Permanece mensagem configurada
		formatTicket.infoHeader3=cfg.optHead3;
	}
	
	if (cfg.lettEnable)								// Verifica configuração da letra
		formatTicket.alphaNum=alphaNum;
	else
		formatTicket.alphaNum=0;

	formatTicket.number=number;						// Informações do número da senha
	formatTicket.lenNumber=cfg.lenNumTicket;
	formatTicket.typeNumber=cfg.typeNumTicket;

	if (formatTicket.typeNumber==2)											// Se número da senha do tipo gráfico	
	{														
		formatTicket.widthNumber=0;											// Inicializa largura do caractere
															
		if (cfg.lettEnable)													// Verifica configuração da letra
		{
			formatTicket.widthNumber=40;									// Soma largura do gráfico da letra
			pontletter= searchTable (formatTicket.alphaNum); 				// Procura a tabela de escrita do caractere da letra
		}
		formatTicket.widthNumber+=(formatTicket.lenNumber*40); 				// Soma a largura dos dígitos da senha
		formatTicket.heightNumber=40;										// altura do gráfico da senha
		numbyte	= ((formatTicket.widthNumber*formatTicket.heightNumber)/8); // número de bytes da senha
		
		if (formatTicket.lenNumber>3)
			pontdigit4= searchTable (((formatTicket.number/1000)%10)); 		// Procura a tabela do (milhar)quarto dígito senha
		if (formatTicket.lenNumber>2)
			pontdigit3= searchTable (((formatTicket.number/100)%10)); 		// Procura a tabela do (centena)terceiro dígito senha 
		if (formatTicket.lenNumber>1)
			pontdigit2= searchTable (((formatTicket.number/10)%10)); 		// Procura a tabela do (dezena)segundo dígito senha 
		pontdigit1= searchTable ((formatTicket.number%10)); 				// Procura a tabela do (unidade) primeiro dígito senha 
			
	 	for (i=0;i<numbyte;i++)
		{			
			if (i%40==0)   													// Troca a tabela de caractere a cada 40 bytes para escrever o próximo caractere
			{
				if (changepont==0)			
					changepont=1;	
				else if (changepont==1&&(cfg.lettEnable||formatTicket.lenNumber>1))	
					changepont=2;
				else if (changepont==2&&((cfg.lettEnable&&formatTicket.lenNumber>1)||formatTicket.lenNumber>2))	
					changepont=3;	
				else if (changepont==3&&((cfg.lettEnable&&formatTicket.lenNumber>2)||formatTicket.lenNumber>3))
					changepont=4;
				else if (changepont==4&&(cfg.lettEnable&&formatTicket.lenNumber>3))
					changepont=5;	
				else
					changepont=1;																	
			}

			if (cfg.ticketOrientation)
			{
				if (changepont==1)	 					// Passo a passo da escrita de cada caractere da senha					
				{
					if (formatTicket.lenNumber==4)	 		pticket.bitmap [i]= *pontdigit1++;					
					else if (formatTicket.lenNumber==3)	 	pticket.bitmap [i]= *pontdigit1++;	
					else if (formatTicket.lenNumber==2)	 	pticket.bitmap [i]= *pontdigit1++;	
					else if (formatTicket.lenNumber==1)	 	pticket.bitmap [i]= *pontdigit1++;			
				}
				else if (changepont==2)
				{
					if (cfg.lettEnable)
					{
						if (formatTicket.lenNumber==4)	 	pticket.bitmap [i]= *pontdigit2++;	
						else if (formatTicket.lenNumber==3)	pticket.bitmap [i]= *pontdigit2++;	
						else if (formatTicket.lenNumber==2)	pticket.bitmap [i]= *pontdigit2++;	
						else if (formatTicket.lenNumber==1)	pticket.bitmap [i]= *pontletter++;
					}
	
					else if (formatTicket.lenNumber==4)	 	pticket.bitmap [i]= *pontdigit2++;	
					else if (formatTicket.lenNumber==3)	 	pticket.bitmap [i]= *pontdigit2++;	
					else if (formatTicket.lenNumber==2)	 	pticket.bitmap [i]= *pontdigit2++;	
				}
				else if (changepont==3)
				{
					if (cfg.lettEnable)
					{
						if (formatTicket.lenNumber==4)	 	pticket.bitmap [i]= *pontdigit3++;	
						else if (formatTicket.lenNumber==3)	pticket.bitmap [i]= *pontdigit3++;	
						else if (formatTicket.lenNumber==2)	pticket.bitmap [i]= *pontletter++;
					}

					else if (formatTicket.lenNumber==4)	 	pticket.bitmap [i]= *pontdigit3++;
					else if (formatTicket.lenNumber==3)	 	pticket.bitmap [i]= *pontdigit3++;
				}
				else if (changepont==4)
				{
					if (cfg.lettEnable)
					{
						if (formatTicket.lenNumber==4)	 	pticket.bitmap [i]= *pontdigit4++;
						else if (formatTicket.lenNumber==3)	pticket.bitmap [i]= *pontletter++;
					}

					else if (formatTicket.lenNumber==4)	 	pticket.bitmap [i]= *pontdigit4++;
				}
				else if (changepont==5)
				{
					if (cfg.lettEnable)						pticket.bitmap [i]= *pontletter++;		
				}
			}
			else
			{
				if (changepont==1)	 					// Passo a passo da escrita de cada caractere da senha					
				{
					if (cfg.lettEnable)						pticket.bitmap [i]= *pontletter++;		
					else if (formatTicket.lenNumber==4)	 	pticket.bitmap [i]= *pontdigit4++;					
					else if (formatTicket.lenNumber==3)	 	pticket.bitmap [i]= *pontdigit3++;	
					else if (formatTicket.lenNumber==2)	 	pticket.bitmap [i]= *pontdigit2++;	
					else if (formatTicket.lenNumber==1)	 	pticket.bitmap [i]= *pontdigit1++;			
				}
				else if (changepont==2)
				{
					if (cfg.lettEnable)
					{								
						if (formatTicket.lenNumber==4)	 	pticket.bitmap [i]= *pontdigit4++;					
						else if (formatTicket.lenNumber==3)	pticket.bitmap [i]= *pontdigit3++;	
						else if (formatTicket.lenNumber==2)	pticket.bitmap [i]= *pontdigit2++;	
						else if (formatTicket.lenNumber==1)	pticket.bitmap [i]= *pontdigit1++;	
					}
	
					else if (formatTicket.lenNumber==4)	 	pticket.bitmap [i]= *pontdigit3++;					
					else if (formatTicket.lenNumber==3)	 	pticket.bitmap [i]= *pontdigit2++;	
					else if (formatTicket.lenNumber==2)	 	pticket.bitmap [i]= *pontdigit1++;	
				}
				else if (changepont==3)
				{
					if (cfg.lettEnable)
					{								
						if (formatTicket.lenNumber==4)	 	pticket.bitmap [i]= *pontdigit3++;					
						else if (formatTicket.lenNumber==3)	pticket.bitmap [i]= *pontdigit2++;	
						else if (formatTicket.lenNumber==2)	pticket.bitmap [i]= *pontdigit1++;	
					}
	
					else if (formatTicket.lenNumber==4)	 	pticket.bitmap [i]= *pontdigit2++;					
					else if (formatTicket.lenNumber==3)	 	pticket.bitmap [i]= *pontdigit1++;	
				}
				else if (changepont==4)
				{
					if (cfg.lettEnable)
					{								
						if (formatTicket.lenNumber==4)	 	pticket.bitmap [i]= *pontdigit2++;					
						else if (formatTicket.lenNumber==3)	pticket.bitmap [i]= *pontdigit1++;	
					}
	
					else if (formatTicket.lenNumber==4)	 	pticket.bitmap [i]= *pontdigit1++;								
				}
				else if (changepont==5)
					pticket.bitmap [i]= *pontdigit1++;	
			}
		}

		pticket.height = formatTicket.heightNumber;	// grava altura na estrutura do ticket
		pticket.width =	formatTicket.widthNumber;	// grava largura na estrutura do ticket
		formatTicket.bufNumber= pticket.bitmap;		// grava o bitmap construido na estrutura do ticket
	}

	formatTicket.department=cfg.depName[depNum-1]; 	// Nome do departamento
	while (cfg.depName[depNum-1][i]!=0 && i<36)
		i++;
	if (i<=18)
		formatTicket.infoDep=MSGEXPAND;
	else
		formatTicket.infoDep=MSGSTANDARD;

	if (cfg.optFoot1==MSGFORECAST) 					// Verifica opções do rodapé 1
		formatTicket.footerLine1=strForecast;
	else								 			
		formatTicket.footerLine1=cfg.footer1;		// Permanece mensagem configurada
	formatTicket.infoFooter1=cfg.optFoot1;

	if (cfg.optFoot2==MSGFORECAST) 					// Verifica opções do rodapé 2
		formatTicket.footerLine2=strForecast;
	else
		formatTicket.footerLine2=cfg.footer2;	   	// Permanece mensagem configurada
	formatTicket.infoFooter2=cfg.optFoot2;

	if (cfg.optFoot3==MSGFORECAST) 					// Verifica opções do rodapé 3
		formatTicket.footerLine3=strForecast;
	else
		formatTicket.footerLine3=cfg.footer3;		// Permanece mensagem configurada
	formatTicket.infoFooter3=cfg.optFoot3;

	if (cfg.optFoot4==MSGFORECAST) 					// Verifica opções do rodapé 4
		formatTicket.footerLine4=strForecast;
	else
		formatTicket.footerLine4=cfg.footer4;		// Permanece mensagem configurada
	formatTicket.infoFooter4=cfg.optFoot4;

	if (cfg.printProtocol)							// Opção para imprimir protocolo
		formatTicket.strNumProtocol=atdProtocol;	// Endereço do array com o número do protocolo
	else
		formatTicket.strNumProtocol="\x0"; 			// Endereço de um text null

	formatTicket.cut=cfg.cut;						// Verifica configuração de corte

	formatTicket.copy=cfg.depCopy[depNum-1];		// Número de vias do ticket

	erroPrint=PrintENG_ReadStatus();				// Lê status da impressora

	if (((erroPrint>>4)&1)==0)						// Verifica se tem pouco papel para avisar
		PAPERWARNING=1;
	else
		PAPERWARNING=0;

	PAPEREND=0;

	if ((erroPrint&1)==1)							// Verifica se esta On-Line
		if (((erroPrint>>1)&1)==0)					// Verifica se tem papel
			if (((erroPrint>>3)&1)==0)				// Verifica se rolete esta travado
				if (PrintENG_Ticket (formatTicket))	// Imprime ticket
					return 1;

	PAPEREND=1;

	return 0;
}

/*----------------------------------------------------------------------------
 * Funções:		Print
 * Descrição: 	Verifica modo de operação e tipo de cliente e imprime o ticket
 * Parâmetros: 	priority - Tipo de cliente
 * Retorno: 	(0) Não foi possível imprimir   (1) Impressão OK
 -----------------------------------------------------------------------------*/
char Print (char priority)
{
	char result=0;
	char strForecast[6];

	if (priority=='N')									// Enviando para impressão ticket normal
	{
		if (cfg.runMode==_MODECLIENT)
		{
			if (Print_Ticket (dptoServer, alphaServer, numTicketServer, "", forecastServer))
				result=1;
		}
		else if (cfg.runMode==_MODESERVER) 
		{
			ShowForecast (atend.depNum,strForecast);	// Calcula previsão de espera
			if (Print_Ticket (atend.depNum, atend.alphaNum, atend.number, "", strForecast))
				result=1;
		}
	}
	else if (priority=='P')								// Enviando para impressão ticket preferencial	
	{	
		if (cfg.runMode==_MODECLIENT)
		{
			if (Print_Ticket (dptoServer, alphaServer, numTicketServer, "P R E F E R E N C I A L", forecastServer)) 	
				result=1;
		}
		else if (cfg.runMode==_MODESERVER)
		{
			ShowForecast (atend.depNum,strForecast);	// Calcula previsão de espera
			if (Print_Ticket (atend.depNum, atend.alphaNum, atend.number, "P R E F E R E N C I A L", strForecast)) 	
				result=1;
		}
	}
	else if (priority=='O')								// Enviando para impressão ticket preferencial +80	
	{	
		if (cfg.runMode==_MODECLIENT)
		{
			if (Print_Ticket (dptoServer, alphaServer, numTicketServer, "P R E F E R E N C I A L  + 8 0", forecastServer)) 	
				result=1;
		}
		else if (cfg.runMode==_MODESERVER)
		{
			ShowForecast (atend.depNum,strForecast);	// Calcula previsão de espera
			if (Print_Ticket (atend.depNum, atend.alphaNum, atend.number, "P R E F E R E N C I A L  + 8 0", strForecast)) 	
				result=1;
		}
	}
	return result;
}

/*----------------------------------------------------------------------------
 * Funções:		Check_DepTicket
 * Descrição: 	Verifica parâmetros e busca novo número do ticket para imprimir
 *				 Se estiver em modo escravo solicita novo número para um servidor
 *				 Se estiver em modo server ou offline cadastra novo número na Flash
 * Parâmetros: 	depNum - Número do departamento
 * 				type - Tipo de ticket (N=normal, P=preferencial e O=preferencial +80
 * Retorno: 	Resultado do cadastro: 0=SEM TICKETS 'N'=fila normal 'P'-fila preferencial
 *										'0'=fila preferencial +80  'X'=Dpto desabilitado
 -----------------------------------------------------------------------------*/
char Check_DepTicket (unsigned char depNum, unsigned char type)
{
	Beep ();
	if (cfg.depEnable[depNum-1])
	{
		if (cfg.runMode==_MODECLIENT)
		{
			Send_GetNumTicket(ITF_UDP, 0, depNum);
			return type;
		}
		else if (cfg.runMode==_MODESERVER)
		{
			if (type=='N')
				atend=New_ticketNumber (depNum, TKTCOMUM);
			else if (type=='P')
				atend=New_ticketNumber (depNum, TKTPREF);
			else if (type=='O')
				atend=New_ticketNumber (depNum, TKTPREF);

			if (atend.number!=0)
				return type;
		}
	}
	else
		return 'X';

	return 0;
}

/*----------------------------------------------------------------------------
 * Funções:		Process_Queue
 * Descrição: 	Cadastra novo número de ticket na fila
 * Parâmetros: 	key - Tecla do atendimento 
 * Retorno: 	Resultado do cadastro: 	0=SEM TICKETS 
 *										'N'=fila normal 
 *										'P'-fila preferencial
 *										'0'=fila preferencial +80  
 *										'X'=Dpto desabilitado
 -----------------------------------------------------------------------------*/
char Process_Queue (unsigned short key)
{
	char result=0;

	switch (key)
	{
		case 1:	result=Check_DepTicket(1,'N'); break;
		case 2:	result=Check_DepTicket(2,'N'); break;
		case 3: result=Check_DepTicket(3,'N'); break;
		case 4: result=Check_DepTicket(4,'N'); break;
		case 5: result=Check_DepTicket(5,'N'); break;
		case 6: result=Check_DepTicket(6,'N'); break;
		case 7: result=Check_DepTicket(7,'N'); break;
		case 8: result=Check_DepTicket(8,'N'); break;
		case 9: result=Check_DepTicket(9,'N'); break;
		case 10:result=Check_DepTicket(10,'N'); break;
		case 11:result=Check_DepTicket(11,'N'); break;
		case 12:result=Check_DepTicket(12,'N'); break;
	
		case 31:result=Check_DepTicket(1,'P'); break;
		case 32:result=Check_DepTicket(2,'P'); break;
		case 33:result=Check_DepTicket(3,'P'); break;
		case 34:result=Check_DepTicket(4,'P'); break;
		case 35:result=Check_DepTicket(5,'P'); break;
		case 36:result=Check_DepTicket(6,'P'); break;
		case 37:result=Check_DepTicket(7,'P'); break;
		case 38:result=Check_DepTicket(8,'P'); break;
		case 39:result=Check_DepTicket(9,'P'); break;
		case 40:result=Check_DepTicket(10,'P'); break;
		case 41:result=Check_DepTicket(11,'P'); break;
		case 42:result=Check_DepTicket(12,'P'); break;
	
		case 61:
		case 62:
		case 63:
		case 64:
		case 65:
		case 66:
		case 67:
		case 68:
		case 69:
		case 70:
		case 71:
		case 72:result=Check_DepTicket(13,'O'); break;
	}

	return result;
}

/*----------------------------------------------------------------------------
 * Funções:		Check_Events
 * Descrição: 	Verifica eventos do sistema
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void Check_Events (void)
{
	TIME_TYPE time;
	time=Get_RTC ();

	if ((time.HOUR==cfg.timeHourRep) && 
		(time.MINUTE==cfg.timeMinRep))					// Verifica o horário do relatório
	{
		if (DOREPORT==0)								// Se ainda não foi realizado
			DOREPORT=1;									// Seta flag para gerar relatório
	}
	else if (DOREPORT==2)								// Flag ja foi atendida?
	{
		DOREPORT=0;										// Desativa flag ja atendida
	}
	else if ((time.HOUR==cfg.timeHourOn) && 
			(time.MINUTE==cfg.timeMinOn) &&				// Verifica o horário de desligar,
			(cfg.enTimeAuto))							// qdo habilitado controle automatico
	{
		if (POWERON==0)	
			POWERON=1;									// Seta flag para desligar dispositivo
	}
	else if (POWERON==2)								// Flag ja foi atendida?
	{
		POWERON=0;										// Desativa flag ja atendida
	}
	else if ((time.HOUR==cfg.timeHourOff) && 
			(time.MINUTE==cfg.timeMinOff) &&			// Verifica o horário de desligar,
			(cfg.enTimeAuto))							// qdo habilitado controle automatico
	{
		if (POWEROFF==0)	
			POWEROFF=1;									// Seta flag para desligar dispositivo
	}
	else if (POWEROFF==2)								// Flag ja foi atendida?
	{
		POWEROFF=0;										// Desativa flag ja atendida
	}
	else if (cfg.autoTest)								// Verifica se o auto-teste esta ligado.
	{
		if (time.SECOND==0)								// Imprime novos tickets em intervalos de 1 min
			TIMETESTING=1;
	}
}

/*----------------------------------------------------------------------------
 * Funções:		ChangePower
 * Descrição: 	Liga/Desliga dispositivo
 * Parâmetros: 	power - (0) desligado (1) ligado
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void ChangePower (char power)
{
	if (power)
	{
		cfg.energyStatus=1;									// Dispositivo foi ligado
		Write_Flash ();
	}
	else
	{
		cfg.energyStatus=0;									// Dispositivo foi desligado
		Write_Flash ();
	}
}

/*----------------------------------------------------------------------------
 * Funções:		init_modelMIG
 * Descrição: 	Lê da configuração o modelo do MIG e grava o nome  do dispositivo
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void init_modelMIG (void)
{
 	devName[0]='M';									// String de resposta ao modelo
	devName[1]='I';
	devName[2]='G';

	if (cfg.devModel==_MIG6)
	{
		devName[3]='6';
		devName[4]=' ';
		devName[5]=0;
	}
	else if (cfg.devModel==_MIG12)
	{
		devName[3]='1';
		devName[4]='2';
		devName[5]=0;
	}
}

/*-------------------------------------------------------------------------------------------
* Função:		Verify_Protocol
* Descrição:	Verifica se chegou chamada de senha por protocolo
* Parâmetros:	Nenhum
* Retorno:		(0) Não recebeu código (1) Recebeu código
-------------------------------------------------------------------------------------------*/
char Verify_Protocol (void)
{
	signed char i;

	for (i=9 ; i>=0 ; i--)
	{
		if (call[i].row!=0 && 								// Verifica se tem ticket no buffer
			call[i].func!=0 && 
			call[i].user!=0)			
		{
			runRow 		= call[i].row;						// Número da linha do display
			runAlpha 	= call[i].alpha;					// Letra do ticket
			runUser 	= call[i].user;						// Número do Box
			runDep		= call[i].dep;						// Departamento de chamada
			runFunc		= call[i].func;						// Função selecionada

			if (runFunc==F_TICKET &&						// Se for função "Chamada de ticket" e
				runUser==call[i].user &&					// o usuário for igual o anterior e
				runTicket==call[i].number)					// o número for igual ao anterior
				runFunc=F_REPEAT;							// Muda para função para "repete"

			if (runFunc==F_NEXT ||							// Se for funções "Próximo ticket" ou
				runFunc==F_TICKET)							// "Chamada de ticket"
				runTicket = call[i].number;					// Atualiza número do ticket

			call[i].row = 0;								// Zera buffer
			call[i].number = 0;
			call[i].alpha = 0;
			call[i].user = 0;
			call[i].dep = 0;
			call[i].func = 0;

			i=-1;											// Sai do loop quando acha o dado 
		}
		else
		{
			runRow=runFunc=runUser=0;
			call[i].row = 0;								// Zera número do display
			call[i].number = 0;								// Zera o número do ticket
			call[i].alpha = 0;								// Zera a letra do ticket
			call[i].dep=0;									// zera depart de chamada
			call[i].user = 0;								// Zera o box do ticket
			call[i].func = 0;								// Zera a função da tecla
		}

		if (runRow!=0 && 									// Mostra se chegaram novos valores
			runFunc!=0 && 
			runUser!=0)
		{
			if (runFunc==F_NONE)							// Chamada de ticket
			{
			}
			else if (runFunc==F_NEXT)						// Chamar Próximo
			{
				TKTINFO tkt = {0,0,0,0};
				tkt = Get_ticketNum (runUser, runDep);		// Busca informações do ticket
				if (tkt.number!=0)
				{
					runDep = tkt.dep;
					runAlpha = tkt.alpha;	 			 	// Verifica a letra do departamento
					runTicket = tkt.number;					// Informa número do ticket
					runType=tkt.type;
					lastTicket[runUser-1]=runTicket;
					lastAlpha[runUser-1]=runAlpha;
					lastDep[runUser-1]=runDep;
					// Informações para chamada WEB "callnext.cgi"
					lastAlphaDep[runDep-1]=runAlpha;
					lastTicketDep[runDep-1]=runTicket;
					lastUser[runDep-1]=runUser;
					if (runTicket!=0)
					{
						Send_SetNumTicket (					// Envia comando para painel:
									ITF_UDP, 			// Informando: * interface de envio
									0,						//  * Retry-"desabilitado"
									cfg.eTop[runDep-1],	  	//  * Status de pesquisa no Top
									cfg.depIpTop[runDep-1],	//  * IP do top
									cfg.depItfTop[runDep-1],//  * Interface do Top
									cfg.depName[runDep-1]);	//  * Nome do departamento
						STATPROG=_RECCALL;					// Status enviando chamada
					}
				}
			}
			else if (runFunc==F_REPEAT)						// Repete chamada
			{
				if (lastTicket[runUser-1]!=0)
				{
					runTicket = lastTicket[runUser-1];		// Recupera último ticket da mesa
					runAlpha = lastAlpha[runUser-1];	  	// Verifica a letra do departamento
  					runDep = lastDep[runUser-1];			// Verifica último dpto. da mesa
					// Informações para chamada WEB "callnext.cgi"
					lastAlphaDep[runDep-1]=runAlpha;
					lastTicketDep[runDep-1]=runTicket;
					lastUser[runDep-1]=runUser;

					if (runTicket!=0)						// Se tiver chamada anterior
					{
						Send_SetNumTicket (					// Envia comando para painel:
									ITF_UDP, 				// Informando: * interface de envio
									0,						//  * Retry-"desabilitado"
									cfg.eTop[runDep-1],	  	//  * Status de pesquisa no Top
									cfg.depIpTop[runDep-1],	//  * IP do top
									cfg.depItfTop[runDep-1],//  * Interface do Top
									cfg.depName[runDep-1]);	//  * Nome do departamento
						STATPROG=_RECCALL;					// Status enviando chamada
					}
				}
			}
			else if (runFunc==F_CLOSE)						// Fechar atendimento
			{
				if (runTicket!=0)							// Se tiver chamada anterior
				{
					Set_ticketSta (runTicket,STA_CLOSED,	// Envia comando para fechar
								runUser,&runType,runDep);
					runTicket=0;							// Encerrado: Não pode mais chamar
				}
			}
			else if (runFunc==F_AUX)						// Cancelar atendimento
			{
				if (runTicket!=0)							// Se tiver chamada anterior
				{
					Set_ticketSta (runTicket,STA_CANCELED,	// Envia comando para cancelar
								runUser,&runType,runDep);
					runTicket=0;							// Cancelado: Não pode mais chamar
				}
			}
			else if (runFunc==F_TICKET)						// Chamar ticket especificado
			{
				if (runTicket!=0)							// Se tiver chamada anterior
				{
					if (Set_ticketSta(runTicket,STA_WORKING,// Muda status do ticket para WORKING
								runUser,&runType,runDep))
					{
						Send_SetNumTicket (					// Envia comando para painel:
									ITF_UDP, 			// Informando: * interface de envio
									0,						//  * Retry-"desabilitado"
									cfg.eTop[runDep-1],	  	//  * Status de pesquisa no Top
									cfg.depIpTop[runDep-1],	//  * IP do top
									cfg.depItfTop[runDep-1],//  * Interface do Top
									cfg.depName[runDep-1]);	//  * Nome do departamento
						STATPROG=_RECCALL;					// Status enviando chamada
					}
					else
						runTicket=0;
				}
			}

			return 1;
		}
	}
	return 0;
}

/*-------------------------------------------------------------------------------------------
 * Função: 		main
 * Descrição: 	Aplicação
 -------------------------------------------------------------------------------------------*/
int main (void)
{
	unsigned char x;
	unsigned char timeBase=0;
	unsigned short timeProg=0;
	unsigned short timeInfoSend=0;
	unsigned short runKey=0;
	char result=0;
	unsigned char flagDataClient=0;							// Flag - Gravando dados do cliente.
	static unsigned char cntTimeOut=0;						// Contador de timeout

	SystemInit();											// Configura clock
	USB_Init();                                 			// Inicializa USB 
	USB_Connect(__TRUE);                      				// Conecta USB 

	init_spi (); 											// Inicializa SPI selecionadas (SPI_CONFIG.c)
	init_rtc();												// Inicializa RTC
	
	ReadConfig(ADDRESS_USER_CONFIG,&cfg,sizeof(cfg));		// Recupera configurações da Flash
	CheckConfig ();	  										// Verifica configurações

	init_modelMIG ();									    // Inicializa o modelo do MIG ; 1=_MIG6KEY, 2=_MIG12KEY

  	SysTick->LOAD = (SystemFrequency / 100) - 1;			// Setup and enable the SysTick timer for 10ms. 
  	SysTick->CTRL = 0x05;

	spi_Flash_Init ();										// Inicializa FLASH de uso geral
	init_uart (UART0, 115200, 8);							// Inicia uart0 para CRF
	init_uart (UART1, 57600, 8);							// Inicia uart1 para impressora
	TIMER1_init ();											// Configura o TIMER1 (uso na chave desliga)
	PrintENG_Init ();										// Inicializa impressora ENGWORK

	TIM1_delay (TIME3s);

	LPC_PINCON->PINSEL2 &= ~(3UL<<18); 						// Pino P1.9 como I/O
	LPC_PINCON->PINMODE2 &= ~(3UL<<18); 					// Pino P1.9 com pullUp
	LPC_GPIO1->FIODIR &= ~(1<<9);							// Pino P1.9 como entrada
	if (((LPC_GPIO1->FIOPIN >> 9) & 1)==0)					// Verifica o pino P1.9 (Rx ethernet)..
	{
		ethExist=1;	
	
		for (x=0 ; x<6 ; x++)
			own_hw_adr[x]=cfg.macAddress[x];				// Restaura MacAdress
		init_TcpNet ();										// Inicializa ethernet
		init_socket ();										// Cria socket UDP
		if (cfg.dhcpStatus==0)								// Restaura status dhcp
			dhcp_disable ();  								// Desabilita DHCP
		else
  			dhcp_tout = DHCP_TOUT;
		http_EnAuth=cfg.authEnable;							// Restaura status da autenticação http
		for (x=0 ; x<11 ; x++)
			http_auth_passw[x]=cfg.authPassword[x];			// Restaura password http
		for (x=0 ; x<16 ; x++)
			lhost_name[x]=cfg.hostName[x];					// Restaura host Name
		for (x=0 ; x<4 ; x++)
			localm[NETIF_ETH].IpAdr[x]=cfg.localIp[x];		// Restaura Ip local
		for (x=0 ; x<4 ; x++)
			localm[NETIF_ETH].NetMask[x]=cfg.ipMask[x];		// Restaura Mascara de rede
		for (x=0 ; x<4 ; x++)
			localm[NETIF_ETH].DefGW[x]=cfg.gateway[x];		// Restaura Gateway
		for (x=0 ; x<4 ; x++)
			localm[NETIF_ETH].PriDNS[x]=cfg.dns1[x];		// Restaura DNS primário
		for (x=0 ; x<4 ; x++)
			localm[NETIF_ETH].SecDNS[x]=cfg.dns2[x];		// Restaura DNS Secundário
	}
	else
	{
		ethExist=0;											// PHY não encontrado, desabilita ethernet	
	}

	PWM_init ();											// Inicializa pwm para beep

	_DEF_DRIVE = cfg.driver;	 							// Restaura driver ativo
	Check_FileSystem ();									// Inicializa drive de sistema de arquivos

	TicketInit ();											// Busca na Flash informações dos tickets

	STATPROG=_GETDEV;

	timeProg=0;

  	while (1) 
	{
    	timer_poll ();
   		if (ethExist) main_TcpNet ();						// Trata pacotes ethernet
		uart_CheckProtocol (UART0);	  						// Verifica o recebimento na interface UART0

		if (tick)
		{
			if (ethExist) dhcp_check ();					// Verifica status/servidor dhcp

			if (++timeBase==100)
			{
				Check_Events (); 							// Verifica programações por horário
				timeBase=0;
				timeInfoSend++;								// Base de tempo para enviar 
															// informações do atendimento
			}

			if (STATPROG==_WAITING ||
				STATPROG==_DPTODISABLE ||
				STATPROG==_NOTICKET ||
				STATPROG==_PRINTING)
				if (Verify_Protocol ())						// Verifica se chegou dado via protocolo WYMA
					timeProg=0;								// Zera contador de tempo

			if (STATPROG==_UDPPROG)
			{
				if (PROGMODE==0)							// Caso desligou modo programação:
				{
					STATPROG=_WAITING;						// Modo aguardando requisição.
					timeProg=0;								// Zera contador de tempo
				}
			}
			else if (STATPROG==_GETDEV)
			{
				if (cfg.runMode==_MODECLIENT)
				{
					if (cfg.ipServer[0]==0 && 				// Nenhum servidor configurado = 0.0.0.0
						cfg.ipServer[1]==0 &&
						cfg.ipServer[2]==0 && 
						cfg.ipServer[3]==0)
					{
						STATPROG=_WAITING;					// Modo aguardando requisição.
						timeProg=0;							// Zera contador de tempo
					}
					else
					{
						Send_GetDeviceName (ITF_UDP, 0);	// Envia requisição para identificar a impressora
						STATPROG=_RECDEV;					// Muda state para receber resposta da impressora
						timeProg=0;							// Zera contador de tempo
					}
				}
				else if (cfg.runMode==_MODESERVER)
				{
					STATPROG=_WAITING;						// Modo servidor habilitado, aguarda requisição.
					timeProg=0;								// Zera contador de tempo
				}
			}
			else if (STATPROG==_RECDEV)
			{
				if (timeProg++==200)						// Verifica se estourou o tempo de recebimento
					STATPROG=_GETDEV; 						// Muda state para verificar impressora novamente
				else if (numFrame==numFrameServer)			// Compara frame enviado com recebido, se for
				{											// mesmo numero chegou resposta do frame.
					STATPROG=_GETDEV;						// Muda state para verificar impressora novamente					
					if (devNameServer[0]=='M')
						if (devNameServer[1]=='I')
							if (devNameServer[2]=='G')
							{
								STATPROG=_WAITING;			// Muda state para aguardar usuário
								timeProg=0;					// Zera contador de tempo
								numFrame=0;					// Inicializa o registro do frame enviado
								numFrameServer=0;			// Inicializa o registro do frame recebido
							}
				}
			}
			else if (STATPROG==_SETCALL)
			{
				Send_SetNumTicket (							// Envia comando para painel:
									ITF_UDP, 			// Informando: * interface de envio
									0,						//  * Retry-"desabilitado"
									cfg.eTop[runDep-1],	  	//  * Status de pesquisa no Top
									cfg.depIpTop[runDep-1],	//  * IP do top
									cfg.depItfTop[runDep-1],//  * Interface do Top
									cfg.depName[runDep-1]);	//  * Nome do departamento

				STATPROG=_RECCALL;							// Muda state para receber resposta ao protocolo
				timeProg=0;									// Zera contador de tempo
			}
			else if (STATPROG==_RECCALL)
			{
				if (timeProg++==100)						// Verifica se estourou o tempo de recebimento
				{
					STATPROG=_SETCALL;						// Muda status para enviar novamente
					if (++cntTimeOut>2)						// Se tentou 2 vezes e não encontrou o server..
					{
						cntTimeOut=0;						// Zera contador de timeout
						STATPROG=_WAITING;					// Muda state para aguardar
						timeProg=0;							// Zera contador de tempo
					}
				}
				else if (numFrame==numFrameServer)			// Compara frame enviado com recebido, se for
				{											// mesmo numero chegou resposta do frame.
					cntTimeOut=0;							// Zera contador de timeout
					numFrame=0;								// Inicializa o registro do frame enviado
					numFrameServer=0;						// Inicializa o registro do frame recebido

					STATPROG=_WAITING;						// Muda state para aguardar 
					timeProg=0;								// Zera contador de tempo
				}				
			}
			else if (STATPROG ==_WAITING)					// ________STATUS "AGUARDANDO"
			{
				if (WEBREQUEST==UPD_FIRM)					// Firmware foi atualizado..?
				{
					Prog_Watchdog (0x005FFFFF);				// Força reinicialização
					STATFILE=_FLOPEN;	  					// Chegou requisição da WEB muda status Data Prog
					STATPROG=_UPDATING;						// Mostra mensagem de atualização
					timeProg=0;
				}
				else if (WEBREQUEST==UPD_SCR1 ||
						WEBREQUEST==UPD_SCR2 ||
						WEBREQUEST==UPD_LOGO)
				{
					STATFILE=_FLOPEN;	  					// Chegou requisição da WEB muda status Data Prog
					STATPROG=_UPDATING;						// Mostra mensagem de atualização
					timeProg=0;
				}
				else if (WEBREQUEST==UPD_NETWORK)
				{
					WEBREQUEST=REQ_NONE;
					STATPROG=_WAITING;					// Muda status _WAITING para aguardar o usuário
					timeProg=0;
				}
				else if (WEBREQUEST==NEW_CALL)
				{
					New_Call();								// Grava nova chamada http no buffer
					WEBREQUEST=REQ_NONE;					// Desabilita flag de solic. http
				}
				else if (POWERON==1)
				{
					POWERON=2;
					ChangePower (1);						// Indica que foi ligado
				}
				else if (POWEROFF==1)						// Verfi. se alcançou horário de desligar
				{
					POWEROFF=2;
					ChangePower (0);						// Indica que foi desligado
				}
				else if (DOREPORT==1)						// Verif. se alcançou horário do relatório
				{
					SaveBeforeReport();
					DOREPORT=2;
					timeProg=0;
					if (cfg.runMode==_MODESERVER)			// Rodando Modo server - Verifica relatório
					{
						if (cfg.saveReport)
						{
							STATFILE=_FLANALYZE;			// Status para iniciar processo de gravação/ajuste
							STATPROG=_UPDATINGREP;			// Mostra mensagem de atualização
						}
						else
						{
							STATFILE=_FLIGNORE;				// Status para iniciar processo de ajuste
							STATPROG=_UPDATINGREP;			// Mostra mensagem de atualização
						}	
					}
				}
				else if (RESTARTSOFT)						// Verifica se recebeu comando UDP que exige reinicio
				{
					Prog_Watchdog (0x0000FFFF);				// Força reinicialização
					while (1);
				}
				else if (PROGMODE)
				{
					STATPROG=_UDPPROG;
					timeProg = 0;
				}
				else if (TIMETESTING)						// Verif. flag de teste de cadastro
				{
					TIMETESTING=0;
					timeProg = 0;
					if (cfg.runMode==_MODECLIENT)			// Rodando Modo cliente - solicita ticket
					{
						if ((result=Check_DepTicket(1,'N'))!=0)// Cadastra senha nova automáticamente
						{
							if (result=='X')
								STATPROG=_DPTODISABLE;		// Indica que dpto esta desabilitado
							else
								STATPROG=_RECTICKET;		// State para receber resposta do protocolo
						}
						else
							STATPROG = _NOTICKET;			// Indica que não tem mais senhas no dpto.
					}
					else if (cfg.runMode==_MODESERVER)		// Rodando Modo server - imprime ticket
					{
						if ((result=Check_DepTicket(1,'N'))!=0)// Cadastra senha nova automáticamente
						{
							if (result=='X')
								STATPROG=_DPTODISABLE;		// Indica que dpto esta desabilitado	
							else
							{
								STATFILE = _FLBUFFER;		// Muda status para gravar na Flash.
								//STATPROG = _QUEUING;		// Muda status para enfileirando o ticket.
								STATPROG = _SETCARD;		// Muda status para enviar número do cartão
							}
						}
						else
							STATPROG = _NOTICKET;			// Indica que não tem mais senhas no dpto.
					}
				}
				else
				{
				  if (remoteKey)
				  {				
				  	  runKey = remoteKey;		
					  remoteKey=0;
					  timeProg = 0;
						
					  if ((result=Process_Queue(runKey))!=0)// Envia comando com o departamento selecionado.
					  {
					  	if (result=='X')
						  STATPROG=_DPTODISABLE;			// Indica que dpto esta desabilitado
					 	else
						{
						  if (cfg.runMode==_MODECLIENT)		// Rodando Modo cliente - solicita ticket
						 	STATPROG=_RECTICKET;			// State para receber resposta do protocolo
						  else if (cfg.runMode==_MODESERVER)// Rodando Modo server - imprime ticket
						  {
						 	STATFILE = _FLBUFFER;			// Muda status para gravar na Flash.
							//STATPROG = _QUEUING;			// Muda status para mostrar "Cadastrando e impr"
							STATPROG = _SETCARD;			// Muda status para enviar número do cartão
						  }
						}
					  }
					  else
					  	STATPROG = _NOTICKET;				// Indica que não tem mais senhas no dpto.
				  }
				  else
				  {
					  if (timeInfoSend>10)
					  {
						timeInfoSend=0;
						if (cfg.broadcastInfo &&			// Se opção estiver habilitada
							cfg.runMode==_MODESERVER)  		// e no modo servidor de tickets:
						  Send_SetInfoAtd(ITF_UDP,0);	// Envia informações do atendimento
					  }
				  }
				}
			}
			else if (STATPROG==_GETTICKET)
			{
				if ((result=Process_Queue(runKey))!=0)		// Envia comando com o departamento selecionado.
				{
					if (result=='X')
						STATPROG=_DPTODISABLE;				// Indica que dpto esta desabilitado
					else
						STATPROG=_RECTICKET;				// State para receber resposta do protocolo
				}
				else
					STATPROG = _NOTICKET;					// Indica que não tem mais senhas no dpto.

				timeProg=0;									// Zera contador de tempo
			}
			else if (STATPROG==_RECTICKET)
			{
				if (timeProg++==200)						// Verifica se estourou o tempo de recebimento
					STATPROG=_GETTICKET;					// Muda status para enviar novamente
				else if (numFrame==numFrameServer)			// Compara frame enviado com recebido, se for
				{											// mesmo numero chegou resposta do frame.
					numFrame=0;								// Inicializa o registro do frame enviado
					numFrameServer=0;						// Inicializa o registro do frame recebido
					if (numTicketServer!=0)
					{
						//STATPROG = _QUEUING;				// Muda status para "Imprimindo"
						STATPROG = _SETCARD;				// Muda status para enviar numero cartão
						timeProg=0;							// Zera contador de tempo
					}
					else
					{
						STATPROG = _NOTICKET;				// Indica que não tem mais senhas no dpto.
						timeProg=0;							// Zera contador de tempo
					}
				}				
			}
			else if (STATPROG == _DPTODISABLE)				// _______STATUS "DPTO DESATIVADO"
			{
				if (timeProg++==0)
				{
				}
				else if (timeProg>300)
				{
					STATPROG=_WAITING;						// Status para aguardar novo cliente
					timeProg = 0;
				}
			}
			else if (STATPROG == _NOTICKET)					// _______STATUS "SENHAS ESGOTADAS"
			{
				if (timeProg++==0)
				{
				}
				else if (timeProg>300)
				{
					STATPROG=_WAITING;						// Status para aguardar novo cliente
					timeProg = 0;
				}
			}
			else if (STATPROG == _SETCARD)					// _______STATUS "ENVIANDO NUMERO CARTÃO"
			{
				if (cfg.printProtocol)						// Verifica se esta habilitada opção
				{
					Send_SetProtocolo (ITF_UDP, 0);		// Envia requisição do numero de protocolo
					STATPROG=_RECCARD;						// Muda state para receber resposta
					timeProg=0;								// Zera contador de tempo
				}
				else
				{
					STATPROG = _QUEUING;					// Muda status para "Imprimindo"
					timeProg=0;								// Zera contador de tempo
				}
			}
			else if (STATPROG == _RECCARD)					// _______STATUS "AGUARDA RESPOSTA"
			{
				if (timeProg++==100)						// Verifica se estourou o tempo de recebimento
				{
					STATPROG=_SETCARD;						// Muda status para enviar número cartão
					timeProg=0;								// Zera contador de tempo

					if (++cntTimeOut>1)						// Se tentou 2 vezes e não encontrou o server..
					{
						cntTimeOut=0;						// Zera contador de timeout
						STATPROG = _QUEUING;				// Muda status para "Imprimindo"
						timeProg=0;							// Zera contador de tempo
						numFrame=0;							// Inicializa o registro do frame enviado
						numFrameServer=0;					// Inicializa o registro do frame recebido
					}
				}
				else if (numFrame==numFrameServer)			// Compara frame enviado com recebido, se for
				{											// mesmo numero chegou resposta do frame.
					numFrame=0;								// Inicializa o registro do frame enviado
					numFrameServer=0;						// Inicializa o registro do frame recebido

					STATPROG = _QUEUING;					// Muda status para "Imprimindo"
					timeProg=0;								// Zera contador de tempo
				}				
			}
			else if (STATPROG == _QUEUING)					// _______STATUS "FAZENDO FILA"
			{	
				if (timeProg++==0)
				{
					if (flagDataClient==0)					// Não mostra mensagem qdo dado de cliente
					{}
				}
				else if (timeProg>100)
				{
					flagDataClient=0;						// Zera flag do cliente
					if (STATFILE==_FLWAIT)					// Verifica se conseguiu gravar na fila
					{
						if (Print (result))
						{
							STATPROG=_PRINTING;				// Status imprimindo o ticket
							timeProg = 0;
						}
						else
						{
							STATPROG=_WAITING;				// Status para aguardar novo cliente
							timeProg = 0;
						}
					}
				}
			}
			else if (STATPROG == _PRINTING)					// _______STATUS "IMPRIMINDO"
			{	
				if (timeProg++==20)
				{
//				}				
//				else if (timeProg>=400)
//				{
					STATPROG=_WAITING;						// Status para aguardar novo cliente
					timeProg = 0;
				}
			}
			else if (STATPROG == _UPDATING ||
					STATPROG == _UPDATINGREP)				// _______STATUS "SALVANDO EM ARQUIVO"
			{
				if (STATFILE!=_FLWAIT) 						// Verifica se conseguiu gravar arquivo	
				{
					if (timeProg++==0)
					{
					}
					else if (timeProg==400)
					{
						timeProg=0;
					}
					else if (timeProg==800)
						timeProg=0;
				}
				else
				{
					if (STATPROG==_UPDATINGREP)
					{
						if (cfg.zero==1)
							Format_DataFlash ();
					}
					STATPROG=_WAITING;						// Status para aguardar novo cliente
					timeProg = 0;
				}
			}

			Check_FileSystem ();							// Verifica se houve mudança de drive
			StateMachine_ticket ();							// Acompanha a gravação em FLASH

			tick=0;
		}
  	}
}

 /*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
