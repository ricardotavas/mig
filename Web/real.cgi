t <html><head><title>Informa&#231;&#245;es em tempo real</title>
t <link rel="icon" type="image/bmp" href="W.bmp" sizes="16x16">
t
t <script language=JavaScript type="text/javascript" src="xml_http.js"></script>
t <script language=JavaScript type="text/javascript">
t
t var formUpdate = new periodicObj("real.cgx", 6000);
t
t function periodicUpdate() {
t   updateMultiple(formUpdate);
t   periodicFormTime = setTimeout(periodicUpdate, formUpdate.period);
t }
t
t function changePage()
t {
t  document.location.href="index.htm";
t }
t </script>
#
t <style>
t
t .info
t {
t  background-color: transparent; 
t  font-size: 16px;
t  font-weight: bold;
t  width: 90%;
t  border: 0px;
t  readonly;
t }
t
t </style></head>
#
t <body onload="periodicUpdate()">
i pg_header.inc
t <h2 align=center><br>Informa&#231;&#245;es em tempo real</h2>
#
t <form action=real.cgi method=post name=cgi>
t <input type=hidden value="real" name=pg>
#
# ---------------------------------------------------------- #
# -----------  INFORMAÇÕES DOS DEPARTAMENTOS --------------- #
# ---------------------------------------------------------- #
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
c o A <th width=100%>%s</th></tr>
t </table>
t <table border=0 width=99%><font size="3">
t <tr><td width=50%><img src=indica1.gif>Pessoas na fila: </td>
c o B <td><input type="text" class="info" id="f1" value="%d"></td></tr>
t <tr><td width=40%><img src=indica1.gif>Tempo previsto de espera (hh:mm): </td>
c o C <td><input type="text" class="info" id="t1" value="%d:%2d"></td></tr>
t </font></table>
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
c o D <th width=100%>%s</th></tr>
t </table>
t <table border=0 width=99%><font size="3">
t <tr><td width=50%><img src=indica1.gif>Pessoas na fila: </td>
c o E <td><input type="text" class="info" id="f2" value="%d"></td></tr>
t <tr><td width=40%><img src=indica1.gif>Tempo previsto de espera (hh:mm): </td>
c o F <td><input type="text" class="info" id="t2" value="%d:%2d"></td></tr>
t </font></table>
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
c o G <th width=100%>%s</th></tr>
t </table>
t <table border=0 width=99%><font size="3">
t <tr><td width=50%><img src=indica1.gif>Pessoas na fila: </td>
c o H <td><input type="text" class="info" id="f3" value="%d"></td></tr>
t <tr><td width=40%><img src=indica1.gif>Tempo previsto de espera (hh:mm): </td>
c o I <td><input type="text" class="info" id="t3" value="%d:%2d"></td></tr>
t </font></table>
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
c o J <th width=100%>%s</th></tr>
t </table>
t <table border=0 width=99%><font size="3">
t <tr><td width=50%><img src=indica1.gif>Pessoas na fila: </td>
c o K <td><input type="text" class="info" id="f4" value="%d"></td></tr>
t <tr><td width=40%><img src=indica1.gif>Tempo previsto de espera (hh:mm): </td>
c o L <td><input type="text" class="info" id="t4" value="%d:%2d"></td></tr>
t </font></table>
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
c o M <th width=100%>%s</th></tr>
t </table>
t <table border=0 width=99%><font size="3">
t <tr><td width=50%><img src=indica1.gif>Pessoas na fila: </td>
c o N <td><input type="text" class="info" id="f5" value="%d"></td></tr>
t <tr><td width=40%><img src=indica1.gif>Tempo previsto de espera (hh:mm): </td>
c o O <td><input type="text" class="info" id="t5" value="%d:%2d"></td></tr>
t </font></table>
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
c o P <th width=100%>%s</th></tr>
t </table>
t <table border=0 width=99%><font size="3">
t <tr><td width=50%><img src=indica1.gif>Pessoas na fila: </td>
c o Q <td><input type="text" class="info" id="f6" value="%d"></td></tr>
t <tr><td width=40%><img src=indica1.gif>Tempo previsto de espera (hh:mm): </td>
c o R <td><input type="text" class="info" id="t6" value="%d:%2d"></td></tr>
t </font></table>
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
c o S <th width=100%>%s</th></tr>
t </table>
t <table border=0 width=99%><font size="3">
t <tr><td width=50%><img src=indica1.gif>Pessoas na fila: </td>
c o T <td><input type="text" class="info" id="f7" value="%d"></td></tr>
t <tr><td width=40%><img src=indica1.gif>Tempo previsto de espera (hh:mm): </td>
c o U <td><input type="text" class="info" id="t7" value="%d:%2d"></td></tr>
t </font></table>
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
c o V <th width=100%>%s</th></tr>
t </table>
t <table border=0 width=99%><font size="3">
t <tr><td width=50%><img src=indica1.gif>Pessoas na fila: </td>
c o W <td><input type="text" class="info" id="f8" value="%d"></td></tr>
t <tr><td width=40%><img src=indica1.gif>Tempo previsto de espera (hh:mm): </td>
c o X <td><input type="text" class="info" id="t8" value="%d:%2d"></td></tr>
t </font></table>
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
c o Y <th width=100%>%s</th></tr>
t </table>
t <table border=0 width=99%><font size="3">
t <tr><td width=50%><img src=indica1.gif>Pessoas na fila: </td>
c o Z <td><input type="text" class="info" id="f9" value="%d"></td></tr>
t <tr><td width=40%><img src=indica1.gif>Tempo previsto de espera (hh:mm): </td>
c o 0 <td><input type="text" class="info" id="t9" value="%d:%2d"></td></tr>
t </font></table>
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
c o 1 <th width=100%>%s</th></tr>
t </table>
t <table border=0 width=99%><font size="3">
t <tr><td width=50%><img src=indica1.gif>Pessoas na fila: </td>
c o 2 <td><input type="text" class="info" id="f10" value="%d"></td></tr>
t <tr><td width=40%><img src=indica1.gif>Tempo previsto de espera (hh:mm): </td>
c o 3 <td><input type="text" class="info" id="t10" value="%d:%2d"></td></tr>
t </font></table>
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
c o 4 <th width=100%>%s</th></tr>
t </table>
t <table border=0 width=99%><font size="3">
t <tr><td width=50%><img src=indica1.gif>Pessoas na fila: </td>
c o 5 <td><input type="text" class="info" id="f11" value="%d"></td></tr>
t <tr><td width=40%><img src=indica1.gif>Tempo previsto de espera (hh:mm): </td>
c o 6 <td><input type="text" class="info" id="t11" value="%d:%2d"></td></tr>
t </font></table>
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
c o 7 <th width=100%>%s</th></tr>
t </table>
t <table border=0 width=99%><font size="3">
t <tr><td width=50%><img src=indica1.gif>Pessoas na fila: </td>
c o 8 <td><input type="text" class="info" id="f12" value="%d"></td></tr>
t <tr><td width=40%><img src=indica1.gif>Tempo previsto de espera (hh:mm): </td>
c o 9 <td><input type="text" class="info" id="t12" value="%d:%2d"></td></tr>
t </font></table>
#
t <p align=center>
#
t <input type=button name=func1 value="voltar" onclick="changePage()">
#
t </p></form>
#
i pg_footer.inc
#
. Fim do script precisa ter um ponto final.
