/*----------------------------------------------------------------------------
 *      Arquivo:    CRC16.h
 *      Proposito: 	Implementa��o do algoritmo CRC16 da norma ISO/IEC 3309
 *----------------------------------------------------------------------------
 *      Vers�o: 	1.00 	Prog: Ricardo T.   Data: 25/04/2013
 * 		Descri��o:  Vers�o inicial
 *----------------------------------------------------------------------------
 *      Este c�digo pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/


#ifndef		__CRC16__
#define		__CRC16__

/*----------------------------------------------------------------------------
 *      Exported functions
 *---------------------------------------------------------------------------*/

extern void init_CRC16 (unsigned short value);
extern unsigned short calc_CRC16 (unsigned char *data, unsigned short length);

#endif

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

