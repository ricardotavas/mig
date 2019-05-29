/*----------------------------------------------------------------------------
 *      Biblioteca:	LPC176x_IMP.lib
 *      Proposito:	Biblioteca com fun��es para impress�o 
 *----------------------------------------------------------------------------
 *      Arquivo:    IMP_CONFIG.h
 *      Proposito: 	Arquivo de defini��es do usu�rio para impressoras
 *----------------------------------------------------------------------------
 *      Vers�o: 	1.00 	Prog: Ricardo T.   Data: 25/04/2013
 * 		Descri��o:  Vers�o inicial
 *----------------------------------------------------------------------------
 *      Este c�digo pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#ifndef		__IMP_CONFIG__
#define		__IMP_CONFIG__

#include 	<stdio.h>
	   	
/*----------------------------------------------------------------------------
 *      Mask for user defitions
 *---------------------------------------------------------------------------*/

#define 	MASKIMP_MOD		0x01

#define 	IMP_MODEL		(IMP_userOptions & MASKIMP_MOD)

/*----------------------------------------------------------------------------
 *      User type definitions for Ticket
 *---------------------------------------------------------------------------*/

/*----------------------------------*/
/*  	  Graphic (optional)		*/
/* 	Head 1: previs�o, hora, texto..	*/
/*	Head 2: previs�o, hora, texto..	*/	
/*	Head 3: previs�o, hora, texto..	*/	
/*		A1234  -  Small/Large		*/  
/*			  department			*/
/*	Foot 1: previs�o, hora, texto..	*/
/*	Foot 2: previs�o, hora, texto..	*/
/*	Foot 3: previs�o, hora, texto..	*/	
/*	Foot 4: previs�o, hora, texto..	*/
/*----------------------------------*/

/*----------------------------------------------*/
/* Informa��o de cada linha de texto pode ser: 	*/
/* 	0=Mostra texto com caractere padr�o			*/
/* 	1=Mostra texto com caractere expandido 		*/
/* 	2=Mostra Previs�o espera 					*/
/* 	3=mostra Data e Hora	   					*/
/*----------------------------------------------*/

typedef struct ticket
{
	unsigned short	widthImage;		  	// Largura da imagem do logo
	unsigned short	heightImage;		// Altura da imagem do logo
	unsigned char 	*bufImage;			// ponteiro para a imagem do logo
	char 			*headerLine1;		// Ponteiro para o texto do cabe�alho1
	unsigned char	infoHeader1;		// Configura a informa��o a ser exibida no cabe�alho 1
	char 			*headerLine2;		// Ponteiro para o texto do cabe�alho1
	unsigned char	infoHeader2;		// Configura a informa��o a ser exibida no cabe�alho 1
	char 			*headerLine3;		// Ponteiro para o texto do cabe�alho1
	unsigned char	infoHeader3;		// Configura a informa��o a ser exibida no cabe�alho 1
	char 			alphaNum;			// Letra do Ticket

	unsigned short 	number;				// N�mero do Ticket
	unsigned char	lenNumber;	   		// Quantidade de caracteres do n�mero do ticket
	unsigned char	typeNumber;			// Impress�o do N�mero do ticket: 0=normal; 1=gr�fico padr�o impressora; 2=gr�fico wyma
 	unsigned short	widthNumber;	  	// Largura da imagem do ticket
	unsigned short	heightNumber;		// Altura da imagem do ticket
	unsigned char 	*bufNumber;			// ponteiro para a imagem do ticket

	char 			*department;		// Ponteiro para o texto do departamento
	unsigned char	infoDep;			// Configura a informa��o a ser exibida no departamento
	char 			*footerLine1;		// Ponteiro para o texto do rodap� 1
	unsigned char	infoFooter1;		// Configura a informa��o a ser exibida no rodap� 1
	char 			*footerLine2;		// Ponteiro para o texto do rodap� 2
	unsigned char	infoFooter2;		// Configura a informa��o a ser exibida no rodap� 2
	char 			*footerLine3;		// Ponteiro para o texto do rodap� 3	   
	unsigned char	infoFooter3;		// Configura a informa��o a ser exibida no rodap� 3
	char 			*footerLine4;		// Ponteiro para o texto do rodap� 4
	unsigned char	infoFooter4;		// Configura a informa��o a ser exibida no rodap� 4
	char			*strNumProtocol;	// Ponteiro para a informa��o do n�mero do protocolo
	unsigned char	cut;				// Tipo de corte: 0=corte total 1=corte parcial
	unsigned char	copy;				// N�mero de vias do ticket
	unsigned char	orientation;		// orienta��o da impress�o normal(1)/invertida(0)
}TICKET;

/*----------------------------------------------------------------------------
 *      User type definitions for Ticket bitmap
 *---------------------------------------------------------------------------*/

typedef struct imgPrn
{
	unsigned char bitmap[1024];					// Imagem at� 128x64 = 1024 bits
												// Alterar este valor pode n�o ser suficiente para suportar 
												// imagens maiores, � necess�rio aumentar tamb�m o buffer 
												// do interpretador do arquivo na LIB IMP, IMGPRN s�o os 
												// dados ja interpretados.
	unsigned short width;						// Largura da Imagem
	unsigned short height;						// Altura da imagem
}IMGPRN;

/* Exported variables ----------------------------------------------------- */

extern unsigned char IMP_userOptions;

/*----------------------------------------------------------------------------
 *      Exported functions
 *---------------------------------------------------------------------------*/

extern char init_uartimp (unsigned char uartPort, unsigned int baudrate);

extern char PrintENG_Init (void);
extern char PrintENG_Ticket (TICKET formatTicket);
extern int PrintENG_ReadStatus (void);

extern char PrintDIE_Init (void);
extern char PrintDIE_Ticket (TICKET formatTicket);

extern char PrintBEMA_Init (void);
extern char PrintBEMA_Ticket (TICKET formatTicket);

extern char PrintENG_CompileBitmap(FILE *filebmp, IMGPRN *buf, unsigned char orientation);
extern char PrintENG_CompileRamBitmap(unsigned char *source, unsigned short lenSource, unsigned char *logo, unsigned short *width,unsigned short *height, unsigned char orientation);
#endif

 /*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
