/*----------------------------------------------------------------------------
 *      RL-ARM - TCPnet
 *----------------------------------------------------------------------------
 *      Name:    HTTP_CGI.C
 *      Purpose: HTTP Server CGI Module
 *      Rev.:    V4.05
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2009 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include 	<Net_Config.h>
#include 	<File_Config.h>
#include 	<stdio.h>
#include 	<string.h>
#include 	"RTC_CONFIG.h"
#include	"LCD_CONFIG.h"
#include	"IMP_CONFIG.h"
#include	"IAP_CONFIG.h"
#include	"COMM_CONFIG.h"
#include	"..\\APP.h"
#include 	"..\\QUEUE.h"
#include 	"..\\REPORT.h"
#include	"CRC16.h"

/* ---------------------------------------------------------------------------
 * The HTTP server provides a small scripting language.
 *
 * The script language is simple and works as follows. Each script line starts
 * with a command character, either "i", "t", "c", "#" or ".".
 *   "i" - command tells the script interpreter to "include" a file from the
 *         virtual file system and output it to the web browser.
 *   "t" - command should be followed by a line of text that is to be output
 *         to the browser.
 *   "c" - command is used to call one of the C functions from the this file.
 *         It may be followed by the line of text. This text is passed to
 *         'cgi_func()' as a pointer to environment variable.
 *   "#' - command is a comment line and is ignored (the "#" denotes a comment)
 *   "." - denotes the last script line.
 *
 * --------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *      Queue.c   variables
 *---------------------------------------------------------------------------*/
extern unsigned short	queue [MAXDEP];

/*----------------------------------------------------------------------------
 *      Report.c   variables
 *---------------------------------------------------------------------------*/
extern unsigned char STATFILE;

/*----------------------------------------------------------------------------
 *      App.c   variables
 *---------------------------------------------------------------------------*/
extern unsigned char 	versionFirm[2];
extern unsigned char 	programMode;
extern CALL			  	call[];
extern CFG 			  	cfg;
extern unsigned char 	runAlpha;
extern unsigned short	runTicket;
extern unsigned char	runUser;
extern unsigned char	runType;
extern char				lastAlpha[];
extern unsigned short	lastTicket[];
extern char 			lastAlphaDep[];
extern unsigned short 	lastTicketDep[];
extern char 			lastUser[];

/*----------------------------------------------------------------------------
 *      NetConfig.c   variables
 *---------------------------------------------------------------------------*/
extern  LOCALM 	localm[];
#define LocM   	localm[NETIF_ETH]
extern struct 	tcp_info tcp_socket[];
extern unsigned char const tcp_NumSocks;
extern unsigned char http_EnAuth;
extern unsigned char http_auth_passw[20];
extern unsigned char lhost_name[16];
extern unsigned char own_hw_adr[ETH_ADRLEN];

/*----------------------------------------------------------------------------
 *      FileConfig.c   variables
 *---------------------------------------------------------------------------*/
extern unsigned short  _DEF_DRIVE;

/*----------------------------------------------------------------------------
 *      Local   variables
 *---------------------------------------------------------------------------*/
unsigned char			WEBREQUEST=REQ_NONE;

static unsigned char  	DEP=1;
static unsigned char   	USER=1;
static unsigned char 	taskFile=0;

unsigned char			httpAlpha=' ';							// Chamada de ticket http
unsigned short 			httpNumber=0;
unsigned char   		httpUser=0;
unsigned char 			httpRow=0;
unsigned char   		httpFunc=0;
unsigned char			httpDep=0;

static char const 		state[][11] = {
									  "FREE",
									  "CLOSED",
									  "LISTEN",
									  "SYN_REC",
									  "SYN_SENT",
									  "FINW1",
									  "FINW2",
									  "CLOSING",
									  "LAST_ACK",
									  "TWAIT",
									  "CONNECT"
									  };

/* My structure of CGI status U32 variable. This variable is private for */
/* each HTTP Session and is not altered by HTTP Server. It is only set to  */
/* zero when the cgi_func() is called for the first time.                  */
typedef struct {
  U16 xcnt;
  U16 unused;
} MY_BUF;
#define MYBUF(p)        ((MY_BUF *)p)

/*----------------------------------------------------------------------------
 *      Local Functions
 *---------------------------------------------------------------------------*/
void dot_format (U64 val, char *sp);
char *Show_DevModel (unsigned char devModel);
char* verType (unsigned char runType);

extern void Write_Flash (void);

/*----------------------------------------------------------------------------
 * HTTP Server Common Gateway Interface Functions
 *---------------------------------------------------------------------------*/

/*--------------------------- cgi_process_var -------------------------------*/

void cgi_process_var (unsigned char *qs) 
{
   	/* This function is called by HTTP server to process the Querry_String   */
   	/* for the CGI Form GET method. It is called on SUBMIT from the browser. */
   	/*.The Querry_String.is SPACE terminated.     
	*/
   	static unsigned char *var;

   	var = (unsigned char *)alloc_mem (40);	
   	do 
   	{
      	/* Loop through all the parameters. */
      	qs = http_get_var (qs, var, 40);
      
	  	/* Check the returned string, 'qs' now points to the next. */
      	if (var[0] != 0) 
	  	{
         	/* Returned string is non 0-length. */
         	if (programMode!=0)
			{
				free_mem ((OS_FRAME *)var);

				return;
			}
		}
   	}while (qs);

	free_mem ((OS_FRAME *)var);	
}

/*----------------------------------------------------------------------------
 * Funções:		Erase_Image
 * Descrição: 	Limpa área da imagem em RAM
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void Erase_Image (unsigned char *pont, unsigned short length)
{
	unsigned short x;
	for (x=0 ; x<length ; x++)
		*pont++=0;
}

/*--------------------------- cgi_process_data ------------------------------*/

