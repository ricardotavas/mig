/*----------------------------------------------------------------------------
 *      Nome:    DataFlash.h
 *      Proposito: Funções para manuseio do ponteiro da Flash de dados. 
 *      Rev.:    V1.00
 *----------------------------------------------------------------------------
 *      Este código pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#ifndef __SPIDATAFLASH
#define __SPIDATAFLASH

/* Globals Definitions ----------------------------------------------------- */

#define		LEN_VALID_DAT	0x0020		/* Quantidade de bytes válidos para gravar.	*/
										/* (32)										*/
#define		LEN_WAITFILE	0x0500  	/* tamanho do buffer dos bytes válidos 		*/
										/* (1280)									*/

/* Exported functions -------------------------------------------------------*/

extern void Init_Address_DataFlash (void);
extern void Erase_DataFlash (void);
extern unsigned int DataFlash_Exist_NotRead (void);
extern char DataFlash_LimitReach (void);

// Funções para leitura sequêncial (vários endereços em sequência).
extern unsigned int SetReader_Begin (void);
extern unsigned int SetReader_End (void);
extern char ReaderFlash_fromBegin (unsigned char *data);
extern char ReaderFlash_fromEnd (unsigned char *data);

// Funções para leitura livre (1 endereço).
extern unsigned int GetReader (void);
extern char SetReader (unsigned int addr);
extern char ReaderFlash (unsigned char *data);

// Funções para gravação e ajuste da área ativa da Flash.
extern char Adjust_ReadDataFlash (void);
extern void Write_DataFlash (unsigned char *data, unsigned char len);
extern void ReWrite_DataFlash (unsigned char *pDat, unsigned int destAddr, unsigned char offset, unsigned char len);


#endif

/*----------------------------------------------------------------------------
 * Fim do arquivo
 *---------------------------------------------------------------------------*/

