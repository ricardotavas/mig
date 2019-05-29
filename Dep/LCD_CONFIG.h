/*----------------------------------------------------------------------------
 *      Biblioteca:	LPC176x_LCD.lib
 *      Proposito:	Biblioteca com funções para LCD
 *----------------------------------------------------------------------------
 *      Arquivo:    LCD_CONFIG.h
 *      Proposito: 	Arquivo de definições do usuário para LCD
 *----------------------------------------------------------------------------
 *      Versão: 	1.00 	Prog: Ricardo T.   Data: 25/04/2013
 * 		Descrição:  Versão inicial
 *----------------------------------------------------------------------------
 *      Este código pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#ifndef	__LCD_CONFIG__
#define	__LCD_CONFIG__

#include <stdio.h>

/*----------------------------------------------------------------------------
 *      Users Mask
 *---------------------------------------------------------------------------*/

#define MASKLCD_GRAPH	0x01
#define MASKLCD_DEFGR	0x0E
#define MASKLCD_CHAR	0x10
#define MASKLCD_DEFCH	0x60

#define USE_LCDG		(LCD_userOptions & MASKLCD_GRAPH)
#define DEF_LCDG		((LCD_userOptions & MASKLCD_DEFGR)>>1)
#define USE_LCDC		((LCD_userOptions & MASKLCD_CHAR)>>4)
#define DEF_LCDC		((LCD_userOptions & MASKLCD_DEFCH)>>5)

/*----------------------------------------------------------------------------
 *      LCD Definitions for LCD16X2 
 *---------------------------------------------------------------------------*/
#define		ARROW_UP			6
#define		ARROW_DOWN			7

/*----------------------------------------------------------------------------
 *      Definitions
 *---------------------------------------------------------------------------*/
#define __128x64		0x00
#define __16x2			0x00

/*----------------------------------------------------------------------------
 *      LCD Definitions
 *---------------------------------------------------------------------------*/
#define	page1			0xB8
#define	page2			0xB9
#define	page3			0xBA
#define	page4			0xBB
#define	page5			0xBC
#define	page6			0xBD
#define	page7			0xBE
#define	page8			0xBF

/*----------------------------------------------------------------------------
 *      Exported variables
 *---------------------------------------------------------------------------*/

extern unsigned char LCD_userOptions;

/*----------------------------------------------------------------------------
 *      User type definitions for Lcd bitmap
 *---------------------------------------------------------------------------*/

typedef struct imgScr
{
	unsigned char bitmap[1024];					// Imagem até 128x64 = 1024 bits
												// Alterar este valor pode não ser suficiente para suportar 
												// imagens maiores, é necessário aumentar também o buffer 
												// do interpretador do arquivo na LIB IMP, IMGPRN são os 
												// dados ja interpretados.
	unsigned short width;						// Largura da Imagem
	unsigned short height;						// Altura da imagem
}IMGSCR;

/*----------------------------------------------------------------------------
 *      Exported functions
 *---------------------------------------------------------------------------*/

extern unsigned char lcd_Configuration (void);

extern unsigned char lcd_init (void);
extern void lcd_gotoxy (unsigned int x, unsigned int y);
extern void lcd_putc (unsigned char databyte);
extern void lcd_puts (unsigned char *buf);
extern void lcd_putsconst (const char *buf); 
extern void lcd_clear (void);
extern void lcd_backlight (unsigned char status);
extern void lcdRow_clear (void);

extern void lcdgraf_init (void); 
extern void lcdgraf_gotoxy (unsigned char x, unsigned char y);
extern void lcdgraf_putc (char databyte, unsigned char reverse);
extern void lcdgraf_putcconst (const char databyte, unsigned char reverse);
extern void lcdgraf_puts (char *buf, unsigned char reverse); 
extern void lcdgraf_putsconst (const char *buf, unsigned char reverse); 
extern void lcdgraf_fillname (char *buf, unsigned char reverse);
extern char lcdgraf_CompileBitmap(FILE *filebmp, IMGSCR *buf);
extern void lcdgraf_showpicture (IMGSCR *buf);
extern void lcdgraf_clear (unsigned char fromPage);
extern void lcdgraf_power (unsigned char status);
extern void lcdgraf_backlight (unsigned char status);

#endif

 /*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