void cgi_process_data (unsigned char code, unsigned char *dat, unsigned short len) 
{
  	/* This function is called by HTTP server to process the returned Data    */
  	/* for the CGI Form POST method. It is called on SUBMIT from the browser. */
  	/* Parameters:                                                            */
  	/*   code  - callback context code                                        */
  	/*           0 = www-url-encoded form data                                */
  	/*           1 = filename for file upload (0-terminated string)           */
  	/*           2 = file upload raw data                                     */
  	/*           3 = end of file upload (file close requested)                */
  	/*           4 = any xml encoded POST data (single or last stream)        */
  	/*           5 = the same as 4, but with more xml data to follow          */
  	/*               Use http_get_content_type() to check the content type    */  
  	/*   dat   - pointer to POST received data                                */
  	/*   len   - received data length                                         */

	static unsigned char *var;
	TIME_TYPE time;
  	
	char doFormat;
	char doIniTkt;
	char doIniCfg;
	char doDateTime;
	char doNetwork;
	char doCall;

	int val=0;
	int s[6];
	unsigned char passw[12],retyped[12];
	unsigned char stpassw;
	char c;

  	switch (code)
	{
    	case 0:
      		/* Url encoded form data received. */
      		break;

    	case 1:
      		/* Filename for file upload received as encoded by the browser. */
      		/* It might contain an absolute path to a file from the sending */
      		/* host. Open a file for writing. */
			switch (taskFile)
			{
				case 3:	Erase_Image (cfg.logoImp,1024); 
						cfg.logoImpWidth=0;
						cfg.logoImpHeight=0;
						break;
			}
     		return;

    	case 2:
      		/* File content data received. Write data to a file. */
      		/* This function will be called several times with   */
      		/* code 2 when a big file is being uploaded.         */

			switch (taskFile)
			{
				case 3:	PrintENG_CompileRamBitmap(dat, len, cfg.logoImp, &cfg.logoImpWidth, 
													&cfg.logoImpHeight, cfg.ticketOrientation);
						break;
			}
   			return;

    	case 3:
      		/* File upload finished. Close a file. */
			Write_Flash ();
      		return;

    	default:
      		/* Ignore all other codes. */
      		return;
  	}

  	if (len == 0) 
	{
    	return;
  	}

  	stpassw=0;
	doFormat=0;
	doIniTkt=0;
	doIniCfg=0;
	doDateTime=0;
	doNetwork=0;
	doCall=0;

  	var = (unsigned char *)alloc_mem (60);
  	do 
	{
    	/* Parse all returned parameters. */
    	dat = http_get_var (dat, var, 60);
    	if (var[0] != 0) 
		{
         	if (programMode!=0)
			{
				free_mem ((OS_FRAME *)var);

				return;
			}

			/* Parameter found, returned string is non 0-length. */
			if (str_scomp (var, "pg=network") == __TRUE)		// Página network.cgi
			{ 													//-------------------
				
			}
			else if (str_scomp (var, "hname=") == __TRUE)
			{
            	str_copy (lhost_name, var+6);
				str_copy (cfg.hostName, var+6);
				doNetwork=1;
         	}
			else if (str_scomp (var, "dhcp=0-") == __TRUE)
			{
				cfg.dhcpStatus = 0;
				doNetwork=1;
			}
			else if (str_scomp (var, "dhcp=1-") == __TRUE)
			{
				cfg.dhcpStatus = 1;
				doNetwork=1;
			}
		 	else if (str_scomp (var, "ip=") == __TRUE) 
		 	{
				sscanf ((const char *)&var[3], "%d.%d.%d.%d",&s[0],&s[1],&s[2],&s[3]);
            	cfg.localIp[0] = s[0];
            	cfg.localIp[1] = s[1];
            	cfg.localIp[2] = s[2];
            	cfg.localIp[3] = s[3];
				if (cfg.dhcpStatus==0)
				{
            		LocM.IpAdr[0] = cfg.localIp[0];
            		LocM.IpAdr[1] = cfg.localIp[1];
            		LocM.IpAdr[2] = cfg.localIp[2];
            		LocM.IpAdr[3] = cfg.localIp[3];
				}
				doNetwork=1;
         	}
         	else if (str_scomp (var, "msk=") == __TRUE) 
			{
	           	sscanf ((const char *)&var[4], "%d.%d.%d.%d",&s[0],&s[1],&s[2],&s[3]);
            	cfg.ipMask[0] = s[0];
            	cfg.ipMask[1] = s[1];
            	cfg.ipMask[2] = s[2];
            	cfg.ipMask[3] = s[3];
				if (cfg.dhcpStatus==0)
				{
            		LocM.NetMask[0] = cfg.ipMask[0];
            		LocM.NetMask[1] = cfg.ipMask[1];
            		LocM.NetMask[2] = cfg.ipMask[2];
            		LocM.NetMask[3] = cfg.ipMask[3];
					doNetwork=1;
				}
         	}
         	else if (str_scomp (var, "gw=") == __TRUE) 
			{
            	sscanf ((const char *)&var[3], "%d.%d.%d.%d",&s[0],&s[1],&s[2],&s[3]);
            	cfg.gateway[0] = LocM.DefGW[0] = s[0];
            	cfg.gateway[1] = LocM.DefGW[1] = s[1];
            	cfg.gateway[2] = LocM.DefGW[2] = s[2];
            	cfg.gateway[3] = LocM.DefGW[3] = s[3];
				doNetwork=1;
         	}
         	else if (str_scomp (var, "dn1=") == __TRUE) 
			{
            	sscanf ((const char *)&var[4], "%d.%d.%d.%d",&s[0],&s[1],&s[2],&s[3]);
            	cfg.dns1[0] = s[0];
            	cfg.dns1[1] = s[1];
            	cfg.dns1[2] = s[2];
            	cfg.dns1[3] = s[3];
				if (cfg.dhcpStatus==0)
				{
	            	LocM.PriDNS[0] = cfg.dns1[0];
	            	LocM.PriDNS[1] = cfg.dns1[1];
	            	LocM.PriDNS[2] = cfg.dns1[2];
	            	LocM.PriDNS[3] = cfg.dns1[3];
					doNetwork=1;
				}
         	}
         	else if (str_scomp (var, "dn2=") == __TRUE) 
			{
            	sscanf ((const char *)&var[4], "%d.%d.%d.%d",&s[0],&s[1],&s[2],&s[3]);
            	cfg.dns2[0] = s[0];
            	cfg.dns2[1] = s[1];
            	cfg.dns2[2] = s[2];
            	cfg.dns2[3] = s[3];
				if (cfg.dhcpStatus==0)
				{
	            	LocM.SecDNS[0] = cfg.dns2[0];
	            	LocM.SecDNS[1] = cfg.dns2[1];
	            	LocM.SecDNS[2] = cfg.dns2[2];
	            	LocM.SecDNS[3] = cfg.dns2[3];
					doNetwork=1;
				}
         	}
			else if (str_scomp (var, "pg=design") == __TRUE)	// Página design.cgi
				taskFile=REQ_NONE;								//-------------------
			else if (str_scomp (var, "tsk=1") == __TRUE) 
       			taskFile=UPD_SCR1;
			else if (str_scomp (var, "tsk=2") == __TRUE) 			
       			taskFile=UPD_SCR2;
			else if (str_scomp (var, "tsk=3") == __TRUE) 			
       			taskFile=UPD_LOGO;

			else if (str_scomp (var, "pg=update") == __TRUE)	// Página update.cgi
			{ 													//-------------------
				WEBREQUEST=UPD_FIRM;							// Força reinicialização
				cfg.update=_FROMAPP;							// para a área do bootloader
			}

			else if (str_scomp (var, "format=on") == __TRUE)	// Página format.cgi
        		doFormat = 1;									//-------------------
			else if (str_scomp (var, "finit=on") == __TRUE)
				doIniCfg = 1;
			else if (str_scomp (var, "ftkt=on") == __TRUE)
				doIniTkt = 1;

         	else if (str_scomp (var, "auth=0") == __TRUE)		// Página ver.cgi
         	   	cfg.authEnable=http_EnAuth=0;					//-------------------
			else if (str_scomp (var, "auth=1") == __TRUE)		
         	   	cfg.authEnable=http_EnAuth=1;				   	
			else if (str_scomp (var, "pw=") == __TRUE) 							
			{
            	str_copy (passw, var+3);
            	stpassw |= 1;
         	}
         	else if (str_scomp (var, "pw2=") == __TRUE) 
			{
            	str_copy (retyped, var+4);
            	stpassw |= 2;
         	}

	        else if (str_scomp (var, "dnum=") == __TRUE)		// Página seldep.cgi
			{													//-------------------
				sscanf ((const char *)&var[5], "%d",&val);
				DEP=val;
			}

			else if (str_scomp (var, "pg=dep") == __TRUE)		// Página dep.cgi
			{													//----------------
				cfg.depEnable[DEP-1]=0;							// Zera variáveis para gravar 
				cfg.depCopy[DEP-1]=0;
				cfg.depPref[DEP-1]=0;
				cfg.depCloseNext[DEP-1]=0;						// nova configuração
				cfg.depCloseRep[DEP-1]=0;
				cfg.depOptionHlp[DEP-1]=0;
				cfg.checkTime[DEP-1]=0;
				cfg.optSchHlp[DEP-1]=0;
				for (c=0 ; c<(MAXUSER) ; c++)
					if (cfg.depUsers[c]==DEP)
						cfg.depUsers[c]=0;
				cfg.eTop[DEP-1]=0;
			}															
			else if (str_scomp (var, "den=on") == __TRUE) 		
				cfg.depEnable[DEP-1]=1;
			else if (str_scomp (var, "dname=") == __TRUE)
			{
				for (c=0 ; c<49 ; c++)
					cfg.depName[DEP-1][c]=0;
				str_copy ((U8 *)cfg.depName[DEP-1], var+6);
			}
			else if (str_scomp (var, "dlet=") == __TRUE)
			{
				sscanf ((const char *)&var[5], "%c",&c);
				cfg.depAlpha[DEP-1]=c;
			}
			else if (str_scomp (var, "drang1=") == __TRUE)
			{
				sscanf ((const char *)&var[7], "%d",&val);
				if ((val>0)&&(val<=9998))	
				{
					cfg.depRange[DEP-1][0] = val;
				}
			}
			else if (str_scomp (var, "drang2=") == __TRUE)
			{
				sscanf ((const char *)&var[7], "%d",&val);
				if ((val>0)&&(val<=9999))	
					if (val>cfg.depRange[DEP-1][0])
						cfg.depRange[DEP-1][1]=val;
					else
						cfg.depRange[DEP-1][1]=cfg.depRange[DEP-1][0]+1;

				if ((tktNumber[DEP-1]<cfg.depRange[DEP-1][0]) ||// Número do ticket ficou fora do range ?
					(tktNumber[DEP-1]>cfg.depRange[DEP-1][1]))	// então ajusta número do ticket
					tktNumber[DEP-1] = cfg.depRange[DEP-1][0];
			}
			else if (str_scomp (var, "via=") == __TRUE)
			{
				sscanf ((const char *)&var[4], "%d",&val);
				if (val>=1 && val<=2)
					cfg.depCopy[DEP-1]=val;
			}
			else if (str_scomp (var, "dpref=on") == __TRUE)
			{
				cfg.depPref[DEP-1]=1;
			}
			else if (str_scomp (var, "dtc1=on") == __TRUE)
			{
				cfg.depCloseNext[DEP-1]=1;
			}
			else if (str_scomp (var, "dtc2=on") == __TRUE)
			{
				cfg.depCloseRep[DEP-1]=1;
			}
			else if (str_scomp (var, "dsch=") == __TRUE)
			{
				sscanf ((const char *)&var[5], "%d",&val);
				if (val!=cfg.depSchPri[DEP-1])					// valor do esquema foi modificado ?
					contSchPri[DEP-1] = 0;						// então zera contador
				cfg.depSchPri[DEP-1]=val;
			}
			else if (str_scomp (var, "dhelp1=") == __TRUE)
			{
				sscanf ((const char *)&var[7], "%d",&val);
				if (val!=DEP)
					cfg.depHelp[DEP-1]=((val&0xF)<<4);
			}
			else if (str_scomp (var, "dhelp2=") == __TRUE)
			{
				sscanf ((const char *)&var[7], "%d",&val);
				if (val!=DEP)
					cfg.depHelp[DEP-1]|=(val&0xF);
			}
			else if (str_scomp (var, "dhelp3=on") == __TRUE)
				cfg.depOptionHlp[DEP-1]|=0x10;
			else if (str_scomp (var, "dhelp4=") == __TRUE)
			{
				sscanf ((const char *)&var[7], "%d",&val);
				if (val!=cfg.depSchHlp[DEP-1])					// valor do esquema foi modificado ?
					contSchHlp[DEP-1] = 0;						// então zera contador
				cfg.depSchHlp[DEP-1]=val;
			}
			else if (str_scomp (var, "dhelp5=on") == __TRUE)
				cfg.depOptionHlp[DEP-1]|=0x01;
			else if (str_scomp (var, "dhelp6=on") == __TRUE)
				cfg.checkTime[DEP-1]=0x01;
			else if (str_scomp (var, "dhelp7=on") == __TRUE)
				cfg.optSchHlp[DEP-1]=0x01;
			else if (str_scomp (var, "du1=on") == __TRUE)
				cfg.depUsers[0]=DEP;
			else if (str_scomp (var, "du2=on") == __TRUE)
				cfg.depUsers[1]=DEP;
			else if (str_scomp (var, "du3=on") == __TRUE)
				cfg.depUsers[2]=DEP;
			else if (str_scomp (var, "du4=on") == __TRUE)
				cfg.depUsers[3]=DEP;
			else if (str_scomp (var, "du5=on") == __TRUE)
				cfg.depUsers[4]=DEP;
			else if (str_scomp (var, "du6=on") == __TRUE)
				cfg.depUsers[5]=DEP;
			else if (str_scomp (var, "du7=on") == __TRUE)
				cfg.depUsers[6]=DEP;
			else if (str_scomp (var, "du8=on") == __TRUE)
				cfg.depUsers[7]=DEP;
			else if (str_scomp (var, "du9=on") == __TRUE)
				cfg.depUsers[8]=DEP;
			else if (str_scomp (var, "du10=on") == __TRUE)
				cfg.depUsers[9]=DEP;
			else if (str_scomp (var, "du11=on") == __TRUE)
				cfg.depUsers[10]=DEP;
			else if (str_scomp (var, "du12=on") == __TRUE)
				cfg.depUsers[11]=DEP;
			else if (str_scomp (var, "du13=on") == __TRUE)
				cfg.depUsers[12]=DEP;
			else if (str_scomp (var, "du14=on") == __TRUE)
				cfg.depUsers[13]=DEP;
			else if (str_scomp (var, "du15=on") == __TRUE)
				cfg.depUsers[14]=DEP;
			else if (str_scomp (var, "du16=on") == __TRUE)
				cfg.depUsers[15]=DEP;
			else if (str_scomp (var, "du17=on") == __TRUE)
				cfg.depUsers[16]=DEP;
			else if (str_scomp (var, "du18=on") == __TRUE)
				cfg.depUsers[17]=DEP;
			else if (str_scomp (var, "du19=on") == __TRUE)
				cfg.depUsers[18]=DEP;
			else if (str_scomp (var, "du20=on") == __TRUE)
				cfg.depUsers[19]=DEP;
			else if (str_scomp (var, "du21=on") == __TRUE)
				cfg.depUsers[20]=DEP;
			else if (str_scomp (var, "du22=on") == __TRUE)
				cfg.depUsers[21]=DEP;
			else if (str_scomp (var, "du23=on") == __TRUE)
				cfg.depUsers[22]=DEP;
			else if (str_scomp (var, "du24=on") == __TRUE)
				cfg.depUsers[23]=DEP;
			else if (str_scomp (var, "du25=on") == __TRUE)
				cfg.depUsers[24]=DEP;
			else if (str_scomp (var, "du26=on") == __TRUE)
				cfg.depUsers[25]=DEP;
			else if (str_scomp (var, "du27=on") == __TRUE)
				cfg.depUsers[26]=DEP;
			else if (str_scomp (var, "du28=on") == __TRUE)
				cfg.depUsers[27]=DEP;
			else if (str_scomp (var, "du29=on") == __TRUE)
				cfg.depUsers[28]=DEP;
			else if (str_scomp (var, "du30=on") == __TRUE)
				cfg.depUsers[29]=DEP;
			else if (str_scomp (var, "du31=on") == __TRUE)
				cfg.depUsers[30]=DEP;
			else if (str_scomp (var, "du32=on") == __TRUE)
				cfg.depUsers[31]=DEP;
			else if (str_scomp (var, "du33=on") == __TRUE)
				cfg.depUsers[32]=DEP;
			else if (str_scomp (var, "du34=on") == __TRUE)
				cfg.depUsers[33]=DEP;
			else if (str_scomp (var, "du35=on") == __TRUE)
				cfg.depUsers[34]=DEP;
			else if (str_scomp (var, "du36=on") == __TRUE)
				cfg.depUsers[35]=DEP;
			else if (str_scomp (var, "du37=on") == __TRUE)
				cfg.depUsers[36]=DEP;
			else if (str_scomp (var, "du38=on") == __TRUE)
				cfg.depUsers[37]=DEP;
			else if (str_scomp (var, "du39=on") == __TRUE)
				cfg.depUsers[38]=DEP;
			else if (str_scomp (var, "du40=on") == __TRUE)
				cfg.depUsers[39]=DEP;
			else if (str_scomp (var, "du41=on") == __TRUE)
				cfg.depUsers[40]=DEP;
			else if (str_scomp (var, "du42=on") == __TRUE)
				cfg.depUsers[41]=DEP;
			else if (str_scomp (var, "du43=on") == __TRUE)
				cfg.depUsers[42]=DEP;
			else if (str_scomp (var, "du44=on") == __TRUE)
				cfg.depUsers[43]=DEP;
			else if (str_scomp (var, "du45=on") == __TRUE)
				cfg.depUsers[44]=DEP;
			else if (str_scomp (var, "du46=on") == __TRUE)
				cfg.depUsers[45]=DEP;
			else if (str_scomp (var, "du47=on") == __TRUE)
				cfg.depUsers[46]=DEP;
			else if (str_scomp (var, "du48=on") == __TRUE)
				cfg.depUsers[47]=DEP;
			else if (str_scomp (var, "du49=on") == __TRUE)
				cfg.depUsers[48]=DEP;
			else if (str_scomp (var, "du50=on") == __TRUE)
				cfg.depUsers[49]=DEP;
			else if (str_scomp (var, "du51=on") == __TRUE)
				cfg.depUsers[50]=DEP;
			else if (str_scomp (var, "du52=on") == __TRUE)
				cfg.depUsers[51]=DEP;
			else if (str_scomp (var, "du53=on") == __TRUE)
				cfg.depUsers[52]=DEP;
			else if (str_scomp (var, "du54=on") == __TRUE)
				cfg.depUsers[53]=DEP;
			else if (str_scomp (var, "du55=on") == __TRUE)
				cfg.depUsers[54]=DEP;
			else if (str_scomp (var, "du56=on") == __TRUE)
				cfg.depUsers[55]=DEP;
			else if (str_scomp (var, "du57=on") == __TRUE)
				cfg.depUsers[56]=DEP;
			else if (str_scomp (var, "du58=on") == __TRUE)
				cfg.depUsers[57]=DEP;
			else if (str_scomp (var, "du59=on") == __TRUE)
				cfg.depUsers[58]=DEP;
			else if (str_scomp (var, "du60=on") == __TRUE)
				cfg.depUsers[59]=DEP;
			else if (str_scomp (var, "du61=on") == __TRUE)
				cfg.depUsers[60]=DEP;
			else if (str_scomp (var, "du62=on") == __TRUE)
				cfg.depUsers[61]=DEP;
			else if (str_scomp (var, "du63=on") == __TRUE)
				cfg.depUsers[62]=DEP;
			else if (str_scomp (var, "du64=on") == __TRUE)
				cfg.depUsers[63]=DEP;
			else if (str_scomp (var, "du65=on") == __TRUE)
				cfg.depUsers[64]=DEP;
			else if (str_scomp (var, "du66=on") == __TRUE)
				cfg.depUsers[65]=DEP;
			else if (str_scomp (var, "du67=on") == __TRUE)
				cfg.depUsers[66]=DEP;
			else if (str_scomp (var, "du68=on") == __TRUE)
				cfg.depUsers[67]=DEP;
			else if (str_scomp (var, "du69=on") == __TRUE)
				cfg.depUsers[68]=DEP;
			else if (str_scomp (var, "du70=on") == __TRUE)
				cfg.depUsers[69]=DEP;
			else if (str_scomp (var, "du71=on") == __TRUE)
				cfg.depUsers[70]=DEP;
			else if (str_scomp (var, "du72=on") == __TRUE)
				cfg.depUsers[71]=DEP;
			else if (str_scomp (var, "du73=on") == __TRUE)
				cfg.depUsers[72]=DEP;
			else if (str_scomp (var, "du74=on") == __TRUE)
				cfg.depUsers[73]=DEP;
			else if (str_scomp (var, "du75=on") == __TRUE)
				cfg.depUsers[74]=DEP;
			else if (str_scomp (var, "du76=on") == __TRUE)
				cfg.depUsers[75]=DEP;
			else if (str_scomp (var, "du77=on") == __TRUE)
				cfg.depUsers[76]=DEP;
			else if (str_scomp (var, "du78=on") == __TRUE)
				cfg.depUsers[77]=DEP;
			else if (str_scomp (var, "du79=on") == __TRUE)
				cfg.depUsers[78]=DEP;
			else if (str_scomp (var, "du80=on") == __TRUE)
				cfg.depUsers[79]=DEP;
			else if (str_scomp (var, "du81=on") == __TRUE)
				cfg.depUsers[80]=DEP;
			else if (str_scomp (var, "du82=on") == __TRUE)
				cfg.depUsers[81]=DEP;
			else if (str_scomp (var, "du83=on") == __TRUE)
				cfg.depUsers[82]=DEP;
			else if (str_scomp (var, "du84=on") == __TRUE)
				cfg.depUsers[83]=DEP;
			else if (str_scomp (var, "du85=on") == __TRUE)
				cfg.depUsers[84]=DEP;
			else if (str_scomp (var, "du86=on") == __TRUE)
				cfg.depUsers[85]=DEP;
			else if (str_scomp (var, "du87=on") == __TRUE)
				cfg.depUsers[86]=DEP;
			else if (str_scomp (var, "du88=on") == __TRUE)
				cfg.depUsers[87]=DEP;
			else if (str_scomp (var, "du89=on") == __TRUE)
				cfg.depUsers[88]=DEP;
			else if (str_scomp (var, "du90=on") == __TRUE)
				cfg.depUsers[89]=DEP;
			else if (str_scomp (var, "du91=on") == __TRUE)
				cfg.depUsers[90]=DEP;
			else if (str_scomp (var, "du92=on") == __TRUE)
				cfg.depUsers[91]=DEP;
			else if (str_scomp (var, "du93=on") == __TRUE)
				cfg.depUsers[92]=DEP;
			else if (str_scomp (var, "du94=on") == __TRUE)
				cfg.depUsers[93]=DEP;
			else if (str_scomp (var, "du95=on") == __TRUE)
				cfg.depUsers[94]=DEP;
			else if (str_scomp (var, "du96=on") == __TRUE)
				cfg.depUsers[95]=DEP;
			else if (str_scomp (var, "dpnl=") == __TRUE)
			{
            	sscanf ((const char *)&var[5], "%d.%d.%d.%d",&s[0],&s[1],&s[2],&s[3]);
            	cfg.depIpDisp[DEP-1][0] = s[0];
            	cfg.depIpDisp[DEP-1][1] = s[1];
            	cfg.depIpDisp[DEP-1][2] = s[2];
            	cfg.depIpDisp[DEP-1][3] = s[3];
			}
			else if (str_scomp (var, "itf1=") == __TRUE)
			{
				sscanf ((const char *)&var[5], "%d",&val);
				if (val==ITF_UDP)
					cfg.depItfDisp[DEP-1]=val;
			}
			else if (str_scomp (var, "dtop=") == __TRUE)
			{
            	sscanf ((const char *)&var[5], "%d.%d.%d.%d",&s[0],&s[1],&s[2],&s[3]);
            	cfg.depIpTop[DEP-1][0] = s[0];
            	cfg.depIpTop[DEP-1][1] = s[1];
            	cfg.depIpTop[DEP-1][2] = s[2];
            	cfg.depIpTop[DEP-1][3] = s[3];
			}
			else if (str_scomp (var, "itf2=") == __TRUE)
			{
				sscanf ((const char *)&var[5], "%d",&val);
				if (val==ITF_UDP)
					cfg.depItfTop[DEP-1]=val;
			}
			else if (str_scomp (var, "epq=on") == __TRUE) 		
				cfg.eTop[DEP-1]=1;
			else if (str_scomp (var, "dplin=") == __TRUE)
			{
				sscanf ((const char *)&var[6], "%d",&val);
				if (val>=1 && val<=10)
					cfg.depRowDisp[DEP-1]=val;
			}

			else if (str_scomp (var, "pg=ticket") == __TRUE)	// Página ticket.cgi
			{													//-------------------
				cfg.logoEnable=0;								// Zera variáveis para gravar 
				cfg.lettEnable=0;								// nova configuração
				cfg.lenNumTicket=0;
				cfg.typeNumTicket=0;
				cfg.cut=0;
			}															
			else if (str_scomp (var, "lg=on") == __TRUE)
				cfg.logoEnable=1;
			else if (str_scomp (var, "dlen=on") == __TRUE)
				cfg.lettEnable=1;
			else if (str_scomp (var, "dlnum=") == __TRUE)
				cfg.lenNumTicket = var[6] & 0x0F;
			else if (str_scomp (var, "dhnum=") == __TRUE)
				cfg.typeNumTicket = var[6] & 0x0F;
			else if (str_scomp (var, "cut=") == __TRUE)
				cfg.cut=var[4]&0x0F;
			else if (str_scomp (var, "h1op=") == __TRUE)
				cfg.optHead1=var[5]&0x0F;
			else if (str_scomp (var, "h2op=") == __TRUE)
				cfg.optHead2=var[5]&0x0F;
			else if (str_scomp (var, "h3op=") == __TRUE)
				cfg.optHead3=var[5]&0x0F;
			else if (str_scomp (var, "f1op=") == __TRUE)
				cfg.optFoot1=var[5]&0x0F;
			else if (str_scomp (var, "f2op=") == __TRUE)
				cfg.optFoot2=var[5]&0x0F;
			else if (str_scomp (var, "f3op=") == __TRUE)
				cfg.optFoot3=var[5]&0x0F;
			else if (str_scomp (var, "f4op=") == __TRUE)
				cfg.optFoot4=var[5]&0x0F;
			else if (str_scomp (var, "h1me=") == __TRUE)
				str_copy ((U8 *)cfg.header1, var+5);
			else if (str_scomp (var, "h2me=") == __TRUE)
				str_copy ((U8 *)cfg.header2, var+5);
			else if (str_scomp (var, "h3me=") == __TRUE)
				str_copy ((U8 *)cfg.header3, var+5);
			else if (str_scomp (var, "f1me=") == __TRUE)
				str_copy ((U8 *)cfg.footer1, var+5);
			else if (str_scomp (var, "f2me=") == __TRUE)
				str_copy ((U8 *)cfg.footer2, var+5);
			else if (str_scomp (var, "f3me=") == __TRUE)
				str_copy ((U8 *)cfg.footer3, var+5);
			else if (str_scomp (var, "f4me=") == __TRUE)
				str_copy ((U8 *)cfg.footer4, var+5);
			else if (str_scomp (var, "pg=system") == __TRUE)	// Página system.cgi
			{													//-------------------
				cfg.enTimeAuto=0;								// Zera variáveis para gravar 	
				cfg.weekDay=0;									// nova configuração
				cfg.saveReport=0;
				cfg.zero=0;
				cfg.broadcastInfo=0;
				cfg.ticketOrientation=0;
			}
			else if (str_scomp (var, "driv=2") == __TRUE)
			{
				if (_DEF_DRIVE!=_FLASHFS)
				{
					Change_Drive (_FLASHFS);
					STATFILE=2;
				}												
			}
			else if (str_scomp (var, "driv=4") == __TRUE)
			{
				if (_DEF_DRIVE!=_SDCARD)
				{
					Change_Drive (_SDCARD);
					STATFILE=2;
				}
			}
	        else if (str_scomp (var, "ajrc=") == __TRUE) 
			{
				doDateTime=1;
			}
			else if (str_scomp (var, "time=") == __TRUE) 
			{
				if (doDateTime)
				{
					time=Get_RTC();
	            	sscanf ((const char *)&var[5], "%d:%d:%d", &s[0],&s[1],&s[2]);
					if (s[0]<=23) time.HOUR = s[0];
					if (s[1]<=59) time.MINUTE = s[1];
					if (s[2]<=59) time.SECOND = s[2];
					Save_RTC(time);
				}
			}
	        else if (str_scomp (var, "date=") == __TRUE) 
			{
				if (doDateTime)
				{
					time=Get_RTC();
	            	sscanf ((const char *)&var[5], "%d/%d/%d", &s[0],&s[1],&s[2]);
					if (s[0]<=31) time.DAY = s[0];
					if (s[1]<=12) time.MONTH = s[1];
					time.YEAR = s[2];

					time.WEEKDAY = DayOfWeek(time);
					time.YEARDAY = DayOfYear(time);

					Save_RTC(time);
				}
			}
			else if (str_scomp (var, "test=") == __TRUE)
			{
				cfg.autoTest=var[5]&0x0F;
			}
			else if (str_scomp (var, "live=") == __TRUE)
			{
				sscanf ((const char *)&var[5], "%d",&val);
				if ((val>=0)&&(val<=18))
				{
					cfg.keepLiveTicket = val*3600;
				}
			}
			else if (str_scomp (var, "prot=") == __TRUE)
			{
				cfg.printProtocol=var[5]&0x0F;
			}
	        else if (str_scomp (var, "ptip=") == __TRUE)
			{
            	sscanf ((const char *)&var[5], "%d.%d.%d.%d", &s[0],&s[1],&s[2],&s[3]);
				cfg.ipSrvProt[0] = s[0];
				cfg.ipSrvProt[1] = s[1];
				cfg.ipSrvProt[2] = s[2];
				cfg.ipSrvProt[3] = s[3];
			}
			else if (str_scomp (var, "inve=") == __TRUE) 
			{
				cfg.ticketOrientation=1;
			}
			else if (str_scomp (var, "taut=on") == __TRUE)
				cfg.enTimeAuto=1;
	        else if (str_scomp (var, "tini=") == __TRUE) 
			{
	            sscanf ((const char *)&var[5], "%d:%d", &s[0],&s[1]);
				cfg.timeHourOn = s[0];
				cfg.timeMinOn = s[1];
			}
	        else if (str_scomp (var, "tend=") == __TRUE) 
			{
	            sscanf ((const char *)&var[5], "%d:%d", &s[0],&s[1]);
				cfg.timeHourOff = s[0];
				cfg.timeMinOff = s[1];
			}
	        else if (str_scomp (var, "trep=") == __TRUE) 
			{
	            sscanf ((const char *)&var[5], "%d:%d", &s[0],&s[1]);
				cfg.timeHourRep = s[0];
				cfg.timeMinRep = s[1];
			}
			else if (str_scomp (var, "erep=on") == __TRUE)
				cfg.saveReport=1;
			else if (str_scomp (var, "zero=on") == __TRUE)
				cfg.zero=1;
			else if (str_scomp (var, "d1=on") == __TRUE)
				cfg.weekDay|=1;
			else if (str_scomp (var, "d2=on") == __TRUE)
				cfg.weekDay|=2;
			else if (str_scomp (var, "d3=on") == __TRUE)
				cfg.weekDay|=4;
			else if (str_scomp (var, "d4=on") == __TRUE)
				cfg.weekDay|=8;
			else if (str_scomp (var, "d5=on") == __TRUE)
				cfg.weekDay|=16;
			else if (str_scomp (var, "d6=on") == __TRUE)
				cfg.weekDay|=32;
			else if (str_scomp (var, "d7=on") == __TRUE)
				cfg.weekDay|=64;
			else if (str_scomp (var, "mode=") == __TRUE)
			{
       			cfg.runMode=var[5]&0x0F;
				if (cfg.runMode==_MODESERVER)
				{
					cfg.ipServer[0] = 0;
					cfg.ipServer[1] = 0;
					cfg.ipServer[2] = 0;
					cfg.ipServer[3] = 0;
				}
			}
	        else if (str_scomp (var, "svip=") == __TRUE)
			{
				if (cfg.runMode==_MODECLIENT)
				{
	            	sscanf ((const char *)&var[5], "%d.%d.%d.%d", &s[0],&s[1],&s[2],&s[3]);
					cfg.ipServer[0] = s[0];
					cfg.ipServer[1] = s[1];
					cfg.ipServer[2] = s[2];
					cfg.ipServer[3] = s[3];
				}
			}
			else if (str_scomp (var, "info=on") == __TRUE)
			{
				cfg.broadcastInfo=1;
			}
			else if (str_scomp (var, "sale=") == __TRUE) 
			{
	            sscanf ((const char *)&var[5], "%d", &s[0]);
				cfg.salesDpto = s[0];
			}
			else if (str_scomp (var, "itfs=") == __TRUE)
			{
				sscanf ((const char *)&var[5], "%d",&val);
				if (val==ITF_UDP)
					cfg.srvItf=val;
			}
			else if (str_scomp (var, "unum=") == __TRUE)		// Página userbox.cgi
			{													//-------------------
				sscanf ((const char *)&var[5], "%d",&val);
				USER=val;
			}
			else if (str_scomp (var, "ubox=") == __TRUE)		// Página userbox.cgi
			{													//-------------------
				sscanf ((const char *)&var[5], "%d",&val);
				cfg.userBox[USER-1]=val;
			}
			else if (str_scomp (var, "pg=callnext") == __TRUE)	// Página callnext.cgi
			{													//---------------------
				httpNumber=0;
				httpAlpha=' ';
				httpRow=1;
			}
			else if (str_scomp (var, "pg=callticket") == __TRUE)// Página callticket.cgi
			{													//----------------------
			}
			else if (str_scomp (var, "callf=") == __TRUE)
			{
				if (WEBREQUEST!=NEW_CALL)
				{
					sscanf ((const char *)&var[6], "%d",&val);
					httpFunc=val;
					doCall=1;
				}
			}			
			else if (str_scomp (var, "calll=") == __TRUE)	 	
			{													
				if (WEBREQUEST!=NEW_CALL)
				{
					httpAlpha=var[6];
					doCall=1;
				}
			}
			else if (str_scomp (var, "calls=") == __TRUE) 		
			{							
				if (WEBREQUEST!=NEW_CALL)
				{						
					sscanf ((const char *)&var[6], "%d",&val);
					httpNumber=val;
					doCall=1;
				}
         	}
			else if (str_scomp (var, "callm=") == __TRUE)
			{
				if (WEBREQUEST!=NEW_CALL)
				{
					sscanf ((const char *)&var[6], "%d",&val);
					httpUser=val;
					doCall=1;
				}
			}
			else if (str_scomp (var, "callr=") == __TRUE)
			{
				if (WEBREQUEST!=NEW_CALL)
				{
					sscanf ((const char *)&var[6], "%d",&val);
					httpRow=val;
					doCall=1;
				}
			}
			else if (str_scomp (var, "calld=") == __TRUE)
			{
				if (WEBREQUEST!=NEW_CALL)
				{
					sscanf ((const char *)&var[6], "%d",&val);
					httpDep=val;
					doCall=1;
				}
			}
    	}
  	}while (dat);

	free_mem ((OS_FRAME *)var);
 
  	if (doFormat) 											// Formata drive se solicitado
		Format_Drive ();
	
	if (doIniCfg)											// Formata Drive e recupera configuração
	{
		InitAppConfig ();									// Sobregrava parametros de fabrica
		InitClient ();										// Sobregrava parametros do cliente
	}

	if (doIniTkt)											// Inicializa numeração de tickets
		Format_DataFlash ();

	if (doNetwork)
		WEBREQUEST=UPD_NETWORK;

	if (doCall)
		WEBREQUEST=NEW_CALL;								// Indica que chegou nova chamada http

  	if (stpassw == 0x03) 
	{
    	len = strlen ((const char *)passw);
    	if (mem_comp (passw, retyped, len) == __TRUE) 
		{
      		str_copy (http_auth_passw, passw);
			str_copy (cfg.authPassword, passw);
    	}
  	}

	Write_Flash ();
}

