/*----------------------------------------------------------------------------
 *      Arquivo:    MENU.c
 *      Proposito: 	Menu de configuração para o usuário
 *----------------------------------------------------------------------------
 *      Versão: 	1.00 	Prog: Ricardo T.   Data: 27/06/2016
 * 		Descrição:  Versão inicial
 *----------------------------------------------------------------------------
 *      Este código pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#include 		<Net_Config.h>

#include		"LCD_CONFIG.h"
#include		"RTC_CONFIG.h"
#include		"KEY_CONFIG.h"
#include		"IAP_CONFIG.h"
#include		"COMM_CONFIG.h"
#include		"REPORT.h"
#include		"App.h"
#include		"Menu.h"

#define			MENUITEMS			10
#define			TCPITEMS			7
#define 		MAXLISTWIFI			10
			 
/*----------------------------------------------------------------------------
 *      Local variables
 *---------------------------------------------------------------------------*/
unsigned char	selectedMenu=0;
unsigned char 	selectedItem=0;
unsigned int	valueAdjust=0;
unsigned char	menuPosition=0;	  
unsigned char	keepPress=0;
unsigned short 	lastKey=0;

unsigned char	selectedInterf=0; 

char 			ListWifi[MAXLISTWIFI][32]={	"TESTE1\x0","TESTE2\x0","TESTE3\x0","\x0","\x0",
											"\x0","\x0","\x0","\x0","\x0"};
unsigned char 	numListWifi=0;

char			wifiPw[32];							// Senha da rede WIFI (temp menu)
char			wifiMode;							// modo de operação do WIFI (temp menu)

const char MenuPrincipal[MENUITEMS][21] =	{
											"AJUSTAR HORA/DATA   \x0",
											"ESCOLHER DRIVER     \x0",
											"FORMATAR DRIVER     \x0",
											"CONFIGURAR REDE LAN \x0",
											"MODO OPERAÇÃO WIFI  \x0",
											"CONFIGURAR REDE WIFI\x0",
											"HORARIO DE OPERAÇÃO \x0",
											"HORARIO FECHAMENTO  \x0",
											"GRAVAÇÃO DE ARQUIVO \x0",
											"INICIALIZAR TICKETS \x0",
											};
const char MenuDateTime [2][21] = 			{
											"AJUSTAR HORARIO     \x0",
											"AJUSTAR DATA        \x0"
											};
const char MenuTcpIP [TCPITEMS][21] = 		{
											"HABILITAR DHCP      \x0",
											"ENDEREÇO IP         \x0",
											"ENDEREÇO GATEWAY    \x0",
											"ENDEREÇO DNS 1      \x0",
											"ENDEREÇO DNS 2      \x0",
											"MASCARA DE SUBREDE  \x0",
											"VISUALIZAR MAC      \x0"
											};
/*----------------------------------------------------------------------------
 *      App.c variables
 *---------------------------------------------------------------------------*/
extern CFG cfg;

/*----------------------------------------------------------------------------
 *      file_config.c variables
 *---------------------------------------------------------------------------*/
extern unsigned short  _DEF_DRIVE;

/*----------------------------------------------------------------------------
 *      Net_Config.c variables
 *---------------------------------------------------------------------------*/
extern unsigned char    http_auth_passw[20];
extern unsigned char   	http_EnAuth;
extern unsigned char	own_hw_adr[ETH_ADRLEN];
extern LOCALM 			localm[];					// Local Machine Settings 
#define LocM   			localm[NETIF_ETH]   		// Local Machine Settings 

/*----------------------------------------------------------------------------
 *      Extern Functions
 *---------------------------------------------------------------------------*/
extern unsigned char Change_Drive (unsigned char drive);
extern unsigned short Read_KeyModel (void);
extern void Beep (void);
extern void Write_Flash (void);

/*----------------------------------------------------------------------------
 *      Local Functions
 *---------------------------------------------------------------------------*/
unsigned char Show_MainMenu (void);
void ChangeMark_MainMenu (void);
unsigned char Show_ChangeMainMenu (unsigned char val);

unsigned char Show_DateTimeMenu (void);
void ChangeMark_DateTimeMenu (void);
unsigned char Show_ChangeDateTimeMenu (unsigned char val);

void Show_InitTime (void);
void Show_ChangeTime (unsigned char val);
unsigned char Save_Time (void);

void Show_InitDate (void);
void Show_ChangeDate (unsigned char val);
unsigned char Save_Date (void);

void Show_InitEscolherDriver (void);
void ChangeMark_Driver (unsigned char val);
void Show_ChangeDriver (unsigned char val);
unsigned char Save_Driver (void);

void Show_InitFormatDriver (void);
void ChangeMark_FormatDriver (void);
void Show_FormatDriver (unsigned char val);
unsigned char Save_OpcaoFormat (void);

void Show_InitTimeRun (void);
void Show_ChangeTimeRun (unsigned char val);
unsigned char Save_TimeRun (void);

void Show_InitTimeRep (void);
void Show_ChangeTimeRep (unsigned char val);
unsigned char Save_TimeRep (void);

void Show_InitSaveRep (void);
void ChangeMark_SaveRep (unsigned char val);
void Show_ChangeSaveRep (unsigned char val);
void Save_SaveRep (void);

void Show_InitTicket (void);
void ChangeMark_InitTicket (void);
void Show_ChangeInitTicket (unsigned char val);
unsigned char Save_InitTicket (void);

unsigned char Show_TCPIP (void);
void ChangeMark_TCPIP (void);
unsigned char Show_ChangeMenuTcpIP (unsigned char val);

void Show_InitIP (void);
void Show_ChangeIP (unsigned char val);
unsigned char Save_IP (void);

void Show_InitGate (void);
void Show_ChangeGate (unsigned char val);
unsigned char Save_Gate (void);

void Show_InitDns1 (void);
void Show_ChangeDns1 (unsigned char val);
unsigned char Save_Dns1 (void);

void Show_InitDns2 (void);
void Show_ChangeDns2 (unsigned char val);
unsigned char Save_Dns2 (void);

void Show_InitMask (void);
void Show_ChangeMask (unsigned char val);
unsigned char Save_Mask (void);

void Show_InitMAC (void);
void Show_ChangeMAC (unsigned char val);
unsigned char Save_MAC (void);

void Show_InitDHCP (void);
void ChangeMark_DHCP (void);
void Show_ChangeDHCP (unsigned char val);
void Save_DHCP (void);

unsigned char Show_WIFI (void);
void ChangeMark_WIFI (void);
unsigned char Show_ChangeWIFI (unsigned char val);

void Show_Ssid (void);
void ChangeMark_Ssid (void);
void Show_ChangeSsid (unsigned char val);
void Show_ChangeSsidPos (unsigned char val);

void Show_Password (void);
void ChangeMark_Password (void);
void Show_ChangePassword (unsigned char val);
void Show_ChangePasswordPos (unsigned char val);

void Save_Ssid_Password (void);

void Show_ModeWifi (void);
void ChangeMark_ModeWifi (void);
void Show_ChangeModeWifi (unsigned char val);

unsigned char CheckMenu (void);


unsigned char Show_MainMenu (void)
{
	lcdgraf_clear (page1);
	
	lcdgraf_gotoxy (1,1);
	lcdgraf_puts ("Para escolher: 3 e 4 ",0);		 	
	lcdgraf_gotoxy (1,2);
	lcdgraf_puts ("Entrar:6  Cancelar:5 ",0);		 	

	selectedItem=0;
	ChangeMark_MainMenu ();

	return selectedItem;
}

void ChangeMark_MainMenu (void)
{
	if (selectedItem>=5)
		menuPosition=5;
	else
		menuPosition=0;

	lcdgraf_gotoxy (1,4);
	if (menuPosition<MENUITEMS)
	{	
		if (selectedItem==menuPosition)
			lcdgraf_putsconst (MenuPrincipal[menuPosition],1);
		else
			lcdgraf_putsconst (MenuPrincipal[menuPosition],0);
	}
	else
		lcdgraf_puts ("                     ",0);		

	lcdgraf_gotoxy (1,5);
	if ((menuPosition+1)<MENUITEMS)
	{	
		if (selectedItem==(menuPosition+1))
			lcdgraf_putsconst (MenuPrincipal[menuPosition+1],1);
		else
			lcdgraf_putsconst (MenuPrincipal[menuPosition+1],0);
	}
	else
		lcdgraf_puts ("                     ",0);		

	lcdgraf_gotoxy (1,6);
	if ((menuPosition+2)<MENUITEMS)
	{	
		if (selectedItem==(menuPosition+2))
			lcdgraf_putsconst (MenuPrincipal[menuPosition+2],1);
		else
			lcdgraf_putsconst (MenuPrincipal[menuPosition+2],0);
	}
	else
		lcdgraf_puts ("                     ",0);		

	lcdgraf_gotoxy (1,7);
	if ((menuPosition+3)<MENUITEMS)
	{	
		if (selectedItem==(menuPosition+3))
			lcdgraf_putsconst (MenuPrincipal[menuPosition+3],1);
		else
			lcdgraf_putsconst (MenuPrincipal[menuPosition+3],0);
	}
	else
		lcdgraf_puts ("                     ",0);		

	lcdgraf_gotoxy (1,8);
	if ((menuPosition+4)<MENUITEMS)
	{	
		if (selectedItem==(menuPosition+4))
			lcdgraf_putsconst (MenuPrincipal[menuPosition+4],1);
		else
			lcdgraf_putsconst (MenuPrincipal[menuPosition+4],0);
	}
	else
		lcdgraf_puts ("                     ",0);		
}

unsigned char Show_ChangeMainMenu (unsigned char val)
{
	if (val==1)
	{
		selectedItem++;
		if (selectedItem==MENUITEMS)
			selectedItem=0;
	}
	else
	{
		selectedItem--;
		if (selectedItem==255)
			selectedItem=MENUITEMS-1;
	}

	ChangeMark_MainMenu ();										// Marca seleção atual

	return selectedItem;
}

unsigned char Show_DateTimeMenu (void)
{
	lcdgraf_clear (page1);

	lcdgraf_gotoxy (1,1);
	lcdgraf_puts ("Para escolher: 3 e 4 ",0);		 	
	lcdgraf_gotoxy (1,2);
	lcdgraf_puts ("Entrar:6  Cancelar:5 ",0);		 	

	selectedItem=0;	
	ChangeMark_DateTimeMenu ();

	return selectedItem;
}

void ChangeMark_DateTimeMenu (void)
{
	if (selectedItem>=5)
		menuPosition=5;
	else
		menuPosition=0;
				

	lcdgraf_gotoxy (1,4);
	if (selectedItem==menuPosition)
		lcdgraf_putsconst (MenuDateTime[menuPosition],1);
	else
		lcdgraf_putsconst (MenuDateTime[menuPosition],0);

	lcdgraf_gotoxy (1,5);

	if (selectedItem==(menuPosition+1))
		lcdgraf_putsconst (MenuDateTime[menuPosition+1],1);
	else
		lcdgraf_putsconst (MenuDateTime[menuPosition+1],0);
}

unsigned char Show_ChangeDateTimeMenu (unsigned char val)
{
	if (val==1)
	{
		selectedItem++;
		if (selectedItem==2)
			selectedItem=0;
	}
	else
	{
		selectedItem--;
		if (selectedItem==255)
			selectedItem=1;
	}
	ChangeMark_DateTimeMenu ();									// Marca seleção atual

	return selectedItem;
}

void Show_InitTime (void)
{
	TIME_TYPE time;
	time=Get_RTC();

	lcdgraf_clear (page1);

	lcdgraf_gotoxy (1,1);
	lcdgraf_puts ("Para ajustar: 3 e 4  ",0);		 	
	lcdgraf_gotoxy (1,2);
	lcdgraf_puts ("Entrar:6  Cancelar:5 ",0);			 	

	lcdgraf_gotoxy (7,5);
	lcdgraf_putc((time.HOUR/10)|0x30,1);
	lcdgraf_putc((time.HOUR%10)|0x30,1);
	lcdgraf_putc(':',0);
	lcdgraf_putc((time.MINUTE/10)|0x30,0);
	lcdgraf_putc((time.MINUTE%10)|0x30,0);
	lcdgraf_putc(':',0);
	lcdgraf_putc((time.SECOND/10)|0x30,0);
	lcdgraf_putc((time.SECOND%10)|0x30,0);

	menuPosition=1;
	valueAdjust=time.HOUR;
}

void Show_ChangeTime (unsigned char val)
{
	if (val==1)
		valueAdjust++;
	else
		valueAdjust--;

	if (menuPosition==1)	
	{
		if (valueAdjust==24) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=23;
		lcdgraf_gotoxy (7,5);
	}
	else if (menuPosition==2) 
	{
		if (valueAdjust==60) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=59;
		lcdgraf_gotoxy (10,5);	
	}
	else if (menuPosition==3) 
	{
		if (valueAdjust==60) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=59;		
		lcdgraf_gotoxy (13,5);
	}
	lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
	lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
}

unsigned char Save_Time (void)
{
	TIME_TYPE time;
	time=Get_RTC();

	if (menuPosition==1) 
	{
		lcdgraf_gotoxy (7,5);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,0);
		time.HOUR = valueAdjust;
		Save_RTC(time);
		valueAdjust=time.MINUTE;
		lcdgraf_gotoxy (10,5);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
	}
	else if (menuPosition==2) 
	{
		lcdgraf_gotoxy (10,5);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,0);
		time.MINUTE = valueAdjust;
		Save_RTC(time);
		valueAdjust=time.SECOND;
		lcdgraf_gotoxy (13,5);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
	}
	else if (menuPosition==3) 
	{
		time.SECOND = valueAdjust;
		Save_RTC(time);
		return 1;
	}

 	menuPosition++;
	return 0;
}

void Show_InitDate (void)
{
	TIME_TYPE time;
	time=Get_RTC();

	lcdgraf_clear (page1);

	lcdgraf_gotoxy (1,1);
	lcdgraf_puts ("Para ajustar: 3 e 4  ",0);		 	
	lcdgraf_gotoxy (1,2);
	lcdgraf_puts ("Entrar:6  Cancelar:5 ",0);			 	

	lcdgraf_gotoxy (7,5);
	lcdgraf_putc((time.DAY/10)|0x30,1);
	lcdgraf_putc((time.DAY%10)|0x30,1);
	lcdgraf_putc('/',0);
	lcdgraf_putc((time.MONTH/10)|0x30,0);
	lcdgraf_putc((time.MONTH%10)|0x30,0);
	lcdgraf_putc('/',0);
	lcdgraf_putc(((time.YEAR/1000)%10)|0x30,0);
	lcdgraf_putc(((time.YEAR/100 )%10)|0x30,0);
	lcdgraf_putc(((time.YEAR/10  )%10)|0x30,0);
	lcdgraf_putc(((time.YEAR     )%10)|0x30,0);

	menuPosition=1;
	valueAdjust=time.DAY;
}

