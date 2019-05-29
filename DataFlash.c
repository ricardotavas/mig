/*----------------------------------------------------------------------------
 *      Nome:    DataFlash.C
 *      Proposito: Fun��es para manuseio do ponteiro da Flash de dados. 
 *      Rev.:    V1.00
 *----------------------------------------------------------------------------
 *      Este c�digo pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#include 	"SPI_CONFIG.h"
#include 	"DataFlash.h"

#define		BASEFLASH_PONT		0x000000		// Endere�o base da �rea de ponteiros
#define		TOPOFLASH_PONT		0x00FFFF		// Endere�o final da �rea de ponteiros
#define		BASEFLASH_DADOS		0x010000		// Endere�o base de dados
#define		TOPOFLASH_DADOS		0x07FFFF		// Endere�o final de dados

#define		flagIN				1
#define		flagOUT				2

unsigned int adrIN;	 		// Armazena �ltimo endere�o de entrada (para gravar), na �rea de dados.
unsigned int adrOUT;		// Armazena �ltimo endere�o de sa�da (para recuperar), na �rea de dados.
unsigned int pontAddr;		// Armazena pr�xima posi��o livre para guardar novos endere�os IN e OUT.

unsigned int pontReader; 	// Armazena endere�o de leitura na �rea de dados.
unsigned int adrLast;		// Armazena �ltimo endere�o lido com o ponteiro de leitura "pontReader"

/* ------------------------------------------------------------------------------------ 
 * Nome: 		Init_Address_DataFlash 													
 * Tarefa: 		Posiciona ponteiro e inicializa endere�os de escrita e leitura na Flash	
 * Par�metro: 	Nenhum                                          						
 * Retorno:		Nenhum   
 -------------------------------------------------------------------------------------- */
void Init_Address_DataFlash (void)
{
	unsigned char TEMP[8];
	unsigned int PONT_TEMP;
	
	PONT_TEMP = ( TOPOFLASH_PONT + 1 );

	do
	{
		PONT_TEMP -= 8;
  		spi_Flash_Read (TEMP, PONT_TEMP, 1);
	}
	while ((TEMP[0]==0xFF) && (PONT_TEMP!=0));

	if (TEMP[0]!=0xFF)
	{
 		pontAddr = (PONT_TEMP+8);

		spi_Flash_Read (TEMP, PONT_TEMP, 8);
		adrIN = ( (TEMP[3]<<16) + (TEMP[2]<<8) + (TEMP[1]) );			
		adrOUT = ( (TEMP[7]<<16) + (TEMP[6]<<8) + (TEMP[5]) );
	}
	else
	{
		// Tudo 0xFF no setor de ponteiros indica mem�ria nova.
		// Formata �rea de dados para garantir 0xFF no setor dados.
		spi_EraseSector (BASEFLASH_DADOS);

		// Grava endere�os iniciais e inicializa ponteiro na base.
		adrIN = BASEFLASH_DADOS;
		adrOUT = BASEFLASH_DADOS;
		pontAddr = BASEFLASH_PONT;
	}
}

/* ------------------------------------------------------------------------------------ 
 * Nome: 		Erase_DataFlash		 													
 * Tarefa: 		Apaga toda a mem�ria Flash e inicializa o ponteiro e endere�os          
 * Parametro:	Nenhum                
 * Retorno:		Nenhum                
 -------------------------------------------------------------------------------------- */
void Erase_DataFlash (void)
{
	spi_EraseChip ();

	adrIN = BASEFLASH_DADOS;
	adrOUT = BASEFLASH_DADOS;
	pontAddr = BASEFLASH_PONT;
}

/* ------------------------------------------------------------------------------------ 
 * Nome: 		DataFlash_Exist_NotRead													
 * Tarefa: 		Verifica se exitem dados n�o lidos na mem�ria flash.    
 * Parametro:	Nenhum                
 * Retorno: 	N�mero de bytes n�o lidos												
 -------------------------------------------------------------------------------------- */