/*--------------------------- cgi_func --------------------------------------*/

U16 cgi_func (U8 *env, U8 *buf, U16 buflen, U32 *pcgi) 
{
  	/* This function is called by HTTP server script interpreter to make a    */
  	/* formated output for 'stdout'. It returns the number of bytes written   */
  	/* to the output buffer. Hi-bit of return value (len is or-ed with 0x8000)*/
  	/* is a repeat flag for the system script interpreter. If this bit is set */
  	/* to 1, the system will call the 'cgi_func()' again for the same script  */
  	/* line with parameter 'pcgi' pointing to a 4-byte buffer. This buffer    */
  	/* can be used for storing different status variables for this function.  */
  	/* It is set to 0 by HTTP Server on first call and is not altered by      */
  	/* HTTP server for repeated calls. This function should NEVER write more  */
  	/* than 'buflen' bytes to the buffer.                                     */
  	/* Parameters:                                                            */
  	/*   env    - environment variable string                                 */
  	/*   buf    - HTTP transmit buffer                                        */
  	/*   buflen - length of this buffer (500-1400 bytes - depends on MSS)     */
  	/*   pcgi   - pointer to session local buffer used for repeated loops     */
  	/*            This is a U32 variable - size is 4 bytes. Value is:         */
  	/*            - on 1st call = 0                                           */
  	/*            - 2nd call    = as set by this function on first call       */
  	
  	static FINFO info;										// only for FILE
  	char temp[32]={0,0,0,0,0,0,0,0,0,0,0,0,0};  			// only for FILE
	char EXTENSION [8];
	char dirbase [5];
	TIME_TYPE time;
	static U64 fsize=0;

	TCP_INFO *tsoc;
  	U32 len = 0;

  	switch (env[0]) 
	{
    	/* Analyze the environment string. It is the script 'c' line starting */
    	/* at position 2. What you write to the script file is returned here. */
    	
		case 'a' :
      		/* Parâmetros gerais - file 'system.cgi' e 'design.cgi' */
      		switch (env[2]) 
			{
				case 'A':
               		len = sprintf((char *)buf,(const char *)&env[4],_DEF_DRIVE==_FLASHFS?"Checked":"");
               		break;
				case 'B':														 
               		len = sprintf((char *)buf,(const char *)&env[4],_DEF_DRIVE==_SDCARD?"Checked":"");
               		break;
				case 'C':
					// Você pode visualizar o horário UTC mudando o type de "hidden" p/ "text"
					// na página "system.cgi" nos elementos de id=jtime e id=jdate
					// Esta funções lêem do RTC e convertem para UTC e depois convertem este
					// UTC para RTC novamente, testando as duas rotinas de conversão.
					time=UTC_To_RTC(1970,RTC_To_UTC(1970));
               		len = sprintf((char *)buf,(const char *)&env[4],time.HOUR,
                             										time.MINUTE,
																	time.SECOND);
               		break;
				case 'D':
					// Você pode visualizar o horário UTC mudando o type de "hidden" p/ "text"
					// na página "system.cgi" nos elementos de id=jtime e id=jdate
					// Esta funções lêem do RTC e convertem para UTC e depois convertem este
					// UTC para RTC novamente, testando as duas rotinas de conversão.
					time=UTC_To_RTC(1970,RTC_To_UTC(1970));
               		len = sprintf((char *)buf,(const char *)&env[4],time.DAY,
                             										time.MONTH,
																	time.YEAR);
					break;
				case 'E':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.autoTest==0?"Checked":"");
               		break;
				case 'F':														 
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.autoTest==1?"Checked":"");
               		break;
				case 'G':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.keepLiveTicket/3600);
					break;
				case 'H':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.printProtocol==0?"Checked":"");
               		break;
				case 'I':														 
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.printProtocol==1?"Checked":"");
               		break;
				case 'J':
					len = sprintf((char *)buf,(const char *)&env[4],cfg.ipSrvProt[0],
																	cfg.ipSrvProt[1],
																	cfg.ipSrvProt[2],
																	cfg.ipSrvProt[3]);
					break;
				case 'K':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.ticketOrientation==1?"Checked":"");
               		break;
				case 'L':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.enTimeAuto==1?"checked":"");
					break;
				case 'M':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.timeHourOn,
                             										cfg.timeMinOn);
               		break;
				case 'N':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.timeHourOff,
                             										cfg.timeMinOff);
               		break;
				case '1':
					len = sprintf((char *)buf,(const char *)&env[4],"Checked");
					break;
				case 'O':
               		len = sprintf((char *)buf,(const char *)&env[4],(cfg.weekDay&1)==1?"checked":"");
					break;
				case 'P':
               		len = sprintf((char *)buf,(const char *)&env[4],(cfg.weekDay&2)==2?"checked":"",
																	(cfg.weekDay&4)==4?"checked":"");
					break;
				case 'Q':
               		len = sprintf((char *)buf,(const char *)&env[4],(cfg.weekDay&8)==8?"checked":"",
																	(cfg.weekDay&16)==16?"checked":"");
					break;
				case 'R':
               		len = sprintf((char *)buf,(const char *)&env[4],(cfg.weekDay&32)==32?"checked":"",
																	(cfg.weekDay&64)==64?"checked":"");
					break;
				case 'S':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.timeHourRep,
                             										cfg.timeMinRep);
               		break;
				case 'T':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.saveReport==1?"checked":"");
					break;
				case 'U':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.zero==1?"checked":"");
					break;
				case 'V':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.runMode==_MODESERVER?"Checked":"");
               		break;
				case 'W':														 
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.runMode==_MODECLIENT?"Checked":"");
               		break;
				case 'X':
					len = sprintf((char *)buf,(const char *)&env[4],cfg.ipServer[0],
																	cfg.ipServer[1],
																	cfg.ipServer[2],
																	cfg.ipServer[3]);
					break;
				case 'Y':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.broadcastInfo==1?"Checked":"");
               		break;
				case 'Z':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.salesDpto);
               		break;
			}
      		break;

    	case 'b':

        	/* Parâmetros do departamento - arquivos 'seldep.cgi' e 'dep.cgi' */
        	switch (env[2]) 
			{
				case '0':
               		len = sprintf((char *)buf,(const char *)&env[4],DEP==1?"Selected":"",
																	cfg.depName[0],
																	DEP==2?"Selected":"",
																	cfg.depName[1],
																	DEP==3?"Selected":"",
																	cfg.depName[2]);
					break;
				case '1':
               		len = sprintf((char *)buf,(const char *)&env[4],DEP==4?"Selected":"",
																	cfg.depName[3],
																	DEP==5?"Selected":"",
																	cfg.depName[4],
																	DEP==6?"Selected":"",
																	cfg.depName[5]);
					break;
				case '2':
               		len = sprintf((char *)buf,(const char *)&env[4],DEP==7?"Selected":"",
																	cfg.depName[6],
																	DEP==8?"Selected":"",
																	cfg.depName[7],
																	DEP==9?"Selected":"",
																	cfg.depName[8]);
					break;
				case '3':
               		len = sprintf((char *)buf,(const char *)&env[4],DEP==10?"Selected":"",
																	cfg.depName[9],
																	DEP==11?"Selected":"",
																	cfg.depName[10],
																	DEP==12?"Selected":"",
																	cfg.depName[11]);
					break;
				case '\'':
               		len = sprintf((char *)buf,(const char *)&env[4],DEP==13?"Selected":"",
																	cfg.depName[12]);
					break;
				case '4':
					len = sprintf((char *)buf,(const char *)&env[4],DEP);
            		break;
            	case '5':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depEnable[DEP-1]==1?"checked":"");
            		break;
            	case '6':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depName[DEP-1]);
            		break;
         		case '7':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depAlpha[DEP-1]==' '?"Selected":"",
																	cfg.depAlpha[DEP-1]=='A'?"Selected":"",
																	cfg.depAlpha[DEP-1]=='B'?"Selected":"",
																	cfg.depAlpha[DEP-1]=='C'?"Selected":"");
            		break;
         		case '8':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depAlpha[DEP-1]=='D'?"Selected":"",
																	cfg.depAlpha[DEP-1]=='E'?"Selected":"",
																	cfg.depAlpha[DEP-1]=='F'?"Selected":"",
																	cfg.depAlpha[DEP-1]=='G'?"Selected":"",
																	cfg.depAlpha[DEP-1]=='H'?"Selected":"");
            		break;
         		case '9':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depAlpha[DEP-1]=='I'?"Selected":"",
																	cfg.depAlpha[DEP-1]=='J'?"Selected":"",
																	cfg.depAlpha[DEP-1]=='K'?"Selected":"",
																	cfg.depAlpha[DEP-1]=='L'?"Selected":"",
																	cfg.depAlpha[DEP-1]=='M'?"Selected":"");
            		break;
         		case ':':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depAlpha[DEP-1]=='N'?"Selected":"",
																	cfg.depAlpha[DEP-1]=='O'?"Selected":"",
																	cfg.depAlpha[DEP-1]=='P'?"Selected":"",
																	cfg.depAlpha[DEP-1]=='Q'?"Selected":"",
																	cfg.depAlpha[DEP-1]=='R'?"Selected":"");
            		break;
         		case ';':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depAlpha[DEP-1]=='S'?"Selected":"",
																	cfg.depAlpha[DEP-1]=='T'?"Selected":"",
																	cfg.depAlpha[DEP-1]=='U'?"Selected":"",
																	cfg.depAlpha[DEP-1]=='V'?"Selected":"",
																	cfg.depAlpha[DEP-1]=='W'?"Selected":"");
            		break;
         		case '<':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depAlpha[DEP-1]=='X'?"Selected":"",
																	cfg.depAlpha[DEP-1]=='Y'?"Selected":"",
																	cfg.depAlpha[DEP-1]=='Z'?"Selected":"");
            		break;
         		case '=':
            		len = sprintf((char *)buf,(const char *)&env[4],cfg.depRange[DEP-1][0]);
            		break;
         		case '>':
            		len = sprintf((char *)buf,(const char *)&env[4],cfg.depRange[DEP-1][1]);
            		break;
				case '&':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depCopy[DEP-1]==1?"Selected":"",
																	cfg.depCopy[DEP-1]==2?"Selected":"");
					break;
         		case '´':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depPref[DEP-1]==1?"checked":"");
            		break;
				case '?':
					len = sprintf((char *)buf,(const char *)&env[4],cfg.depCloseNext[DEP-1]==1?"checked":"");	
					break;
				case '@':
					len = sprintf((char *)buf,(const char *)&env[4],cfg.depCloseRep[DEP-1]==1?"checked":"");	
					break;
         		case 'A':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depSchPri[DEP-1]==0xA1?"Selected":"",
																	cfg.depSchPri[DEP-1]==0x91?"Selected":"",
																	cfg.depSchPri[DEP-1]==0x81?"Selected":"");
            		break;
         		case 'B':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depSchPri[DEP-1]==0x71?"Selected":"",
																	cfg.depSchPri[DEP-1]==0x61?"Selected":"",
																	cfg.depSchPri[DEP-1]==0x51?"Selected":"");
            		break;
         		case 'C':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depSchPri[DEP-1]==0x41?"Selected":"",
																	cfg.depSchPri[DEP-1]==0x31?"Selected":"",
																	cfg.depSchPri[DEP-1]==0x21?"Selected":"");
            		break;
         		case 'D':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depSchPri[DEP-1]==0x11?"Selected":"",
																	cfg.depSchPri[DEP-1]==0x12?"Selected":"",
																	cfg.depSchPri[DEP-1]==0x13?"Selected":"");
            		break;
         		case 'E':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depSchPri[DEP-1]==0x14?"Selected":"",
																	cfg.depSchPri[DEP-1]==0x15?"Selected":"",
																	cfg.depSchPri[DEP-1]==0x16?"Selected":"");
            		break;
         		case 'F':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depSchPri[DEP-1]==0x17?"Selected":"",
																	cfg.depSchPri[DEP-1]==0x18?"Selected":"",
																	cfg.depSchPri[DEP-1]==0x19?"Selected":"");
            		break;
         		case 'G':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depSchPri[DEP-1]==0x1A?"Selected":"");
            		break;
         		case 'H':
               		len = sprintf((char *)buf,(const char *)&env[4],((cfg.depHelp[DEP-1]&0xF0)>>4)==0?"Selected":"",
																	((cfg.depHelp[DEP-1]&0xF0)>>4)==1?"Selected":"",
																	((cfg.depHelp[DEP-1]&0xF0)>>4)==2?"Selected":"",
																	((cfg.depHelp[DEP-1]&0xF0)>>4)==3?"Selected":"");
            		break;
         		case 'I':
               		len = sprintf((char *)buf,(const char *)&env[4],((cfg.depHelp[DEP-1]&0xF0)>>4)==4?"Selected":"",
																	((cfg.depHelp[DEP-1]&0xF0)>>4)==5?"Selected":"",
																	((cfg.depHelp[DEP-1]&0xF0)>>4)==6?"Selected":"",
																	((cfg.depHelp[DEP-1]&0xF0)>>4)==7?"Selected":"",
																	((cfg.depHelp[DEP-1]&0xF0)>>4)==8?"Selected":"");
            		break;
         		case 'J':
               		len = sprintf((char *)buf,(const char *)&env[4],((cfg.depHelp[DEP-1]&0xF0)>>4)==9?"Selected":"",
																	((cfg.depHelp[DEP-1]&0xF0)>>4)==10?"Selected":"",
																	((cfg.depHelp[DEP-1]&0xF0)>>4)==11?"Selected":"",
																	((cfg.depHelp[DEP-1]&0xF0)>>4)==12?"Selected":"");
            		break;
         		case 'K':
               		len = sprintf((char *)buf,(const char *)&env[4],(cfg.depHelp[DEP-1]&0x0F)==0?"Selected":"",
																	(cfg.depHelp[DEP-1]&0x0F)==1?"Selected":"",
																	(cfg.depHelp[DEP-1]&0x0F)==2?"Selected":"",
																	(cfg.depHelp[DEP-1]&0x0F)==3?"Selected":"");
            		break;
         		case 'L':
               		len = sprintf((char *)buf,(const char *)&env[4],(cfg.depHelp[DEP-1]&0x0F)==4?"Selected":"",
																	(cfg.depHelp[DEP-1]&0x0F)==5?"Selected":"",
																	(cfg.depHelp[DEP-1]&0x0F)==6?"Selected":"",
																	(cfg.depHelp[DEP-1]&0x0F)==7?"Selected":"",
																	(cfg.depHelp[DEP-1]&0x0F)==8?"Selected":"");
            		break;
         		case 'M':
               		len = sprintf((char *)buf,(const char *)&env[4],(cfg.depHelp[DEP-1]&0x0F)==9?"Selected":"",
																	(cfg.depHelp[DEP-1]&0x0F)==10?"Selected":"",
																	(cfg.depHelp[DEP-1]&0x0F)==11?"Selected":"",
																	(cfg.depHelp[DEP-1]&0x0F)==12?"Selected":"");
            		break;
         		case 'N':
					len = sprintf((char *)buf,(const char *)&env[4],(cfg.depOptionHlp[DEP-1]&0xF0)==0x10?"checked":"");
            		break;
         		case 'O':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depSchHlp[DEP-1]==0xA1?"Selected":"",
																	cfg.depSchHlp[DEP-1]==0x91?"Selected":"",
																	cfg.depSchHlp[DEP-1]==0x81?"Selected":"");
            		break;
         		case 'P':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depSchHlp[DEP-1]==0x71?"Selected":"",
																	cfg.depSchHlp[DEP-1]==0x61?"Selected":"",
																	cfg.depSchHlp[DEP-1]==0x51?"Selected":"");
            		break;
         		case 'Q':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depSchHlp[DEP-1]==0x41?"Selected":"",
																	cfg.depSchHlp[DEP-1]==0x31?"Selected":"",
																	cfg.depSchHlp[DEP-1]==0x21?"Selected":"");
            		break;
         		case 'R':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depSchHlp[DEP-1]==0x11?"Selected":"",
																	cfg.depSchHlp[DEP-1]==0x12?"Selected":"",
																	cfg.depSchHlp[DEP-1]==0x13?"Selected":"");
            		break;
         		case 'S':
					len = sprintf((char *)buf,(const char *)&env[4],cfg.depSchHlp[DEP-1]==0x14?"Selected":"",
																	cfg.depSchHlp[DEP-1]==0x15?"Selected":"",
																	cfg.depSchHlp[DEP-1]==0x16?"Selected":"");
            		break;
         		case 'T':
					len = sprintf((char *)buf,(const char *)&env[4],cfg.depSchHlp[DEP-1]==0x17?"Selected":"",
																	cfg.depSchHlp[DEP-1]==0x18?"Selected":"",
																	cfg.depSchHlp[DEP-1]==0x19?"Selected":"");
            		break;
         		case 'U':
					len = sprintf((char *)buf,(const char *)&env[4],cfg.depSchHlp[DEP-1]==0x1A?"Selected":"");
            		break;
         		case 'V':
					len = sprintf((char *)buf,(const char *)&env[4],(cfg.depOptionHlp[DEP-1]&0x0F)==0x01?"checked":"");
            		break;
         		case 'W':
					len = sprintf((char *)buf,(const char *)&env[4],cfg.checkTime[DEP-1]==1?"checked":"");
            		break;
         		case 'X':
					len = sprintf((char *)buf,(const char *)&env[4],cfg.optSchHlp[DEP-1]==1?"checked":"");
            		break;
         		case 'Y':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[0]==DEP?"checked":"",
                                                        			cfg.depUsers[1]==DEP?"checked":"",
																	cfg.depUsers[2]==DEP?"checked":"");
            		break;
         		case 'Z':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[3]==DEP?"checked":"",
                                                        			cfg.depUsers[4]==DEP?"checked":"",
																	cfg.depUsers[5]==DEP?"checked":"");
            		break;
         		case '[':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[6]==DEP?"checked":"",
                                                        			cfg.depUsers[7]==DEP?"checked":"",
																	cfg.depUsers[8]==DEP?"checked":"");
					break;

         		case '\\':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[9]==DEP?"checked":"",
                                                        			cfg.depUsers[10]==DEP?"checked":"",
																	cfg.depUsers[11]==DEP?"checked":"");
					break;
         		case ']':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[12]==DEP?"checked":"",
                                                        			cfg.depUsers[13]==DEP?"checked":"",
																	cfg.depUsers[14]==DEP?"checked":"");
					break;
         		case '^':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[15]==DEP?"checked":"",
                                                        			cfg.depUsers[16]==DEP?"checked":"",
																	cfg.depUsers[17]==DEP?"checked":"");
					break;
         		case '_':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[18]==DEP?"checked":"",
                                                        			cfg.depUsers[19]==DEP?"checked":"",
																	cfg.depUsers[20]==DEP?"checked":"");
					break;
         		case '`':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[21]==DEP?"checked":"",
                                                        			cfg.depUsers[22]==DEP?"checked":"",
																	cfg.depUsers[23]==DEP?"checked":"");
					break;
         		case 'a':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[24]==DEP?"checked":"",
                                                        			cfg.depUsers[25]==DEP?"checked":"",
																	cfg.depUsers[26]==DEP?"checked":"");
					break;
         		case 'b':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[27]==DEP?"checked":"",
                                                        			cfg.depUsers[28]==DEP?"checked":"",
																	cfg.depUsers[29]==DEP?"checked":"");
					break;
         		case 'c':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[30]==DEP?"checked":"",
                                                        			cfg.depUsers[31]==DEP?"checked":"",
																	cfg.depUsers[32]==DEP?"checked":"");
					break;
         		case 'd':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[33]==DEP?"checked":"",
                                                        			cfg.depUsers[34]==DEP?"checked":"",
																	cfg.depUsers[35]==DEP?"checked":"");
					break;
         		case 'e':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[36]==DEP?"checked":"",
                                                        			cfg.depUsers[37]==DEP?"checked":"",
																	cfg.depUsers[38]==DEP?"checked":"");
					break;
         		case 'f':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[39]==DEP?"checked":"",
                                                        			cfg.depUsers[40]==DEP?"checked":"",
																	cfg.depUsers[41]==DEP?"checked":"");
					break;
         		case 'g':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[42]==DEP?"checked":"",
                                                        			cfg.depUsers[43]==DEP?"checked":"",
																	cfg.depUsers[44]==DEP?"checked":"");
					break;
         		case 'h':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[45]==DEP?"checked":"",
                                                        			cfg.depUsers[46]==DEP?"checked":"",
																	cfg.depUsers[47]==DEP?"checked":"");
					break;
         		case 'i':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[48]==DEP?"checked":"",
                                                        			cfg.depUsers[49]==DEP?"checked":"",
																	cfg.depUsers[50]==DEP?"checked":"");
					break;
         		case 'j':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[51]==DEP?"checked":"",
                                                        			cfg.depUsers[52]==DEP?"checked":"",
																	cfg.depUsers[53]==DEP?"checked":"");
					break;
         		case 'k':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[54]==DEP?"checked":"",
                                                        			cfg.depUsers[55]==DEP?"checked":"",
																	cfg.depUsers[56]==DEP?"checked":"");
					break;
         		case 'l':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[57]==DEP?"checked":"",
                                                        			cfg.depUsers[58]==DEP?"checked":"",
																	cfg.depUsers[59]==DEP?"checked":"");
					break;
         		case 'm':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[60]==DEP?"checked":"",
                                                        			cfg.depUsers[61]==DEP?"checked":"",
																	cfg.depUsers[62]==DEP?"checked":"");
					break;
         		case 'n':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[63]==DEP?"checked":"",
                                                        			cfg.depUsers[64]==DEP?"checked":"",
																	cfg.depUsers[65]==DEP?"checked":"");
					break;
         		case 'o':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[66]==DEP?"checked":"",
                                                        			cfg.depUsers[67]==DEP?"checked":"",
																	cfg.depUsers[68]==DEP?"checked":"");
					break;
         		case 'p':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[69]==DEP?"checked":"",
                                                        			cfg.depUsers[70]==DEP?"checked":"",
																	cfg.depUsers[71]==DEP?"checked":"");
					break;
         		case 'q':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[72]==DEP?"checked":"",
                                                        			cfg.depUsers[73]==DEP?"checked":"",
																	cfg.depUsers[74]==DEP?"checked":"");
					break;
         		case 'r':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[75]==DEP?"checked":"",
                                                        			cfg.depUsers[76]==DEP?"checked":"",
																	cfg.depUsers[77]==DEP?"checked":"");
					break;
         		case 's':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[78]==DEP?"checked":"",
                                                        			cfg.depUsers[79]==DEP?"checked":"",
																	cfg.depUsers[80]==DEP?"checked":"");
					break;
         		case 't':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[81]==DEP?"checked":"",
                                                        			cfg.depUsers[82]==DEP?"checked":"",
																	cfg.depUsers[83]==DEP?"checked":"");
					break;
         		case 'u':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[84]==DEP?"checked":"",
                                                        			cfg.depUsers[85]==DEP?"checked":"",
																	cfg.depUsers[86]==DEP?"checked":"");
					break;
         		case 'v':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[87]==DEP?"checked":"",
                                                        			cfg.depUsers[88]==DEP?"checked":"",
																	cfg.depUsers[89]==DEP?"checked":"");
					break;
         		case 'w':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[90]==DEP?"checked":"",
                                                        			cfg.depUsers[91]==DEP?"checked":"",
																	cfg.depUsers[92]==DEP?"checked":"");
					break;
         		case 'x':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.depUsers[93]==DEP?"checked":"",
                                                        			cfg.depUsers[94]==DEP?"checked":"",
																	cfg.depUsers[95]==DEP?"checked":"");
					break;
				case 'y':
					len = sprintf((char *)buf,(const char *)&env[4],cfg.depIpDisp[DEP-1][0],
																	cfg.depIpDisp[DEP-1][1],
																	cfg.depIpDisp[DEP-1][2],
																	cfg.depIpDisp[DEP-1][3]);
					break;
				case '{':
					len = sprintf((char *)buf,(const char *)&env[4],cfg.depIpTop[DEP-1][0],
																	cfg.depIpTop[DEP-1][1],
																	cfg.depIpTop[DEP-1][2],
																	cfg.depIpTop[DEP-1][3]);
					break;
            	case '}':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.eTop[DEP-1]==1?"checked":"");
            		break;
				case '~':
					len = sprintf((char *)buf,(const char *)&env[4],cfg.depRowDisp[DEP-1]==1?"Selected":"",
																	cfg.depRowDisp[DEP-1]==2?"Selected":"",
																	cfg.depRowDisp[DEP-1]==3?"Selected":"",
																	cfg.depRowDisp[DEP-1]==4?"Selected":"",
																	cfg.depRowDisp[DEP-1]==5?"Selected":"");
					break;
				case '!':
					len = sprintf((char *)buf,(const char *)&env[4],cfg.depRowDisp[DEP-1]==6?"Selected":"",
																	cfg.depRowDisp[DEP-1]==7?"Selected":"",
																	cfg.depRowDisp[DEP-1]==8?"Selected":"",
																	cfg.depRowDisp[DEP-1]==9?"Selected":"",
																	cfg.depRowDisp[DEP-1]==10?"Selected":"");
					break;
         		case '%':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.runMode);
            		break;
			}
			break;

    	case 'c':
        	/* Parâmetros do ticket - arquivo 'ticket.cgi' */
        	switch (env[2]) 
			{
				case 'A':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.logoEnable==1?"checked":"");
					break;
         		case 'B':
					len = sprintf((char *)buf,(const char *)&env[4],cfg.lettEnable==1?"checked":"");
            		break;
         		case 'C':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.lenNumTicket==1?"Selected":"",
																	cfg.lenNumTicket==2?"Selected":"");
            		break;
         		case 'D':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.lenNumTicket==3?"Selected":"",
																	cfg.lenNumTicket==4?"Selected":"");
            		break;
         		case 'E':
            		len = sprintf((char *)buf,(const char *)&env[4],cfg.typeNumTicket==0?"Selected":"",
																	cfg.typeNumTicket==1?"Selected":"");
					break;
         		case 'F':
            		len = sprintf((char *)buf,(const char *)&env[4],cfg.typeNumTicket==2?"Selected":"",
																	cfg.typeNumTicket==3?"Selected":"");
					break;
         		case 'G':
            		len = sprintf((char *)buf,(const char *)&env[4],cfg.typeNumTicket==4?"Selected":"",
																	cfg.typeNumTicket==5?"Selected":"");
					break;
				case 'H':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.cut==0?"Selected":"",
																	cfg.cut==1?"Selected":"");
					break;
				case 'J':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.optHead1==0?"Selected":"",
																	cfg.optHead1==1?"Selected":"");
					break;
				case 'K':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.optHead1==2?"Selected":"",
																	cfg.optHead1==3?"Selected":"");
					break;
				case 'L':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.optHead1==4?"Selected":"",
																	cfg.optHead1==5?"Selected":"");
					break;
				case 'M':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.header1);
					break;
				case 'N':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.optHead2==0?"Selected":"",
																	cfg.optHead2==1?"Selected":"");
					break;
				case 'O':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.optHead2==2?"Selected":"",
																	cfg.optHead2==3?"Selected":"");
					break;
				case 'P':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.optHead2==4?"Selected":"",
																	cfg.optHead2==5?"Selected":"");
					break;
				case 'Q':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.header2);
					break;
				case 'R':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.optHead3==0?"Selected":"",
																	cfg.optHead3==1?"Selected":"");
					break;
				case 'S':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.optHead3==2?"Selected":"",
																	cfg.optHead3==3?"Selected":"");
					break;
				case 'T':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.optHead3==4?"Selected":"",
																	cfg.optHead3==5?"Selected":"");
					break;
				case 'U':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.header3);
					break;
				case 'V':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.optFoot1==0?"Selected":"",
																	cfg.optFoot1==1?"Selected":"");
					break;
				case 'W':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.optFoot1==2?"Selected":"",
																	cfg.optFoot1==3?"Selected":"");
					break;
				case 'X':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.optFoot1==4?"Selected":"",
																	cfg.optFoot1==5?"Selected":"");
					break;
				case 'Y':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.footer1);
					break;
				case 'Z':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.optFoot2==0?"Selected":"",
																	cfg.optFoot2==1?"Selected":"");
					break;
				case 'a':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.optFoot2==2?"Selected":"",
																	cfg.optFoot2==3?"Selected":"");
					break;
				case 'b':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.optFoot2==4?"Selected":"",
																	cfg.optFoot2==5?"Selected":"");
					break;
				case 'c':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.footer2);
					break;
				case 'd':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.optFoot3==0?"Selected":"",
																	cfg.optFoot3==1?"Selected":"");
					break;
				case 'e':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.optFoot3==2?"Selected":"",
																	cfg.optFoot3==3?"Selected":"");
					break;
				case 'f':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.optFoot3==4?"Selected":"",
																	cfg.optFoot3==5?"Selected":"");
					break;
				case 'g':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.footer3);
					break;
				case 'h':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.optFoot4==0?"Selected":"",
																	cfg.optFoot4==1?"Selected":"");
					break;
				case 'i':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.optFoot4==2?"Selected":"",
																	cfg.optFoot4==3?"Selected":"");
					break;
				case 'j':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.optFoot4==4?"Selected":"",
																	cfg.optFoot4==5?"Selected":"");
					break;
				case 'k':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.footer4);
					break;
			}
			break;

    	case 'd':
			/* Parâmetros do sistema - arquivo 'ver.cgi' */
      		switch (env[2]) 
			{
	        	case '0':
          			len = sprintf((char *)buf,(const char *)&env[4],versionFirm);
          			break;

	        	case '1':
          			len = sprintf((char *)buf,(const char *)&env[4],http_EnAuth==0?"Selected":"",
                                                        			http_EnAuth==1?"Selected":"");
          			break;
	        	
				case '2':
	          		len = sprintf((char *)buf,(const char *)&env[4],http_auth_passw);
          			break;

				case '3':
					len = sprintf((char *)buf,(const char *)&env[4],Show_DevModel(cfg.devModel));					
					break;
      		}
      		break;
	
		case 'e' :
        	/* Parâmetros de rede - arquivo 'network.cgi' */
        	switch (env[2]) 
			{
            	case 'A':
					len = sprintf((char *)buf,(const char *)&env[4],cfg.hostName);
					break;
				case 'B':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.dhcpStatus==0?"Selected":"",
                                                        			cfg.dhcpStatus==1?"Selected":"");
               		break;
				case 'C':
               		len = sprintf((char *)buf,(const char *)&env[4],LocM.IpAdr[0],
                             										LocM.IpAdr[1],
																	LocM.IpAdr[2],
																	LocM.IpAdr[3]);
               		break;
				case 'E':
               		len = sprintf((char *)buf,(const char *)&env[4],LocM.NetMask[0],
                             										LocM.NetMask[1],
																	LocM.NetMask[2],
																	LocM.NetMask[3]);
               		break;
				case 'F':
               		len = sprintf((char *)buf,(const char *)&env[4],LocM.DefGW[0],
                             										LocM.DefGW[1],
																	LocM.DefGW[2],
																	LocM.DefGW[3]);
               		break;
				case 'G':
               		len = sprintf((char *)buf,(const char *)&env[4],LocM.PriDNS[0],
                             										LocM.PriDNS[1],
																	LocM.PriDNS[2],
																	LocM.PriDNS[3]);
               		break;
				case 'H':
               		len = sprintf((char *)buf,(const char *)&env[4],LocM.SecDNS[0],
                             										LocM.SecDNS[1],
																	LocM.SecDNS[2],
																	LocM.SecDNS[3]);
               		break;
				case 'I':
               		len = sprintf((char *)buf,(const char *)&env[4],own_hw_adr[0],
                             										own_hw_adr[1],
																	own_hw_adr[2],
																	own_hw_adr[3],
																	own_hw_adr[4],
																	own_hw_adr[5]);
               		break;
         	}
         	break;

		case 'f':
      		/* Format SD Card - file 'format.cgi' */
      		switch (env[2]) 
			{
        		case '1':
          			break;
      		}
      		break;

		case 'g': 	// lista diretório raiz
		case 'h':	// lista diretório relatório
		case 'i':	// lista diretórios sistema
		case 'j':	// lista diretório update
			
			if (_DEF_DRIVE==_SDCARD)
			{
				EXTENSION[3]='\\';
				EXTENSION[4]='*';
				EXTENSION[5]='.';
				EXTENSION[6]='*';
				EXTENSION[7]=0;

				if (env[0]=='g')
				{
					EXTENSION[0]='*';
					EXTENSION[1]='.';
					EXTENSION[2]='*';
					EXTENSION[3]=0;
					dirbase[0]=0;
				}
				else if (env[0]=='h')
				{					
					EXTENSION[0]='R';
					EXTENSION[1]='E';
					EXTENSION[2]='L';
					dirbase[0]='R';
					dirbase[1]='E';
					dirbase[2]='L';
					dirbase[3]='/';
					dirbase[4]=0;
				}
				else if (env[0]=='i')
				{
					EXTENSION[0]='S';
					EXTENSION[1]='Y';
					EXTENSION[2]='S';
					dirbase[0]='S';
					dirbase[1]='Y';
					dirbase[2]='S';
					dirbase[3]='/';
					dirbase[4]=0;
				}
				else if (env[0]=='j')
				{
					EXTENSION[0]='U';
					EXTENSION[1]='P';
					EXTENSION[2]='D';
					dirbase[0]='U';
					dirbase[1]='P';
					dirbase[2]='D';
					dirbase[3]='/';
					dirbase[4]=0;
				}
      		
				/* Directory of SD Card - file 'dir.cgi' */
      			if (MYBUF(pcgi)->xcnt == 0) 
				{
	        		/* First Call, set initial value for 'ffind' function. */
  					fsize = 0;
        			info.fileID = 0;
        			if (finit() != 0) 
					{
	          			/* No card or failed to initialize. */
          				break;
        			}
      			}
      			/* Repeat for all files, ignore Folders. */
      			MYBUF(pcgi)->xcnt++;
      			if (ffind (EXTENSION,&info) == 0) 
				{
					if (info.attrib & ATTR_DIRECTORY) 
					{
						if (info.name[0]!='.')
						{
        					len += sprintf ((char *)buf,"<tr align=center><td>%d.</td>"
                		                    	"<td align=left><a href=\"/dir%s.cgi\">%s</a></td>"
                        		            	"<td align=right>&#60;DIR&#62;</td>"
                                		    	"<td>%02d.%02d.%04d - %02d:%02d</td></tr>\r\n",
                           						MYBUF(pcgi)->xcnt,info.name,info.name,
                           						info.time.day, info.time.mon, info.time.year,
                           						info.time.hr, info.time.min);
						}
					}
					else
					{
	        			dot_format (info.size, temp);
        				len += sprintf ((char *)buf,"<tr align=center><td>%d.</td>"
                		                    	"<td align=left><a href=\"%s%s\">%s</a></td>"
                        		            	"<td align=right>%s</td>"
                                		    	"<td>%02d.%02d.%04d - %02d:%02d</td></tr>\r\n",
                           						MYBUF(pcgi)->xcnt,dirbase,info.name,info.name,temp,
                           						info.time.day, info.time.mon, info.time.year,
                           						info.time.hr, info.time.min);
      					fsize += info.size;
					}
        			/* Hi bit is a repeat flag. */
        			len |= 0x8000;
				}
				else
				{
					dot_format (fsize, temp);
        			len += sprintf ((char *)buf,"<tr align=center><td></td>"
												"<th align=right>Total:</th>"
												"<th align=right>%s</th></tr>"
												"\r\n",temp);
				}
			}
			else if (_DEF_DRIVE==_FLASHFS)
			{
				if (env[0]=='g')
				{
					EXTENSION[0]='*';
					EXTENSION[1]='.';
					EXTENSION[2]='*';
					EXTENSION[3]=0;
				}
				else if (env[0]=='h')
				{					
					EXTENSION[0]='*';
					EXTENSION[1]='.';
					EXTENSION[2]='C';
					EXTENSION[3]='S';
					EXTENSION[4]='V';
					EXTENSION[5]=0;
				}
				else if (env[0]=='i')
				{					
					EXTENSION[0]='*';
					EXTENSION[1]='.';
					EXTENSION[2]='B';
					EXTENSION[3]='M';
					EXTENSION[4]='P';
					EXTENSION[5]=0;
				}
				else if (env[0]=='j')
				{
					EXTENSION[0]='*';
					EXTENSION[1]='.';
					EXTENSION[2]='B';
					EXTENSION[3]='I';
					EXTENSION[4]='N';
					EXTENSION[5]=0;
				}

	      		/* Directory of SPI Flash - file 'dir.cgi' */
				if (MYBUF(pcgi)->xcnt == 0) 
				{
		       		/* First Call, set initial value for 'ffind' function. */
  					fsize = 0;
	        		info.fileID = 0;
				}

	      		/* Repeat for all files, ignore Folders. */
	      		MYBUF(pcgi)->xcnt++;

				//os_dly_wait(10);
	      		if (ffind (EXTENSION,&info) == 0) 
				{
					dot_format (info.size, temp);
	        		len += sprintf ((char *)buf,"<tr align=center><td>%d.</td>"
	               		                    	"<td align=left><a href=\"/%s\">%s</a></td>"
	                       		            	"<td align=right>%s</td>"
	                               		    	"</tr>\r\n",
	                       						MYBUF(pcgi)->xcnt,info.name,info.name,temp);
      				fsize += info.size;
					/* Hi bit is a repeat flag. */
	       			len |= 0x8000;
				}
				else
				{
					dot_format (fsize, temp);
        			len += sprintf ((char *)buf,"<tr align=center><td></td>"
												"<th align=right>Total:</th>"
												"<th align=right>%s</th></tr>"
												"\r\n",temp);
				}
		  	}
      		break;  	
		
		case 'm':
		    if (_DEF_DRIVE==_SDCARD)
			{
				fsize = ffree("M:");
				dot_format (fsize, temp);
				if (fsize<64000)
				{
        			len += sprintf ((char *)buf,"<tr bgcolor=#aaccff align=left>"
                		                    	"<td>Usando SD CARD: <B>%s</B> bytes livres.</td></tr>"
												"<tr bgcolor=#FF0000 align=left>"
                			                    "<td>ATEN&#199;&#195;O: Salve os arquivos e formate a unidade.</td></tr>"
												"\r\n",temp);
				}
				else
				{
        			len += sprintf ((char *)buf,"<tr bgcolor=#aaccff align=left>"
                		                    	"<td>Usando SD CARD: <B>%s</B> bytes livres.</td></tr>"
												"\r\n",temp);
				}

			}
			else
			{
				fsize = ffree("S:");
				dot_format (fsize, temp);
				if (fsize<64000)
				{
        			len += sprintf ((char *)buf,"<tr bgcolor=#AACCFF align=left>"
                			                    "<td>Usando FLASH: <B>%s</B> bytes livres.</td></tr>"
												"<tr bgcolor=#FF0000 align=left>"
                			                    "<td>ATEN&#199;&#195;O: Salve os arquivos e formate a unidade.</td></tr>"
												"\r\n",temp);
				}
				else
				{
        			len += sprintf ((char *)buf,"<tr bgcolor=#AACCFF align=left>"
                			                    "<td>Usando FLASH: <B>%s</B> bytes livres.</td></tr>"
												"\r\n",temp);
				}
			}
			break;

    	case 'n':
	      	/* TCP status - file 'tcp.cgi' */
      		while ((len + 150) < buflen) 
			{
		        tsoc = &tcp_socket[MYBUF(pcgi)->xcnt];
        		MYBUF(pcgi)->xcnt++;
        		/* 'sprintf' format string is defined here. */
        		len += sprintf((char *)(buf+len),"<tr align=\"center\">");
        		if (tsoc->State <= TCP_STATE_CLOSED) 
				{
	          		len += sprintf ((char *)(buf+len),
                          	"<td>%d</td><td>%s</td><td>-</td><td>-</td>"
                          	"<td>-</td><td>-</td></tr>\r\n",
                          	MYBUF(pcgi)->xcnt,state[tsoc->State]);
        		}
        		else if (tsoc->State == TCP_STATE_LISTEN) 
				{
	          		len += sprintf ((char *)(buf+len),
                          	"<td>%d</td><td>%s</td><td>-</td><td>-</td>"
                          	"<td>%d</td><td>-</td></tr>\r\n",
                          	MYBUF(pcgi)->xcnt,state[tsoc->State],tsoc->LocPort);
        		}
        		else 
				{
	          		len += sprintf ((char *)(buf+len),
                          	"<td>%d</td><td>%s</td><td>%d.%d.%d.%d</td>"
                          	"<td>%d</td><td>%d</td><td>%d</td></tr>\r\n",
                          	MYBUF(pcgi)->xcnt,state[tsoc->State],
                          	tsoc->RemIpAdr[0],tsoc->RemIpAdr[1],
                          	tsoc->RemIpAdr[2],tsoc->RemIpAdr[3],
                          	tsoc->RemPort,tsoc->LocPort,tsoc->AliveTimer);
        		}
        		/* Repeat for all TCP Sockets. */
        		if (MYBUF(pcgi)->xcnt == tcp_NumSocks) 
				{
	          		break;
        		}
      		}
      		if (MYBUF(pcgi)->xcnt < tcp_NumSocks) 
			{
	        	/* Hi bit is a repeat flag. */
        		len |= 0x8000;
      		}
      		break;
		    
		case 'o':
			/* Informações tempo real - arquivo 'real.cgi' */
      		switch (env[2]) 
			{
				case 'A':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depName[0]);
					break;
				case 'B':
               		len = sprintf((char *)buf,(const char *)&env[4],queue[0]);
					break;
				case 'C':
	          		len = sprintf((char *)buf,(const char *)&env[4],CalcForecast(1)/60,
																	CalcForecast(1)%60);
          			break;
				case 'D':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depName[1]);
					break;
				case 'E':
               		len = sprintf((char *)buf,(const char *)&env[4],queue[1]);
					break;
				case 'F':
	          		len = sprintf((char *)buf,(const char *)&env[4],CalcForecast(2)/60,
																	CalcForecast(2)%60);
          			break;
				case 'G':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depName[2]);
					break;
				case 'H':
               		len = sprintf((char *)buf,(const char *)&env[4],queue[2]);
					break;
				case 'I':
	          		len = sprintf((char *)buf,(const char *)&env[4],CalcForecast(3)/60,
																	CalcForecast(3)%60);
          			break;
				case 'J':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depName[3]);
					break;
				case 'K':
               		len = sprintf((char *)buf,(const char *)&env[4],queue[3]);
					break;
				case 'L':
	          		len = sprintf((char *)buf,(const char *)&env[4],CalcForecast(4)/60,
																	CalcForecast(4)%60);
          			break;
				case 'M':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depName[4]);
					break;
				case 'N':
               		len = sprintf((char *)buf,(const char *)&env[4],queue[4]);
					break;
				case 'O':
	          		len = sprintf((char *)buf,(const char *)&env[4],CalcForecast(5)/60,
																	CalcForecast(5)%60);
          			break;
				case 'P':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depName[5]);
					break;
				case 'Q':
               		len = sprintf((char *)buf,(const char *)&env[4],queue[5]);
					break;
				case 'R':
	          		len = sprintf((char *)buf,(const char *)&env[4],CalcForecast(6)/60,
																	CalcForecast(6)%60);
          			break;
				case 'S':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depName[6]);
					break;
				case 'T':
               		len = sprintf((char *)buf,(const char *)&env[4],queue[6]);
					break;
				case 'U':
	          		len = sprintf((char *)buf,(const char *)&env[4],CalcForecast(7)/60,
																	CalcForecast(7)%60);
          			break;
				case 'V':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depName[7]);
					break;
				case 'W':
               		len = sprintf((char *)buf,(const char *)&env[4],queue[7]);
					break;
				case 'X':
	          		len = sprintf((char *)buf,(const char *)&env[4],CalcForecast(8)/60,
																	CalcForecast(8)%60);
          			break;
				case 'Y':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depName[8]);
					break;
				case 'Z':
               		len = sprintf((char *)buf,(const char *)&env[4],queue[8]);
					break;
				case '0':
	          		len = sprintf((char *)buf,(const char *)&env[4],CalcForecast(9)/60,
																	CalcForecast(9)%60);
          			break;
				case '1':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depName[9]);
					break;
				case '2':
               		len = sprintf((char *)buf,(const char *)&env[4],queue[9]);
					break;
				case '3':
	          		len = sprintf((char *)buf,(const char *)&env[4],CalcForecast(10)/60,
																	CalcForecast(10)%60);
          			break;
				case '4':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depName[10]);
					break;
				case '5':
               		len = sprintf((char *)buf,(const char *)&env[4],queue[10]);
					break;
				case '6':
	          		len = sprintf((char *)buf,(const char *)&env[4],CalcForecast(11)/60,
																	CalcForecast(11)%60);
          			break;
				case '7':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.depName[11]);
					break;
				case '8':
               		len = sprintf((char *)buf,(const char *)&env[4],queue[11]);
					break;
				case '9':
	          		len = sprintf((char *)buf,(const char *)&env[4],CalcForecast(12)/60,
																	CalcForecast(12)%60);
          			break;
      		}
      		break;

		case 'p':
			/* Relação Usuário X Mesa - arquivo 'userbox.cgi' */
        	switch (env[2]) 
			{
				case '0':
               		len = sprintf((char *)buf,(const char *)&env[4],USER==1?"Selected":"",
																	USER==2?"Selected":"",
																	USER==3?"Selected":"",
																	USER==4?"Selected":"",
																	USER==5?"Selected":"");
					break;
				case '1':
               		len = sprintf((char *)buf,(const char *)&env[4],USER==6?"Selected":"",
																	USER==7?"Selected":"",
																	USER==8?"Selected":"",
																	USER==9?"Selected":"",
																	USER==10?"Selected":"");
					break;
				case '2':
               		len = sprintf((char *)buf,(const char *)&env[4],USER==11?"Selected":"",
																	USER==12?"Selected":"",
																	USER==13?"Selected":"",
																	USER==14?"Selected":"",
																	USER==15?"Selected":"");
					break;
				case '3':
               		len = sprintf((char *)buf,(const char *)&env[4],USER==16?"Selected":"",
																	USER==17?"Selected":"",
																	USER==18?"Selected":"",
																	USER==19?"Selected":"",
																	USER==20?"Selected":"");
					break;
				case '4':
               		len = sprintf((char *)buf,(const char *)&env[4],USER==21?"Selected":"",
																	USER==22?"Selected":"",
																	USER==23?"Selected":"",
																	USER==24?"Selected":"",
																	USER==25?"Selected":"");
					break;
            	case '5':
               		len = sprintf((char *)buf,(const char *)&env[4],USER==26?"Selected":"",
																	USER==27?"Selected":"",
																	USER==28?"Selected":"",
																	USER==29?"Selected":"",
																	USER==30?"Selected":"");
					break;
            	case '6':
               		len = sprintf((char *)buf,(const char *)&env[4],USER==31?"Selected":"",
																	USER==32?"Selected":"",
																	USER==33?"Selected":"",
																	USER==34?"Selected":"",
																	USER==35?"Selected":"");
         		case '7':
               		len = sprintf((char *)buf,(const char *)&env[4],USER==36?"Selected":"",
																	USER==37?"Selected":"",
																	USER==38?"Selected":"",
																	USER==39?"Selected":"",
																	USER==40?"Selected":"");
         		case '8':
               		len = sprintf((char *)buf,(const char *)&env[4],USER==41?"Selected":"",
																	USER==42?"Selected":"",
																	USER==43?"Selected":"",
																	USER==44?"Selected":"",
																	USER==45?"Selected":"");
            		break;
         		case '9':
               		len = sprintf((char *)buf,(const char *)&env[4],USER==46?"Selected":"",
																	USER==47?"Selected":"",
																	USER==48?"Selected":"",
																	USER==49?"Selected":"",
																	USER==50?"Selected":"");
            		break;
         		case ':':
               		len = sprintf((char *)buf,(const char *)&env[4],USER==51?"Selected":"",
																	USER==52?"Selected":"",
																	USER==53?"Selected":"",
																	USER==54?"Selected":"",
																	USER==55?"Selected":"");
            		break;
         		case ';':
               		len = sprintf((char *)buf,(const char *)&env[4],USER==56?"Selected":"",
																	USER==57?"Selected":"",
																	USER==58?"Selected":"",
																	USER==59?"Selected":"",
																	USER==60?"Selected":"");
            		break;
         		case '<':
               		len = sprintf((char *)buf,(const char *)&env[4],USER==61?"Selected":"",
																	USER==62?"Selected":"",
																	USER==63?"Selected":"",
																	USER==64?"Selected":"",
																	USER==65?"Selected":"");
            		break;
         		case '=':
               		len = sprintf((char *)buf,(const char *)&env[4],USER==66?"Selected":"",
																	USER==67?"Selected":"",
																	USER==68?"Selected":"",
																	USER==69?"Selected":"",
																	USER==70?"Selected":"");
            		break;
         		case '>':
               		len = sprintf((char *)buf,(const char *)&env[4],USER==71?"Selected":"",
																	USER==72?"Selected":"",
																	USER==73?"Selected":"",
																	USER==74?"Selected":"",
																	USER==75?"Selected":"");
            		break;
				case '?':
               		len = sprintf((char *)buf,(const char *)&env[4],USER==76?"Selected":"",
																	USER==77?"Selected":"",
																	USER==78?"Selected":"",
																	USER==79?"Selected":"",
																	USER==80?"Selected":"");
					break;
				case '@':
               		len = sprintf((char *)buf,(const char *)&env[4],USER==81?"Selected":"",
																	USER==82?"Selected":"",
																	USER==83?"Selected":"",
																	USER==84?"Selected":"",
																	USER==85?"Selected":"");
					break;
         		case 'A':
               		len = sprintf((char *)buf,(const char *)&env[4],USER==86?"Selected":"",
																	USER==87?"Selected":"",
																	USER==88?"Selected":"",
																	USER==89?"Selected":"",
																	USER==90?"Selected":"");
					break;
         		case 'B':
               		len = sprintf((char *)buf,(const char *)&env[4],USER==91?"Selected":"",
																	USER==92?"Selected":"",
																	USER==93?"Selected":"",
																	USER==94?"Selected":"",
																	USER==95?"Selected":"");
					break;
         		case 'C':
               		len = sprintf((char *)buf,(const char *)&env[4],USER==96?"Selected":"",
																	USER==97?"Selected":"",
																	USER==98?"Selected":"",
																	USER==99?"Selected":"");
					break;
         		case 'D':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[USER-1]==1?"Selected":"",
																	cfg.userBox[USER-1]==2?"Selected":"",
																	cfg.userBox[USER-1]==3?"Selected":"",
																	cfg.userBox[USER-1]==4?"Selected":"",
																	cfg.userBox[USER-1]==5?"Selected":"");
					break;
         		case 'E':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[USER-1]==6?"Selected":"",
																	cfg.userBox[USER-1]==7?"Selected":"",
																	cfg.userBox[USER-1]==8?"Selected":"",
																	cfg.userBox[USER-1]==9?"Selected":"",
																	cfg.userBox[USER-1]==10?"Selected":"");
					break;
         		case 'F':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[USER-1]==11?"Selected":"",
																	cfg.userBox[USER-1]==12?"Selected":"",
																	cfg.userBox[USER-1]==13?"Selected":"",
																	cfg.userBox[USER-1]==14?"Selected":"",
																	cfg.userBox[USER-1]==15?"Selected":"");
					break;
         		case 'G':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[USER-1]==16?"Selected":"",
																	cfg.userBox[USER-1]==17?"Selected":"",
																	cfg.userBox[USER-1]==18?"Selected":"",
																	cfg.userBox[USER-1]==19?"Selected":"",
																	cfg.userBox[USER-1]==20?"Selected":"");
					break;
         		case 'H':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[USER-1]==21?"Selected":"",
																	cfg.userBox[USER-1]==22?"Selected":"",
																	cfg.userBox[USER-1]==23?"Selected":"",
																	cfg.userBox[USER-1]==24?"Selected":"",
																	cfg.userBox[USER-1]==25?"Selected":"");
					break;
         		case 'I':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[USER-1]==26?"Selected":"",
																	cfg.userBox[USER-1]==27?"Selected":"",
																	cfg.userBox[USER-1]==28?"Selected":"",
																	cfg.userBox[USER-1]==29?"Selected":"",
																	cfg.userBox[USER-1]==30?"Selected":"");
					break;
         		case 'J':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[USER-1]==31?"Selected":"",
																	cfg.userBox[USER-1]==32?"Selected":"",
																	cfg.userBox[USER-1]==33?"Selected":"",
																	cfg.userBox[USER-1]==34?"Selected":"",
																	cfg.userBox[USER-1]==35?"Selected":"");
					break;
         		case 'K':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[USER-1]==36?"Selected":"",
																	cfg.userBox[USER-1]==37?"Selected":"",
																	cfg.userBox[USER-1]==38?"Selected":"",
																	cfg.userBox[USER-1]==39?"Selected":"",
																	cfg.userBox[USER-1]==40?"Selected":"");
					break;
         		case 'L':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[USER-1]==41?"Selected":"",
																	cfg.userBox[USER-1]==42?"Selected":"",
																	cfg.userBox[USER-1]==43?"Selected":"",
																	cfg.userBox[USER-1]==44?"Selected":"",
																	cfg.userBox[USER-1]==45?"Selected":"");
					break;
         		case 'M':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[USER-1]==46?"Selected":"",
																	cfg.userBox[USER-1]==47?"Selected":"",
																	cfg.userBox[USER-1]==48?"Selected":"",
																	cfg.userBox[USER-1]==49?"Selected":"",
																	cfg.userBox[USER-1]==50?"Selected":"");
					break;
         		case 'N':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[USER-1]==51?"Selected":"",
																	cfg.userBox[USER-1]==52?"Selected":"",
																	cfg.userBox[USER-1]==53?"Selected":"",
																	cfg.userBox[USER-1]==54?"Selected":"",
																	cfg.userBox[USER-1]==55?"Selected":"");
					break;
         		case 'O':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[USER-1]==56?"Selected":"",
																	cfg.userBox[USER-1]==57?"Selected":"",
																	cfg.userBox[USER-1]==58?"Selected":"",
																	cfg.userBox[USER-1]==59?"Selected":"",
																	cfg.userBox[USER-1]==60?"Selected":"");
					break;
         		case 'P':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[USER-1]==61?"Selected":"",
																	cfg.userBox[USER-1]==62?"Selected":"",
																	cfg.userBox[USER-1]==63?"Selected":"",
																	cfg.userBox[USER-1]==64?"Selected":"",
																	cfg.userBox[USER-1]==65?"Selected":"");
					break;
         		case 'Q':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[USER-1]==66?"Selected":"",
																	cfg.userBox[USER-1]==67?"Selected":"",
																	cfg.userBox[USER-1]==68?"Selected":"",
																	cfg.userBox[USER-1]==69?"Selected":"",
																	cfg.userBox[USER-1]==70?"Selected":"");
					break;
         		case 'R':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[USER-1]==71?"Selected":"",
																	cfg.userBox[USER-1]==72?"Selected":"",
																	cfg.userBox[USER-1]==73?"Selected":"",
																	cfg.userBox[USER-1]==74?"Selected":"",
																	cfg.userBox[USER-1]==75?"Selected":"");
					break;
         		case 'S':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[USER-1]==76?"Selected":"",
																	cfg.userBox[USER-1]==77?"Selected":"",
																	cfg.userBox[USER-1]==78?"Selected":"",
																	cfg.userBox[USER-1]==79?"Selected":"",
																	cfg.userBox[USER-1]==80?"Selected":"");
					break;
         		case 'T':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[USER-1]==81?"Selected":"",
																	cfg.userBox[USER-1]==82?"Selected":"",
																	cfg.userBox[USER-1]==83?"Selected":"",
																	cfg.userBox[USER-1]==84?"Selected":"",
																	cfg.userBox[USER-1]==85?"Selected":"");
					break;
         		case 'U':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[USER-1]==86?"Selected":"",
																	cfg.userBox[USER-1]==87?"Selected":"",
																	cfg.userBox[USER-1]==88?"Selected":"",
																	cfg.userBox[USER-1]==89?"Selected":"",
																	cfg.userBox[USER-1]==90?"Selected":"");
					break;
         		case 'V':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[USER-1]==91?"Selected":"",
																	cfg.userBox[USER-1]==92?"Selected":"",
																	cfg.userBox[USER-1]==93?"Selected":"",
																	cfg.userBox[USER-1]==94?"Selected":"",
																	cfg.userBox[USER-1]==95?"Selected":"");
					break;
         		case 'W':
               		len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[USER-1]==96?"Selected":"",
																	cfg.userBox[USER-1]==97?"Selected":"",
																	cfg.userBox[USER-1]==98?"Selected":"",
																	cfg.userBox[USER-1]==99?"Selected":"");
					break;
         		case 'X':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[0],
                                                        			cfg.userBox[1],
																	cfg.userBox[2],
																	cfg.userBox[3]);
					break;
         		case 'Y':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[4],
                                                        			cfg.userBox[5],
																	cfg.userBox[6],
																	cfg.userBox[7]);
					break;
         		case 'Z':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[8],
                                                        			cfg.userBox[9],
																	cfg.userBox[10],
																	cfg.userBox[11]);
					break;
         		case '[':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[12],
                                                        			cfg.userBox[13],
																	cfg.userBox[14],
																	cfg.userBox[15]);
					break;
         		case '\\':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[16],
                                                        			cfg.userBox[17],
																	cfg.userBox[18],
																	cfg.userBox[19]);
					break;
         		case ']':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[20],
                                                        			cfg.userBox[21],
																	cfg.userBox[22],
																	cfg.userBox[23]);
					break;
         		case '^':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[24],
                                                        			cfg.userBox[25],
																	cfg.userBox[26],
																	cfg.userBox[27]);
					break;
         		case '_':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[28],
                                                        			cfg.userBox[29],
																	cfg.userBox[30],
																	cfg.userBox[31]);
					break;
         		case '`':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[32],
                                                        			cfg.userBox[33],
																	cfg.userBox[34],
																	cfg.userBox[35]);
					break;
         		case 'a':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[36],
                                                        			cfg.userBox[37],
																	cfg.userBox[38],
																	cfg.userBox[39]);
					break;
         		case 'b':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[40],
                                                        			cfg.userBox[41],
																	cfg.userBox[42],
																	cfg.userBox[43]);
					break;
         		case 'c':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[44],
                                                        			cfg.userBox[45],
																	cfg.userBox[46],
																	cfg.userBox[47]);
					break;
         		case 'd':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[48],
                                                        			cfg.userBox[49],
																	cfg.userBox[50],
																	cfg.userBox[51]);
					break;
         		case 'e':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[52],
                                                        			cfg.userBox[53],
																	cfg.userBox[54],
																	cfg.userBox[55]);
					break;
         		case 'f':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[56],
                                                        			cfg.userBox[57],
																	cfg.userBox[58],
																	cfg.userBox[59]);
					break;
         		case 'g':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[60],
                                                        			cfg.userBox[61],
																	cfg.userBox[62],
																	cfg.userBox[63]);
					break;
         		case 'h':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[64],
                                                        			cfg.userBox[65],
																	cfg.userBox[66],
																	cfg.userBox[67]);
					break;
         		case 'i':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[68],
                                                        			cfg.userBox[69],
																	cfg.userBox[70],
																	cfg.userBox[71]);
					break;
         		case 'j':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[72],
                                                        			cfg.userBox[73],
																	cfg.userBox[74],
																	cfg.userBox[75]);
					break;
         		case 'k':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[76],
                                                        			cfg.userBox[77],
																	cfg.userBox[78],
																	cfg.userBox[79]);
					break;
         		case 'l':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[80],
                                                        			cfg.userBox[81],
																	cfg.userBox[82],
																	cfg.userBox[83]);
					break;
         		case 'm':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[84],
                                                        			cfg.userBox[85],
																	cfg.userBox[86],
																	cfg.userBox[87]);
					break;
         		case 'n':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[88],
                                                        			cfg.userBox[89],
																	cfg.userBox[90],
																	cfg.userBox[91]);
					break;
         		case 'o':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[92],
                                                        			cfg.userBox[93],
																	cfg.userBox[94],
																	cfg.userBox[95]);
					break;
				case 'p':
          			len = sprintf((char *)buf,(const char *)&env[4],cfg.userBox[96],
                                                        			cfg.userBox[97],
																	cfg.userBox[98]);

			}
			break;
    	
		case 'u':

        	/* Parâmetros da etiqueta - arquivos 'callnext.cgi' */
        	switch (env[2]) 
			{
				case 'A':
               		len = sprintf((char *)buf,(const char *)&env[4],httpDep==1?"Checked":"",
																	cfg.depName[0]);
					break;
				case 'B':
               		len = sprintf((char *)buf,(const char *)&env[4],queue[0]);
					break;
				case 'C':
               		len = sprintf((char *)buf,(const char *)&env[4],lastAlphaDep[0],
																	lastTicketDep[0],
																	lastUser[0]);
					break;
				case 'D':
               		len = sprintf((char *)buf,(const char *)&env[4],httpDep==2?"Checked":"",
																	cfg.depName[1]);
					break;
				case 'E':
               		len = sprintf((char *)buf,(const char *)&env[4],queue[1]);
					break;
				case 'F':
               		len = sprintf((char *)buf,(const char *)&env[4],lastAlphaDep[1],
																	lastTicketDep[1],
																	lastUser[1]);
					break;
				case 'G':
               		len = sprintf((char *)buf,(const char *)&env[4],httpDep==3?"Checked":"",
																	cfg.depName[2]);
					break;
				case 'H':
               		len = sprintf((char *)buf,(const char *)&env[4],queue[2]);
					break;
				case 'I':
               		len = sprintf((char *)buf,(const char *)&env[4],lastAlphaDep[2],
																	lastTicketDep[2],
																	lastUser[2]);
					break;
				case 'J':
               		len = sprintf((char *)buf,(const char *)&env[4],httpDep==4?"Checked":"",
																	cfg.depName[3]);
					break;
				case 'K':
               		len = sprintf((char *)buf,(const char *)&env[4],queue[3]);
					break;
				case 'L':
               		len = sprintf((char *)buf,(const char *)&env[4],lastAlphaDep[3],
																	lastTicketDep[3],
																	lastUser[3]);
					break;
				case 'M':
               		len = sprintf((char *)buf,(const char *)&env[4],httpDep==5?"Checked":"",
																	cfg.depName[4]);
					break;
				case 'N':
               		len = sprintf((char *)buf,(const char *)&env[4],queue[4]);
					break;
				case 'O':
               		len = sprintf((char *)buf,(const char *)&env[4],lastAlphaDep[4],
																	lastTicketDep[4],
																	lastUser[4]);
					break;
				case 'P':
               		len = sprintf((char *)buf,(const char *)&env[4],httpDep==6?"Checked":"",
																	cfg.depName[5]);
					break;
				case 'Q':
               		len = sprintf((char *)buf,(const char *)&env[4],queue[5]);
					break;
				case 'R':
               		len = sprintf((char *)buf,(const char *)&env[4],lastAlphaDep[5],
																	lastTicketDep[5],
																	lastUser[5]);
					break;
				case 'S':
               		len = sprintf((char *)buf,(const char *)&env[4],httpDep==7?"Checked":"",
																	cfg.depName[6]);
					break;
				case 'T':
               		len = sprintf((char *)buf,(const char *)&env[4],queue[6]);
					break;
				case 'U':
               		len = sprintf((char *)buf,(const char *)&env[4],lastAlphaDep[6],
																	lastTicketDep[6],
																	lastUser[6]);
					break;
				case 'V':
               		len = sprintf((char *)buf,(const char *)&env[4],httpDep==8?"Checked":"",
																	cfg.depName[7]);
					break;
				case 'W':
               		len = sprintf((char *)buf,(const char *)&env[4],queue[7]);
					break;
				case 'X':
               		len = sprintf((char *)buf,(const char *)&env[4],lastAlphaDep[7],
																	lastTicketDep[7],
																	lastUser[7]);
					break;
				case 'Y':
               		len = sprintf((char *)buf,(const char *)&env[4],httpDep==9?"Checked":"",
																	cfg.depName[8]);
					break;
				case 'Z':
               		len = sprintf((char *)buf,(const char *)&env[4],queue[8]);
					break;
				case '0':
               		len = sprintf((char *)buf,(const char *)&env[4],lastAlphaDep[8],
																	lastTicketDep[8],
																	lastUser[8]);
					break;
				case '1':
               		len = sprintf((char *)buf,(const char *)&env[4],httpDep==10?"Checked":"",
																	cfg.depName[9]);
					break;
				case '2':
               		len = sprintf((char *)buf,(const char *)&env[4],queue[9]);
					break;
				case '3':
               		len = sprintf((char *)buf,(const char *)&env[4],lastAlphaDep[9],
																	lastTicketDep[9],
																	lastUser[9]);
					break;
				case '4':
               		len = sprintf((char *)buf,(const char *)&env[4],httpDep==11?"Checked":"",
																	cfg.depName[10]);
					break;
				case '5':
               		len = sprintf((char *)buf,(const char *)&env[4],queue[10]);
					break;
				case '6':
               		len = sprintf((char *)buf,(const char *)&env[4],lastAlphaDep[10],
																	lastTicketDep[10],
																	lastUser[10]);
					break;
				case '7':
               		len = sprintf((char *)buf,(const char *)&env[4],httpDep==12?"Checked":"",
																	cfg.depName[11]);
					break;
				case '8':
               		len = sprintf((char *)buf,(const char *)&env[4],queue[11]);
					break;
				case '9':
               		len = sprintf((char *)buf,(const char *)&env[4],lastAlphaDep[11],
																	lastTicketDep[11],
																	lastUser[11]);
					break;
			}
			break;

		case 'v':
			/* Parâmetros de upload de imagens - arquivos 'design.cgi' */
        	switch (env[2]) 
			{
				case 'A':
               		len = sprintf((char *)buf,(const char *)&env[4],taskFile==UPD_SCR1?"Checked":"");
               		break;
				case 'B':														 
               		len = sprintf((char *)buf,(const char *)&env[4],taskFile==UPD_SCR2?"Checked":"");
               		break;
				case 'C':
               		len = sprintf((char *)buf,(const char *)&env[4],taskFile==UPD_LOGO?"Checked":"");
               		break;
			}
			break;
	}
  	return ((U16)len);
}

