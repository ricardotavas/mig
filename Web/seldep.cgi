t <html><head><title>Escolha do departamento</title>
t <link rel="icon" type="image/bmp" href="W.bmp" sizes="16x16">
t <script language=JavaScript>
t function changePage(){
t document.location.href="dep.cgi";
t }
t </script></head>
#
i pg_header.inc
t <h2 align=center><br>Escolha do departamento</h2>
t <form action=seldep.cgi method=post name=cgi>
t <input type=hidden value="seldep" name=pg>
#
# ---------------------------------------------------- #
# -----------  NÚMERO DO DEPARTAMENTO ---------------- #
# ---------------------------------------------------- #
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
t <th width=100%>Selecione o departamento: </th>
#
t <tr>
t <td align=center><select name=dnum OnChange="submit();">
c b 0 <option value=1 %s>%s</option><option value=2 %s>%s</option><option value=3 %s>%s</option>
c b 1 <option value=4 %s>%s</option><option value=5 %s>%s</option><option value=6 %s>%s</option>
c b 2 <option value=7 %s>%s</option><option value=8 %s>%s</option><option value=9 %s>%s</option>
c b 3 <option value=10 %s>%s</option><option value=11 %s>%s</option><option value=12 %s>%s</option>
c b ' <option value=13 %s>%s</option>
t </select></td></tr>
#
t </font></table>
#
t <p align=center>
#
t <input type=button name=set value="Configurar" onclick="changePage()">
#
t </p></form>
#
i pg_footer.inc
#
. Fim do script precisa ter um ponto final.


