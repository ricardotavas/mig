/*----------------------------------------------------------------------------
 *      Arquivo:    REPORT.c
 *      Proposito: 	Gerenciamento dos arquivos de relatório
 *----------------------------------------------------------------------------
 *      Versão: 	1.00 	Prog: Ricardo T.   Data: 25/04/2013
 * 		Descrição:  Versão inicial
 *----------------------------------------------------------------------------
 *      Este código pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#include 	<LPC17xx.h>
#include 	<File_Config.h>
#include 	<stdio.h>
#include	"RTC_CONFIG.h"
#include	"IMP_CONFIG.h"
#include	"IAP_CONFIG.h"
#include	"DataFlash.h"
#include	"APP.h"
#include	"QUEUE.h"
#include	"REPORT.h"
#include	"CLIENT.h"

/*----------------------------------------------------------------------------
 *      Report Header definitions
 *---------------------------------------------------------------------------*/
#define		MAX_BUFDATA		512							// Setor FLASH FS=256 / Setor SDCARD=512 bytes

#define		HEADERFILE		"STATUS:\x0D A=Atendendo / E=Esperando / C=Cancelado / F,FR,FA,FI,FO=Fechado / FE=Encaminhado \x0DTIPO:\x0D N=Normal / P=Preferencial\x0DOPINIAO:\x0D O=Otimo / B=Bom / C=Regular / R=Ruim / N=Nulo / X=Nenhum\x0D\x0D\x0ASTATUS;DPTO;TIPO;MESA;TICKET;CHEGADA;INICIO;FIM;FILA;OPINIAO;\x0D\x0A"
#define		HEADERLEN		256

/*----------------------------------------------------------------------------
 *      HTTP_CGI.c   variables
 *---------------------------------------------------------------------------*/
extern unsigned char	WEBREQUEST;

/*----------------------------------------------------------------------------
 *      NetConfig.c   variables
 *---------------------------------------------------------------------------*/
extern unsigned char	http_auth_passw[20];

/*----------------------------------------------------------------------------
 *      App.c   variables
 *---------------------------------------------------------------------------*/
extern unsigned char 	versionFirm[2];
extern ATD				atend;

/*----------------------------------------------------------------------------
 *      QUEUE.c   variables
 *---------------------------------------------------------------------------*/
extern unsigned short	queue [MAXDEP];
extern unsigned int		lastCallUser [MAXUSER];
extern unsigned int		lastPrioUser [MAXUSER];
extern unsigned int		lastCallDpto [MAXDEP];
extern unsigned int		lastPrioDpto [MAXDEP];

/*----------------------------------------------------------------------------
 *      Local variables
 *---------------------------------------------------------------------------*/
FILE *f = NULL;
FINFO info;
unsigned char bufDat[MAX_BUFDATA];
unsigned short nDat=0;	   								// Número de bytes para gravar em arquivos
unsigned char SYSTEM_DONE=_INISTART;
unsigned char STATFILE=_FLWAIT;

/*----------------------------------------------------------------------------
 *      Local functions
 *---------------------------------------------------------------------------*/
unsigned char Verifica_FLASH (unsigned char flagInit);
unsigned char Format_Drive (void);
unsigned char Change_Drive (unsigned char driver);
void Check_RequestWeb (void);
void Check_FileSystem (void);
void MakeFileName (char* pName);
char PutBuf (char byte);
char SaveBlock (void);
void Save_FormatInfo (ATD *atd);
void StateMachine_ticket (void);

/*----------------------------------------------------------------------------
 *      Extern Functions
 *---------------------------------------------------------------------------*/
extern void Write_Flash (void);

/*----------------------------------------------------------------------------
 * Funções:		Verifica_FLASH
 * Descrição: 	Verifica se o cartão esta inserido
 * Parâmetros: 	flagInit - Opção para inicializar o sistema de arquivos
 *				(1) inicializa SDCARD (0) não inicializa SDCARD
 * Retorno: 	(0) SDCARD não inserido ou erro (1) SDCARD inserido
 -----------------------------------------------------------------------------*/