void Show_ChangeDate (unsigned char val)
{
	if (val==1)
		valueAdjust++;
	else
		valueAdjust--;

	if (menuPosition==1)	
	{
		if (valueAdjust==32) valueAdjust=1;
		if (valueAdjust==0) valueAdjust=31;
		lcdgraf_gotoxy (7,5);
	}
	else if (menuPosition==2) 
	{	
		if (valueAdjust==13) valueAdjust=1;
		if (valueAdjust==0) valueAdjust=12;	
		lcdgraf_gotoxy (10,5);
	}
	else if (menuPosition==3) 
	{
		if (valueAdjust>=2100) valueAdjust=2000;
		if (valueAdjust<=1999) valueAdjust=2099;		
		lcdgraf_gotoxy (13,5);
		lcdgraf_putc(((valueAdjust/1000)%10)|0x30,1);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,1);
	}

	lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
	lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
}

unsigned char Save_Date (void)
{
	TIME_TYPE time;
	time=Get_RTC();

 	if (menuPosition==1) 
	{	
		lcdgraf_gotoxy (7,5);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,0);
		time.DAY = valueAdjust;
		Save_RTC(time);
		valueAdjust=time.MONTH;
		lcdgraf_gotoxy (10,5);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
	}
	else if (menuPosition==2) 
	{
		lcdgraf_gotoxy (10,5);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,0);
		time.MONTH = valueAdjust;
		Save_RTC(time);
		valueAdjust=time.YEAR;
		lcdgraf_gotoxy (13,5);
		lcdgraf_putc(((valueAdjust/1000)%10)|0x30,1);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,1);		
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
	}				
	else if (menuPosition==3) 
	{
		time.YEAR = valueAdjust;
		
		time.WEEKDAY = DayOfWeek(time);
		time.YEARDAY = DayOfYear(time);
		Save_RTC(time);
		return 1;
	}

	menuPosition++;
	return 0;
}

void Show_InitEscolherDriver (void)
{
	lcdgraf_clear (page1);

	lcdgraf_gotoxy (1,1);
	lcdgraf_puts ("Para escolher: 3 e 4 ",0);		 	
	lcdgraf_gotoxy (1,2);
	lcdgraf_puts ("Gravar:6  Cancelar:5 ",0);	

	selectedItem = (unsigned char) (_DEF_DRIVE/2);

	ChangeMark_Driver (selectedItem);
}

void ChangeMark_Driver (unsigned char val)
{
	lcdgraf_gotoxy (7,5);
	if (selectedItem==1)
		lcdgraf_puts(" FLASH   ",1);
	else
		lcdgraf_puts(" FLASH   ",0);

	lcdgraf_gotoxy (7,6);
	if (selectedItem==2)
		lcdgraf_puts(" SD CARD ",1);
	else
		lcdgraf_puts(" SD CARD ",0);
}

void Show_ChangeDriver (unsigned char val)
{
	ChangeMark_Driver (0);										// Desmarca seleção anterior
	if (val==1)
	{
		selectedItem++;
		if (selectedItem==3)
			selectedItem=1;
	}
	else
	{
		selectedItem--;
		if (selectedItem==0)
			selectedItem=2;
	}
	ChangeMark_Driver (1);										// Marca seleção atual
}

unsigned char Save_Driver (void)
{
	if (Change_Drive (selectedItem * 2))
		return 1;
	else 
		return 0;
}

void Show_InitFormatDriver (void)
{
	lcdgraf_clear (page1);

	lcdgraf_gotoxy (1,1);
	lcdgraf_puts ("  Tem certeza de que ",0);		 	
	lcdgraf_gotoxy (1,2);	
	lcdgraf_puts ("   deseja formatar?  ",0);		 	

	selectedItem = 1;

	ChangeMark_FormatDriver ();
}

void ChangeMark_FormatDriver (void)
{
	lcdgraf_gotoxy (7,5);
	if (selectedItem==1)
		lcdgraf_puts("   NAO   ",1);
	else
		lcdgraf_puts("   NAO   ",0);

	lcdgraf_gotoxy (7,6);
	if (selectedItem==2)
		lcdgraf_puts("   SIM   ",1);
	else
		lcdgraf_puts("   SIM   ",0);
}

void Show_FormatDriver (unsigned char val)
{
	ChangeMark_FormatDriver ();								// Desmarca seleção anterior
	if (val==1)
	{
		selectedItem++;
		if (selectedItem==3)
			selectedItem=1;
	}
	else
	{
		selectedItem--;
		if (selectedItem==0)
			selectedItem=2;
	}
	ChangeMark_FormatDriver ();								// Marca seleção atual
}

unsigned char Save_OpcaoFormat (void)
{
	if (selectedItem==2)
		return 1;
	else 
		return 0;
}

void Show_InitTimeRun (void)
{
	lcdgraf_clear (page1);

	lcdgraf_gotoxy (1,1);
	lcdgraf_puts ("Para ajustar: 3 e 4  ",0);		 	
	lcdgraf_gotoxy (1,2);
	lcdgraf_puts ("Entrar:6  Cancelar:5 ",0);			 	

	lcdgraf_gotoxy (1,4);
	lcdgraf_puts ("Habilita: ",0);
	if (cfg.enTimeAuto)
		lcdgraf_puts ("SIM",1);
	else
		lcdgraf_puts ("NÃO",1);

	lcdgraf_gotoxy (1,5);
	lcdgraf_puts ("Hora: ",0);
	lcdgraf_putc((cfg.timeHourOn/10)|0x30,0);
	lcdgraf_putc((cfg.timeHourOn%10)|0x30,0);
	lcdgraf_putc(':',0);
	lcdgraf_putc((cfg.timeMinOn/10)|0x30,0);
	lcdgraf_putc((cfg.timeMinOn%10)|0x30,0);
	lcdgraf_puts (" às ",0);
	lcdgraf_putc((cfg.timeHourOff/10)|0x30,0);
	lcdgraf_putc((cfg.timeHourOff%10)|0x30,0);
	lcdgraf_putc(':',0);
	lcdgraf_putc((cfg.timeMinOff/10)|0x30,0);
	lcdgraf_putc((cfg.timeMinOff%10)|0x30,0);

	lcdgraf_gotoxy (1,7);
	lcdgraf_puts ("Dias: D S T Q Q S S",0);
	lcdgraf_gotoxy (7,8);
	lcdgraf_putc (((cfg.weekDay>>6)&1)|0x30,0);
	lcdgraf_putc(' ',0);
	lcdgraf_putc (((cfg.weekDay>>5)&1)|0x30,0);
	lcdgraf_putc(' ',0);
	lcdgraf_putc (((cfg.weekDay>>4)&1)|0x30,0);
	lcdgraf_putc(' ',0);
	lcdgraf_putc (((cfg.weekDay>>3)&1)|0x30,0);
	lcdgraf_putc(' ',0);
	lcdgraf_putc (((cfg.weekDay>>2)&1)|0x30,0);
	lcdgraf_putc(' ',0);
	lcdgraf_putc (((cfg.weekDay>>1)&1)|0x30,0);
	lcdgraf_putc(' ',0);
	lcdgraf_putc ((cfg.weekDay&1)|0x30,0);
		
	menuPosition=1;
	valueAdjust=cfg.enTimeAuto;
}

void Show_ChangeTimeRun (unsigned char val)
{
	if (val==1)
		valueAdjust++;
	else
		valueAdjust--;

	if (menuPosition==1)	
	{
		if (valueAdjust==2) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=1;
		lcdgraf_gotoxy (11,4);
		if (valueAdjust==0)
			lcdgraf_puts ("NÃO",1);
		else if (valueAdjust==1)
			lcdgraf_puts ("SIM",1);
	}
	else if (menuPosition==2 || menuPosition==4)	
	{
		if (valueAdjust==24) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=23;
		
		if (menuPosition==2) lcdgraf_gotoxy (7,5);
		else				 lcdgraf_gotoxy (16,5);

		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
	}
	else if (menuPosition==3 || menuPosition==5) 
	{
		if (valueAdjust==60) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=59;
		
		if (menuPosition==3) lcdgraf_gotoxy (10,5);
		else				 lcdgraf_gotoxy (19,5);
		
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
	}
	else if (menuPosition>=6 && menuPosition<=12)
	{
		if (valueAdjust==2) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=1;

		switch (menuPosition)
		{
			case 6:		lcdgraf_gotoxy (7,8); break;
			case 7:		lcdgraf_gotoxy (9,8); break;
			case 8:		lcdgraf_gotoxy (11,8); break;
			case 9:		lcdgraf_gotoxy (13,8); break;
			case 10:	lcdgraf_gotoxy (15,8); break;
			case 11:	lcdgraf_gotoxy (17,8); break;
			case 12:	lcdgraf_gotoxy (19,8); break;
		}
		lcdgraf_putc (valueAdjust|0x30,1);
	}
}

unsigned char Save_TimeRun (void)
{
	if (menuPosition==1) 
	{
		lcdgraf_gotoxy (11,4);
		if (valueAdjust==0)
			lcdgraf_puts ("NÃO",0);
		else if (valueAdjust==1)
			lcdgraf_puts ("SIM",0);
		cfg.enTimeAuto = valueAdjust;

		valueAdjust=cfg.timeHourOn;
		lcdgraf_gotoxy (7,5);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
	}
	else if (menuPosition==2)
	{
		lcdgraf_gotoxy (7,5);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,0);
		cfg.timeHourOn = valueAdjust;

		valueAdjust=cfg.timeMinOn;
		lcdgraf_gotoxy (10,5);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
	}
	else if (menuPosition==3)
	{
		lcdgraf_gotoxy (10,5);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,0);
		cfg.timeMinOn = valueAdjust;

		valueAdjust=cfg.timeHourOff;
		lcdgraf_gotoxy (16,5);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
	}
	else if (menuPosition==4)
	{
		lcdgraf_gotoxy (16,5);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,0);
		cfg.timeHourOff = valueAdjust;
		
		valueAdjust=cfg.timeMinOff;
		lcdgraf_gotoxy (19,5);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
	}
	else if (menuPosition==5) 
	{
		lcdgraf_gotoxy (19,5);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,0);
		cfg.timeMinOff = valueAdjust;

		valueAdjust=(cfg.weekDay>>6)&1;
		lcdgraf_gotoxy (7,8);
		lcdgraf_putc (valueAdjust|0x30,1);
	}
	else if (menuPosition==6) 
	{
		lcdgraf_gotoxy (7,8);
		lcdgraf_putc (valueAdjust|0x30,0);
		cfg.weekDay &= ~(1<<6);
		cfg.weekDay |= (valueAdjust<<6);

		valueAdjust=(cfg.weekDay>>5)&1;
		lcdgraf_gotoxy (9,8);
		lcdgraf_putc (valueAdjust|0x30,1);
	}
	else if (menuPosition==7) 
	{
		lcdgraf_gotoxy (9,8);
		lcdgraf_putc (valueAdjust|0x30,0);
		cfg.weekDay &= ~(1<<5);
		cfg.weekDay |= (valueAdjust<<5);

		valueAdjust=(cfg.weekDay>>4)&1;
		lcdgraf_gotoxy (11,8);
		lcdgraf_putc (valueAdjust|0x30,1);
	}
	else if (menuPosition==8) 
	{
		lcdgraf_gotoxy (11,8);
		lcdgraf_putc (valueAdjust|0x30,0);
		cfg.weekDay &= ~(1<<4);
		cfg.weekDay |= (valueAdjust<<4);

		valueAdjust=(cfg.weekDay>>3)&1;
		lcdgraf_gotoxy (13,8);
		lcdgraf_putc (valueAdjust|0x30,1);
	}
	else if (menuPosition==9) 
	{
		lcdgraf_gotoxy (13,8);
		lcdgraf_putc (valueAdjust|0x30,0);
		cfg.weekDay &= ~(1<<3);
		cfg.weekDay |= (valueAdjust<<3);

		valueAdjust=(cfg.weekDay>>2)&1;
		lcdgraf_gotoxy (15,8);
		lcdgraf_putc (valueAdjust|0x30,1);
	}
	else if (menuPosition==10) 
	{
		lcdgraf_gotoxy (15,8);
		lcdgraf_putc (valueAdjust|0x30,0);
		cfg.weekDay &= ~(1<<2);
		cfg.weekDay |= (valueAdjust<<2);

		valueAdjust=(cfg.weekDay>>1)&1;
		lcdgraf_gotoxy (17,8);
		lcdgraf_putc (valueAdjust|0x30,1);
	}
	else if (menuPosition==11) 
	{
		lcdgraf_gotoxy (17,8);
		lcdgraf_putc (valueAdjust|0x30,0);
		cfg.weekDay &= ~(1<<1);
		cfg.weekDay |= (valueAdjust<<1);

		valueAdjust=cfg.weekDay&1;
		lcdgraf_gotoxy (19,8);
		lcdgraf_putc (valueAdjust|0x30,1);
	}
	else if (menuPosition==12) 
	{
		lcdgraf_gotoxy (19,8);
		lcdgraf_putc (valueAdjust|0x30,0);
		cfg.weekDay &= ~(1<<0);
		cfg.weekDay |= valueAdjust;
		Write_Flash ();

		return 1;
	}

 	menuPosition++;
	return 0;
}

void Show_InitTimeRep (void)
{
	lcdgraf_clear (page1);

	lcdgraf_gotoxy (1,1);
	lcdgraf_puts ("Para ajustar: 3 e 4  ",0);		 	
	lcdgraf_gotoxy (1,2);
	lcdgraf_puts ("Entrar:6  Cancelar:5 ",0);			 	

	lcdgraf_gotoxy (9,5);
	lcdgraf_putc((cfg.timeHourRep/10)|0x30,1);
	lcdgraf_putc((cfg.timeHourRep%10)|0x30,1);
	lcdgraf_putc(':',0);
	lcdgraf_putc((cfg.timeMinRep/10)|0x30,0);
	lcdgraf_putc((cfg.timeMinRep%10)|0x30,0);

	menuPosition=1;
	valueAdjust=cfg.timeHourRep;
}

void Show_ChangeTimeRep (unsigned char val)
{
	if (val==1)
		valueAdjust++;
	else
		valueAdjust--;

	if (menuPosition==1)	
	{
		if (valueAdjust==24) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=23;
		lcdgraf_gotoxy (9,5);
	}
	else if (menuPosition==2) 
	{
		if (valueAdjust==60) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=59;
		lcdgraf_gotoxy (12,5);	
	}
	lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
	lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
}