char* verType (unsigned char runType)
{
	if (runType==0)
		return "Ticket Normal\x0";
	else if (runType==1)
		return "Ticket Preferencial\x0";

	return "\x0";
}

/*----------------------------------------------------------------------------
 *        Print size in dotted fomat
 *---------------------------------------------------------------------------*/
void dot_format (U64 val, char *sp) 
{
  	if (val >= (U64)1e9) 
  	{
  		sp += sprintf (sp,"%d.",(U32)(val/(U64)1e9));
    	val %= (U64)1e9;
    	sp += sprintf (sp,"%03d.",(U32)(val/(U64)1e6));
    	val %= (U64)1e6;
    	sprintf (sp,"%03d.%03d",(U32)(val/1000),(U32)(val%1000));
    	return;
  	}
  	if (val >= (U64)1e6) 
	{
    	sp += sprintf (sp,"%d.",(U32)(val/(U64)1e6));
    	val %= (U64)1e6;
    	sprintf (sp,"%03d.%03d",(U32)(val/1000),(U32)(val%1000));
    	return;
  	}
  	if (val >= 1000) 
	{
	    sprintf (sp,"%d.%03d",(U32)(val/1000),(U32)(val%1000));
	    return;
  	}
  	sprintf (sp,"%d",(U32)(val));
}