unsigned char Verifica_FLASH (unsigned char flagInit)
{
	unsigned char flashOK=0;

	if (_DEF_DRIVE==_SDCARD)
	{
		if (~(LPC_GPIO4->FIOPIN >> 29) & 1)
		{
			if (flagInit)
			{
				if (finit()==0)							// Inicializa o SDCARD, opcional
				{
					flashOK=1;
				}
			}
			else
				flashOK=1;
		}
	}
	else if (_DEF_DRIVE==_FLASHFS)
		flashOK=1;
	
	
	if (flashOK==0)
	{
	}
	
	return flashOK;
}

/*----------------------------------------------------------------------------
 * Funções:		Format_Drive
 * Descrição: 	Formata o drive ativo
 * Parâmetros: 	Nenhum
 * Retorno: 	0=Erro 1=Sucesso
 -----------------------------------------------------------------------------*/
unsigned char Format_Drive (void)
{
	if ((STATFILE==_FLWAIT) || 
		(STATFILE==_FLERROR) ||
		(STATFILE==_FLCLOSE) ||
		(STATFILE==_FLBUFFER))							// Verifica status dos trabalhos em Flash
	{
		if (f != NULL)									// Garante que não tenha arquivo aberto.
		{
			fclose (f);
			f=NULL;
		}

		if (_DEF_DRIVE==_FLASHFS)
		{
			if (fformat ("")==0)
			{
				SYSTEM_DONE=_INIFILESYSTEM;				// Força o programa reiniciar F.S.
				STATFILE=_FLWAIT;						// Programa esperando ticket
				return 1;
			}
		}
		else if (_DEF_DRIVE==_SDCARD)
		{
			if (Verifica_FLASH (1))
			{
				if (fformat ("SYSTEM /FAT32")==0)
				{
					SYSTEM_DONE=_INIFILESYSTEM;			// Força o programa reiniciar F.S.
					STATFILE=_FLWAIT;					// Programa esperando ticket
					return 1;
				}
			}
		}
	}

	return 0;
}

/*----------------------------------------------------------------------------
 * Funções:		Format_DataFlash
 * Descrição: 	Formata a flash de dados
 * Parâmetros: 	Nenhum
 * Retorno: 	0=Erro 1=Sucesso
 -----------------------------------------------------------------------------*/
unsigned char Format_DataFlash (void)
{
	unsigned char x;

	if ((STATFILE==_FLWAIT) || 
		(STATFILE==_FLERROR) ||
		(STATFILE==_FLCLOSE) ||
		(STATFILE==_FLBUFFER))							// Verifica status dos trabalhos em Flash
	{
		Erase_DataFlash();								// Apaga toda a Flash Dados
		for (x=0; x<MAXDEP ; x++)
		{
		 	cfg.lastBeforeReport[x]=cfg.depRange[x][0];	// Apaga ultimo ticket antes do relatório
			tktNumber[x]=cfg.depRange[x][0];			// Reinicializa números de tickets
			queue[x]=0;									// Zera fila
			lastCallDpto[x]=0;							// Zera referência de chamadas anteriores
			lastPrioDpto[x]=0;							// Zera referência de chamadas anteriores
		}

		for (x=0; x<MAXUSER ; x++)
		{
			lastCallUser[x]=0;							// Zera referência de chamadas anteriores
			lastPrioUser[x]=0;							// Zera referência de chamadas anteriores
		}
		Write_Flash ();

				
		STATFILE=_FLWAIT;								// Programa esperando ticket
		return 1;
	}

	return 0;
}