unsigned char Save_TimeRep (void)
{
	if (menuPosition==1) 
	{
		lcdgraf_gotoxy (9,5);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,0);
		cfg.timeHourRep = valueAdjust;

		valueAdjust=cfg.timeMinRep;
		lcdgraf_gotoxy (12,5);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
	}
	else if (menuPosition==2) 
	{
		lcdgraf_gotoxy (12,5);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,0);
		cfg.timeMinRep = valueAdjust;
		Write_Flash ();

		return 1;
	}

 	menuPosition++;
	return 0;
}

void Show_InitSaveRep (void)
{
	lcdgraf_clear (page1);

	lcdgraf_gotoxy (1,1);
	lcdgraf_puts ("Para escolher: 3 e 4 ",0);		 	
	lcdgraf_gotoxy (1,2);
	lcdgraf_puts ("Gravar:6  Cancelar:5 ",0);	

	selectedItem = cfg.saveReport+1;

	ChangeMark_SaveRep (selectedItem);
}

void ChangeMark_SaveRep (unsigned char val)
{
	lcdgraf_gotoxy (5,5);
	if (selectedItem==1)
		lcdgraf_puts(" Desabilitado ",1);
	else
		lcdgraf_puts(" Desabilitado ",0);

	lcdgraf_gotoxy (5,6);
	if (selectedItem==2)
		lcdgraf_puts(" Habilitado   ",1);
	else
		lcdgraf_puts(" Habilitado   ",0);
}

void Show_ChangeSaveRep (unsigned char val)
{
	ChangeMark_SaveRep (0);										// Desmarca seleção anterior
	if (val==1)
	{
		selectedItem++;
		if (selectedItem==3)
			selectedItem=1;
	}
	else
	{
		selectedItem--;
		if (selectedItem==0)
			selectedItem=2;
	}
	ChangeMark_SaveRep (1);										// Marca seleção atual
}

void Save_SaveRep (void)
{
	cfg.saveReport=selectedItem-1;
	Write_Flash ();
}

void Show_InitTicket (void)
{
	lcdgraf_clear (page1);

	lcdgraf_gotoxy (1,1);
	lcdgraf_puts ("  Tem certeza de que ",0);		 	
	lcdgraf_gotoxy (1,2);	
	lcdgraf_puts ("  deseja inicializar?",0);		 	

	selectedItem = 1;

	ChangeMark_InitTicket ();
}

void ChangeMark_InitTicket (void)
{
	lcdgraf_gotoxy (7,5);
	if (selectedItem==1)
		lcdgraf_puts("   NAO   ",1);
	else
		lcdgraf_puts("   NAO   ",0);

	lcdgraf_gotoxy (7,6);
	if (selectedItem==2)
		lcdgraf_puts("   SIM   ",1);
	else
		lcdgraf_puts("   SIM   ",0);
}

void Show_ChangeInitTicket (unsigned char val)
{
	ChangeMark_InitTicket ();								// Desmarca seleção anterior
	if (val==1)
	{
		selectedItem++;
		if (selectedItem==3)
			selectedItem=1;
	}
	else
	{
		selectedItem--;
		if (selectedItem==0)
			selectedItem=2;
	}
	ChangeMark_InitTicket ();								// Marca seleção atual
}

unsigned char Save_InitTicket (void)
{
	if (selectedItem==2)
		return 1;
	else 
		return 0;
}

unsigned char Show_TCPIP (void)
{
	lcdgraf_clear (page1);

	lcdgraf_gotoxy (1,1);
	lcdgraf_puts ("Para escolher: 3 e 4 ",0);		 	
	lcdgraf_gotoxy (1,2);
	lcdgraf_puts ("Entrar:6  Cancelar:5 ",0);			 	

	selectedItem=0;	
	ChangeMark_TCPIP ();

	return selectedItem;
}

void ChangeMark_TCPIP (void)
{
	if (selectedItem>=5)
		menuPosition=5;
	else
		menuPosition=0;

	lcdgraf_gotoxy (1,4);
	if (menuPosition<TCPITEMS)
	{
		if (selectedItem==menuPosition)
			lcdgraf_putsconst (MenuTcpIP[menuPosition],1);
		else
			lcdgraf_putsconst (MenuTcpIP[menuPosition],0);
	}
	else
		lcdgraf_puts ("                     ",0);		 	
	

	lcdgraf_gotoxy (1,5);
	if ((menuPosition+1)<TCPITEMS)
	{
		if (selectedItem==(menuPosition+1))
			lcdgraf_putsconst (MenuTcpIP[menuPosition+1],1);
		else
			lcdgraf_putsconst (MenuTcpIP[menuPosition+1],0);
	}
	else
		lcdgraf_puts ("                     ",0);		 	
	
	lcdgraf_gotoxy (1,6);
	if ((menuPosition+2)<TCPITEMS)
	{
		if (selectedItem==(menuPosition+2))
			lcdgraf_putsconst (MenuTcpIP[menuPosition+2],1);
		else
			lcdgraf_putsconst (MenuTcpIP[menuPosition+2],0);
	}
	else
		lcdgraf_puts ("                     ",0);		 	
	
	lcdgraf_gotoxy (1,7);
	if ((menuPosition+3)<TCPITEMS)
	{
		if (selectedItem==(menuPosition+3))
			lcdgraf_putsconst (MenuTcpIP[menuPosition+3],1);
		else
			lcdgraf_putsconst (MenuTcpIP[menuPosition+3],0);
	}
	else
		lcdgraf_puts ("                     ",0);		 	
	
	lcdgraf_gotoxy (1,8);
	if ((menuPosition+4)<TCPITEMS)
	{
		if (selectedItem==(menuPosition+4))
			lcdgraf_putsconst (MenuTcpIP[menuPosition+4],1);
		else
			lcdgraf_putsconst (MenuTcpIP[menuPosition+4],0);
	}
	else
		lcdgraf_puts ("                     ",0);		 	
}

unsigned char Show_ChangeMenuTcpIP (unsigned char val)
{
	if (val==1)
	{
		selectedItem++;
		if (selectedItem==TCPITEMS)
			selectedItem=0;
	}
	else
	{
		selectedItem--;
		if (selectedItem==255)
			selectedItem=TCPITEMS-1;
	}
	ChangeMark_TCPIP ();									// Marca seleção atual

	return selectedItem;
}

void Show_InitIP (void)
{
	lcdgraf_clear (page1);

	lcdgraf_gotoxy (1,1);
	lcdgraf_puts ("Para ajustar: 3 e 4  ",0);		 	
	lcdgraf_gotoxy (1,2);
	lcdgraf_puts ("Entrar:6  Cancelar:5 ",0);			 	

	lcdgraf_gotoxy (4,5);
	if (selectedInterf==ITF_UDP)
	{
		lcdgraf_putc(((LocM.IpAdr[0]/100 )%10)|0x30,1);
		lcdgraf_putc(((LocM.IpAdr[0]/10  )%10)|0x30,1);
		lcdgraf_putc(((LocM.IpAdr[0]     )%10)|0x30,1);
	}
	else
	{
		lcdgraf_putc((((cfg.wifiIp&0xFF)/100 )%10)|0x30,1);
		lcdgraf_putc((((cfg.wifiIp&0xFF)/10  )%10)|0x30,1);
		lcdgraf_putc((((cfg.wifiIp&0xFF)     )%10)|0x30,1);
	}
	lcdgraf_putc('.',0);
	if (selectedInterf==ITF_UDP)
	{
		lcdgraf_putc(((LocM.IpAdr[1]/100 )%10)|0x30,0);
		lcdgraf_putc(((LocM.IpAdr[1]/10  )%10)|0x30,0);
		lcdgraf_putc(((LocM.IpAdr[1]     )%10)|0x30,0);
	}
	else
	{
		lcdgraf_putc(((((cfg.wifiIp>>8)&0xFF)/100 )%10)|0x30,0);
		lcdgraf_putc(((((cfg.wifiIp>>8)&0xFF)/10  )%10)|0x30,0);
		lcdgraf_putc(((((cfg.wifiIp>>8)&0xFF)     )%10)|0x30,0);
	}
	lcdgraf_putc('.',0);
	if (selectedInterf==ITF_UDP)
	{
		lcdgraf_putc(((LocM.IpAdr[2]/100 )%10)|0x30,0);
		lcdgraf_putc(((LocM.IpAdr[2]/10  )%10)|0x30,0);
		lcdgraf_putc(((LocM.IpAdr[2]     )%10)|0x30,0);
	}
	else
	{
		lcdgraf_putc(((((cfg.wifiIp>>16)&0xFF)/100 )%10)|0x30,0);
		lcdgraf_putc(((((cfg.wifiIp>>16)&0xFF)/10  )%10)|0x30,0);
		lcdgraf_putc(((((cfg.wifiIp>>16)&0xFF)     )%10)|0x30,0);
	}
	lcdgraf_putc('.',0);
	if (selectedInterf==ITF_UDP)
	{
		lcdgraf_putc(((LocM.IpAdr[3]/100 )%10)|0x30,0);
		lcdgraf_putc(((LocM.IpAdr[3]/10  )%10)|0x30,0);
		lcdgraf_putc(((LocM.IpAdr[3]     )%10)|0x30,0);
	}
	else
	{
		lcdgraf_putc(((((cfg.wifiIp>>24)&0xFF)/100 )%10)|0x30,0);
		lcdgraf_putc(((((cfg.wifiIp>>24)&0xFF)/10  )%10)|0x30,0);
		lcdgraf_putc(((((cfg.wifiIp>>24)&0xFF)     )%10)|0x30,0);
	}

	menuPosition=1;
	if (selectedInterf==ITF_UDP)
		valueAdjust=LocM.IpAdr[0];
	else
		valueAdjust=cfg.wifiIp&0xFF;
}

void Show_ChangeIP (unsigned char val)
{
	if (val==1)
		valueAdjust++;
	else
		valueAdjust--;

	if (menuPosition==1)	
	{
		if (valueAdjust==256) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=255;
		lcdgraf_gotoxy (4,5);
	}
	else if (menuPosition==2) 
	{
		if (valueAdjust==256) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=255;
		lcdgraf_gotoxy (8,5);	
	}
	else if (menuPosition==3) 
	{
		if (valueAdjust==256) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=255;		
		lcdgraf_gotoxy (12,5);
	}
	else if (menuPosition==4) 
	{
		if (valueAdjust==256) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=255;		
		lcdgraf_gotoxy (16,5);
	}
	lcdgraf_putc(((valueAdjust/100 )%10)|0x30,1);
	lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
	lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
}

