/*----------------------------------------------------------------------------
 *      Arquivo:    USERCOMM.c
 *      Proposito: 	Definições do usuário para o driver de comunicação
 *----------------------------------------------------------------------------
 *      Versão: 	1.00 	Prog: Ricardo T.   Data: 25/04/2013
 * 		Descrição:  Versão inicial
 *----------------------------------------------------------------------------
 *      Este código pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#include		<Net_Config.h>
#include		"COMM_CONFIG.h"	
#include		"IAP_CONFIG.h"
#include		"RTC_CONFIG.h"
#include		"..\\QUEUE.h"

/*----------------------------------------------------------------------------
 *      App.c variables
 *---------------------------------------------------------------------------*/
extern char 			devName[];					// Nome do dipositivo
extern unsigned char 	versionFirm[];				// Versão do dispositivo
extern unsigned char	RESTARTSOFT;				// Flag para restart
extern CALL				call[];
extern unsigned char	runUser;
extern int 				erroPrint;
extern USERCALL			userCall[MAXUSER];

/*----------------------------------------------------------------------------
 *      NetConfig.c   variables
 *---------------------------------------------------------------------------*/
extern unsigned char  	http_auth_passw[20];
extern unsigned char   	http_EnAuth;
extern unsigned char	own_hw_adr[ETH_ADRLEN];
extern unsigned char    lhost_name[16];
extern LOCALM 			localm[];   
#define LocM   			localm[NETIF_ETH]   		// Local Machine Settings 

/*----------------------------------------------------------------------------
 *      Local variables
 *---------------------------------------------------------------------------*/
unsigned int 	numTicketServer=0;					// Número do ticket recebido do servidor
char			alphaServer;						// Letra do ticket recebida do servidor
unsigned char  	dptoServer;							// Departamento do ticket recebida do servidor
unsigned char	numFrameServer=0;					// Número do frame recebido do servidor
char			devNameServer[60];					// Nome do dispositivo recebido do servidor
unsigned char	PROGMODE=0;							// Flag do modo de programação
char 			forecastServer[6];					// Previsão de espera do ticket recebida do servidor
char 			atdProtocol[24]={0,0,0,0,0,0,0,0,	// Número do protocolo de atendimento do cliente
								 0,0,0,0,0,0,0,0,
								 0,0,0,0,0,0,0,0};
unsigned short	remoteKey=0;

/*----------------------------------------------------------------------------
 *      Extern function
 *---------------------------------------------------------------------------*/
extern void ShowForecast (unsigned char depNum, char *fcast);
extern char Test_Beep (void);
extern char Test_Flash (void);
extern char Test_Uart (unsigned char uart);
extern char Test_Lcd (void);
extern char Test_FlashFs (void);
extern void Write_Flash (void);

/*----------------------------------------------------------------------------
 *      Local functions
 *---------------------------------------------------------------------------*/
void ErrorUndefinedCommand (char cmdType);
void Response_InfoTicket (FRMRES fr, TKTINFO tkt, unsigned char userNum, unsigned char depNum);
unsigned short len (unsigned char *pDat);
unsigned char doArray (unsigned char *array, unsigned int value);

/*----------------------------------------------------------------------------
 * Funções:		cmd_DomainName
 * Descrição: 	Comando para tratamento do nome do domínio (local host name)
 * Parâmetros: 	fr - Estrutura com informações do frame recebido
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void cmd_DomainName (FRMRES fr)
{
	unsigned char i;

	if (PROGMODE==0)										// Se não estiver em modo programação
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (fr.cmdType=='S')								// Comando é requisição com "Set"
	{
		if (fr.dataLen<=15)									// Verifica comprimento dos dados
		{
			if (fr.interface==ITF_UDP)
			{
				for (i=0 ; i<fr.dataLen ; i++)				// Grava nome de domínio LAN
					lhost_name[i]=cfg.hostName[i]=*fr.dataBuf++;

				if (fr.dataLen==0)							// Comprimento zero = nenhum nome
					lhost_name[0]=cfg.hostName[0]=0;

				Write_Flash ();

				Send_Response (ACK,"",0);					// Responde com ACK - Sucesso
			}
		}
		else
			Send_Response (DC2,"",0);						// Responde com DC2-Comprimento incorreto
	}
	else if (fr.cmdType=='G')								// Comando é requisição com "Get"
	{
		if (fr.interface==ITF_UDP)
			Send_Response (ACK,lhost_name,len(lhost_name));	// Devolve nome de domínio LAN
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_DHCP
 * Descrição: 	Comando para tratamento do nome do dispositivo
 -----------------------------------------------------------------------------*/
