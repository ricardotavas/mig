/*----------------------------------------------------------------------------
 *      Biblioteca:	LPC176x_COMM.lib
 *      Proposito:	Biblioteca do protocolo WYMA
 *----------------------------------------------------------------------------
 *      Arquivo:    COMM_CONFIG.h
 *      Proposito: 	Arquivo de definições do usuário para o protocolo
 *----------------------------------------------------------------------------
 *      Versão: 	5.00 	Prog: Ricardo T.   Data: 2/3/2016
 * 		Descrição:  
 *----------------------------------------------------------------------------
 *      Este código pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#ifndef		__COMM_CONFIG__
#define		__COMM_CONFIG__

/*----------------------------------------------------------------------------
 *      WYMA Protocol definitions 
 *---------------------------------------------------------------------------*/
#define			STX					2 	// Início de frame
#define			ETX					3	// Fim do frame
#define			ACK					6	// Frame sem erros
#define 		DC1					17	// Set com valor fora do range
#define 		DC2					18	// Set com comprimento do valor incorreto
#define 		DC3					19	// Set em valor somente leitura
#define 		DC4					20	// Erro genérico (não especifico)
#define			NAK					21	// Erro na verificação de integridade do frame

/*----------------------------------------------------------------------------
 *      WYMA Protocol Class definitions 
 *---------------------------------------------------------------------------*/
#define			CLS_ACIONADOR		1
#define			CLS_PAINELSENHA		2
#define			CLS_EMISSORSENHA	3
#define			CLS_SOFTWARE		4
#define			CLS_TECLADO			5
#define			CLS_TECLADOOPINIAO	6
#define			CLS_PAINELMENSAGEM	7
#define			CLS_MIDIA			8
#define			CLS_MEDIDOR			9
#define			CLS_CRF				10
#define			CLS_CGF				11

/*----------------------------------------------------------------------------
 *      WYMA protocol definitions for commands and parameters
 *---------------------------------------------------------------------------*/
