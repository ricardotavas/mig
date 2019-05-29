/*----------------------------------------------------------------------------
 *      Biblioteca:	LPC176x_SPI.lib
 *      Proposito:	Biblioteca com funções SPI para todos os dispositivos
 *----------------------------------------------------------------------------
 *      Arquivo:    SPI_CONFIG.h
 *      Proposito: 	Arquivo de definições do usuário
 *----------------------------------------------------------------------------
 *      Versão: 	2.00 	Prog: Ricardo T.   Data: 16/10/2014
 * 		Descrição:  Versão inicial
 *----------------------------------------------------------------------------
 *      Este código pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#ifndef	__SPI_CONFIG__
#define	__SPI_CONFIG__

/* Mask for user --------------------------------------------------------- */

#define MASKSPI_SSP0	0x00000001
#define MASKSPI_EEPROM	0x00000002
#define MASKSPI_FLASH	0x00000004
#define MASKSPI_SRAM	0x00000008
#define	MASKSPI_LCD		0x00000010
#define	MASKSPI_RF		0x00000020

#define	MASKSPI_SSP1	0x00000100
#define	MASKSPI_FLASHFS	0x00000200
#define	MASKSPI_SDCARD	0x00000400

#define	MASKSPI_SPI		0x00002000
#define	MASKSPI_LED		0x00004000
#define MASKSPI_SELBITS	0x00008000
#define MASKSPI_NUMBITS	0x001F0000

#define USE_SSP0		(SPI_userOptions & MASKSPI_SSP0)
#define USE_EEPROM		((SPI_userOptions & MASKSPI_EEPROM)>>1)
#define USE_FLASH		((SPI_userOptions & MASKSPI_FLASH)>>2)
#define USE_SRAM		((SPI_userOptions & MASKSPI_SRAM)>>3)
#define USE_LCD			((SPI_userOptions & MASKSPI_LCD)>>4)
#define USE_RF			((SPI_userOptions & MASKSPI_RF)>>5)
#define USE_SSP1		((SPI_userOptions & MASKSPI_SSP1)>>8)
#define USE_FLASHFS		((SPI_userOptions & MASKSPI_FLASHFS)>>9)
#define USE_SDCARD		((SPI_userOptions & MASKSPI_SDCARD)>>10)
#define USE_SPI			((SPI_userOptions & MASKSPI_SPI)>>13)
#define USE_LED			((SPI_userOptions & MASKSPI_LED)>>14)
#define USE_SELBITS		((SPI_userOptions & MASKSPI_SELBITS)>>15)
#define USE_NUMBITS		((SPI_userOptions & MASKSPI_NUMBITS)>>16)

/* Exported variables ----------------------------------------------------- */

extern unsigned int SPI_userOptions;

/* Exported functions ----------------------------------------------------- */

extern void init_spi (void);

extern void spi_Eeprom_Read (unsigned char *pontbuf, unsigned short iniAddr, unsigned short size);
extern void spi_Eeprom_Write (unsigned char *pontbuf, unsigned short iniAddr, unsigned short size);
extern void spi_Eeprom_Erase (unsigned short iniAddr, unsigned short sizeEeprom);

extern void init_spiSRAM (void);
extern void spi_Sram_Read (unsigned char *pontbuf, unsigned int iniAddr, unsigned int size);
extern void spi_Sram_Write (unsigned char *pontbuf, unsigned int iniAddr, unsigned int size);
extern void spi_Sram_Erase (unsigned short iniAddr, unsigned short sizeSram);

extern void spi_Flash_Init (void);
extern void spi_Flash_Read (unsigned char *pontBuf, unsigned int iniAddr, unsigned int size);
extern void spi_Flash_Write (unsigned char *pontBuf, unsigned int iniAddr, unsigned int size);
extern void spi_EraseSector (unsigned int adr);
extern void spi_EraseChip (void);

extern int spi_FlashFs_Init (void);
extern int spi_FlashFs_EraseSector (unsigned int iniAddr);
extern int spi_FlashFs_ProgramPage (unsigned char *pontbuf, unsigned int iniAddr, unsigned int size);
extern int spi_FlashFs_ReadData (unsigned char *pontbuf, unsigned int iniAddr, unsigned int size);

extern unsigned char spi_Lcd_Read (void);
extern void spi_Lcd_Write (unsigned char dataByte);

extern unsigned short spi_Led_Read (void);
extern void spi_Led_Write (unsigned short data);

extern unsigned char spi_Wifi (unsigned char byte);

#endif

 /*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
