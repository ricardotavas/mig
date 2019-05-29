/*----------------------------------------------------------------------------
 *      Biblioteca:	LPC176xCOMM.lib
 *      Proposito:	Biblioteca com funções do protocolo Wyma
 *----------------------------------------------------------------------------
 *      Arquivo:    HTTP.c
 *      Proposito: 	
 *----------------------------------------------------------------------------
 *      Versão: 	1.00 	Prog: Ricardo T.   Data: 27/06/2016
 * 		Descrição:  Versão inicial
 *----------------------------------------------------------------------------
 *      Este código pertence a Wyma Tecnologia e Sistemas LTDA.
 *---------------------------------------------------------------------------*/

#include 	<stdio.h>
#include 	<File_Config.h>
#include	"IAP_CONFIG.h"
#include 	"RTC_CONFIG.h"
#include	"COMM_CONFIG.h"
#include	"..\\APP.h"
#include 	"..\\QUEUE.h"
#include 	"..\\REPORT.h"

/*----------------------------------------------------------------------------
 *      Definitions for device flash
 *---------------------------------------------------------------------------*/
#define	ID_SAVE			 99
#define ID_BOOT			100
// STATUS.HTML
#define ID_MODE			101
#define ID_SSID			102
#define ID_BSSID		103
#define ID_CHANNEL		104
#define ID_SECURITY		105
#define ID_WIFIPW		106
#define ID_DHCP			107
#define ID_IP			108
#define ID_MASK			109
#define ID_GW			110
#define ID_DNS1			111
#define ID_DNS2			112
#define ID_DHCPS		113
#define ID_DHCPS_IP		114
// SYSTEM.HTML
#define ID_DRIVE		115
#define ID_DATE			116
#define ID_TIME			117
#define ID_TICKETTEST	118
#define ID_TIMELIVE		119
#define ID_CONTROL		120
#define ID_TIMEON		121
#define ID_TIMEOFF		122
#define ID_D1			123
#define ID_D2			124
#define ID_D3			125
#define ID_D4			126
#define ID_D5			127
#define ID_D6			128
#define ID_D7			129
#define ID_REPORTTIME	130
#define ID_REPORTCSV	131
#define ID_MIGMODE		132
#define ID_MIGIP		133
#define ID_LINE			134
#define ID_INTERF		135
#define ID_UPLOAD		136
#define ID_TASK			137
#define ID_DIR			138
#define ID_DOWNLOAD		139
// SELDEP.HTML
#define ID_DNUM			140
#define ID_DNAME1		141
#define ID_DNAME2		142
#define ID_DNAME3		143
#define ID_DNAME4		144
#define ID_DNAME5		145
#define ID_DNAME6		146
// DEP.HTML
#define ID_DENAB		147
#define ID_DNAME		148
#define ID_DALPH		149
#define ID_DRANG1		150
#define ID_DRANG2		151
#define ID_DCLOS1		152
#define ID_DCLOS2		153
#define ID_DCALL1		154
#define ID_DCALL2		155
#define ID_DHLP1		156
#define ID_DHLP2		157
#define ID_DHLP3		158
#define ID_DHLP4		159
#define ID_DHLP5		160
#define ID_DIPD			161
#define ID_DITFD		162
#define ID_DIPT			163
#define ID_DITFT		164
#define ID_ENATOP		165
#define ID_DLIND		166
#define ID_DPRFD		167
#define ID_DU01			168
#define ID_DU02			169
#define ID_DU03			170
#define ID_DU04			171
#define ID_DU05			172
#define ID_DU06			173
#define ID_DU07			174
#define ID_DU08			175
#define ID_DU09			176
#define ID_DU10			177
#define ID_DU11			178
#define ID_DU12			179
#define ID_DU13			180
#define ID_DU14			181
#define ID_DU15			182
#define ID_DU16			183
#define ID_DU17			184
#define ID_DU18			185
#define ID_DU19			186
#define ID_DU20			187
#define ID_DU21			188
#define ID_DU22			189
#define ID_DU23			190
#define ID_DU24			191
#define ID_DU25			192
#define ID_DU26			193
#define ID_DU27			194
#define ID_DU28			195
#define ID_DU29			196
#define ID_DU30			197
#define ID_DU31			198
#define ID_DU32			199
#define ID_DU33			200
#define ID_DU34			201
#define ID_DU35			202
#define ID_DU36			203
#define ID_DU37			204
#define ID_DU38			205
#define ID_DU39			206
#define ID_DU40			207
#define ID_DU41			208
#define ID_DU42			209
#define ID_DU43			210
#define ID_DU44			211
#define ID_DU45			212
#define ID_DU46			213
#define ID_DU47			214
#define ID_DU48			215
#define ID_DU49			216
#define ID_DU50			217
#define ID_DU51			218
#define ID_DU52			219
#define ID_DU53			220
#define ID_DU54			221
#define ID_DU55			222
#define ID_DU56			223
#define ID_DU57			224
#define ID_DU58			225
#define ID_DU59			226
#define ID_DU60			227
#define ID_DU61			228
#define ID_DU62			229
#define ID_DU63			230
#define ID_DU64			231
#define ID_DU65			232
#define ID_DU66			233
#define ID_DU67			234
#define ID_DU68			235
#define ID_DU69			236
#define ID_DU70			237
#define ID_DU71			238
#define ID_DU72			239
#define ID_DU73			240
#define ID_DU74			241
#define ID_DU75			242
#define ID_DU76			243
#define ID_DU77			244
#define ID_DU78			245
#define ID_DU79			246
#define ID_DU80			247 
#define ID_DU81			248
#define ID_DU82			249
#define ID_DU83			250
#define ID_DU84			251
#define ID_DU85			252
#define ID_DU86			253
#define ID_DU87			254
#define ID_DU88			255
#define ID_DU89			256
#define ID_DU90			257
#define ID_DU91			258
#define ID_DU92			259
#define ID_DU93			260
#define ID_DU94			261
#define ID_DU95			262
#define ID_DU96			263
#define ID_DU97			264
#define ID_DU98			265
#define ID_DU99			266
// USERBOX.HTML
#define ID_USER			267
#define ID_BOX			268
#define ID_UB01			269
#define ID_UB02			270
#define ID_UB03			271
#define ID_UB04			272
#define ID_UB05			273
#define ID_UB06			274
#define ID_UB07			275
#define ID_UB08			276
#define ID_UB09			277
#define ID_UB10			278
#define ID_UB11			279
#define ID_UB12			280
#define ID_UB13			281
#define ID_UB14			282
#define ID_UB15			283
#define ID_UB16			284
#define ID_UB17			285
#define ID_UB18			286
#define ID_UB19			287
#define ID_UB20			288
#define ID_UB21			289
#define ID_UB22			290
#define ID_UB23			291
#define ID_UB24			292
#define ID_UB25			293
#define ID_UB26			294
#define ID_UB27			295
#define ID_UB28			296
#define ID_UB29			297
#define ID_UB30			298
#define ID_UB31			299
#define ID_UB32			300
#define ID_UB33			301
#define ID_UB34			302
#define ID_UB35			303
#define ID_UB36			304
#define ID_UB37			305
#define ID_UB38			306
#define ID_UB39			307
#define ID_UB40			308
#define ID_UB41			309
#define ID_UB42			310
#define ID_UB43			311
#define ID_UB44			312
#define ID_UB45			313
#define ID_UB46			314
#define ID_UB47			315
#define ID_UB48			316
#define ID_UB49			317
#define ID_UB50			318
#define ID_UB51			319
#define ID_UB52			320
#define ID_UB53			321
#define ID_UB54			322
#define ID_UB55			323
#define ID_UB56			324
#define ID_UB57			325
#define ID_UB58			326
#define ID_UB59			327
#define ID_UB60			328
#define ID_UB61			329
#define ID_UB62			330
#define ID_UB63			331
#define ID_UB64			332
#define ID_UB65			333
#define ID_UB66			334
#define ID_UB67			335
#define ID_UB68			336
#define ID_UB69			337
#define ID_UB70			338
#define ID_UB71			339
#define ID_UB72			340
#define ID_UB73			341
#define ID_UB74			342
#define ID_UB75			343
#define ID_UB76			344
#define ID_UB77			345
#define ID_UB78			346
#define ID_UB79			347
#define ID_UB80			348 
#define ID_UB81			349
#define ID_UB82			350
#define ID_UB83			351
#define ID_UB84			352
#define ID_UB85			353
#define ID_UB86			354
#define ID_UB87			355
#define ID_UB88			356
#define ID_UB89			357
#define ID_UB90			358
#define ID_UB91			359
#define ID_UB92			360
#define ID_UB93			361
#define ID_UB94			362
#define ID_UB95			363
#define ID_UB96			364
#define ID_UB97			365
#define ID_UB98			366
#define ID_UB99			367
// TICKET.HTML
#define ID_LG			368
#define ID_DLEN			369
#define ID_DLNUM		370
#define ID_DHNUM		371
#define ID_CUT			372
#define ID_VIA			373
#define ID_H1OP			374
#define ID_H2OP			375
#define ID_H3OP			376
#define ID_F1OP			377
#define ID_F2OP			378
#define ID_F3OP			379
#define ID_F4OP			380
#define ID_H1ME			381
#define ID_H2ME			382
#define ID_H3ME			383
#define ID_F1ME			384
#define ID_F2ME			385
#define ID_F3ME			386
#define ID_F4ME			387
// REAL.HTML
#define ID_FILA1		388
#define ID_PREV1		389
#define ID_FILA2		390
#define ID_PREV2		391
#define ID_FILA3		392
#define ID_PREV3		393
#define ID_FILA4		394
#define ID_PREV4		395
#define ID_FILA5		396
#define ID_PREV5		397
#define ID_FILA6		398
#define ID_PREV6		399
// VER.HTML
#define ID_MODEL		400
#define ID_VERS			401
#define ID_AUTH			402
#define ID_PW			403
// FORMAT.HTML
#define ID_FORMAT		404
#define ID_INIT			405
						 
/*----------------------------------------------------------------------------
 *      App variables
 *---------------------------------------------------------------------------*/
extern CFG 				cfg;

/*----------------------------------------------------------------------------
 *      Queue.c   variables
 *---------------------------------------------------------------------------*/
extern unsigned short	queue [MAXDEP];

/*----------------------------------------------------------------------------
 *      Report.c   variables
 *---------------------------------------------------------------------------*/
extern unsigned char 	STATFILE;

/*----------------------------------------------------------------------------
 *      HTTP_CGI variables
 *---------------------------------------------------------------------------*/
extern unsigned char	WEBREQUEST;

/*----------------------------------------------------------------------------
 *      Local variables
 *---------------------------------------------------------------------------*/
unsigned char 			api_resp[255];					// Buffer de resposta http
unsigned char 			api_resp_len;					// Comprimento do buffer de resposta http
unsigned char 			api_req_handled;				// Indica se requisição foi atendida
unsigned char 			authOk=0;						// Indica que autenticou

extern DATAREQHTTP		dataReqHttp;					// Dados para gravação da requisição Http pendentes

char 					ListWifi[10][32];
extern unsigned char 	numListWifi;

static FINFO 			info;							// Informações do arquivo somente local
static FILE 			*f = NULL;						// Ponteiro do arquivo somente local
static unsigned char 	taskFile=0;						// Função do arquivo somente local
static unsigned char 	DEP=1;							// Função do arquivo somente local
static unsigned char   	USER=1;							// Função do arquivo somente local

/*----------------------------------------------------------------------------
 *      Constants for http directory identification
 *---------------------------------------------------------------------------*/
const unsigned char API_SAVE_LEN = 9;
const char  		API_SAVE[] = "/api/save";
const unsigned char API_DIR_LEN = 9;
const char 			API_DIR[] = "/api/dirs";
const unsigned char API_SAVEFILE_LEN = 11;
const char 			API_SAVEFILE[] = "/api/upload";
const unsigned char API_READFILE_LEN = 14;
const char 			API_READFILE[] = "/api/download/";
const unsigned char API_TASK_LEN = 9;
const char 			API_TASK[] = "/api/task";
const unsigned char API_STATUS_LEN = 11;
const char 			API_STATUS[] = "/api/status";
const unsigned char API_BOOT_LEN = 9;
const char  		API_BOOT[] = "/api/boot";
const unsigned char API_NETWORK_LEN = 13;
const char 			API_NETWORK[] = "/api/networks";
const unsigned char API_MODE_LEN = 9;
const char  		API_MODE[] = "/api/mode";
const unsigned char API_APGET_LEN = 7;
const char  		API_APGET[] = "/api/ap";
const unsigned char API_APSET_LEN = 8;
const char  		API_APSET[] = "/api/ap/";
const unsigned char API_STAGET_LEN = 8;
const char  		API_STAGET[] = "/api/sta";
const unsigned char API_STASET_LEN = 9;
const char  		API_STASET[] = "/api/sta/";
const unsigned char API_SYSGET_LEN = 11;
const char  		API_SYSGET[] = "/api/system";
const unsigned char API_SYSSET_LEN = 12;
const char  		API_SYSSET[] = "/api/system/";
const unsigned char API_SELDEP_LEN = 11;
const char 			API_SELDEP[] = "/api/seldep";
const unsigned char API_DEPGET_LEN = 8;
const char  		API_DEPGET[] = "/api/dep";
const unsigned char API_DEPSET_LEN = 9;
const char  		API_DEPSET[] = "/api/dep/";
const unsigned char API_BOXGET_LEN = 12;
const char  		API_BOXGET[] = "/api/userbox";
const unsigned char API_BOXSET_LEN = 13;
const char  		API_BOXSET[] = "/api/userbox/";
const unsigned char API_TKTGET_LEN = 11;
const char  		API_TKTGET[] = "/api/ticket";
const unsigned char API_TKTSET_LEN = 12;
const char  		API_TKTSET[] = "/api/ticket/";
const unsigned char API_VERGET_LEN = 12;
const char  		API_VERGET[] = "/api/version";
const unsigned char API_VERSET_LEN = 13;
const char  		API_VERSET[] = "/api/version/";
const unsigned char API_REAL_LEN = 9;
const char  		API_REAL[] = "/api/real";
const unsigned char API_FORMAT_LEN = 12;
const char  		API_FORMAT[] = "/api/format/";
const unsigned char API_LABEL_LEN = 10;
const char  		API_LABEL[] = "/api/label";

/*----------------------------------------------------------------------------
 *      Constants for http parameters identification
 *---------------------------------------------------------------------------*/
