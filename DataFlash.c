/*----------------------------------------------------------------------------
 *      Nome:    DataFlash.C
 *      Proposito: Funções para manuseio do ponteiro da Flash de dados. 
 *      Rev.:    V1.00
 *----------------------------------------------------------------------------
 *      Este código pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#include 	"SPI_CONFIG.h"
#include 	"DataFlash.h"

#define		BASEFLASH_PONT		0x000000		// Endereço base da área de ponteiros
#define		TOPOFLASH_PONT		0x00FFFF		// Endereço final da área de ponteiros
#define		BASEFLASH_DADOS		0x010000		// Endereço base de dados
#define		TOPOFLASH_DADOS		0x07FFFF		// Endereço final de dados

#define		flagIN				1
#define		flagOUT				2

unsigned int adrIN;	 		// Armazena último endereço de entrada (para gravar), na área de dados.
unsigned int adrOUT;		// Armazena último endereço de saída (para recuperar), na área de dados.
unsigned int pontAddr;		// Armazena próxima posição livre para guardar novos endereços IN e OUT.

unsigned int pontReader; 	// Armazena endereço de leitura na área de dados.
unsigned int adrLast;		// Armazena último endereço lido com o ponteiro de leitura "pontReader"

/* ------------------------------------------------------------------------------------ 
 * Nome: 		Init_Address_DataFlash 													
 * Tarefa: 		Posiciona ponteiro e inicializa endereços de escrita e leitura na Flash	
 * Parâmetro: 	Nenhum                                          						
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
		// Tudo 0xFF no setor de ponteiros indica memória nova.
		// Formata área de dados para garantir 0xFF no setor dados.
		spi_EraseSector (BASEFLASH_DADOS);

		// Grava endereços iniciais e inicializa ponteiro na base.
		adrIN = BASEFLASH_DADOS;
		adrOUT = BASEFLASH_DADOS;
		pontAddr = BASEFLASH_PONT;
	}
}

/* ------------------------------------------------------------------------------------ 
 * Nome: 		Erase_DataFlash		 													
 * Tarefa: 		Apaga toda a memória Flash e inicializa o ponteiro e endereços          
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
 * Tarefa: 		Verifica se exitem dados não lidos na memória flash.    
 * Parametro:	Nenhum                
 * Retorno: 	Número de bytes não lidos												
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
 * Função: 		SetReader_Begin
 * Descrição:   Copia o endereço do primeiro dado gravado para um ponteiro de leitura 	 
 * 				para ler dados com a função ReaderFlash_fromBegin() sem alterar o 
 *				ponteiro original de leitura/gravação.
 * Parâmetro: 	Nenhum
 * Retorno: 	Endereço de leitura
 -------------------------------------------------------------------------------------- */
unsigned int SetReader_Begin (void)
{
	return (pontReader=adrOUT);
}

/* ------------------------------------------------------------------------------------ 
 * Função: 		SetReader_End
 * Descrição:   Copia o endereço do último dado gravado para um ponteiro leitor	 	
 * 				para ler dados com a função ReaderFlash_fromEnd() sem alterar o 
 *				ponteiro original de leitura/gravação.
 * Parâmetro: 	Nenhum
 * Retorno: 	Endereço de leitura
 -------------------------------------------------------------------------------------- */
unsigned int SetReader_End (void)
{
	return (pontReader=adrIN);
}

/* ------------------------------------------------------------------------------------ 
 * Função: 		ReaderFlash_fromBegin  													
 * Descrição:   Lê os dados da Flash a partir do ponteiro (OUT) e retorna um bloco de	
 *				(LEN_VALID_DAT) bytes de dados,	posiciona o ponteiro leitor para ler os 
 *				próximos dados enquanto não atingir o ponteiro (IN).
 * PréRequisito: Antes da primeira chamada a esta função chame a função	SetReader_Begin()			
 * Parametro: 	*data - Ponteiro para o buffer destino									
 * Retorno:		(0) Não há mais dados para ler (1) Sucesso
 ------------------------------------------------------------------------------------- */
char ReaderFlash_fromBegin (unsigned char *data)
{
	if (adrIN != pontReader)
	{
		// Lê no endereço atual e armazena no buffer indicado por *data.
		spi_Flash_Read (data, pontReader, LEN_VALID_DAT);

		// Armazena próximo endereço ainda não lido.
		pontReader+=LEN_VALID_DAT;

		if (pontReader==(TOPOFLASH_DADOS+1))
			pontReader=BASEFLASH_DADOS;

		return 1;
	}
	return 0;
}

/* ------------------------------------------------------------------------------------ 
 * Função: 		ReaderFlash_fromEnd  													
 * Descrição:   Lê os dados da Flash a partir do ponteiro (IN) e retorna um bloco de	
 *				(LEN_VALID_DAT) bytes de dados,	posiciona o ponteiro leitor para ler os 
 *				próximos dados enquanto não atingir o ponteiro (OUT).	
 * PréRequisito: Antes da primeira chamada a esta função chame a função	SetReader_End()			
 * Parâmetro: 	*data - Ponteiro para o buffer destino									
 * Retorno:		(0) Não há mais dados para ler (1) Sucesso
 -------------------------------------------------------------------------------------- */
