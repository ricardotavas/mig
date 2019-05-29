/*----------------------------------------------------------------------------
 *      Nome:    DataFlash.h
 *      Proposito: Fun��es para manuseio do ponteiro da Flash de dados. 
 *      Rev.:    V1.00
 *----------------------------------------------------------------------------
 *      Este c�digo pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#ifndef __SPIDATAFLASH
#define __SPIDATAFLASH

/* Globals Definitions ----------------------------------------------------- */

#define		LEN_VALID_DAT	0x0020		/* Quantidade de bytes v�lidos para gravar.	*/
										/* (32)										*/
#define		LEN_WAITFILE	0x0500  	/* tamanho do buffer dos bytes v�lidos 		*/
										/* (1280)									*/

/* Exported functions -------------------------------------------------------*/

extern void Init_Address_DataFlash (void);
extern void Erase_DataFlash (void);
extern unsigned int DataFlash_Exist_NotRead (void);
extern char DataFlash_LimitReach (void);

// Fun��es para leitura sequ�ncial (v�rios endere�os em sequ�ncia).
extern unsigned int SetReader_Begin (void);
extern unsigned int SetReader_End (void);
extern char ReaderFlash_fromBegin (unsigned char *data);
extern char ReaderFlash_fromEnd (unsigned char *data);

// Fun��es para leitura livre (1 endere�o).
extern unsigned int GetReader (void);
extern char SetReader (unsigned int addr);
extern char ReaderFlash (unsigned char *data);

// Fun��es para grava��o e ajuste da �rea ativa da Flash.
extern char Adjust_ReadDataFlash (void);
extern void Write_DataFlash (unsigned char *data, unsigned char len);
extern void ReWrite_DataFlash (unsigned char *pDat, unsigned int destAddr, unsigned char offset, unsigned char len);


#endif

/*----------------------------------------------------------------------------
 * Fim do arquivo
 *---------------------------------------------------------------------------*/

