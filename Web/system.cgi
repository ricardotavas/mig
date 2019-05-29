t <html><head><title>Configura&#231;&#227;o do sistema</title>
t <link rel="icon" type="image/bmp" href="W.bmp" sizes="16x16">
t <script language=JavaScript>
t function changeConfirm(f)
t {
t  if(!confirm('Confirma gravacao das alteracoes ?')) return;
t  showDT();
t  f.submit();
t }
t function showDT()
t {
t  var t = new Date();
t  document.getElementById("jtime").value = ct(t.getHours())+":"+ct(t.getMinutes())+":"+ct(t.getSeconds());
t  document.getElementById("jdate").value = ct(t.getDate())+"/"+ct(t.getMonth()+1)+"/"+t.getFullYear();
t }
t function ct(i)
t {
t  if (i<10) i="0" + i;
t  return i;
t }
t function initdisable ()
t {
t  disablecontrol(cgi.elements['taut'],'taut');
t  if (cgi.elements['live'].value!=0)
t  {
t   cgi.elements['zero'].disabled = true;
t   cgi.elements['zero'].checked = false;
t  }
t  else
t   cgi.elements['zero'].disabled = false;
t  if (cgi.mode[0].checked)
t  {
t   cgi.elements['svip'].disabled = true;
t   cgi.elements['info'].disabled = false;
t  }
t  else
t  {
t   cgi.elements['svip'].disabled = false;
t   cgi.elements['info'].disabled = true;
t  }
t }
t function disablecontrol(obj,element)
t {
t  if (element=='taut')
t  {
t 	if(obj.checked)
t   {
t 		cgi.elements['tini'].disabled = false;
t 		cgi.elements['tend'].disabled = false;
t 		cgi.elements['d1'].disabled = false;
t 		cgi.elements['d2'].disabled = false;
t 		cgi.elements['d3'].disabled = false;
t 		cgi.elements['d4'].disabled = false;
t 		cgi.elements['d5'].disabled = false;
t 		cgi.elements['d6'].disabled = false;
t 		cgi.elements['d7'].disabled = false;
t   }
t  	else
t   {
t 		cgi.elements['tini'].disabled = true;
t 		cgi.elements['tend'].disabled = true;
t 		cgi.elements['d1'].disabled = true;
t 		cgi.elements['d2'].disabled = true;
t 		cgi.elements['d3'].disabled = true;
t 		cgi.elements['d4'].disabled = true;
t 		cgi.elements['d5'].disabled = true;
t 		cgi.elements['d6'].disabled = true;
t 		cgi.elements['d7'].disabled = true;
t   }
t  }
t  else if (element=='mode')
t  {
t 	if(obj==2)
t   {
t 	 cgi.elements['svip'].disabled = false;
t    cgi.elements['info'].checked = false;
t    cgi.elements['info'].disabled = true;
t   }
t   else if (obj==1)
t   {
t 	 cgi.elements['svip'].disabled = true;
t    cgi.elements['info'].disabled = false;
t   }
t  }
t }
t </script></head>
#
#
#
#
t <body onload="initdisable()">
i pg_header.inc
t <h2 align=center><br>Configura&#231;&#227;o do sistema</h2>
t <form action=index.htm method=post name=cgi>
t <input type=hidden value="system" name=pg>
#
# ----------------------------------------------- #
# -----------  PARÂMETROS GERAIS ---------------- #
# ----------------------------------------------- #
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
t <th width=100%>Sistema de arquivos</th></tr>
t </font></table>
#
t <table border=0 width=99%><font face="verdana" size="3">
#
t <tr><td width=50%><img src=indica1.gif>Drive em uso:</td>
c a A <td width=50%><input type=radio name=driv value=2 %s>Flash<br>
c a B <input type=radio name=driv value=4 %s>SD&nbsp;CARD<br>
t </td></tr>
#
t </font></table>
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
t <th width=100%>Hor&#225;rio do sistema</th></tr>
t </font></table>
#
t <table border=0 width=99%><font size="3">
#
#
t <tr><td width=50%><img src=indica1.gif>Ajustar data e hora do sistema:</td>
t <td width=50%><input type=checkbox name=ajrc %s>&nbsp;Quando clicar no botão Confirmar
c a C <input type=hidden name=time id=jtime value="%2d:%2d:%2d">
c a D <input type=hidden name=date id=jdate value="%2d/%2d/%4d">
t </td></tr>
#
t </font></table>
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
t <th width=100%>Op&#231;&#245;es do ticket</th></tr>
t </font></table>
#
t <table border=0 width=99%><font face="verdana" size="3">
#
t <tr><td width=50%><img src=indica1.gif>Auto teste de impress&#227;o de ticket: </td>
c a E <td width=50%><input type=radio name=test value=0 %s>
t Desabilitado<br>
c a F <input type=radio name=test value=1 %s>
t Habilitado<br>
t </td></tr>
#
t <tr><td><img src=indica1.gif>Tempo de vida do ticket: </td>
c a G <td><input type=text name=live value="%d" onchange="initdisable()" size=2 maxlength=2>&#32;(0 a 18 Horas)
t </td></tr>
#
t <tr><td width=50%><img src=indica1.gif>Imprimir protocolo de atendimento: </td>
c a H <td width=50%><input type=radio name=prot value=0 %s> Desabilitado<br>
c a I <input type=radio name=prot value=1 %s> Habilitado<br>
t </td></tr>
#
t <tr><td><IMG SRC=indica1.gif>Endere&#231;o IP do servidor de protocolo: </td>
c a J <td><input type=text name=ptip value="%d.%d.%d.%d" size=15 maxlength=15>
t </td></tr>
#
t <tr><td width=50%><img src=indica1.gif>Inverter impressão do ticket: </td>
c a K <td width=50%><input type=checkbox name=inve %s> Habilitado (Requer reinicialização)<br>
t </td></tr>
#
t </font></table>
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
t <th width=100%>Controle de funcionamento</th></tr>
t </font></table>
#
t <table border=0 width=99%><font size="3">
#
t <tr><td width=50%><img src=indica1.gif>Habilitar controle:</td>
c a L <td width=50%><input type=checkbox name=taut onclick="disablecontrol(this,'taut')" %s></td></tr>
#
t <tr><td><img src=indica1.gif>Hor&#225;rio para ligar: </td>
c a M <td><input type=text name=tini value="%2d:%2d" size=5 maxlength=5>&nbsp;Formato:&nbsp;(hh:mm)</td></tr>
#
t <tr><td><img src=indica1.gif>Hor&#225;rio para desligar:</td>
c a N <td><input type=text name=tend value="%2d:%2d" size=5 maxlength=5>&nbsp;Formato:&nbsp;(hh:mm)</td></tr>
#
t <tr><td><img src=indica1.gif>Ligar somente nos dias:</td>
t <td align=left>
c a O <input type=checkbox name=d1 %s>Dom
c a P <input type=checkbox name=d2 %s>Seg<input type=checkbox name=d3 %s>Ter
c a Q <input type=checkbox name=d4 %s>Qua<input type=checkbox name=d5 %s>Qui
c a R <input type=checkbox name=d6 %s>Sex<input type=checkbox name=d7 %s>Sab
t </td></tr>
#
t </font></table>
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
t <th width=100%>Controle do relat&#243;rio</th></tr>
t </font></table>
#
t <table border=0 width=99%><font face="verdana" size="3">
#
t <tr><td width=50%><img src=indica1.gif>Hor&#225;rio para fechar o atendimento: </td>
c a S <td width=50%><input type=text name=trep value="%2d:%2d" size=5 maxlength=5>
t &nbsp;Formato:&nbsp;(hh:mm)</td></tr>
#
t <tr><td width=50%><img src=indica1.gif>Gravar arquivo .CSV de relat&#243;rio:</td>
c a T <td width=50%><input type=checkbox name=erep %s>Habilitado</td></tr>
#
t <tr><td width=50%><img src=indica1.gif>Inicializar tickets dos departamentos:</td>
c a U <td width=50%><input type=checkbox name=zero %s>Habilitado</td></tr>
#
t </font></table>
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
t <th width=100%>Modo de uso da impressora de tickets</th></tr>
t </font></table>
#
t <table border=0 width=99%><font face="verdana" size="3">
#
t <tr><td width=50%><img src=indica1.gif>Escolha o modo: </td>
t <td width=50%>
c a V <input type=radio name=mode value=1 onclick="disablecontrol(this.value,'mode')" %s>Servidor<br>
c a W <input type=radio name=mode value=2 onclick="disablecontrol(this.value,'mode')" %s>Cliente<br>
t </td></tr>
#
t <tr><td><IMG SRC=indica1.gif>Endere&#231;o IP do servidor (modo cliente): </td>
c a X <td><input type=text name=svip value="%d.%d.%d.%d" size=15 maxlength=15>
#
t </font></table>
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
t <th width=100%>Informa&#231;&#245;es de fila</th></tr>
t </font></table>
#
t <table border=0 width=99%><font size="3">
#
t <tr><td width=50%><img src=indica1.gif>Enviar informa&#231;&#245;es de filas:</td>
c a Y <td width=50%><input type=checkbox name=info %s>&nbsp;Habilitar
t </td></tr>
#
t </font></table>
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
t <th width=100%>Departamento de vendas</th></tr>
t </font></table>
#
t <table border=0 width=99%><font size="3">
#
t <tr><td width=50%><img src=indica1.gif>Departamento de vendas:</td>
c a Z <td width=50%><input type=text name=sale value="%2d" size=2 maxlength=2>
t </td></tr>
#
t </font></table>
#
# Here begin button definitions
#
t <p align=center>
#
t <input type=button name=set value="Confirmar" onclick="changeConfirm(this.form)">
t <input type=reset value="Desfazer" onclick="history.go()">
#
t </p></form>
#
i pg_footer.inc
#
. End of script must be closed with period.