/*----------------------------------------------------------------------------
 * Funções:		Change_Drive
 * Descrição: 	Troca o drive ativo do sistema de arquivos (2=FLASHSPI 4=SDCARD)
 * Parâmetros: 	driver - Número do driver
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
unsigned char Change_Drive (unsigned char driver)
{
	if ((STATFILE==_FLWAIT) || 
		(STATFILE==_FLERROR) ||
		(STATFILE==_FLCLOSE) ||
		(STATFILE==_FLBUFFER))							// Verifica status dos trabalhos em Flash
	{
		if (f != NULL)	 								// Garante que não tenha arquivo aberto.
		{
			fclose (f);
			f=NULL;
		}

		_DEF_DRIVE = driver;							// Grava novo driver.
		cfg.driver = driver;
		Write_Flash ();
		
		if (Verifica_FLASH (1))
		{
			if (_DEF_DRIVE==_SDCARD)
				SYSTEM_DONE=_INIFILESYSTEM;				// Força o programa reiniciar F.S.
			STATFILE=_FLWAIT;
			return 1;
		}
	}
	return 0;
}

/*----------------------------------------------------------------------------
 * Funções:		Prog_Watchdog
 * Descrição: 	Programa o watchdog para re-inicializar o sistema
 * Parâmetros: 	value - valor que define o tempo antes da re-inicialização
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void Prog_Watchdog (unsigned int value)
{
	if( LPC_WDT->WDMOD & 0x04 ) 			// Limpa Flag de Time-Out
		LPC_WDT->WDMOD &= ~0x04;

	LPC_SC->PCLKSEL0 &= ~(3UL<<0);			// Configura clock do WatchDog Timer para pclk/8
	LPC_SC->PCLKSEL0 |= (3UL<<0);

	LPC_WDT->WDCLKSEL=0x01;					// Seleciona clock interno, Ajusta valor do Time-Out
	LPC_WDT->WDTC  = value;				   				

	LPC_WDT->WDMOD = 0x03;					// Habilita Time-Out e Reset
	
	LPC_WDT->WDFEED = 0xAA;					// Liga o Watch Dog.
	LPC_WDT->WDFEED = 0x55;  	
}

/*----------------------------------------------------------------------------
 * Funções:		Check_FileSystem
 * Descrição: 	Inicializa sistema de arquivo no driver ativo
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void Check_FileSystem (void)
{
	unsigned int x;
	unsigned char retry=0;

	while(SYSTEM_DONE!=_INICOMPLETE)
	{
		if (SYSTEM_DONE==_INISTART)
		{
			for (x=0;x<30000000;x++);
			SYSTEM_DONE=_INIFILESYSTEM;
		}
		else if (SYSTEM_DONE==_INIFILESYSTEM)			// Inicializa sistema de arquivos
		{												//--------------------------------
			if (_DEF_DRIVE==_SDCARD)	
			{											// Se drive for SD card
				if (Verifica_FLASH (0))					// Verifica se SD card esta inserido
				{
					if (finit()==0)
					{	
						if (cfg.update==_FROMBOOT)
						{
							cfg.update=_NOACTION;
							Write_Flash ();
						}
						SYSTEM_DONE=_INIWRITEFS;
					}
				}
				else
					SYSTEM_DONE=_INICOMPLETE;			// Cartão SD selecionado e não localizado
			}	
			else if (_DEF_DRIVE==_FLASHFS)
			{
				if (cfg.update==_FROMBOOT)
				{
					if (Format_Drive())
					{
						cfg.update=_NOACTION;
						Write_Flash ();
					}
				}
				SYSTEM_DONE=_INIWRITEFS;
			}
			
			if (SYSTEM_DONE==_INIWRITEFS)
			{
			}
			else if (retry++==5)						// Aborta inicialização..
			{
				retry=0;
				SYSTEM_DONE=_INIWRITEFS;
			}
		}
		else if (SYSTEM_DONE==_INIWRITEFS) 				// Testa criação e gravação de arquivos
		{												//---------------------------------------
			if (_DEF_DRIVE==_SDCARD)
			{											// Se drive for SD card
				if (Verifica_FLASH (1))					// Verifica se SD card esta inserido
				{
					f = fopen ("system.ini","w");		// Cria arquivo "system.ini"
					if (f!=NULL)
					{
						if (fputc ('0',f)=='0')			// Grava no arquivo "system.ini"
							SYSTEM_DONE=_INIREADFS;
						fclose (f);
						f=NULL;
					}
				}
				else
					SYSTEM_DONE=_INICOMPLETE;			// Cartão SD selecionado e não localizado
			}
			else
				SYSTEM_DONE=_INIREADFS;
				

			if (SYSTEM_DONE==_INIREADFS)
			{
			}
			else if (retry++==5)						// Aborta gravação..
			{
				retry=0;
				SYSTEM_DONE=_INIREADFS;
			}
		}
		else if (SYSTEM_DONE==_INIREADFS)				// Testa leitura de arquivo
		{											    //---------------------------
			if (_DEF_DRIVE==_SDCARD)
			{
				if (Verifica_FLASH (1))	  				// Verifica se o cartão SD esta inserido
				{
					f = fopen ("system.ini","r");		// Faz leitura do arquivo "system.ini"
					if (f!=NULL)
					{
						if (fgetc(f)==(int)'0')
							SYSTEM_DONE=_INIDATA;
						fclose (f);
						f=NULL;
					}
				}
				else
					SYSTEM_DONE=_INICOMPLETE;			// Cartão SD selecionado e não localizado
			}
			else
				SYSTEM_DONE=_INIDATA;

			if (SYSTEM_DONE==_INIDATA)
			{
			}
			else if (retry++==5)						// Aborta leitura..
			{
				retry=0;
				SYSTEM_DONE=_INIDATA;
			}
		}
		else if (SYSTEM_DONE==_INIDATA)
		{
			Init_Address_DataFlash ();					// Inicializa ponteiros da flash de dados
			SYSTEM_DONE=_INIFILE;
		}
		else if (SYSTEM_DONE==_INIFILE)
		{
			STATFILE=_FLWAIT;							// Aguardando..
			SYSTEM_DONE=_INICOMPLETE; 					// Inicialização finalizada ..
			WEBREQUEST=REQ_NONE;
		}
	}
}

/*----------------------------------------------------------------------------
 * Funções:		MakeFileName
 * Descrição: 	Monta nome do arquivo para gravar os relatórios
 * Parâmetros: 	pName - Ponteiro para o array com o nome do arquivo
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void MakeFileName (char *pName)
{
	TIME_TYPE time;		
	time=Get_RTC ();
	if (_DEF_DRIVE==_SDCARD)
	{											
		pName[0]='r';
		pName[1]='e';//(time.MONTH/10)|0x30;
		pName[2]='l';//(time.MONTH%10)|0x30;
		pName[3]='\\';
		pName[4]=(time.DAY/10)|0x30;
		pName[5]=(time.DAY%10)|0x30;
		pName[6]=(time.MONTH/10)|0x30;
		pName[7]=(time.MONTH%10)|0x30;
		pName[8]=((time.YEAR/1000)%10)|0x30;
		pName[9]=((time.YEAR/100)%10)|0x30;
		pName[10]=((time.YEAR/10)%10)|0x30;
		pName[11]=(time.YEAR%10)|0x30;
		pName[12]='.';
		pName[13]='c';
		pName[14]='s';
		pName[15]='v';											
		pName[16]=0;
	}
	else if (_DEF_DRIVE==_FLASHFS)
	{
		pName[0]=(time.DAY/10)|0x30;
		pName[1]=(time.DAY%10)|0x30;
		pName[2]=(time.MONTH/10)|0x30;
		pName[3]=(time.MONTH%10)|0x30;
		pName[4]=((time.YEAR/1000)%10)|0x30;
		pName[5]=((time.YEAR/100)%10)|0x30;
		pName[6]=((time.YEAR/10)%10)|0x30;
		pName[7]=(time.YEAR%10)|0x30;
		pName[8]='.';
		pName[9]='c';
		pName[10]='s';
		pName[11]='v';											
		pName[12]=0;
	}
}

/*----------------------------------------------------------------------------
 * Funções:		PutBuf
 * Descrição: 	Verifica se o número de bytes no buffer é igual ao setor do FS, 
 *				se for igual salva em arquivo. Armazena próximo byte no buffer.
 * Parâmetros: 	Nenhum
 * Retorno: 	(0) Erro na tentativa de gravar em arquivo (1) Sem erros
 -----------------------------------------------------------------------------*/