const unsigned char PAR_MODE_LEN = 4;
const char  		PAR_MODE[] = "mode";
const unsigned char PAR_BOOT_LEN = 4;
const char  		PAR_BOOT[] = "boot";
const unsigned char PAR_SET_LEN = 8;
const char  		PAR_SET[] = "settings";
const unsigned char PAR_SSID_LEN = 4;
const char  		PAR_SSID[] = "ssid";
const unsigned char PAR_BSSID_LEN = 5;
const char  		PAR_BSSID[] = "bssid";
const unsigned char PAR_CHANNEL_LEN = 7;
const char  		PAR_CHANNEL[] = "channel";
const unsigned char PAR_SECURITY_LEN = 8;
const char  		PAR_SECURITY[] = "security";
const unsigned char PAR_NETWORKS_LEN = 8;
const char  		PAR_NETWORKS[] = "networks";
const unsigned char PAR_DIR_LEN = 4;
const char  		PAR_DIR[] = "dirs";
const unsigned char PAR_DOWNLOAD_LEN = 8;
const char  		PAR_DOWNLOAD[] = "download";
const unsigned char PAR_WIFIPW_LEN = 8;
const char  		PAR_WIFIPW[] = "password";
const unsigned char PAR_DHCP_LEN = 4;
const char  		PAR_DHCP[] = "dhcp";
const unsigned char PAR_DHCPS_LEN = 5;
const char  		PAR_DHCPS[] = "dhcps";
const unsigned char PAR_DHCPS_IP_LEN = 7;
const char  		PAR_DHCPS_IP[] = "dhcpsIp";
const unsigned char PAR_LOCALIP_LEN = 7;
const char  		PAR_LOCALIP[] = "localIp";
const unsigned char PAR_MASK_LEN = 4;
const char  		PAR_MASK[] = "mask";
const unsigned char PAR_GATEWAY_LEN = 7;
const char  		PAR_GATEWAY[] = "gateway";
const unsigned char PAR_DNS1_LEN = 4;
const char  		PAR_DNS1[] = "dns1";
const unsigned char PAR_DNS2_LEN = 4;
const char  		PAR_DNS2[] = "dns2";
// SYSTEM.HTML
const unsigned char PAR_DRIVE_LEN = 5;
const char  		PAR_DRIVE[] = "drive";
const unsigned char PAR_DATE_LEN = 4;
const char  		PAR_DATE[] = "date";
const unsigned char PAR_TIME_LEN = 4;
const char  		PAR_TIME[] = "time";
const unsigned char PAR_TICKETTEST_LEN = 10;
const char  		PAR_TICKETTEST[] = "ticketTest";
const unsigned char PAR_TIMELIVE_LEN = 8;
const char  		PAR_TIMELIVE[] = "timeLive";
const unsigned char PAR_CONTROL_LEN = 7;
const char  		PAR_CONTROL[] = "control";
const unsigned char PAR_TIMEON_LEN = 6;
const char  		PAR_TIMEON[] = "timeOn";
const unsigned char PAR_TIMEOFF_LEN = 7;
const char  		PAR_TIMEOFF[] = "timeOff";
const unsigned char PAR_D1_LEN = 2;
const char  		PAR_D1[] = "d1";
const unsigned char PAR_D2_LEN = 2;
const char  		PAR_D2[] = "d2";
const unsigned char PAR_D3_LEN = 2;
const char  		PAR_D3[] = "d3";
const unsigned char PAR_D4_LEN = 2;
const char  		PAR_D4[] = "d4";
const unsigned char PAR_D5_LEN = 2;
const char  		PAR_D5[] = "d5";
const unsigned char PAR_D6_LEN = 2;
const char  		PAR_D6[] = "d6";
const unsigned char PAR_D7_LEN = 2;
const char  		PAR_D7[] = "d7";
const unsigned char PAR_REPORTTIME_LEN = 10;
const char  		PAR_REPORTTIME[] = "reportTime";
const unsigned char PAR_REPORTCSV_LEN = 9;
const char  		PAR_REPORTCSV[] = "reportCsv";
const unsigned char PAR_MIGMODE_LEN = 7;
const char  		PAR_MIGMODE[] = "migMode";
const unsigned char PAR_MIGIP_LEN = 5;
const char  		PAR_MIGIP[] = "migIp";
const unsigned char PAR_LINE_LEN = 4;
const char  		PAR_LINE[] = "line";
const unsigned char PAR_INTERF_LEN = 6;
const char  		PAR_INTERF[] = "interf";
const unsigned char PAR_TASK_LEN = 4;
const char  		PAR_TASK[] = "task";
// SELDEP.HTML
const unsigned char PAR_DNUM_LEN = 4;
const char  		PAR_DNUM[] = "dnum";
const unsigned char PAR_DNAME1_LEN = 6;
const char  		PAR_DNAME1[] = "dname1";
const unsigned char PAR_DNAME2_LEN = 6;
const char  		PAR_DNAME2[] = "dname2";
const unsigned char PAR_DNAME3_LEN = 6;
const char  		PAR_DNAME3[] = "dname3";
const unsigned char PAR_DNAME4_LEN = 6;
const char  		PAR_DNAME4[] = "dname4";
const unsigned char PAR_DNAME5_LEN = 6;
const char  		PAR_DNAME5[] = "dname5";
const unsigned char PAR_DNAME6_LEN = 6;
const char  		PAR_DNAME6[] = "dname6";
// DEP.HTML
const unsigned char PAR_DENAB_LEN = 6;
const char  		PAR_DENAB[] = "enadep";
const unsigned char PAR_DNAME_LEN = 6;
const char  		PAR_DNAME[] = "dpname";
const unsigned char PAR_DALPH_LEN = 6;
const char  		PAR_DALPH[] = "dlette";
const unsigned char PAR_DRANG1_LEN = 6;
const char  		PAR_DRANG1[] = "drang1";
const unsigned char PAR_DRANG2_LEN = 6;
const char  		PAR_DRANG2[] = "drang2";
const unsigned char PAR_DCLOS1_LEN = 6;
const char  		PAR_DCLOS1[] = "dclos1";
const unsigned char PAR_DCLOS2_LEN = 6;
const char  		PAR_DCLOS2[] = "dclos2";
const unsigned char PAR_DCALL1_LEN = 6;
const char  		PAR_DCALL1[] = "dsche1";
const unsigned char PAR_DCALL2_LEN = 6;
const char  		PAR_DCALL2[] = "dsche2";
const unsigned char PAR_DHLP1_LEN = 6;
const char  		PAR_DHLP1[] = "dhelp1";
const unsigned char PAR_DHLP2_LEN = 6;
const char  		PAR_DHLP2[] = "dhelp2";
const unsigned char PAR_DHLP3_LEN = 6;
const char  		PAR_DHLP3[] = "dhelp3";
const unsigned char PAR_DHLP4_LEN = 6;
const char  		PAR_DHLP4[] = "dhelp4";
const unsigned char PAR_DHLP5_LEN = 6;
const char  		PAR_DHLP5[] = "dhelp5";
const unsigned char PAR_DU01_LEN = 4;
const char  		PAR_DU01[] = "du01";
const unsigned char PAR_DU02_LEN = 4;
const char  		PAR_DU02[] = "du02";
const unsigned char PAR_DU03_LEN = 4;
const char  		PAR_DU03[] = "du03";
const unsigned char PAR_DU04_LEN = 4;
const char  		PAR_DU04[] = "du04";
const unsigned char PAR_DU05_LEN = 4;
const char  		PAR_DU05[] = "du05";
const unsigned char PAR_DU06_LEN = 4;
const char  		PAR_DU06[] = "du06";
const unsigned char PAR_DU07_LEN = 4;
const char  		PAR_DU07[] = "du07";
const unsigned char PAR_DU08_LEN = 4;
const char  		PAR_DU08[] = "du08";
const unsigned char PAR_DU09_LEN = 4;
const char  		PAR_DU09[] = "du09";
const unsigned char PAR_DU10_LEN = 4;
const char  		PAR_DU10[] = "du10";
const unsigned char PAR_DU11_LEN = 4;
const char  		PAR_DU11[] = "du11";
const unsigned char PAR_DU12_LEN = 4;
const char  		PAR_DU12[] = "du12";
const unsigned char PAR_DU13_LEN = 4;
const char  		PAR_DU13[] = "du13";
const unsigned char PAR_DU14_LEN = 4;
const char  		PAR_DU14[] = "du14";
const unsigned char PAR_DU15_LEN = 4;
const char  		PAR_DU15[] = "du15";
const unsigned char PAR_DU16_LEN = 4;
const char  		PAR_DU16[] = "du16";
const unsigned char PAR_DU17_LEN = 4;
const char  		PAR_DU17[] = "du17";
const unsigned char PAR_DU18_LEN = 4;
const char  		PAR_DU18[] = "du18";
const unsigned char PAR_DU19_LEN = 4;
const char  		PAR_DU19[] = "du19";
const unsigned char PAR_DU20_LEN = 4;
const char  		PAR_DU20[] = "du20";
const unsigned char PAR_DU21_LEN = 4;
const char  		PAR_DU21[] = "du21";
const unsigned char PAR_DU22_LEN = 4;
const char  		PAR_DU22[] = "du22";
const unsigned char PAR_DU23_LEN = 4;
const char  		PAR_DU23[] = "du23";
const unsigned char PAR_DU24_LEN = 4;
const char  		PAR_DU24[] = "du24";
const unsigned char PAR_DU25_LEN = 4;
const char  		PAR_DU25[] = "du25";
const unsigned char PAR_DU26_LEN = 4;
const char  		PAR_DU26[] = "du26";
const unsigned char PAR_DU27_LEN = 4;
const char  		PAR_DU27[] = "du27";
const unsigned char PAR_DU28_LEN = 4;
const char  		PAR_DU28[] = "du28";
const unsigned char PAR_DU29_LEN = 4;
const char  		PAR_DU29[] = "du29";
const unsigned char PAR_DU30_LEN = 4;
const char  		PAR_DU30[] = "du30";
const unsigned char PAR_DU31_LEN = 4;
const char  		PAR_DU31[] = "du31";
const unsigned char PAR_DU32_LEN = 4;
const char  		PAR_DU32[] = "du32";
const unsigned char PAR_DU33_LEN = 4;
const char  		PAR_DU33[] = "du33";
const unsigned char PAR_DU34_LEN = 4;
const char  		PAR_DU34[] = "du34";
const unsigned char PAR_DU35_LEN = 4;
const char  		PAR_DU35[] = "du35";
const unsigned char PAR_DU36_LEN = 4;
const char  		PAR_DU36[] = "du36";
const unsigned char PAR_DU37_LEN = 4;
const char  		PAR_DU37[] = "du37";
const unsigned char PAR_DU38_LEN = 4;
const char  		PAR_DU38[] = "du38";
const unsigned char PAR_DU39_LEN = 4;
const char  		PAR_DU39[] = "du39";
const unsigned char PAR_DU40_LEN = 4;
const char  		PAR_DU40[] = "du40";
const unsigned char PAR_DU41_LEN = 4;
const char  		PAR_DU41[] = "du41";
const unsigned char PAR_DU42_LEN = 4;
const char  		PAR_DU42[] = "du42";
const unsigned char PAR_DU43_LEN = 4;
const char  		PAR_DU43[] = "du43";
const unsigned char PAR_DU44_LEN = 4;
const char  		PAR_DU44[] = "du44";
const unsigned char PAR_DU45_LEN = 4;
const char  		PAR_DU45[] = "du45";
const unsigned char PAR_DU46_LEN = 4;
const char  		PAR_DU46[] = "du46";
const unsigned char PAR_DU47_LEN = 4;
const char  		PAR_DU47[] = "du47";
const unsigned char PAR_DU48_LEN = 4;
const char  		PAR_DU48[] = "du48";
const unsigned char PAR_DU49_LEN = 4;
const char  		PAR_DU49[] = "du49";
const unsigned char PAR_DU50_LEN = 4;
const char  		PAR_DU50[] = "du50";
const unsigned char PAR_DU51_LEN = 4;
const char  		PAR_DU51[] = "du51";
const unsigned char PAR_DU52_LEN = 4;
const char  		PAR_DU52[] = "du52";
const unsigned char PAR_DU53_LEN = 4;
const char  		PAR_DU53[] = "du53";
const unsigned char PAR_DU54_LEN = 4;
const char  		PAR_DU54[] = "du54";
const unsigned char PAR_DU55_LEN = 4;
const char  		PAR_DU55[] = "du55";
const unsigned char PAR_DU56_LEN = 4;
const char  		PAR_DU56[] = "du56";
const unsigned char PAR_DU57_LEN = 4;
const char  		PAR_DU57[] = "du57";
const unsigned char PAR_DU58_LEN = 4;
const char  		PAR_DU58[] = "du58";
const unsigned char PAR_DU59_LEN = 4;
const char  		PAR_DU59[] = "du59";
const unsigned char PAR_DU60_LEN = 4;
const char  		PAR_DU60[] = "du60";
const unsigned char PAR_DU61_LEN = 4;
const char  		PAR_DU61[] = "du61";
const unsigned char PAR_DU62_LEN = 4;
const char  		PAR_DU62[] = "du62";
const unsigned char PAR_DU63_LEN = 4;
const char  		PAR_DU63[] = "du63";
const unsigned char PAR_DU64_LEN = 4;
const char  		PAR_DU64[] = "du64";
const unsigned char PAR_DU65_LEN = 4;
const char  		PAR_DU65[] = "du65";
const unsigned char PAR_DU66_LEN = 4;
const char  		PAR_DU66[] = "du66";
const unsigned char PAR_DU67_LEN = 4;
const char  		PAR_DU67[] = "du67";
const unsigned char PAR_DU68_LEN = 4;
const char  		PAR_DU68[] = "du68";
const unsigned char PAR_DU69_LEN = 4;
const char  		PAR_DU69[] = "du69";
const unsigned char PAR_DU70_LEN = 4;
const char  		PAR_DU70[] = "du70";
const unsigned char PAR_DU71_LEN = 4;
const char  		PAR_DU71[] = "du71";
const unsigned char PAR_DU72_LEN = 4;
const char  		PAR_DU72[] = "du72";
const unsigned char PAR_DU73_LEN = 4;
const char  		PAR_DU73[] = "du73";
const unsigned char PAR_DU74_LEN = 4;
const char  		PAR_DU74[] = "du74";
const unsigned char PAR_DU75_LEN = 4;
const char  		PAR_DU75[] = "du75";
const unsigned char PAR_DU76_LEN = 4;
const char  		PAR_DU76[] = "du76";
const unsigned char PAR_DU77_LEN = 4;
const char  		PAR_DU77[] = "du77";
const unsigned char PAR_DU78_LEN = 4;
const char  		PAR_DU78[] = "du78";
const unsigned char PAR_DU79_LEN = 4;
const char  		PAR_DU79[] = "du79";
const unsigned char PAR_DU80_LEN = 4;
const char  		PAR_DU80[] = "du80";
const unsigned char PAR_DU81_LEN = 4;
const char  		PAR_DU81[] = "du81";
const unsigned char PAR_DU82_LEN = 4;
const char  		PAR_DU82[] = "du82";
const unsigned char PAR_DU83_LEN = 4;
const char  		PAR_DU83[] = "du83";
const unsigned char PAR_DU84_LEN = 4;
const char  		PAR_DU84[] = "du84";
const unsigned char PAR_DU85_LEN = 4;
const char  		PAR_DU85[] = "du85";
const unsigned char PAR_DU86_LEN = 4;
const char  		PAR_DU86[] = "du86";
const unsigned char PAR_DU87_LEN = 4;
const char  		PAR_DU87[] = "du87";
const unsigned char PAR_DU88_LEN = 4;
const char  		PAR_DU88[] = "du88";
const unsigned char PAR_DU89_LEN = 4;
const char  		PAR_DU89[] = "du89";
const unsigned char PAR_DU90_LEN = 4;
const char  		PAR_DU90[] = "du90";
const unsigned char PAR_DU91_LEN = 4;
const char  		PAR_DU91[] = "du91";
const unsigned char PAR_DU92_LEN = 4;
const char  		PAR_DU92[] = "du92";
const unsigned char PAR_DU93_LEN = 4;
const char  		PAR_DU93[] = "du93";
const unsigned char PAR_DU94_LEN = 4;
const char  		PAR_DU94[] = "du94";
const unsigned char PAR_DU95_LEN = 4;
const char  		PAR_DU95[] = "du95";
const unsigned char PAR_DU96_LEN = 4;
const char  		PAR_DU96[] = "du96";
const unsigned char PAR_DU97_LEN = 4;
const char  		PAR_DU97[] = "du97";
const unsigned char PAR_DU98_LEN = 4;
const char  		PAR_DU98[] = "du98";
const unsigned char PAR_DU99_LEN = 4;
const char  		PAR_DU99[] = "du99";
const unsigned char PAR_DIPD_LEN = 6;
const char  		PAR_DIPD[] = "dpanel";
const unsigned char PAR_DITFD_LEN = 6;
const char  		PAR_DITFD[] = "ditfpn";
const unsigned char PAR_DIPT_LEN = 6;
const char  		PAR_DIPT[] = "deptop";
const unsigned char PAR_DITFT_LEN = 6;
const char  		PAR_DITFT[] = "ditftp";
const unsigned char PAR_ENATOP_LEN = 6;
const char  		PAR_ENATOP[] = "enatop";
const unsigned char PAR_DLIND_LEN = 6;
const char  		PAR_DLIND[] = "linpnl";
const unsigned char PAR_DPRFD_LEN = 6;
const char  		PAR_DPRFD[] = "prefdp";
// USERBOX.HTML
const unsigned char PAR_USER_LEN = 4;
const char  		PAR_USER[] = "user";
const unsigned char PAR_BOX_LEN = 3;
const char  		PAR_BOX[] = "box";
const unsigned char PAR_UB01_LEN = 4;
const char  		PAR_UB01[] = "ub01";
const unsigned char PAR_UB02_LEN = 4;
const char  		PAR_UB02[] = "ub02";
const unsigned char PAR_UB03_LEN = 4;
const char  		PAR_UB03[] = "ub03";
const unsigned char PAR_UB04_LEN = 4;
const char  		PAR_UB04[] = "ub04";
const unsigned char PAR_UB05_LEN = 4;
const char  		PAR_UB05[] = "ub05";
const unsigned char PAR_UB06_LEN = 4;
const char  		PAR_UB06[] = "ub06";
const unsigned char PAR_UB07_LEN = 4;
const char  		PAR_UB07[] = "ub07";
const unsigned char PAR_UB08_LEN = 4;
const char  		PAR_UB08[] = "ub08";
const unsigned char PAR_UB09_LEN = 4;
const char  		PAR_UB09[] = "ub09";
const unsigned char PAR_UB10_LEN = 4;
const char  		PAR_UB10[] = "ub10";
const unsigned char PAR_UB11_LEN = 4;
const char  		PAR_UB11[] = "ub11";
const unsigned char PAR_UB12_LEN = 4;
const char  		PAR_UB12[] = "ub12";
const unsigned char PAR_UB13_LEN = 4;
const char  		PAR_UB13[] = "ub13";
const unsigned char PAR_UB14_LEN = 4;
const char  		PAR_UB14[] = "ub14";
const unsigned char PAR_UB15_LEN = 4;
const char  		PAR_UB15[] = "ub15";
const unsigned char PAR_UB16_LEN = 4;
const char  		PAR_UB16[] = "ub16";
const unsigned char PAR_UB17_LEN = 4;
const char  		PAR_UB17[] = "ub17";
const unsigned char PAR_UB18_LEN = 4;
const char  		PAR_UB18[] = "ub18";
const unsigned char PAR_UB19_LEN = 4;
const char  		PAR_UB19[] = "ub19";
const unsigned char PAR_UB20_LEN = 4;
const char  		PAR_UB20[] = "ub20";
const unsigned char PAR_UB21_LEN = 4;
const char  		PAR_UB21[] = "ub21";
const unsigned char PAR_UB22_LEN = 4;
const char  		PAR_UB22[] = "ub22";
const unsigned char PAR_UB23_LEN = 4;
const char  		PAR_UB23[] = "ub23";
const unsigned char PAR_UB24_LEN = 4;
const char  		PAR_UB24[] = "ub24";
const unsigned char PAR_UB25_LEN = 4;
const char  		PAR_UB25[] = "ub25";
const unsigned char PAR_UB26_LEN = 4;
const char  		PAR_UB26[] = "ub26";
const unsigned char PAR_UB27_LEN = 4;
const char  		PAR_UB27[] = "ub27";
const unsigned char PAR_UB28_LEN = 4;
const char  		PAR_UB28[] = "ub28";
const unsigned char PAR_UB29_LEN = 4;
const char  		PAR_UB29[] = "ub29";
const unsigned char PAR_UB30_LEN = 4;
const char  		PAR_UB30[] = "ub30";
const unsigned char PAR_UB31_LEN = 4;
const char  		PAR_UB31[] = "ub31";
const unsigned char PAR_UB32_LEN = 4;
const char  		PAR_UB32[] = "ub32";
const unsigned char PAR_UB33_LEN = 4;
const char  		PAR_UB33[] = "ub33";
const unsigned char PAR_UB34_LEN = 4;
const char  		PAR_UB34[] = "ub34";
const unsigned char PAR_UB35_LEN = 4;
const char  		PAR_UB35[] = "ub35";
const unsigned char PAR_UB36_LEN = 4;
const char  		PAR_UB36[] = "ub36";
const unsigned char PAR_UB37_LEN = 4;
const char  		PAR_UB37[] = "ub37";
const unsigned char PAR_UB38_LEN = 4;
const char  		PAR_UB38[] = "ub38";
const unsigned char PAR_UB39_LEN = 4;
const char  		PAR_UB39[] = "ub39";
const unsigned char PAR_UB40_LEN = 4;
const char  		PAR_UB40[] = "ub40";
const unsigned char PAR_UB41_LEN = 4;
const char  		PAR_UB41[] = "ub41";
const unsigned char PAR_UB42_LEN = 4;
const char  		PAR_UB42[] = "ub42";
const unsigned char PAR_UB43_LEN = 4;
const char  		PAR_UB43[] = "ub43";
const unsigned char PAR_UB44_LEN = 4;
const char  		PAR_UB44[] = "ub44";
const unsigned char PAR_UB45_LEN = 4;
const char  		PAR_UB45[] = "ub45";
const unsigned char PAR_UB46_LEN = 4;
const char  		PAR_UB46[] = "ub46";
const unsigned char PAR_UB47_LEN = 4;
const char  		PAR_UB47[] = "ub47";
const unsigned char PAR_UB48_LEN = 4;
const char  		PAR_UB48[] = "ub48";
const unsigned char PAR_UB49_LEN = 4;
const char  		PAR_UB49[] = "ub49";
const unsigned char PAR_UB50_LEN = 4;
const char  		PAR_UB50[] = "ub50";
const unsigned char PAR_UB51_LEN = 4;
const char  		PAR_UB51[] = "ub51";
const unsigned char PAR_UB52_LEN = 4;
const char  		PAR_UB52[] = "ub52";
const unsigned char PAR_UB53_LEN = 4;
const char  		PAR_UB53[] = "ub53";
const unsigned char PAR_UB54_LEN = 4;
const char  		PAR_UB54[] = "ub54";
const unsigned char PAR_UB55_LEN = 4;
const char  		PAR_UB55[] = "ub55";
const unsigned char PAR_UB56_LEN = 4;
const char  		PAR_UB56[] = "ub56";
const unsigned char PAR_UB57_LEN = 4;
const char  		PAR_UB57[] = "ub57";
const unsigned char PAR_UB58_LEN = 4;
const char  		PAR_UB58[] = "ub58";
const unsigned char PAR_UB59_LEN = 4;
const char  		PAR_UB59[] = "ub59";
const unsigned char PAR_UB60_LEN = 4;
const char  		PAR_UB60[] = "ub60";
const unsigned char PAR_UB61_LEN = 4;
const char  		PAR_UB61[] = "ub61";
const unsigned char PAR_UB62_LEN = 4;
const char  		PAR_UB62[] = "ub62";
const unsigned char PAR_UB63_LEN = 4;
const char  		PAR_UB63[] = "ub63";
const unsigned char PAR_UB64_LEN = 4;
const char  		PAR_UB64[] = "ub64";
const unsigned char PAR_UB65_LEN = 4;
const char  		PAR_UB65[] = "ub65";
const unsigned char PAR_UB66_LEN = 4;
const char  		PAR_UB66[] = "ub66";
const unsigned char PAR_UB67_LEN = 4;
const char  		PAR_UB67[] = "ub67";
const unsigned char PAR_UB68_LEN = 4;
const char  		PAR_UB68[] = "ub68";
const unsigned char PAR_UB69_LEN = 4;
const char  		PAR_UB69[] = "ub69";
const unsigned char PAR_UB70_LEN = 4;
const char  		PAR_UB70[] = "ub70";
const unsigned char PAR_UB71_LEN = 4;
const char  		PAR_UB71[] = "ub71";
const unsigned char PAR_UB72_LEN = 4;
const char  		PAR_UB72[] = "ub72";
const unsigned char PAR_UB73_LEN = 4;
const char  		PAR_UB73[] = "ub73";
const unsigned char PAR_UB74_LEN = 4;
const char  		PAR_UB74[] = "ub74";
const unsigned char PAR_UB75_LEN = 4;
const char  		PAR_UB75[] = "ub75";
const unsigned char PAR_UB76_LEN = 4;
const char  		PAR_UB76[] = "ub76";
const unsigned char PAR_UB77_LEN = 4;
const char  		PAR_UB77[] = "ub77";
const unsigned char PAR_UB78_LEN = 4;
const char  		PAR_UB78[] = "ub78";
const unsigned char PAR_UB79_LEN = 4;
const char  		PAR_UB79[] = "ub79";
const unsigned char PAR_UB80_LEN = 4;
const char  		PAR_UB80[] = "ub80";
const unsigned char PAR_UB81_LEN = 4;
const char  		PAR_UB81[] = "ub81";
const unsigned char PAR_UB82_LEN = 4;
const char  		PAR_UB82[] = "ub82";
const unsigned char PAR_UB83_LEN = 4;
const char  		PAR_UB83[] = "ub83";
const unsigned char PAR_UB84_LEN = 4;
const char  		PAR_UB84[] = "ub84";
const unsigned char PAR_UB85_LEN = 4;
const char  		PAR_UB85[] = "ub85";
const unsigned char PAR_UB86_LEN = 4;
const char  		PAR_UB86[] = "ub86";
const unsigned char PAR_UB87_LEN = 4;
const char  		PAR_UB87[] = "ub87";
const unsigned char PAR_UB88_LEN = 4;
const char  		PAR_UB88[] = "ub88";
const unsigned char PAR_UB89_LEN = 4;
const char  		PAR_UB89[] = "ub89";
const unsigned char PAR_UB90_LEN = 4;
const char  		PAR_UB90[] = "ub90";
const unsigned char PAR_UB91_LEN = 4;
const char  		PAR_UB91[] = "ub91";
const unsigned char PAR_UB92_LEN = 4;
const char  		PAR_UB92[] = "ub92";
const unsigned char PAR_UB93_LEN = 4;
const char  		PAR_UB93[] = "ub93";
const unsigned char PAR_UB94_LEN = 4;
const char  		PAR_UB94[] = "ub94";
const unsigned char PAR_UB95_LEN = 4;
const char  		PAR_UB95[] = "ub95";
const unsigned char PAR_UB96_LEN = 4;
const char  		PAR_UB96[] = "ub96";
const unsigned char PAR_UB97_LEN = 4;
const char  		PAR_UB97[] = "ub97";
const unsigned char PAR_UB98_LEN = 4;
const char  		PAR_UB98[] = "ub98";
const unsigned char PAR_UB99_LEN = 4;
const char  		PAR_UB99[] = "ub99";
// TICKET.HTML
const unsigned char PAR_LG_LEN = 4;
const char  		PAR_LG[] = "logo";
const unsigned char PAR_DLEN_LEN = 5;
const char  		PAR_DLEN[] = "enlet";
const unsigned char PAR_DLNUM_LEN = 6;
const char  		PAR_DLNUM[] = "lentkt";
const unsigned char PAR_DHNUM_LEN = 7;
const char  		PAR_DHNUM[] = "hnumtkt";
const unsigned char PAR_CUT_LEN = 3;
const char  		PAR_CUT[] = "cut";
const unsigned char PAR_VIA_LEN = 3;
const char  		PAR_VIA[] = "via";
const unsigned char PAR_H1OP_LEN = 6;
const char  		PAR_H1OP[] = "ohead1";
const unsigned char PAR_H2OP_LEN = 6;
const char  		PAR_H2OP[] = "ohead2";
const unsigned char PAR_H3OP_LEN = 6;
const char  		PAR_H3OP[] = "ohead3";
const unsigned char PAR_F1OP_LEN = 6;
const char  		PAR_F1OP[] = "ofoot1";
const unsigned char PAR_F2OP_LEN = 6;
const char  		PAR_F2OP[] = "ofoot2";
const unsigned char PAR_F3OP_LEN = 6;
const char  		PAR_F3OP[] = "ofoot3";
const unsigned char PAR_F4OP_LEN = 6;
const char  		PAR_F4OP[] = "ofoot4";
const unsigned char PAR_H1ME_LEN = 6;
const char  		PAR_H1ME[] = "mhead1";
const unsigned char PAR_H2ME_LEN = 6;
const char  		PAR_H2ME[] = "mhead2";
const unsigned char PAR_H3ME_LEN = 6;
const char  		PAR_H3ME[] = "mhead3";
const unsigned char PAR_F1ME_LEN = 6;
const char  		PAR_F1ME[] = "mfoot1";
const unsigned char PAR_F2ME_LEN = 6;
const char  		PAR_F2ME[] = "mfoot2";
const unsigned char PAR_F3ME_LEN = 6;
const char  		PAR_F3ME[] = "mfoot3";
const unsigned char PAR_F4ME_LEN = 6;
const char  		PAR_F4ME[] = "mfoot4";
// REAL.HTML
const unsigned char PAR_FILA1_LEN = 5;
const char  		PAR_FILA1[] = "fila1";
const unsigned char PAR_PREV1_LEN = 5;
const char  		PAR_PREV1[] = "prev1";
const unsigned char PAR_FILA2_LEN = 5;
const char  		PAR_FILA2[] = "fila2";
const unsigned char PAR_PREV2_LEN = 5;
const char  		PAR_PREV2[] = "prev2";
const unsigned char PAR_FILA3_LEN = 5;
const char  		PAR_FILA3[] = "fila3";
const unsigned char PAR_PREV3_LEN = 5; 
const char  		PAR_PREV3[] = "prev3";
const unsigned char PAR_FILA4_LEN = 5;
const char  		PAR_FILA4[] = "fila4";
const unsigned char PAR_PREV4_LEN = 5;
const char  		PAR_PREV4[] = "prev4";
const unsigned char PAR_FILA5_LEN = 5;
const char  		PAR_FILA5[] = "fila5";
const unsigned char PAR_PREV5_LEN = 5;
const char  		PAR_PREV5[] = "prev5";
const unsigned char PAR_FILA6_LEN = 5;
const char  		PAR_FILA6[] = "fila6";
const unsigned char PAR_PREV6_LEN = 5;
const char  		PAR_PREV6[] = "prev6";
// VER.HTML
const unsigned char PAR_MODEL_LEN = 5;
const char  		PAR_MODEL[] = "model";
const unsigned char PAR_VERS_LEN = 4;
const char  		PAR_VERS[] = "vers";
const unsigned char PAR_AUTH_LEN = 4;
const char  		PAR_AUTH[] = "auth";
const unsigned char PAR_PW_LEN = 2;
const char  		PAR_PW[] = "pw";
// FORMAT.HTML
const unsigned char PAR_FORMAT_LEN = 6;
const char  		PAR_FORMAT[] = "format";
const unsigned char PAR_INIT_LEN = 4;
const char  		PAR_INIT[] = "init";
											  
/*----------------------------------------------------------------------------
 *      extern funcitons definitions
 *---------------------------------------------------------------------------*/
void dot_format (U64 val, char *sp);
extern void Write_Flash (void);

/*----------------------------------------------------------------------------
 *      Local functions definitions
 *---------------------------------------------------------------------------*/
void https_api_request(unsigned char frames, unsigned int numReq, unsigned char method, unsigned char len, unsigned char *str);
unsigned char https_api_request_numFrames(unsigned char len, unsigned char *data);
unsigned char https_api_request_cmd(unsigned char method, unsigned char len, unsigned char *data);
void https_api_request_data(unsigned char len, unsigned char *data);
void https_api_request_header(unsigned char len, unsigned char *data);
void https_api_request_finished(void);
void sme_scan_sort_result(	unsigned char *local_bssid, unsigned char local_channel, unsigned char local_rssi, unsigned char local_snr, unsigned char local_secure, unsigned char local_ssid_len, char *local_ssid);
void sme_scan_sort_finished(void);
void api_add_json_quoted_string(unsigned char len, char *str);
void api_add_json_string(unsigned char len, char *str);
void api_add_json_integer(int value, char minLength, char isUnsigned);
void api_add_json_quoted_time (unsigned char hour, unsigned char min, unsigned char seg, char useSeg);
void api_add_json_quoted_date (unsigned char hour, unsigned char min, unsigned char seg);
void api_add_json_object_name(unsigned char len, char *name);
void api_add_json_array_start(void);
void api_add_json_array_end(void);
void api_add_json_object_start(void);
void api_add_json_object_end(void);
void api_add_crlf(void);
void api_add_comma(void);
void api_add_http_headers(char type);
void api_add_nibble(unsigned char nibble);
void api_add_hex_string(unsigned char len, unsigned char *buffer);
void api_add_quoted_hex_string(unsigned char len, unsigned char *buffer);
unsigned char verLen (char *str);
void api_add_settings_item(	unsigned short item, unsigned char len, char *name);
void api_save_from_post(void);
void api_handle_unknown_request(void);
void api_handle_reboot_request(unsigned int numReq);
void api_handle_status_request(void);
void api_handle_networks_request(unsigned int numReq);
void api_handle_dir_request (unsigned int numReq);
void api_handle_ap_req_part1(void);
void api_handle_ap_req_part2(void);
void api_handle_ap_req_part3(void);
void api_handle_sta_req_part1(void);
void api_handle_sta_req_part2(void);
void api_handle_gen_req_part1(void);
void api_handle_gen_req_part2(void);
void api_handle_task_request(void);
void api_handle_dep_req_part1(void);
void api_handle_dep_req_part2(void);
void api_handle_dep_req_part3(void);
void api_handle_dep_req_part4(void);
void api_handle_dep_req_part5(void);
void api_handle_dep_req_part6(void);
void api_handle_ubox_req_part1(void);
void api_handle_ubox_req_part2(void);
void api_handle_ubox_req_part3(void);
void api_handle_ubox_req_part4(void);
void api_handle_ubox_req_part5(void);
void api_handle_tkt_req_part1(void);
void api_handle_tkt_req_part2(void);
void api_handle_tkt_req_part3(void);
void api_handle_ver_request(void);
void api_handle_real_request(void);
void api_handle_seldep_req_part1(void);
void api_handle_seldep_req_part2(void);
void api_handle_download_request (unsigned int numReq, char *fileName);
void api_parse_save_request(unsigned int numReq);
void api_parse_sta_request(unsigned int numReq, unsigned char len, unsigned char *data);
void api_parse_mode_request(unsigned int numReq);
void api_parse_ap_request(unsigned int numReq, unsigned char len, unsigned char *data);
void api_parse_system_request(unsigned int numReq, unsigned char len, unsigned char *data);
void api_parse_upload_request (unsigned int numReq);
void api_parse_task_request (unsigned int numReq, unsigned char len, unsigned char *data);
void api_parse_seldep_request(unsigned int numReq, unsigned char len, unsigned char *data);
void api_parse_department_request(unsigned int numReq, unsigned char len, unsigned char *data);
void api_parse_ubox_request(unsigned int numReq, unsigned char len, unsigned char *data);
void api_parse_tkt_request(unsigned int numReq, unsigned char len, unsigned char *data);
void api_parse_ver_request(unsigned int numReq, unsigned char len, unsigned char *data);
void api_parse_format_request(unsigned int numReq, unsigned char len, unsigned char *data);