char ReaderFlash_fromEnd (unsigned char *data)
{
	if (adrOUT != pontReader)
	{
		if (pontReader==BASEFLASH_DADOS)
			pontReader=TOPOFLASH_DADOS+1;

		// Lê no endereço anterior e armazena no buffer indicado por *data.
		spi_Flash_Read (data, pontReader-LEN_VALID_DAT, LEN_VALID_DAT);

		// Armazena próximo endereço ainda não lido.
		pontReader-=LEN_VALID_DAT;

		return 1;
	}
	return 0;
}

/* ------------------------------------------------------------------------------------  
 * Função: 		GetReader	
 * Descrição:   Retorna o endereço do ponteiro de leitura. 
 * Parâmetro: 	Nenhum
 * Retorno: 	Nenhum
 -------------------------------------------------------------------------------------- */
unsigned int GetReader (void)
{
	return pontReader;
}

/* ------------------------------------------------------------------------------------  
 * Função: 		SetReader	
 * Descrição:   Ajusta o ponteiro de leitura em endereço especifico. 
 * Parâmetro: 	addr - Endereço de leitura
 * Retorno: 	(0) Não há dados para ler (1) Sucesso
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
 * Função: 		ReaderFlash
 * Descrição:   Lê os dados da Flash no endereço atual e retorna um bloco de	
 *				(LEN_VALID_DAT) bytes de dados.
 * Parametro: 	*data - Ponteiro para o buffer destino									
 * Retorno:		(0) Não há dados para ler (1) Sucesso
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
 * Função: 		Adjust_ReadDataFlash 
 * Descrição:	Posiciona ponteiro de leitura da flash pula bytes(LEN_VALID_DAT), ja 
 *				gravados em arquivo. Chame esta função após leitura com a função 
 *				ReaderFlash_fromBegin e após os dados serem gravados em arquivo com sucesso.
 * Parâmetro:   Nenhum
 * Retorno:   	(0) Não há dados   (1) Sucesso
 -------------------------------------------------------------------------------------- */
char Adjust_ReadDataFlash (void)
{
	unsigned char TEMP[8];

	if (adrIN != adrOUT)
	{
		adrOUT+=LEN_VALID_DAT;
		if (adrOUT==(TOPOFLASH_DADOS+1))
			adrOUT=BASEFLASH_DADOS;

		// Prepara o endereço para gravar no ponteiro, caso a energia caia recupera-se 
		// o último endereço livre através dele.
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
			// Grava na posição atual do ponteiro o próximo endereço livre para gravar.
			spi_Flash_Write (TEMP, pontAddr, 8);
			// Vai para próxima posição livre do ponteiro.
			pontAddr +=8;
		}
		else
		{
			// Formata setor dos ponteiros para receber novos dados.
			spi_EraseSector (BASEFLASH_PONT);
			pontAddr = BASEFLASH_PONT;
		
			// Grava na posição atual do ponteiro o próximo endereço livre para gravar.
			spi_Flash_Write (TEMP, pontAddr, 8);
			// Vai para próxima posição livre do ponteiro.
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
 * Parâmetro: 	*data - Informa onde estão os dados a serem gravados na flash					
 *				len - Comprimento dos dados (máx = LEN_VALID_DAT)
 * Retorno:		Nenhum
 -------------------------------------------------------------------------------------- */
void Write_DataFlash (unsigned char *data, unsigned char len)
{
	unsigned char TEMP[8];

	// Grava no endereço atual os dados de cadastro do ticket do buffer indicado por *data. 
	spi_Flash_Write (data, adrIN, len);
	// Armazena próximo endereço livre para gravação de dados.
	adrIN+=LEN_VALID_DAT;

	if (adrIN==(TOPOFLASH_DADOS+1))
		adrIN=BASEFLASH_DADOS;

	// Verifica se caiu em endereço base de setor da flash, ex: 0x00010000, 0x00020000, 0x00030000..
	// Se caiu formata o setor para iniciar as gravações.
	if ((adrIN & 0x0000FFFF)==0)  
	 	spi_EraseSector (adrIN);
	
	// Prepara o endereço para gravar no ponteiro, caso a energia caia recupera-se 
	// o último endereço livre através dele.
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
		// Grava na posição atual do ponteiro o próximo endereço livre para gravar.
		spi_Flash_Write (TEMP, pontAddr, 8);
		// Vai para próxima posição livre do ponteiro.
		pontAddr +=8;
	}
	else
	{
		// Formata setor dos ponteiros para receber novos dados.
		spi_EraseSector (BASEFLASH_PONT);
		pontAddr = BASEFLASH_PONT;
		
		// Grava na posição atual do ponteiro o próximo endereço livre para gravar.
		spi_Flash_Write (TEMP, pontAddr, 8);
		// Vai para próxima posição livre do ponteiro.
		pontAddr +=8;
	}
}

/* ------------------------------------------------------------------------------------  
 * Função: 		ReWrite_DataFlash	    													 
 * Descrição:   Grava dados em endereços específicos, para completar uma gravação anterior.
 * Parâmetro: 	*pDat - Ponteiro para buffer onde estão os dados
 *				destAddr - Endereço da Flash para gravar os dados
 *				offset - Ajusta a posição de gravação dentro do bloco (LEN_VALID_DAT)
 *				len - quantidade de byte. A soma (len+offset) não pode exceder (LEN_VALID_DAT)
 * Retorno: 	Nenhum	  
 -------------------------------------------------------------------------------------- */
void ReWrite_DataFlash (unsigned char *pDat, unsigned int destAddr, unsigned char offset, unsigned char len)
{
	spi_Flash_Write (pDat + offset, destAddr + offset, len);
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
