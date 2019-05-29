/*----------------------------------------------------------------------------
 *      Biblioteca:	LPC176xCOMM.lib
 *      Proposito:	Biblioteca com fun��es do protocolo Wyma
 *----------------------------------------------------------------------------
 *      Arquivo:    HTTP.h
 *      Proposito:	Fun��es de tratamento de frames da p�gina http wifi
 *----------------------------------------------------------------------------
 *      Vers�o: 	1.00 	Prog: Ricardo T.   Data: 27/06/2016
 * 		Descri��o:  Vers�o inicial
 *----------------------------------------------------------------------------
 *      Este c�digo pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#ifndef HTTP_H_
#define HTTP_H_

#include 	<string.h>

/*----------------------------------------------------------------------------
 *      Definition id for commands following http req
 *---------------------------------------------------------------------------*/
#define		CMDFW_NONE		0
#define		CMDFW_BOOT		1
#define		CMDFW_SCAN		2
#define		CMDFW_DIR		3
#define		CMDFW_FILE		4

/*----------------------------------------------------------------------------
 *      Definition id for methods http
 *---------------------------------------------------------------------------*/
#define		METHOD_GET 		0
#define		METHOD_PUT		1
#define		METHOD_POST		2
#define		METHOD_DELETE	3

/*----------------------------------------------------------------------------
 *      Export functions
 *---------------------------------------------------------------------------*/
unsigned char https_api_request_numFrames(unsigned char len, unsigned char *data);
unsigned char https_api_request_cmd(unsigned char method, unsigned char len, unsigned char *data);
void https_api_request(unsigned char frames, unsigned int request, unsigned char method, unsigned char len, unsigned char *data);
void https_api_request_header(unsigned char len, unsigned char *data);
void https_api_request_data(unsigned char data_len, unsigned char *data);
void https_api_request_finished(void);
unsigned char api_handle_boot_request_data (void);
unsigned char api_handle_networks_request_data (void);
unsigned char api_handle_dir_request_data (void);
unsigned char api_handle_download_request_data (void);

typedef struct requestHttp
{
	unsigned long	request;							// ID da requisi��o
	unsigned char	method;								// M�todo usado na requisi��o 
	unsigned char 	len;								// Comprimento dos dados da requisi��o
	unsigned char	data[50];							// Tipo de requisi��o "api"
	unsigned char	frmResp;							// Indica quantos frames ser�o devolvidos
	unsigned char	fwCmd;								// Indica se precisa outro comando
}REQHTTP;

typedef struct dataReqHttp
{
	unsigned long request;   			       			// ID da requisi��o corrente
	unsigned long reqPrev;				       			// ID da requisi��o anterior pendente
	unsigned char reqItem;								// Contador de itens da requisi��o
	unsigned char reqEnd;								// Sinaliza requisi��o pendente finalizada
	unsigned short key;   					  			// Item da requisi��o de grava��o pendente
	unsigned short len;									// Comprimento dos dados da grava��o pendente
	unsigned char data[255];							// Dados da req. de grava��o pendente
}DATAREQHTTP;

#endif

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