/*----------------------------------------------------------------------------
 * Funções:		https_api_request  
 * Descrição: 	Função chamada pelo EVENTO WIFI gerado ao receber uma requisição 
 *				http. Verifica qual o comando "/api" foi recebido na requisição.
 * Parâmetros: 	frames - Número de frames
 *				numReq - Identificador da requisição
 *				method - Método usado na requisição http
 *				len - Comprimento dos dados
 *				str - string do comando /api
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void https_api_request(unsigned char frames, unsigned int numReq, unsigned char method, 
								unsigned char len, unsigned char *str)
{
  	api_req_handled=0;
	authOk=0;

	if (len == API_SAVE_LEN && 							// ___Req POST: "/api/save"
  		memcmp(str, API_SAVE, API_SAVE_LEN)==0)
	{
		api_parse_save_request(numReq);					// Analisa dados http recebidos
	}
	else if (len == API_BOOT_LEN &&						// ___Req GET: "/api/boot"
  		memcmp(str, API_BOOT, API_BOOT_LEN)==0)
	{
		if (method==METHOD_GET)
			api_handle_reboot_request(numReq);			// Prepara resposta http para enviar
	}
	else if (len == API_STATUS_LEN && 					// ___Req GET: "/api/status"
  		memcmp(str, API_STATUS, API_STATUS_LEN)==0)
	{
    	api_handle_status_request();					// Prepara resposta http para enviar
	}
	else if (len == API_NETWORK_LEN &&	 				// ___Req GET: "/api/networks"
  		memcmp(str, API_NETWORK, API_NETWORK_LEN)==0)
	{
		api_handle_networks_request(numReq);			// Prepara resposta http para enviar
	}
	else if (len == API_DIR_LEN && 						// ___Req GET: "/api/dirs"
  		memcmp(str, API_DIR, API_DIR_LEN)==0)
	{
		api_handle_dir_request(numReq);					// Prepara resposta http para enviar
	}
	else if (len == API_APGET_LEN && 					// ___Req GET: "/api/ap"
  		memcmp(str, API_APGET, API_APGET_LEN)==0)
	{
		if (frames==2)		api_handle_ap_req_part1();  // Prepara resposta http para enviar (part 1)
		else if (frames==1)	api_handle_ap_req_part2();	// Prepara resposta http para enviar (part 2)
		else if (frames==0)	api_handle_ap_req_part3();	// Prepara resposta http para enviar (part 3)
	}											
	else if (len > API_APSET_LEN &&						// ___Req POST: "/api/ap/"
  		memcmp(str, API_APSET, API_APSET_LEN)==0)
	{
    	api_parse_ap_request(numReq, 					// Analisa dados http recebidos
			len-API_APSET_LEN,&str[API_APSET_LEN]);						
	}
	else if (len == API_STAGET_LEN && 					// ___Req GET: "/api/sta"
  		memcmp(str, API_STAGET, API_STAGET_LEN)==0)
	{
    	if (frames==1)		api_handle_sta_req_part1();	// Prepara resposta http para enviar (part 1)
		else if (frames==0)	api_handle_sta_req_part2();	// Prepara resposta http para enviar (part 2)
	}
	else if (len > API_STASET_LEN &&					// ___Req POST: "/api/sta/"
  		memcmp(str, API_STASET, API_STASET_LEN)==0)
	{
  		api_parse_sta_request(numReq, 					// Analisa dados http recebidos
			len-API_STASET_LEN,&str[API_STASET_LEN]);						
	}													
	else if (len == API_SYSGET_LEN &&					// ___Req GET "/api/system"
  		memcmp(str, API_SYSGET, API_SYSGET_LEN)==0)
	{
    	if (frames==1)		api_handle_gen_req_part1();	// Prepara resposta http para enviar (part 1)
		else if (frames==0)	api_handle_gen_req_part2();	// Prepara resposta http para enviar (part 2)
	}
	else if (len > API_SYSSET_LEN &&					// ___Req POST: "/api/system/"
  		memcmp(str, API_SYSSET, API_SYSSET_LEN)==0)
	{
  		api_parse_system_request(numReq,				// Analisa dados http recebidos
			len-API_SYSSET_LEN,&str[API_SYSSET_LEN]);						
	}
	else if (len == API_MODE_LEN && 					// ___Req POST: "/api/mode"
  		memcmp(str, API_MODE, API_MODE_LEN)==0)
	{
		api_parse_mode_request(numReq);					// Analisa dados http recebidos
	}
	else if (len == API_SAVEFILE_LEN && 				// ___Req POST: "/api/upload"
  		memcmp(str, API_SAVEFILE, API_SAVEFILE_LEN)==0)
	{
		api_parse_upload_request(numReq);				// Analisa dados http recebidos
	}
	else if (len > API_READFILE_LEN && 					// ___Req GET: "/api/download/file.nnn"
  		memcmp(str, API_READFILE, API_READFILE_LEN)==0)
	{
		api_handle_download_request(numReq, 			// Prepara resposta http para enviar
					(char *)&str[API_READFILE_LEN]);	
	}
	else if (len == API_TASK_LEN && 					// ___Req POST: "/api/task"
  		memcmp(str, API_TASK, API_TASK_LEN)==0)
	{
		if (method==METHOD_POST)
			api_parse_task_request(numReq,				// Analisa dados http recebidos
				len-API_TASK_LEN,&str[API_TASK_LEN]);
		else if (method==METHOD_GET)
			api_handle_task_request();					// Prepara resposta http para enviar
	}
	else if (len == API_SELDEP_LEN && 					// ___Req POST: "/api/seldep"
  		memcmp(str, API_SELDEP, API_SELDEP_LEN)==0)
	{
		if (method==METHOD_POST)
			api_parse_seldep_request(numReq,			// Analisa dados http recebidos
				len-API_SELDEP_LEN,&str[API_SELDEP_LEN]);
		else if (method==METHOD_GET)
		{
			if (frames==1)api_handle_seldep_req_part1();// Prepara resposta http part1
			else if (frames==0)api_handle_seldep_req_part2();
		}
	}
	else if (len == API_DEPGET_LEN && 					// ___Req GET: "/api/dep"
  		memcmp(str, API_DEPGET, API_DEPGET_LEN)==0)
	{
    	if (frames==5)		api_handle_dep_req_part1();	// Prepara resposta http para enviar (part 1)
		else if (frames==4)	api_handle_dep_req_part2();	// Prepara resposta http para enviar (part 2)
		else if (frames==3)	api_handle_dep_req_part3();	// Prepara resposta http para enviar (part 3)
		else if (frames==2)	api_handle_dep_req_part4();	// Prepara resposta http para enviar (part 4)
		else if (frames==1)	api_handle_dep_req_part5();	// Prepara resposta http para enviar (part 5)
		else if (frames==0)	api_handle_dep_req_part6();	// Prepara resposta http para enviar (part 6)
	}
	else if (len > API_DEPSET_LEN && 					// ___Req POST: "/api/dep/"
  		memcmp(str, API_DEPSET, API_DEPSET_LEN)==0)
	{
		api_parse_department_request(numReq,			// Analisa dados http recebidos
			len-API_DEPSET_LEN,&str[API_DEPSET_LEN]);	
	}
	else if (len == API_BOXGET_LEN && 					// ___Req GET: "/api/userbox"
  		memcmp(str, API_BOXGET, API_BOXGET_LEN)==0)
	{
    	if (frames==4)		api_handle_ubox_req_part1();// Prepara resp http (part 1)
		else if (frames==3)	api_handle_ubox_req_part2();// Prepara resp http (part 2)
		else if (frames==2)	api_handle_ubox_req_part3();// Prepara resp http (part 3)
		else if (frames==1)	api_handle_ubox_req_part4();// Prepara resp http (part 4)
		else if (frames==0)	api_handle_ubox_req_part5();// Prepara resp http (part 5)
	}
	else if (len > API_BOXSET_LEN && 					// ___Req POST: "/api/userbox/"
  		memcmp(str, API_BOXSET, API_BOXSET_LEN)==0)
	{
		api_parse_ubox_request(numReq,					// Analisa dados http recebidos
			len-API_BOXSET_LEN,&str[API_BOXSET_LEN]);	
	}
	else if (len == API_TKTGET_LEN && 					// ___Req GET: "/api/ticket"
  		memcmp(str, API_TKTGET, API_TKTGET_LEN)==0)
	{
    	if (frames==2)		api_handle_tkt_req_part1(); // Prepara resp http (part 1)
		else if (frames==1)	api_handle_tkt_req_part2(); // Prepara resp http (part 2)
		else if (frames==0)	api_handle_tkt_req_part3(); // Prepara resp http (part 3)
	}
	else if (len > API_TKTSET_LEN && 					// ___Req POST: "/api/ticket/"
  		memcmp(str, API_TKTSET, API_TKTSET_LEN)==0)
	{
		api_parse_tkt_request(numReq,					// Analisa dados http recebidos
			len-API_TKTSET_LEN,&str[API_TKTSET_LEN]);	
	}
	else if (len == API_VERGET_LEN && 					// ___Req GET: "/api/ver"
  		memcmp(str, API_VERGET, API_VERGET_LEN)==0)
	{
		api_handle_ver_request();						// Prepara resposta http para enviar
	}
	else if (len > API_VERSET_LEN && 					// ___Req POST: "/api/ver/"
  		memcmp(str, API_VERSET, API_VERSET_LEN)==0)
	{
		api_parse_ver_request(numReq,					// Analisa dados http recebidos
			len-API_VERSET_LEN,&str[API_VERSET_LEN]);	
	}
	else if (len == API_REAL_LEN && 					// ___Req GET: "/api/real"
  		memcmp(str, API_REAL, API_REAL_LEN)==0)
	{
		api_handle_real_request();						// Prepara resposta http para enviar
	}
	else if (len > API_FORMAT_LEN && 					// ___Req POST: "/api/format"
  		memcmp(str, API_FORMAT, API_FORMAT_LEN)==0)
	{
		api_parse_format_request(numReq,				// Analisa dados http recebidos
			len-API_FORMAT_LEN,&str[API_FORMAT_LEN]);
	}

	if (api_req_handled==0)								// Nenhuma requisição foi tratada..
		api_add_http_headers('n');						// HTTP headers "404 - Not Found"
}

/*----------------------------------------------------------------------------
 * Funções:		https_api_request_numFrames  
 * Descrição: 	Função chamada pelo EVENTO WIFI gerado ao receber uma requisicao
 *				http. Indica o número de frames que serão devolvidos na resposta.
 * Parâmetros: 	len - Comprimento dos dados
 *				data - Dados da requisição http
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
unsigned char https_api_request_numFrames(unsigned char len, unsigned char *data)
{
  	unsigned char frames=1;

	if (len == API_APGET_LEN && 	   					// ___Req GET: "/api/ap/settings"
		memcmp(data, API_APGET, API_APGET_LEN)==0)
	{
		frames=3;
	}

	else if (len == API_STAGET_LEN &&   				// ___Req GET: "/api/sta/settings"
		memcmp(data, API_STAGET, API_STAGET_LEN)==0)
	{
    	frames=2;
	}

	else if (len == API_SYSGET_LEN &&   				// ___Req GET: "/api/system"
		memcmp(data, API_SYSGET, API_SYSGET_LEN)==0)
	{
    	frames=2;
	}
	else if (len == API_SELDEP_LEN &&   				// ___Req GET: "/api/seldep"
		memcmp(data, API_SELDEP, API_SELDEP_LEN)==0)
	{
    	frames=2;
	}
	else if (len == API_DEPGET_LEN &&   				// ___Req GET: "/api/dep"
		memcmp(data, API_DEPGET, API_DEPGET_LEN)==0)
	{
    	frames=6;
	}
	else if (len == API_BOXGET_LEN &&   				// ___Req GET: "/api/userbox"
		memcmp(data, API_BOXGET, API_BOXGET_LEN)==0)
	{
    	frames=5;
	}		 
	else if (len == API_TKTGET_LEN &&   				// ___Req GET: "/api/ticket"
		memcmp(data, API_TKTGET, API_TKTGET_LEN)==0)
	{
    	frames=3;
	}

	return frames;
}

/*----------------------------------------------------------------------------
 * Funções:		https_api_request_cmd  
 * Descrição: 	Função chamada pelo EVENTO WIFI gerado ao receber uma requisicao
 *				http. Verifica se tem alguma lista de informações ou outras tarefas 
 *				a serem devolvidas ou realizadas na requisição.
 * Parâmetros: 	method - Método usado na requisição http
 *				len - Comprimento dos dados
 *				data - Dados da requisição http
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
unsigned char https_api_request_cmd(unsigned char method, unsigned char len, unsigned char *data)
{
  	unsigned char cmd=CMDFW_NONE;

	if (len == API_BOOT_LEN &&							// ___Req POST: "/api/reboot/"
  		memcmp(data, API_BOOT, API_BOOT_LEN)==0)
	{
		if (method==METHOD_POST)
			cmd=CMDFW_BOOT;								// Realiza o boot do módulo
	}

	else if (len == API_NETWORK_LEN && 					// ___Req GET: "/api/networks"
  		memcmp(data, API_NETWORK, API_NETWORK_LEN)==0)
	{
		cmd=CMDFW_SCAN;	 								// Devolve lista de redes WIFI
	}

	else if (len == API_DIR_LEN &&		  				// ___Req GET: "/api/dirs"
		memcmp(data, API_DIR, API_DIR_LEN)==0)
	{
		cmd=CMDFW_DIR;									// Devolve lista de arquivos
	}

	else if (len > API_READFILE_LEN &&		  			// ___Req GET: "/api/download"
		memcmp(data, API_READFILE, API_READFILE_LEN)==0)
	{
		cmd=CMDFW_FILE;									// Devolve dados do arquivo
	}

	return cmd;
}

/*----------------------------------------------------------------------------
 * Funções:		https_api_request_data  
 * Descrição: 	Função chamada pelo EVENTO WIFI gerado ao receber os dados de
 *				da requisição POST http. Armazena os dados recebidos
 * Parâmetros: 	len - Comprimento dos dados
 *				data - Dados da requisição http
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void https_api_request_data(unsigned char len, unsigned char *data)
{
	if (len!=0)
	{
		if (dataReqHttp.key==ID_UPLOAD)					// Comando "/api" = UPLOAD
		{
			unsigned int n;
			if (f != NULL)								// Verifica se o arquivo foi aberto 
			{
        		while ((n = len) > 0) 					// Enquanto existirem dados no buffer,
				{										// verifica comprimento dos dados:
					if (_DEF_DRIVE==_SDCARD && n>512) 
						n = 512;			  			// Grava até 512 bytes por vez no SDCARD
					else if (_DEF_DRIVE==_FLASHFS && n > 256)
						n = 256;						// Grava até 256 bytes por vez na FLASH
       				fwrite (data,1,n,f);				// Grava dados no drive
       				data += n;							// Incrementa o ponteiro do buffer
        			len -= n;							// Decrementa quantidade de bytes para ler
        		}
	      	}
	      	return;
		}
		else
		{
			short cont;
			for (cont=0 ; cont<len ; cont++)
			{
				dataReqHttp.data[dataReqHttp.len]=data[cont];
				dataReqHttp.len++;
			}
			dataReqHttp.data[dataReqHttp.len]=0;		// Adiciona um null no final, quando é
		}												// número da problema com função "scanf" 
	}													// sem o null...
}

/*----------------------------------------------------------------------------
 * Funções:		https_api_request_header  
 * Descrição: 	Função chamada pelo EVENTO WIFI gerado ao receber o cabeçalho 
 *				da requisição http. 
 * Parâmetros: 	len - Comprimento dos dados do cabeçalho
 *				data - Dados do cabeçalho
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void https_api_request_header(unsigned char len, unsigned char *data)
{
	if (cfg.authEnable)	 								// Se autenticação habilitada...
	{
	  unsigned char i=0;

	  while (len--!=0)									// Varredura no cabeçalho..
	  {
		if (data[i]=='A' && data[i+1]=='u')				// Procura por tag "Authorization"
		{
		  if (memcmp(&data[i],"Authorization: Basic ",21)==0)
		  {
		  	i+=21;										// Ao encontrar..	
			if (checkAuth (	"admin",					// Verifica usuário e senha...
							(char *)cfg.authPassword,
							(char *)&data[i]))			// Com dados criptografados em base64
				authOk=1;
			break;										// Sai do loop
		  }
		}
		i++;
	  }
	  if (authOk==0)
	    api_add_http_headers ('u');						// HTTP headers "401 - Unautherized"
	}													// Sobregrava os dados de resposta
}

/*----------------------------------------------------------------------------
 * Funções:		https_api_request_finished  
 * Descrição: 	Função chamada pelo EVENTO WIFI gerado ao receber o fim da
 *				requisição http. 
 * Parâmetros: 	Nenhum															  
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void https_api_request_finished(void)
{
	if (dataReqHttp.key==ID_UPLOAD)
	{
		if (f != NULL) 
		{
	    	fclose (f);									// Fecha o arquivo 
			switch (taskFile)							// Verifica tarefa do arquivo
			{
				case 1: WEBREQUEST=UPD_SCR1; break;		// Sinaliza que atualizou a tela 1
				case 2:	WEBREQUEST=UPD_SCR2; break; 	// Sinaliza que atualizou a tela 2
				case 3: WEBREQUEST=UPD_LOGO; break; 	// Sinaliza que atualizou o logo
			}
			taskFile=0;									// Reseta flag de tarefa
		}												
		api_add_http_headers('f');						// HTTP headers "image/JPEG"
	}
	else
	{
		api_save_from_post();							// Grava informações pendentes

		api_add_http_headers('f');						// HTTP headers "application/JSON"
	}

	dataReqHttp.request=0;
	dataReqHttp.len=0;
}

/*----------------------------------------------------------------------------
 * Função:		api_add_json_quoted_string  
 * Descrição:   Adiciona um string com aspas no buffer
 ----------------------------------------------------------------------------*/
void api_add_json_quoted_string(unsigned char len, char *str)
{
	char i;
  	api_resp[api_resp_len++] = '\x22';
  	for (i=0 ; i<len ; i++) api_resp[api_resp_len++]=*str++;
  	api_resp[api_resp_len++] = '\x22';
}

/*----------------------------------------------------------------------------
 * Função:		api_add_json_string  
 * Descrição:   Adiciona um string no buffer
 ----------------------------------------------------------------------------*/
void api_add_json_string(unsigned char len, char *str)
{
	char i;
	for (i=0 ; i<len ; i++) api_resp[api_resp_len++]=*str++;
}

/*----------------------------------------------------------------------------
 * Função:		api_add_json_integer  
 * Descrição:   Converte inteiro em string e adiciona no buffer
 ----------------------------------------------------------------------------*/
void api_add_json_integer(int value, char minLength, char isUnsigned)
{
    char isNegative=0;
	long long reverseValue=0;
	unsigned int newValue=0;
	char cnt=0;

	if (value == 0)	   									// Se o número for zero:
    {
		api_resp[api_resp_len++]='0';					// preeche string com "0"
		cnt++;
		
		while (cnt<minLength)				 			// Verifica comprimento requerido e
		{
			api_resp[api_resp_len++]='0';				// Preenche com "0" antes do número	
			minLength--;
		}
        return;
    }

    if (value < 0)										// Se número negativo:
    {
        if (isUnsigned==0)
			isNegative=1;								// Indica através da flag.
    }

    newValue = value;									// Transforma em valor positivo.

   	while (newValue != 0)								// Enquanto número diferente de zero:
   	{
      	reverseValue=reverseValue*10;					// Calcula o número reverso...
      	reverseValue=reverseValue+newValue%10;			// Ex:  1234 = 4321
      	newValue=newValue/10;							// Antes da conversão..
		cnt++;
   	}

    if (isNegative)										// Se for valor negativo:
        api_resp[api_resp_len++]='-';					// Adiciona sinal "-"

	while (cnt<minLength)								// Verifica comprimento requerido e
	{
		api_resp[api_resp_len++]='0';					// Preenche com "0" antes do número
		minLength--;
	}

    while (cnt-- != 0)  								// Enquanto numero diferente de zero:
    {
        int rem =reverseValue%10;						// Transforma em String....
        api_resp[api_resp_len++]=(rem>9)?(rem-10)+'a':rem+'0';
        reverseValue=reverseValue/10;
    }
}

unsigned int api_add_json_swap (unsigned int ip)
{
	unsigned int result=0;
    result = (((ip & 0xFF) << 24)
      | ((ip & 0xFF00) << 8)
      | ((ip >> 8) & 0xFF00)
      | ((ip >> 24) & 0xFF));

	return result;
}

//void api_add_json_ip (unsigned int value)
//{
//	unsigned int backup = value;
//
//    while (value!=0)  								// Enquanto numero diferente de zero:
//    {
//        int rem =value%10;							// Transforma em String....
//        api_resp[api_resp_len++]=(rem>9)?(rem-10)+'a':rem+'0';
//        value=value/10;
//    }
//
//	backup=api_add_json_swap(backup);
//
//    while (backup!=0)  								// Enquanto numero diferente de zero:
//    {
//        int rem =backup%10;							// Transforma em String....
//        api_resp[api_resp_len++]=(rem>9)?(rem-10)+'a':rem+'0';
//        backup=backup/10;
//    }
//}
   
/*----------------------------------------------------------------------------
 * Função:		api_add_json_quoted_time  
 * Descrição:   Converte hora e minuto em string e adiciona no buffer
 ----------------------------------------------------------------------------*/
void api_add_json_quoted_time (unsigned char hour, unsigned char min, unsigned char seg, char useSeg)
{
	api_resp[api_resp_len++] = '\x22';
	api_add_json_integer(hour,2,0);
	api_resp[api_resp_len++] = ':';
	api_add_json_integer(min,2,0);
	if (useSeg)
	{
		api_resp[api_resp_len++] = ':';
		api_add_json_integer(seg,2,0);
	}
	api_resp[api_resp_len++] = '\x22';
}

/*----------------------------------------------------------------------------
 * Função:		api_add_json_quoted_date
 * Descrição:   Converte hora e minuto em string e adiciona no buffer
 ----------------------------------------------------------------------------*/
void api_add_json_quoted_date (unsigned char hour, unsigned char min, unsigned char seg)
{
	api_resp[api_resp_len++] = '\x22';
	api_add_json_integer(hour,2,0);
	api_resp[api_resp_len++] = '/';
	api_add_json_integer(min,2,0);
	api_resp[api_resp_len++] = '/';
	api_add_json_integer(seg,2,0);
	api_resp[api_resp_len++] = '\x22';
}

/*----------------------------------------------------------------------------
 * Função:		api_add_json_object_name  
 * Descrição:   Adiciona um nome de objeto no buffer
 ----------------------------------------------------------------------------*/
void api_add_json_object_name(unsigned char len, char *name)
{
	char i;
  	api_resp[api_resp_len++] = '\x22';
  	for (i=0 ; i<len ; i++) api_resp[api_resp_len++]=*name++;
  	api_resp[api_resp_len++] = '\x22';
	api_resp[api_resp_len++] = ':';
}

/*----------------------------------------------------------------------------
 * Função:		api_add_json_array_start
 * Descrição:   Adiciona inicio de array no buffer
 ----------------------------------------------------------------------------*/
void api_add_json_array_start(void)
{
  	api_resp[api_resp_len++]='[';
}

/*----------------------------------------------------------------------------
 * Função:		api_add_json_array_end
 * Descrição:   Adicion fim de array no buffer
 ----------------------------------------------------------------------------*/
void api_add_json_array_end(void)
{
  	api_resp[api_resp_len++]=']';
}

/*----------------------------------------------------------------------------
 * Função:		api_add_json_object_start
 * Descrição:   Adiciona inicio de objeto no buffer
 ----------------------------------------------------------------------------*/
void api_add_json_object_start(void)
{
  	api_resp[api_resp_len++]='{';
}

/*----------------------------------------------------------------------------
 * Função:		api_add_json_object_end
 * Descrição:   Adiciona fim de objeto no buffer
 ----------------------------------------------------------------------------*/
void api_add_json_object_end(void)
{
  	api_resp[api_resp_len++]='}';
}

/*----------------------------------------------------------------------------
 * Função:		api_add_crlf
 * Descrição:   Adicion caracteres de mudança de linha no buffer
 ----------------------------------------------------------------------------*/
void api_add_crlf(void)
{
  	api_resp[api_resp_len++]='\x0D';
  	api_resp[api_resp_len++]='\x0A';
}

/*----------------------------------------------------------------------------
 * Função:		api_add_comma
 * Descrição:   Adiciona vírgula no buffer
 ----------------------------------------------------------------------------*/
void api_add_comma(void)
{
  	api_resp[api_resp_len++]=',';
}

/*----------------------------------------------------------------------------
 * Função:		api_dec
 * Descrição:   Subtrai buffer
 ----------------------------------------------------------------------------*/
void api_dec(void)
{
  	api_resp[--api_resp_len]='\x0';
}

/*----------------------------------------------------------------------------
 * Função:		api_add_http_headers
 * Descrição:   Adiciona cabeçalho de resposta HTTP no buffer
 ----------------------------------------------------------------------------*/
void api_add_http_headers(char type)
{
	api_resp_len=0;											// Inicializa resposta http

	if (type=='j' || type=='o' || type=='f')
		api_add_json_string (15,"HTTP/1.1 200 OK");			// HTTP 200 - OK
	else if (type=='u')
		api_add_json_string(25,"HTTP/1.1 401 Unauthorized");// HTTP 401 - Não autorizado
	else if (type=='n')
		api_add_json_string (22,"HTTP/1.1 404 Not Found");	// HTTP 404 - Arquivo não encontrado
	api_add_crlf();											// Add CR+LF.	
	
	api_add_json_string (16,"Server: MIG 1.05");			// Nome do Servidor
	api_add_crlf();											// Add CR+LF.

	api_add_json_string (14,"Content-Type: ");				// Content-Type
	if (type=='j')
	{
		api_add_json_string (16,"application/json");		// Dados formato JSON
		api_add_crlf();										// Add CR+LF.
		api_add_json_string(33,"Cache-Control: private, max-age=0");// Add Cache-Control field to prevent caching.
	}
	else if (type=='f' || type=='u')
	{
		api_add_json_string (9,"text/html");				// Dados texto HTML
		api_add_crlf();										// Add CR+LF.
		api_add_json_string (17,"Connection: close");
	}
	else if (type=='o')
		api_add_json_string (24,"application/octet-stream");// Dados formato binario/texto
	else if (type=='u')
		api_add_json_string (47,"WWW-Authenticate: Basic realm=\x22Wyma Tecnologia\x22");// Dados de Imagem

	api_add_crlf();											// Add CR+LF.
	api_add_crlf();											// Add CR+LF.
}

/*----------------------------------------------------------------------------
 * Função:		api_add_nibble
 * Descrição:   Converte 4 bits em hexa e adiciona no buffer
 ----------------------------------------------------------------------------*/
void api_add_nibble(unsigned char nibble)
{
  if (nibble < 0xA)
      api_resp[api_resp_len++]=nibble+0x30;
  else
      api_resp[api_resp_len++]=nibble+0x37;
}

/*----------------------------------------------------------------------------
 * Função:		api_add_hex_string
 * Descrição:   Convert array de numero em string de hexa e adiciona no buffer
 ----------------------------------------------------------------------------*/
void api_add_hex_string(unsigned char len, unsigned char *buffer)
{
	unsigned char i=0;
	while (i<len)
	{
		api_add_nibble(*buffer/16);
		api_add_nibble(*buffer&0xF);
		buffer++;
		i++;
	}
}

/*----------------------------------------------------------------------------
 * Função:		api_add_quoted_hex_string
 * Descrição:   Convert array de numero em string de hexa com aspas e adiciona no buffer
 ----------------------------------------------------------------------------*/
void api_add_quoted_hex_string(unsigned char len, unsigned char *buffer)
{
  	unsigned char i=0;
	api_resp[api_resp_len++] = '\x22';
	while (i<len)
	{
		api_add_nibble(*buffer/16);
		api_add_nibble(*buffer&0xF);
		buffer++;
		i++;
	}
	api_resp[api_resp_len++] = '\x22';
}

/*----------------------------------------------------------------------------
 * Função:		verLen
 * Descrição:   Verifica comprimento da string
 ----------------------------------------------------------------------------*/
unsigned char verLen (char *str)
{
	unsigned char len=0;
	while (str[len++]!=0);
	return len;
}

/*----------------------------------------------------------------------------
 * Função:		length
 * Descrição:   Conta número de bytes da string
 * Parametros:	Nenhum
 * Retorno: 	Comprimento da string
 ----------------------------------------------------------------------------*/
unsigned char length (char *str)
{
	unsigned char len=0;
	while (*str++!=0) len++;
	return len;
}

/*----------------------------------------------------------------------------
 * Função:		api_add_settings_item
 * Descrição:   Recupera um parâmetro gravado em Flash para adicionar ao buffer de
 *				resposta http ao comando GET recebido
 ----------------------------------------------------------------------------*/
void api_add_settings_item(	unsigned short item, unsigned char len, char *name)
{
	unsigned char cont;
	unsigned int ip;
	TIME_TYPE time=Get_RTC();

	// Setting item name
	api_add_json_object_name(len, name);

	// Setting item value
	if (item==ID_BOOT) api_add_json_integer(0,0,0);
	else if (item==ID_MODE) api_add_json_integer(cfg.wifiMode,0,0);
	else if (item==ID_SSID) api_add_json_quoted_string(length(cfg.wifiSsid), cfg.wifiSsid);
	else if (item==ID_BSSID) api_add_quoted_hex_string(6,wifiBSsid);
	else if (item==ID_CHANNEL) api_add_json_integer(cfg.wifiChannel,0,0);
	else if (item==ID_SECURITY) api_add_json_integer(cfg.wifiSecurity,0,0);
	else if (item==ID_WIFIPW) api_add_json_quoted_string(length(cfg.wifiPw), cfg.wifiPw);
	else if (item==ID_DHCP) api_add_json_integer(cfg.wifiDhcp,0,0);
	else if (item==ID_IP) api_add_json_integer(cfg.wifiIp,0,1);
	else if (item==ID_MASK) api_add_json_integer(cfg.wifiMask,0,1);
	else if (item==ID_GW) api_add_json_integer(cfg.wifiGateway,0,1);
	else if (item==ID_DNS1) api_add_json_integer(cfg.wifiDns1,0,1);
	else if (item==ID_DNS2) api_add_json_integer(cfg.wifiDns2,0,1);
	else if (item==ID_DHCPS) api_add_json_integer(cfg.wifiSdhcp,0,0);
	else if (item==ID_DHCPS_IP) api_add_json_integer(cfg.wifiSdhcpIp,0,1);
	else if (item==ID_DRIVE) api_add_json_integer(cfg.driver,0,0);
	else if (item==ID_DATE) api_add_json_quoted_date(time.DAY,time.MONTH,time.YEAR);
	else if (item==ID_TIME) api_add_json_quoted_time(time.HOUR,time.MINUTE,time.SECOND,1);
	else if (item==ID_TICKETTEST) api_add_json_integer(cfg.autoTest,0,0);
	else if (item==ID_TIMELIVE) api_add_json_integer(cfg.keepLiveTicket/3600,0,0);
	else if (item==ID_CONTROL) api_add_json_integer(cfg.enTimeAuto,0,0);
	else if (item==ID_TIMEON) api_add_json_quoted_time(cfg.timeHourOn,cfg.timeMinOn,0,0);
	else if (item==ID_TIMEOFF) api_add_json_quoted_time(cfg.timeHourOff,cfg.timeMinOff,0,0);
	else if (item==ID_D1) api_add_json_integer(cfg.weekDay&1,0,0);
	else if (item==ID_D2) api_add_json_integer((cfg.weekDay>>1)&1,0,0);
	else if (item==ID_D3) api_add_json_integer((cfg.weekDay>>2)&1,0,0);
	else if (item==ID_D4) api_add_json_integer((cfg.weekDay>>3)&1,0,0);
	else if (item==ID_D5) api_add_json_integer((cfg.weekDay>>4)&1,0,0);
	else if (item==ID_D6) api_add_json_integer((cfg.weekDay>>5)&1,0,0);
	else if (item==ID_D7) api_add_json_integer((cfg.weekDay>>6)&1,0,0);
	else if (item==ID_REPORTTIME) api_add_json_quoted_time(cfg.timeHourRep,cfg.timeMinRep,0,0);
	else if (item==ID_REPORTCSV) api_add_json_integer(cfg.saveReport,0,0);
	else if (item==ID_MIGMODE) api_add_json_integer(cfg.runMode,0,0);
	else if (item==ID_MIGIP) 
	{
		ip=cfg.ipServer[0];
		ip|=(cfg.ipServer[1]<<8);
		ip|=(cfg.ipServer[2]<<16);
		ip|=(cfg.ipServer[3]<<24);
		api_add_json_integer(ip,0,1); 
	}
	else if (item==ID_LINE) api_add_json_integer(cfg.broadcastInfo,0,0);
	else if (item==ID_INTERF) api_add_json_integer(cfg.srvItf,0,0);
	else if (item==ID_TASK) api_add_json_integer(taskFile,0,0);
	// DEP.HTML
	else if (item==ID_DNUM) api_add_json_integer(DEP,0,0);
	else if (item==ID_DNAME1) api_add_json_quoted_string(length(cfg.depName[0]), cfg.depName[0]);
	else if (item==ID_DNAME2) api_add_json_quoted_string(length(cfg.depName[1]), cfg.depName[1]);
	else if (item==ID_DNAME3) api_add_json_quoted_string(length(cfg.depName[2]), cfg.depName[2]);
	else if (item==ID_DNAME4) api_add_json_quoted_string(length(cfg.depName[3]), cfg.depName[3]);
	else if (item==ID_DNAME5) api_add_json_quoted_string(length(cfg.depName[4]), cfg.depName[4]);
	else if (item==ID_DNAME6) api_add_json_quoted_string(length(cfg.depName[5]), cfg.depName[5]);
	else if (item==ID_DENAB) api_add_json_integer(cfg.depEnable[DEP-1],0,0);
	else if (item==ID_DNAME) api_add_json_quoted_string(length(cfg.depName[DEP-1]), cfg.depName[DEP-1]);
	else if (item==ID_DALPH) api_add_json_quoted_string(1,&cfg.depAlpha[DEP-1]);
	else if (item==ID_DRANG1) api_add_json_integer(cfg.depRange[DEP-1][0],0,0);
	else if (item==ID_DRANG2) api_add_json_integer(cfg.depRange[DEP-1][1],0,0);
	else if (item==ID_DCLOS1) api_add_json_integer(cfg.depCloseNext[DEP-1],0,0);
	else if (item==ID_DCLOS2) api_add_json_integer(cfg.depCloseRep[DEP-1],0,0);
	else if (item==ID_DCALL1) api_add_json_integer((cfg.depSchPri[DEP-1]&0xF0)>>4,0,0);
	else if (item==ID_DCALL2) api_add_json_integer(cfg.depSchPri[DEP-1]&0x0F,0,0);
	else if (item==ID_DHLP1) api_add_json_integer((cfg.depHelp[DEP-1]&0xF0)>>4,0,0);
	else if (item==ID_DHLP2) api_add_json_integer(cfg.depHelp[DEP-1]&0x0F,0,0);
	else if (item==ID_DHLP3) 
	{
		if (cfg.depHelp[DEP-1]==0xF0) 	api_add_json_integer(1,0,0); 
		else 							api_add_json_integer(0,0,0);
	}
	else if (item==ID_DHLP4) api_add_json_integer((cfg.depSchHlp[DEP-1]&0xF0)>>4,0,0);
	else if (item==ID_DHLP5) api_add_json_integer(cfg.depSchHlp[DEP-1]&0x0F,0,0);
	else if (item==ID_DIPD) 
	{
		ip=cfg.depIpDisp[DEP-1][0];
		ip|=(cfg.depIpDisp[DEP-1][1]<<8);
		ip|=(cfg.depIpDisp[DEP-1][2]<<16);
		ip|=(cfg.depIpDisp[DEP-1][3]<<24);
		api_add_json_integer(ip,0,1);
	}
	else if (item==ID_DITFD)	api_add_json_integer(cfg.depItfDisp[DEP-1],0,0);
	else if (item==ID_DIPT) 
	{
		ip=cfg.depIpTop[0][DEP-1];
		ip|=(cfg.depIpTop[1][DEP-1]<<8);
		ip|=(cfg.depIpTop[2][DEP-1]<<16);
		ip|=(cfg.depIpTop[3][DEP-1]<<24);
		api_add_json_integer(ip,0,1);
	}
	else if (item==ID_DITFT) api_add_json_integer(cfg.depItfTop[DEP-1],0,0);
	else if (item==ID_ENATOP) api_add_json_integer(cfg.eTop[DEP-1],0,0);
	else if (item==ID_DLIND) api_add_json_integer(cfg.depRowDisp[DEP-1],0,0);
	else if (item==ID_DPRFD) api_add_json_integer(cfg.depIsBtPref[DEP-1],0,0);
	else if (item>=ID_DU01 && item<=ID_DU99)
	{
		cont = item-ID_DU01;
		api_add_json_string(1,cfg.depUsers[cont]==DEP?"1":"0");
	}
	// USERBOX.HTML
	else if (item>=ID_UB01 && item<=ID_UB99)
	{
		cont = item-ID_UB01;
		api_add_json_integer(cfg.userBox[cont],0,0);
	}	
	// TICKET.HTML
	else if (item==ID_LG) api_add_json_integer(cfg.logoEnable,0,0);
	else if (item==ID_DLEN) api_add_json_integer(cfg.lettEnable,0,0);
	else if (item==ID_DLNUM) api_add_json_integer(cfg.lenNumTicket,0,0);
	else if (item==ID_DHNUM) api_add_json_integer(cfg.typeNumTicket,0,0);
	else if (item==ID_CUT) api_add_json_integer(cfg.cut,0,0);
	else if (item==ID_VIA) api_add_json_integer(cfg.copy,0,0);
	else if (item==ID_H1OP) api_add_json_integer(cfg.optHead1,0,0);
	else if (item==ID_H2OP) api_add_json_integer(cfg.optHead2,0,0);
	else if (item==ID_H3OP) api_add_json_integer(cfg.optHead3,0,0);
	else if (item==ID_F1OP) api_add_json_integer(cfg.optFoot1,0,0);
	else if (item==ID_F2OP) api_add_json_integer(cfg.optFoot2,0,0);
	else if (item==ID_F3OP) api_add_json_integer(cfg.optFoot3,0,0);
	else if (item==ID_F4OP) api_add_json_integer(cfg.optFoot4,0,0);
	else if (item==ID_H1ME) api_add_json_quoted_string(length(cfg.header1),cfg.header1);
	else if (item==ID_H2ME) api_add_json_quoted_string(length(cfg.header2),cfg.header2);
	else if (item==ID_H3ME) api_add_json_quoted_string(length(cfg.header3),cfg.header3);
	else if (item==ID_F1ME)	api_add_json_quoted_string(length(cfg.footer1),cfg.footer1);
	else if (item==ID_F2ME)	api_add_json_quoted_string(length(cfg.footer2),cfg.footer2);
	else if (item==ID_F3ME)	api_add_json_quoted_string(length(cfg.footer3),cfg.footer3);
	else if (item==ID_F4ME)	api_add_json_quoted_string(length(cfg.footer4),cfg.footer4);
	// REAL.HTML
	else if (item==ID_FILA1) api_add_json_integer(queue[0],0,0);
	else if (item==ID_PREV1) api_add_json_integer(CalcForecast(1),0,0);
	else if (item==ID_FILA2) api_add_json_integer(queue[1],0,0);
	else if (item==ID_PREV2) api_add_json_integer(CalcForecast(2),0,0);
	else if (item==ID_FILA3) api_add_json_integer(queue[2],0,0);
	else if (item==ID_PREV3) api_add_json_integer(CalcForecast(3),0,0);
	else if (item==ID_FILA4) api_add_json_integer(queue[3],0,0);
	else if (item==ID_PREV4) api_add_json_integer(CalcForecast(4),0,0);
	else if (item==ID_FILA5) api_add_json_integer(queue[4],0,0);
	else if (item==ID_PREV5) api_add_json_integer(CalcForecast(5),0,0);
	else if (item==ID_FILA6) api_add_json_integer(queue[5],0,0);
	else if (item==ID_PREV6) api_add_json_integer(CalcForecast(6),0,0);
	// VER.HTML
	else if (item==ID_MODEL) api_add_json_quoted_string(4, "MIG6");
	else if (item==ID_VERS) api_add_json_quoted_string(4, "0103");
	else if (item==ID_AUTH) api_add_json_integer(cfg.authEnable,0,0);
	else if (item==ID_PW) api_add_json_quoted_string(length((char *)cfg.authPassword),(char *)cfg.authPassword);

	api_add_comma();									// Virgula
}

