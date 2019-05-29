/*----------------------------------------------------------------------------
 *      Biblioteca:	LPC176x_IAP.lib
 *      Proposito:	Biblioteca com funções In-Application Programming (IAP)
 *----------------------------------------------------------------------------
 *      Arquivo:    IAP_CONFIG.c
 *      Proposito: 	Funções de leitura e gravação usando IAP
 *----------------------------------------------------------------------------
 *      Versão: 	1.00 	Prog: Ricardo T.   Data: 25/04/2013
 * 		Descrição:  Versão inicial
 *----------------------------------------------------------------------------
 *      Este código pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#ifndef	__IAP_CONFIG__
#define	__IAP_CONFIG__

/*----------------------------------------------------------------------------
 *      Definitions for IAP
 *---------------------------------------------------------------------------*/
#define			ADDRESS_USER_CONFIG	0x0000E000	// Endereço da flash para configurações do usuário
#define			INI_SECTOR_CONFIG	14			// Setor para configurações do usuário
#define			END_SECTOR_CONFIG	14

#define			ADDRESS_USER_CODE	0x00010000	// Endereço da flash para o código do usuário
#define			INI_SECTOR_CODE		16			// Setor para o código do usuário
#define			END_SECTOR_CODE		29

/* Exported functions ----------------------------------------------------- */
extern void ExecuteUserCode(void);
extern unsigned int EraseFlashSector (unsigned int iniSector, unsigned int endSector);
extern void InitOffset (unsigned int value);
extern unsigned int WriteFlashPage (unsigned int iniSector, unsigned int endSector, void *bufRAM);
extern unsigned int ReadFlashPage (unsigned int iniSector, unsigned int endSector, void *bufRAM);
extern unsigned int CompareBlank (unsigned int iniSector, unsigned int endSector);
extern unsigned int WriteConfig (unsigned int sector, void *var, unsigned int length);
extern void ReadConfig (unsigned int srcAddress, void *var, unsigned int size);
extern void CheckConfig (void);

#endif

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