/*-------------------------------------------------------------------------------------------
 * Função: 		Show_DevModel
 * Descrição: 	Identifica o nome do modelo do dispositivo através do número
 * Parâmetros: 	devModel - Número do modelo para mostrar a informação
 * Retorno: 	Devolve um ponteiro para a string do modelo
-------------------------------------------------------------------------------------------*/
char *Show_DevModel (unsigned char devModel)
{
	char *pt=NULL;

	switch (devModel)
	{
		case 0: pt="Nenhum"; 		break;
	 	case 1: pt="DISPLAYCALL"; 	break;
		case 2:	pt="DISPLAYMETER"; 	break;
		case 3: pt="DISPLAYPRICE"; 	break;
		case 4: pt="----"; 			break;
		case 5: pt="----"; 			break;
		case 6: pt="----"; 			break;
		case 7: pt="TD12"; 			break;
		case 8: pt="TD16"; 			break;
		case 9: pt="MIG6"; 			break;
		case 10:pt="MIG12"; 		break;
		case 11:pt="MCIE";			break;
		case 12:pt="IE"; 			break;
		case 13:pt="JM"; 			break;
		case 14:pt="JMTELV"; 		break;
		case 15:pt="JMCOMP"; 		break;
		case 16:pt="JMERI"; 		break;
		case 17:pt="JMTERM"; 		break;
		case 20:pt="TOPLIGHT";		break;
		case 21:pt="TOPMID";		break;
		case 22:pt="TOPPLUS";		break;
		case 23:pt="WYNET";			break;
		case 24:pt="CASCATA";		break;
	}
	return pt;
}