/*----------------------------------------------------------------------------
 * Função:		api_save_from_post
 * Descrição:   Salva dados recebidos de método post para memória flash.
 ----------------------------------------------------------------------------*/
void api_save_from_post(void)
{
	TIME_TYPE time=Get_RTC();
	int s[4];
	unsigned char var=0;

	if (dataReqHttp.key==ID_MODE)						// *  Nas páginas AP.html e STA.html 
	{													// o ID_MODE´é o último a ser
		if (dataReqHttp.data[0]=='1')	cfg.wifiMode=1;	// enviado então salvamos aqui..
		else							cfg.wifiMode=0;
		Write_Flash ();
		WEBREQUEST=UPD_NETWORK;							// Reinicializa rede wifi...
	}
	else if (dataReqHttp.key==ID_SAVE)					// *  Nas demais páginas salva aqui:
	{
		Write_Flash ();
	}
	else if (dataReqHttp.key==ID_SSID)
	{
		while (var<dataReqHttp.len)
		{
			cfg.wifiSsid[var]=dataReqHttp.data[var];
			var++;
		}
		cfg.wifiSsid[var]=0;
	}
	else if (dataReqHttp.key==ID_CHANNEL)
	{
		cfg.wifiChannel=dataReqHttp.data[0]&0x0F;
	}														
	else if (dataReqHttp.key==ID_SECURITY)
	{
		cfg.wifiSecurity=dataReqHttp.data[0]&0x0F;
	}
	else if (dataReqHttp.key==ID_WIFIPW)
	{
		while (var<dataReqHttp.len)
		{
			cfg.wifiPw[var]=dataReqHttp.data[var];
			var++;
		}
		cfg.wifiPw[var]=0;
	}
	else if (dataReqHttp.key==ID_DHCP)
	{
		if (dataReqHttp.data[0]=='1')	cfg.wifiDhcp=1;
		else							cfg.wifiDhcp=0;
	}
	else if (dataReqHttp.key==ID_IP)
	{
	    sscanf ((const char *)dataReqHttp.data, "%d", &s[0]);
		cfg.wifiIp = (s[0]&0xFF000000);
		cfg.wifiIp |= (s[0]&0x00FF0000);
		cfg.wifiIp |= (s[0]&0x0000FF00);
		cfg.wifiIp |= (s[0]&0x000000FF);
	}
	else if (dataReqHttp.key==ID_MASK)
	{
	    sscanf ((const char *)dataReqHttp.data, "%d", &s[0]);
		cfg.wifiMask = (s[0]&0xFF000000);
		cfg.wifiMask |= (s[0]&0x00FF0000);
		cfg.wifiMask |= (s[0]&0x0000FF00);
		cfg.wifiMask |= (s[0]&0x000000FF);
	}
	else if (dataReqHttp.key==ID_GW)
	{
	    sscanf ((const char *)dataReqHttp.data, "%d", &s[0]);
		cfg.wifiGateway = (s[0]&0xFF000000);
		cfg.wifiGateway |= (s[0]&0x00FF0000);
		cfg.wifiGateway |= (s[0]&0x0000FF00);
		cfg.wifiGateway |= (s[0]&0x000000FF);
	}
	else if (dataReqHttp.key==ID_DNS1)
	{
	    sscanf ((const char *)dataReqHttp.data, "%d", &s[0]);
		cfg.wifiDns1 = (s[0]&0xFF000000);
		cfg.wifiDns1 |= (s[0]&0x00FF0000);
		cfg.wifiDns1 |= (s[0]&0x0000FF00);
		cfg.wifiDns1 |= (s[0]&0x000000FF);
	}
	else if (dataReqHttp.key==ID_DNS2)
	{
	    sscanf ((const char *)dataReqHttp.data, "%d", &s[0]);
		cfg.wifiDns2 = (s[0]&0xFF000000);
		cfg.wifiDns2 |= (s[0]&0x00FF0000);
		cfg.wifiDns2 |= (s[0]&0x0000FF00);
		cfg.wifiDns2 |= (s[0]&0x000000FF);
	}
	else if (dataReqHttp.key==ID_DHCPS)
	{
		if (dataReqHttp.data[0]=='1')	cfg.wifiSdhcp=1;
		else							cfg.wifiSdhcp=0;
	}
	else if (dataReqHttp.key==ID_DHCPS_IP)
	{
	    sscanf ((const char *)dataReqHttp.data, "%d", &s[0]);
		cfg.wifiSdhcpIp = (s[0]&0xFF000000);
		cfg.wifiSdhcpIp |= (s[0]&0x00FF0000);
		cfg.wifiSdhcpIp |= (s[0]&0x0000FF00);
		cfg.wifiSdhcpIp |= (s[0]&0x000000FF);
	}
	// SYSTEM.HTML
	else if (dataReqHttp.key==ID_DRIVE)
	{
		if (dataReqHttp.data[0]=='2')
		{
			if (_DEF_DRIVE!=_FLASHFS)
			{
				Change_Drive (_FLASHFS);
				STATFILE=2;
			}
		}
		else
		{
			if (_DEF_DRIVE!=_SDCARD)
			{
				Change_Drive (_SDCARD);
				STATFILE=2;
			}
		}
	}
	else if (dataReqHttp.key==ID_DATE)
	{
		time=Get_RTC();
		sscanf ((const char *)dataReqHttp.data, "%d/%d/%d", &s[0],&s[1],&s[2]);
		if (s[0]<=31) time.DAY = s[0];
		if (s[1]<=12) time.MONTH = s[1];
		time.YEAR = s[2];

		time.WEEKDAY = DayOfWeek(time);
		time.YEARDAY = DayOfYear(time);

		Save_RTC(time);
	}
	else if (dataReqHttp.key==ID_TIME)
	{
		time=Get_RTC();
		sscanf ((const char *)dataReqHttp.data, "%d:%d:%d", &s[0],&s[1],&s[2]);
		if (s[0]<=23) time.HOUR = s[0];
		if (s[1]<=59) time.MINUTE = s[1];
		if (s[2]<=59) time.SECOND = s[2];
		Save_RTC(time);
	}
	else if (dataReqHttp.key==ID_TICKETTEST)
	{
		if (dataReqHttp.data[0]=='1')	cfg.autoTest=1;
		else							cfg.autoTest=0;
	}
	else if (dataReqHttp.key==ID_TIMELIVE)
	{
		sscanf ((const char *)dataReqHttp.data, "%d",&s[0]);
		if ((s[0]>=0)&&(s[0]<=18))
		{
			cfg.keepLiveTicket = s[0]*3600;
		}
	}
	else if (dataReqHttp.key==ID_CONTROL)
	{
		if (dataReqHttp.data[0]=='1')	cfg.enTimeAuto=1;
		else							cfg.enTimeAuto=0;
	}
	else if (dataReqHttp.key==ID_TIMEON)
	{
		sscanf ((const char *)dataReqHttp.data, "%d:%d", &s[0],&s[1]);
		if (s[0]<24) cfg.timeHourOn = s[0];
		if (s[1]<60) cfg.timeMinOn = s[1];
	}
	else if (dataReqHttp.key==ID_TIMEOFF)
	{
		sscanf ((const char *)dataReqHttp.data, "%d:%d", &s[0],&s[1]);
		if (s[0]<24) cfg.timeHourOff = s[0];
		if (s[1]<60) cfg.timeMinOff = s[1];
	}
	else if (dataReqHttp.key==ID_D1)
	{
		if (dataReqHttp.data[0]=='1')	cfg.weekDay|=1;
		else							cfg.weekDay&=~1;
	}
	else if (dataReqHttp.key==ID_D2)
	{
		if (dataReqHttp.data[0]=='1')	cfg.weekDay|=2;
		else							cfg.weekDay&=~2;
	}
	else if (dataReqHttp.key==ID_D3)
	{
		if (dataReqHttp.data[0]=='1')	cfg.weekDay|=4;
		else							cfg.weekDay&=~4;
	}
	else if (dataReqHttp.key==ID_D4)
	{
		if (dataReqHttp.data[0]=='1')	cfg.weekDay|=8;
		else							cfg.weekDay&=~8;
	}
	else if (dataReqHttp.key==ID_D5)
	{
		if (dataReqHttp.data[0]=='1')	cfg.weekDay|=16;
		else							cfg.weekDay&=~16;
	}
	else if (dataReqHttp.key==ID_D6)
	{
		if (dataReqHttp.data[0]=='1')	cfg.weekDay|=32;
		else							cfg.weekDay&=~32;
	}
	else if (dataReqHttp.key==ID_D7)
	{
		if (dataReqHttp.data[0]=='1')	cfg.weekDay|=64;
		else							cfg.weekDay&=~64;
	}
	else if (dataReqHttp.key==ID_TASK)
	{
		taskFile=dataReqHttp.data[0]&0x0F;
	}
	else if (dataReqHttp.key==ID_REPORTTIME)
	{
		sscanf ((const char *)dataReqHttp.data, "%d:%d", &s[0],&s[1]);
		if (s[0]<24) cfg.timeHourRep = s[0];
		if (s[1]<60) cfg.timeMinRep = s[1];
	}
	else if (dataReqHttp.key==ID_REPORTCSV)
	{
		if (dataReqHttp.data[0]=='1')	cfg.saveReport=1;
		else							cfg.saveReport=0;
	}
	else if (dataReqHttp.key==ID_MIGMODE)
	{
		s[0]=dataReqHttp.data[0]&0x0F;
		if (s[0]==_MODESERVER || s[0]==_MODECLIENT)
		{
			if (cfg.runMode!=s[0])
			{
				cfg.runMode=s[0];
				if (cfg.runMode==_MODESERVER)
				{
					cfg.ipServer[0] = 0;
					cfg.ipServer[1] = 0;
					cfg.ipServer[2] = 0;
					cfg.ipServer[3] = 0;
				}
			}
		}
	}
	else if (dataReqHttp.key==ID_MIGIP)
	{
		if (cfg.runMode==_MODECLIENT)
		{
	   	 	sscanf ((const char *)dataReqHttp.data, "%d", &s[0]);
			cfg.ipServer[0] = (s[0]&0xFF000000);
			cfg.ipServer[1] |= (s[0]&0x00FF0000);
			cfg.ipServer[2] |= (s[0]&0x0000FF00);
			cfg.ipServer[3] |= (s[0]&0x000000FF);
		}
	}
	else if (dataReqHttp.key==ID_LINE)
	{
		if (dataReqHttp.data[0]=='1')	cfg.broadcastInfo=1;
		else							cfg.broadcastInfo=0;
	}		   
	else if (dataReqHttp.key==ID_INTERF)
	{
		sscanf ((const char *)dataReqHttp.data, "%d", &s[0]);
		if (s[0]==ITF_UDP || s[0]==ITF_WIFI)
		{
			if (cfg.srvItf!=s[0])
			{
				cfg.srvItf=s[0];
			}
		}
	}
	// SELDEP.HTML
	else if (dataReqHttp.key==ID_DNUM)
	{
		DEP=dataReqHttp.data[0]&0x0F;
	}
	// DEP.HTML
	else if (dataReqHttp.key==ID_DENAB)
	{
		if (dataReqHttp.data[0]=='1')	cfg.depEnable[DEP-1]=1;
		else							cfg.depEnable[DEP-1]=0;
	}
	else if (dataReqHttp.key==ID_DNAME)
	{
		while (var<dataReqHttp.len)
		{
			cfg.depName[DEP-1][var]=dataReqHttp.data[var];
			var++;
		}
		cfg.depName[DEP-1][var]=0;
	}
	else if (dataReqHttp.key==ID_DALPH)
	{
		cfg.depAlpha[DEP-1]=dataReqHttp.data[0];
	}
	else if (dataReqHttp.key==ID_DRANG1)
	{
		sscanf ((const char *)dataReqHttp.data, "%d",&s[0]);
		if ((s[0]>0)&&(s[0]<=9998))	
			cfg.depRange[DEP-1][0] = s[0];
	}
	else if (dataReqHttp.key==ID_DRANG2)
	{
		sscanf ((const char *)dataReqHttp.data, "%d",&s[0]);
		if ((s[0]>0)&&(s[0]<=9999))	
			if (s[0]>cfg.depRange[DEP-1][0])
				cfg.depRange[DEP-1][1]=s[0];
			else
				cfg.depRange[DEP-1][1]=cfg.depRange[DEP-1][0]+1;

		if ((tktNumber[DEP-1]<cfg.depRange[DEP-1][0]) ||// Número do ticket ficou fora do range ?
			(tktNumber[DEP-1]>cfg.depRange[DEP-1][1]))	// então ajusta número do ticket
			tktNumber[DEP-1] = cfg.depRange[DEP-1][0];
	}
	else if (dataReqHttp.key==ID_DCLOS1)
	{
		if (dataReqHttp.data[0]=='1')	cfg.depCloseNext[DEP-1]=1;
		else							cfg.depCloseNext[DEP-1]=0;

	}
	else if (dataReqHttp.key==ID_DCLOS2)
	{
		if (dataReqHttp.data[0]=='1')	cfg.depCloseRep[DEP-1]=1;
		else							cfg.depCloseRep[DEP-1]=0;
	}
	else if (dataReqHttp.key==ID_DCALL1)
	{
		sscanf ((const char *)dataReqHttp.data, "%d",&s[0]);
		if ((s[0]&0xF)!=((cfg.depSchPri[DEP-1]&0xF0)>>4))// valor do esquema foi modificado ?
			contSchPri[DEP-1] = 0;						// então zera contador
		cfg.depSchPri[DEP-1]=((s[0]&0xF)<<4);
	}
	else if (dataReqHttp.key==ID_DCALL2)
	{
		sscanf ((const char *)dataReqHttp.data, "%d",&s[0]);
		if ((s[0]&0xF)!=(cfg.depSchPri[DEP-1]&0xF))		// valor do esquema foi modificado ?
			contSchPri[DEP-1] = 0;						// então zera contador
		cfg.depSchPri[DEP-1]|=(s[0]&0xF);
	}
	else if (dataReqHttp.key==ID_DHLP1)
	{
		sscanf ((const char *)dataReqHttp.data, "%d",&s[0]);
		if (s[0]!=DEP)
			cfg.depHelp[DEP-1]=((s[0]&0xF)<<4);
	}
	else if (dataReqHttp.key==ID_DHLP2)
	{
		sscanf ((const char *)dataReqHttp.data, "%d",&s[0]);
		if (s[0]!=DEP)
			cfg.depHelp[DEP-1]|=(s[0]&0xF);
	}
	else if (dataReqHttp.key==ID_DHLP3)
	{
		cfg.depHelp[DEP-1]=0xF0;
	}
	else if (dataReqHttp.key==ID_DHLP4)
	{
		sscanf ((const char *)dataReqHttp.data, "%d",&s[0]);
		if ((s[0]&0xF)!=((cfg.depSchHlp[DEP-1]&0xF0)>>4))// valor do esquema foi modificado ?
			contSchHlp[DEP-1] = 0;						// então zera contador
		cfg.depSchHlp[DEP-1]=((s[0]&0xF)<<4);
	}
	else if (dataReqHttp.key==ID_DHLP5)
	{
		sscanf ((const char *)dataReqHttp.data, "%d",&s[0]);
		if ((s[0]&0xF)!=(cfg.depSchHlp[DEP-1]&0xF))		// valor do esquema foi modificado ?
			contSchHlp[DEP-1] = 0;						// então zera contador
		cfg.depSchHlp[DEP-1]|=(s[0]&0xF);
	}
	else if (dataReqHttp.key==ID_DIPD)
	{
    	sscanf ((const char *)dataReqHttp.data, "%d.%d.%d.%d",&s[0],&s[1],&s[2],&s[3]);
        cfg.depIpDisp[DEP-1][0] = s[0];
        cfg.depIpDisp[DEP-1][1] = s[1];
        cfg.depIpDisp[DEP-1][2] = s[2];
        cfg.depIpDisp[DEP-1][3] = s[3];
	}
	else if (dataReqHttp.key==ID_DITFD)
	{
		sscanf ((const char *)dataReqHttp.data, "%d",&s[0]);
		if (s[0]==ITF_UDP || s[0]==ITF_WIFI)
			cfg.depItfDisp[DEP-1]=s[0];
	}
	else if (dataReqHttp.key==ID_DIPT)
	{
        sscanf ((const char *)dataReqHttp.data, "%d.%d.%d.%d",&s[0],&s[1],&s[2],&s[3]);
        cfg.depIpTop[DEP-1][0] = s[0];
      	cfg.depIpTop[DEP-1][1] = s[1];
      	cfg.depIpTop[DEP-1][2] = s[2];
      	cfg.depIpTop[DEP-1][3] = s[3];
	}
	else if (dataReqHttp.key==ID_DITFT)
	{
		sscanf ((const char *)dataReqHttp.data, "%d",&s[0]);
		if (s[0]==ITF_UDP || s[0]==ITF_WIFI)
			cfg.depItfTop[DEP-1]=s[0];
	}
	else if (dataReqHttp.key==ID_ENATOP)
	{
		cfg.eTop[DEP-1]=1;
	}
	else if (dataReqHttp.key==ID_DLIND)
	{
		sscanf ((const char *)dataReqHttp.data, "%d",&s[0]);
		if (s[0]>=1 && s[0]<=10)
			cfg.depRowDisp[DEP-1]=s[0];
	}
	else if (dataReqHttp.key==ID_DPRFD)
	{
		sscanf ((const char *)dataReqHttp.data, "%d",&s[0]);
		if (s[0]!=DEP)
			cfg.depIsBtPref[DEP-1]=s[0];
	}
	else if (dataReqHttp.key>=ID_DU01 && dataReqHttp.key<=ID_DU99)
	{
		var = dataReqHttp.key-ID_DU01;
		if (dataReqHttp.data[0]=='1')	cfg.depUsers[var]=1;
		else							cfg.depUsers[var]=0;
	}
	// USERBOX.HTML
	else if (dataReqHttp.key==ID_USER)
	{
		sscanf ((const char *)dataReqHttp.data, "%d",&s[0]);
		USER=s[0];
	}
	else if (dataReqHttp.key==ID_BOX)
	{
		sscanf ((const char *)dataReqHttp.data, "%d",&s[0]);
		cfg.userBox[USER-1]=s[0];
	}
	// TICKET.HTML
	else if (dataReqHttp.key==ID_LG)
	{
		if (dataReqHttp.data[0]=='1')	cfg.logoEnable=1;
		else							cfg.logoEnable=0;
	}
	else if (dataReqHttp.key==ID_DLEN)
	{
		if (dataReqHttp.data[0]=='1')	cfg.lettEnable=1;
		else							cfg.lettEnable=0;
	}
	else if (dataReqHttp.key==ID_DLNUM)
	{
		cfg.lenNumTicket = dataReqHttp.data[0]&0x0F;
	}
	else if (dataReqHttp.key==ID_DHNUM)
	{
		cfg.typeNumTicket = dataReqHttp.data[0]&0x0F;
	}
	else if (dataReqHttp.key==ID_CUT)
	{
		cfg.cut = dataReqHttp.data[0]&0x0F;
	}
	else if (dataReqHttp.key==ID_VIA)
	{
		cfg.copy = dataReqHttp.data[0]&0x0F;
	}
	else if (dataReqHttp.key==ID_H1OP)
	{
		cfg.optHead1 = dataReqHttp.data[0]&0x0F;
	}
	else if (dataReqHttp.key==ID_H2OP)
	{
		cfg.optHead2 = dataReqHttp.data[0]&0x0F;
	}
	else if (dataReqHttp.key==ID_H3OP)
	{
		cfg.optHead2 = dataReqHttp.data[0]&0x0F;
	}
	else if (dataReqHttp.key==ID_F1OP)
	{
		cfg.optFoot1 = dataReqHttp.data[0]&0x0F;
	}
	else if (dataReqHttp.key==ID_F2OP)
	{
		cfg.optFoot2 = dataReqHttp.data[0]&0x0F;
	}
	else if (dataReqHttp.key==ID_F3OP)
	{
		cfg.optFoot3 = dataReqHttp.data[0]&0x0F;
	}
	else if (dataReqHttp.key==ID_F4OP)
	{
		cfg.optFoot4 = dataReqHttp.data[0]&0x0F;
	}
	else if (dataReqHttp.key==ID_H1ME)
	{
		while (var<dataReqHttp.len)
		{
			cfg.header1[var]=dataReqHttp.data[var];
			var++;
		}
		cfg.header1[var]=0;
	}
	else if (dataReqHttp.key==ID_H2ME)
	{
		while (var<dataReqHttp.len)
		{
			cfg.header2[var]=dataReqHttp.data[var];
			var++;
		}
		cfg.header2[var]=0;
	}
	else if (dataReqHttp.key==ID_H3ME)
	{
		while (var<dataReqHttp.len)
		{
			cfg.header3[var]=dataReqHttp.data[var];
			var++;
		}
		cfg.header3[var]=0;
	}
	else if (dataReqHttp.key==ID_F1ME)
	{
		while (var<dataReqHttp.len)
		{
			cfg.footer1[var]=dataReqHttp.data[var];
			var++;
		}
		cfg.footer1[var]=0;
	}
	else if (dataReqHttp.key==ID_F2ME)
	{
		while (var<dataReqHttp.len)
		{
			cfg.footer2[var]=dataReqHttp.data[var];
			var++;
		}
		cfg.footer2[var]=0;
	}
	else if (dataReqHttp.key==ID_F3ME)
	{
		while (var<dataReqHttp.len)
		{
			cfg.footer3[var]=dataReqHttp.data[var];
			var++;
		}
		cfg.footer3[var]=0;
	}
	else if (dataReqHttp.key==ID_F4ME)
	{
		while (var<dataReqHttp.len)
		{
			cfg.footer4[var]=dataReqHttp.data[var];
			var++;
		}
		cfg.footer4[var]=0;
	}
	// VER.HTML
	else if (dataReqHttp.key==ID_AUTH)
	{
		if (dataReqHttp.data[0]=='1')	cfg.authEnable=1;
		else							cfg.authEnable=0;
	}
	else if (dataReqHttp.key==ID_PW)
	{
		while (var<dataReqHttp.len)
		{
			cfg.authPassword[var]=dataReqHttp.data[var];
			var++;
		}
		cfg.authPassword[var]=0;
	}
	// FORMAT.HTML
	else if (dataReqHttp.key==ID_FORMAT)
	{
		if (dataReqHttp.data[0]=='1')
			Format_Drive ();
	}
	else if (dataReqHttp.key==ID_INIT)
	{
		if (dataReqHttp.data[0]=='1')
			Format_DataFlash ();
	}
}

/*----------------------------------------------------------------------------
 * Função:		api_handle_reboot_request
 * Descrição:   Monta primeira resposta http a requisição de boot
 * Parâmetros: 	Número da requisição
 * Retorno: 	Nenhum
 ----------------------------------------------------------------------------*/
