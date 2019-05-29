t <html><head><title>Upload de arquivos</title>
t <link rel="icon" type="image/bmp" href="W.bmp" sizes="16x16">
t </head>
i pg_header.inc
t <h2 align=center><br>Upload de arquivos</h2>
#
t <form action=index.htm method=POST enctype="multipart/form-data" name=uploadbmp>
#
t <p><font face="verdana"><br><br>Passo 1: Clique abaixo para escolher o arquivo bitmap:</font></p>
t <input type=file value="Procurar" name=file size="50" maxlength="1000">
#
t <p><font face="verdana"><br><br>Passo 2: Clique abaixo para iniciar a transfer&#234;ncia:</font></p>
t <input type=button value="Enviar" OnClick="this.form.submit()">
#
t </form>
#
i pg_footer.inc
. End of script must be closed with period.