unsigned int DataFlash_Exist_NotRead (void)
{
	unsigned int len=0;

	if (adrIN != adrOUT)
	{
		if (adrIN>adrOUT)
			len=adrIN-adrOUT;
		else
			len=(TOPOFLASH_DADOS-adrOUT)+adrIN;
	}

	return len;
}

/* ------------------------------------------------------------------------------------  
 * Fun��o: 		SetReader_Begin
 * Descri��o:   Copia o endere�o do primeiro dado gravado para um ponteiro de leitura 	 
 * 				para ler dados com a fun��o ReaderFlash_fromBegin() sem alterar o 
 *				ponteiro original de leitura/grava��o.
 * Par�metro: 	Nenhum
 * Retorno: 	Endere�o de leitura
 -------------------------------------------------------------------------------------- */
unsigned int SetReader_Begin (void)
{
	return (pontReader=adrOUT);
}

/* ------------------------------------------------------------------------------------ 
 * Fun��o: 		SetReader_End
 * Descri��o:   Copia o endere�o do �ltimo dado gravado para um ponteiro leitor	 	
 * 				para ler dados com a fun��o ReaderFlash_fromEnd() sem alterar o 
 *				ponteiro original de leitura/grava��o.
 * Par�metro: 	Nenhum
 * Retorno: 	Endere�o de leitura
 -------------------------------------------------------------------------------------- */
unsigned int SetReader_End (void)
{
	return (pontReader=adrIN);
}

/* ------------------------------------------------------------------------------------ 
 * Fun��o: 		ReaderFlash_fromBegin  													
 * Descri��o:   L� os dados da Flash a partir do ponteiro (OUT) e retorna um bloco de	
 *				(LEN_VALID_DAT) bytes de dados,	posiciona o ponteiro leitor para ler os 
 *				pr�ximos dados enquanto n�o atingir o ponteiro (IN).
 * Pr�Requisito: Antes da primeira chamada a esta fun��o chame a fun��o	SetReader_Begin()			
 * Parametro: 	*data - Ponteiro para o buffer destino									
 * Retorno:		(0) N�o h� mais dados para ler (1) Sucesso
 ------------------------------------------------------------------------------------- */
char ReaderFlash_fromBegin (unsigned char *data)
{
	if (adrIN != pontReader)
	{
		// L� no endere�o atual e armazena no buffer indicado por *data.
		spi_Flash_Read (data, pontReader, LEN_VALID_DAT);

		// Armazena pr�ximo endere�o ainda n�o lido.
		pontReader+=LEN_VALID_DAT;

		if (pontReader==(TOPOFLASH_DADOS+1))
			pontReader=BASEFLASH_DADOS;

		return 1;
	}
	return 0;
}

/* ------------------------------------------------------------------------------------ 
 * Fun��o: 		ReaderFlash_fromEnd  													
 * Descri��o:   L� os dados da Flash a partir do ponteiro (IN) e retorna um bloco de	
 *				(LEN_VALID_DAT) bytes de dados,	posiciona o ponteiro leitor para ler os 
 *				pr�ximos dados enquanto n�o atingir o ponteiro (OUT).	
 * Pr�Requisito: Antes da primeira chamada a esta fun��o chame a fun��o	SetReader_End()			
 * Par�metro: 	*data - Ponteiro para o buffer destino									
 * Retorno:		(0) N�o h� mais dados para ler (1) Sucesso
 -------------------------------------------------------------------------------------- */
char ReaderFlash_fromEnd (unsigned char *data)
{
	if (adrOUT != pontReader)
	{
		if (pontReader==BASEFLASH_DADOS)
			pontReader=TOPOFLASH_DADOS+1;

		// L� no endere�o anterior e armazena no buffer indicado por *data.
		spi_Flash_Read (data, pontReader-LEN_VALID_DAT, LEN_VALID_DAT);

		// Armazena pr�ximo endere�o ainda n�o lido.
		pontReader-=LEN_VALID_DAT;

		return 1;
	}
	return 0;
}