void api_handle_reboot_request(unsigned int numReq)
{
	api_req_handled=1;									// Mark the request as handled

	dataReqHttp.reqPrev=numReq;							// Guarda requisição para uso posterior
	dataReqHttp.reqItem=0;								// Inicializa item da requisição
	dataReqHttp.reqEnd=0;								// Indica que ainda não terminou
	dataReqHttp.len=0;									// Inicializa resposta http posterior
	
	api_add_http_headers('j');							// HTTP headers "application/JSON"

	api_add_json_object_start();						// "{" = JSON status object start 

	api_add_settings_item(ID_BOOT,						// Parâmetro: boot: "0"=não "1"=sim"
						PAR_BOOT_LEN,
						(char *)PAR_BOOT);
	api_dec ();											// Retira último caractere: "Vírgula"

	api_add_json_object_end();							// "}" = JSON status object end 
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_boot_request_data
 * Descrição: 	Monta respostas http complementares a requisição "boot" ao final 
 * 				seta flags para o BOOT.
 * Parâmetros: 	Nenhum
 * Retorno: 	(0) fim (1) mais um bloco
 -----------------------------------------------------------------------------*/
unsigned char api_handle_boot_request_data (void)
{
//	if (dataReqHttp.reqPrev!=0 && 						// Tem requisição pendente e
//		dataReqHttp.reqEnd==0)							// requisição tem frames para enviar
//	{
//		.....dados......
//		dataReqHttp.reqEnd=1;							// Indica que é o último frame..
//	}
//	else												
//	{													// Ja enviou todos os frames..
//		dataReqHttp.reqEnd=0;							// Inicializa flag para próxima req.
//		return 0;										// Retorna indicando que pode encerrar
//	}

	WEBREQUEST=UPD_FIRM;								// Força reinicialização do MIG
	cfg.update=_FROMAPP;								// para a área do bootloader
	Write_Flash ();
	return 0;
}

/*----------------------------------------------------------------------------
 * Função:		api_handle_status_request
 * Descrição:   Monta resposta http para a requisição de status
 ----------------------------------------------------------------------------*/
void api_handle_status_request(void)
{
	api_req_handled=1;									// Mark the request as handled

	api_add_http_headers('j');							// HTTP headers "application/JSON"

	api_add_json_object_start();						// "{" = JSON status object start 

	api_add_settings_item(ID_BOOT,						// Parâmetro: boot: "0"=não "1"=sim"
						PAR_BOOT_LEN,
						(char *)PAR_BOOT);

	api_add_settings_item(ID_MODE,						// Parâmetro: boot: "0"=não "1"=sim"
						PAR_MODE_LEN,
						(char *)PAR_MODE);

	api_add_json_object_name(PAR_SET_LEN,				// Parâmetro: "Object Settings"
						(char *)PAR_SET);				// ":"

	api_add_json_object_start();						// "{" = JSON settings value object start 

	api_add_settings_item(ID_SSID, 						// SSID
						PAR_SSID_LEN, 
						(char *)PAR_SSID);

	api_add_settings_item(ID_BSSID, 					// BSSID
						PAR_BSSID_LEN, 
						(char *)PAR_BSSID);

	api_add_settings_item(ID_CHANNEL, 					// Channel
						PAR_CHANNEL_LEN, 
						(char *)PAR_CHANNEL);

	api_add_settings_item(ID_SECURITY, 					// Security
						PAR_SECURITY_LEN, 
						(char *)PAR_SECURITY);
	api_dec ();											// Retira último caractere: "Vírgula"

	api_add_json_object_end();							// "}" = JSON settings value object end
	api_add_json_object_end();							// "}" = JSON status object end
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_networks_request
 * Descrição: 	Monta primeira resposta http para a requisição "network"
 * Parâmetros: 	Número da requisição
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_handle_networks_request (unsigned int numReq)
{
	api_req_handled=1;									// Mark the request as handled
	
	dataReqHttp.reqPrev=numReq;							// Guarda requisição para uso posterior
	dataReqHttp.reqItem=0;								// Inicializa item da requisição
	dataReqHttp.reqEnd=0;								// Indica que ainda não terminou
	dataReqHttp.len=0;									// Inicializa resposta http posterior

	api_add_http_headers('j');							// HTTP headers "application/JSON"

	api_add_json_object_start();						// JSON networks object start "{"

	api_add_json_object_name(PAR_NETWORKS_LEN,			// Parâmetro: "Nome de redes"
						(char *)PAR_NETWORKS);			// ":"

	api_add_json_array_start();							// JSON networks value array start "["
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_networks_request_data
 * Descrição: 	Monta respostas http complementares para a requisição "network" envia resultados 
 *				envia resultados dos nomes das redes AP.
 * Parâmetros: 	Nenhum
 * Retorno: 	(0) fim (1) mais um bloco
 -----------------------------------------------------------------------------*/
unsigned char api_handle_networks_request_data (void)
{
	unsigned char len=0;

	if (dataReqHttp.reqPrev!=0 && 						// Tem requisição pendente e
		dataReqHttp.reqEnd==0)							// requisição tem frames para enviar
	{
		while (ListWifi[dataReqHttp.reqItem][len]!=0 && // Verifica se é fim do nome da rede AP
				len<32)									// Comprimento deve ser menor que 32
			len++;										// Verifica próximo caractere 

		if (ListWifi[dataReqHttp.reqItem][0]!='\x0') 
		{
			api_resp_len=0;								// Inicializa resposta http
			if (dataReqHttp.reqItem!=0) api_add_comma();// Virgula se for o segundo item
			api_add_json_quoted_string(len,ListWifi[dataReqHttp.reqItem]); // Nome do item
			dataReqHttp.reqItem++;						// Incrementa item 
		}
		else
		{
			dataReqHttp.reqEnd=1;						// Indica que é o último frame..
			api_resp_len=0;								// Inicializa resposta http
			api_add_json_array_end();					// JSON networks value array end
			api_add_json_object_end();					// JSON networks object end
		}
		return 1;
	}
	else
	{
		dataReqHttp.reqEnd=0;							// Inicializa flag para próxima req.
		return 0;										// Retorna indicando que pode encerrar
	}
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_dir_request
 * Descrição: 	Monta primeira resposta http para a requisição "dir"
 * Parâmetros: 	Número da requisição
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_handle_dir_request (unsigned int numReq)
{
	api_req_handled=1;									// Mark the request as handled

	dataReqHttp.reqPrev=numReq;							// Guarda requisição para uso posterior
	dataReqHttp.reqItem=0;								// Inicializa item da requisição
	dataReqHttp.reqEnd=0;								// Indica que ainda não terminou
	dataReqHttp.len=0;									// Inicializa resposta http posterior

	api_add_http_headers('j');							// HTTP headers "application/JSON"

	api_add_json_object_start();						// JSON dir object start "{"

	api_add_json_object_name(PAR_DIR_LEN,				// Parâmetro: "Nome dos diretórios"
						(char *)PAR_DIR);
	
	api_add_json_array_start();							// JSON dir value array start "["

	info.fileID = 0;
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_dir_request_data
 * Descrição: 	Monta respostas http complementares para a requisição "dir" 
 *				envia resultados dos nomes diretórios e arquivos.
 * Parâmetros: 	Nenhum
 * Retorno: 	(0) fim (1) mais um bloco
 -----------------------------------------------------------------------------*/
unsigned char api_handle_dir_request_data (void)
{
	unsigned char len=0;

	if (dataReqHttp.reqPrev!=0 && 						// Tem requisição pendente e
		dataReqHttp.reqEnd==0)							// requisição tem frames para enviar
	{



						   // RASCUNHO DE "CGI"
//			char temp[32]={0,0,0,0,0,0,0,0,0,0,0,0,0};
//			filesCount++;
//	      	if (ffind ("*.*",&info) == 0) 
//			{
//				dot_format (info.size, temp);
//	        	len += sprintf ((char *)data,"<tr align=center><td>%d.</td>"
//	           		                    	"<td align=left><a href=\"/%s\">%s</a></td>"
//	                   		            	"<td align=right>%s</td>"
//	                           		    	"</tr>\r\n",
//	                   						filesCount,info.name,info.name,temp);
//      			fsize += info.size;
//			}
//			else
//			{
//				dot_format (fsize, temp);
//        		len += sprintf ((char *)data,"<tr align=center><td></td>"
//											"<th align=right>Total:</th>"
//											"<th align=right>%s</th></tr>"
//											"\r\n",temp);
//			}

		if (ffind ("*.*",&info) == 0)
		{
			api_resp_len=0;								// Inicializa resposta http
			if (dataReqHttp.reqItem!=0) api_add_comma();// Virgula se for o segundo item
			
			len=0;
			while (info.name[len]!=0)
				len++;

			api_add_json_quoted_string(len,(char *)info.name);// Nome do item
			dataReqHttp.reqItem++;						// Incrementa item 
		}
		else
		{
			dataReqHttp.reqEnd=1;						// Indica que é o último frame..
			api_resp_len=0;								// Inicializa resposta http
			api_add_json_array_end();					// JSON networks value array end
			api_add_json_object_end();					// JSON networks object end
		}
		return 1;
	}
	else
	{
		dataReqHttp.reqEnd=0;							// Inicializa flag para próxima req.
		return 0;										// Retorna indicando que pode encerrar
	}
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_download_request
 * Descrição: 	Monta primeira resposta http para a requisição "download"
 * Parâmetros: 	numReq - Número da requisição
 *				*fileName - Nome do arquivo requisitado
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_handle_download_request (unsigned int numReq, char *fileName)
{
	api_req_handled=1;									// Marca como requisição tratada.

	dataReqHttp.reqPrev=numReq;							// Guarda requisição para uso posterior
	dataReqHttp.reqEnd=0;								// Indica que ainda não terminou
	dataReqHttp.len=0;									// Inicializa resposta http posterior

	api_add_http_headers('o');							// HTTP headers "application/octet-stream"

	f = fopen ((const char *)fileName, "r");
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_download_request_data
 * Descrição: 	Monta respostas http complementares para a requisição "download" 
 *				envia dados do arquivo.
 * Parâmetros: 	Nenhum
 * Retorno: 	(0) fim (1) mais um bloco
 -----------------------------------------------------------------------------*/
unsigned char api_handle_download_request_data (void)
{
	if (dataReqHttp.reqPrev!=0 && 						// Tem requisição pendente e
		dataReqHttp.reqEnd==0)							// requisição tem frames para enviar
	{
		api_resp_len=0;									// Inicializa resposta http
		api_resp_len=fread (api_resp, 1, 200, f);		// Lê do arquivo até 200 bytes por vez
		
	
		if (api_resp_len==200)
		{
														// Leu 200 bytes do arquivo..		
		}
		else
		{
			fclose (f);
			dataReqHttp.reqEnd=1;						// Indica que é o último frame..
		}

		return 1;
	}
	else
	{
		dataReqHttp.reqEnd=0;							// Inicializa flag para próxima req.
		return 0;										// Retorna indicando que pode encerrar
	}
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_ap_req_part1
 * Descrição: 	Monta resposta a requisição "parametros AP", com a primeira
 *				parte dos parâmetros de configuração WIFI no modo AP
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_handle_ap_req_part1(void)
{
	api_req_handled=1;									// Marca como requisição tratada.

	api_add_http_headers('j');							// HTTP headers "application/JSON"
	api_add_json_object_start();						// JSON settings object start

	api_add_settings_item(ID_BOOT,						// Parâmetro: boot: "0"=não "1"=sim"
						PAR_BOOT_LEN,
						(char *)PAR_BOOT);

	api_add_json_object_name(PAR_SET_LEN,				// Settings name 
						(char *)PAR_SET);			
	
	api_add_json_object_start();						// JSON settings value object start

	api_add_settings_item(ID_SSID, 						// SSID
						PAR_SSID_LEN, 
						(char *)PAR_SSID);

	api_add_settings_item(ID_CHANNEL, 					// Channel
						PAR_CHANNEL_LEN, 
						(char *)PAR_CHANNEL);
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_ap_req_part2
 * Descrição: 	Monta resposta a requisição "parametros AP", com a segunda
 *				parte dos parâmetros de configuração WIFI no modo AP
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_handle_ap_req_part2(void)
{
	api_req_handled=1;									// Marca como requisição tratada.
	api_resp_len=0;										// Inicializa resposta http

	api_add_settings_item(ID_SECURITY, 					// Security
						PAR_SECURITY_LEN, 
						(char *)PAR_SECURITY);

	api_add_settings_item(ID_WIFIPW, 						// Password
						PAR_WIFIPW_LEN, 
						(char *)PAR_WIFIPW);

	api_add_settings_item(ID_IP,						// IP address
						PAR_LOCALIP_LEN, 
						(char *)PAR_LOCALIP);
	
	api_add_settings_item(ID_MASK,						// IP netmask
						PAR_MASK_LEN, 
						(char *)PAR_MASK);
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_ap_req_part3
 * Descrição: 	Monta resposta a requisição "parametros AP", com a terceira
 *				parte dos parâmetros de configuração WIFI no modo AP
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_handle_ap_req_part3(void)
{
	api_req_handled=1;									// Marca como requisição tratada.
	api_resp_len=0;										// Inicializa resposta http

	api_add_settings_item(ID_DHCPS,						// DHCP enabled
						PAR_DHCPS_LEN, 
						(char *)PAR_DHCPS);
	
	api_add_settings_item(ID_DHCPS_IP, 					// DHCP range
						PAR_DHCPS_IP_LEN, 
						(char *)PAR_DHCPS_IP);
	api_dec ();											// Retira último caractere: "Vírgula"

	api_add_json_object_end();							// JSON settings value object end
	api_add_json_object_end();							// JSON settings object end
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_sta_req_part1
 * Descrição: 	Monta resposta a requisição "parametros STA", com a primeira
 *				parte dos parâmetros de configuração WIFI no modo STA
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_handle_sta_req_part1(void)
{
	api_req_handled=1;									// Marca como requisição tratada.
	api_add_http_headers('j');							// HTTP headers "application/JSON"
	api_add_json_object_start();						// JSON settings object start

	api_add_settings_item(ID_BOOT,						// Parâmetro: boot: "0"=não "1"=sim"
						PAR_BOOT_LEN,
						(char *)PAR_BOOT);

	api_add_json_object_name(PAR_SET_LEN,				// Settings name
						(char *)PAR_SET);	

	api_add_json_object_start();						// JSON settings value object start

	api_add_settings_item(ID_SSID, 						// SSID
						PAR_SSID_LEN, 
						(char *)PAR_SSID);
	
	api_add_settings_item(ID_WIFIPW, 						// Password name
						PAR_WIFIPW_LEN, 
						(char *)PAR_WIFIPW);

	api_add_settings_item(ID_DHCP,						// DHCP enabled
						PAR_DHCP_LEN, 
						(char *)PAR_DHCP);
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_sta_req_part2
 * Descrição: 	Monta resposta a requisição "parametros STA", com a segunda
 *				parte dos parâmetros de configuração WIFI no modo STA
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_handle_sta_req_part2(void)
{
	api_req_handled=1;									// Marca como requisição tratada.
	api_resp_len=0;										// Inicializa resposta http

	api_add_settings_item(ID_IP,						// IP address
						PAR_LOCALIP_LEN, 
						(char *)PAR_LOCALIP);
	
	api_add_settings_item(ID_MASK,						// IP netmask
						PAR_MASK_LEN, 
						(char *)PAR_MASK);
	
	api_add_settings_item(ID_GW, 						// IP gateway
						PAR_GATEWAY_LEN, 
						(char *)PAR_GATEWAY);
	
	api_add_settings_item(ID_DNS1,						// DNS 1 address
						PAR_DNS1_LEN, 
						(char *)PAR_DNS1);
	
	api_add_settings_item(ID_DNS2,						// DNS 2 address
						PAR_DNS2_LEN, 
						(char *)PAR_DNS2);

	api_dec();											// Retira último caractere: "Vírgula"

	api_add_json_object_end();							// JSON settings value object end  
	api_add_json_object_end();							// JSON settings object end
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_gen_req_part1
 * Descrição: 	Monta resposta a requisição "parametros gerais", com a primeira
 *				parte dos parâmetros gerais do produto
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_handle_gen_req_part1(void)
{
	api_req_handled=1;									// Marca como requisição tratada.
	api_add_http_headers('j');							// HTTP headers "application/JSON"
	api_add_json_object_start();						// JSON settings object start

	api_add_settings_item(ID_BOOT,						// Parâmetro: boot: "0"=não "1"=sim"
						PAR_BOOT_LEN,
						(char *)PAR_BOOT);

	api_add_json_object_name(PAR_SET_LEN,				// Settings name
						(char *)PAR_SET);

	api_add_json_object_start();						// JSON settings value object start

	api_add_settings_item(ID_DRIVE,						// Parâmetro: "Drive em uso"
						PAR_DRIVE_LEN,
						(char *)PAR_DRIVE);

	api_add_settings_item(ID_DATE,						// Parâmetro: "Data"
						PAR_DATE_LEN,
						(char *)PAR_DATE);

	api_add_settings_item(ID_TIME,						// Parâmetro: "Hora"
						PAR_TIME_LEN,
						(char *)PAR_TIME);

	api_add_settings_item(ID_TICKETTEST,				// Parâmetro: "Teste de ticket"
						PAR_TICKETTEST_LEN,
						(char *)PAR_TICKETTEST);

	api_add_settings_item(ID_TIMELIVE,					// Parâmetro: "Tempo de vida do ticket"
						PAR_TIMELIVE_LEN,
						(char *)PAR_TIMELIVE);

	api_add_settings_item(ID_CONTROL,					// Parâmetro: "Controle de horário"
						PAR_CONTROL_LEN,
						(char *)PAR_CONTROL);

	api_add_settings_item(ID_TIMEON,					// Parâmetro: "Hora de ligar"
						PAR_TIMEON_LEN,
						(char *)PAR_TIMEON);

	api_add_settings_item(ID_TIMEOFF,					// Parâmetro: "Hora de desligar"
						PAR_TIMEOFF_LEN,
						(char *)PAR_TIMEOFF);
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_gen_req_part2
 * Descrição: 	Monta resposta a requisição "parametros gerais", com a segunda
 *				parte dos parâmetros gerais do produto
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_handle_gen_req_part2(void)
{
	api_req_handled=1;									// Marca como requisição tratada.
	api_resp_len=0;										// Inicializa resposta http

	api_add_settings_item(ID_REPORTTIME,				// Parâmetro: "Hora do relatório"
						PAR_REPORTTIME_LEN,
						(char *)PAR_REPORTTIME);

	api_add_settings_item(ID_REPORTCSV,					// Parâmetro: "Status para gerar arquivo CSV"
						PAR_REPORTCSV_LEN,
						(char *)PAR_REPORTCSV);

	api_add_settings_item(ID_MIGMODE,					// Parâmetro: "Modo do MIG (Client/Server)"
						PAR_MIGMODE_LEN,
						(char *)PAR_MIGMODE);

	api_add_settings_item(ID_MIGIP,						// Parâmetro: "IP do Mig"
						PAR_MIGIP_LEN,
						(char *)PAR_MIGIP);

	api_add_settings_item(ID_LINE,						// Parâmetro: "status das Informações de fila"
						PAR_LINE_LEN,
						(char *)PAR_LINE);

	api_add_settings_item(ID_INTERF,					// Parâmetro: "Interface de envio (Lan/Wifi)"
						PAR_INTERF_LEN,
						(char *)PAR_INTERF);

	api_add_settings_item(ID_D1,						// Habilitar no Domingo
						PAR_D1_LEN, 
						(char *)PAR_D1);

	api_add_settings_item(ID_D2,						// Habilitar na Segundaça-feira
						PAR_D2_LEN, 
						(char *)PAR_D2);

	api_add_settings_item(ID_D3,						// Habilitar na Terca-feira
						PAR_D3_LEN, 
						(char *)PAR_D3);

	api_add_settings_item(ID_D4,						// Habilitar na Quarta-feira
						PAR_D4_LEN, 
						(char *)PAR_D4);

	api_add_settings_item(ID_D5,						// Habilitar na Quinta-feira
						PAR_D5_LEN, 
						(char *)PAR_D5);

	api_add_settings_item(ID_D6,						// Habilitar na Sexta-feira
						PAR_D6_LEN, 
						(char *)PAR_D6);

	api_add_settings_item(ID_D7,						// Habilitar no Sabado
						PAR_D7_LEN, 
						(char *)PAR_D7);

	api_dec();											// Decrementa último caractere: "Vírgula"

	api_add_json_object_end();							// JSON settings value object end  
	api_add_json_object_end();							// JSON settings object end
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_task_request
 * Descrição: 	Monta resposta a requisição "task file"
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_handle_task_request(void)
{
	api_req_handled=1;									// Marca como requisição tratada.
	api_add_http_headers('j');							// HTTP headers "application/JSON"
	api_add_json_object_start();						// JSON settings object start

	api_add_settings_item(ID_BOOT,						// Parâmetro: boot: "0"=não "1"=sim"
						PAR_BOOT_LEN,
						(char *)PAR_BOOT);

	api_add_settings_item(ID_TASK,						// Parâmetro: task: "Tarefa de 1 a 3"
						PAR_TASK_LEN,
						(char *)PAR_TASK);

	api_dec();											// Decrementa último caractere: "Vírgula"

	api_add_json_object_end();							// JSON settings value object end  
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_seldep_req_part1
 * Descrição: 	Monta resposta a requisição "department select", com a primeira 
 *				parte das opções de nomes dos departamentos
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_handle_seldep_req_part1(void)
{
	api_req_handled=1;									// Marca como requisição tratada.
	api_add_http_headers('j');							// HTTP headers "application/JSON"
	api_add_json_object_start();						// JSON settings object start

	api_add_settings_item(ID_BOOT,						// Parâmetro: boot: "0"=não "1"=sim"
						PAR_BOOT_LEN,
						(char *)PAR_BOOT);

	api_add_json_object_name(PAR_SET_LEN,				// Settings name
						(char *)PAR_SET);

	api_add_json_object_start();						// JSON settings value object start

	api_add_settings_item(ID_DNUM,						// Parâmetro: Número do dpto
						PAR_DNUM_LEN,
						(char *)PAR_DNUM);

	api_add_settings_item(ID_DNAME1,					// Parâmetro: "Nome do dpto 1"
						PAR_DNAME1_LEN,
						(char *)PAR_DNAME1);

	api_add_settings_item(ID_DNAME2,					// Parâmetro: "Nome do dpto 2"
						PAR_DNAME2_LEN,
						(char *)PAR_DNAME2);
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_seldep_req_part2
 * Descrição: 	Monta resposta a requisição "department select", com a segunda 
 *				parte das opções de nomes dos departamentos
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_handle_seldep_req_part2(void)
{
	api_req_handled=1;									// Marca como requisição tratada.
	api_resp_len=0;										// Inicializa resposta http
	api_add_settings_item(ID_DNAME3,					// Parâmetro: "Nome do dpto 3"
						PAR_DNAME3_LEN,
						(char *)PAR_DNAME3);

	api_add_settings_item(ID_DNAME4,					// Parâmetro: "Nome do dpto 4"
						PAR_DNAME4_LEN,
						(char *)PAR_DNAME4);

	api_add_settings_item(ID_DNAME5,					// Parâmetro: "Nome do dpto 5"
						PAR_DNAME5_LEN,
						(char *)PAR_DNAME5);

	api_add_settings_item(ID_DNAME6,					// Parâmetro: "Nome do dpto 6"
						PAR_DNAME6_LEN,
						(char *)PAR_DNAME6);
	api_dec();											// Decrementa último caractere: "Vírgula"

	api_add_json_object_end();							// JSON settings value object end  
	api_add_json_object_end();							// JSON settings object end
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_dep_req_part1
 * Descrição: 	Monta resposta a requisição "department", com a primeira parte
 *				dos parâmetros do departamento
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_handle_dep_req_part1(void)
{
	api_req_handled=1;									// Marca como requisição tratada.
	api_add_http_headers('j');							// HTTP headers "application/JSON"
	api_add_json_object_start();						// JSON settings object start

	api_add_settings_item(ID_BOOT,						// Parâmetro: boot: "0"=não "1"=sim"
						PAR_BOOT_LEN,
						(char *)PAR_BOOT);

	api_add_json_object_name(PAR_SET_LEN,				// Settings name
						(char *)PAR_SET);

	api_add_json_object_start();						// JSON settings value object start

	api_add_settings_item(ID_DENAB,						// Parâmetro: "Habilita departamento"
						PAR_DENAB_LEN,
						(char *)PAR_DENAB);

	api_add_settings_item(ID_DNAME,						// Parâmetro: "Nome do departamento"
						PAR_DNAME_LEN,
						(char *)PAR_DNAME);

	api_add_settings_item(ID_DALPH,						// Parâmetro: "Letra do departamento"
						PAR_DALPH_LEN,
						(char *)PAR_DALPH);

	api_add_settings_item(ID_DRANG1,					// Parâmetro: "Inicio do range dos tickets"
						PAR_DRANG1_LEN,
						(char *)PAR_DRANG1);

	api_add_settings_item(ID_DRANG2,					// Parâmetro: "Final do range dos tickets"
						PAR_DRANG2_LEN,
						(char *)PAR_DRANG2);
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_dep_req_part2
 * Descrição: 	Monta resposta a requisição "department", com a segunda parte
 *				dos parâmetros do departamento
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_handle_dep_req_part2(void)
{
	api_req_handled=1;									// Marca como requisição tratada.
	api_resp_len=0;										// Inicializa resposta http

	api_add_settings_item(ID_DCLOS1,					// Parâmetro: "Fechar atendimento chamando próximo"
						PAR_DCLOS1_LEN,
						(char *)PAR_DCLOS1);

	api_add_settings_item(ID_DCLOS2,					// Parâmetro: "Fechar atendimento no relatório"
						PAR_DCLOS2_LEN,
						(char *)PAR_DCLOS2);

	api_add_settings_item(ID_DCALL1,					// Parâmetro: "Proporção sistema chamada para: normais"
						PAR_DCALL1_LEN,
						(char *)PAR_DCALL1);

	api_add_settings_item(ID_DCALL2,					// Parâmetro: "Proporção sistema chamada para: preferencial"
						PAR_DCALL2_LEN,
						(char *)PAR_DCALL2);

	api_add_settings_item(ID_DHLP1,						// Parâmetro: "Ajudar departamento 1"
						PAR_DHLP1_LEN,
						(char *)PAR_DHLP1);

	api_add_settings_item(ID_DHLP2,						// Parâmetro: "Ajudar departamento 2"
						PAR_DHLP2_LEN,
						(char *)PAR_DHLP2);

	api_add_settings_item(ID_DHLP3,						// Parâmetro: "Ajudar maior fila"
						PAR_DHLP3_LEN,
						(char *)PAR_DHLP3);

	api_add_settings_item(ID_DHLP4,						// Parâmetro: "Proporção sistema ajuda para: Local"
						PAR_DHLP4_LEN,
						(char *)PAR_DHLP4);

	api_add_settings_item(ID_DHLP5,						// Parâmetro: "Proporção sistema ajuda para: Outro dpto"
						PAR_DHLP5_LEN,
						(char *)PAR_DHLP5);

	api_add_settings_item(ID_DIPD,						// Parâmetro: "IP display do dpto"
						PAR_DIPD_LEN,
						(char *)PAR_DIPD);

	api_add_settings_item(ID_DITFD,						// Parâmetro: "Interf display do dpto"
						PAR_DITFD_LEN,
						(char *)PAR_DITFD);

	api_add_settings_item(ID_DIPT,						// Parâmetro: "IP TOP do dpto"
						PAR_DIPT_LEN,
						(char *)PAR_DIPT);

	api_add_settings_item(ID_DITFT,						// Parâmetro: "Interf TOP do dpto"
						PAR_DITFT_LEN,
						(char *)PAR_DITFT);

	api_add_settings_item(ID_ENATOP,					// Parâmetro: "Habilita TOP do dpto"
						PAR_ENATOP_LEN,
						(char *)PAR_ENATOP);

	api_add_settings_item(ID_DLIND,						// Parâmetro: "Linha display do departamento"
						PAR_DLIND_LEN,
						(char *)PAR_DLIND);

	api_add_settings_item(ID_DPRFD,						// Parâmetro: "Dpto Preferencial de outro dpto"
						PAR_DPRFD_LEN,
						(char *)PAR_DPRFD);

	api_add_settings_item(ID_DU01,						// Parâmetro: "Departamento do usuário"
						PAR_DU01_LEN,
						(char *)PAR_DU01);

	api_add_settings_item(ID_DU02,						// Parâmetro: "Departamento do usuário"
						PAR_DU02_LEN,
						(char *)PAR_DU02);

	api_add_settings_item(ID_DU03,						// Parâmetro: "Departamento do usuário"
						PAR_DU03_LEN,
						(char *)PAR_DU03);

	api_add_settings_item(ID_DU04,						// Parâmetro: "Departamento do usuário"
						PAR_DU04_LEN,
						(char *)PAR_DU04);
						
	api_add_settings_item(ID_DU05,						// Parâmetro: "Departamento do usuário"
						PAR_DU05_LEN,
						(char *)PAR_DU05);
						
	api_add_settings_item(ID_DU06,						// Parâmetro: "Departamento do usuário"
						PAR_DU06_LEN,
						(char *)PAR_DU06);
						
	api_add_settings_item(ID_DU07,						// Parâmetro: "Departamento do usuário"
						PAR_DU07_LEN,
						(char *)PAR_DU07);
						
	api_add_settings_item(ID_DU08,						// Parâmetro: "Departamento do usuário"
						PAR_DU08_LEN,
						(char *)PAR_DU08);
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_dep_req_part3
 * Descrição: 	Monta resposta a requisição "department", com a terceira parte
 *				dos parâmetros do departamento
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_handle_dep_req_part3(void)
{
	api_req_handled=1;									// Marca como requisição tratada.
	api_resp_len=0;			
					
	api_add_settings_item(ID_DU09,						// Parâmetro: "Departamento do usuário"
						PAR_DU09_LEN,
						(char *)PAR_DU09);
						
	api_add_settings_item(ID_DU10,						// Parâmetro: "Departamento do usuário"
						PAR_DU10_LEN,
						(char *)PAR_DU10);

	api_add_settings_item(ID_DU11,						// Parâmetro: "Departamento do usuário"
						PAR_DU11_LEN,
						(char *)PAR_DU11);

	api_add_settings_item(ID_DU12,						// Parâmetro: "Departamento do usuário"
						PAR_DU12_LEN,
						(char *)PAR_DU12);

	api_add_settings_item(ID_DU13,						// Parâmetro: "Departamento do usuário"
						PAR_DU13_LEN,
						(char *)PAR_DU13);

	api_add_settings_item(ID_DU14,						// Parâmetro: "Departamento do usuário"
						PAR_DU14_LEN,
						(char *)PAR_DU14);
						
	api_add_settings_item(ID_DU15,						// Parâmetro: "Departamento do usuário"
						PAR_DU15_LEN,
						(char *)PAR_DU15);
						
	api_add_settings_item(ID_DU16,						// Parâmetro: "Departamento do usuário"
						PAR_DU16_LEN,
						(char *)PAR_DU16);
						
	api_add_settings_item(ID_DU17,						// Parâmetro: "Departamento do usuário"
						PAR_DU17_LEN,
						(char *)PAR_DU17);
						
	api_add_settings_item(ID_DU18,						// Parâmetro: "Departamento do usuário"
						PAR_DU18_LEN,
						(char *)PAR_DU18);
						
	api_add_settings_item(ID_DU19,						// Parâmetro: "Departamento do usuário"
						PAR_DU19_LEN,
						(char *)PAR_DU19);

	api_add_settings_item(ID_DU20,						// Parâmetro: "Departamento do usuário"
						PAR_DU20_LEN,
						(char *)PAR_DU20);

	api_add_settings_item(ID_DU21,						// Parâmetro: "Departamento do usuário"
						PAR_DU21_LEN,
						(char *)PAR_DU21);

	api_add_settings_item(ID_DU22,						// Parâmetro: "Departamento do usuário"
						PAR_DU22_LEN,
						(char *)PAR_DU22);

	api_add_settings_item(ID_DU23,						// Parâmetro: "Departamento do usuário"
						PAR_DU23_LEN,
						(char *)PAR_DU23);

	api_add_settings_item(ID_DU24,						// Parâmetro: "Departamento do usuário"
						PAR_DU24_LEN,
						(char *)PAR_DU24);
						
	api_add_settings_item(ID_DU25,						// Parâmetro: "Departamento do usuário"
						PAR_DU25_LEN,
						(char *)PAR_DU25);
						
	api_add_settings_item(ID_DU26,						// Parâmetro: "Departamento do usuário"
						PAR_DU26_LEN,
						(char *)PAR_DU26);
						
	api_add_settings_item(ID_DU27,						// Parâmetro: "Departamento do usuário"
						PAR_DU27_LEN,
						(char *)PAR_DU27);
						
	api_add_settings_item(ID_DU28,						// Parâmetro: "Departamento do usuário"
						PAR_DU28_LEN,
						(char *)PAR_DU28);
						
	api_add_settings_item(ID_DU29,						// Parâmetro: "Departamento do usuário"
						PAR_DU29_LEN,
						(char *)PAR_DU29);

	api_add_settings_item(ID_DU30,						// Parâmetro: "Departamento do usuário"
						PAR_DU30_LEN,
						(char *)PAR_DU30);

	api_add_settings_item(ID_DU31,						// Parâmetro: "Departamento do usuário"
						PAR_DU31_LEN,
						(char *)PAR_DU31);

	api_add_settings_item(ID_DU32,						// Parâmetro: "Departamento do usuário"
						PAR_DU32_LEN,
						(char *)PAR_DU32);

	api_add_settings_item(ID_DU33,						// Parâmetro: "Departamento do usuário"
						PAR_DU33_LEN,
						(char *)PAR_DU33);

	api_add_settings_item(ID_DU34,						// Parâmetro: "Departamento do usuário"
						PAR_DU34_LEN,
						(char *)PAR_DU34);
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_dep_req_part4
 * Descrição: 	Monta resposta a requisição "department", com a quarta parte
 *				dos parâmetros do departamento
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_handle_dep_req_part4(void)
{
	api_req_handled=1;									// Marca como requisição tratada.
	api_resp_len=0;	
							
	api_add_settings_item(ID_DU35,						// Parâmetro: "Departamento do usuário"
						PAR_DU35_LEN,
						(char *)PAR_DU35);
						
	api_add_settings_item(ID_DU36,						// Parâmetro: "Departamento do usuário"
						PAR_DU36_LEN,
						(char *)PAR_DU36);
						
	api_add_settings_item(ID_DU37,						// Parâmetro: "Departamento do usuário"
						PAR_DU37_LEN,
						(char *)PAR_DU37);
						
	api_add_settings_item(ID_DU38,						// Parâmetro: "Departamento do usuário"
						PAR_DU38_LEN,
						(char *)PAR_DU38);
						
	api_add_settings_item(ID_DU39,						// Parâmetro: "Departamento do usuário"
						PAR_DU39_LEN,
						(char *)PAR_DU39);	

	api_add_settings_item(ID_DU40,						// Parâmetro: "Departamento do usuário"
						PAR_DU40_LEN,
						(char *)PAR_DU40);

	api_add_settings_item(ID_DU41,						// Parâmetro: "Departamento do usuário"
						PAR_DU41_LEN,
						(char *)PAR_DU41);

	api_add_settings_item(ID_DU42,						// Parâmetro: "Departamento do usuário"
						PAR_DU42_LEN,
						(char *)PAR_DU42);

	api_add_settings_item(ID_DU43,						// Parâmetro: "Departamento do usuário"
						PAR_DU43_LEN,
						(char *)PAR_DU43);

	api_add_settings_item(ID_DU44,						// Parâmetro: "Departamento do usuário"
						PAR_DU44_LEN,
						(char *)PAR_DU44);
						
	api_add_settings_item(ID_DU45,						// Parâmetro: "Departamento do usuário"
						PAR_DU45_LEN,
						(char *)PAR_DU45);
						
	api_add_settings_item(ID_DU46,						// Parâmetro: "Departamento do usuário"
						PAR_DU46_LEN,
						(char *)PAR_DU46);
						
	api_add_settings_item(ID_DU47,						// Parâmetro: "Departamento do usuário"
						PAR_DU47_LEN,
						(char *)PAR_DU47);
						
	api_add_settings_item(ID_DU48,						// Parâmetro: "Departamento do usuário"
						PAR_DU48_LEN,
						(char *)PAR_DU48);
						
	api_add_settings_item(ID_DU49,						// Parâmetro: "Departamento do usuário"
						PAR_DU49_LEN,
						(char *)PAR_DU49);	

	api_add_settings_item(ID_DU50,						// Parâmetro: "Departamento do usuário"
						PAR_DU50_LEN,
						(char *)PAR_DU50);

	api_add_settings_item(ID_DU51,						// Parâmetro: "Departamento do usuário"
						PAR_DU51_LEN,
						(char *)PAR_DU51);

	api_add_settings_item(ID_DU52,						// Parâmetro: "Departamento do usuário"
						PAR_DU52_LEN,
						(char *)PAR_DU52);

	api_add_settings_item(ID_DU53,						// Parâmetro: "Departamento do usuário"
						PAR_DU53_LEN,
						(char *)PAR_DU53);

	api_add_settings_item(ID_DU54,						// Parâmetro: "Departamento do usuário"
						PAR_DU54_LEN,
						(char *)PAR_DU54);
						
	api_add_settings_item(ID_DU55,						// Parâmetro: "Departamento do usuário"
						PAR_DU55_LEN,
						(char *)PAR_DU55);
						
	api_add_settings_item(ID_DU56,						// Parâmetro: "Departamento do usuário"
						PAR_DU56_LEN,
						(char *)PAR_DU56);
						
	api_add_settings_item(ID_DU57,						// Parâmetro: "Departamento do usuário"
						PAR_DU57_LEN,
						(char *)PAR_DU57);
						
	api_add_settings_item(ID_DU58,						// Parâmetro: "Departamento do usuário"
						PAR_DU58_LEN,
						(char *)PAR_DU58);
						
	api_add_settings_item(ID_DU59,						// Parâmetro: "Departamento do usuário"
						PAR_DU59_LEN,
						(char *)PAR_DU59);	
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_dep_req_part5
 * Descrição: 	Monta resposta a requisição "department", com a quinta parte
 *				dos parâmetros do departamento
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_handle_dep_req_part5(void)
{
	api_req_handled=1;									// Marca como requisição tratada.
	api_resp_len=0;	
							
	api_add_settings_item(ID_DU60,						// Parâmetro: "Departamento do usuário"
						PAR_DU60_LEN,
						(char *)PAR_DU60);

	api_add_settings_item(ID_DU61,						// Parâmetro: "Departamento do usuário"
						PAR_DU61_LEN,
						(char *)PAR_DU61);

	api_add_settings_item(ID_DU62,						// Parâmetro: "Departamento do usuário"
						PAR_DU62_LEN,
						(char *)PAR_DU62);

	api_add_settings_item(ID_DU63,						// Parâmetro: "Departamento do usuário"
						PAR_DU63_LEN,
						(char *)PAR_DU63);

	api_add_settings_item(ID_DU64,						// Parâmetro: "Departamento do usuário"
						PAR_DU64_LEN,
						(char *)PAR_DU64);
						
	api_add_settings_item(ID_DU65,						// Parâmetro: "Departamento do usuário"
						PAR_DU65_LEN,
						(char *)PAR_DU65);
						
	api_add_settings_item(ID_DU66,						// Parâmetro: "Departamento do usuário"
						PAR_DU66_LEN,
						(char *)PAR_DU66);
						
	api_add_settings_item(ID_DU67,						// Parâmetro: "Departamento do usuário"
						PAR_DU67_LEN,
						(char *)PAR_DU67);
						
	api_add_settings_item(ID_DU68,						// Parâmetro: "Departamento do usuário"
						PAR_DU68_LEN,
						(char *)PAR_DU68);
						
	api_add_settings_item(ID_DU69,						// Parâmetro: "Departamento do usuário"
						PAR_DU69_LEN,
						(char *)PAR_DU69);	

	api_add_settings_item(ID_DU70,						// Parâmetro: "Departamento do usuário"
						PAR_DU70_LEN,
						(char *)PAR_DU70);

	api_add_settings_item(ID_DU71,						// Parâmetro: "Departamento do usuário"
						PAR_DU71_LEN,
						(char *)PAR_DU71);

	api_add_settings_item(ID_DU72,						// Parâmetro: "Departamento do usuário"
						PAR_DU72_LEN,
						(char *)PAR_DU72);

	api_add_settings_item(ID_DU73,						// Parâmetro: "Departamento do usuário"
						PAR_DU73_LEN,
						(char *)PAR_DU73);

	api_add_settings_item(ID_DU74,						// Parâmetro: "Departamento do usuário"
						PAR_DU74_LEN,
						(char *)PAR_DU74);
						
	api_add_settings_item(ID_DU75,						// Parâmetro: "Departamento do usuário"
						PAR_DU75_LEN,
						(char *)PAR_DU75);
						
	api_add_settings_item(ID_DU76,						// Parâmetro: "Departamento do usuário"
						PAR_DU76_LEN,
						(char *)PAR_DU76);
						
	api_add_settings_item(ID_DU77,						// Parâmetro: "Departamento do usuário"
						PAR_DU77_LEN,
						(char *)PAR_DU77);
						
	api_add_settings_item(ID_DU78,						// Parâmetro: "Departamento do usuário"
						PAR_DU78_LEN,
						(char *)PAR_DU78);
						
	api_add_settings_item(ID_DU79,						// Parâmetro: "Departamento do usuário"
						PAR_DU79_LEN,
						(char *)PAR_DU79);	

	api_add_settings_item(ID_DU80,						// Parâmetro: "Departamento do usuário"
						PAR_DU80_LEN,
						(char *)PAR_DU80);

	api_add_settings_item(ID_DU81,						// Parâmetro: "Departamento do usuário"
						PAR_DU81_LEN,
						(char *)PAR_DU81);

	api_add_settings_item(ID_DU82,						// Parâmetro: "Departamento do usuário"
						PAR_DU82_LEN,
						(char *)PAR_DU82);

	api_add_settings_item(ID_DU83,						// Parâmetro: "Departamento do usuário"
						PAR_DU83_LEN,
						(char *)PAR_DU83);
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_dep_req_part6
 * Descrição: 	Monta resposta a requisição "department", com a sexta parte
 *				dos parâmetros do departamento
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_handle_dep_req_part6(void)
{
	api_req_handled=1;									// Marca como requisição tratada.
	api_resp_len=0;	

	api_add_settings_item(ID_DU84,						// Parâmetro: "Departamento do usuário"
						PAR_DU84_LEN,
						(char *)PAR_DU84);
						
	api_add_settings_item(ID_DU85,						// Parâmetro: "Departamento do usuário"
						PAR_DU85_LEN,
						(char *)PAR_DU85);
						
	api_add_settings_item(ID_DU86,						// Parâmetro: "Departamento do usuário"
						PAR_DU86_LEN,
						(char *)PAR_DU86);
						
	api_add_settings_item(ID_DU87,						// Parâmetro: "Departamento do usuário"
						PAR_DU87_LEN,
						(char *)PAR_DU87);
						
	api_add_settings_item(ID_DU88,						// Parâmetro: "Departamento do usuário"
						PAR_DU88_LEN,
						(char *)PAR_DU88);
						
	api_add_settings_item(ID_DU89,						// Parâmetro: "Departamento do usuário"
						PAR_DU89_LEN,
						(char *)PAR_DU89);	

	api_add_settings_item(ID_DU90,						// Parâmetro: "Departamento do usuário"
						PAR_DU90_LEN,
						(char *)PAR_DU90);

	api_add_settings_item(ID_DU91,						// Parâmetro: "Departamento do usuário"
						PAR_DU91_LEN,
						(char *)PAR_DU91);

	api_add_settings_item(ID_DU92,						// Parâmetro: "Departamento do usuário"
						PAR_DU92_LEN,
						(char *)PAR_DU92);

	api_add_settings_item(ID_DU93,						// Parâmetro: "Departamento do usuário"
						PAR_DU93_LEN,
						(char *)PAR_DU93);

	api_add_settings_item(ID_DU94,						// Parâmetro: "Departamento do usuário"
						PAR_DU94_LEN,
						(char *)PAR_DU94);
						
	api_add_settings_item(ID_DU95,						// Parâmetro: "Departamento do usuário"
						PAR_DU95_LEN,
						(char *)PAR_DU95);
						
	api_add_settings_item(ID_DU96,						// Parâmetro: "Departamento do usuário"
						PAR_DU96_LEN,
						(char *)PAR_DU96);
						
	api_add_settings_item(ID_DU97,						// Parâmetro: "Departamento do usuário"
						PAR_DU97_LEN,
						(char *)PAR_DU97);
						
	api_add_settings_item(ID_DU98,						// Parâmetro: "Departamento do usuário"
						PAR_DU98_LEN,
						(char *)PAR_DU98);
						
	api_add_settings_item(ID_DU99,						// Parâmetro: "Departamento do usuário"
						PAR_DU99_LEN,
						(char *)PAR_DU99);	
	api_dec();											// Decrementa último caractere: "Vírgula"

	api_add_json_object_end();							// JSON settings value object end
	api_add_json_object_end();							// JSON settings object end
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_ubox_req_part1
 * Descrição: 	Monta resposta a requisição "user box", com a primeira parte
 *				dos parâmetros do box do usuário
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_handle_ubox_req_part1(void)
{
	api_req_handled=1;									// Marca como requisição tratada.
	api_add_http_headers('j');							// HTTP headers "application/JSON"
	api_add_json_object_start();						// JSON settings object start

	api_add_settings_item(ID_BOOT,						// Parâmetro: boot: "0"=não "1"=sim"
						PAR_BOOT_LEN,
						(char *)PAR_BOOT);

	api_add_json_object_name(PAR_SET_LEN,				// Settings name
						(char *)PAR_SET);

	api_add_json_object_start();						// JSON settings value object start

	api_add_settings_item(ID_UB01,						// Parâmetro: "Box do usuário"
						PAR_UB01_LEN,
						(char *)PAR_UB01);

	api_add_settings_item(ID_UB02,						// Parâmetro: "Box do usuário"
						PAR_UB02_LEN,
						(char *)PAR_UB02);

	api_add_settings_item(ID_UB03,						// Parâmetro: "Box do usuário"
						PAR_UB03_LEN,
						(char *)PAR_UB03);

	api_add_settings_item(ID_UB04,						// Parâmetro: "Box do usuário"
						PAR_UB04_LEN,
						(char *)PAR_UB04);

	api_add_settings_item(ID_UB05,						// Parâmetro: "Box do usuário"
						PAR_UB05_LEN,
						(char *)PAR_UB05);

	api_add_settings_item(ID_UB06,						// Parâmetro: "Box do usuário"
						PAR_UB06_LEN,
						(char *)PAR_UB06);

	api_add_settings_item(ID_UB07,						// Parâmetro: "Box do usuário"
						PAR_UB07_LEN,
						(char *)PAR_UB07);

	api_add_settings_item(ID_UB08,						// Parâmetro: "Box do usuário"
						PAR_UB08_LEN,
						(char *)PAR_UB08);

	api_add_settings_item(ID_UB09,						// Parâmetro: "Box do usuário"
						PAR_UB09_LEN,
						(char *)PAR_UB09);

	api_add_settings_item(ID_UB10,						// Parâmetro: "Box do usuário"
						PAR_UB10_LEN,
						(char *)PAR_UB10);

	api_add_settings_item(ID_UB11,						// Parâmetro: "Box do usuário"
						PAR_UB11_LEN,
						(char *)PAR_UB11);

	api_add_settings_item(ID_UB12,						// Parâmetro: "Box do usuário"
						PAR_UB12_LEN,
						(char *)PAR_UB12);

	api_add_settings_item(ID_UB13,						// Parâmetro: "Box do usuário"
						PAR_UB13_LEN,
						(char *)PAR_UB13);
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_ubox_req_part2
 * Descrição: 	Monta resposta a requisição "user box", com a segunda parte
 *				dos parâmetros do box do usuário
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_handle_ubox_req_part2(void)
{
	api_req_handled=1;									// Marca como requisição tratada.
	api_resp_len=0;										// Inicializa resposta http

	api_add_settings_item(ID_UB14,						// Parâmetro: "Box do usuário"
						PAR_UB14_LEN,
						(char *)PAR_UB14);

	api_add_settings_item(ID_UB15,						// Parâmetro: "Box do usuário"
						PAR_UB15_LEN,
						(char *)PAR_UB15);

	api_add_settings_item(ID_UB16,						// Parâmetro: "Box do usuário"
						PAR_UB16_LEN,
						(char *)PAR_UB16);

	api_add_settings_item(ID_UB17,						// Parâmetro: "Box do usuário"
						PAR_UB17_LEN,
						(char *)PAR_UB17);

	api_add_settings_item(ID_UB18,						// Parâmetro: "Box do usuário"
						PAR_UB18_LEN,
						(char *)PAR_UB18);

	api_add_settings_item(ID_UB19,						// Parâmetro: "Box do usuário"
						PAR_UB19_LEN,
						(char *)PAR_UB19);

	api_add_settings_item(ID_UB20,						// Parâmetro: "Box do usuário"
						PAR_UB20_LEN,
						(char *)PAR_UB20);

	api_add_settings_item(ID_UB21,						// Parâmetro: "Box do usuário"
						PAR_UB21_LEN,
						(char *)PAR_UB21);

	api_add_settings_item(ID_UB22,						// Parâmetro: "Box do usuário"
						PAR_UB22_LEN,
						(char *)PAR_UB22);

	api_add_settings_item(ID_UB23,						// Parâmetro: "Box do usuário"
						PAR_UB23_LEN,
						(char *)PAR_UB23);

	api_add_settings_item(ID_UB24,						// Parâmetro: "Box do usuário"
						PAR_UB24_LEN,
						(char *)PAR_UB24);

	api_add_settings_item(ID_UB25,						// Parâmetro: "Box do usuário"
						PAR_UB25_LEN,
						(char *)PAR_UB25);

	api_add_settings_item(ID_UB26,						// Parâmetro: "Box do usuário"
						PAR_UB26_LEN,
						(char *)PAR_UB26);

	api_add_settings_item(ID_UB27,						// Parâmetro: "Box do usuário"
						PAR_UB27_LEN,
						(char *)PAR_UB27);

	api_add_settings_item(ID_UB28,						// Parâmetro: "Box do usuário"
						PAR_UB28_LEN,
						(char *)PAR_UB28);

	api_add_settings_item(ID_UB29,						// Parâmetro: "Box do usuário"
						PAR_UB29_LEN,
						(char *)PAR_UB29);

	api_add_settings_item(ID_UB30,						// Parâmetro: "Box do usuário"
						PAR_UB30_LEN,
						(char *)PAR_UB30);

	api_add_settings_item(ID_UB31,						// Parâmetro: "Box do usuário"
						PAR_UB31_LEN,
						(char *)PAR_UB31);

	api_add_settings_item(ID_UB32,						// Parâmetro: "Box do usuário"
						PAR_UB32_LEN,
						(char *)PAR_UB32);

	api_add_settings_item(ID_UB33,						// Parâmetro: "Box do usuário"
						PAR_UB33_LEN,
						(char *)PAR_UB33);

	api_add_settings_item(ID_UB34,						// Parâmetro: "Box do usuário"
						PAR_UB34_LEN,
						(char *)PAR_UB34);

	api_add_settings_item(ID_UB35,						// Parâmetro: "Box do usuário"
						PAR_UB35_LEN,
						(char *)PAR_UB35);

	api_add_settings_item(ID_UB36,						// Parâmetro: "Box do usuário"
						PAR_UB36_LEN,
						(char *)PAR_UB36);

	api_add_settings_item(ID_UB37,						// Parâmetro: "Box do usuário"
						PAR_UB37_LEN,
						(char *)PAR_UB37);

	api_add_settings_item(ID_UB38,						// Parâmetro: "Box do usuário"
						PAR_UB38_LEN,
						(char *)PAR_UB38);
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_ubox_req_part3
 * Descrição: 	Monta resposta a requisição "user box", com a terceira parte
 *				dos parâmetros do box do usuário
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_handle_ubox_req_part3(void)
{
	api_req_handled=1;									// Marca como requisição tratada.
	api_resp_len=0;										// Inicializa resposta http

	api_add_settings_item(ID_UB39,						// Parâmetro: "Box do usuário"
						PAR_UB39_LEN,
						(char *)PAR_UB39);

	api_add_settings_item(ID_UB40,						// Parâmetro: "Box do usuário"
						PAR_UB40_LEN,
						(char *)PAR_UB40);

	api_add_settings_item(ID_UB41,						// Parâmetro: "Box do usuário"
						PAR_UB41_LEN,
						(char *)PAR_UB41);

	api_add_settings_item(ID_UB42,						// Parâmetro: "Box do usuário"
						PAR_UB42_LEN,
						(char *)PAR_UB42);

	api_add_settings_item(ID_UB43,						// Parâmetro: "Box do usuário"
						PAR_UB43_LEN,
						(char *)PAR_UB43);

	api_add_settings_item(ID_UB44,						// Parâmetro: "Box do usuário"
						PAR_UB44_LEN,
						(char *)PAR_UB44);

	api_add_settings_item(ID_UB45,						// Parâmetro: "Box do usuário"
						PAR_UB45_LEN,
						(char *)PAR_UB45);

	api_add_settings_item(ID_UB46,						// Parâmetro: "Box do usuário"
						PAR_UB46_LEN,
						(char *)PAR_UB46);

	api_add_settings_item(ID_UB47,						// Parâmetro: "Box do usuário"
						PAR_UB47_LEN,
						(char *)PAR_UB47);

	api_add_settings_item(ID_UB48,						// Parâmetro: "Box do usuário"
						PAR_UB48_LEN,
						(char *)PAR_UB48);

	api_add_settings_item(ID_UB49,						// Parâmetro: "Box do usuário"
						PAR_UB49_LEN,
						(char *)PAR_UB49);

	api_add_settings_item(ID_UB50,						// Parâmetro: "Box do usuário"
						PAR_UB50_LEN,
						(char *)PAR_UB50);

	api_add_settings_item(ID_UB51,						// Parâmetro: "Box do usuário"
						PAR_UB51_LEN,
						(char *)PAR_UB51);

	api_add_settings_item(ID_UB52,						// Parâmetro: "Box do usuário"
						PAR_UB52_LEN,
						(char *)PAR_UB52);

	api_add_settings_item(ID_UB53,						// Parâmetro: "Box do usuário"
						PAR_UB53_LEN,
						(char *)PAR_UB53);

	api_add_settings_item(ID_UB54,						// Parâmetro: "Box do usuário"
						PAR_UB54_LEN,
						(char *)PAR_UB54);

	api_add_settings_item(ID_UB55,						// Parâmetro: "Box do usuário"
						PAR_UB55_LEN,
						(char *)PAR_UB55);

	api_add_settings_item(ID_UB56,						// Parâmetro: "Box do usuário"
						PAR_UB56_LEN,
						(char *)PAR_UB56);

	api_add_settings_item(ID_UB57,						// Parâmetro: "Box do usuário"
						PAR_UB57_LEN,
						(char *)PAR_UB57);

	api_add_settings_item(ID_UB58,						// Parâmetro: "Box do usuário"
						PAR_UB58_LEN,
						(char *)PAR_UB58);

	api_add_settings_item(ID_UB59,						// Parâmetro: "Box do usuário"
						PAR_UB59_LEN,
						(char *)PAR_UB59);

	api_add_settings_item(ID_UB60,						// Parâmetro: "Box do usuário"
						PAR_UB60_LEN,
						(char *)PAR_UB60);

	api_add_settings_item(ID_UB61,						// Parâmetro: "Box do usuário"
						PAR_UB61_LEN,
						(char *)PAR_UB61);

	api_add_settings_item(ID_UB62,						// Parâmetro: "Box do usuário"
						PAR_UB62_LEN,
						(char *)PAR_UB62);

	api_add_settings_item(ID_UB63,						// Parâmetro: "Box do usuário"
						PAR_UB63_LEN,
						(char *)PAR_UB63);
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_ubox_req_part4
 * Descrição: 	Monta resposta a requisição "user box", com a quarta parte
 *				dos parâmetros do box do usuário
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_handle_ubox_req_part4(void)
{
	api_req_handled=1;									// Marca como requisição tratada.
	api_resp_len=0;										// Inicializa resposta http

	api_add_settings_item(ID_UB64,						// Parâmetro: "Box do usuário"
						PAR_UB64_LEN,
						(char *)PAR_UB64);

	api_add_settings_item(ID_UB65,						// Parâmetro: "Box do usuário"
						PAR_UB65_LEN,
						(char *)PAR_UB65);

	api_add_settings_item(ID_UB66,						// Parâmetro: "Box do usuário"
						PAR_UB66_LEN,
						(char *)PAR_UB66);

	api_add_settings_item(ID_UB67,						// Parâmetro: "Box do usuário"
						PAR_UB67_LEN,
						(char *)PAR_UB67);

	api_add_settings_item(ID_UB68,						// Parâmetro: "Box do usuário"
						PAR_UB68_LEN,
						(char *)PAR_UB68);

	api_add_settings_item(ID_UB69,						// Parâmetro: "Box do usuário"
						PAR_UB69_LEN,
						(char *)PAR_UB69);

	api_add_settings_item(ID_UB70,						// Parâmetro: "Box do usuário"
						PAR_UB70_LEN,
						(char *)PAR_UB70);

	api_add_settings_item(ID_UB71,						// Parâmetro: "Box do usuário"
						PAR_UB71_LEN,
						(char *)PAR_UB71);

	api_add_settings_item(ID_UB72,						// Parâmetro: "Box do usuário"
						PAR_UB72_LEN,
						(char *)PAR_UB72);

	api_add_settings_item(ID_UB73,						// Parâmetro: "Box do usuário"
						PAR_UB73_LEN,
						(char *)PAR_UB73);

	api_add_settings_item(ID_UB74,						// Parâmetro: "Box do usuário"
						PAR_UB74_LEN,
						(char *)PAR_UB74);

	api_add_settings_item(ID_UB75,						// Parâmetro: "Box do usuário"
						PAR_UB75_LEN,
						(char *)PAR_UB75);

	api_add_settings_item(ID_UB76,						// Parâmetro: "Box do usuário"
						PAR_UB76_LEN,
						(char *)PAR_UB76);

	api_add_settings_item(ID_UB77,						// Parâmetro: "Box do usuário"
						PAR_UB77_LEN,
						(char *)PAR_UB77);

	api_add_settings_item(ID_UB78,						// Parâmetro: "Box do usuário"
						PAR_UB78_LEN,
						(char *)PAR_UB78);

	api_add_settings_item(ID_UB79,						// Parâmetro: "Box do usuário"
						PAR_UB79_LEN,
						(char *)PAR_UB79);

	api_add_settings_item(ID_UB80,						// Parâmetro: "Box do usuário"
						PAR_UB80_LEN,
						(char *)PAR_UB80);

	api_add_settings_item(ID_UB81,						// Parâmetro: "Box do usuário"
						PAR_UB81_LEN,
						(char *)PAR_UB81);

	api_add_settings_item(ID_UB82,						// Parâmetro: "Box do usuário"
						PAR_UB82_LEN,
						(char *)PAR_UB82);

	api_add_settings_item(ID_UB83,						// Parâmetro: "Box do usuário"
						PAR_UB83_LEN,
						(char *)PAR_UB83);

	api_add_settings_item(ID_UB84,						// Parâmetro: "Box do usuário"
						PAR_UB84_LEN,
						(char *)PAR_UB84);

	api_add_settings_item(ID_UB85,						// Parâmetro: "Box do usuário"
						PAR_UB85_LEN,
						(char *)PAR_UB85);

	api_add_settings_item(ID_UB86,						// Parâmetro: "Box do usuário"
						PAR_UB86_LEN,
						(char *)PAR_UB86);

	api_add_settings_item(ID_UB87,						// Parâmetro: "Box do usuário"
						PAR_UB87_LEN,
						(char *)PAR_UB87);

	api_add_settings_item(ID_UB88,						// Parâmetro: "Box do usuário"
						PAR_UB88_LEN,
						(char *)PAR_UB88);
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_ubox_req_part5
 * Descrição: 	Monta resposta a requisição "user box", com a quinta parte
 *				dos parâmetros do box do usuário
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_handle_ubox_req_part5(void)
{
	api_req_handled=1;									// Marca como requisição tratada.
	api_resp_len=0;										// Inicializa resposta http

	api_add_settings_item(ID_UB89,						// Parâmetro: "Box do usuário"
						PAR_UB89_LEN,
						(char *)PAR_UB89);

	api_add_settings_item(ID_UB90,						// Parâmetro: "Box do usuário"
						PAR_UB90_LEN,
						(char *)PAR_UB90);

	api_add_settings_item(ID_UB91,						// Parâmetro: "Box do usuário"
						PAR_UB91_LEN,
						(char *)PAR_UB91);

	api_add_settings_item(ID_UB92,						// Parâmetro: "Box do usuário"
						PAR_UB92_LEN,
						(char *)PAR_UB92);

	api_add_settings_item(ID_UB93,						// Parâmetro: "Box do usuário"
						PAR_UB93_LEN,
						(char *)PAR_UB93);

	api_add_settings_item(ID_UB94,						// Parâmetro: "Box do usuário"
						PAR_UB94_LEN,
						(char *)PAR_UB94);

	api_add_settings_item(ID_UB95,						// Parâmetro: "Box do usuário"
						PAR_UB95_LEN,
						(char *)PAR_UB95);

	api_add_settings_item(ID_UB96,						// Parâmetro: "Box do usuário"
						PAR_UB96_LEN,
						(char *)PAR_UB96);

	api_add_settings_item(ID_UB97,						// Parâmetro: "Box do usuário"
						PAR_UB97_LEN,
						(char *)PAR_UB97);

	api_add_settings_item(ID_UB98,						// Parâmetro: "Box do usuário"
						PAR_UB98_LEN,
						(char *)PAR_UB98);

	api_add_settings_item(ID_UB99,						// Parâmetro: "Box do usuário"
						PAR_UB99_LEN,
						(char *)PAR_UB99);
	api_dec();											// Decrementa último caractere: "Vírgula"

	api_add_json_object_end();							// JSON settings value object end
	api_add_json_object_end();							// JSON settings object end
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_tkt_req_part1
 * Descrição: 	Monta resposta a requisição "ticket", com a primeira parte
 *				dos parâmetros do ticket
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_handle_tkt_req_part1(void)
{
	api_req_handled=1;									// Marca como requisição tratada.
	api_add_http_headers('j');							// HTTP headers "application/JSON"
	api_add_json_object_start();						// JSON settings object start

	api_add_settings_item(ID_BOOT,						// Parâmetro: boot: "0"=não "1"=sim"
						PAR_BOOT_LEN,
						(char *)PAR_BOOT);

	api_add_json_object_name(PAR_SET_LEN,				// Settings name
						(char *)PAR_SET);

	api_add_json_object_start();						// JSON settings value object start

	api_add_settings_item(ID_LG,						// Parâmetro: "Habilita logotipo"
						PAR_LG_LEN,
						(char *)PAR_LG);

	api_add_settings_item(ID_DLEN,						// Parâmetro: "Habilita letra"
						PAR_DLEN_LEN,
						(char *)PAR_DLEN);

	api_add_settings_item(ID_DLNUM,						// Parâmetro: "Comprimento do ticket"
						PAR_DLNUM_LEN,
						(char *)PAR_DLNUM);

	api_add_settings_item(ID_DHNUM,						// Parâmetro: "Altura número ticket"
						PAR_DHNUM_LEN,
						(char *)PAR_DHNUM);

	api_add_settings_item(ID_CUT,						// Parâmetro: "Altura número ticket"
						PAR_CUT_LEN,
						(char *)PAR_CUT);

	api_add_settings_item(ID_VIA,						// Parâmetro: "Número de vias"
						PAR_VIA_LEN,
						(char *)PAR_VIA);

	api_add_settings_item(ID_H1OP,						// Parâmetro: "Opções do cabeç. 1"
						PAR_H1OP_LEN,
						(char *)PAR_H1OP);

	api_add_settings_item(ID_H2OP,						// Parâmetro: "Opções do cabeç. 2"
						PAR_H2OP_LEN,
						(char *)PAR_H2OP);

	api_add_settings_item(ID_H3OP,						// Parâmetro: "Opções do cabeç. 3"
						PAR_H3OP_LEN,
						(char *)PAR_H3OP);

	api_add_settings_item(ID_F1OP,						// Parâmetro: "Opções do rodapé 1"
						PAR_F1OP_LEN,
						(char *)PAR_F1OP);

	api_add_settings_item(ID_F2OP,						// Parâmetro: "Opções do rodapé 2"
						PAR_F2OP_LEN,
						(char *)PAR_F2OP);
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_tkt_req_part2
 * Descrição: 	Monta resposta a requisição "ticket", com a segunda parte
 *				dos parâmetros do ticket
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_handle_tkt_req_part2(void)
{
	api_req_handled=1;									// Marca como requisição tratada.
	api_resp_len=0;										// Inicializa resposta http

	api_add_settings_item(ID_F3OP,						// Parâmetro: "Opções do rodapé 3"
						PAR_F3OP_LEN,
						(char *)PAR_F3OP);

	api_add_settings_item(ID_F4OP,						// Parâmetro: "Opções do rodapé 4"
						PAR_F4OP_LEN,
						(char *)PAR_F4OP);

	api_add_settings_item(ID_H1ME,						// Parâmetro: "Mensag. do cabeç. 1"
						PAR_H1ME_LEN,
						(char *)PAR_H1ME);

	api_add_settings_item(ID_H2ME,						// Parâmetro: "Mensag. do cabeç. 2"
						PAR_H2ME_LEN,
						(char *)PAR_H2ME);

	api_add_settings_item(ID_H3ME,						// Parâmetro: "Mensag. do cabeç. 3"
						PAR_H3ME_LEN,
						(char *)PAR_H3ME);

	api_add_settings_item(ID_F1ME,						// Parâmetro: "Mensag. do rodapé 1"
						PAR_F1ME_LEN,
						(char *)PAR_F1ME);
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_tkt_req_part3
 * Descrição: 	Monta resposta a requisição "ticket", com a terceira parte
 *				dos parâmetros do ticket
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_handle_tkt_req_part3(void)
{
	api_req_handled=1;									// Marca como requisição tratada.
	api_resp_len=0;										// Inicializa resposta http

	api_add_settings_item(ID_F2ME,						// Parâmetro: "Mensag. do rodapé 2"
						PAR_F2ME_LEN,
						(char *)PAR_F2ME);

	api_add_settings_item(ID_F3ME,						// Parâmetro: "Mensag. do rodapé 3"
						PAR_F3ME_LEN,
						(char *)PAR_F3ME);

	api_add_settings_item(ID_F4ME,						// Parâmetro: "Mensag. do rodapé 4"
						PAR_F4ME_LEN,
						(char *)PAR_F4ME);
	api_dec();											// Decrementa último caractere: "Vírgula"

	api_add_json_object_end();							// JSON settings value object end
	api_add_json_object_end();							// JSON settings object end
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_ver_request
 * Descrição: 	Monta resposta a requisição "version"
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_handle_ver_request(void)
{
	api_req_handled=1;									// Marca como requisição tratada.
	api_add_http_headers('j');							// HTTP headers "application/JSON"
	api_add_json_object_start();						// JSON settings object start

	api_add_settings_item(ID_BOOT,						// Parâmetro: boot: "0"=não "1"=sim"
						PAR_BOOT_LEN,
						(char *)PAR_BOOT);

	api_add_json_object_name(PAR_SET_LEN,				// Settings name
						(char *)PAR_SET);

	api_add_json_object_start();						// JSON settings value object start

	api_add_settings_item(ID_MODEL,						// Parâmetro: "Modelo do dispositivo"
						PAR_MODEL_LEN,
						(char *)PAR_MODEL);

	api_add_settings_item(ID_VERS,						// Parâmetro: "Versão do dispositivo"
						PAR_VERS_LEN,
						(char *)PAR_VERS);

	api_add_settings_item(ID_AUTH,						// Parâmetro: "Status autenticação"
						PAR_AUTH_LEN,
						(char *)PAR_AUTH);

	api_add_settings_item(ID_PW,						// Parâmetro: "Password de acesso"
						PAR_PW_LEN,
						(char *)PAR_PW);
	api_dec();											// Decrementa último caractere: "Vírgula"

	api_add_json_object_end();							// JSON settings value object end
	api_add_json_object_end();							// JSON settings object end
}

/*----------------------------------------------------------------------------
 * Funções:		api_handle_real_request
 * Descrição: 	Monta resposta a requisição "real time"
 * Parâmetros: 	Nenhum
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_handle_real_request(void)
{
	api_req_handled=1;									// Marca como requisição tratada.
	api_add_http_headers('j');							// HTTP headers "application/JSON"
	api_add_json_object_start();						// JSON settings object start

	api_add_settings_item(ID_BOOT,						// Parâmetro: boot: "0"=não "1"=sim"
						PAR_BOOT_LEN,
						(char *)PAR_BOOT);

	api_add_json_object_name(PAR_SET_LEN,				// Settings name
						(char *)PAR_SET);

	api_add_json_object_start();						// JSON settings value object start

	api_add_settings_item(ID_FILA1,						// Parâmetro: "Info de fila dpto 1"
						PAR_FILA1_LEN,
						(char *)PAR_FILA1);

	api_add_settings_item(ID_PREV1,						// Parâmetro: "Info de previsão dpto 1"
						PAR_PREV1_LEN,
						(char *)PAR_PREV1);

	api_add_settings_item(ID_FILA2,						// Parâmetro: "Info de fila dpto 2"
						PAR_FILA2_LEN,
						(char *)PAR_FILA2);

	api_add_settings_item(ID_PREV2,						// Parâmetro: "Info de previsão dpto 2"
						PAR_PREV2_LEN,
						(char *)PAR_PREV2);

	api_add_settings_item(ID_FILA3,						// Parâmetro: "Info de fila dpto 3"
						PAR_FILA3_LEN,
						(char *)PAR_FILA3);

	api_add_settings_item(ID_PREV3,						// Parâmetro: "Info de previsão dpto 3"
						PAR_PREV3_LEN,
						(char *)PAR_PREV3);

	api_add_settings_item(ID_FILA4,						// Parâmetro: "Info de fila dpto 4"
						PAR_FILA4_LEN,
						(char *)PAR_FILA4);

	api_add_settings_item(ID_PREV4,						// Parâmetro: "Info de previsão dpto 4"
						PAR_PREV4_LEN,
						(char *)PAR_PREV4);

	api_add_settings_item(ID_FILA5,						// Parâmetro: "Info de fila dpto 5"
						PAR_FILA5_LEN,
						(char *)PAR_FILA5);

	api_add_settings_item(ID_PREV5,						// Parâmetro: "Info de previsão dpto 5"
						PAR_PREV5_LEN,
						(char *)PAR_PREV5);

	api_add_settings_item(ID_FILA6,						// Parâmetro: "Info de fila dpto 6"
						PAR_FILA6_LEN,
						(char *)PAR_FILA6);

	api_add_settings_item(ID_PREV6,						// Parâmetro: "Info de previsão dpto 6"
						PAR_PREV6_LEN,
						(char *)PAR_PREV6);
	api_dec();											// Decrementa último caractere: "Vírgula"

	api_add_json_object_end();							// JSON settings value object end
	api_add_json_object_end();							// JSON settings object end
}

/*----------------------------------------------------------------------------
 * Funções:		api_parse_save_request
 * Descrição: 	Analisa os dados recebidos de método POST para "wifi save"
 * Parâmetros: 	numReq - Número da requisição
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_parse_save_request(unsigned int numReq)
{
	dataReqHttp.request=numReq;							// Guarda o número da requisição
	dataReqHttp.len=0;									// Inicializa resposta http posterior

	api_req_handled=1;									// Marca como requisição tratada.
	dataReqHttp.key=ID_SAVE;
}

/*----------------------------------------------------------------------------
 * Funções:		api_parse_sta_request
 * Descrição: 	Analisa os dados recebidos de método POST para "wifi station"
 * Parâmetros: 	numReq - Número da requisição
 *				len - Comprimento dos dados
 *				*data - Dados recebidos
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_parse_sta_request(unsigned int numReq, unsigned char len, unsigned char *data)
{
	dataReqHttp.request=numReq;							// Guarda o número da requisição
	dataReqHttp.len=0;									// Inicializa resposta http posterior
	dataReqHttp.key=0;									// Inicializa index da informação
	
	if (len == PAR_MODE_LEN && 
		memcmp(data,PAR_MODE,PAR_MODE_LEN)==0)
		dataReqHttp.key=ID_MODE;

	if (len == PAR_SSID_LEN && 
		memcmp(data,PAR_SSID,PAR_SSID_LEN)==0)
		dataReqHttp.key=ID_SSID;

	if (len == PAR_WIFIPW_LEN && 
		memcmp(data,PAR_WIFIPW,PAR_WIFIPW_LEN)==0)
		dataReqHttp.key=ID_WIFIPW;

	if (len == PAR_DHCP_LEN && 
		memcmp(data,PAR_DHCP,PAR_DHCP_LEN)==0)
		dataReqHttp.key=ID_DHCP;

	if (len == PAR_LOCALIP_LEN && 
		memcmp(data,PAR_LOCALIP,PAR_LOCALIP_LEN)==0)
		dataReqHttp.key=ID_IP;

	if (len == PAR_MASK_LEN && 
		memcmp(data,PAR_MASK,PAR_MASK_LEN)==0)
		dataReqHttp.key=ID_MASK;

	if (len == PAR_GATEWAY_LEN && 
		memcmp(data,PAR_GATEWAY,PAR_GATEWAY_LEN)==0)
		dataReqHttp.key=ID_GW;

	if (len == PAR_DNS1_LEN && 
		memcmp(data,PAR_DNS1,PAR_DNS1_LEN)==0)
		dataReqHttp.key=ID_DNS1;

	if (len == PAR_DNS2_LEN && 
		memcmp(data,PAR_DNS2,PAR_DNS2_LEN)==0)
		dataReqHttp.key=ID_DNS2;

	if (dataReqHttp.key) api_req_handled=1;				// Marca como requisição tratada.
}

/*----------------------------------------------------------------------------
 * Funções:		api_parse_mode_request
 * Descrição: 	Analisa os dados recebidos de método POST para "wifi mode"
 * Parâmetros: 	numReq - Número da requisição
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_parse_mode_request(unsigned int numReq)
{
	dataReqHttp.request=numReq;							// Guarda o número da requisição
	dataReqHttp.len=0;									// Inicializa resposta http posterior

	dataReqHttp.key=ID_MODE;
	api_req_handled=1;									// Marca como requisição tratada.
}

/*----------------------------------------------------------------------------
 * Funções:		api_parse_ap_request
 * Descrição: 	Analisa os dados recebidos de método POST para "wifi access point"
 * Parâmetros: 	numReq - Número da requisição
 *				len - Comprimento dos dados
 *				*data - Dados recebidos
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_parse_ap_request(unsigned int numReq, unsigned char len, unsigned char *data)
{
	dataReqHttp.request=numReq;							// Guarda o número da requisição
	dataReqHttp.len=0;									// Inicializa resposta http posterior
	dataReqHttp.key=0;									// Inicializa index da informação
	
	if (len == PAR_SSID_LEN && 
		memcmp(data, PAR_SSID, PAR_SSID_LEN)==0)
		dataReqHttp.key=ID_SSID;

	if (len == PAR_WIFIPW_LEN && 
		memcmp(data, PAR_WIFIPW, PAR_WIFIPW_LEN)==0)
		dataReqHttp.key=ID_WIFIPW;

	if (len == PAR_CHANNEL_LEN && 
		memcmp(data, PAR_CHANNEL, PAR_CHANNEL_LEN)==0)
		dataReqHttp.key=ID_CHANNEL;

	if (len == PAR_SECURITY_LEN && 
		memcmp(data, PAR_SECURITY, PAR_SECURITY_LEN)==0)
		dataReqHttp.key=ID_SECURITY;

	if (len == PAR_LOCALIP_LEN && 
		memcmp(data, PAR_LOCALIP, PAR_LOCALIP_LEN)==0)
		dataReqHttp.key=ID_IP;

	if (len == PAR_MASK_LEN &&
		memcmp(data, PAR_MASK, PAR_MASK_LEN)==0)
		dataReqHttp.key=ID_MASK;

	if (len == PAR_DHCPS_LEN && 
		memcmp(data, PAR_DHCPS, PAR_DHCPS_LEN)==0)
		dataReqHttp.key=ID_DHCPS;

	if (len == PAR_DHCPS_IP_LEN && 
		memcmp(data, PAR_DHCPS_IP, PAR_DHCPS_IP_LEN)==0)
		dataReqHttp.key=ID_DHCPS_IP;

	if (dataReqHttp.key) api_req_handled=1;				// Marca como requisição tratada.
}

/*----------------------------------------------------------------------------
 * Funções:		api_parse_system_request
 * Descrição: 	Analisa os dados recebidos de método POST para "dados gerais"
 * Parâmetros: 	numReq - Número da requisição
 *				len - Comprimento dos dados
 *				*data - Dados recebidos
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_parse_system_request(unsigned int numReq, unsigned char len, unsigned char *data)
{
	dataReqHttp.request=numReq;							// Guarda o número da requisição
	dataReqHttp.len=0;									// Inicializa resposta http posterior
	dataReqHttp.key=0;									// Inicializa index da informação

	if (len == PAR_DRIVE_LEN &&
		memcmp(data, PAR_DRIVE, PAR_DRIVE_LEN)==0)
		dataReqHttp.key=ID_DRIVE;

	else if (len == PAR_DATE_LEN &&
		memcmp(data, PAR_DATE, PAR_DATE_LEN)==0)
		dataReqHttp.key=ID_DATE;

	else if (len == PAR_TIME_LEN &&
		memcmp(data, PAR_TIME, PAR_TIME_LEN)==0)
		dataReqHttp.key=ID_TIME;

	else if (len == PAR_TICKETTEST_LEN &&
		memcmp(data, PAR_TICKETTEST, PAR_TICKETTEST_LEN)==0)
		dataReqHttp.key=ID_TICKETTEST;

	else if (len == PAR_TIMELIVE_LEN &&
		memcmp(data, PAR_TIMELIVE, PAR_TIMELIVE_LEN)==0)
		dataReqHttp.key=ID_TIMELIVE;

	else if (len == PAR_CONTROL_LEN &&
		memcmp(data, PAR_CONTROL, PAR_CONTROL_LEN)==0)
		dataReqHttp.key=ID_CONTROL;

	else if (len == PAR_TIMEON_LEN &&
		memcmp(data, PAR_TIMEON, PAR_TIMEON_LEN)==0)
		dataReqHttp.key=ID_TIMEON;

	else if (len == PAR_TIMEOFF_LEN &&
		memcmp(data, PAR_TIMEOFF, PAR_TIMEOFF_LEN)==0)
		dataReqHttp.key=ID_TIMEOFF;

	else if (len == PAR_D1_LEN &&
		memcmp(data, PAR_D1, PAR_D1_LEN)==0)
		dataReqHttp.key=ID_D1;

	else if (len == PAR_D2_LEN &&
		memcmp(data, PAR_D2, PAR_D2_LEN)==0)
		dataReqHttp.key=ID_D2;

	else if (len == PAR_D3_LEN &&
		memcmp(data, PAR_D3, PAR_D3_LEN)==0)
		dataReqHttp.key=ID_D3;

	else if (len == PAR_D4_LEN &&
		memcmp(data, PAR_D4, PAR_D4_LEN)==0)
		dataReqHttp.key=ID_D4;

	else if (len == PAR_D5_LEN &&
		memcmp(data, PAR_D5, PAR_D5_LEN)==0)
		dataReqHttp.key=ID_D5;

	else if (len == PAR_D6_LEN &&
		memcmp(data, PAR_D6, PAR_D6_LEN)==0)
		dataReqHttp.key=ID_D6;

	else if (len == PAR_D7_LEN &&
		memcmp(data, PAR_D7, PAR_D7_LEN)==0)
		dataReqHttp.key=ID_D7;

	else if (len == PAR_REPORTTIME_LEN &&
		memcmp(data, PAR_REPORTTIME, PAR_REPORTTIME_LEN)==0)
		dataReqHttp.key=ID_REPORTTIME;

	else if (len == PAR_REPORTCSV_LEN &&
		memcmp(data, PAR_REPORTCSV, PAR_REPORTCSV_LEN)==0)
		dataReqHttp.key=ID_REPORTCSV;

	else if (len == PAR_MIGIP_LEN &&
		memcmp(data, PAR_MIGIP, PAR_MIGIP_LEN)==0)
		dataReqHttp.key=ID_MIGIP;

	else if (len == PAR_LINE_LEN &&
		memcmp(data, PAR_LINE, PAR_LINE_LEN)==0)
		dataReqHttp.key=ID_LINE;

	else if (len == PAR_INTERF_LEN &&
		memcmp(data, PAR_INTERF, PAR_INTERF_LEN)==0)
		dataReqHttp.key=ID_INTERF;

	if (dataReqHttp.key) api_req_handled=1;				// Marca como requisição tratada.
}

/*----------------------------------------------------------------------------
 * Funções:		api_parse_upload_request
 * Descrição: 	Analisa os dados recebidos de método POST para "file"
 * Parâmetros: 	numReq - Número da requisição
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_parse_upload_request (unsigned int numReq)
{
	unsigned char *fileName=NULL;

	dataReqHttp.request=numReq;							// Guarda o número da requisição
	dataReqHttp.len=0;									// Inicializa resposta http posterior

	api_req_handled=1;									// Marca como requisição tratada.
	dataReqHttp.key=ID_UPLOAD;

	if (_DEF_DRIVE==_FLASHFS)
	{
		switch (taskFile)
		{
			case 1: fileName="scr1.bmp"; break;
			case 2:	fileName="scr2.bmp"; break;
			case 3: fileName="logo.bmp"; break;
			default: fileName=""; break;
		}
	}
	else if (_DEF_DRIVE==_SDCARD)
	{
		switch (taskFile)
		{
			case 1: fileName="sys\\scr1.bmp"; break;
			case 2:	fileName="sys\\scr2.bmp"; break;
			case 3: fileName="sys\\logo.bmp"; break;
			default: fileName=""; break;
		}
	}
	
    if (*fileName != 0)
	{
		if (_DEF_DRIVE==_SDCARD)
		{
    		if (finit() == 0) 
			{
				f = fopen ((const char *)fileName,"w");
				return;
			}
		}
		else if (_DEF_DRIVE==_FLASHFS)
		{
  			f = fopen ((const char *)fileName,"w");
   			return;
    	}
	}

	f = NULL;

    return;
}

/*----------------------------------------------------------------------------
 * Funções:		api_parse_task_request
 * Descrição: 	Analisa os dados recebidos de método POST para "task File"
 * Parâmetros: 	numReq - Número da requisição
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_parse_task_request (unsigned int numReq, unsigned char len, unsigned char *data)
{
	dataReqHttp.request=numReq;							// Guarda o número da requisição
	dataReqHttp.len=0;									// Inicializa resposta http posterior
	
	api_req_handled=1;				// Marca como requisição tratada.
	dataReqHttp.key=ID_TASK;
}

/*----------------------------------------------------------------------------
 * Funções:		api_parse_seldep_request
 * Descrição: 	Analisa os dados recebidos de método POST para "department select"
 * Parâmetros: 	numReq - Número da requisição
 *				len - Comprimento dos dados
 *				*data - Dados recebidos
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_parse_seldep_request(unsigned int numReq, unsigned char len, unsigned char *data)
{
	dataReqHttp.request=numReq;							// Guarda o número da requisição
	dataReqHttp.len=0;									// Inicializa resposta http posterior
	dataReqHttp.key=0;									// Inicializa index da informação

	if (len == PAR_DNUM_LEN &&
		memcmp(data, PAR_DNUM, PAR_DNUM_LEN)==0)
		dataReqHttp.key=ID_DNUM;

	if (dataReqHttp.key) api_req_handled=1;				// Marca como requisição tratada.
}

/*----------------------------------------------------------------------------
 * Funções:		api_parse_department_request
 * Descrição: 	Analisa os dados recebidos de método POST para "department"
 * Parâmetros: 	numReq - Número da requisição
 *				len - Comprimento dos dados
 *				*data - Dados recebidos
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_parse_department_request(unsigned int numReq, unsigned char len, unsigned char *data)
{
	dataReqHttp.request=numReq;							// Guarda o número da requisição
	dataReqHttp.len=0;									// Inicializa resposta http posterior
	dataReqHttp.key=0;									// Inicializa index da informação

	if (len == PAR_DENAB_LEN &&
		memcmp(data, PAR_DENAB, PAR_DENAB_LEN)==0)
		dataReqHttp.key=ID_DENAB;

	else if (len == PAR_DNAME_LEN &&
		memcmp(data, PAR_DNAME, PAR_DNAME_LEN)==0)
		dataReqHttp.key=ID_DNAME;

	else if (len == PAR_DALPH_LEN &&
		memcmp(data, PAR_DALPH, PAR_DALPH_LEN)==0)
		dataReqHttp.key=ID_DALPH;

	else if (len == PAR_DRANG1_LEN &&
		memcmp(data, PAR_DRANG1, PAR_DRANG1_LEN)==0)
		dataReqHttp.key=ID_DRANG1;

	else if (len == PAR_DRANG2_LEN &&
		memcmp(data, PAR_DRANG2, PAR_DRANG2_LEN)==0)
		dataReqHttp.key=ID_DRANG2;

	else if (len == PAR_DCLOS1_LEN &&
		memcmp(data, PAR_DCLOS1, PAR_DCLOS1_LEN)==0)
		dataReqHttp.key=ID_DCLOS1;

	else if (len == PAR_DCLOS2_LEN &&
		memcmp(data, PAR_DCLOS2, PAR_DCLOS2_LEN)==0)
		dataReqHttp.key=ID_DCLOS2;

	else if (len == PAR_DCALL1_LEN &&
		memcmp(data, PAR_DCALL1, PAR_DCALL1_LEN)==0)
		dataReqHttp.key=ID_DCALL1;

	else if (len == PAR_DCALL2_LEN &&
		memcmp(data, PAR_DCALL2, PAR_DCALL2_LEN)==0)
		dataReqHttp.key=ID_DCALL2;

	else if (len == PAR_DHLP1_LEN &&
		memcmp(data, PAR_DHLP1, PAR_DHLP1_LEN)==0)
		dataReqHttp.key=ID_DHLP1;

	else if (len == PAR_DHLP2_LEN &&
		memcmp(data, PAR_DHLP2, PAR_DHLP2_LEN)==0)
		dataReqHttp.key=ID_DHLP2;

	else if (len == PAR_DHLP3_LEN &&
		memcmp(data, PAR_DHLP3, PAR_DHLP3_LEN)==0)
		dataReqHttp.key=ID_DHLP3;

	else if (len == PAR_DHLP4_LEN &&
		memcmp(data, PAR_DHLP4, PAR_DHLP4_LEN)==0)
		dataReqHttp.key=ID_DHLP4;

	else if (len == PAR_DHLP5_LEN &&
		memcmp(data, PAR_DHLP5, PAR_DHLP5_LEN)==0)
		dataReqHttp.key=ID_DHLP5;

	else if (len == PAR_DIPD_LEN &&
		memcmp(data, PAR_DIPD, PAR_DIPD_LEN)==0)
		dataReqHttp.key=ID_DIPD;
		
	else if (len == PAR_DITFD_LEN &&
		memcmp(data, PAR_DITFD, PAR_DITFD_LEN)==0)
		dataReqHttp.key=ID_DITFD;

	else if (len == PAR_DIPT_LEN &&
		memcmp(data, PAR_DIPT, PAR_DIPT_LEN)==0)
		dataReqHttp.key=ID_DIPT;

	else if (len == PAR_DITFT_LEN &&
		memcmp(data, PAR_DITFT, PAR_DITFT_LEN)==0)
		dataReqHttp.key=ID_DITFT;

	else if (len == PAR_ENATOP_LEN &&
		memcmp(data, PAR_ENATOP, PAR_ENATOP_LEN)==0)
		dataReqHttp.key=ID_ENATOP;

	else if (len == PAR_DLIND_LEN &&
		memcmp(data, PAR_DLIND, PAR_DLIND_LEN)==0)
		dataReqHttp.key=ID_DLIND;

	else if (len == PAR_DPRFD_LEN &&
		memcmp(data, PAR_DPRFD, PAR_DPRFD_LEN)==0)
		dataReqHttp.key=ID_DPRFD;

	else if (len == PAR_DU01_LEN &&
		memcmp(data, PAR_DU01, PAR_DU01_LEN)==0)
		dataReqHttp.key=ID_DU01;

	else if (len == PAR_DU02_LEN &&
		memcmp(data, PAR_DU02, PAR_DU02_LEN)==0)
		dataReqHttp.key=ID_DU02;

	else if (len == PAR_DU03_LEN &&
		memcmp(data, PAR_DU03, PAR_DU03_LEN)==0)
		dataReqHttp.key=ID_DU03;

	else if (len == PAR_DU04_LEN &&
		memcmp(data, PAR_DU04, PAR_DU04_LEN)==0)
		dataReqHttp.key=ID_DU04;

	else if (len == PAR_DU05_LEN &&
		memcmp(data, PAR_DU05, PAR_DU05_LEN)==0)
		dataReqHttp.key=ID_DU05;

	else if (len == PAR_DU06_LEN &&
		memcmp(data, PAR_DU06, PAR_DU06_LEN)==0)
		dataReqHttp.key=ID_DU06;

	else if (len == PAR_DU07_LEN &&
		memcmp(data, PAR_DU07, PAR_DU07_LEN)==0)
		dataReqHttp.key=ID_DU07;

	else if (len == PAR_DU08_LEN &&
		memcmp(data, PAR_DU08, PAR_DU08_LEN)==0)
		dataReqHttp.key=ID_DU08;

	else if (len == PAR_DU09_LEN &&
		memcmp(data, PAR_DU09, PAR_DU09_LEN)==0)
		dataReqHttp.key=ID_DU09;

	else if (len == PAR_DU10_LEN &&
		memcmp(data, PAR_DU10, PAR_DU10_LEN)==0)
		dataReqHttp.key=ID_DU10;

	else if (len == PAR_DU11_LEN &&
		memcmp(data, PAR_DU11, PAR_DU11_LEN)==0)
		dataReqHttp.key=ID_DU11;

	else if (len == PAR_DU12_LEN &&
		memcmp(data, PAR_DU12, PAR_DU12_LEN)==0)
		dataReqHttp.key=ID_DU12;

	else if (len == PAR_DU13_LEN &&
		memcmp(data, PAR_DU13, PAR_DU13_LEN)==0)
		dataReqHttp.key=ID_DU13;

	else if (len == PAR_DU14_LEN &&
		memcmp(data, PAR_DU14, PAR_DU14_LEN)==0)
		dataReqHttp.key=ID_DU14;

	else if (len == PAR_DU15_LEN &&
		memcmp(data, PAR_DU15, PAR_DU15_LEN)==0)
		dataReqHttp.key=ID_DU15;

	else if (len == PAR_DU16_LEN &&
		memcmp(data, PAR_DU16, PAR_DU16_LEN)==0)
		dataReqHttp.key=ID_DU16;

	else if (len == PAR_DU17_LEN &&
		memcmp(data, PAR_DU17, PAR_DU17_LEN)==0)
		dataReqHttp.key=ID_DU17;

	else if (len == PAR_DU18_LEN &&
		memcmp(data, PAR_DU18, PAR_DU18_LEN)==0)
		dataReqHttp.key=ID_DU18;

	else if (len == PAR_DU19_LEN &&
		memcmp(data, PAR_DU19, PAR_DU19_LEN)==0)
		dataReqHttp.key=ID_DU19;

	else if (len == PAR_DU20_LEN &&
		memcmp(data, PAR_DU20, PAR_DU20_LEN)==0)
		dataReqHttp.key=ID_DU20;

	else if (len == PAR_DU21_LEN &&
		memcmp(data, PAR_DU21, PAR_DU21_LEN)==0)
		dataReqHttp.key=ID_DU21;

	else if (len == PAR_DU22_LEN &&
		memcmp(data, PAR_DU22, PAR_DU22_LEN)==0)
		dataReqHttp.key=ID_DU22;

	else if (len == PAR_DU23_LEN &&
		memcmp(data, PAR_DU23, PAR_DU23_LEN)==0)
		dataReqHttp.key=ID_DU23;

	else if (len == PAR_DU24_LEN &&
		memcmp(data, PAR_DU24, PAR_DU24_LEN)==0)
		dataReqHttp.key=ID_DU24;

	else if (len == PAR_DU25_LEN &&
		memcmp(data, PAR_DU25, PAR_DU25_LEN)==0)
		dataReqHttp.key=ID_DU25;

	else if (len == PAR_DU26_LEN &&
		memcmp(data, PAR_DU26, PAR_DU26_LEN)==0)
		dataReqHttp.key=ID_DU26;

	else if (len == PAR_DU27_LEN &&
		memcmp(data, PAR_DU27, PAR_DU27_LEN)==0)
		dataReqHttp.key=ID_DU27;

	else if (len == PAR_DU28_LEN &&
		memcmp(data, PAR_DU28, PAR_DU28_LEN)==0)
		dataReqHttp.key=ID_DU28;

	else if (len == PAR_DU29_LEN &&
		memcmp(data, PAR_DU29, PAR_DU29_LEN)==0)
		dataReqHttp.key=ID_DU29;

	else if (len == PAR_DU30_LEN &&
		memcmp(data, PAR_DU30, PAR_DU30_LEN)==0)
		dataReqHttp.key=ID_DU30;

	else if (len == PAR_DU31_LEN &&
		memcmp(data, PAR_DU31, PAR_DU31_LEN)==0)
		dataReqHttp.key=ID_DU31;

	else if (len == PAR_DU32_LEN &&
		memcmp(data, PAR_DU32, PAR_DU32_LEN)==0)
		dataReqHttp.key=ID_DU32;

	else if (len == PAR_DU33_LEN &&
		memcmp(data, PAR_DU33, PAR_DU33_LEN)==0)
		dataReqHttp.key=ID_DU33;

	else if (len == PAR_DU34_LEN &&
		memcmp(data, PAR_DU34, PAR_DU34_LEN)==0)
		dataReqHttp.key=ID_DU34;

	else if (len == PAR_DU35_LEN &&
		memcmp(data, PAR_DU35, PAR_DU35_LEN)==0)
		dataReqHttp.key=ID_DU35;

	else if (len == PAR_DU36_LEN &&
		memcmp(data, PAR_DU36, PAR_DU36_LEN)==0)
		dataReqHttp.key=ID_DU36;

	else if (len == PAR_DU37_LEN &&
		memcmp(data, PAR_DU37, PAR_DU37_LEN)==0)
		dataReqHttp.key=ID_DU37;

	else if (len == PAR_DU38_LEN &&
		memcmp(data, PAR_DU38, PAR_DU38_LEN)==0)
		dataReqHttp.key=ID_DU38;

	else if (len == PAR_DU39_LEN &&
		memcmp(data, PAR_DU39, PAR_DU39_LEN)==0)
		dataReqHttp.key=ID_DU39;

	else if (len == PAR_DU40_LEN &&
		memcmp(data, PAR_DU40, PAR_DU40_LEN)==0)
		dataReqHttp.key=ID_DU40;

	else if (len == PAR_DU41_LEN &&
		memcmp(data, PAR_DU41, PAR_DU41_LEN)==0)
		dataReqHttp.key=ID_DU41;

	else if (len == PAR_DU42_LEN &&
		memcmp(data, PAR_DU42, PAR_DU42_LEN)==0)
		dataReqHttp.key=ID_DU42;

	else if (len == PAR_DU43_LEN &&
		memcmp(data, PAR_DU43, PAR_DU43_LEN)==0)
		dataReqHttp.key=ID_DU43;

	else if (len == PAR_DU44_LEN &&
		memcmp(data, PAR_DU44, PAR_DU44_LEN)==0)
		dataReqHttp.key=ID_DU44;

	else if (len == PAR_DU45_LEN &&
		memcmp(data, PAR_DU45, PAR_DU45_LEN)==0)
		dataReqHttp.key=ID_DU45;

	else if (len == PAR_DU46_LEN &&
		memcmp(data, PAR_DU46, PAR_DU46_LEN)==0)
		dataReqHttp.key=ID_DU46;

	else if (len == PAR_DU47_LEN &&
		memcmp(data, PAR_DU47, PAR_DU47_LEN)==0)
		dataReqHttp.key=ID_DU47;

	else if (len == PAR_DU48_LEN &&
		memcmp(data, PAR_DU48, PAR_DU48_LEN)==0)
		dataReqHttp.key=ID_DU48;

	else if (len == PAR_DU49_LEN &&
		memcmp(data, PAR_DU49, PAR_DU49_LEN)==0)
		dataReqHttp.key=ID_DU49;

	else if (len == PAR_DU50_LEN &&
		memcmp(data, PAR_DU50, PAR_DU50_LEN)==0)
		dataReqHttp.key=ID_DU50;

	else if (len == PAR_DU51_LEN &&
		memcmp(data, PAR_DU51, PAR_DU51_LEN)==0)
		dataReqHttp.key=ID_DU51;

	else if (len == PAR_DU52_LEN &&
		memcmp(data, PAR_DU52, PAR_DU52_LEN)==0)
		dataReqHttp.key=ID_DU52;

	else if (len == PAR_DU53_LEN &&
		memcmp(data, PAR_DU53, PAR_DU53_LEN)==0)
		dataReqHttp.key=ID_DU53;

	else if (len == PAR_DU54_LEN &&
		memcmp(data, PAR_DU54, PAR_DU54_LEN)==0)
		dataReqHttp.key=ID_DU54;

	else if (len == PAR_DU55_LEN &&
		memcmp(data, PAR_DU55, PAR_DU55_LEN)==0)
		dataReqHttp.key=ID_DU55;

	else if (len == PAR_DU56_LEN &&
		memcmp(data, PAR_DU56, PAR_DU56_LEN)==0)
		dataReqHttp.key=ID_DU56;

	else if (len == PAR_DU57_LEN &&
		memcmp(data, PAR_DU57, PAR_DU57_LEN)==0)
		dataReqHttp.key=ID_DU57;

	else if (len == PAR_DU58_LEN &&
		memcmp(data, PAR_DU58, PAR_DU58_LEN)==0)
		dataReqHttp.key=ID_DU58;

	else if (len == PAR_DU59_LEN &&
		memcmp(data, PAR_DU59, PAR_DU59_LEN)==0)
		dataReqHttp.key=ID_DU59;

	else if (len == PAR_DU60_LEN &&
		memcmp(data, PAR_DU60, PAR_DU60_LEN)==0)
		dataReqHttp.key=ID_DU60;

	else if (len == PAR_DU61_LEN &&
		memcmp(data, PAR_DU61, PAR_DU61_LEN)==0)
		dataReqHttp.key=ID_DU61;

	else if (len == PAR_DU62_LEN &&
		memcmp(data, PAR_DU62, PAR_DU62_LEN)==0)
		dataReqHttp.key=ID_DU62;

	else if (len == PAR_DU63_LEN &&
		memcmp(data, PAR_DU63, PAR_DU63_LEN)==0)
		dataReqHttp.key=ID_DU63;

	else if (len == PAR_DU64_LEN &&
		memcmp(data, PAR_DU64, PAR_DU64_LEN)==0)
		dataReqHttp.key=ID_DU64;

	else if (len == PAR_DU65_LEN &&
		memcmp(data, PAR_DU65, PAR_DU65_LEN)==0)
		dataReqHttp.key=ID_DU65;

	else if (len == PAR_DU66_LEN &&
		memcmp(data, PAR_DU66, PAR_DU66_LEN)==0)
		dataReqHttp.key=ID_DU66;

	else if (len == PAR_DU67_LEN &&
		memcmp(data, PAR_DU67, PAR_DU67_LEN)==0)
		dataReqHttp.key=ID_DU67;

	else if (len == PAR_DU68_LEN &&
		memcmp(data, PAR_DU68, PAR_DU68_LEN)==0)
		dataReqHttp.key=ID_DU68;

	else if (len == PAR_DU69_LEN &&
		memcmp(data, PAR_DU69, PAR_DU69_LEN)==0)
		dataReqHttp.key=ID_DU69;

	else if (len == PAR_DU70_LEN &&
		memcmp(data, PAR_DU70, PAR_DU70_LEN)==0)
		dataReqHttp.key=ID_DU70;

	else if (len == PAR_DU71_LEN &&
		memcmp(data, PAR_DU71, PAR_DU71_LEN)==0)
		dataReqHttp.key=ID_DU71;

	else if (len == PAR_DU72_LEN &&
		memcmp(data, PAR_DU72, PAR_DU72_LEN)==0)
		dataReqHttp.key=ID_DU72;

	else if (len == PAR_DU73_LEN &&
		memcmp(data, PAR_DU73, PAR_DU73_LEN)==0)
		dataReqHttp.key=ID_DU73;

	else if (len == PAR_DU74_LEN &&
		memcmp(data, PAR_DU74, PAR_DU74_LEN)==0)
		dataReqHttp.key=ID_DU74;

	else if (len == PAR_DU75_LEN &&
		memcmp(data, PAR_DU75, PAR_DU75_LEN)==0)
		dataReqHttp.key=ID_DU75;

	else if (len == PAR_DU76_LEN &&
		memcmp(data, PAR_DU76, PAR_DU76_LEN)==0)
		dataReqHttp.key=ID_DU76;

	else if (len == PAR_DU77_LEN &&
		memcmp(data, PAR_DU77, PAR_DU77_LEN)==0)
		dataReqHttp.key=ID_DU77;

	else if (len == PAR_DU78_LEN &&
		memcmp(data, PAR_DU78, PAR_DU78_LEN)==0)
		dataReqHttp.key=ID_DU78;

	else if (len == PAR_DU79_LEN &&
		memcmp(data, PAR_DU79, PAR_DU79_LEN)==0)
		dataReqHttp.key=ID_DU79;

	else if (len == PAR_DU80_LEN &&
		memcmp(data, PAR_DU80, PAR_DU80_LEN)==0)
		dataReqHttp.key=ID_DU80;

	else if (len == PAR_DU81_LEN &&
		memcmp(data, PAR_DU81, PAR_DU81_LEN)==0)
		dataReqHttp.key=ID_DU81;

	else if (len == PAR_DU82_LEN &&
		memcmp(data, PAR_DU82, PAR_DU82_LEN)==0)
		dataReqHttp.key=ID_DU82;

	else if (len == PAR_DU83_LEN &&
		memcmp(data, PAR_DU83, PAR_DU83_LEN)==0)
		dataReqHttp.key=ID_DU83;

	else if (len == PAR_DU84_LEN &&
		memcmp(data, PAR_DU84, PAR_DU84_LEN)==0)
		dataReqHttp.key=ID_DU84;

	else if (len == PAR_DU85_LEN &&
		memcmp(data, PAR_DU85, PAR_DU85_LEN)==0)
		dataReqHttp.key=ID_DU85;

	else if (len == PAR_DU86_LEN &&
		memcmp(data, PAR_DU86, PAR_DU86_LEN)==0)
		dataReqHttp.key=ID_DU86;

	else if (len == PAR_DU87_LEN &&
		memcmp(data, PAR_DU87, PAR_DU87_LEN)==0)
		dataReqHttp.key=ID_DU87;

	else if (len == PAR_DU88_LEN &&
		memcmp(data, PAR_DU88, PAR_DU88_LEN)==0)
		dataReqHttp.key=ID_DU88;

	else if (len == PAR_DU89_LEN &&
		memcmp(data, PAR_DU89, PAR_DU89_LEN)==0)
		dataReqHttp.key=ID_DU89;

	else if (len == PAR_DU90_LEN &&
		memcmp(data, PAR_DU90, PAR_DU90_LEN)==0)
		dataReqHttp.key=ID_DU90;

	else if (len == PAR_DU91_LEN &&
		memcmp(data, PAR_DU91, PAR_DU91_LEN)==0)
		dataReqHttp.key=ID_DU91;

	else if (len == PAR_DU92_LEN &&
		memcmp(data, PAR_DU92, PAR_DU92_LEN)==0)
		dataReqHttp.key=ID_DU92;

	else if (len == PAR_DU93_LEN &&
		memcmp(data, PAR_DU93, PAR_DU93_LEN)==0)
		dataReqHttp.key=ID_DU93;

	else if (len == PAR_DU94_LEN &&
		memcmp(data, PAR_DU94, PAR_DU94_LEN)==0)
		dataReqHttp.key=ID_DU94;

	else if (len == PAR_DU95_LEN &&
		memcmp(data, PAR_DU95, PAR_DU95_LEN)==0)
		dataReqHttp.key=ID_DU95;

	else if (len == PAR_DU96_LEN &&
		memcmp(data, PAR_DU96, PAR_DU96_LEN)==0)
		dataReqHttp.key=ID_DU96;

	else if (len == PAR_DU97_LEN &&
		memcmp(data, PAR_DU97, PAR_DU97_LEN)==0)
		dataReqHttp.key=ID_DU97;

	else if (len == PAR_DU98_LEN &&
		memcmp(data, PAR_DU98, PAR_DU98_LEN)==0)
		dataReqHttp.key=ID_DU98;

	else if (len == PAR_DU99_LEN &&
		memcmp(data, PAR_DU99, PAR_DU99_LEN)==0)
		dataReqHttp.key=ID_DU99;

	if (dataReqHttp.key) api_req_handled=1;				// Marca como requisição tratada.
}

/*----------------------------------------------------------------------------
 * Funções:		api_parse_ubox_request
 * Descrição: 	Analisa os dados recebidos de método POST para "user box"
 * Parâmetros: 	numReq - Número da requisição
 *				len - Comprimento dos dados
 *				*data - Dados recebidos
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_parse_ubox_request(unsigned int numReq, unsigned char len, unsigned char *data)
{
	dataReqHttp.request=numReq;							// Guarda o número da requisição
	dataReqHttp.len=0;									// Inicializa resposta http posterior
	dataReqHttp.key=0;									// Inicializa index da informação

	if (len == PAR_USER_LEN &&
		memcmp(data, PAR_USER, PAR_USER_LEN)==0)
		dataReqHttp.key=ID_USER;

	else if (len == PAR_BOX_LEN &&
		memcmp(data, PAR_BOX, PAR_BOX_LEN)==0)
		dataReqHttp.key=ID_BOX;

	if (dataReqHttp.key) api_req_handled=1;				// Marca como requisição tratada.
}

/*----------------------------------------------------------------------------
 * Funções:		api_parse_tkt_request
 * Descrição: 	Analisa os dados recebidos de método POST para "ticket"
 * Parâmetros: 	numReq - Número da requisição
 *				len - Comprimento dos dados
 *				*data - Dados recebidos
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_parse_tkt_request(unsigned int numReq, unsigned char len, unsigned char *data)
{
	dataReqHttp.request=numReq;							// Guarda o número da requisição
	dataReqHttp.len=0;									// Inicializa resposta http posterior
	dataReqHttp.key=0;									// Inicializa index da informação

	if (len == PAR_LG_LEN &&
		memcmp(data, PAR_LG, PAR_LG_LEN)==0)
		dataReqHttp.key=ID_LG;

	if (len == PAR_DLEN_LEN &&
		memcmp(data, PAR_DLEN, PAR_DLEN_LEN)==0)
		dataReqHttp.key=ID_DLEN;

	if (len == PAR_DLNUM_LEN &&
		memcmp(data, PAR_DLNUM, PAR_DLNUM_LEN)==0)
		dataReqHttp.key=ID_DLNUM;

	if (len == PAR_DHNUM_LEN &&
		memcmp(data, PAR_DHNUM, PAR_DHNUM_LEN)==0)
		dataReqHttp.key=ID_DHNUM;

	if (len == PAR_CUT_LEN &&
		memcmp(data, PAR_CUT, PAR_CUT_LEN)==0)
		dataReqHttp.key=ID_CUT;

	if (len == PAR_VIA_LEN &&
		memcmp(data, PAR_VIA, PAR_VIA_LEN)==0)
		dataReqHttp.key=ID_VIA;

	if (len == PAR_H1OP_LEN &&
		memcmp(data, PAR_H1OP, PAR_H1OP_LEN)==0)
		dataReqHttp.key=ID_H1OP;

	if (len == PAR_H2OP_LEN &&
		memcmp(data, PAR_H2OP, PAR_H2OP_LEN)==0)
		dataReqHttp.key=ID_H2OP;

	if (len == PAR_H3OP_LEN &&
		memcmp(data, PAR_H3OP, PAR_H3OP_LEN)==0)
		dataReqHttp.key=ID_H3OP;

	if (len == PAR_F1OP_LEN &&
		memcmp(data, PAR_F1OP, PAR_F1OP_LEN)==0)
		dataReqHttp.key=ID_F1OP;

	if (len == PAR_F2OP_LEN &&
		memcmp(data, PAR_F2OP, PAR_F2OP_LEN)==0)
		dataReqHttp.key=ID_F2OP;

	if (len == PAR_F3OP_LEN &&
		memcmp(data, PAR_F3OP, PAR_F3OP_LEN)==0)
		dataReqHttp.key=ID_F3OP;

	if (len == PAR_F4OP_LEN &&
		memcmp(data, PAR_F4OP, PAR_F4OP_LEN)==0)
		dataReqHttp.key=ID_F4OP;

	if (len == PAR_H1ME_LEN &&
		memcmp(data, PAR_H1ME, PAR_H1ME_LEN)==0)
		dataReqHttp.key=ID_H1ME;

	if (len == PAR_H2ME_LEN &&
		memcmp(data, PAR_H2ME, PAR_H2ME_LEN)==0)
		dataReqHttp.key=ID_H2ME;

	if (len == PAR_H3ME_LEN &&
		memcmp(data, PAR_H3ME, PAR_H3ME_LEN)==0)
		dataReqHttp.key=ID_H3ME;

	if (len == PAR_F1ME_LEN &&
		memcmp(data, PAR_F1ME, PAR_F1ME_LEN)==0)
		dataReqHttp.key=ID_F1ME;

	if (len == PAR_F2ME_LEN &&
		memcmp(data, PAR_F2ME, PAR_F2ME_LEN)==0)
		dataReqHttp.key=ID_F2ME;

	if (len == PAR_F3ME_LEN &&
		memcmp(data, PAR_F3ME, PAR_F3ME_LEN)==0)
		dataReqHttp.key=ID_F3ME;

	if (len == PAR_F4ME_LEN &&
		memcmp(data, PAR_F4ME, PAR_F4ME_LEN)==0)
		dataReqHttp.key=ID_F4ME;

	if (dataReqHttp.key) api_req_handled=1;				// Marca como requisição tratada.
}

/*----------------------------------------------------------------------------
 * Funções:		api_parse_ver_request
 * Descrição: 	Analisa os dados recebidos de método POST para "version"
 * Parâmetros: 	numReq - Número da requisição
 *				len - Comprimento dos dados
 *				*data - Dados recebidos
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_parse_ver_request(unsigned int numReq, unsigned char len, unsigned char *data)
{
	dataReqHttp.request=numReq;							// Guarda o número da requisição
	dataReqHttp.len=0;									// Inicializa resposta http posterior
	dataReqHttp.key=0;									// Inicializa index da informação

	if (len == PAR_MODEL_LEN &&
		memcmp(data, PAR_MODEL, PAR_MODEL_LEN)==0)
		dataReqHttp.key=ID_MODEL;

	if (len == PAR_VERS_LEN &&
		memcmp(data, PAR_VERS, PAR_VERS_LEN)==0)
		dataReqHttp.key=ID_VERS;

	if (len == PAR_AUTH_LEN &&
		memcmp(data, PAR_AUTH, PAR_AUTH_LEN)==0)
		dataReqHttp.key=ID_AUTH;

	if (len == PAR_PW_LEN &&
		memcmp(data, PAR_PW, PAR_PW_LEN)==0)
		dataReqHttp.key=ID_PW;

	if (dataReqHttp.key) api_req_handled=1;				// Marca como requisição tratada.
}

/*----------------------------------------------------------------------------
 * Funções:		api_parse_format_request
 * Descrição: 	Analisa os dados recebidos de método POST para "format"
 * Parâmetros: 	numReq - Número da requisição
 *				len - Comprimento dos dados
 *				*data - Dados recebidos
 * Retorno: 	Nenhum
 -----------------------------------------------------------------------------*/
void api_parse_format_request(unsigned int numReq, unsigned char len, unsigned char *data)
{
	dataReqHttp.request=numReq;							// Guarda o número da requisição
	dataReqHttp.len=0;									// Inicializa resposta http posterior
	dataReqHttp.key=0;									// Inicializa index da informação

	if (len == PAR_FORMAT_LEN &&
		memcmp(data, PAR_FORMAT, PAR_FORMAT_LEN)==0)
		dataReqHttp.key=ID_FORMAT;

	if (len == PAR_INIT_LEN &&
		memcmp(data, PAR_INIT, PAR_INIT_LEN)==0)
		dataReqHttp.key=ID_INIT;

	if (dataReqHttp.key) api_req_handled=1;				// Marca como requisição tratada.
}
 
/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