void cmd_DHCP (FRMRES fr)
{
	if (PROGMODE==0)										// Se não estiver em modo programação
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (fr.cmdType=='S')								// Comando é requisição com "Set"
	{
		if (fr.dataLen==1) 									// Verifica comprimento dos dados
		{
			if (*fr.dataBuf==0 || *fr.dataBuf==1)			// Verifica resposta
			{
				if (fr.interface==ITF_UDP ||
				 	fr.interface==ITF_USB)
				{
					cfg.dhcpStatus=*fr.dataBuf;				// Grava dhcp LAN
					Write_Flash ();
				}

				Send_Response (ACK,"",0);					// Responde com ACK - Sucesso
			}
			else
				Send_Response (DC1,"",0);					// Responde com DC1-Valor fora do range
		}
		else
			Send_Response (DC2,"",0);						// Responde com DC2-Comprimento incorreto
	}
	else if (fr.cmdType=='G')								// Comando é requisição com "Get"
	{
		if (fr.interface==ITF_UDP  || 
			fr.interface==ITF_USB)
			Send_Response (ACK,&cfg.dhcpStatus,1);			// Devolve dhcp LAN
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_LocalIP
 * Descrição: 	Comando para tratamento do IP local
 -----------------------------------------------------------------------------*/
void cmd_LocalIP (FRMRES fr)
{
	if (PROGMODE==0)										// Se não estiver em modo programação
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (fr.cmdType=='S')								// Comando é requisição com "Set"
	{
		if (fr.dataLen==4) 									// Verifica comprimento dos dados
		{
			if (fr.interface==ITF_UDP  || 
				fr.interface==ITF_USB)
			{
				cfg.localIp[0]=*fr.dataBuf++; 				// Grava ip LAN
				cfg.localIp[1]=*fr.dataBuf++;
				cfg.localIp[2]=*fr.dataBuf++;
				cfg.localIp[3]=*fr.dataBuf;
				if (cfg.dhcpStatus==0)
				{											// Ativa novo IP se DHCP=desligado
					localm[NETIF_ETH].IpAdr[0]=cfg.localIp[0];	
					localm[NETIF_ETH].IpAdr[1]=cfg.localIp[1];
					localm[NETIF_ETH].IpAdr[2]=cfg.localIp[2];
					localm[NETIF_ETH].IpAdr[3]=cfg.localIp[3];
				}
				Write_Flash ();
			}
			Send_Response (ACK,"",0);						// Responde com ACK - Sucesso
		}
		else
			Send_Response (DC2,"",0);						// Responde com DC2-Comprimento incorreto
	}
	else if (fr.cmdType=='G')								// Comando é requisição com "Get"
	{
		if (fr.interface==ITF_UDP || 
			fr.interface==ITF_USB)
			Send_Response (ACK,localm[NETIF_ETH].IpAdr,4);	// Devolve ip LAN
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_LocalPort
 * Descrição: 	Comando para tratamento da porta ethernet local
 -----------------------------------------------------------------------------*/
void cmd_LocalPort (FRMRES fr)
{
	if (PROGMODE==0)										// Se não estiver em modo programação
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (fr.cmdType=='S')								// Comando é requisição com "Set"
		Send_Response (DC3,"",0);							// Responde com DC3-Valor somente leitura
	else if (fr.cmdType=='G')								// Comando é requisição com "Get"
		Send_Response (ACK, "\x63\x9E", 2);					// Devolve resposta com informação
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_Mask
 * Descrição: 	Comando para tratamento da máscara de rede
 -----------------------------------------------------------------------------*/
void cmd_Mask (FRMRES fr)
{
	if (PROGMODE==0)										// Se não estiver em modo programação
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (fr.cmdType=='S')								// Comando é requisição com "Set"
	{
		if (fr.dataLen==4) 									// Verifica comprimento dos dados
		{
			if (fr.interface==ITF_UDP  || 
				fr.interface==ITF_USB)
			{			
				cfg.ipMask[0]=*fr.dataBuf++; 				// Grava mascara LAN
				cfg.ipMask[1]=*fr.dataBuf++;
				cfg.ipMask[2]=*fr.dataBuf++;
				cfg.ipMask[3]=*fr.dataBuf;
				if (cfg.dhcpStatus==0)
				{											// Ativa nova máscara se DHCP=desligado
					localm[NETIF_ETH].NetMask[0]=cfg.ipMask[0];	
					localm[NETIF_ETH].NetMask[1]=cfg.ipMask[1];
					localm[NETIF_ETH].NetMask[2]=cfg.ipMask[2];
					localm[NETIF_ETH].NetMask[3]=cfg.ipMask[3];
				}
				Write_Flash ();
			}
			Send_Response (ACK,"",0);						// Responde com ACK - Sucesso
		}
		else
			Send_Response (DC2,"",0);						// Responde com DC2-Comprimento incorreto
	}
	else if (fr.cmdType=='G')								// Comando é requisição com "Get"
	{
		if (fr.interface==ITF_UDP  || 
			fr.interface==ITF_USB)
			Send_Response (ACK,localm[NETIF_ETH].NetMask,4);// Devolve mascara LAN
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_Gateway
 * Descrição: 	Comando para tratamento do gateway
 -----------------------------------------------------------------------------*/
void cmd_Gateway (FRMRES fr)
{
	if (PROGMODE==0)										// Se não estiver em modo programação
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (fr.cmdType=='S')								// Comando é requisição com "Set"
	{
		if (fr.dataLen==4) 									// Verifica comprimento dos dados
		{
			if (fr.interface==ITF_UDP  || 
				fr.interface==ITF_USB)
			{												// Grava gateway LAN
				localm[NETIF_ETH].DefGW[0]=cfg.gateway[0]=*fr.dataBuf++; 
				localm[NETIF_ETH].DefGW[1]=cfg.gateway[1]=*fr.dataBuf++;
				localm[NETIF_ETH].DefGW[2]=cfg.gateway[2]=*fr.dataBuf++;
				localm[NETIF_ETH].DefGW[3]=cfg.gateway[3]=*fr.dataBuf;
				Write_Flash ();
			}
			Send_Response (ACK,"",0);						// Responde com ACK - Sucesso
		}
		else
			Send_Response (DC2,"",0);						// Responde com DC2-Comprimento incorreto
	}
	else if (fr.cmdType=='G')								// Comando é requisição com "Get"
	{
		if (fr.interface==ITF_UDP  || 
			fr.interface==ITF_USB)
			Send_Response (ACK,localm[NETIF_ETH].DefGW,4);	// Devolve gateway LAN
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_Dns1
 * Descrição: 	Comando para tratamento do DNS preferêncial
 -----------------------------------------------------------------------------*/
void cmd_Dns1 (FRMRES fr)
{
	if (PROGMODE==0)										// Se não estiver em modo programação
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (fr.cmdType=='S')								// Comando é requisição com "Set"
	{
		if (fr.dataLen==4) 									// Verifica comprimento dos dados
		{
			if (fr.interface==ITF_UDP  || 
				fr.interface==ITF_USB)
			{												
				cfg.dns1[0]=*fr.dataBuf++; 					// Grava dns1 LAN
				cfg.dns1[1]=*fr.dataBuf++;
				cfg.dns1[2]=*fr.dataBuf++;
				cfg.dns1[3]=*fr.dataBuf;
				if (cfg.dhcpStatus==0)
				{											// Ativa novo dns1 se dhcp=desligado
					localm[NETIF_ETH].PriDNS[0]=cfg.dns1[0];	
					localm[NETIF_ETH].PriDNS[1]=cfg.dns1[1];
					localm[NETIF_ETH].PriDNS[2]=cfg.dns1[2];
					localm[NETIF_ETH].PriDNS[3]=cfg.dns1[3];
				}
				Write_Flash ();
			}
			Send_Response (ACK,"",0);						// Responde com ACK - Sucesso
		}
		else
			Send_Response (DC2,"",0);						// Responde com DC2-Comprimento incorreto
	}
	else if (fr.cmdType=='G')								// Comando é requisição com "Get"
	{
		if (fr.interface==ITF_UDP  || 
			fr.interface==ITF_USB)
			Send_Response (ACK,localm[NETIF_ETH].PriDNS,4);	// Devolve dns1 LAN
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_Dns2
 * Descrição: 	Comando para tratamento do DNS alternativo
 -----------------------------------------------------------------------------*/
void cmd_Dns2 (FRMRES fr)
{
	if (PROGMODE==0)										// Se não estiver em modo programação
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (fr.cmdType=='S')								// Comando é requisição com "Set"
	{
		if (fr.dataLen==4) 									// Verifica comprimento dos dados
		{
			if (fr.interface==ITF_UDP  || 
				fr.interface==ITF_USB)
			{												
				cfg.dns2[0]=*fr.dataBuf++; 					// Grava dns2 LAN
				cfg.dns2[1]=*fr.dataBuf++;
				cfg.dns2[2]=*fr.dataBuf++;
				cfg.dns2[3]=*fr.dataBuf;
				if (cfg.dhcpStatus==0)
				{											// Ativa novo dns2 se dhcp=desligado
					localm[NETIF_ETH].SecDNS[0]=cfg.dns2[0]; 	
					localm[NETIF_ETH].SecDNS[1]=cfg.dns2[1];
					localm[NETIF_ETH].SecDNS[2]=cfg.dns2[2];
					localm[NETIF_ETH].SecDNS[3]=cfg.dns2[3];
				}
				Write_Flash ();
			}
			Send_Response (ACK,"",0);						// Responde com ACK - Sucesso
		}
		else
			Send_Response (DC2,"",0);						// Responde com DC2-Comprimento incorreto
	}
	else if (fr.cmdType=='G')								// Comando é requisição com "Get"
	{
		if (fr.interface==ITF_UDP  || 
			fr.interface==ITF_USB)
			Send_Response (ACK,localm[NETIF_ETH].SecDNS,4);	// Devolve dns2 LAN
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_Mac
 * Descrição: 	Comando para tratamento do MAC Adress ethernet
 -----------------------------------------------------------------------------*/
void cmd_Mac (FRMRES fr)
{
 	if (PROGMODE==0)										// Se não estiver em modo programação
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (fr.cmdType=='S')								// Comando é requisição com "Set"
	{
		if (fr.dataLen==10)									// Verifica comprimento dos dados
		{
			if ((*fr.dataBuf++==0xFE) &&
				(*fr.dataBuf++==0xFD) &&
				(*fr.dataBuf++==0xFC) &&
				(*fr.dataBuf++==0xFB))
			{
				if (fr.interface==ITF_UDP  || 
					fr.interface==ITF_USB)
				{
					cfg.macAddress[0]=*fr.dataBuf++;		// Grava novo mac LAN
					cfg.macAddress[1]=*fr.dataBuf++;		// Precisa reiniciar para atualizar valor
					cfg.macAddress[2]=*fr.dataBuf++;		// da variável: own_hw_adr
					cfg.macAddress[3]=*fr.dataBuf++;
					cfg.macAddress[4]=*fr.dataBuf++;
					cfg.macAddress[5]=*fr.dataBuf;
					Write_Flash ();
				}
			
				Send_Response (ACK,"",0);					// Responde com ACK - Sucesso
				return;
			}
			Send_Response (DC4, "", 0);						// Responde com DC4-Erro não especificado
		}
		else
			Send_Response (DC2,"",0);						// Responde com DC2-Comprimento incorreto
	}
	else if (fr.cmdType=='G')								// Comando é requisição com "Get"
	{
		if (fr.interface==ITF_UDP  || 
			fr.interface==ITF_USB)
			Send_Response (ACK,own_hw_adr,6);				// Devolve mac LAN
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_Protocol
 * Descrição: 	Comando para tratamento do protocolo (UDP/TCP) 
 * 				Não retirar (resposta para DEVNET)
 -----------------------------------------------------------------------------*/
void cmd_Protocol (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_TimeOut
 * Descrição: 	Comando para tratamento do Time-out TCP
 * 				Não retirar (resposta para DEVNET)
 -----------------------------------------------------------------------------*/
void cmd_TimeOut (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_TcpModel
 * Descrição: 	Comando para tratamento de roteamento (Server=0/client=1)
 * 				Não retirar (resposta para DEVNET)
 -----------------------------------------------------------------------------*/
void cmd_TcpModel (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_IpConnect
 * Descrição: 	Comando para tratamento de conexão (0=qualquer IP / 1=Ip destino)
 * 				Não retirar (resposta para DEVNET)
 -----------------------------------------------------------------------------*/
void cmd_IpConnect (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_ConnectMode
 * Descrição: 	Comando para tratamento do modo de conexão ethernet
 * 				Não retirar (resposta para DEVNET)
 -----------------------------------------------------------------------------*/
void cmd_ConnectMode (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_RemotePort
 * Descrição: 	Comando para tratamento da porta ethernet de destino
 * 				Não retirar (resposta para DEVNET)
 -----------------------------------------------------------------------------*/
void cmd_RemotePort (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_RemoteID
 * Descrição: 	Comando para tratamento do IP de destino
 * 				Não retirar (resposta para DEVNET)
 -----------------------------------------------------------------------------*/
void cmd_RemoteIP (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_Ping
 * Descrição: 	Comando para tratamento das informações do dispositivo
 -----------------------------------------------------------------------------*/
void cmd_Ping (FRMRES fr)
{
	unsigned char i;
	unsigned char data[60];
	unsigned char lenData=0;

	if (PROGMODE==0)										// Não deve estar em modo programação
	{
		if (fr.cmdType=='G')								// Monta resposta ao "Get deviceType"
		{
			if (fr.interface==ITF_UDP)			   			// Quando interface UDP:
			{
				data[lenData++]=localm[NETIF_ETH].IpAdr[0];	// Monta resposta com IP/MAC
				data[lenData++]=localm[NETIF_ETH].IpAdr[1];
				data[lenData++]=localm[NETIF_ETH].IpAdr[2];
				data[lenData++]=localm[NETIF_ETH].IpAdr[3];
				data[lenData++]=own_hw_adr[0];
				data[lenData++]=own_hw_adr[1];
				data[lenData++]=own_hw_adr[2];
				data[lenData++]=own_hw_adr[3];
				data[lenData++]=own_hw_adr[4];
				data[lenData++]=own_hw_adr[5];
			}
			data[lenData++]=versionFirm[0];	  				// Monta resposta com Versão/Nome
			data[lenData++]=versionFirm[1];
			data[lenData++]=versionFirm[2];
			data[lenData++]=versionFirm[3];
			for (i=0 ; devName[i]!=0 && i<6 ; i++)
				data[lenData++]=devName[i];

			Send_Response (ACK, data, lenData);				// Devolve resposta		
		}
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_IpBroadcast
 * Descrição: 	Comando para tratamento da configuração do IP via broadcast com MAC 
 -----------------------------------------------------------------------------*/
void cmd_IpBroadcast (FRMRES fr)
{
	if (fr.cmdType=='S')									// Comando é requisição com "Set"
	{
		if (fr.dataLen==10)									// Verifica comprimento dos dados
		{
			if (fr.interface==ITF_UDP)
			{
				if ((*fr.dataBuf++==own_hw_adr[0]) &&		// Verifica o MAC adrress LAN
					(*fr.dataBuf++==own_hw_adr[1]) &&
					(*fr.dataBuf++==own_hw_adr[2]) &&
					(*fr.dataBuf++==own_hw_adr[3]) &&
					(*fr.dataBuf++==own_hw_adr[4]) &&
					(*fr.dataBuf++==own_hw_adr[5]))
				{											// Grava novo IP LAN
					localm[NETIF_ETH].IpAdr[0]=cfg.localIp[0]=*fr.dataBuf++; 
					localm[NETIF_ETH].IpAdr[1]=cfg.localIp[1]=*fr.dataBuf++;
					localm[NETIF_ETH].IpAdr[2]=cfg.localIp[2]=*fr.dataBuf++;
					localm[NETIF_ETH].IpAdr[3]=cfg.localIp[3]=*fr.dataBuf;
					Write_Flash ();

					Send_Response (ACK,"",0);				// Responde com ACK - Sucesso
				}
			}
		}													// Não envia outras resposta 
	}														// devido a ser broadcast..
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_UartMode
 * Descrição: 	Comando para tratamento da UART
 * 				Não retirar (resposta para DEVNET)  
 -----------------------------------------------------------------------------*/
void cmd_UartMode (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_FlowControl
 * Descrição: 	Comando para tratamento da UART
 * 				Não retirar (resposta para DEVNET)
 -----------------------------------------------------------------------------*/
void cmd_FlowControl (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_UartBaud
 * Descrição: 	Comando para tratamento da UART
 * 				Não retirar (resposta para DEVNET)
 -----------------------------------------------------------------------------*/
void cmd_UartBaud (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_UartParity
 * Descrição: 	Comando para tratamento da UART
 * 				Não retirar (resposta para DEVNET)
 -----------------------------------------------------------------------------*/
void cmd_UartParity (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_UartData
 * Descrição: 	Comando para tratamento da UART
 * 				Não retirar (resposta para DEVNET)
 -----------------------------------------------------------------------------*/
void cmd_UartData (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_UartStop
 * Descrição: 	Comando para tratamento da UART
 * 				Não retirar (resposta para DEVNET)
 -----------------------------------------------------------------------------*/
void cmd_UartStop (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_ProgramMode
 * Descrição: 	Comando para tratamento da UART
 * 				Não retirar (resposta para DEVNET)
 -----------------------------------------------------------------------------*/
void cmd_ProgramMode (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_CharEscape
 * Descrição: 	Comando para tratamento da UART
 * 				Não retirar (resposta para DEVNET)
 -----------------------------------------------------------------------------*/
void cmd_CharEscape (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_Password
 * Descrição: 	Comando para tratamento da senha da autenticação
 -----------------------------------------------------------------------------*/
void cmd_Password (FRMRES fr)
{
	unsigned char i;

	if (PROGMODE==0)										// Se não estiver em modo programação
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (fr.cmdType=='S')								// Comando é requisição com "Set"
	{
		if (fr.dataLen<=10)									// Verifica comprimento dos dados
		{
			for (i=0 ; i<fr.dataLen ; i++)					// Grava novo valor
				http_auth_passw[i]=cfg.authPassword[i]=*fr.dataBuf++;

			if (fr.dataLen==0)								// comprimento zero = nenhum password
				http_auth_passw[0]=cfg.authPassword[0]=0;

			Write_Flash ();
			Send_Response (ACK,"",0);						// Responde com ACK - Sucesso
		}
		else
			Send_Response (DC2,"",0);						// Responde com DC2-Comprimento incorreto
	}
	else if (fr.cmdType=='G')								// Comando é requisição com "Get"
		Send_Response (ACK, http_auth_passw, len(http_auth_passw));	// Responde com informação requisitada
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_Authentic
 * Descrição: 	Comando para tratamento do status da autenticação
 -----------------------------------------------------------------------------*/
void cmd_Authentic (FRMRES fr)
{
	if (fr.cmdType=='S')								// Comando é requisição com "Set"
	{
		if (fr.dataLen==1) 									// Verifica comprimento dos dados
		{
			if (*fr.dataBuf==0 || *fr.dataBuf==1)			// Verifica resposta
			{
				http_EnAuth=cfg.authEnable=*fr.dataBuf;		// Grava novo valor
				Write_Flash ();
				Send_Response (ACK, "", 0);					// Responde com ACK - Sucesso
			}
			else
				Send_Response (DC1, "", 0);					// Responde com DC1-Valor fora do range
		}
		else
			Send_Response (DC2, "", 0);						// Responde com DC2-Comprimento incorreto
	}
	else if (fr.cmdType=='G')								// Comando é requisição com "Get"
		Send_Response (ACK, &http_EnAuth, 1);				// Responde com informação requisitada
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_Login
 * Descrição: 	Comando para tratamento do Login do usuário para configuração
 -----------------------------------------------------------------------------*/
void cmd_Login (FRMRES fr)
{
	unsigned char i;

	if (fr.cmdType=='S')									// Comando é requisição com "Set"
	{
		if (http_EnAuth)
		{
			if (fr.dataLen<=10)								// Verifica comprimento dos dados
			{
				if (http_auth_passw[i]==0)					// password configurado? se não estiver..
				{
					PROGMODE=1;								// Habilita modo programação
					Send_Response (ACK, "", 0);				// Responde com ACK - Sucesso
				}
				else
				{
					for (i=0 ; i<fr.dataLen ; i++)			// Compara password
						if (http_auth_passw[i]!=*fr.dataBuf++)
						{
							Send_Response (DC4, "", 0);		// Responde com DC4-Erro não especificado
							return;							
						}
	
					if (http_auth_passw[i]==0)				// Verifica se comparou todos os caracteres
					{
						PROGMODE=1;							// Habilita modo programação
						Send_Response (ACK, "", 0);			// Responde com ACK - Sucesso
					}
					else
						Send_Response (DC4, "", 0);			// Responde com DC4-Erro não especificado
				}
			}
			else
				Send_Response (DC2, "", 0);					// Responde com DC2-Comprimento incorreto
		}
		else												// Autenticação desligada
		{
			PROGMODE=1;										// Habilita modo programação
			Send_Response (ACK, "", 0);						// Responde com ACK - Sucesso
		}
	}
	else if (fr.cmdType=='G')								// Comando é requisição com "Get"
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_Logout
 * Descrição: 	Comando para tratamento do Logout do usuário para configuração
 -----------------------------------------------------------------------------*/
void cmd_Logout (FRMRES fr)
{
	if (fr.cmdType=='S')									// Comando é requisição com "Set"
	{
		PROGMODE=0;
		Send_Response (ACK, "", 0);							// Responde com ACK - Sucesso
	}
	else if (fr.cmdType=='G')								// Comando é requisição com "Get"
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_IdClient
 * Descrição: 	Comando para tratamento do 
 -----------------------------------------------------------------------------*/
void cmd_IdClient (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_staUser
 * Descrição: 	Comando para tratamento do 
 -----------------------------------------------------------------------------*/
void cmd_staUser (FRMRES fr)
{
	if (fr.cmdType=='S')									// Comando é requisição com "Set"
	{
		if (fr.dataLen==1)									// Verifica comprimento dos dados
		{
			if (fr.cmdIdx>0 && fr.cmdIdx<100)
			{
				userCall[fr.cmdIdx-1].sta = *fr.dataBuf;
				Send_Response (ACK, "", 0);					// Responde com ACK - Sucesso
			}
			else 
				Send_Response (DC4, "", 0);					// Responde com DC4-Erro não especificado
		}
		else
			Send_Response (DC2, "", 0);						// Responde com DC2-Comprimento incorreto
	}
	else if (fr.cmdType=='G')								// Comando é requisição com "Get"
		Send_Response (ACK, &userCall[fr.cmdIdx-1].sta, 1);	// Responde com DC4-Erro não especificado
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_DeviceName
 * Descrição: 	Comando para tratamento do nome do dispositivo 
 -----------------------------------------------------------------------------*/
void cmd_DeviceName (FRMRES fr)
{
	unsigned short i;
	if (fr.cmdType=='R')
	{
		if (fr.flag==ACK)									// Verifica se a resposta chegou sem erros
		{
			numFrameServer = fr.idFrame;
			if (fr.dataLen<=60)
			{
				for (i=0;i<60;i++)
					devNameServer[i]=0;

				for (i=0 ; i<fr.dataLen ; i++)
					devNameServer[i]=*fr.dataBuf++;
			}		
		}
	}
	else if (fr.cmdType=='S')								// Monta resposta ao "Set deviceType"
	{
		Send_Response (DC3,"",0);							// Responde com DC3-Valor somente leitura
	}
	else if (fr.cmdType=='G')								// Monta resposta ao "Get deviceType"
	{
		Send_Response (ACK, (unsigned char *)devName, 6);	// Devolve resposta
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_DeviceVersion
 * Descrição: 	Comando para tratamento da versão do firmware 
 -----------------------------------------------------------------------------*/
void cmd_DeviceVersion (FRMRES fr)
{
	if (fr.cmdType=='S')									// Comando é requisição com "Set"
		Send_Response (DC3, "", 0);							// Responde com DC3-Valor somente leitura
	else if (fr.cmdType=='G')								// Comando é requisição com "Get"
		Send_Response (ACK, versionFirm, 2);				// Responde com informação requisitada
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_TimeDate
 * Descrição: 	Comando para tratamento de data e hora 
 -----------------------------------------------------------------------------*/
void cmd_TimeDate (FRMRES fr)
{
	unsigned char 	array[7]={0,0,0,0,0,0,0};
	TIME_TYPE time;

	if (fr.cmdType=='S')									// Comando é requisição com "Set"
	{
		if (fr.dataLen==7)
		{
			time.HOUR = *fr.dataBuf++;
			time.MINUTE = *fr.dataBuf++;
			time.SECOND = *fr.dataBuf++;
			time.DAY = *fr.dataBuf++;
			time.MONTH = *fr.dataBuf++;
			time.YEAR = (unsigned short)*fr.dataBuf++;
			time.YEAR |= (unsigned short)((*fr.dataBuf)<<8);

			time.WEEKDAY = DayOfWeek(time);
			time.YEARDAY = DayOfYear(time);

			Save_RTC(time);

			Send_Response (ACK, "", 0);						// Responde com ACK
		}
		else
			Send_Response (DC2, "", 0);						// Responde com DC2-Comprimento incorreto
	}
	else if (fr.cmdType=='G')								// Comando é requisição com "Get"
	{
		time=Get_RTC();
		array[0]=time.HOUR;
		array[1]=time.MINUTE;
		array[2]=time.SECOND;
		array[3]=time.DAY;
		array[4]=time.MONTH;
		array[5]=(unsigned char)(time.YEAR&0xFF);
		array[6]=(unsigned char)((time.YEAR>>8)&0xFF);
		Send_Response (ACK, array, 7);						// Responde com data e hora
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_FormatDate
 * Descrição: 	Comando para tratamento do 
 -----------------------------------------------------------------------------*/
void cmd_FormatDate (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_DeviceId
 * Descrição: 	Comando para tratamento do 
 -----------------------------------------------------------------------------*/
void cmd_DeviceId (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_DevInit
 * Descrição: 	Comando para tratamento da inicialização do dispositivo
 -----------------------------------------------------------------------------*/
void cmd_DevInit (FRMRES fr)
{
	if (fr.cmdType=='S')									// Comando é requisição com "Set"
	{
		RESTARTSOFT=1;										// Sinaliza para reiniciar dispositivo
		Send_Response (ACK, "", 0);							// Responde com ACK
	}
	else if (fr.cmdType=='G')								// Comando é requisição com "Get"
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_Restore
 * Descrição: 	Comando para tratamento da restauração dos parâmetros de fábrica
 -----------------------------------------------------------------------------*/
void cmd_Restore (FRMRES fr)
{
	if (PROGMODE==0)										// Se não estiver em modo programação
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado	
	else if (fr.cmdType=='S')								// Comando é requisição com "Set"
	{
		InitAppConfig ();									// Inicializa configurações do software
		InitClient();										// Inicializa configuração do cliente

		Write_Flash ();										// Grava configurações em FLASH

		Send_Response (ACK, "", 0);							// Responde com ACK
	}
	else if (fr.cmdType=='G')								// Comando é requisição com "Get"
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_DevPower
 * Descrição: 	Comando para tratamento da energia do dispositivo
 -----------------------------------------------------------------------------*/
void cmd_DevPower (FRMRES fr)
{
	if (fr.cmdType=='S')									// Monta resposta ao "Set Display Model"
	{
		if (fr.dataLen==1)									// Modelo do display deve ter 1 byte
		{
			if (*fr.dataBuf<2)								// Verifica valor
			{
				cfg.energyStatus = *fr.dataBuf;
				Write_Flash ();

				Send_Response (ACK, "", 0);					// Devolve ACK
			}
			else
				Send_Response (DC4, "", 0);					// Responde com DC4-Erro não especificado
		}
		else
			Send_Response (DC2,"",0);						// Responde com DC2-Comprimento incorreto
	}
	else if (fr.cmdType=='G')								// Monta resposta ao "Get Display Model"
	{
		Send_Response (ACK, &cfg.energyStatus, 1);			// Responde com valor requisitado
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_DevModel
 * Descrição: 	Comando para troca do modelo do dispositivo
 -----------------------------------------------------------------------------*/
void cmd_DevModel (FRMRES fr)
{
	if (fr.cmdType=='S')									// Monta resposta ao "Set Display Model"
	{
		if (fr.dataLen==1)									// Modelo do display deve ter 1 byte
		{
			if (*fr.dataBuf>=1 && *fr.dataBuf<=99)			// Verifica valor
			{
				if (fr.cmdIdx==0)							// Não usa index o modelo é único
				{
					cfg.devModel = *fr.dataBuf;
	
					Write_Flash ();

					Send_Response (ACK, "", 0);				// Devolve ACK
				}
				else
					Send_Response (DC4, "", 0);				// Responde com DC4-Erro não especificado
			}
			else
				Send_Response (DC4, "", 0);					// Responde com DC4-Erro não especificado
		}
		else
			Send_Response (DC2,"",0);						// Responde com DC2-Comprimento incorreto
	}
	else if (fr.cmdType=='G')								// Monta resposta ao "Get Display Model"
	{
		Send_Response (ACK, &cfg.devModel, 1);				// Responde com valor requisitado
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_DevTimeRep
 * Descrição: 	Comando para tratamento de horário de gerar relatório
 -----------------------------------------------------------------------------*/
void cmd_DevTimeRep (FRMRES fr)
{
	unsigned char 	array[2]={0,0};

	if (fr.cmdType=='S')									// Comando é requisição com "Set"
	{
		if (fr.dataLen==2)
		{
			cfg.timeHourRep = *fr.dataBuf++;
			cfg.timeMinRep = *fr.dataBuf;

			Write_Flash ();

			Send_Response (ACK, "", 0);						// Responde com ACK
		}
		else
			Send_Response (DC2, "", 0);						// Responde com DC2-Comprimento incorreto
	}
	else if (fr.cmdType=='G')								// Comando é requisição com "Get"
	{
		array[0] = cfg.timeHourRep;
		array[1] = cfg.timeMinRep;
		Send_Response (ACK, array, 2);						// Responde com horário do relatório
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_DevTimePower
 * Descrição: 	Comando para tratamento de horário de operação
 -----------------------------------------------------------------------------*/
void cmd_DevTimePower (FRMRES fr)
{
	unsigned char 	array[6]={0,0,0,0,0,0};

	if (fr.cmdType=='S')									// Comando é requisição com "Set"
	{
		if (fr.dataLen==6)
		{
			cfg.enTimeAuto = *fr.dataBuf++;
			cfg.timeHourOn = *fr.dataBuf++;
			cfg.timeMinOn = *fr.dataBuf++;
			cfg.timeHourOff = *fr.dataBuf++;
			cfg.timeMinOff = *fr.dataBuf++;
			cfg.weekDay = *fr.dataBuf;

			Send_Response (ACK, "", 0);						// Responde com ACK
		}
		else
			Send_Response (DC2, "", 0);						// Responde com DC2-Comprimento incorreto
	}
	else if (fr.cmdType=='G')								// Comando é requisição com "Get"
	{
		array[0]=cfg.enTimeAuto;
		array[1]=cfg.timeHourOn;
		array[2]=cfg.timeMinOn;
		array[3]=cfg.timeHourOff;
		array[4]=cfg.timeMinOff;
		array[5]=cfg.weekDay;

		Send_Response (ACK, array, 6);						// Responde com data e hora
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_Temperature
 * Descrição: 	Comando para tratamento da temperatura
 -----------------------------------------------------------------------------*/
void cmd_Temperature (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_Voltage
 * Descrição: 	Comando para tratamento de voltagem
 -----------------------------------------------------------------------------*/
void cmd_Voltage (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_Calibration
 * Descrição: 	Comando para tratamento de voltagem
 -----------------------------------------------------------------------------*/
void cmd_Calibration (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_Rf
 * Descrição: 	Comando para tratamento de Rf 315MHZ
 -----------------------------------------------------------------------------*/
void cmd_Rf (FRMRES fr)
{
	if (fr.cmdType=='S')									// Comando é requisição com "Set"
	{
		if (fr.dataLen==1)
		{
			cfg.idRf = *fr.dataBuf++;
			Write_Flash ();

			Send_Response (ACK, "", 0);						// Responde com ACK
		}
		else
			Send_Response (DC2, "", 0);						// Responde com DC2-Comprimento incorreto
	}
	else if (fr.cmdType=='G')								// Comando é requisição com "Get"
	{
		Send_Response (ACK, &cfg.idRf, 1);					// Responde com código RF
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_TypeSound
 * Descrição: 	Comando para tratamento do tipo de som 
 -----------------------------------------------------------------------------*/
void cmd_TypeSound (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_Volume
 * Descrição: 	Comando para tratamento do volume do dom 
 -----------------------------------------------------------------------------*/
void cmd_Volume (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_TypeLcd
 * Descrição: 	Comando para tratamento do 
 -----------------------------------------------------------------------------*/
void cmd_TypeLcd (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_Backlight
 * Descrição: 	Comando para tratamento do 
 -----------------------------------------------------------------------------*/
void cmd_Backlight (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_Contrast
 * Descrição: 	Comando para tratamento do 
 -----------------------------------------------------------------------------*/
void cmd_Contrast (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_DefineDrive
 * Descrição: 	Comando para tratamento do 
 -----------------------------------------------------------------------------*/
void cmd_DefineDrive (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_FormatDrive
 * Descrição: 	Comando para tratamento do 
 -----------------------------------------------------------------------------*/
void cmd_FormatDrive (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_FileName
 * Descrição: 	Comando para tratamento do 
 -----------------------------------------------------------------------------*/
void cmd_FileName (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_FileData
 * Descrição: 	Comando para tratamento do 
 -----------------------------------------------------------------------------*/
void cmd_FileData (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}


/*----------------------------------------------------------------------------
 * Funções:		cmd_ListFiles
 * Descrição: 	Comando para tratamento do 
 -----------------------------------------------------------------------------*/
 void cmd_ListFiles (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_ipDisplay
 * Descrição: 	Comando para tratamento de solicitação do IP do display do dpto
 -----------------------------------------------------------------------------*/
void cmd_DisplayIp (FRMRES fr)
{
	if (PROGMODE==1)										// Se estiver em modo programação
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (fr.cmdType=='S')								// Monta resposta ao "Set ipDisplay"
	{
		Send_Response (DC3,"",0);							// Responde com DC3-Valor somente leitura
	}
	else if (fr.cmdType=='G')								// Monta resposta ao "Get ipDisplay"
	{
		unsigned char array[6]={0,0,0,0,0,0};
		unsigned char userNum=fr.cmdIdx;
		unsigned char depNum=0;

		if (userNum>=1 && userNum<=MAXUSER)
		{
			if (fr.dataLen==1)
				depNum=*fr.dataBuf;
			else			
				depNum=cfg.depUsers[userNum-1];

			array[0]=cfg.depIpDisp[depNum-1][0];
			array[1]=cfg.depIpDisp[depNum-1][1];
			array[2]=cfg.depIpDisp[depNum-1][2];
			array[3]=cfg.depIpDisp[depNum-1][3];
			array[4]=cfg.depRowDisp[depNum-1];
			array[5]=cfg.depItfDisp[depNum-1];

			Send_Response (ACK, array, 6);					// Devolve resposta com informação
		}
		else
			Send_Response (DC4, "", 0);						// Responde com DC4-Erro não especificado
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_DisplayModel
 * Descrição: 	Comando para tratamento do modelo do display
 -----------------------------------------------------------------------------*/
void cmd_DisplayType (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_DisplayHeight
 * Descrição: 	Comando para tratamento da altura do display
 -----------------------------------------------------------------------------*/
void cmd_DisplayHeight (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_DisplayNumLin
 * Descrição: 	Comando para tratamento do número de linhas
 -----------------------------------------------------------------------------*/
void cmd_DisplayNumLin (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_DisplayPos
 * Descrição: 	Comando para tratamento da posição
 -----------------------------------------------------------------------------*/
void cmd_DisplayPos (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_DisplayNumDec
 * Descrição: 	Comando para tratamento do número de decimais
 -----------------------------------------------------------------------------*/
void cmd_DisplayNumDec (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_DisplayBright
 * Descrição: 	Comando para tratamento do brilho do display
 -----------------------------------------------------------------------------*/
void cmd_DisplayBright (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_DisplayMirror
 * Descrição: 	Comando para tratamento do espelho do display
 -----------------------------------------------------------------------------*/
void cmd_DisplayMirror (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_DisplayShow
 * Descrição: 	Comando para mostrar mensagem no display
 -----------------------------------------------------------------------------*/
void cmd_DisplayShow (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_DisplayInfo
 * Descrição: 	Comando para selecionar as informações no display
 -----------------------------------------------------------------------------*/
void cmd_DisplayInfo (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_DisplayDefLin
 * Descrição: 	Comando para tratamento da quantidade de linhas na definição do display
 -----------------------------------------------------------------------------*/
void cmd_DisplayDefLin (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_DisplayDefCol
 * Descrição: 	Comando para tratamento da quantidade de colunas na definição do display
 -----------------------------------------------------------------------------*/
void cmd_DisplayDefCol (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_DisplayNumDig
 * Descrição: 	Comando para tratamento do número de dígitos
 -----------------------------------------------------------------------------*/
void cmd_DisplayNumDig (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_TestDisplay
 * Descrição: 	Comando para tratamento do teste do display 
 -----------------------------------------------------------------------------*/
void cmd_TestDisplay (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_TestFlash
 * Descrição: 	Comando para tratamento do teste da memória flash 
 -----------------------------------------------------------------------------*/
void cmd_TestFlash (FRMRES fr)
{
	unsigned char result=0;

	if (PROGMODE==0)										// Se não estiver logado
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (fr.cmdType=='S')								// Monta resposta ao "Set TestFlash"
		Send_Response (DC3, "", 0);							// Responde com DC3-Valor somente leitura
	else if (fr.cmdType=='G')								// Monta resposta ao "Get TestFlash"
	{
		result=Test_Flash ();
		Send_Response (ACK, &result, 1);					// Responde com valor requisitado
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_TestEeprom
 * Descrição: 	Comando para tratamento do teste da memória EEPROM
 -----------------------------------------------------------------------------*/
void cmd_TestEeprom (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_TestKey
 * Descrição: 	Comando para tratamento do teste das teclas 
 -----------------------------------------------------------------------------*/
void cmd_TestKey (FRMRES fr)
{
	if (PROGMODE==0)										// Se não estiver logado
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (fr.cmdType=='S')								// Monta resposta ao "Set TestKey"
	{
		Send_Response (ACK, "", 0);							// Responde com ACK
	}
	else if (fr.cmdType=='G')								// Monta resposta ao "Get TestKey"
		Send_Response (ACK, "\x0\x0", 2);					// Responde com valor requisitado
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_TestUart
 * Descrição: 	Comando para tratamento do teste da UART 
 -----------------------------------------------------------------------------*/
void cmd_TestUart (FRMRES fr)
{
	unsigned char result=0;

	if (PROGMODE==0)										// Se não estiver logado
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (fr.cmdType=='S')								// Monta resposta ao "Set TestUart"
		Send_Response (DC3, "", 0);							// Responde com DC3-Valor somente leitura
	else if (fr.cmdType=='G')								// Monta resposta ao "Get TestUart"
	{
		if (fr.cmdIdx==0 || fr.cmdIdx==1)					// Index: 0=uart0 1=uart1
		{		
			result=Test_Uart (fr.cmdIdx);	 
			Send_Response (ACK, &result, 1);				// Responde com valor requisitado
		}
		else
			Send_Response (DC4,"",0);						// Responde com DC4-erro não especif.
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_TestBeep
 * Descrição: 	Comando para tratamento do teste do beep
 -----------------------------------------------------------------------------*/
void cmd_TestBeep (FRMRES fr)
{
	unsigned char result=0;

	if (PROGMODE==0)										// Se não estiver logado
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (fr.cmdType=='S')								// Monta resposta ao "Set TestBeep"
		Send_Response (DC3, "", 0);							// Responde com DC3-Valor somente leitura
	else if (fr.cmdType=='G')								// Monta resposta ao "Get TestBeep"
	{
		result=Test_Beep ();
		Send_Response (ACK, &result, 1);					// Responde com valor requisitado
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_TestVolume
 * Descrição: 	Comando para tratamento do teste do buzzer
 -----------------------------------------------------------------------------*/
void cmd_TestVolume (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_TestLcd
 * Descrição: 	Comando para tratamento do teste do LCD
 -----------------------------------------------------------------------------*/
void cmd_TestLcd (FRMRES fr)
{
	unsigned char result=0;

	if (PROGMODE==0)										// Se não estiver logado
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (fr.cmdType=='S')								// Monta resposta ao "Set TestLcd"
		Send_Response (DC3, "", 0);							// Responde com DC3-Valor somente leitura
	else if (fr.cmdType=='G')								// Monta resposta ao "Get TestLcd"
	{
		result=2;
		Send_Response (ACK, &result, 1);					// Responde com valor requisitado
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_TestFlashFs
 * Descrição: 	Comando para tratamento do teste da Flash FS
 -----------------------------------------------------------------------------*/
void cmd_TestFlashFs (FRMRES fr)
{
	unsigned char result=0;

	if (PROGMODE==0)										// Se não estiver logado
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (fr.cmdType=='S')								// Monta resposta ao "Set TestFlashFs"
		Send_Response (DC3, "", 0);							// Responde com DC3-Valor somente leitura
	else if (fr.cmdType=='G')								// Monta resposta ao "Get TestFlashFs"
	{
		result=Test_FlashFs ();
		Send_Response (ACK, &result, 1);					// Responde com valor requisitado
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_IdBoard
 * Descrição: 	Comando para tratamento da identificação do ID da placa
 -----------------------------------------------------------------------------*/
void cmd_IdBoard (FRMRES fr)
{
	unsigned char 	i;
	unsigned char 	array[4]={0,0,0,0};
	
	if (PROGMODE==0)										// Se não estiver logado
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (fr.cmdType=='S')								// Monta resposta ao "Set IdBoard"
	{
		if (fr.dataLen<=4)
		{
			cfg.idBoard	= *fr.dataBuf++;
			cfg.idBoard	|= *fr.dataBuf++<<8;
			cfg.idBoard	|= *fr.dataBuf++<<16;
			cfg.idBoard	|= *fr.dataBuf++<<24;

			Write_Flash ();

			Send_Response (ACK,"",0);						// Devolve ACK
		}
		else
			Send_Response (DC2,"",0);						// Responde com DC2-Comprimento incorreto
	}
	else if (fr.cmdType=='G')								// Monta resposta ao "Get IdBoard"
	{
		i=doArray (array, cfg.idBoard); 
		Send_Response (ACK, array, i); 						// Devolve o número de identificação
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_KeyStatus
 * Descrição: 	Comando para tratamento das teclas
 -----------------------------------------------------------------------------*/
void cmd_KeyStatus (FRMRES fr)
{
	unsigned char 	i;
	unsigned char 	array[2]={0,0};
	
	if (PROGMODE==1)										// Se estiver logado
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (fr.cmdType=='S')								// Monta resposta ao "Set IdBoard"
	{
		if (fr.dataLen<=2)
		{
			remoteKey = *fr.dataBuf++;
			remoteKey |= *fr.dataBuf++<<8;

			Send_Response (ACK,"",0);						// Devolve ACK
		}
		else
			Send_Response (DC2,"",0);						// Responde com DC2-Comprimento incorreto
	}
	else if (fr.cmdType=='G')								// Monta resposta ao "Get IdBoard"
	{
		i=doArray (array, remoteKey); 
		Send_Response (ACK, array, i); 						// Devolve o número de identificação
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_InitFlash
 * Descrição: 	Comando para tratamento da inicialização da flash interna 
 -----------------------------------------------------------------------------*/
void cmd_InitFlash (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_FlashData
 * Descrição: 	Comando para tratamento dos dados da flash interna 
 -----------------------------------------------------------------------------*/
void cmd_FlashData (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_FlashTimeAtd
 * Descrição: 	Comando para tratamento de tempos de atend na Flash interna
 -----------------------------------------------------------------------------*/
void cmd_FlashTimeAtd (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_InitSurvey
 * Descrição: 	Comando para tratamento do início da pesquisa
 -----------------------------------------------------------------------------*/
void cmd_InitSurvey (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_SurveyStatus
 * Descrição: 	Comando para tratamento da status de uma pesquisa
 -----------------------------------------------------------------------------*/
void cmd_SurveyStatus (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_SurveyName
 * Descrição: 	Comando para tratamento do nome da pesquisa
 -----------------------------------------------------------------------------*/
void cmd_SurveyName (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_SurveyResult
 * Descrição: 	Comando para tratamento do resultado da pesquisa
 -----------------------------------------------------------------------------*/
void cmd_SurveyResult (FRMRES fr)
{
	unsigned char opinion=0;
	unsigned char userNum=0;

	if (PROGMODE==1)										// Se estiver em modo programação
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (fr.cmdType=='S')								// Monta resposta ao "Set SurveyResult"
	{
		if (fr.cmdIdx>0)									// Index=senha não pode ser 0
		{
			if (fr.dataLen==2) 								// Verifica comprimento dos dados
			{
				userNum = *fr.dataBuf++;					// Pega dos dados o número do usuário
				opinion  = *fr.dataBuf;						// Pega dos dados a opinião
				if (opinion=='O' ||							// Verifica se a opinião é válida..
					opinion=='B' ||
					opinion=='C' ||
					opinion=='R' ||
					opinion=='P' ||
					opinion=='N')						
				{
					if (Set_Opinion (fr.cmdIdx, 			// Configura na estrutura do atendimento
										opinion, 
										userNum))
						Send_Response (ACK,"",0);			// Responde com ACK
					else
						Send_Response (DC4,"",0);			// Responde com DC4-erro não especif.
				}
				else
					Send_Response (DC4,"",0);				// Responde com DC4-erro não especif.
			}
			else
				Send_Response (DC2,"",0);					// Responde com DC2-Comprimento incorreto
		}													
		else
			Send_Response (DC4,"",0);						// Responde com DC4-erro não especif.
	}
	else if (fr.cmdType=='G')								// Monta resposta ao "Get SurveyResult"
	{
		if (fr.cmdIdx>0)									// Index=senha não pode ser 0
		{
			if (fr.dataLen==1) 								// Verifica comprimento dos dados
			{
				userNum = *fr.dataBuf++;					// Pega dos dados o número do usuário
				if (userNum>=1 && userNum<MAXUSER)
				{
					opinion=Get_Opinion (fr.cmdIdx,			// Busca opinião do ticket
										userNum);		
					Send_Response (ACK, &opinion, 1); 		// Responde a requisição.
				}
				else
					Send_Response (DC1,"",0);				// Responde com DC1-Valor fora do range
			}
			else
				Send_Response (DC2,"",0);					// Responde com DC2-Comprimento incorreto
		}
		else
			Send_Response (DC4,"",0);						// Responde com DC4-erro não especif.
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_SurveyKeyCfg
 * Descrição: 	Comando para tratamento da configuração das teclas de pesquisa
 -----------------------------------------------------------------------------*/
void cmd_SurveyKeyCfg (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_SurveyUser
 * Descrição: 	Comando para tratamento do usuário avaliado
 -----------------------------------------------------------------------------*/
void cmd_SurveyUser (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_SurveyReg
 * Descrição: 	Comando para tratamento do registro do usuário avaliado
 -----------------------------------------------------------------------------*/
void cmd_SurveyReg (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_SurveyInfTop
 * Descrição: 	Comando para tratamento das informações do Top
 -----------------------------------------------------------------------------*/
void cmd_SurveyInfTop (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_SurveyQuestion
 * Descrição: 	Comando para tratamento das informações do Top
 -----------------------------------------------------------------------------*/
void cmd_SurveyQuestion (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_ticketNum
 * Descrição: 	Comando para tratamento do número do ticket de senha existente
 -----------------------------------------------------------------------------*/
void cmd_ticketNum (FRMRES fr)
{
	char i;
	if (fr.cmdType=='R')									// Recebe resposta, número anterior de ticket
	{
		if (fr.flag==ACK)									// Verifica se a resposta chegou sem erros
			numFrameServer = fr.idFrame;
	}
	else if (PROGMODE==1)									// Se estiver em modo programação
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (fr.cmdType=='S')								// Monta resposta ao "Set ticketNum"
		Send_Response (DC3,"",0);							// Responde com DC3-Valor somente leitura
	else if (fr.cmdType=='G')								// Monta resposta ao "Get ticketNum"
	{
		TKTINFO tkt = {0,0,0,0};
		unsigned char userNum=fr.cmdIdx;					// Pega do index o número do usuário
		unsigned char depNum=0;

		if (fr.dataLen==1)									// Verifica se tem informação do dpto
			depNum = *fr.dataBuf;							// Pega dos dados o número do dpto

		if (userNum!=0 && userNum<=MAXUSER)
		{
			if (depNum==0)
				depNum = cfg.depUsers [userNum-1];

			if (depNum==cfg.salesDpto)
			{
				if (userCall[userNum-1].run==1)
				{
					Send_Response (13,"",0);				// Responde com "13-Falta encerrar".
					return;
				}
	
				if (userCall[userNum-1].sta==0)
				{
					Send_Response (12,"",0);				// Responde com "12-Usuario Desabilitado".
					return;
				}

				for (i=0 ; i<MAXUSER ; i++)					// Varredura em todos usuários
				{
					if (userCall[i].sta == 1 &&				// Verifica se tem usuario habilitado
						userCall[i].run == 0)				// e se esta desocupado para comparar
					{
						if (userCall[userNum-1].time > 		// Compara tempos dos usuários
							userCall[i].time)				// atende se for o mais antigo
						{
							Send_Response (11,"",0);		// Responde com "11-aguarde a sua vez".
							return;
						}
					}
				}
			}

			tkt = Get_ticketNum (userNum, depNum);			// Busca número do ticket
			Response_InfoTicket(fr, tkt, userNum, depNum);	// tkt = Tipo e Número do ticket
		}
		else
			Send_Response (DC4,"",0);						// Responde com DC4-erro não especif.
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_ticketSta
 * Descrição: 	Comando para tratamento do status do ticket existente
 -----------------------------------------------------------------------------*/
void cmd_ticketSta (FRMRES fr)
{
	unsigned char status=0;
	unsigned char userNum=0;
	unsigned char depNum=0;
	TKTINFO tkt = {0,0,0,0};

	if (PROGMODE==1)										// Se estiver em modo programação
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (fr.cmdType=='S')								// Monta resposta ao "Set ticketSta"
	{
		if (fr.cmdIdx>0)									// Index=senha não pode ser 0
		{
			if (fr.dataLen>=2) 								// Verifica comprimento dos dados
			{
				userNum = *fr.dataBuf;						// Pega dos dados o número do usuário
				if (userNum>=1 && userNum<MAXUSER)
				{
					status  = *(fr.dataBuf+1);				// Pega dos dados o status
					if (status==STA_WORKING ||
						status==STA_CLOSED ||
						status==STA_CANCELED || 
						status==STA_REDIRECT)				// Verifica se o status é válido
					{
						if (fr.dataLen==3)			 		// Verifica se tem informação do dpto
							depNum = *(fr.dataBuf+2);		// Pega o número do dpto...
						else
							depNum=cfg.depUsers[userNum-1];	// Verifica pelo usuário

						if (Set_ticketSta (fr.cmdIdx, 
											status, 
											userNum,
											&tkt.type,
											depNum))
						{
							tkt.number = fr.cmdIdx;
							tkt.dep = depNum;
							Response_InfoTicket(fr,
												tkt,
												userNum,
												depNum);
						}
						else
							Send_Response (DC4,"",0);		// Responde com DC4-erro não especif.
					}
					else
						Send_Response (DC4,"",0);			// Responde com DC4-erro não especif.
				}
				else
					Send_Response (DC1,"",0);				// Responde com DC1-Valor fora do range
			}
			else
				Send_Response (DC2,"",0);					// Responde com DC2-Comprimento incorreto
		}													
		else
			Send_Response (DC4,"",0);						// Responde com DC4-erro não especif.
	}
	else if (fr.cmdType=='G')								// Monta resposta ao "Get ticketSta"
	{
		if (fr.cmdIdx>0)									// Index=senha não pode ser 0
		{
			if (fr.dataLen>=1) 								// Verifica comprimento dos dados
			{
				userNum = *fr.dataBuf;						// Pega dos dados o número do usuário
				if (userNum>=1 && userNum<MAXUSER)
				{
					if (fr.dataLen==2)						// Verifica se tem informação do dpto 
						depNum = *(fr.dataBuf+1);			// Pega o número do dpto...

					status=Get_ticketSta(	fr.cmdIdx,		// Busca status do ticket
											userNum,
											depNum);		
					Send_Response (ACK, &status, 1); 		// Responde a requisição.
				}
				else
					Send_Response (DC1,"",0);				// Responde com DC1-Valor fora do range
			}
			else
				Send_Response (DC2,"",0);					// Responde com DC2-Comprimento incorreto
		}
		else
			Send_Response (DC4,"",0);						// Responde com DC4-erro não especif.
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_ticketNew
 * Descrição: 	Comando para tratamento de solicitação de novo número de ticket
 -----------------------------------------------------------------------------*/
void cmd_ticketNew (FRMRES fr)
{
	unsigned char i;

	if (fr.cmdType=='R')									// Recebe resposta, número anterior de ticket
	{
		if (fr.flag==ACK)									// Verifica se a resposta chegou sem erros
		{
			numFrameServer = fr.idFrame;
			if (fr.dataLen>=5 && fr.dataLen<10)				// Se for resposta de comando "Get" tem dados
			{
				if (cfg.runMode!=_MODECLIENT)
					dptoServer = cfg.depUsers[fr.cmdIdx-1];		// Retira o departamento
				else
					dptoServer = fr.cmdIdx;

				for (i=0 ; i<5 ; i++)
					forecastServer[i]=*fr.dataBuf++;		// Retira previsão de espera
				forecastServer[5]=0;

				alphaServer=*fr.dataBuf++;					// Retira letra do ticket
				
				for (numTicketServer=0, i=0; i<(fr.dataLen-6) ; i++)
					numTicketServer|=((*fr.dataBuf++)<<(i*8)); // Retira número do ticket
			}
		}			
	}
	else if (PROGMODE==1)									// Se estiver em modo programação
	{
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	}
	else if (fr.cmdType=='S')								// Monta resposta ao "Set ticketNum"
	{
		Send_Response (DC3,"",0);							// Responde com DC3-Valor somente leitura
	}
	else if (fr.cmdType=='G')								// Monta resposta ao "Get ticketNum"
	{
		unsigned char array[10]={0,0,0,0,0,0,0,0,0,0};
		unsigned int number=0;

		ShowForecast (fr.cmdIdx, (char*)array); 			// Primeiros 5 bytes=string da previsão
		array[5]=cfg.depAlpha[fr.cmdIdx-1];					// busca letra do departamento, idx=userNum
		number=New_ticketNumber_MIGCli (fr.cmdIdx);			// Cadastra novo ticket no departamento indicado
		i=doArray (&array[6], number); 						// Monta resposta em array a partir do byte 5
		Send_Response (ACK, array, i+6); 					// Responde previsão e número da senha
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_ticketReNew
 * Descrição: 	Comando para tratamento de solicitação de recadastro de 
 * 				número de ticket
 -----------------------------------------------------------------------------*/
void cmd_ticketReNew (FRMRES fr)
{
	unsigned char priority;
	unsigned int ticketNum;
	unsigned char alpha;
	unsigned char resp=0;

	if (PROGMODE==1)										// Se estiver em modo programação
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (fr.cmdType=='S')								// Monta resposta ao "Set ticketNum"
	{
		if (fr.dataLen>1 && fr.dataLen<=5)
		{
			if (fr.cmdIdx>=1 && fr.cmdIdx<=12)
			{
				priority = (*fr.dataBuf++)&0x01;			// Verifica prioridade 0=Comum 1=Preferêncial
				
				ticketNum = *fr.dataBuf++;
				ticketNum|=(*fr.dataBuf++)<<8;
				
				alpha = *fr.dataBuf++;

				resp = Route_ticketNumber (ticketNum,		// Cadastra novamente número do ticket  
										alpha,				// Letra do ticket
										priority,			// Status de "encaminhado" 
										fr.cmdIdx);			// Departamento indicado 
										
				if (resp)
					Send_Response (ACK,"",0);				// Devolve ACK
				else 
					Send_Response (DC4,"",0);				// Responde com DC4-Erro não especificado
			}
			else
				Send_Response (DC4, "", 0);					// Responde com DC4-Erro não especificado
		}
		else
			Send_Response (DC1, "", 0);						// Responde com DC1-Valor fora do range
	}
	else if (fr.cmdType=='G')								// Monta resposta ao "Get ticketNum"
	{
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_ticketAlpha
 * Descrição: 	Comando para tratamento de solicitação da letra do ticket
 -----------------------------------------------------------------------------*/
void cmd_ticketAlpha (FRMRES fr)
{
	if (PROGMODE==1)										// Se estiver em modo programação
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (fr.cmdType=='S')								// Comando é requisição com "Set"
		Send_Response (DC3, "", 0);							// Responde com DC3-Valor somente leitura
	else if (fr.cmdType=='G')								// Comando é requisição com "Get"
	{
		unsigned char userNum=fr.cmdIdx;
		unsigned char depNum=0;

		if (userNum>=1 && userNum<MAXUSER)
		{
			if (fr.dataLen==1)
				depNum=*fr.dataBuf;							// Usa atd informado
			else
				depNum=cfg.depUsers[userNum-1];				// Busca atd a partir da config. do usuário

			Send_Response (	ACK, 							// Devolve resposta com informação
							(unsigned char *)&cfg.depAlpha[depNum-1], 
							1);
		}
		else
			Send_Response (DC4, "", 0);						// Responde com DC4-Erro não especificado
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_ticketBox
 * Descrição: 	Comando para tratamento de solicitação do box do usuário
 -----------------------------------------------------------------------------*/
void cmd_ticketBox (FRMRES fr)
{
	if (PROGMODE==1)										// Se estiver em modo programação
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (fr.cmdType=='S')								// Comando é requisição com "Set"
		Send_Response (DC3, "", 0);							// Responde com DC3-Valor somente leitura
	else if (fr.cmdType=='G')								// Comando é requisição com "Get"
	{
		if (fr.cmdIdx>=1 && fr.cmdIdx<=99)
			Send_Response (ACK, (unsigned char *)&cfg.userBox[fr.cmdIdx-1], 1);// Devolve resposta com informação
		else
			Send_Response (DC4, "", 0);						// Responde com DC4-Erro não especificado
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_ticketDepDisp
 * Descrição: 	Comando para tratamento de solicitação do display do dpto
 -----------------------------------------------------------------------------*/
void cmd_ticketDepDisp (FRMRES fr)
{
	if (PROGMODE==1)										// Se estiver em modo programação
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (fr.cmdType=='S')								// Comando é requisição com "Set"
		Send_Response (DC3, "", 0);							// Responde com DC3-Valor somente leitura
	else if (fr.cmdType=='G')								// Comando é requisição com "Get"
	{
		unsigned char userNum=fr.cmdIdx;
		unsigned char depNum=0;

		if (userNum>=1 && userNum<=MAXUSER)
		{
			if (fr.dataLen==1)
				depNum=*fr.dataBuf;							// Usa atd informado
			else
				depNum=cfg.depUsers[userNum-1];				// Busca atd a partir da config. do usuário

			Send_Response (	ACK, 							// Devolve resposta com informação
							(unsigned char *)&cfg.depRowDisp[depNum-1], 
							1);
		}
		else
			Send_Response (DC4, "", 0);						// Responde com DC4-Erro não especificado
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_ticketDepName
 * Descrição: 	Comando para tratamento de solicitação do nome do depto
 -----------------------------------------------------------------------------*/
void cmd_ticketDepName (FRMRES fr)
{
	if (PROGMODE==1)										// Se estiver em modo programação
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (fr.cmdType=='S')								// Comando é requisição com "Set"
		Send_Response (DC3, "", 0);							// Responde com DC3-Valor somente leitura
	else if (fr.cmdType=='G')								// Comando é requisição com "Get"
	{
		if (fr.cmdIdx>=1 && fr.cmdIdx<=13)
			Send_Response (ACK, (unsigned char *)cfg.depName[fr.cmdIdx-1], len((unsigned char *)cfg.depName[fr.cmdIdx-1]));// Devolve resposta com informação
		else
			Send_Response (DC4, "", 0);						// Responde com DC4-Erro não especificado
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_ticketLast
 * Descrição: 	Comando para tratamento do último ticket chamado
 -----------------------------------------------------------------------------*/
void cmd_ticketLast (FRMRES fr)
{
	if (PROGMODE==1)										// Se estiver em modo programação
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (fr.cmdType=='S')								// Monta resposta ao "Set ticketNum"
		Send_Response (DC3,"",0);							// Responde com DC3-Valor somente leitura
	else if (fr.cmdType=='G')								// Monta resposta ao "Get ticketNum"
	{
		TKTINFO tkt = {0,0,0,0};
		unsigned char userNum=fr.cmdIdx;

		if (userNum>=1 && userNum<=MAXUSER)
		{
			tkt = Get_ticketNumLast (userNum);				// Busca número do ticket
			Response_InfoTicket (fr,tkt,userNum,0); 		// tkt = Tipo e Número do ticket
		}
		else
			Send_Response (DC4, "", 0);						// Responde com DC4-Erro não especificado
	}														
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_ticketSeq
 * Descrição: 	Comando para tratamento da chamada de ticket sequêncial
 -----------------------------------------------------------------------------*/
void cmd_ticketSeq (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_ticketSeqAdj
 * Descrição: 	Comando para tratamento do ajuste do ticket sequêncial
 -----------------------------------------------------------------------------*/
void cmd_ticketSeqAdj (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_infoAtd
 * Descrição: 	Comando para tratamento das informações do atendimento
 -----------------------------------------------------------------------------*/
void cmd_infoAtd (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_ticketDepUser
 * Descrição: 	Comando para tratamento do departametno do usuário
 -----------------------------------------------------------------------------*/
void cmd_ticketDepUser (FRMRES fr)
{
	unsigned char userNum=fr.cmdIdx;

	if (PROGMODE==1)										// Se estiver em modo programação
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (fr.cmdType=='G')								// Comando é requisição com "Get"
	{
		if (userNum>=1 && userNum<=99)
		{
			Send_Response (	ACK, 							// Devolve resposta com informação
							(unsigned char *)&cfg.depUsers[userNum-1],
							1);
		}
		else
			Send_Response (DC4, "", 0);						// Responde com DC4-Erro não especificado
	}
	else if (fr.cmdType=='S')								// Comando é requisição com "Get"
	{
		if (userNum>=1 && userNum<=99)
		{
			if (fr.dataLen==1)
			{
				if (*fr.dataBuf>=1 && *fr.dataBuf<=6)
				{
					cfg.depUsers[userNum-1]=*fr.dataBuf;
					Send_Response (ACK,"",0);				// Devolve ACK
				}
				else
					Send_Response (DC4, "", 0);				// Responde com DC4-Erro não especificado
			}
			else
				Send_Response (DC1, "", 0);					// Responde com DC1-Valor fora do range
		}
		else
			Send_Response (DC4, "", 0);						// Responde com DC4-Erro não especificado
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_ticketOld
 * Descrição: 	Comando para tratamento de ticket formato antigo
 -----------------------------------------------------------------------------*/
void cmd_ticketOld (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}
/*----------------------------------------------------------------------------
 * Funções:		cmd_DisplayProdName
 * Descrição: 	Comando para tratamento do nome do produto
 -----------------------------------------------------------------------------*/
void cmd_ProdName (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_ProdPrice
 * Descrição: 	Comando para tratamento do preço do produto
 -----------------------------------------------------------------------------*/
void cmd_ProdPrice (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_PriceDisplay
 * Descrição: 	Comando para tratamento do display do preço
 -----------------------------------------------------------------------------*/
void cmd_PriceDisplay (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_PriceEffect
 * Descrição: 	Comando para tratamento do efeito do preço
 -----------------------------------------------------------------------------*/
void cmd_PriceEffect (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_Sequence
 * Descrição: 	Comando para tratamento das sequencias de mensagens
 -----------------------------------------------------------------------------*/
void cmd_Sequence (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_groupSeq
 * Descrição: 	Comando para tratamento
 -----------------------------------------------------------------------------*/
void cmd_groupSeq (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_nameMsg
 * Descrição: 	Comando para tratamento das sequencias de mensagens
 -----------------------------------------------------------------------------*/
void cmd_nameMsg (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_FiscalInfo
 * Descrição: 	Comando para tratamento de informações do chamada de fiscal
 -----------------------------------------------------------------------------*/
void cmd_FiscalInfo (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_FiscalCall
 * Descrição: 	Comando para tratamento de Chamada de fiscal
 -----------------------------------------------------------------------------*/
void cmd_FiscalCall (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_InfoRF
 * Descrição: 	Comando para tratamento das informações do RF
 -----------------------------------------------------------------------------*/
void cmd_InfoRF (FRMRES fr)
{
	unsigned char   i;
	unsigned char   user=0;
	unsigned char   func=0;
	
	if (PROGMODE==1)										// Se estiver em modo programação
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (fr.cmdType=='G')								// Monta resposta ao "Get ticketNum"
	{
		Send_Response (DC4,"",0);
	}
	else if (fr.cmdType=='S')								// Grava valor e devolve resposta ao comando "Set Ticket"
	{
		if (fr.dataLen==2)
		{
		  func = *fr.dataBuf++;								// Retira função do botão: 1=próximo 2=repete 3=encerra 4=cancela
		  user = *fr.dataBuf++;								// Retira o box

		  if (cfg.idRf == fr.cmdIdx)						// Verifica o código
		  {
			for (i=0; i<10; i++)							// Faz varredura procurando chamada no buffer:
				if (call[i].func==func &&
					call[i].user==user)						// Se a chamada ja estiver no buffer..
					func=0;									// Zera a chamada
		
			if (func!=0 && user!=runUser)					// Caso informações sejam válidas:
			{
				for (i=10-1; i>0; i--)						// Realoca o buffer 
				{
					call[i].row = call[i-1].row;
					call[i].alpha = call[i-1].alpha;
					call[i].number = call[i-1].number;
					call[i].user = call[i-1].user;
					call[i].dep = call[i-1].dep;
					call[i].func = call[i-1].func;
				}
				call[0].row=1;								// Evita erro colocando linha 1
				call[0].alpha = 0;							// Zera a letra do ticket
				call[0].number = 0;							// Zera o número do ticket
				call[0].user = user;						// Grava o box do ticket
				call[0].dep = 0;
				call[0].func = func;						// Grava a função da tecla
			}
		  }
		}
		else
			Send_Response (DC2,"",0);						// Responde com DC2-Comprimento incorreto
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_ReqProtocol
 * Descrição: 	Comando para tratamento da solicitação do número do protocolo
 -----------------------------------------------------------------------------*/
void cmd_ReqProtocol (FRMRES fr)
{
	unsigned short i;
	if (fr.cmdType=='R')
	{
		if (fr.flag==ACK)									// Verifica se a resposta chegou sem erros
		{
			numFrameServer = fr.idFrame;
			if (fr.dataLen<=24)
			{
				for (i=0 ; i<fr.dataLen ; i++)
					atdProtocol[i]=*fr.dataBuf++;

				for (;i<24;i++)
					atdProtocol[i]=0;
			}		
		}
	}
	else if (fr.cmdType=='S')								// Monta resposta ao "Set protocol num"
	{
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	}
	else if (fr.cmdType=='G')								// Monta resposta ao "Get protocol num"
	{
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_PaperStatus
 * Descrição: 	Comando para tratamento da solicitação do status do papel
 -----------------------------------------------------------------------------*/
void cmd_PaperStatus (FRMRES fr)
{
	unsigned char status;
	if (fr.cmdType=='R')
	{
		if (fr.flag==ACK)									// Verifica se a resposta chegou sem erros
		{
			numFrameServer = fr.idFrame;
		}
	}
	else if (fr.cmdType=='S')								// Monta resposta ao "Set protocol num"
	{
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	}
	else if (fr.cmdType=='G')								// Monta resposta ao "Get protocol num"
	{
		status = (unsigned char) erroPrint;
		Send_Response (ACK, &status, 1);					// Responde com status mig
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_DptoStatus
 * Descrição: 	Comando para tratamento de solicitação do nome do depto
 -----------------------------------------------------------------------------*/
void cmd_DptoStatus (FRMRES fr)
{
	unsigned char array[50];
	unsigned char len;
	unsigned char i=0;
	if (PROGMODE==1)										// Se estiver em modo programação
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (fr.cmdType=='S')								// Comando é requisição com "Set"
		Send_Response (DC3, "", 0);							// Responde com DC3-Valor somente leitura
	else if (fr.cmdType=='G')								// Comando é requisição com "Get"
	{
		len=0;
		array[len++]=cfg.depEnable[fr.cmdIdx-1];
		while (cfg.depName[fr.cmdIdx-1][i]!=0)
		{
			array[len++]=cfg.depName[fr.cmdIdx-1][i];
			i++;
		}
		if (fr.cmdIdx>=1 && fr.cmdIdx<=12)
			Send_Response (ACK, array, len);				// Devolve resposta com informação
		else
			Send_Response (DC4, "", 0);						// Responde com DC4-Erro não especificado
	}
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_AlarmTime
 * Descrição: 	Comando para tratamento do alarme de hora do display meter
 -----------------------------------------------------------------------------*/
void cmd_AlarmTime (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_AlarmeTemp
 * Descrição: 	Comando para tratamento do alarme de temperatura do display meter
 -----------------------------------------------------------------------------*/
void cmd_AlarmTemp (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_AlarmStatus
 * Descrição: 	Comando para tratamento do status dos alarme do display meter
 -----------------------------------------------------------------------------*/
void cmd_AlarmStatus (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*----------------------------------------------------------------------------
 * Funções:		cmd_SilenceTime
 * Descrição: 	Comando para tratamento do tempo do comando inibe do display meter
 -----------------------------------------------------------------------------*/
void cmd_SilenceTime (FRMRES fr)
{
	ErrorUndefinedCommand(fr.cmdType);
}

/*-------------------------------------------------------------------------------------------
 * Funções:		Receive_Ntp
 * Descrição: 	Recebe frame do servidor NTP
 * Parâmetros: 	*buf - Ponteiro para os dados do NTP
 *				len - Comprimento dos dados
 * Retorno: 	Nenhum
 -------------------------------------------------------------------------------------------*/
void Receive_Ntp (unsigned char *rip, unsigned char *buf, unsigned short len)
{
}

/*----------------------------------------------------------------------------
 * Funções:		ErrorUndefinedCommand
 * Descrição: 	Devolve resposta de erro de comando inexistente "SET" ou "GET"
 *				ignora comando "RESPONSE"
 * Parâmetros:  nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void ErrorUndefinedCommand (char cmdType)
{
	if (cmdType=='S')										// Monta resposta ao "Set"
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
	else if (cmdType=='G')									// Monta resposta ao "Get"
		Send_Response (DC4, "", 0);							// Responde com DC4-Erro não especificado
}

/*----------------------------------------------------------------------------
 * Funções:		Response_InfoTicket
 * Descrição: 	Comando para enviar informações do ticket requisitado
 * Parâmetros:  fr - Estrutura do frame
 *				userNum - Número do usuário
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void Response_InfoTicket (FRMRES fr, TKTINFO tkt, unsigned char userNum, unsigned char depNum)
{
	unsigned char array[53]= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	unsigned char i;
	unsigned char nBytes=0;

	if (depNum==0)
		depNum = cfg.depUsers[userNum-1];					// Verifica dpto padrão do usuário

	if (tkt.number!=0)
	{
		array[nBytes++]=cfg.depRowDisp[tkt.dep-1];			// Busca display do dpto do ticket chamado.
		array[nBytes++]=tkt.alpha; //cfg.depAlpha[tkt.dep-1]; // Busca letra do dpto do ticket chamado.
	}
	else
		nBytes+=2;

	array[nBytes++]=cfg.userBox[userNum-1];					// Busca box do usuário.

	if (tkt.number!=0)
	{
		array[nBytes++]=(unsigned char)(tkt.number&0xFF);	// Ticket Byte Low
		array[nBytes++]=(unsigned char)((tkt.number&0xFF00)>>8);// Ticket Byte High
		array[nBytes++]=tkt.type;							// Tipo do ticket chamado (1)pref / (0)Normal
		array[nBytes++]=tkt.dep;							// Busca dpto do ticket chamado.
		array[nBytes++]=cfg.depIpDisp[tkt.dep-1][0];		// IP PAINEL do ticket chamado
		array[nBytes++]=cfg.depIpDisp[tkt.dep-1][1];		// IP PAINEL
		array[nBytes++]=cfg.depIpDisp[tkt.dep-1][2];		// IP PAINEL
		array[nBytes++]=cfg.depIpDisp[tkt.dep-1][3];		// IP PAINEL
		array[nBytes++]=cfg.depItfDisp[tkt.dep-1];			// Interface do PAINEL do ticket chamado
	}
	else
		nBytes+=9;

	if (cfg.eTop[depNum-1]!=0)								// Se TOP estiver habilitado..
	{
		array[nBytes++]=cfg.depIpTop[depNum-1][0];			// IP TOP do departamento do usuário
		array[nBytes++]=cfg.depIpTop[depNum-1][1];			// IP TOP
		array[nBytes++]=cfg.depIpTop[depNum-1][2];			// IP Top
		array[nBytes++]=cfg.depIpTop[depNum-1][3];			// IP Top
		array[nBytes++]=cfg.depItfTop[depNum-1];			// Interface do TOP do departamento do usuário
	}
	else
		nBytes+=5;

	if (tkt.number!=0)
	{
		for (i=0;i<36 && cfg.depName[tkt.dep-1][i]!=0;i++)	// Nome do departamento do ticket chamado
			array[nBytes++]=(unsigned char)cfg.depName[tkt.dep-1][i];
	}

	Send_Response (ACK, array, nBytes);						// Responde com informações da chamada
}

/*----------------------------------------------------------------------------
 * Funções:		len
 * Descrição: 	Verifica o tamanho do array.
 * Parâmetros:  *pDat - Ponteiro para o array
 * Retorno: 	Número de bytes no array.
 -----------------------------------------------------------------------------*/
unsigned short len (unsigned char *pDat)
{
	unsigned short val=0;

	while (*pDat!=0)
	{
		pDat++;
		val++;
	}
	return val;
}

/*----------------------------------------------------------------------------
 * Funções:		doArray
 * Descrição: 	Transfere bytes de valor numérico para array, otimizando o 
 *				número de bytes.
 * Parâmetros: *pDat - Ponteiro para o array de 4 bytes
 *				value - Valor a ser transferido para o array
 * Retorno: 	Número de bytes transferidos para o array.
 -----------------------------------------------------------------------------*/

unsigned char doArray (unsigned char *pDat, unsigned int value)
{
	unsigned char numBytes=0;

	if (value > 0xFFFFFF)  									// Valor utiliza 4 bytes da variável Int
	{
		*pDat++=(unsigned char)(value&0xFF);
		*pDat++=(unsigned char)((value&0xFF00)>>8);
		*pDat++=(unsigned char)((value&0xFF0000)>>16);
		*pDat  =(unsigned char)((value&0xFF000000)>>24);
		numBytes=4;
	}
	else if (value > 0xFFFF)								// Valor utiliza 3 bytes da variável Int.
	{
		*pDat++=(unsigned char)(value&0xFF);
		*pDat++=(unsigned char)((value&0xFF00)>>8);
		*pDat  =(unsigned char)((value&0xFF0000)>>16);
		numBytes=3;
	}
	else if (value > 0xFF)
	{
		*pDat++=(unsigned char)(value&0xFF);				// Valor utiliza 2 bytes da variável Int
		*pDat  =(unsigned char)((value&0xFF00)>>8);
		numBytes=2;
	}
	else
	{
		*pDat=(unsigned char)value;						// Valor utiliza 1 byte da variável Int
		numBytes=1;
	}

	return numBytes;
}

 /*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
