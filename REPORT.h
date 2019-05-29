/*----------------------------------------------------------------------------
 *      Arquivo:    REPORT.h
 *      Proposito: 	Gerenciamento dos arquivos de relatório
 *----------------------------------------------------------------------------
 *      Versão: 	1.00 	Prog: Ricardo T.   Data: 25/04/2013
 * 		Descrição:  Versão inicial
 *----------------------------------------------------------------------------
 *      Este código pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#ifndef		__REPORT__
#define		__REPORT__

/*----------------------------------------------------------------------------
 *      User type definitions
 *---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *      Extern variables
 *---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *      Extern functions
 *---------------------------------------------------------------------------*/

extern unsigned char Format_Drive (void);
extern unsigned char Format_DataFlash (void);
extern unsigned char Change_Drive (unsigned char driver);
extern void Check_RequestWeb (void);
extern void Check_FileSystem (void);
extern void StateMachine_ticket (void);
extern void Prog_Watchdog (unsigned int value);
extern void InitFilesClient (void);

#endif


 /*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
