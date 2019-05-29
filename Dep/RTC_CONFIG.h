/*----------------------------------------------------------------------------
 *      Biblioteca:	LPC176x_RTC.lib
 *      Proposito:	Biblioteca com funções RTC
 *----------------------------------------------------------------------------
 *      Arquivo:    RTC_CONFIG.h
 *      Proposito: 	Arquivo de definições do usuário
 *----------------------------------------------------------------------------
 *      Versão: 	1.00 	Prog: Ricardo T.   Data: 25/04/2013
 * 		Descrição:  Versão inicial
 *----------------------------------------------------------------------------
 *      Este código pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#ifndef	__RTC_CONFIG__
#define	__RTC_CONFIG__

/*----------------------------------------------------------------------------
 *      User Typedefs
 *---------------------------------------------------------------------------*/

typedef struct TIME 
{
  	unsigned short  YEAR;
  	unsigned char   MONTH;
  	unsigned char   WEEKDAY;
  	unsigned char	DAY;
  	unsigned short  YEARDAY;
  	unsigned char	HOUR;
  	unsigned char  	MINUTE;
  	unsigned char  	SECOND;
}TIME_TYPE;

/* Mask for user --------------------------------------------------------- */

#define MASKRTC_CAL		0x0001

#define USE_CAL			(RTC_userOptions & MASKRTC_CAL)

/* Exported variables ----------------------------------------------------- */

extern unsigned char RTC_userOptions;

/* Exported functions ----------------------------------------------------- */

extern void init_rtc (void);
extern void init_calibration (unsigned int actualFreq);
extern unsigned char DayOfWeek (TIME_TYPE time);
extern unsigned short DayOfYear (TIME_TYPE time);
extern TIME_TYPE Get_RTC (void);
extern void Save_RTC (TIME_TYPE TIME);
extern TIME_TYPE UTC_To_RTC (unsigned short yearBase, unsigned int utcTime);
extern unsigned int RTC_To_UTC (unsigned short yearBase);
extern unsigned int RTC_To_UTCLocal (unsigned short yearBase, int timeZone, char dayLight);
extern TIME_TYPE RTC_To_RTCLocal (unsigned short yearBase, int timeZone, char dayLight);

#endif

 /*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
