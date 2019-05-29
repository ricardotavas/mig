t <html><head><title>Personalizar</title>
t <link rel="icon" type="image/bmp" href="W.bmp" sizes="16x16">
t <script language=JavaScript>
t function changePage(){
t document.location.href="uploadbmp.cgi";
t }
t </script>
t </head>
#
i pg_header.inc
t <h2 align=center><br>Personalizar</h2>
#
t <form action=design.cgi method=post name=cgi>
t <input type=hidden value="design" name=pg>
#
t <p><font size="3"><br>Permite personalizar o logotipo no ticket entregue ao cliente. 
t Imagens para logotipo devem ter no m&#225;ximo 128x64 pixels. O formato das imagens 
t deve ser: bitmap monocrom&#225;tico.</font></p>
#
t <p><font size="3"><br>Escolha um dos itens listados abaixo para personalizar:</font></p>
#
c v C <font face="verdana"><input type=radio name=tsk value=3 OnChange="submit();" %s>
t &#32;Escolher um logotipo para o ticket (arquivo .bmp)</font><br>
#
t <p><font face="verdana"><br><br>Clique abaixo para escolher um arquivo:</font></p>
t <input type=button value="Continuar" onclick="changePage()">
#
t </form>
#
i pg_footer.inc
. End of script must be closed with period.
