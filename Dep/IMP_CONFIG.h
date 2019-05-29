/*----------------------------------------------------------------------------
 *      Biblioteca:	LPC176x_IMP.lib
 *      Proposito:	Biblioteca com funções para impressão 
 *----------------------------------------------------------------------------
 *      Arquivo:    IMP_CONFIG.h
 *      Proposito: 	Arquivo de definições do usuário para impressoras
 *----------------------------------------------------------------------------
 *      Versão: 	1.00 	Prog: Ricardo T.   Data: 25/04/2013
 * 		Descrição:  Versão inicial
 *----------------------------------------------------------------------------
 *      Este código pertence a Wyma Tecnologia e Sistemas LTDA.
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
/* 	Head 1: previsão, hora, texto..	*/
/*	Head 2: previsão, hora, texto..	*/	
/*	Head 3: previsão, hora, texto..	*/	
/*		A1234  -  Small/Large		*/  
/*			  department			*/
/*	Foot 1: previsão, hora, texto..	*/
/*	Foot 2: previsão, hora, texto..	*/
/*	Foot 3: previsão, hora, texto..	*/	
/*	Foot 4: previsão, hora, texto..	*/
/*----------------------------------*/

/*----------------------------------------------*/
/* Informação de cada linha de texto pode ser: 	*/
/* 	0=Mostra texto com caractere padrão			*/
/* 	1=Mostra texto com caractere expandido 		*/
/* 	2=Mostra Previsão espera 					*/
/* 	3=mostra Data e Hora	   					*/
/*----------------------------------------------*/

typedef struct ticket
{
	unsigned short	widthImage;		  	// Largura da imagem do logo
	unsigned short	heightImage;		// Altura da imagem do logo
	unsigned char 	*bufImage;			// ponteiro para a imagem do logo
	char 			*headerLine1;		// Ponteiro para o texto do cabeçalho1
	unsigned char	infoHeader1;		// Configura a informação a ser exibida no cabeçalho 1
	char 			*headerLine2;		// Ponteiro para o texto do cabeçalho1
	unsigned char	infoHeader2;		// Configura a informação a ser exibida no cabeçalho 1
	char 			*headerLine3;		// Ponteiro para o texto do cabeçalho1
	unsigned char	infoHeader3;		// Configura a informação a ser exibida no cabeçalho 1
	char 			alphaNum;			// Letra do Ticket

	unsigned short 	number;				// Número do Ticket
	unsigned char	lenNumber;	   		// Quantidade de caracteres do número do ticket
	unsigned char	typeNumber;			// Impressão do Número do ticket: 0=normal; 1=gráfico padrão impressora; 2=gráfico wyma
 	unsigned short	widthNumber;	  	// Largura da imagem do ticket
	unsigned short	heightNumber;		// Altura da imagem do ticket
	unsigned char 	*bufNumber;			// ponteiro para a imagem do ticket

	char 			*department;		// Ponteiro para o texto do departamento
	unsigned char	infoDep;			// Configura a informação a ser exibida no departamento
	char 			*footerLine1;		// Ponteiro para o texto do rodapé 1
	unsigned char	infoFooter1;		// Configura a informação a ser exibida no rodapé 1
	char 			*footerLine2;		// Ponteiro para o texto do rodapé 2
	unsigned char	infoFooter2;		// Configura a informação a ser exibida no rodapé 2
	char 			*footerLine3;		// Ponteiro para o texto do rodapé 3	   
	unsigned char	infoFooter3;		// Configura a informação a ser exibida no rodapé 3
	char 			*footerLine4;		// Ponteiro para o texto do rodapé 4
	unsigned char	infoFooter4;		// Configura a informação a ser exibida no rodapé 4
	char			*strNumProtocol;	// Ponteiro para a informação do número do protocolo
	unsigned char	cut;				// Tipo de corte: 0=corte total 1=corte parcial
	unsigned char	copy;				// Número de vias do ticket
	unsigned char	orientation;		// orientação da impressão normal(1)/invertida(0)
}TICKET;

/*----------------------------------------------------------------------------
 *      User type definitions for Ticket bitmap
 *---------------------------------------------------------------------------*/

typedef struct imgPrn
{
	unsigned char bitmap[1024];					// Imagem até 128x64 = 1024 bits
												// Alterar este valor pode não ser suficiente para suportar 
												// imagens maiores, é necessário aumentar também o buffer 
												// do interpretador do arquivo na LIB IMP, IMGPRN são os 
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