#define		COMM_NETCONFIG		1
#define			_DOMAINNAME			1
#define			_DHCP				2
#define			_LOCALIP			3
#define			_LOCALPORT			4
#define			_MASK				5		 
#define			_GATEWAY			6
#define			_DNS1				7
#define			_DNS2				8
#define			_MAC				9	  
#define			_PROTOCOL			10
#define			_TIMEOUT			11
#define			_TCPMODEL			12
#define			_IPCONNECT			13
#define			_CONNECTMODE		14
#define			_REMOTEPORT			15
#define			_REMOTEIP			16
#define			_PING				17
#define			_IPBC  				18
#define		COMM_UARTCONFIG		2
#define			_UARTMODE			1
#define			_FLOWCONTROL		2
#define			_UARTBAUD			3
#define			_UARTPARITY			4
#define			_UARTDATA			5
#define			_UARTSTOP			6
#define			_PROGRAMMODE		7
#define			_CHARESC			8
#define		COMM_USB			3
#define		COMM_CAN			4
#define		COMM_USER			5
#define			_AUTHPASSWORD		1
#define			_AUTHENABLE			2
#define			_LOGIN				3
#define			_LOGOUT				4
#define			_IDCLIENT			5
#define			_STAUSER			6
#define		COMM_DEVICE			6
#define			_DEVNAME			1
#define			_DEVVERSION			2
#define			_TIMEDATE			3
#define			_DATEFORMAT			4
#define			_DEVID				5
#define			_DEVINIT			6
#define			_RESTORE			7
#define			_DEVPOWER			8
#define			_DEVMODEL			9
#define			_DEVTIMEREP			10
#define			_DEVTIMEPOWER		11
#define			_TEMPERATURE		12
#define			_VOLTAGE			13
#define			_CALIBRATION		14
#define			_RF					15
#define		COMM_SOUND			7
#define			_TYPESOUND			1
#define			_VOLUME				2
#define		COMM_LCD			8
#define			_TYPELCD			1
#define			_BACKLIGHT			2
#define			_CONTRAST			3
#define		COMM_DRIVE			9
#define			_DEFDRV				1
#define			_FORMATDRV			2
#define		COMM_FILE			10
#define			_FILENAME			1
#define			_FILEDATA			2
#define			_LISTFILES			3
#define		COMM_DISPLAY		11
#define			_DISPLAYIP			1
#define			_DISPLAYTYPE		2
#define			_DISPLAYHEIGHT		3
#define			_DISPLAYNUMLIN		4
#define			_DISPLAYNUMDEC		5
#define			_DISPLAYBRIGHT		6
#define			_DISPLAYMIRROR		7
#define			_DISPLAYSHOW		8
#define			_DISPLAYINFO		9
#define			_DISPLAYDEFLIN		10
#define			_DISPLAYDEFCOL		11
#define			_DISPLAYNUMDIG		12
#define			_DISPLAYPOS			13
#define		COMM_TEST			12
#define			_TESTDISPLAY		1
#define			_TESTBEEP			2
#define			_TESTFLASH			3
#define			_TESTEEPROM			4
#define			_TESTKEY			5
#define			_TESTUART			6
#define			_TESTVOLUME			7
#define			_TESTLCD			8
#define			_TESTFLASHFS		9
#define		COMM_HARDWARE		13
#define			_IDBOARD			1
#define			_KEYSTATUS			2
#define		COMM_DATAFLASH		14
#define			_INITFLASH			1
#define			_FLASHDATA			2
#define			_FLASHTIMEATD		3
#define		COMM_TOP			50
#define			_INITSURVEY			1
#define			_SURVEYSTATUS		2
#define			_SURVEYNAME			3
#define			_SURVEYRESULT		4
#define			_SURVEYKEYCFG		5
#define			_SURVEYUSER			6
#define			_SURVEYREG			7
#define			_SURVEYINFTOP		8
#define			_SURVEYQUESTION		9
#define		COMM_TICKET			51
#define			_TICKETNUM			1
#define			_TICKETSTA			2
#define			_TICKETNEW			3
#define			_TICKETRENEW		4
#define			_TICKETALPHA		5
#define			_TICKETBOX			6
#define			_TICKETDEPDISP		7
#define			_TICKETDEPNAME		8
#define			_TICKETLAST			9
#define			_TICKETSEQ			10
#define			_TICKETSEQADJ		11
#define			_INFOATD			12
#define			_TICKETDEPUSER		13
#define			_TICKETOLD			14
#define			_TIMEATD			15
#define		COMM_PRICE			52
#define			_PRODNAME			1
#define			_PRODPRICE			2
#define			_PRICEDISPLAY		3
#define			_PRICEEFFECT		4
#define		COMM_JMIE			53
#define			_SEQUENCE			1
#define			_GROUPSEQ			2
#define			_NAMEMSG			3
#define		COMM_FISCAL			54
#define			_FISCALINFO			1
#define			_FISCALCALL			2
#define		COMM_CRF			55
#define			_INFORF				1
#define		COMM_MIG			56
#define			_REQPROTOCOL		1
#define			_PAPERSTATUS		2
#define			_DPTOSTATUS			3
#define		COMM_METER 			57
#define			_ALARMTIME			1
#define			_ALARMTEMP			2
#define			_ALARMSTATUS		3
#define			_SILENCETIME		4

/*----------------------------------------------------------------------------
 *      WYMA protocol interfaces definitions 
 *---------------------------------------------------------------------------*/
#define		ITF_NONE			0x00
#define 	ITF_TCP  			0x01
#define 	ITF_UDP  			0x02
#define		ITF_UART0			0x04
#define		ITF_UART1			0x08
#define		ITF_USB				0x10
#define		ITF_CAN				0x20
#define		ITF_WIFI			0x40

/*----------------------------------------------------------------------------
 *      Mask for user defitions
 *---------------------------------------------------------------------------*/
#define 	MASKCOMM_CLASS		0x000000FF
#define 	MASKCOMM_LENBUF		0x0003FF00
#define 	MASKCOMM_WIFISEND	0x0FFC0000

#define 	LOCALCLASS			(COMM_userOptions & MASKCOMM_CLASS)
#define 	LENBUF_COMM 		((COMM_userOptions & MASKCOMM_LENBUF)>>8)
#define 	LENBUF_WIFISEND		((COMM_userOptions & MASKCOMM_WIFISEND)>>18)

/*----------------------------------------------------------------------------
 *      Protocol exported variables
 *---------------------------------------------------------------------------*/

extern unsigned int 	COMM_userOptions;

extern unsigned char 	bufComm [];

/*----------------------------------------------------------------------------
 *      USB exported variables 
 *---------------------------------------------------------------------------*/

extern unsigned char InReport [64];
extern unsigned char OutReport [64];

/*----------------------------------------------------------------------------
 *      ETHERNET exported variables 
 *---------------------------------------------------------------------------*/

extern unsigned short	remPortUdp;
extern unsigned char	remIpUdp[4];
extern unsigned char	socket_udp;

/*----------------------------------------------------------------------------
 *      WIFI exported variables 
 *---------------------------------------------------------------------------*/
