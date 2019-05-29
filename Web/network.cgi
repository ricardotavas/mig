t <html><head><title>Par&#226;metros de rede</title>
t <link rel="icon" type="image/bmp" href="W.bmp" sizes="16x16">
t <script language=JavaScript>
t function changeConfirm(f){
t  if(!confirm('Confirma gravacao das alteracoes ?')) return;
t  f.submit();
t }
t function disablecontrol(obj) 
t {
t  if (obj.checked)
t  {
t 		cgi.elements['wfss'].disabled = false;
t 		cgi.elements['wfpw'].disabled = false;
t  }
t  else
t  {
t 		cgi.elements['wfss'].disabled = true;
t 		cgi.elements['wfpw'].disabled = true;
t  }
t }
t </script></head>
t
t
t <body onload="disablecontrol(cgi.elements['wfes'])">
t
i pg_header.inc
t <h2 align=center><br>Par&#226;metros de rede</h2>
t <form action=index.htm method=post name=cgi>
t <input type=hidden value="network" name=pg>
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
t <th width=40%>Item</th>
t <th width=60%>Par&#226;metros</th></tr>
#
# Here begin data setting which is formatted in HTTP_CGI.C module
#
t <tr><td><img src=indica1.gif>Nome do dispositivo: </td>
c e A <td><input type=text name=hname value="%s" size=16 maxlength=16></td></tr>
#
t <tr><td><img src=indica1.gif>DHCP: </td>
t <td><select name="dhcp">
c e B <option %s>0&#45;Desabilitado</option><option %s>1&#45;Habilitado</option></select></td></tr>
#
t <tr><td><img src=indica1.gif>Endere&#231;o IP: </td>
c e C <td><input type=text name=ip value="%d.%d.%d.%d" size=15 maxlength=15></td></tr>
#
t <tr><td><IMG SRC=indica1.gif>M&#225;scara de sub-rede: </td>
c e E <td><input type=text name=msk value="%d.%d.%d.%d" size=15 maxlength=15></td></tr>
#
t <tr><td><IMG SRC=indica1.gif>Gateway padr&#227;o: </td>
c e F <td><input type=text name=gw value="%d.%d.%d.%d" size=15 maxlength=15></td></tr>
#
t <tr><td><IMG SRC=indica1.gif>Servido DNS prefer&#234;ncial: </td>
c e G <td><input type=text name=dn1 value="%d.%d.%d.%d" size=15 maxlength=15></td></tr>
#
t <tr><td><IMG SRC=indica1.gif>Servido DNS alternativo: </td>
c e H <td><input type=text name=dn2 value="%d.%d.%d.%d" size=15 maxlength=15></td></tr>
#
t <tr><td><img src=indica1.gif>Endere&#231;o MAC: </td>
c e I <td><b>%d.%d.%d.%d.%d.%d</b></td></tr>
#
t </font></table>
#
# Here begin button definitions
#
t <p align=center>
#
t <input type=button name=set value="Alterar" onclick="changeConfirm(this.form)">
t <input type=reset value="Desfazer">
#
t </p></form>
#
i pg_footer.inc
#
. End of script must be closed with period.