/* ------------------------------------------------------------------------------------  
 * Fun��o: 		GetReader	
 * Descri��o:   Retorna o endere�o do ponteiro de leitura. 
 * Par�metro: 	Nenhum
 * Retorno: 	Nenhum
 -------------------------------------------------------------------------------------- */
unsigned int GetReader (void)
{
	return pontReader;
}

/* ------------------------------------------------------------------------------------  
 * Fun��o: 		SetReader	
 * Descri��o:   Ajusta o ponteiro de leitura em endere�o especifico. 
 * Par�metro: 	addr - Endere�o de leitura
 * Retorno: 	(0) N�o h� dados para ler (1) Sucesso
 -------------------------------------------------------------------------------------- */
char SetReader (unsigned int addr)
{
	if (addr>=adrOUT && addr<=adrIN)
	{
		pontReader=addr;
		return 1;
	}
	return 0;
}

/* ------------------------------------------------------------------------------------ 
 * Fun��o: 		ReaderFlash
 * Descri��o:   L� os dados da Flash no endere�o atual e retorna um bloco de	
 *				(LEN_VALID_DAT) bytes de dados.
 * Parametro: 	*data - Ponteiro para o buffer destino									
 * Retorno:		(0) N�o h� dados para ler (1) Sucesso
 ------------------------------------------------------------------------------------- */
char ReaderFlash (unsigned char *data)
{
	if (pontReader>=adrOUT && pontReader<=adrIN)
	{
		spi_Flash_Read (data, pontReader, LEN_VALID_DAT);
		return 1;
	}
	return 0;

}

/* ------------------------------------------------------------------------------------ 
 * Fun��o: 		Adjust_ReadDataFlash 
 * Descri��o:	Posiciona ponteiro de leitura da flash pula bytes(LEN_VALID_DAT), ja 
 *				gravados em arquivo. Chame esta fun��o ap�s leitura com a fun��o 
 *				ReaderFlash_fromBegin e ap�s os dados serem gravados em arquivo com sucesso.
 * Par�metro:   Nenhum
 * Retorno:   	(0) N�o h� dados   (1) Sucesso
 -------------------------------------------------------------------------------------- */
char Adjust_ReadDataFlash (void)
{
	unsigned char TEMP[8];

	if (adrIN != adrOUT)
	{
		adrOUT+=LEN_VALID_DAT;
		if (adrOUT==(TOPOFLASH_DADOS+1))
			adrOUT=BASEFLASH_DADOS;

		// Prepara o endere�o para gravar no ponteiro, caso a energia caia recupera-se 
		// o �ltimo endere�o livre atrav�s dele.
		TEMP[0] = flagIN;
		TEMP[1] = (adrIN & 0x000000FF);
		TEMP[2] = (adrIN & 0x0000FF00) >> 8;
		TEMP[3] = (adrIN & 0x00FF0000) >> 16;
		TEMP[4] = flagOUT;
		TEMP[5] = (adrOUT & 0x000000FF);
		TEMP[6] = (adrOUT & 0x0000FF00) >> 8;
		TEMP[7] = (adrOUT & 0x00FF0000) >> 16;
		
		if (pontAddr!=(TOPOFLASH_PONT+1))
		{
			// Grava na posi��o atual do ponteiro o pr�ximo endere�o livre para gravar.
			spi_Flash_Write (TEMP, pontAddr, 8);
			// Vai para pr�xima posi��o livre do ponteiro.
			pontAddr +=8;
		}
		else
		{
			// Formata setor dos ponteiros para receber novos dados.
			spi_EraseSector (BASEFLASH_PONT);
			pontAddr = BASEFLASH_PONT;
		
			// Grava na posi��o atual do ponteiro o pr�ximo endere�o livre para gravar.
			spi_Flash_Write (TEMP, pontAddr, 8);
			// Vai para pr�xima posi��o livre do ponteiro.
			pontAddr +=8;
		}

		return 1;
	}
	else
	{
		return 0;
	}
}