extern unsigned char	wifiExist;

extern unsigned short	remPortWifi;					// Porta Local WIFI UDP
extern unsigned char	remIpWifi[4];					// Ip Local WIFI UDP

extern char				wifiSsid[32];					// Nome da rede WIFI (preenchido por scan)
extern unsigned char 	wifiMac[6];						// Endereço MacAddress wifi
extern unsigned char 	wifiBSsid[6];					// Endereço MacAddress wifi do AP

extern unsigned char 	staRadio;						// Indica status do radio WIFI
extern unsigned char	staConnect;						// Indica status de conexão do wifi
extern unsigned char	staHttpsEn;						// Indica status de WEB server

/*----------------------------------------------------------------------------
 *      User type definitions for protocol WYMA
 *---------------------------------------------------------------------------*/

typedef struct frmRes
{
	unsigned char	idFrame;		// header - Identificação do frame para o remetente
	unsigned char	flag;			// header - indicação de erro
	unsigned short 	frame;			// header - número do frame
	unsigned short	frameNum;		// header - total de frames
	unsigned char	cmdType;		// header - tipo de comando (set, get ou response)
	unsigned char	cmdNum;			// header - número do comando
	unsigned char	cmdSub;			// header - sub-número do comando
	unsigned short	cmdIdx;			// header - index do comando
	unsigned short	dataLen;		// header - tamanho dos dados
	unsigned char	*dataBuf;		// Dados do frame
	unsigned char 	interface;		// Interface em que chegou o dado
}FRMRES;

/*----------------------------------------------------------------------------
 *      User type definitions for protocol NTP
 *---------------------------------------------------------------------------*/

typedef struct frmNTP
{
	unsigned char  	dstIP[4];		// destination address (local or remote)
	char    		leap;			// leap indicator
	char    		version;		// version number
	char    		mode;			// mode
	char    		stratum;		// stratum 
	char    		poll;			// poll interval 
	char  			precision;		// precision 
	unsigned int 	rootdelay;		// root delay 
	unsigned int 	rootdisp;		// root dispersion 
	unsigned int 	refid;			// reference ID 
	unsigned int	reftime;		// reference time - Somente parte inteira não armazeno parte decimal.. 
	unsigned int	org;			// origin timestamp - Somente parte inteira não armazeno parte decimal..
	unsigned int	rec;			// receive timestamp - Somente parte inteira não armazeno parte decimal..
	unsigned int	xmt;			// transmit timestamp - Somente parte inteira não armazeno parte decimal..
}FRMNTP;

/*----------------------------------------------------------------------------
 *      Exported functions
 *---------------------------------------------------------------------------*/

unsigned char Send_SetRequest (unsigned char interf, unsigned char lClass, unsigned char lId, 
								unsigned char rClass, unsigned char rId, unsigned char useCrc, 
								unsigned char retry, unsigned char cmdNum, unsigned char cmdSub, 
								unsigned short cmdIdx, unsigned short len, unsigned char *pData);
unsigned char Send_GetRequest (unsigned char interf, unsigned char lClass, unsigned char lId, 
								unsigned char rClass, unsigned char rId, unsigned char useCrc, 
								unsigned char retry, unsigned char cmdNum, unsigned char cmdSub, 
								unsigned short cmdIdx, unsigned char cmdParam);
extern void Send_Response (unsigned char flag, unsigned char *pData, unsigned short len);
extern void Check_Command (unsigned char interf, unsigned char *pData, unsigned short *pLen);

extern unsigned short udp_callback (unsigned char soc, unsigned char *rip, unsigned short rport, 
								unsigned char *buf, unsigned short len);
extern void uart_CheckProtocol (unsigned char uartPort);
extern void usbrx_callback (void);
extern void usbtx_callback (void);
extern void can_CheckProtocol (void);
extern void wifi_CheckProtocol (void);
extern void init_wifi (void);
extern void ResetHardWifi (void);
extern unsigned char radio_wifi (unsigned char sta);
extern unsigned char scan_wifi (unsigned char sta, unsigned char init);
extern unsigned char read_mac (void);
extern unsigned char config_wifi (unsigned char param, unsigned int ip, unsigned int mask, unsigned int gateway, unsigned char dhcp, unsigned int dns1, unsigned int dns2, unsigned char *mac);
extern unsigned char resume_wifi (char mode, unsigned char ch, unsigned char sec, char *ptSsid, char *ptPassword);
extern char checkAuth (char *user, char *password, char *authBase64);

extern void Send_Ntp (unsigned char *rip, unsigned int utcTime);

#endif

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
