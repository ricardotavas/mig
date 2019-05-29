t <html><head><title>Configura&#231;&#227;o do ticket</title>
t <link rel="icon" type="image/bmp" href="W.bmp" sizes="16x16">
t <script language=JavaScript>
t function changeConfirm(f){
t  if(!confirm('Confirma gravacao das alteracoes ?')) return;
t  f.submit();
t }
t function initdisable (){
t disablecontrol(cgi.h1op.value,'h1op');
t disablecontrol(cgi.h2op.value,'h2op');
t disablecontrol(cgi.h3op.value,'h3op');
t disablecontrol(cgi.f1op.value,'f1op');
t disablecontrol(cgi.f2op.value,'f2op');
t disablecontrol(cgi.f3op.value,'f3op');
t disablecontrol(cgi.f4op.value,'f4op');
t }
t function disablecontrol(obj,element) {
t if (element=='h1op'){
t 	if(obj==0 || obj==1){ 
t 		cgi.elements['h1me'].disabled = false;}
t 	else if (obj==2 || obj==3 || obj==4 || obj==5){ 
t 		cgi.elements['h1me'].disabled = true;}}
t else if (element=='h2op'){
t 	if(obj==0 || obj==1){ 
t 		cgi.elements['h2me'].disabled = false;}
t 	else if (obj==2 || obj==3 || obj==4 || obj==5){ 
t 		cgi.elements['h2me'].disabled = true;}}
t else if (element=='h3op'){
t 	if(obj==0 || obj==1){ 
t 		cgi.elements['h3me'].disabled = false;}
t 	else if (obj==2 || obj==3 || obj==4 || obj==5){ 
t 		cgi.elements['h3me'].disabled = true;}}
t else if (element=='f1op'){
t 	if(obj==0 || obj==1){ 
t 		cgi.elements['f1me'].disabled = false;}
t 	else if (obj==2 || obj==3 || obj==4 || obj==5){ 
t 		cgi.elements['f1me'].disabled = true;}}
t else if (element=='f2op'){
t 	if(obj==0 || obj==1){ 
t 		cgi.elements['f2me'].disabled = false;}
t 	else if (obj==2 || obj==3 || obj==4 || obj==5){ 
t 		cgi.elements['f2me'].disabled = true;}}
t else if (element=='f3op'){
t 	if(obj==0 || obj==1){ 
t 		cgi.elements['f3me'].disabled = false;}
t 	else if (obj==2 || obj==3 || obj==4 || obj==5){ 
t 		cgi.elements['f3me'].disabled = true;}}
t else if (element=='f4op'){
t 	if(obj==0 || obj==1){ 
t 		cgi.elements['f4me'].disabled = false;}
t 	else if (obj==2 || obj==3 || obj==4 || obj==5){ 
t 		cgi.elements['f4me'].disabled = true;}}
t }
t </script></head>
#
#
#
#
t <body onload="initdisable();">
i pg_header.inc
t <h2 align=center><br>Configura&#231;&#227;o do ticket</h2>
t <form action=index.htm method=post name=cgi>
t <input type=hidden value="ticket" name=pg>
#
# ----------------------------------------------- #
# -----------  PARÂMETROS GERAIS ---------------- #
# ----------------------------------------------- #
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
t <th width=100%>Par&#226;metros gerais</th></tr>
t </table>
#
t <table border=0 width=99%><font size="3">
#
t <tr><td width=40%><img src=indica1.gif>Cabe&#231;alho com logotipo:</td>
c c A <td width=60%><input type=checkbox name=lg %s>Habilitar</td></tr>
#
t <tr><td><img src=indica1.gif>Letra do ticket: </td>
c c B <td><input type=checkbox name=dlen %s>Habilitar</td></tr>
#
t <tr><td><img src=indica1.gif>Comprimento do n&#250;mero: </td>
t <td><select name=dlnum>
c c C <option value=1 %s>1 D&#237;gito</option><option value=2 %s>2 D&#237;gitos</option>
c c D <option value=3 %s>3 D&#237;gitos</option><option value=4 %s>4 D&#237;gitos</option>
t </select></td></tr>
#
t <tr><td><img src=indica1.gif>Formato do n&#250;mero: </td>
t <td><select name=dhnum>
c c E <option value=0 %s>Caractere Normal</option><option value=1 %s>Caractere Expandido</option>
c c F <option value=2 %s>Caractere Gr&#225;fico</option><option value=3 %s>Cod barras: codigo39</option>
#c c G <option value=4 %s>Cod barras: itf 2 por 5</option><option value=5 %s>Cod barras: codabar</option>
t </select></td></tr>
#
t <tr><td><img src=indica1.gif>Corte do papel:</td>
t <td><select name=cut>
c c H <option value=0 %s>Total</option><option value=1 %s>Parcial</option>
t </select></td></tr>
#
t </font></table>
#
# ----------------------------------------------------- #
# -----------  MENSAGEM DO CABEÇALHO 1 ---------------- #
# ----------------------------------------------------- #
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
t <th width=100%>Linha 1 do cabe&#231;alho</th></tr>
t </table>
#
t <table border=0 width=99%><font size="3">
#
t <tr><td width=40%><img src=indica1.gif>Op&#231;&#245;es da mensagem: </td>
t <td width=60%><select name=h1op onchange="disablecontrol(this.value,'h1op')">
c c J <option value=0 %s>Mensagem fonte padr&#227;o</option><option value=1 %s>Mensagem fonte expandida</option>
c c K <option value=2 %s>Mostrar previs&#227;o de espera</option><option value=3 %s>Mostrar data e hora</option>
c c L <option value=4 %s>Mostrar data</option><option value=5 %s>Mostrar hora</option>
t </select></td></tr>
#
t <tr><td><img src=indica1.gif>Mensagem: </td>
c c M <td><input type=text name=h1me value="%s" size=36 maxlength=36></td></tr>
#
t </font></table>
#
# ----------------------------------------------------- #
# -----------  MENSAGEM DO CABEÇALHO 2 ---------------- #
# ----------------------------------------------------- #
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
t <th width=100%>Linha 2 do cabe&#231;alho</th></tr>
t </table>
#
t <table border=0 width=99%><font size="3">
#
t <tr><td width=40%><img src=indica1.gif>Op&#231;&#245;es da mensagem: </td>
t <td width=60%><select name=h2op onchange="disablecontrol(this.value,'h2op')">
c c N <option value=0 %s>Mensagem fonte padr&#227;o</option><option value=1 %s>Mensagem fonte expandida</option>
c c O <option value=2 %s>Mostrar previs&#227;o de espera</option><option value=3 %s>Mostrar data e hora</option>
c c P <option value=4 %s>Mostrar data</option><option value=5 %s>Mostrar hora</option>
t </select></td></tr>
#
t <tr><td><img src=indica1.gif>Mensagem: </td>
c c Q <td><input type=text name=h2me value="%s" size=36 maxlength=36></td></tr>
#
t </font></table>
#
# ----------------------------------------------------- #
# -----------  MENSAGEM DO CABEÇALHO 3 ---------------- #
# ----------------------------------------------------- #
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
t <th width=100%>Linha 3 do cabe&#231;alho</th></tr>
t </table>
#
t <table border=0 width=99%><font size="3">
#
t <tr><td width=40%><img src=indica1.gif>Op&#231;&#245;es da mensagem: </td>
t <td width=60%><select name=h3op onchange="disablecontrol(this.value,'h3op')">
c c R <option value=0 %s>Mensagem fonte padr&#227;o</option><option value=1 %s>Mensagem fonte expandida</option>
c c S <option value=2 %s>Mostrar previs&#227;o de espera</option><option value=3 %s>Mostrar data e hora</option>
c c T <option value=4 %s>Mostrar data</option><option value=5 %s>Mostrar hora</option>
t </select></td></tr>
#
t <tr><td><img src=indica1.gif>Mensagem: </td>
c c U <td><input type=text name=h3me value="%s" size=36 maxlength=36></td></tr>
#
t </font></table>
#
# -------------------------------------------------- #
# -----------  MENSAGEM DO RODAPÉ 1 ---------------- #
# -------------------------------------------------- #
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
t <th width=100%>Linha 1 do rodap&#233;</th></tr>
t </table>
#
t <table border=0 width=99%><font size="3">
#
t <tr><td width=40%><img src=indica1.gif>Op&#231;&#245;es da mensagem: </td>
t <td width=60%><select name=f1op onchange="disablecontrol(this.value,'f1op')">
c c V <option value=0 %s>Mensagem fonte padr&#227;o</option><option value=1 %s>Mensagem fonte expandida</option>
c c W <option value=2 %s>Mostrar previs&#227;o de espera</option><option value=3 %s>Mostrar data e hora</option>
c c X <option value=4 %s>Mostrar data</option><option value=5 %s>Mostrar hora</option>
t </select></td></tr>
#
t <tr><td><img src=indica1.gif>Mensagem: </td>
c c Y <td><input type=text name=f1me value="%s" size=36 maxlength=36></td></tr>
#
t </font></table>
#
# -------------------------------------------------- #
# -----------  MENSAGEM DO RODAPÉ 2 ---------------- #
# -------------------------------------------------- #
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
t <th width=100%>Linha 2 do rodap&#233;</th></tr>
t </table>
#
t <table border=0 width=99%><font size="3">
#
t <tr><td width=40%><img src=indica1.gif>Op&#231;&#245;es da mensagem: </td>
t <td width=60%><select name=f2op onchange="disablecontrol(this.value,'f2op')">
c c Z <option value=0 %s>Mensagem fonte padr&#227;o</option><option value=1 %s>Mensagem fonte expandida</option>
c c a <option value=2 %s>Mostrar previs&#227;o de espera</option><option value=3 %s>Mostrar data e hora</option>
c c b <option value=4 %s>Mostrar data</option><option value=5 %s>Mostrar hora</option>
t </select></td></tr>
#
t <tr><td><img src=indica1.gif>Mensagem: </td>
c c c <td><input type=text name=f2me value="%s" size=36 maxlength=36></td></tr>
#
t </font></table>
#
# -------------------------------------------------- #
# -----------  MENSAGEM DO RODAPÉ 3 ---------------- #
# -------------------------------------------------- #
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
t <th width=100%>Linha 3 do rodap&#233;</th></tr>
t </font></table>
#
t <table border=0 width=99%><font size="3">
#
t <tr><td width=40%><img src=indica1.gif>Op&#231;&#245;es da mensagem: </td>
t <td width=60%><select name=f3op onchange="disablecontrol(this.value,'f3op')">
c c d <option value=0 %s>Mensagem fonte padr&#227;o</option><option value=1 %s>Mensagem fonte expandida</option>
c c e <option value=2 %s>Mostrar previs&#227;o de espera</option><option value=3 %s>Mostrar data e hora</option>
c c f <option value=4 %s>Mostrar data</option><option value=5 %s>Mostrar hora</option>
t </select></td></tr>
#
t <tr><td><img src=indica1.gif>Mensagem: </td>
c c g <td><input type=text name=f3me value="%s" size=36 maxlength=36></td></tr>
#
t </font></table>
#
# -------------------------------------------------- #
# -----------  MENSAGEM DO RODAPÉ 4 ---------------- #
# -------------------------------------------------- #
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
t <th width=100%>Linha 4 do rodap&#233;</th></tr>
t </font></table>
#
t <table border=0 width=99%><font size="3">
#
t <tr><td width=40%><img src=indica1.gif>Op&#231;&#245;es da mensagem: </td>
t <td width=60%><select name=f4op onchange="disablecontrol(this.value,'f4op')">
c c h <option value=0 %s>Mensagem fonte padr&#227;o</option><option value=1 %s>Mensagem fonte expandida</option>
c c i <option value=2 %s>Mostrar previs&#227;o de espera</option><option value=3 %s>Mostrar data e hora</option>
c c j <option value=4 %s>Mostrar data</option><option value=5 %s>Mostrar hora</option>
t </select></td></tr>
#
t <tr><td><img src=indica1.gif>Mensagem: </td>
c c k <td><input type=text name=f4me value="%s" size=36 maxlength=36></td></tr>
#
t </font></table>
#
# --------------------------------------------------------------------------------------------------- #
# -----------  Termina aqui configuração de dados como formatado no módulo HTTP_CGI.c --------------- #
# --------------------------------------------------------------------------------------------------- #
#
t <p align=center>
#
t <input type=button name=func1 value="Confirmar" onclick="changeConfirm(this.form)">
t <input type=reset value="Desfazer" onclick="history.go()">
#
t </p></form>
#
i pg_footer.inc
#
. Fim do script precisa ter um ponto final.