/* ------------------------------------------------------------------------------------ 
 * Nome: 		Write_DataFlash      													
 * Tarefa: 	    Grava os bytes (LEN_VALID_DAT) indicados pelo ponteiro					
 * Par�metro: 	*data - Informa onde est�o os dados a serem gravados na flash					
 *				len - Comprimento dos dados (m�x = LEN_VALID_DAT)
 * Retorno:		Nenhum
 -------------------------------------------------------------------------------------- */
void Write_DataFlash (unsigned char *data, unsigned char len)
{
	unsigned char TEMP[8];

	// Grava no endere�o atual os dados de cadastro do ticket do buffer indicado por *data. 
	spi_Flash_Write (data, adrIN, len);
	// Armazena pr�ximo endere�o livre para grava��o de dados.
	adrIN+=LEN_VALID_DAT;

	if (adrIN==(TOPOFLASH_DADOS+1))
		adrIN=BASEFLASH_DADOS;

	// Verifica se caiu em endere�o base de setor da flash, ex: 0x00010000, 0x00020000, 0x00030000..
	// Se caiu formata o setor para iniciar as grava��es.
	if ((adrIN & 0x0000FFFF)==0)  
	 	spi_EraseSector (adrIN);
	
	// Prepara o endere�o para gravar no ponteiro, caso a energia caia recupera-se 
	// o �ltimo endere�o livre atrav�s dele.
	TEMP[0] = flagIN;
	TEMP[1] = (adrIN & 0x000000FF);
	TEMP[2] = (adrIN & 0x0000FF00) >> 8;
	TEMP[3] = (adrIN & 0x00FF0000) >> 16;
	TEMP[4] = flagOUT;
	TEMP[5] = (adrOUT & 0x000000FF);
	TEMP[6] = (adrOUT & 0x0000FF00) >> 8;
	TEMP[7] = (adrOUT & 0x00FF0000) >> 16;

	if (pontAddr!=(TOPOFLASH_PONT+1))
	{
		// Grava na posi��o atual do ponteiro o pr�ximo endere�o livre para gravar.
		spi_Flash_Write (TEMP, pontAddr, 8);
		// Vai para pr�xima posi��o livre do ponteiro.
		pontAddr +=8;
	}
	else
	{
		// Formata setor dos ponteiros para receber novos dados.
		spi_EraseSector (BASEFLASH_PONT);
		pontAddr = BASEFLASH_PONT;
		
		// Grava na posi��o atual do ponteiro o pr�ximo endere�o livre para gravar.
		spi_Flash_Write (TEMP, pontAddr, 8);
		// Vai para pr�xima posi��o livre do ponteiro.
		pontAddr +=8;
	}
}

/* ------------------------------------------------------------------------------------  
 * Fun��o: 		ReWrite_DataFlash	    													 
 * Descri��o:   Grava dados em endere�os espec�ficos, para completar uma grava��o anterior.
 * Par�metro: 	*pDat - Ponteiro para buffer onde est�o os dados
 *				destAddr - Endere�o da Flash para gravar os dados
 *				offset - Ajusta a posi��o de grava��o dentro do bloco (LEN_VALID_DAT)
 *				len - quantidade de byte. A soma (len+offset) n�o pode exceder (LEN_VALID_DAT)
 * Retorno: 	Nenhum	  
 -------------------------------------------------------------------------------------- */
void ReWrite_DataFlash (unsigned char *pDat, unsigned int destAddr, unsigned char offset, unsigned char len)
{
	spi_Flash_Write (pDat + offset, destAddr + offset, len);
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
