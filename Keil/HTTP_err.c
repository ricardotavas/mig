/*----------------------------------------------------------------------------
 *      R T L  -  T C P N E T
 *----------------------------------------------------------------------------
 *      Name:    HTTP_ERR.C
 *      Purpose: HTTP Server Error Page Definitions
 *      Rev.:    V4.00
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2009 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>

/* Note: Keep HTTP Error pages small sized. */

/* HTTP Error 401 - Unauthorized access. */
const U8 http_err_401[] = {
        "<h2>HTTP 1.1 Erro 401. Acesso n&#227;o autorizado</h2>"
        "Voc&#234; n&#227;o esta autorizado a acessar este servidor."};

/* HTTP Error 404 - File not Found */
const U8 http_err_404[] = {
        "<h2>HTTP 1.1 Erro 404. Arquivo n&#227;o encontrado.</h2>"
        "O URL solicitado n&#227;o foi encontrado neste servidor."};

/* HTTP Error 501 - Not Implemented */
const U8 http_err_501[] = {
        "<h2>HTTP 1.1 Erro 501. N&#227;o implementado</h2>"
        "O m&#233;todo solicitado n&#227;o &#233; suportado."};

/* Common HTTP Error Page Header */
const U8 http_err_head[] = {
        "<head><title>WYMA Tecnologia</title></head>"
        "<body>"};

/* Common HTTP Error Page Footer */
const U8 http_err_foot[] = {
        "<hr><br>"
        "<i>IMPRESSORA WYMA V1.10, 2017<br>"
        "<a href=http://www.wyma.com> www.wyma.com</a>"
        " - Wyma Tecnologia</i>"
        "</body>"};




/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