unsigned char Save_IP (void)
{
	if (menuPosition==1) 
	{
		lcdgraf_gotoxy (4,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,0);
		if (selectedInterf==ITF_UDP)
		{
			cfg.localIp[0] = valueAdjust;						// Salva 1º hexa
			if (cfg.dhcpStatus==0)			 					// Somente se DHCP desligado,
				LocM.IpAdr[0]=cfg.localIp[0];					// muda endereço IP.
			valueAdjust=LocM.IpAdr[1];		 					// Carrega 2º hexa para ajuste
		}
		else
		{
			cfg.wifiIp &= ~0xFF;
			cfg.wifiIp |= valueAdjust;							// Salva 1º hexa
			valueAdjust=((cfg.wifiIp>>8)&0xFF);					// Carrega 2º hexa para ajuste
		}
		lcdgraf_gotoxy (8,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
	}
	else if (menuPosition==2) 
	{
		lcdgraf_gotoxy (8,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,0);
		if (selectedInterf==ITF_UDP)
		{
			cfg.localIp[1] = valueAdjust;						// Salva 2º hexa
			if (cfg.dhcpStatus==0)								// Somente se DHCP desligado,
				LocM.IpAdr[1]=cfg.localIp[1];					// muda endereço IP.
			valueAdjust=LocM.IpAdr[2];							// Carrega 3º hexa para ajuste
		}
		else
		{
			cfg.wifiIp &= 0xFFFF00FF;
			cfg.wifiIp |= (valueAdjust<<8);						// Salva 2º hexa
			valueAdjust=((cfg.wifiIp>>16)&0xFF);				// Carrega 3º hexa para ajuste
		}
		lcdgraf_gotoxy (12,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
	}
	else if (menuPosition==3) 
	{
		lcdgraf_gotoxy (12,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,0);
		if (selectedInterf==ITF_UDP)
		{
			cfg.localIp[2] = valueAdjust;						// Salva 3º hexa
			if (cfg.dhcpStatus==0)								// Somente se DHCP desligado,
				LocM.IpAdr[2]=cfg.localIp[2];					// muda endereço IP.
			valueAdjust=LocM.IpAdr[3];							// Carrega 4º hexa para ajuste
		}
		else
		{
			cfg.wifiIp &= 0xFF00FFFF;
			cfg.wifiIp |= (valueAdjust<<16);					// Salva 3º hexa
			valueAdjust=((cfg.wifiIp>>24)&0xFF);				// Carrega 4º hexa para ajuste
		}
		lcdgraf_gotoxy (16,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
	}
	else if (menuPosition==4) 
	{
		if (selectedInterf==ITF_UDP)
		{
			cfg.localIp[3] = valueAdjust;						// Salva 4º hexa
			if (cfg.dhcpStatus==0)								// Somente se DHCP desligado,
				LocM.IpAdr[3]=cfg.localIp[3];					// muda endereço IP.
			Write_Flash ();
		}
		else
		{
			cfg.wifiIp &= 0x00FFFFFF;
			cfg.wifiIp |= (valueAdjust<<24);					// Salva 4º hexa
			Write_Flash ();
		}
		return 1;
	}

 	menuPosition++;
	return 0;
}

void Show_InitGate (void)
{
	lcdgraf_clear (page1);

	lcdgraf_gotoxy (1,1);
	lcdgraf_puts ("PARA AJUSTAR: 3 e 4  ",0);		 	
	lcdgraf_gotoxy (1,2);
	lcdgraf_puts ("ENTRAR:6  CANCELAR:5 ",0);			 	

	lcdgraf_gotoxy (4,5);
	if (selectedInterf==ITF_UDP)
	{
		lcdgraf_putc(((LocM.DefGW[0]/100 )%10)|0x30,1);
		lcdgraf_putc(((LocM.DefGW[0]/10  )%10)|0x30,1);
		lcdgraf_putc(((LocM.DefGW[0]     )%10)|0x30,1);
	}
	else
	{
		lcdgraf_putc((((cfg.wifiGateway&0xFF)/100 )%10)|0x30,1);
		lcdgraf_putc((((cfg.wifiGateway&0xFF)/10  )%10)|0x30,1);
		lcdgraf_putc((((cfg.wifiGateway&0xFF)     )%10)|0x30,1);
	}
	lcdgraf_putc('.',0);
	if (selectedInterf==ITF_UDP)
	{
		lcdgraf_putc(((LocM.DefGW[1]/100 )%10)|0x30,0);
		lcdgraf_putc(((LocM.DefGW[1]/10  )%10)|0x30,0);
		lcdgraf_putc(((LocM.DefGW[1]     )%10)|0x30,0);
	}
	else
	{
		lcdgraf_putc(((((cfg.wifiGateway>>8)&0xFF)/100 )%10)|0x30,0);
		lcdgraf_putc(((((cfg.wifiGateway>>8)&0xFF)/10  )%10)|0x30,0);
		lcdgraf_putc(((((cfg.wifiGateway>>8)&0xFF)     )%10)|0x30,0);
	}
	lcdgraf_putc('.',0);
	if (selectedInterf==ITF_UDP)
	{
		lcdgraf_putc(((LocM.DefGW[2]/100 )%10)|0x30,0);
		lcdgraf_putc(((LocM.DefGW[2]/10  )%10)|0x30,0);
		lcdgraf_putc(((LocM.DefGW[2]     )%10)|0x30,0);
	}
	else
	{
		lcdgraf_putc(((((cfg.wifiGateway>>16)&0xFF)/100 )%10)|0x30,0);
		lcdgraf_putc(((((cfg.wifiGateway>>16)&0xFF)/10  )%10)|0x30,0);
		lcdgraf_putc(((((cfg.wifiGateway>>16)&0xFF)     )%10)|0x30,0);
	}
	lcdgraf_putc('.',0);
	if (selectedInterf==ITF_UDP)
	{
		lcdgraf_putc(((LocM.DefGW[3]/100 )%10)|0x30,0);
		lcdgraf_putc(((LocM.DefGW[3]/10  )%10)|0x30,0);
		lcdgraf_putc(((LocM.DefGW[3]     )%10)|0x30,0);
	}
	else
	{
		lcdgraf_putc(((((cfg.wifiGateway>>24)&0xFF)/100 )%10)|0x30,0);
		lcdgraf_putc(((((cfg.wifiGateway>>24)&0xFF)/10  )%10)|0x30,0);
		lcdgraf_putc(((((cfg.wifiGateway>>24)&0xFF)     )%10)|0x30,0);
	}

	menuPosition=1;
	if (selectedInterf==ITF_UDP)
		valueAdjust=LocM.DefGW[0];
	else
		valueAdjust=cfg.wifiGateway&0xFF;
}

void Show_ChangeGate (unsigned char val)
{
	if (val==1)
		valueAdjust++;
	else
		valueAdjust--;

	if (menuPosition==1)	
	{
		if (valueAdjust==256) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=255;
		lcdgraf_gotoxy (4,5);
	}
	else if (menuPosition==2) 
	{
		if (valueAdjust==256) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=255;
		lcdgraf_gotoxy (8,5);	
	}
	else if (menuPosition==3) 
	{
		if (valueAdjust==256) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=255;		
		lcdgraf_gotoxy (12,5);
	}
	else if (menuPosition==4) 
	{
		if (valueAdjust==256) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=255;		
		lcdgraf_gotoxy (16,5);
	}
	lcdgraf_putc(((valueAdjust/100 )%10)|0x30,1);
	lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
	lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
}

unsigned char Save_Gate (void)
{
	if (menuPosition==1) 
	{
		lcdgraf_gotoxy (4,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,0);
		if (selectedInterf==ITF_UDP)
		{
			LocM.DefGW[0]=cfg.gateway[0]=valueAdjust;			// Salva 1º hexa
			valueAdjust=LocM.DefGW[1];		 					// Carrega 2º hexa para ajuste
		}
		else
		{
			cfg.wifiGateway &= ~0xFF;
			cfg.wifiGateway |= valueAdjust;						// Salva 1º hexa
			valueAdjust=((cfg.wifiGateway>>8)&0xFF);			// Carrega 2º hexa para ajuste
		}
		lcdgraf_gotoxy (8,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
	}
	else if (menuPosition==2) 
	{
		lcdgraf_gotoxy (8,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,0);
		if (selectedInterf==ITF_UDP)
		{
			LocM.DefGW[1]=cfg.gateway[1]=valueAdjust;			// Salva 2º hexa
			valueAdjust=LocM.DefGW[2];							// Carrega 3º hexa para ajuste
		}
		else
		{
			cfg.wifiGateway &= 0xFFFF00FF;
			cfg.wifiGateway |= (valueAdjust<<8);				// Salva 2º hexa
			valueAdjust=((cfg.wifiGateway>>16)&0xFF);			// Carrega 3º hexa para ajuste
		}
		lcdgraf_gotoxy (12,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
	}
	else if (menuPosition==3) 
	{
		lcdgraf_gotoxy (12,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,0);
		if (selectedInterf==ITF_UDP)
		{
			LocM.DefGW[2]=cfg.gateway[2]=valueAdjust;			// Salva 3º hexa
			valueAdjust=LocM.DefGW[3];							// Carrega 4º hexa para ajuste
		}
		else
		{
			cfg.wifiGateway &= 0xFF00FFFF;
			cfg.wifiGateway |= (valueAdjust<<16);				// Salva 3º hexa
			valueAdjust=((cfg.wifiGateway>>24)&0xFF);			// Carrega 4º hexa para ajuste
		}
		lcdgraf_gotoxy (16,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
	}
	else if (menuPosition==4) 
	{
		if (selectedInterf==ITF_UDP)
		{
			LocM.DefGW[3]=cfg.gateway[3]=valueAdjust;			// Salva 4º hexa
			Write_Flash ();
		}
		else
		{
			cfg.wifiGateway &= 0x00FFFFFF;
			cfg.wifiGateway |= (valueAdjust<<24);				// Salva 4º hexa
			Write_Flash ();
		}
		return 1;
	}

 	menuPosition++;
	return 0;
}

void Show_InitDns1 (void)
{
	lcdgraf_clear (page1);

	lcdgraf_gotoxy (1,1);
	lcdgraf_puts ("PARA AJUSTAR: 3 e 4  ",0);		 	
	lcdgraf_gotoxy (1,2);
	lcdgraf_puts ("ENTRAR:6  CANCELAR:5 ",0);			 	

	lcdgraf_gotoxy (4,5);
	if (selectedInterf==ITF_UDP)
	{
		lcdgraf_putc(((LocM.PriDNS[0]/100 )%10)|0x30,1);
		lcdgraf_putc(((LocM.PriDNS[0]/10  )%10)|0x30,1);
		lcdgraf_putc(((LocM.PriDNS[0]     )%10)|0x30,1);
	}
	else
	{
		lcdgraf_putc((((cfg.wifiDns1&0xFF)/100 )%10)|0x30,1);
		lcdgraf_putc((((cfg.wifiDns1&0xFF)/10  )%10)|0x30,1);
		lcdgraf_putc((((cfg.wifiDns1&0xFF)     )%10)|0x30,1);
	}
	lcdgraf_putc('.',0);
	if (selectedInterf==ITF_UDP)
	{
		lcdgraf_putc(((LocM.PriDNS[1]/100 )%10)|0x30,0);
		lcdgraf_putc(((LocM.PriDNS[1]/10  )%10)|0x30,0);
		lcdgraf_putc(((LocM.PriDNS[1]     )%10)|0x30,0);
	}
	else
	{
		lcdgraf_putc(((((cfg.wifiDns1>>8)&0xFF)/100 )%10)|0x30,0);
		lcdgraf_putc(((((cfg.wifiDns1>>8)&0xFF)/10  )%10)|0x30,0);
		lcdgraf_putc(((((cfg.wifiDns1>>8)&0xFF)     )%10)|0x30,0);
	}
	lcdgraf_putc('.',0);
	if (selectedInterf==ITF_UDP)
	{
		lcdgraf_putc(((LocM.PriDNS[2]/100 )%10)|0x30,0);
		lcdgraf_putc(((LocM.PriDNS[2]/10  )%10)|0x30,0);
		lcdgraf_putc(((LocM.PriDNS[2]     )%10)|0x30,0);
	}
	else
	{
		lcdgraf_putc(((((cfg.wifiDns1>>16)&0xFF)/100 )%10)|0x30,0);
		lcdgraf_putc(((((cfg.wifiDns1>>16)&0xFF)/10  )%10)|0x30,0);
		lcdgraf_putc(((((cfg.wifiDns1>>16)&0xFF)     )%10)|0x30,0);
	}
	lcdgraf_putc('.',0);
	if (selectedInterf==ITF_UDP)
	{
		lcdgraf_putc(((LocM.PriDNS[3]/100 )%10)|0x30,0);
		lcdgraf_putc(((LocM.PriDNS[3]/10  )%10)|0x30,0);
		lcdgraf_putc(((LocM.PriDNS[3]     )%10)|0x30,0);
	}
	else
	{
		lcdgraf_putc(((((cfg.wifiDns1>>24)&0xFF)/100 )%10)|0x30,0);
		lcdgraf_putc(((((cfg.wifiDns1>>24)&0xFF)/10  )%10)|0x30,0);
		lcdgraf_putc(((((cfg.wifiDns1>>24)&0xFF)     )%10)|0x30,0);
	}

	menuPosition=1;
	if (selectedInterf==ITF_UDP)
		valueAdjust=LocM.PriDNS[0];
	else
		valueAdjust=cfg.wifiDns1&0xFF;
}

void Show_ChangeDns1 (unsigned char val)
{
	if (val==1)
		valueAdjust++;
	else
		valueAdjust--;

	if (menuPosition==1)	
	{
		if (valueAdjust==256) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=255;
		lcdgraf_gotoxy (4,5);
	}
	else if (menuPosition==2) 
	{
		if (valueAdjust==256) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=255;
		lcdgraf_gotoxy (8,5);	
	}
	else if (menuPosition==3) 
	{
		if (valueAdjust==256) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=255;		
		lcdgraf_gotoxy (12,5);
	}
	else if (menuPosition==4) 
	{
		if (valueAdjust==256) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=255;		
		lcdgraf_gotoxy (16,5);
	}
	lcdgraf_putc(((valueAdjust/100 )%10)|0x30,1);
	lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
	lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
}

unsigned char Save_Dns1 (void)
{
	if (menuPosition==1) 
	{
		lcdgraf_gotoxy (4,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,0);
		if (selectedInterf==ITF_UDP)
		{
			cfg.dns1[0] = valueAdjust;							// Salva 1º hexa
			if (cfg.dhcpStatus==0)			 					// Somente se DHCP desligado,
				LocM.PriDNS[0]=cfg.dns1[0];						// muda endereço IP.
			valueAdjust=LocM.PriDNS[1];		 					// Carrega 2º hexa para ajuste
		}
		else
		{
			cfg.wifiDns1 &= ~0xFF;
			cfg.wifiDns1 |= valueAdjust;						// Salva 1º hexa
			valueAdjust=((cfg.wifiDns1>>8)&0xFF);				// Carrega 2º hexa para ajuste
		}
		lcdgraf_gotoxy (8,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
	}
	else if (menuPosition==2) 
	{
		lcdgraf_gotoxy (8,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,0);
		if (selectedInterf==ITF_UDP)
		{
			cfg.dns1[1] = valueAdjust;							// Salva 2º hexa
			if (cfg.dhcpStatus==0)								// Somente se DHCP desligado,
				LocM.PriDNS[1]=cfg.dns1[1];						// muda endereço IP.
			valueAdjust=LocM.PriDNS[2];							// Carrega 3º hexa para ajuste
		}
		else
		{
			cfg.wifiDns1 &= 0xFFFF00FF;
			cfg.wifiDns1 |= (valueAdjust<<8);					// Salva 2º hexa
			valueAdjust=((cfg.wifiDns1>>16)&0xFF);				// Carrega 3º hexa para ajuste
		}
		lcdgraf_gotoxy (12,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
	}
	else if (menuPosition==3) 
	{
		lcdgraf_gotoxy (12,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,0);
		if (selectedInterf==ITF_UDP)
		{
			cfg.dns1[2] = valueAdjust;							// Salva 3º hexa
			if (cfg.dhcpStatus==0)								// Somente se DHCP desligado,
				LocM.PriDNS[2]=cfg.dns1[2];						// muda endereço IP.
			valueAdjust=LocM.PriDNS[3];							// Carrega 4º hexa para ajuste
		}
		else
		{
			cfg.wifiDns1 &= 0xFF00FFFF;
			cfg.wifiDns1 |= (valueAdjust<<16);					// Salva 3º hexa
			valueAdjust=((cfg.wifiDns1>>24)&0xFF);				// Carrega 4º hexa para ajuste
		}
		lcdgraf_gotoxy (16,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
	}
	else if (menuPosition==4) 
	{
		if (selectedInterf==ITF_UDP)
		{
			cfg.dns1[3] = valueAdjust;							// Salva 4º hexa
			if (cfg.dhcpStatus==0)								// Somente se DHCP desligado,
				LocM.PriDNS[3]=cfg.dns1[3];						// muda endereço IP.
			Write_Flash ();
		}
		else
		{
			cfg.wifiDns1 &= 0x00FFFFFF;
			cfg.wifiDns1 |= (valueAdjust<<24);					// Salva 4º hexa
			Write_Flash ();
		}
		return 1;
	}

 	menuPosition++;
	return 0;
}

void Show_InitDns2 (void)
{
	lcdgraf_clear (page1);

	lcdgraf_gotoxy (1,1);
	lcdgraf_puts ("PARA AJUSTAR: 3 e 4  ",0);		 	
	lcdgraf_gotoxy (1,2);
	lcdgraf_puts ("ENTRAR:6  CANCELAR:5 ",0);			 	

	lcdgraf_gotoxy (4,5);
	if (selectedInterf==ITF_UDP)
	{
		lcdgraf_putc(((LocM.SecDNS[0]/100 )%10)|0x30,1);
		lcdgraf_putc(((LocM.SecDNS[0]/10  )%10)|0x30,1);
		lcdgraf_putc(((LocM.SecDNS[0]     )%10)|0x30,1);
	}
	else
	{
		lcdgraf_putc((((cfg.wifiDns2&0xFF)/100 )%10)|0x30,1);
		lcdgraf_putc((((cfg.wifiDns2&0xFF)/10  )%10)|0x30,1);
		lcdgraf_putc((((cfg.wifiDns2&0xFF)     )%10)|0x30,1);
	}
	lcdgraf_putc('.',0);
	if (selectedInterf==ITF_UDP)
	{
		lcdgraf_putc(((LocM.SecDNS[1]/100 )%10)|0x30,0);
		lcdgraf_putc(((LocM.SecDNS[1]/10  )%10)|0x30,0);
		lcdgraf_putc(((LocM.SecDNS[1]     )%10)|0x30,0);
	}
	else
	{
		lcdgraf_putc(((((cfg.wifiDns2>>8)&0xFF)/100 )%10)|0x30,0);
		lcdgraf_putc(((((cfg.wifiDns2>>8)&0xFF)/10  )%10)|0x30,0);
		lcdgraf_putc(((((cfg.wifiDns2>>8)&0xFF)     )%10)|0x30,0);
	}
	lcdgraf_putc('.',0);
	if (selectedInterf==ITF_UDP)
	{
		lcdgraf_putc(((LocM.SecDNS[2]/100 )%10)|0x30,0);
		lcdgraf_putc(((LocM.SecDNS[2]/10  )%10)|0x30,0);
		lcdgraf_putc(((LocM.SecDNS[2]     )%10)|0x30,0);
	}
	else
	{
		lcdgraf_putc(((((cfg.wifiDns2>>16)&0xFF)/100 )%10)|0x30,0);
		lcdgraf_putc(((((cfg.wifiDns2>>16)&0xFF)/10  )%10)|0x30,0);
		lcdgraf_putc(((((cfg.wifiDns2>>16)&0xFF)     )%10)|0x30,0);
	}
	lcdgraf_putc('.',0);
	if (selectedInterf==ITF_UDP)
	{
		lcdgraf_putc(((LocM.SecDNS[3]/100 )%10)|0x30,0);
		lcdgraf_putc(((LocM.SecDNS[3]/10  )%10)|0x30,0);
		lcdgraf_putc(((LocM.SecDNS[3]     )%10)|0x30,0);
	}
	else
	{
		lcdgraf_putc(((((cfg.wifiDns2>>24)&0xFF)/100 )%10)|0x30,0);
		lcdgraf_putc(((((cfg.wifiDns2>>24)&0xFF)/10  )%10)|0x30,0);
		lcdgraf_putc(((((cfg.wifiDns2>>24)&0xFF)     )%10)|0x30,0);
	}

	menuPosition=1;
	if (selectedInterf==ITF_UDP)
		valueAdjust=LocM.SecDNS[0];
	else
		valueAdjust=cfg.wifiDns2&0xFF;
}

void Show_ChangeDns2 (unsigned char val)
{
	if (val==1)
		valueAdjust++;
	else
		valueAdjust--;

	if (menuPosition==1)	
	{
		if (valueAdjust==256) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=255;
		lcdgraf_gotoxy (4,5);
	}
	else if (menuPosition==2) 
	{
		if (valueAdjust==256) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=255;
		lcdgraf_gotoxy (8,5);	
	}
	else if (menuPosition==3) 
	{
		if (valueAdjust==256) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=255;		
		lcdgraf_gotoxy (12,5);
	}
	else if (menuPosition==4) 
	{
		if (valueAdjust==256) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=255;		
		lcdgraf_gotoxy (16,5);
	}
	lcdgraf_putc(((valueAdjust/100 )%10)|0x30,1);
	lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
	lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
}

unsigned char Save_Dns2 (void)
{
	if (menuPosition==1) 
	{
		lcdgraf_gotoxy (4,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,0);
		if (selectedInterf==ITF_UDP)
		{
			cfg.dns2[0] = valueAdjust;							// Salva 1º hexa
			if (cfg.dhcpStatus==0)			 					// Somente se DHCP desligado,
				LocM.SecDNS[0]=cfg.dns2[0];						// muda endereço IP.
			valueAdjust=LocM.SecDNS[1];		 					// Carrega 2º hexa para ajuste
		}
		else
		{
			cfg.wifiDns2 &= ~0xFF;
			cfg.wifiDns2 |= valueAdjust;						// Salva 1º hexa
			valueAdjust=((cfg.wifiDns2>>8)&0xFF);				// Carrega 2º hexa para ajuste
		}
		lcdgraf_gotoxy (8,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
	}
	else if (menuPosition==2) 
	{
		lcdgraf_gotoxy (8,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,0);
		if (selectedInterf==ITF_UDP)
		{
			cfg.dns2[1] = valueAdjust;							// Salva 2º hexa
			if (cfg.dhcpStatus==0)								// Somente se DHCP desligado,
				LocM.SecDNS[1]=cfg.dns2[1];						// muda endereço IP.
			valueAdjust=LocM.SecDNS[2];							// Carrega 3º hexa para ajuste
		}
		else
		{
			cfg.wifiDns2 &= 0xFFFF00FF;
			cfg.wifiDns2 |= (valueAdjust<<8);					// Salva 2º hexa
			valueAdjust=((cfg.wifiDns2>>16)&0xFF);				// Carrega 3º hexa para ajuste
		}
		lcdgraf_gotoxy (12,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
	}
	else if (menuPosition==3) 
	{
		lcdgraf_gotoxy (12,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,0);
		if (selectedInterf==ITF_UDP)
		{
			cfg.dns2[2] = valueAdjust;							// Salva 3º hexa
			if (cfg.dhcpStatus==0)								// Somente se DHCP desligado,
				LocM.SecDNS[2]=cfg.dns2[2];						// muda endereço IP.
			valueAdjust=LocM.SecDNS[3];							// Carrega 4º hexa para ajuste
		}
		else
		{
			cfg.wifiDns2 &= 0xFF00FFFF;
			cfg.wifiDns2 |= (valueAdjust<<16);					// Salva 3º hexa
			valueAdjust=((cfg.wifiDns2>>24)&0xFF);				// Carrega 4º hexa para ajuste
		}
		lcdgraf_gotoxy (16,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
	}
	else if (menuPosition==4) 
	{
		if (selectedInterf==ITF_UDP)
		{
			cfg.dns2[3] = valueAdjust;							// Salva 4º hexa
			if (cfg.dhcpStatus==0)								// Somente se DHCP desligado,
				LocM.SecDNS[3]=cfg.dns2[3];						// muda endereço IP.
			Write_Flash ();
		}
		else
		{
			cfg.wifiDns2 &= 0x00FFFFFF;
			cfg.wifiDns2 |= (valueAdjust<<24);					// Salva 4º hexa
			Write_Flash ();
		}
		return 1;
	}

 	menuPosition++;
	return 0;
}

void Show_InitMask (void)
{
	lcdgraf_clear (page1);

	lcdgraf_gotoxy (1,1);
	lcdgraf_puts ("PARA AJUSTAR: 3 e 4  ",0);		 	
	lcdgraf_gotoxy (1,2);
	lcdgraf_puts ("ENTRAR:6  CANCELAR:5 ",0);			 	

	lcdgraf_gotoxy (4,5);
	if (selectedInterf==ITF_UDP)
	{
		lcdgraf_putc(((LocM.NetMask[0]/100 )%10)|0x30,1);
		lcdgraf_putc(((LocM.NetMask[0]/10  )%10)|0x30,1);
		lcdgraf_putc(((LocM.NetMask[0]     )%10)|0x30,1);
	}
	else
	{
		lcdgraf_putc((((cfg.wifiMask&0xFF)/100 )%10)|0x30,1);
		lcdgraf_putc((((cfg.wifiMask&0xFF)/10  )%10)|0x30,1);
		lcdgraf_putc((((cfg.wifiMask&0xFF)     )%10)|0x30,1);
	}
	lcdgraf_putc('.',0);
	if (selectedInterf==ITF_UDP)
	{
		lcdgraf_putc(((LocM.NetMask[1]/100 )%10)|0x30,0);
		lcdgraf_putc(((LocM.NetMask[1]/10  )%10)|0x30,0);
		lcdgraf_putc(((LocM.NetMask[1]     )%10)|0x30,0);
	}
	else
	{
		lcdgraf_putc(((((cfg.wifiMask>>8)&0xFF)/100 )%10)|0x30,0);
		lcdgraf_putc(((((cfg.wifiMask>>8)&0xFF)/10  )%10)|0x30,0);
		lcdgraf_putc(((((cfg.wifiMask>>8)&0xFF)     )%10)|0x30,0);
	}
	lcdgraf_putc('.',0);
	if (selectedInterf==ITF_UDP)
	{
		lcdgraf_putc(((LocM.NetMask[2]/100 )%10)|0x30,0);
		lcdgraf_putc(((LocM.NetMask[2]/10  )%10)|0x30,0);
		lcdgraf_putc(((LocM.NetMask[2]     )%10)|0x30,0);
	}
	else
	{
		lcdgraf_putc(((((cfg.wifiMask>>16)&0xFF)/100 )%10)|0x30,0);
		lcdgraf_putc(((((cfg.wifiMask>>16)&0xFF)/10  )%10)|0x30,0);
		lcdgraf_putc(((((cfg.wifiMask>>16)&0xFF)     )%10)|0x30,0);
	}
	lcdgraf_putc('.',0);
	if (selectedInterf==ITF_UDP)
	{
		lcdgraf_putc(((LocM.NetMask[3]/100 )%10)|0x30,0);
		lcdgraf_putc(((LocM.NetMask[3]/10  )%10)|0x30,0);
		lcdgraf_putc(((LocM.NetMask[3]     )%10)|0x30,0);
	}
	else
	{
		lcdgraf_putc(((((cfg.wifiMask>>24)&0xFF)/100 )%10)|0x30,0);
		lcdgraf_putc(((((cfg.wifiMask>>24)&0xFF)/10  )%10)|0x30,0);
		lcdgraf_putc(((((cfg.wifiMask>>24)&0xFF)     )%10)|0x30,0);
	}

	menuPosition=1;
	if (selectedInterf==ITF_UDP)
		valueAdjust=LocM.NetMask[0];
	else
		valueAdjust=cfg.wifiMask&0xFF;
}

void Show_ChangeMask (unsigned char val)
{
	if (val==1)
		valueAdjust++;
	else
		valueAdjust--;

	if (menuPosition==1)
	{
		if (valueAdjust==256) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=255;
		lcdgraf_gotoxy (4,5);
	}
	else if (menuPosition==2) 
	{
		if (valueAdjust==256) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=255;
		lcdgraf_gotoxy (8,5);	
	}
	else if (menuPosition==3) 
	{
		if (valueAdjust==256) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=255;		
		lcdgraf_gotoxy (12,5);
	}
	else if (menuPosition==4) 
	{
		if (valueAdjust==256) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=255;		
		lcdgraf_gotoxy (16,5);
	}
	lcdgraf_putc(((valueAdjust/100 )%10)|0x30,1);
	lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
	lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
}

unsigned char Save_Mask (void)
{
	if (menuPosition==1) 
	{
		lcdgraf_gotoxy (4,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,0);
		if (selectedInterf==ITF_UDP)
		{
			cfg.ipMask[0] = valueAdjust;						// Salva 1º hexa
			if (cfg.dhcpStatus==0)								// Somente se DHCP desligado,
				LocM.NetMask[0]=cfg.ipMask[0];					// muda endereço IP.
			valueAdjust=LocM.NetMask[1];						// Carrega 2º hexa para ajuste
		}
		else
		{
			cfg.wifiMask &= ~0xFF;
			cfg.wifiMask |= valueAdjust;						// Salva 1º hexa
			valueAdjust=((cfg.wifiMask>>8)&0xFF);				// Carrega 2º hexa para ajuste
		}
		lcdgraf_gotoxy (8,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
	}
	else if (menuPosition==2) 
	{
		lcdgraf_gotoxy (8,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,0);
		if (selectedInterf==ITF_UDP)
		{
			cfg.ipMask[1] = valueAdjust;						// Salva 2º hexa
			if (cfg.dhcpStatus==0)								// Somente se DHCP desligado,
				LocM.NetMask[1]=cfg.ipMask[1];					// muda endereço IP.
			valueAdjust=LocM.NetMask[2];						// Carrega 3º hexa para ajuste
		}
		else
		{
			cfg.wifiMask &= 0xFFFF00FF;
			cfg.wifiMask |= (valueAdjust<<8);					// Salva 2º hexa
			valueAdjust=((cfg.wifiMask>>16)&0xFF);				// Carrega 3º hexa para ajuste
		}
		lcdgraf_gotoxy (12,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
	}
	else if (menuPosition==3) 
	{
		lcdgraf_gotoxy (12,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,0);
		if (selectedInterf==ITF_UDP)
		{
			cfg.ipMask[2] = valueAdjust;						// Salva 3º hexa
			if (cfg.dhcpStatus==0)								// Somente se DHCP desligado,
				LocM.NetMask[2]=cfg.ipMask[2];					// muda endereço IP.
			valueAdjust=LocM.NetMask[3];						// Carrega 4º hexa para ajuste
		}
		else
		{
			cfg.wifiMask &= 0xFF00FFFF;
			cfg.wifiMask |= (valueAdjust<<16);					// Salva 3º hexa
			valueAdjust=((cfg.wifiMask>>24)&0xFF);				// Carrega 4º hexa para ajuste
		}
		lcdgraf_gotoxy (16,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
	}
	else if (menuPosition==4) 
	{
		if (selectedInterf==ITF_UDP)
		{
			cfg.ipMask[3] = valueAdjust;						// Salva 4º hexa
			if (cfg.dhcpStatus==0)								// Somente se DHCP desligado,
				LocM.NetMask[3]=cfg.ipMask[3];					// muda endereço IP.
			Write_Flash ();
		}
		else
		{
			cfg.wifiMask &= 0x00FFFFFF;
			cfg.wifiMask |= (valueAdjust<<24);					// Salva 4º hexa
		}

		return 1;
	}

 	menuPosition++;
	return 0;
}

void Show_InitMAC (void)
{
	lcdgraf_clear (page1);

	lcdgraf_gotoxy (1,1);
	lcdgraf_puts ("PARA AJUSTAR: 3 e 4  ",0);		 	
	lcdgraf_gotoxy (1,2);
	lcdgraf_puts ("ENTRAR:6  CANCELAR:5 ",0);			 	

	lcdgraf_gotoxy (6,5);
	if (selectedInterf==ITF_UDP)
	{
		lcdgraf_putc(((own_hw_adr[0]/100 )%10)|0x30,1);
		lcdgraf_putc(((own_hw_adr[0]/10  )%10)|0x30,1);
		lcdgraf_putc(((own_hw_adr[0]     )%10)|0x30,1);
	}
	else
	{
		lcdgraf_putc(((wifiMac[0]/100 )%10)|0x30,1);
		lcdgraf_putc(((wifiMac[0]/10  )%10)|0x30,1);
		lcdgraf_putc(((wifiMac[0]     )%10)|0x30,1);
	}
	lcdgraf_putc('.',0);
	if (selectedInterf==ITF_UDP)
	{
		lcdgraf_putc(((own_hw_adr[1]/100 )%10)|0x30,0);
		lcdgraf_putc(((own_hw_adr[1]/10  )%10)|0x30,0);
		lcdgraf_putc(((own_hw_adr[1]     )%10)|0x30,0);
	}
	else
	{
		lcdgraf_putc(((wifiMac[1]/100 )%10)|0x30,0);
		lcdgraf_putc(((wifiMac[1]/10  )%10)|0x30,0);
		lcdgraf_putc(((wifiMac[1]     )%10)|0x30,0);
	}
	lcdgraf_putc('.',0);
	if (selectedInterf==ITF_UDP)
	{
		lcdgraf_putc(((own_hw_adr[2]/100 )%10)|0x30,0);
		lcdgraf_putc(((own_hw_adr[2]/10  )%10)|0x30,0);
		lcdgraf_putc(((own_hw_adr[2]     )%10)|0x30,0);
	}
	else
	{
		lcdgraf_putc(((wifiMac[2]/100 )%10)|0x30,0);
		lcdgraf_putc(((wifiMac[2]/10  )%10)|0x30,0);
		lcdgraf_putc(((wifiMac[2]     )%10)|0x30,0);
	}
	lcdgraf_putc('.',0);
	lcdgraf_gotoxy (6,6);
	if (selectedInterf==ITF_UDP)
	{
		lcdgraf_putc(((own_hw_adr[3]/100 )%10)|0x30,0);
		lcdgraf_putc(((own_hw_adr[3]/10  )%10)|0x30,0);
		lcdgraf_putc(((own_hw_adr[3]     )%10)|0x30,0);
	}
	else
	{
		lcdgraf_putc(((wifiMac[3]/100 )%10)|0x30,0);
		lcdgraf_putc(((wifiMac[3]/10  )%10)|0x30,0);
		lcdgraf_putc(((wifiMac[3]     )%10)|0x30,0);
	}
	lcdgraf_putc('.',0);
	if (selectedInterf==ITF_UDP)
	{
		lcdgraf_putc(((own_hw_adr[4]/100 )%10)|0x30,0);
		lcdgraf_putc(((own_hw_adr[4]/10  )%10)|0x30,0);
		lcdgraf_putc(((own_hw_adr[4]     )%10)|0x30,0);
	}
	else
	{
		lcdgraf_putc(((wifiMac[4]/100 )%10)|0x30,0);
		lcdgraf_putc(((wifiMac[4]/10  )%10)|0x30,0);
		lcdgraf_putc(((wifiMac[4]     )%10)|0x30,0);
	}
	lcdgraf_putc('.',0);
	if (selectedInterf==ITF_UDP)
	{
		lcdgraf_putc(((own_hw_adr[5]/100 )%10)|0x30,0);
		lcdgraf_putc(((own_hw_adr[5]/10  )%10)|0x30,0);
		lcdgraf_putc(((own_hw_adr[5]     )%10)|0x30,0);
	}
	else
	{
		lcdgraf_putc(((wifiMac[5]/100 )%10)|0x30,0);
		lcdgraf_putc(((wifiMac[5]/10  )%10)|0x30,0);
		lcdgraf_putc(((wifiMac[5]     )%10)|0x30,0);
	}

	menuPosition=1;
	if (selectedInterf==ITF_UDP)
		valueAdjust=own_hw_adr[0];
	else
		valueAdjust=wifiMac[0];
}

void Show_ChangeMAC (unsigned char val)
{
	if (val==1)
		valueAdjust++;
	else
		valueAdjust--;

	if (menuPosition==1)	
	{
		if (valueAdjust==256) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=255;
		lcdgraf_gotoxy (6,5);
	}
	else if (menuPosition==2) 
	{
		if (valueAdjust==256) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=255;
		lcdgraf_gotoxy (10,5);	
	}
	else if (menuPosition==3) 
	{
		if (valueAdjust==256) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=255;		
		lcdgraf_gotoxy (14,5);
	}
	else if (menuPosition==4) 
	{
		if (valueAdjust==256) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=255;		
		lcdgraf_gotoxy (6,6);
	}
	else if (menuPosition==5) 
	{
		if (valueAdjust==256) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=255;		
		lcdgraf_gotoxy (10,6);
	}
	else if (menuPosition==6) 
	{
		if (valueAdjust==256) valueAdjust=0;
		if (valueAdjust==0xFFFFFFFF) valueAdjust=255;		
		lcdgraf_gotoxy (14,6);
	}
	lcdgraf_putc(((valueAdjust/100 )%10)|0x30,1);
	lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
	lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
}

unsigned char Save_MAC (void)
{
	if (menuPosition==1) 
	{
		lcdgraf_gotoxy (6,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,0);
		if (selectedInterf==ITF_UDP)
		{
			cfg.macAddress[0] = own_hw_adr[0] =  valueAdjust;	// Salva 1º hexa
			valueAdjust=own_hw_adr[1];							// Carrega 2º hexa para ajuste
		}
		else
		{
			wifiMac[0] = valueAdjust;							// Salva 1º hexa
			valueAdjust=wifiMac[1];								// Carrega 2º hexa para ajuste
		}
		lcdgraf_gotoxy (10,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
	}
	else if (menuPosition==2) 
	{
		lcdgraf_gotoxy (10,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,0);
		if (selectedInterf==ITF_UDP)
		{
			cfg.macAddress[1] = own_hw_adr[1] =  valueAdjust;	// Salva 2º hexa
			valueAdjust=own_hw_adr[2];							// Carrega 3º hexa para ajuste
		}
		else
		{
			wifiMac[1] = valueAdjust;							// Salva 2º hexa
			valueAdjust=wifiMac[2];								// Carrega 3º hexa para ajuste
		}
		lcdgraf_gotoxy (14,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
	}
	else if (menuPosition==3) 
	{
		lcdgraf_gotoxy (14,5);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,0);
		if (selectedInterf==ITF_UDP)
		{
			cfg.macAddress[2] = own_hw_adr[2] =  valueAdjust;	// Salva 3º hexa
			valueAdjust=own_hw_adr[3];							// Carrega 4º hexa para ajuste
		}
		else
		{
			wifiMac[2] = valueAdjust;							// Salva 3º hexa
			valueAdjust=wifiMac[3];								// Carrega 4º hexa para ajuste
		}
		lcdgraf_gotoxy (6,6);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
	}
	else if (menuPosition==4) 
	{
		lcdgraf_gotoxy (6,6);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,0);
		if (selectedInterf==ITF_UDP)
		{
			cfg.macAddress[3] = own_hw_adr[3] =  valueAdjust;	// Salva 4º hexa
			valueAdjust=own_hw_adr[4];							// Carrega 5º hexa para ajuste
		}
		else
		{
			wifiMac[3] = valueAdjust;							// Salva 4º hexa
			valueAdjust=wifiMac[4];								// Carrega 5º hexa para ajuste
		}
		lcdgraf_gotoxy (10,6);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
	}
	else if (menuPosition==5) 
	{
		lcdgraf_gotoxy (10,6);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,0);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,0);
		if (selectedInterf==ITF_UDP)
		{
			cfg.macAddress[4] = own_hw_adr[4] =  valueAdjust;	// Salva 5º hexa
			valueAdjust=own_hw_adr[5];							// Carrega 6º hexa para ajuste
		}
		else
		{
			wifiMac[4] = valueAdjust;							// Salva 5º hexa
			valueAdjust=wifiMac[5];								// Carrega 6º hexa para ajuste
		}
		lcdgraf_gotoxy (14,6);
		lcdgraf_putc(((valueAdjust/100 )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust/10  )%10)|0x30,1);
		lcdgraf_putc(((valueAdjust     )%10)|0x30,1);
	}
	else if (menuPosition==6) 
	{
		if (selectedInterf==ITF_UDP)
		{
			cfg.macAddress[5] = own_hw_adr[5] =  valueAdjust;	// Salva 6º hexa
			Write_Flash ();
		}
		else
		{
			wifiMac[5] = valueAdjust;							// Salva 6º hexa
		}

		return 1;
	}

 	menuPosition++;
	return 0;
}

void Show_InitDHCP (void)
{
	lcdgraf_clear (page1);

	lcdgraf_gotoxy (1,1);
	lcdgraf_puts ("para escolher  3 e 4 ",0);		 	
	lcdgraf_gotoxy (1,2);
	lcdgraf_puts ("Gravar:6  Cancelar:5 ",0);			 	

	if (selectedInterf==ITF_UDP)
		selectedItem = cfg.dhcpStatus+1;
	else
		selectedItem = cfg.wifiDhcp+1;

	ChangeMark_DHCP();
}

void ChangeMark_DHCP (void)
{
	lcdgraf_gotoxy (7,5);
	if (selectedItem==1)
		lcdgraf_puts(" DESLIGADO ",1);
	else
		lcdgraf_puts(" DESLIGADO ",0);

	lcdgraf_gotoxy (7,6);
	if (selectedItem==2)
		lcdgraf_puts(" LIGADO    ",1);
	else
		lcdgraf_puts(" LIGADO    ",0);
}

void Show_ChangeDHCP (unsigned char val)
{
	if (val==1)
	{
		selectedItem++;
		if (selectedItem==3)
			selectedItem=1;
	}
	else
	{
		selectedItem--;
		if (selectedItem==0)
			selectedItem=2;
	}
	ChangeMark_DHCP ();											// Marca seleção atual
}

void Save_DHCP (void)
{
  	if (selectedInterf==ITF_UDP)
	{
		cfg.dhcpStatus = selectedItem -1;
		Write_Flash ();
	}
	else
	{
		cfg.wifiDhcp = selectedItem -1;
		Write_Flash ();
	}
}

unsigned char Show_WIFI (void)
{
	lcdgraf_clear (page1);

	lcdgraf_gotoxy (1,1);
	lcdgraf_puts ("Para escolher: 3 e 4 ",0);		 	
	lcdgraf_gotoxy (1,2);
	lcdgraf_puts ("Entrar:6  Cancelar:5 ",0);			 	

	selectedItem=0;	
	ChangeMark_WIFI ();

	return selectedItem;
}

void ChangeMark_WIFI (void)
{
	if (selectedItem>=5)
		menuPosition=5;
	else
		menuPosition=0;

	lcdgraf_gotoxy (1,4);
	if (menuPosition<numListWifi)
	{
		lcdgraf_puts ("                     ",0);
		lcdgraf_gotoxy (1,4);
		if (selectedItem==menuPosition)
			lcdgraf_puts (ListWifi[menuPosition],1);
		else
			lcdgraf_puts (ListWifi[menuPosition],0);
	}
	else
		lcdgraf_puts ("                     ",0);		 	
	

	lcdgraf_gotoxy (1,5);
	if ((menuPosition+1)<numListWifi)
	{
		lcdgraf_puts ("                     ",0);
		lcdgraf_gotoxy (1,5);
		if (selectedItem==(menuPosition+1))
			lcdgraf_puts (ListWifi[menuPosition+1],1);
		else
			lcdgraf_puts (ListWifi[menuPosition+1],0);
	}
	else
		lcdgraf_puts ("                     ",0);		 	
	
	lcdgraf_gotoxy (1,6);
	if ((menuPosition+2)<numListWifi)
	{
		lcdgraf_puts ("                     ",0);
		lcdgraf_gotoxy (1,6);
		if (selectedItem==(menuPosition+2))
			lcdgraf_puts (ListWifi[menuPosition+2],1);
		else
			lcdgraf_puts (ListWifi[menuPosition+2],0);
	}
	else
		lcdgraf_puts ("                     ",0);		 	
	
	lcdgraf_gotoxy (1,7);
	if ((menuPosition+3)<numListWifi)
	{
		lcdgraf_puts ("                     ",0);
		lcdgraf_gotoxy (1,7);
		if (selectedItem==(menuPosition+3))
			lcdgraf_puts (ListWifi[menuPosition+3],1);
		else
			lcdgraf_puts (ListWifi[menuPosition+3],0);
	}
	else
		lcdgraf_puts ("                     ",0);

	lcdgraf_gotoxy (1,8);
	if ((menuPosition+4)<numListWifi)
	{
		lcdgraf_puts ("                     ",0);
		lcdgraf_gotoxy (1,8);
		if (selectedItem==(menuPosition+4))
			lcdgraf_puts (ListWifi[menuPosition+4],1);
		else
			lcdgraf_puts (ListWifi[menuPosition+4],0);
	}
	else
		lcdgraf_puts ("                     ",0);
}

unsigned char Show_ChangeWIFI (unsigned char val)
{
	if (val==1)
	{
		selectedItem++;
		if (selectedItem==numListWifi)
			selectedItem=0;
	}
	else
	{
		selectedItem--;
		if (selectedItem==255)
			selectedItem=numListWifi-1;
	}
	ChangeMark_WIFI ();											// Marca seleção atual

	return selectedItem;
}

void Show_Ssid (void)
{
	lcdgraf_clear (page1);

	lcdgraf_gotoxy (1,1);
	lcdgraf_puts ("INFORME O SSID:       ",0);		 	
	lcdgraf_gotoxy (1,6);
	lcdgraf_puts ("Mover cursor:   1 e 2 ",0);		 	
	lcdgraf_gotoxy (1,7);
	lcdgraf_puts ("Escolher letra: 3 e 4 ",0);		 	
	lcdgraf_gotoxy (1,8);
	lcdgraf_puts ("Gravar:6   Cancelar:5 ",0);			 	

	while (wifiSsid[menuPosition]!=0)	menuPosition++;			// Procura o marcador de fim de texto.
	if (menuPosition!=0) menuPosition--;						// Cursor sobre o último caractere do texto.
	else														// Ainda não tem ssid gravado:
		wifiSsid[menuPosition]=' ';								// Inicializa ssid com espaço..

	ChangeMark_Ssid ();
}

void ChangeMark_Ssid (void)
{
	lcdgraf_gotoxy (1,3);										// Posiciona cursor para escrever a senha
	lcdgraf_puts (wifiSsid,0);									// Mostra a senha completa
	lcdgraf_gotoxy (menuPosition+1,3);							// Posiciona cursor no caractere selecionado
	lcdgraf_putc (wifiSsid[menuPosition],1);					// Mostra o caractere selecionado
}

void Show_ChangeSsidPos (unsigned char val)
{
	if (val==1)
	{
		if (menuPosition<31 && wifiSsid[menuPosition]!=0)		
		{
			menuPosition++;										// Avança index para o próximo caractere
			wifiSsid[menuPosition]=' ';							// Insere um caractere "espaço"
		}
	}
	else
	{
		if (menuPosition>0)
		{
			wifiSsid[menuPosition]=0;							// Apaga o caractere no buffer
			lcdgraf_gotoxy (menuPosition+1,3);					// Posiciona cursor no caractere atual
			lcdgraf_putc (' ',0);								// Apaga o caractere no LCD
			menuPosition--;										// Retorna o index para posição anterior
		}
		else													// Não tem mais texto para apagar
			wifiSsid[menuPosition]=' ';							// Insere um caractere "espaço"
	}
	ChangeMark_Ssid ();											// Marca seleção atual
}

void Show_ChangeSsid (unsigned char val)
{
	if (val==1)
	{
		wifiSsid[menuPosition]++;
		if (wifiSsid[menuPosition]<32 || 
			wifiSsid[menuPosition]>122)
			wifiSsid[menuPosition]=32;
	}
	else
	{
		wifiSsid[menuPosition]--;
		if (wifiSsid[menuPosition]<32 ||
			wifiSsid[menuPosition]>122)
			wifiSsid[menuPosition]=122;
	}
	ChangeMark_Ssid ();											// Marca seleção atual
}

void Show_Password (void)
{
	lcdgraf_clear (page1);

	lcdgraf_gotoxy (1,1);
	lcdgraf_puts ("INFORME A SENHA:      ",0);		 	
	lcdgraf_gotoxy (1,6);
	lcdgraf_puts ("Mover cursor:   1 e 2 ",0);		 	
	lcdgraf_gotoxy (1,7);
	lcdgraf_puts ("Escolher letra: 3 e 4 ",0);		 	
	lcdgraf_gotoxy (1,8);
	lcdgraf_puts ("Gravar:6   Cancelar:5 ",0);			 	

	while (wifiPw[menuPosition]!=0)	menuPosition++;				// Procura o marcador de fim de texto.
	if (menuPosition!=0) menuPosition--;						// Cursor sobre o último caractere do texto.
	else														// Ainda não tem password gravado:
		wifiPw[menuPosition]=' ';								// Inicializa password com espaço..

	ChangeMark_Password ();
}

void ChangeMark_Password (void)
{
	lcdgraf_gotoxy (1,3);										// Posiciona cursor para escrever a senha
	lcdgraf_puts (wifiPw,0);									// Mostra a senha completa
	lcdgraf_gotoxy (menuPosition+1,3);							// Posiciona cursor no caractere selecionado
	lcdgraf_putc (wifiPw[menuPosition],1);						// Mostra o caractere selecionado
}

void Show_ChangePasswordPos (unsigned char val)
{
	if (val==1)
	{
		if (menuPosition<31 && wifiPw[menuPosition]!=0)		
		{
			menuPosition++;										// Avança index para o próximo caractere
			wifiPw[menuPosition]=' ';							// Insere um caractere "espaço"
		}
	}
	else
	{
		if (menuPosition>0)
		{
			wifiPw[menuPosition]=0;								// Apaga o caractere no buffer
			lcdgraf_gotoxy (menuPosition+1,3);					// Posiciona cursor no caractere atual
			lcdgraf_putc (' ',0);								// Apaga o caractere no LCD
			menuPosition--;										// Retorna o index para posição anterior
		}
		else													// Não tem mais texto para apagar
			wifiPw[menuPosition]=' ';							// Insere um caractere "espaço"
	}
	ChangeMark_Password ();										// Marca seleção atual
}

void Show_ChangePassword (unsigned char val)
{
	if (val==1)
	{
		wifiPw[menuPosition]++;
		if (wifiPw[menuPosition]<32 || 
			wifiPw[menuPosition]>122)
			wifiPw[menuPosition]=32;
	}
	else
	{
		wifiPw[menuPosition]--;
		if (wifiPw[menuPosition]<32 ||
			wifiPw[menuPosition]>122)
			wifiPw[menuPosition]=122;
	}
	ChangeMark_Password ();										// Marca seleção atual
}

void Save_Ssid_Password (void)
{
	unsigned char n;

	if (wifiSsid[1]==0)
		cfg.wifiUser=0;											// Não configurou rede, usa "Wyma"
	else
		cfg.wifiUser=1;											// Vai usar Rede do usuário...

	cfg.wifiMode = wifiMode-1;
	
	for (n=0 ; n<32 ; n++)
	{
		cfg.wifiPw[n]=wifiPw[n];  								// Copia nova senha ativa para a configuração
		cfg.wifiSsid[n]=wifiSsid[n];							// Copia nova rede ativa para a configuração
	}
	Write_Flash ();
}

void Show_ModeWifi (void)
{
	lcdgraf_clear (page1);

	lcdgraf_gotoxy (1,1);
	lcdgraf_puts ("para escolher  3 e 4 ",0);		 	
	lcdgraf_gotoxy (1,2);
	lcdgraf_puts ("Gravar:6  Cancelar:5 ",0);			 	

	wifiMode = cfg.wifiMode+1;

	ChangeMark_ModeWifi ();
}

void ChangeMark_ModeWifi (void)
{
	lcdgraf_gotoxy (1,5);
	if (wifiMode==1)
		lcdgraf_puts(" STA - ESTAÇÃO        ",1);
	else
		lcdgraf_puts(" STA - ESTAÇÃO        ",0);

	lcdgraf_gotoxy (1,6);
	if (wifiMode==2)
		lcdgraf_puts(" AP - PONTO DE ACESSO ",1);
	else
		lcdgraf_puts(" AP - PONTO DE ACESSO ",0);
}

void Show_ChangeModeWifi (unsigned char val)
{
	if (val==1)
	{
		wifiMode++;
		if (wifiMode==3)
			wifiMode=1;
	}
	else
	{
		wifiMode--;
		if (wifiMode==0)
			wifiMode=2;
	}
	ChangeMark_ModeWifi ();											// Marca seleção atual
}

void Verif_KeepKey (unsigned short keyPress, unsigned char cSeg)
{
	unsigned int cont=0;

	if (keepPress==0)
	{
		Beep ();
		while (Read_KeyModel()==keyPress)
		{
			if (++cont==0x5FFFF)
			{
				keepPress=1;
				break;
			}
		}
	}
	else
		while (cSeg--)
		{
			for (cont=0 ; cont<0xFFFF ; cont++);
		}
}

unsigned char CheckMenu (void)
{
	unsigned short keyPress=Read_KeyModel();
	unsigned char n, cont, exist;

	if (keyPress!=lastKey)
	{
		lastKey=keyPress;
		keepPress=0;
	}

	if (selectedMenu==_MENUNONE)
	{
		selectedMenu = _MAINMENU;
		selectedItem = Show_MainMenu ();
	}

	if (selectedMenu == _MAINMENU)								// Opções do Menu Principal
	{
		if (keyPress==KEY3)
		{
			selectedItem = Show_ChangeMainMenu (0);
			Verif_KeepKey (keyPress, 100);
		}
		else if (keyPress==KEY4)
		{
			selectedItem = Show_ChangeMainMenu (1);
			Verif_KeepKey (keyPress, 100);
		}
		else if (keyPress==KEY5)
		{
			selectedMenu = _MENUNONE;
			Beep ();
			while (Read_KeyModel()==keyPress);
			return 1;
		}
		else if (keyPress==KEY6)
		{
			if (selectedItem==0)
			{
				selectedMenu=_TIMEMENU;
				selectedItem = Show_DateTimeMenu ();
			}
			else if (selectedItem==1)
			{
				selectedMenu=_DRIVERMENU;
				Show_InitEscolherDriver ();
			}
			else if (selectedItem==2)
			{
				selectedMenu=_FTDRIVERMENU;
				Show_InitFormatDriver ();
			}
			else if (selectedItem==3)
			{
				selectedInterf=ITF_UDP;
				selectedMenu=_TCPIP;
				selectedItem=Show_TCPIP ();
			}
			else if (selectedItem==4)
			{
				selectedMenu=_MODEWIFI;
				Show_ModeWifi ();
			}
			else if (selectedItem==5)
			{
				selectedInterf=ITF_WIFI;
				selectedMenu=_TCPIP;
				selectedItem=Show_TCPIP ();
			}
			else if (selectedItem==6)
			{
				selectedMenu=_TIMERUNMENU;
				Show_InitTimeRun ();
			}
			else if (selectedItem==7)
			{
				selectedMenu=_TIMEREPMENU;
				Show_InitTimeRep ();
			}
			else if (selectedItem==8)
			{
				selectedMenu=_SAVEREPMENU;
				Show_InitSaveRep ();
			}
			else if (selectedItem==9)
			{
				selectedMenu=_INITTICKETMENU;
				Show_InitTicket ();
			}

			Beep ();
			while (Read_KeyModel()==keyPress);
		}
	}
	
	else if (selectedMenu==_TIMEMENU)							// Opções do Menu de Data e Hora
	{
		if (keyPress==KEY3)
		{
			selectedItem = Show_ChangeDateTimeMenu (0);
			Verif_KeepKey (keyPress, 100);
		}
		else if (keyPress==KEY4)
		{
		 	selectedItem = Show_ChangeDateTimeMenu (1);
			Verif_KeepKey (keyPress, 100);
		}
		else if (keyPress==KEY5)
		{
			selectedMenu=_MAINMENU;
			selectedItem = Show_MainMenu ();
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
		else if (keyPress==KEY6)
		{
			if (selectedItem==0)
			{
				selectedMenu=_AJTIMEMENU;
				Show_InitTime ();
			}
			else if (selectedItem==1)
			{
				selectedMenu=_AJDATEMENU;
				Show_InitDate ();
			}
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
	}
	
	else if (selectedMenu==_AJTIMEMENU)							// Ajuste de hora selecionado
	{
		if (keyPress==KEY3)
		{
			Show_ChangeTime (1);
			Verif_KeepKey (keyPress, 50);
		}						
		else if (keyPress==KEY4)
		{
		 	Show_ChangeTime (0);
			Verif_KeepKey (keyPress, 50);
		}
		else if (keyPress==KEY5)
		{
			selectedMenu=_MAINMENU;
			selectedItem = Show_MainMenu ();
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
		else if (keyPress==KEY6)
		{
			if (Save_Time ())
			{					
				selectedMenu=_TIMEMENU;
				selectedItem=Show_DateTimeMenu ();
			}
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
	}
	
	else if (selectedMenu==_AJDATEMENU)							// Ajuste de Data selecionado
	{
		if (keyPress==KEY3)
		{
			Show_ChangeDate (1);
			Verif_KeepKey (keyPress, 50);
		}
		else if (keyPress==KEY4)
		{
		 	Show_ChangeDate (0);
			Verif_KeepKey (keyPress, 50);
		}
		else if (keyPress==KEY5)
		{
			selectedMenu=_MAINMENU;
			selectedItem = Show_MainMenu ();
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
		else if (keyPress==KEY6)
		{
			if (Save_Date ())
			{
				selectedMenu=_TIMEMENU;
				selectedItem=Show_DateTimeMenu ();
			}
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
	}
	
	else if (selectedMenu==_DRIVERMENU)							// Escolha do drive selecionado
	{
		if (keyPress==KEY3)
		{
			Show_ChangeDriver (1);
			Verif_KeepKey (keyPress, 50);
		}
		else if (keyPress==KEY4)
		{
			Show_ChangeDriver (0);
			Verif_KeepKey (keyPress, 50);
		}
		else if (keyPress==KEY5)
		{
			selectedMenu=_MAINMENU;
			selectedItem = Show_MainMenu ();
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
		else if (keyPress==KEY6)
		{
			Save_Driver ();
			selectedMenu=_MAINMENU;
			selectedItem = Show_MainMenu (); 
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
	}
	
	else if (selectedMenu==_FTDRIVERMENU)						// Formatação do drive selecionado
	{
		if (keyPress==KEY3)
		{
			Show_FormatDriver (1);
			Verif_KeepKey (keyPress, 50);
		}
		else if (keyPress==KEY4)
		{
			Show_FormatDriver (0);
			Verif_KeepKey (keyPress, 50);
		}
		else if (keyPress==KEY5)
		{
			selectedMenu=_MAINMENU;
			selectedItem = Show_MainMenu ();
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
		else if (keyPress==KEY6)
		{
			lcdgraf_clear (page1);
			lcdgraf_gotoxy (1,3);
			lcdgraf_puts ("Aguarde formatando...",0);
			if (Save_OpcaoFormat())
				Format_Drive ();
			selectedMenu=_MAINMENU;
			selectedItem = Show_MainMenu ();
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
	}
								
	else if (selectedMenu==_TCPIP)							// Opções do menu de TCP/IP LAN
	{
		if (keyPress==KEY3)
		{
			selectedItem = Show_ChangeMenuTcpIP (0);
			Verif_KeepKey (keyPress, 100);
		}
		else if (keyPress==KEY4)
		{
		 	selectedItem = Show_ChangeMenuTcpIP (1);
			Verif_KeepKey (keyPress, 100);
		}
		else if (keyPress==KEY5)
		{
			selectedMenu=_MAINMENU;
			selectedItem = Show_MainMenu ();
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
		else if (keyPress==KEY6)
		{
			if (selectedItem==0)
			{
				selectedMenu=_AJDHCP;
				Show_InitDHCP ();
			}
			else if (selectedItem==1)
			{
				selectedMenu=_AJIP;
				Show_InitIP ();
			}
			else if (selectedItem==2)
			{
				selectedMenu=_AJGATE;
				Show_InitGate ();
			}
			else if (selectedItem==3)
			{
				selectedMenu=_AJDNS1;
				Show_InitDns1 ();
			}
			else if (selectedItem==4)
			{
				selectedMenu=_AJDNS2;
				Show_InitDns2 ();
			}
			else if (selectedItem==5)
			{
				selectedMenu=_AJMASCARA;
				Show_InitMask ();
			}
			else if (selectedItem==6)
			{
				selectedMenu=_AJMAC;
				Show_InitMAC ();
			}
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
	}

	else if (selectedMenu==_TIMERUNMENU)
	{
		if (keyPress==KEY3)
		{
			Show_ChangeTimeRun (1);
			Verif_KeepKey (keyPress, 50);
		}
		else if (keyPress==KEY4)
		{
			Show_ChangeTimeRun (0);
			Verif_KeepKey (keyPress, 50);
		}
		else if (keyPress==KEY5)
		{
			selectedMenu=_MAINMENU;
			selectedItem = Show_MainMenu ();
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
		else if (keyPress==KEY6)
		{
			if (Save_TimeRun ())
			{
				selectedMenu=_MAINMENU;
				selectedItem = Show_MainMenu (); 
			}
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
	}

	else if (selectedMenu==_TIMEREPMENU)
	{
		if (keyPress==KEY3)
		{
			Show_ChangeTimeRep (1);
			Verif_KeepKey (keyPress, 50);
		}
		else if (keyPress==KEY4)
		{
			Show_ChangeTimeRep (0);
			Verif_KeepKey (keyPress, 50);
		}
		else if (keyPress==KEY5)
		{
			selectedMenu=_MAINMENU;
			selectedItem = Show_MainMenu ();
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
		else if (keyPress==KEY6)
		{
			if (Save_TimeRep ())
			{				
				selectedMenu=_MAINMENU;
				selectedItem = Show_MainMenu (); 
			}
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
	}

	else if (selectedMenu==_SAVEREPMENU)
	{
		if (keyPress==KEY3)
		{
			Show_ChangeSaveRep (1);
			Verif_KeepKey (keyPress, 50);
		}
		else if (keyPress==KEY4)
		{
			Show_ChangeSaveRep (0);
			Verif_KeepKey (keyPress, 50);
		}
		else if (keyPress==KEY5)
		{
			selectedMenu=_MAINMENU;
			selectedItem = Show_MainMenu ();
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
		else if (keyPress==KEY6)
		{
			Save_SaveRep ();
			selectedMenu=_MAINMENU;
			selectedItem = Show_MainMenu (); 
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
	}

	else if (selectedMenu==_INITTICKETMENU)
	{
		if (keyPress==KEY3)
		{
			Show_ChangeInitTicket (1);
			Verif_KeepKey (keyPress, 50);
		}
		else if (keyPress==KEY4)
		{
			Show_ChangeInitTicket (0);
			Verif_KeepKey (keyPress, 50);
		}
		else if (keyPress==KEY5)
		{
			selectedMenu=_MAINMENU;
			selectedItem = Show_MainMenu ();
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
		else if (keyPress==KEY6)
		{
			lcdgraf_clear (page1);
			lcdgraf_gotoxy (1,3);
			lcdgraf_puts ("Aguarde inicializando",0);
			if (Save_InitTicket())
				Format_DataFlash ();
			selectedMenu=_MAINMENU;
			selectedItem = Show_MainMenu ();
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
	}
	
	else if (selectedMenu==_FINDWIFI)							// Selecionar rede wifi
	{
		if (radio_wifi(1)==1)
		{
			if ((n=scan_wifi(1,1))==1)
			{
				if (numListWifi<MAXLISTWIFI)
				{
					exist=0;
					for (cont=0 ; cont<numListWifi ; cont++)	// Varredura em todas redes da lista..
					{
						exist=1;					
						for (n=0 ; n<32 ; n++)					// Verifica se o nome ja esta na lista
						{
							if (wifiSsid[n]!=ListWifi[cont][n])
							{
								exist=0;
								break;
							}
							if (wifiSsid[n]==0)					// Chegou ao final sem nada diferente
								break;
						}
						if (exist)
							break;
					}
										
					if (exist==0)								// Se ainda não existe grava nova rede
					{
						for (n=0 ; n<32 ; n++)
							ListWifi[numListWifi][n]=wifiSsid[n];
						numListWifi++;
						Show_WIFI();
					}
				}
			}
			else if (n==0)
			{
				lcdgraf_gotoxy (1,3);
				lcdgraf_puts ("    Procurando...    ",0);
			}
			else if (n==2)
			{
				lcdgraf_gotoxy (1,3);
				lcdgraf_puts ("                     ",0);
			}
		}
		else
		{
			lcdgraf_gotoxy (1,3);
			lcdgraf_puts ("   Ligando rádio...  ",0);
		}

		if (keyPress==KEY3)
		{
			Show_ChangeWIFI (0);
			Verif_KeepKey (keyPress, 100);
		}						
		else if (keyPress==KEY4)
		{
			Show_ChangeWIFI (1);
			Verif_KeepKey (keyPress, 100);
		}
		else if (keyPress==KEY5)
		{
			scan_wifi(0,1);										// Desliga o scan 
			selectedMenu=_MAINMENU;
			selectedItem=Show_MainMenu ();
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
		else if (keyPress==KEY6)
		{
			scan_wifi(0,1);										// Desliga o scan wifi..

			for (n=0 ; n<32 ; n++)
				wifiSsid[n]=cfg.wifiSsid[n];					// Copia nome da rede ativa temporariamente..

			for (exist=1, n=0 ; n<32 ; n++)
			{
				if (ListWifi[selectedItem][n]!=wifiSsid[n])		// Verifica se a rede escolhida é diferente da ativa..
				{
					exist=0;
					break;
				}	
				if (ListWifi[selectedItem][n]==0)				// Se chegou aui é a mesma rede ..
					break;
			}

			if (exist==0)										// A rede escolhida ainda não foi ativada..
			{
				for (n=0 ; n<32 ; n++)
				{
					wifiSsid[n]=ListWifi[selectedItem][n];		// Copia nome da rede escolhida..
					wifiPw[n]=0;								// Zera password..
				}
			}
			else
			{
				for (n=0 ; n<32 ; n++)
					wifiPw[n]=cfg.wifiPw[n];					// Copia password da rede ativa..
			}

			menuPosition=0;
			selectedMenu=_AJPASSWORD;							// Ajusta flag para mostrar password.
			Show_Password ();									// Mostra password da rede ativa com cursor.

			Beep ();
			while (Read_KeyModel()==keyPress);
		}
	}

	else if (selectedMenu==_MODEWIFI)							// Modo de operação wifi:AP/STA
	{
		if (keyPress==KEY3)
		{
			Show_ChangeModeWifi (1);
			Verif_KeepKey (keyPress, 50);
		}
		else if (keyPress==KEY4)
		{
			Show_ChangeModeWifi (0);
			Verif_KeepKey (keyPress, 50);
		}
		else if (keyPress==KEY5)
		{
			selectedMenu=_MAINMENU;
			selectedItem = Show_MainMenu ();
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
		else if (keyPress==KEY6)
		{
			if ((wifiMode-1)==WIFI_MODE_STA)
			{
				selectedMenu=_FINDWIFI;
				numListWifi=0;
				selectedItem=Show_WIFI ();
			}
			else if ((wifiMode-1)==WIFI_MODE_AP)
			{
				for (n=0 ; n<32 ; n++)
					wifiSsid[n]=cfg.wifiSsid[n];				// Copia nome da rede ativa..
				
				menuPosition=0;
				selectedMenu=_AJSSID;							// Ajusta flag para mostrar ssid.
				Show_Ssid ();									// Mostra password da rede ativa com cursor.
			}
			
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
	}

	else if (selectedMenu==_AJSSID)
	{
		if (keyPress==KEY1)
		{
			Show_ChangeSsidPos (1);
			Verif_KeepKey (keyPress, 50);
		}				
		else if (keyPress==KEY2)
		{
			Show_ChangeSsidPos (0);
			Verif_KeepKey (keyPress, 50);
		}				
		else if (keyPress==KEY3)
		{
			Show_ChangeSsid (1);
			Verif_KeepKey (keyPress, 30);
		}						
		else if (keyPress==KEY4)
		{
			Show_ChangeSsid (0);
			Verif_KeepKey (keyPress, 30);
		}
		else if (keyPress==KEY5)
		{
			selectedMenu=_MODEWIFI;
			Show_ModeWifi ();
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
		else if (keyPress==KEY6)
		{
			if (wifiSsid[1]==0)
			{	
				for (n=0 ; n<32 ; n++)								
					wifiPw[n]=0;									// Zera senha da rede ativa..
				Save_Ssid_Password ();								// Salva e termina..
				selectedMenu=_MAINMENU;
				selectedItem = Show_MainMenu (); 
			}
			else
			{
				for (n=0 ; n<32 ; n++)
					wifiPw[n]=cfg.wifiPw[n];						// Copia senha da rede ativa..

				menuPosition=0;
				selectedMenu=_AJPASSWORD;							// Ajusta flag para mostrar password.
				Show_Password ();									// Mostra password da rede ativa com cursor.
			}

			Beep ();
			while (Read_KeyModel()==keyPress);
		}
	}

	else if (selectedMenu==_AJPASSWORD)
	{
		if (keyPress==KEY1)
		{
			Show_ChangePasswordPos (1);
			Verif_KeepKey (keyPress, 50);
		}				
		else if (keyPress==KEY2)
		{
			Show_ChangePasswordPos (0);
			Verif_KeepKey (keyPress, 50);
		}				
		else if (keyPress==KEY3)
		{
			Show_ChangePassword (1);
			Verif_KeepKey (keyPress, 30);
		}						
		else if (keyPress==KEY4)
		{
			Show_ChangePassword (0);
			Verif_KeepKey (keyPress, 30);
		}
		else if (keyPress==KEY5)
		{
			if ((wifiMode-1)==WIFI_MODE_STA)
			{
				selectedMenu=_FINDWIFI;
				selectedItem=Show_WIFI ();
			}
			else if ((wifiMode-1)==WIFI_MODE_AP)
			{
				for (n=0 ; n<32 ; n++)
					wifiSsid[n]=cfg.wifiSsid[n];				// Copia nome da rede ativa..
				
				menuPosition=0;
				selectedMenu=_AJSSID;							// Ajusta flag para mostrar ssid.
				Show_Ssid ();									// Mostra password da rede ativa com cursor.
			}

			Beep ();
			while (Read_KeyModel()==keyPress);
		}
		else if (keyPress==KEY6)
		{
			Save_Ssid_Password ();
			selectedMenu=_MAINMENU;
			selectedItem = Show_MainMenu (); 
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
	}

	else if (selectedMenu==_AJDHCP)								// Ajuste do DHCP selecionado
	{
		if (keyPress==KEY3)
		{
			Show_ChangeDHCP (1);
			Verif_KeepKey (keyPress, 50);
		}						
		else if (keyPress==KEY4)
		{
			Show_ChangeDHCP (0);
			Verif_KeepKey (keyPress, 50);
		}
		else if (keyPress==KEY5)
		{
			selectedMenu=_TCPIP;
			selectedItem=Show_TCPIP ();
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
		else if (keyPress==KEY6)
		{
			Save_DHCP ();
			selectedMenu=_TCPIP;
			selectedItem=Show_TCPIP ();
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
	}
	
	else if (selectedMenu==_AJIP)								// Ajuste do endereço IP selecionado
	{
		if (keyPress==KEY3)
		{
			Show_ChangeIP (1);
			Verif_KeepKey (keyPress, 50);
		}						
		else if (keyPress==KEY4)
		{
			Show_ChangeIP (0);
			Verif_KeepKey (keyPress, 50);
		}
		else if (keyPress==KEY5)
		{
			selectedMenu=_TCPIP;
			selectedItem=Show_TCPIP ();
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
		else if (keyPress==KEY6)
		{
			if (Save_IP ())
			{					
				selectedMenu=_TCPIP;
				selectedItem=Show_TCPIP ();
			}
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
	}

	else if (selectedMenu==_AJGATE)								// Ajuste do endereço IP selecionado
	{
		if (keyPress==KEY3)
		{
			Show_ChangeGate (1);
			Verif_KeepKey (keyPress, 50);
		}						
		else if (keyPress==KEY4)
		{
			Show_ChangeGate (0);
			Verif_KeepKey (keyPress, 50);
		}
		else if (keyPress==KEY5)
		{
			selectedMenu=_TCPIP;
			selectedItem=Show_TCPIP ();
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
		else if (keyPress==KEY6)
		{
			if (Save_Gate ())
			{					
				selectedMenu=_TCPIP;
				selectedItem=Show_TCPIP ();
			}
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
	}

	else if (selectedMenu==_AJDNS1)								// Ajuste do endereço IP selecionado
	{
		if (keyPress==KEY3)
		{
			Show_ChangeDns1 (1);
			Verif_KeepKey (keyPress, 50);
		}						
		else if (keyPress==KEY4)
		{
			Show_ChangeDns1 (0);
			Verif_KeepKey (keyPress, 50);
		}
		else if (keyPress==KEY5)
		{
			selectedMenu=_TCPIP;
			selectedItem=Show_TCPIP ();
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
		else if (keyPress==KEY6)
		{
			if (Save_Dns1 ())
			{					
				selectedMenu=_TCPIP;
				selectedItem=Show_TCPIP ();
			}
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
	}

	else if (selectedMenu==_AJDNS2)								// Ajuste do endereço IP selecionado
	{
		if (keyPress==KEY3)
		{
			Show_ChangeDns2 (1);
			Verif_KeepKey (keyPress, 50);
		}						
		else if (keyPress==KEY4)
		{
			Show_ChangeDns2 (0);
			Verif_KeepKey (keyPress, 50);
		}
		else if (keyPress==KEY5)
		{
			selectedMenu=_TCPIP;
			selectedItem=Show_TCPIP ();
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
		else if (keyPress==KEY6)
		{
			if (Save_Dns2 ())
			{					
				selectedMenu=_TCPIP;
				selectedItem=Show_TCPIP ();
			}
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
	}
	
	else if (selectedMenu==_AJMASCARA)							// Ajuste da máscara de IP selecionado
	{
		if (keyPress==KEY3)
		{
			Show_ChangeMask (1);
			Verif_KeepKey (keyPress, 50);
		}									   
		else if (keyPress==KEY4)
		{
			Show_ChangeMask (0);
			Verif_KeepKey (keyPress, 50);
		}
		else if (keyPress==KEY5)
		{
			selectedMenu=_TCPIP;
			selectedItem=Show_TCPIP ();
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
		else if (keyPress==KEY6)
		{
			if (Save_Mask ())
			{					
				selectedMenu=_TCPIP;
				selectedItem=Show_TCPIP ();
			}
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
	}
	
	else if (selectedMenu==_AJMAC)								// Ajuste do endereço MAC selecionado
	{
		if (keyPress==KEY3)
		{
			//Show_ChangeMAC (0);
			Verif_KeepKey (keyPress, 50);
		}						
		else if (keyPress==KEY4)
		{
			 //Show_ChangeMAC (1);
			Verif_KeepKey (keyPress, 50);
		}
		else if (keyPress==KEY5)
		{
			selectedMenu=_TCPIP;
			selectedItem=Show_TCPIP ();
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
		else if (keyPress==KEY6)
		{
			//if (Save_MAC ())
			//{					
				selectedMenu=_TCPIP;
				selectedItem=Show_TCPIP ();
			//}
			Beep ();
			while (Read_KeyModel()==keyPress);
		}
	}
	
	return 0;
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
