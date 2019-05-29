t <html><head><title>Etiquetas</title>
t <link rel="icon" type="image/bmp" href="W.bmp" sizes="16x16">
t </head>
#
i pg_header.inc
t <h2 align=center><br>Imprima e recorte os atendimentos</h2>
#
t <script language=JavaScript>
t
t var inv=0;
t
t function imprimir()
t {
t  window.print();
t }
t
t function inverte()
t {
t  var str1 = "";
t  var str2 = "";
t
t  if ((inv%2)==0)
t  {
t    str1="fill:black;";
t    str2="fill:white; stroke-width:2; stroke:black;"
t  }
t  else
t  {
t    str1="fill:white;";
t    str2="fill:black; stroke-width:2; stroke:white;"
t  }
t  inv++;
t
t  document.getElementById('t1').style= str1;
t  document.getElementById('t2').style= str1;
t  document.getElementById('t3').style= str1;
t  document.getElementById('t4').style= str1;
t  document.getElementById('t5').style= str1;
t  document.getElementById('t6').style= str1;
t
t  document.getElementById('r1').style= str2;
t  document.getElementById('r2').style= str2;
t  document.getElementById('r3').style= str2;
t  document.getElementById('r4').style= str2;
t  document.getElementById('r5').style= str2;
t  document.getElementById('r6').style= str2;
t }
t
t </script>
#
t <form action=label.cgi method=post name=cgi>
t <input type=hidden value="label" name=pg>
#
t <table border=0 width=46% align="center">
t <tr>
t <td width=100%>
t <svg width="314" height="46">
t <rect id=r1 width="314" height="46" style="fill:black; stroke-width:2; stroke:white;" />
c u A <text id=t1 y="30" x="10" fill="white" font-family="Arial" font-size="20px">%s</text>
t </svg>
t </td></tr>
t </table>
t <table border=0 width=40%>
t <tr><td width=100%>        </td></tr>
t </table>
#
t <table border=0 width=46% align="center">
t <tr>
t <td width=100%>
t <svg width="314" height="46">
t <rect id=r2 width="314" height="46" style="fill:black; stroke-width:2; stroke:white;" />
c u B <text id=t2 y="30" x="10" fill="white" font-family="Arial" font-size="20px">%s</text>
t </svg>
t </td></tr>
t </table>
t <table border=0 width=40%>
t <tr><td width=100%>        </td></tr>
t </table>
#
t <table border=0 width=46% align="center">
t <tr>
t <td width=100%>
t <svg width="314" height="46">
t <rect id=r3 width="314" height="46" style="fill:black; stroke-width:2; stroke:white;" />
c u C <text id=t3 y="30" x="10" fill="white" font-family="Arial" font-size="20px">%s</text>
t </svg>
t </td></tr>
t </table>
t <table border=0 width=40%>
t <tr><td width=100%>        </td></tr>
t </table>
#
t <table border=0 width=46% align="center">
t <tr>
t <td width=100%>
t <svg width="314" height="46">
t <rect id=r4 width="314" height="46" style="fill:black; stroke-width:2; stroke:white;" />
c u D <text id=t4 y="30" x="10" fill="white" font-family="Arial" font-size="20px">%s</text>
t </svg>
t </td></tr>
t </table>
t <table border=0 width=40%>
t <tr><td width=100%>        </td></tr>
t </table>
#
t <table border=0 width=46% align="center">
t <tr>
t <td width=100%>
t <svg width="314" height="46">
t <rect id=r5 width="314" height="46" style="fill:black; stroke-width:2; stroke:white;" />
c u E <text id=t5 y="30" x="10" fill="white" font-family="Arial" font-size="20px">%s</text>
t </svg>
t </td></tr>
t </table>
t <table border=0 width=40%>
t <tr><td width=100%>        </td></tr>
t </table>
#
t <table border=0 width=46% align="center">
t <tr>
t <td width=100%>
t <svg width="314" height="46">
t <rect id=r6 width="314" height="46" style="fill:black; stroke-width:2; stroke:white;" />
c u F <text id=t6 y="30" x="10" fill="white" font-family="Arial" font-size="20px">%s</text>
t </svg>
t </td></tr>
t </table>
t <table border=0 width=40%>
t <tr><td width=100%>        </td></tr>
t </table>
#
t <p align=center>
#
t <input type=button name=cor value="Inverter cor" onclick="inverte()">
t <input type=button name=func1 value="imprimir" onclick="imprimir()">
#
t </p></form>
#
i pg_footer.inc
#
. Fim do script precisa ter um ponto final.