/*-------------------------------------------------------------------------------------------
* Função:		New_Call
* Descrição:	Verifica e salva chamada no buffer de espera
* Parâmetros:	Nenhum
* Retorno:		Nenhum
-------------------------------------------------------------------------------------------*/
void New_Call (void)
{
	unsigned char i;

	for (i=0; i<10; i++)								// Faz varredura procurando chamada no buffer:
	if (call[i].row==httpRow &&
		call[i].dep==httpDep &&
		call[i].func==httpFunc &&
		call[i].user==httpUser)
	{
		httpRow=0;
		httpDep=0;
		httpFunc=0;
		httpUser=0;
		break;
	}

	if (httpRow!=0 && httpDep!=0 && 
		httpFunc!=0 && httpUser!=0)						// Caso informações sejam válidas:
	{
		for (i=9; i>0; i--)								// Realoca o buffer
		{
			call[i].row = call[i-1].row;
			call[i].alpha = call[i-1].alpha;
			call[i].number = call[i-1].number;
			call[i].user = call[i-1].user;
			call[i].dep = call[i-1].dep;
			call[i].func = call[i-1].func;
		}
		call[0].row = httpRow;							// Grava número do display
		call[0].alpha = httpAlpha;						// Zera a letra do ticket
		call[0].number = httpNumber;					// Zera o número do ticket
		call[0].user = httpUser;						// Grava o box do ticket
		call[0].dep = httpDep;							// Grava o departamento de chamada
		call[0].func = httpFunc;						// Grava a função da tecla
	}
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