char PutBuf (char byte)
{
	if ((_DEF_DRIVE==_SDCARD && nDat==512) ||
		(_DEF_DRIVE==_FLASHFS && nDat==256))
		if (SaveBlock ()==0)
			return 0;

	bufDat[nDat++]=byte;
	return 1;
}

/*----------------------------------------------------------------------------
 * Funções:		SaveBlock
 * Descrição: 	Salva o número de bytes indicados em "nDat" presentes no buffer 
 *				"bufDat" no arquivo "f"
 * Parâmetros: 	Nenhum
 * Retorno: 	(0) Erro na tentativa de gravar em arquivo (1) Sem erros
 -----------------------------------------------------------------------------*/
char SaveBlock (void)
{
	unsigned char *pDat = bufDat;						// ponteiro para buffer em RAM
	
	if (fwrite (pDat,1,nDat,f)!=nDat)
	{
		STATFILE=_FLERROR;
		return 0;
	}
	nDat = 0;

	return 1;
}

/*----------------------------------------------------------------------------
 * Funções:		Save_FormatInfo
 * Descrição: 	Formata e salva o texto para gravar em arquivo o relatório
 * Parâmetros: 	*atd - Ponteiro para a estrutura de atendimento a ser formatada
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void Save_FormatInfo (ATD *atd)
{
	TIME_TYPE timeImp;
	TIME_TYPE timeIni;
	TIME_TYPE timeEnd;

	timeImp=UTC_To_RTC(1970,atd->utcImp);
	timeIni=UTC_To_RTC(1970,atd->utcIni);
	timeEnd=UTC_To_RTC(1970,atd->utcEnd);

	if (atd->staEnd!=0xFF){
		switch (atd->staEnd){
			case STA_CLOSED:	PutBuf('F'); break;
			case STA_CANCELED:	PutBuf('C'); break;
			case STA_CLOSEDREP:	PutBuf('F'); PutBuf('R'); break;
			case STA_CLOSEDAUTO:PutBuf('F'); PutBuf('A'); break;
			case STA_REDIRECT:	PutBuf('F'); PutBuf('E'); break;
			case STA_CLOSEDINIT:PutBuf('F'); PutBuf('I'); break;
			case STA_CLOSEDOVER:PutBuf('F'); PutBuf('O'); break;
			default:			PutBuf(' '); break; 
		}
	} else if (atd->staAtend!=0xFF) {
		PutBuf('A');
	} else if (atd->staIni!=0xFF) {
		PutBuf('E');
	} else {
		PutBuf(' ');
	}
	PutBuf(';');

	if (atd->depNum>=10) 	PutBuf((atd->depNum/10)|0x30);
	PutBuf((atd->depNum%10)|0x30);
	PutBuf(';');

	if (atd->priority) {
		if (atd->depNum==13)
	 		PutBuf('O');
		else
			PutBuf('P');
	} else {
		PutBuf('N');
	}
	PutBuf(';');

	if (atd->userNum!=0xFF) {
		if (atd->userNum>=100) 	PutBuf(((atd->userNum/100)%10)|0x30);
		if (atd->userNum>=10)  	PutBuf(((atd->userNum/10)%10)|0x30);
		PutBuf((atd->userNum%10)|0x30);
	} else {
		PutBuf(0x30);
	}
	PutBuf(';');

	if (atd->alphaNum!=0) 	PutBuf(atd->alphaNum);
	if (atd->number>=1000) 	PutBuf(((atd->number/1000)%10)|0x30);
	if (atd->number>=100)  	PutBuf(((atd->number/100)%10)|0x30);
	if (atd->number>=10)  	PutBuf(((atd->number/10)%10)|0x30);
	PutBuf((atd->number%10)|0x30);
	PutBuf(';');

	if (timeImp.HOUR>=10) 	PutBuf((timeImp.HOUR/10)|0x30);
	PutBuf((timeImp.HOUR%10)|0x30);
	PutBuf(':');
	if (timeImp.MINUTE>=10) 	PutBuf((timeImp.MINUTE/10)|0x30);
	PutBuf((timeImp.MINUTE%10)|0x30);
	PutBuf(':');
	if (timeImp.SECOND>=10) 	PutBuf((timeImp.SECOND/10)|0x30);
	PutBuf((timeImp.SECOND%10)|0x30);
	PutBuf(' ');
	if (timeImp.DAY>=10) 		PutBuf((timeImp.DAY/10)|0x30);
	PutBuf((timeImp.DAY%10)|0x30);
	PutBuf('/');
	if (timeImp.MONTH>=10) 	PutBuf((timeImp.MONTH/10)|0x30);
	PutBuf((timeImp.MONTH%10)|0x30);
	PutBuf('/');
	PutBuf(((timeImp.YEAR/10)%10)|0x30);
	PutBuf((timeImp.YEAR%10)|0x30);
	PutBuf(';');

	if (atd->staAtend==0xFF)
	{
//		PutBuf('0');
//		PutBuf(':');
//		PutBuf('0');
//		PutBuf(':');
//		PutBuf('0');
//		PutBuf(' ');
//		PutBuf('0');
//		PutBuf('/');
//		PutBuf('0');
//		PutBuf('/');
//		PutBuf('0');
	}
	else
	{
		if (timeIni.HOUR>=10) 	PutBuf((timeIni.HOUR/10)|0x30);
		PutBuf((timeIni.HOUR%10)|0x30);
		PutBuf(':');
		if (timeIni.MINUTE>=10) 	PutBuf((timeIni.MINUTE/10)|0x30);
		PutBuf((timeIni.MINUTE%10)|0x30);
		PutBuf(':');
		if (timeIni.SECOND>=10) 	PutBuf((timeIni.SECOND/10)|0x30);
		PutBuf((timeIni.SECOND%10)|0x30);
		PutBuf(' ');
		if (timeIni.DAY>=10) 		PutBuf((timeIni.DAY/10)|0x30);
		PutBuf((timeIni.DAY%10)|0x30);
		PutBuf('/');
		if (timeIni.MONTH>=10) 	PutBuf((timeIni.MONTH/10)|0x30);
		PutBuf((timeIni.MONTH%10)|0x30);
		PutBuf('/');
		PutBuf(((timeIni.YEAR/10)%10)|0x30);
		PutBuf((timeIni.YEAR%10)|0x30);
	}
	PutBuf(';');

	if (atd->staEnd==0xFF)
	{
//		PutBuf('0');
//		PutBuf(':');
//		PutBuf('0');
//		PutBuf(':');
//		PutBuf('0');
//		PutBuf(' ');
//		PutBuf('0');
//		PutBuf('/');
//		PutBuf('0');
//		PutBuf('/');
//		PutBuf('0');
	}
	else
	{
		if (timeEnd.HOUR>=10) 	PutBuf((timeEnd.HOUR/10)|0x30);
		PutBuf((timeEnd.HOUR%10)|0x30);
		PutBuf(':');
		if (timeEnd.MINUTE>=10) 	PutBuf((timeEnd.MINUTE/10)|0x30);
		PutBuf((timeEnd.MINUTE%10)|0x30);
		PutBuf(':');
		if (timeEnd.SECOND>=10) 	PutBuf((timeEnd.SECOND/10)|0x30);
		PutBuf((timeEnd.SECOND%10)|0x30);
		PutBuf(' ');
		if (timeEnd.DAY>=10) 		PutBuf((timeEnd.DAY/10)|0x30);
		PutBuf((timeEnd.DAY%10)|0x30);
		PutBuf('/');
		if (timeEnd.MONTH>=10) 	PutBuf((timeEnd.MONTH/10)|0x30);
		PutBuf((timeEnd.MONTH%10)|0x30);
		PutBuf('/');
		PutBuf(((timeEnd.YEAR/10)%10)|0x30);
		PutBuf((timeEnd.YEAR%10)|0x30);
	}
	PutBuf(';');

	if (atd->queue>=10000) 	PutBuf(((atd->queue/10000)%10)|0x30);
	if (atd->queue>=1000)  	PutBuf(((atd->queue/1000)%10)|0x30);
	if (atd->queue>=100)   	PutBuf(((atd->queue/100)%10)|0x30);
	if (atd->queue>=10)    	PutBuf(((atd->queue/10)%10)|0x30);
	PutBuf((atd->queue%10)|0x30);
	PutBuf(';');

	if (atd->opinion!=0xFF)	
		PutBuf(atd->opinion);
	else
		PutBuf('X');
	PutBuf(';');

	PutBuf(13);
	PutBuf(10);
}

/*----------------------------------------------------------------------------
 * Funções:		StateMachine_ticket
 * Descrição: 	Estados de máquina para gravação de dados e arquivos
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void StateMachine_ticket (void)
{
	
	if (STATFILE==_FLBUFFER)
	{
		unsigned char *pt=(unsigned char*)&atend;		// Aponta para os dados do novo ticket
		Write_DataFlash (pt, 14);						// Grava o novo ticket na Flash de dados
		
		STATFILE=_FLWAIT;								// Volta para aguardar outros tickets
	}

	else if (STATFILE==_FLANALYZE)
	{
		ATD atd;
		if (DataFlash_Exist_NotRead())					// Verifica se existem dados não recuperados no dia
		{
			if (Check_liveTicket (&atd))				// Verifica tempo de vida do ticket, se existir algum
			{											// excedendo o tempo abre o arquivo para gravar..
				STATFILE=_FLOPEN;						// Status para abrir arquivo para gravação
			}
			else
				STATFILE=_FLWAIT;						// Não existem dados para gravar..
		}			
		else
			STATFILE=_FLWAIT;							// Não existem dados para gravar..
	}

	else if (STATFILE==_FLOPEN)
	{
		if (Verifica_FLASH (1))	   					// Verifica se SdCard esta inserido
		{
			unsigned short x;
			char fileName[17];
			MakeFileName (fileName);				// Monta nome novo de arquivo				
			info.fileID = 0;
			if (ffind (fileName,&info)!=0)			// Procura pelo nome do arquivo.. se não existir ainda..
			{										// cria cabeçalho e cria arquivo novo.
				f = fopen ((const char *)
						fileName,"w");				// Cria um arquivo novo e abre para gravação
								
				if (f != NULL)
				{	
					for (x=0; x<HEADERLEN; x++) 	// Salva cabeçalho do arquivo .CSV
						PutBuf (HEADERFILE[x]);
					STATFILE=_FLSAVE;				// Status para salvar dados no arquivo
				}
				else
					STATFILE=_FLERROR;				// Ocorreu um erro no arquivo 
			}
			else
			{
				f = fopen ((const char *)
						fileName,"a");				// Abre arquivo para adicionar dados
								
				if (f != NULL)
					STATFILE=_FLSAVE;				// Status para salvar dados no arquivo
				else
					STATFILE=_FLERROR;				// Ocorreu um erro no arquivo 
			}
		}
		else
			STATFILE=_FLWAIT;						// Cartão SD não inserido volta para coletar tickets..
	}
	
	else if (STATFILE==_FLSAVE)
	{
		ATD atd;

		if (Close_ticket (&atd))						// Verifica tempo de vida do ticket e encerra
		{
			Save_FormatInfo (&atd);						// Formata dados e grava em bufDat
			Adjust_ReadDataFlash();						// Ajusta ponteiro da FLASH após tratar pacote
			if (DataFlash_Exist_NotRead()==0)			// Se não houver mais dados para gravar muda status
			{
				if (nDat!=0)							// Verifica se sobrou algum dado para salvar
					SaveBlock ();						// Salva os dados antes de fechar..
				STATFILE=_FLCLOSE;
			}
		}
		else											// Aqui ainda podem existir tickets não fechados
		{
			if (nDat!=0)								// Verifica se sobrou algum dado para salvar
				SaveBlock ();							// Salva os dados antes de fechar..
			STATFILE=_FLCLOSE;							// Não há senhas para gravar em arquivo
		}
	}

	else if (STATFILE==_FLCLOSE)
	{
		if (f != NULL)
		{
			fclose (f);
			f=NULL;
		}
		STATFILE=_FLWAIT;
		WEBREQUEST=REQ_NONE;
	}

	else if (STATFILE==_FLERROR)
	{
		if (f != NULL)
		{
			fclose (f);
			f=NULL;
		}
		STATFILE=_FLWAIT;
		WEBREQUEST=REQ_NONE;
	}

	else if (STATFILE==_FLIGNORE)
	{
		ATD atd;
		if (DataFlash_Exist_NotRead())					// Verifica se existem dados não recuperados no dia
		{
			if (Close_ticket (&atd))					// Verifica tempo de vida do ticket, se 
			{											// existir algum excedendo o tempo..
				Adjust_ReadDataFlash();					// Ajusta ponteiro da FLASH após tratar pacote
			}
			else
			{
				STATFILE=_FLWAIT;						// Não existem mais tickets excedendo o tempo
				WEBREQUEST=REQ_NONE;
			}
		}			
		else
		{
			STATFILE=_FLWAIT;							// Não existem mais tickets não lidos
			WEBREQUEST=REQ_NONE;
		}
	}
}

 /*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
