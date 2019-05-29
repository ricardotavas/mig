t <html><head><title>Configura&#231;&#227;o do departamento</title>
t <link rel="icon" type="image/bmp" href="W.bmp" sizes="16x16">
t <script type="text/javascript">
t
t function changeConfirm(f)
t {
t  if(!confirm('Confirma gravacao das alteracoes ?')) 
t   return;
t  f.submit();
t }
t
t function tctr(obj)
t {
t  if (obj.checked)
t  {
t   cgi.elements['dtop'].disabled = false;
t   document.getElementsByName("itf2")[0].disabled = false;
t   document.getElementsByName("itf2")[1].disabled = false;
t  }
t  else
t  {
t   cgi.elements['dtop'].disabled = true;
t   document.getElementsByName("itf2")[0].disabled = true;
t   document.getElementsByName("itf2")[1].disabled = true;
t  }
t }
t
t function disable(obj,modo)
t {
t 	cgi.elements['dhelp1'].disabled = false;
t 	cgi.elements['dhelp2'].disabled = false;
t
t   if (modo!=2)
t   {
t    if (cgi.elements['dhelp3'].checked)
t    { 
t     cgi.elements['dhelp1'].disabled = true;
t     cgi.elements['dhelp2'].disabled = true;
t    }
t   }
t }
t </script></head>
#
t <body onload="disable(cgi.elements['dhelp3'],cgi.elements['xmod'].value);tctr(cgi.elements['epq'])">
#
i pg_header2.inc
c b 4 <h2 align=center><br>Configura&#231;&#227;o do departamento %d</h2>
t <form action=seldep.cgi method=post name=cgi>
t <input type=hidden value="dep" name=pg>
#
# ------------------------------------------------------- #
# -----------  PARÂMETROS DO DEPARTAMENTO --------------- #
# ------------------------------------------------------- #
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
t <th width=100%>Par&#226;metros do Departamento</th></tr>
t </font></table>
#
t <table border=0 width=99%><font size="3">
#
t <tr><td><IMG SRC=indica1.gif>Status do bot&#227;o na impressora: </td></font><font size="2">
c b 5 <td><input type=checkbox name=den %s><font size="2">
t Habilitar</font></td></tr>
#
t <tr><td width=45%><img src=indica1.gif>Nome:</td>
c b 6 <td width=55%><input type=text name=dname value="%s" size=48 maxlength=48></td></tr>
#
t <tr><td><IMG SRC=indica1.gif>Letra do ticket: </td>
t <td><select name=dlet>
c b 7 <option value=" " %s>Nenhuma</option><option %s>A</option><option %s>B</option><option %s>C</option>
c b 8 <option %s>D</option><option %s>E</option><option %s>F</option><option %s>G</option><option %s>H</option>
c b 9 <option %s>I</option><option %s>J</option><option %s>K</option><option %s>L</option><option %s>M</option>
c b : <option %s>N</option><option %s>O</option><option %s>P</option><option %s>Q</option><option %s>R</option>
c b ; <option %s>S</option><option %s>T</option><option %s>U</option><option %s>V</option><option %s>W</option>
c b < <option %s>X</option><option %s>Y</option><option %s>Z</option>
t </select>
t </td></tr>
#
t <tr><td><img src=indica1.gif>Faixa de n&#250;meros do ticket: </td>
c b = <td><input type=text name=drang1 value="%d" size=4 maxlength=4>&#32;e&#32;
c b > <input type=text name=drang2 value="%d" size=4 maxlength=4><font size="2">(Inicial e Final)</font></td></tr>
#
t <tr><td><img src=indica1.gif>Numero de vias:</td>
t <td><select name=via>
c b & <option value=1 %s>1 via</option><option value=2 %s>2 vias</option>
t </select></td></tr>
#
t <tr><td><IMG SRC=indica1.gif>Atender preferenciais +80 na frente: </td>
c b ´ <td><input type=checkbox name=dpref %s><font size="2">
t Habilitar</font></td></tr>
#
t <tr><td><IMG SRC=indica1.gif>Encerrar ticket autom&#225;tico: </td>
c b ? <td><input type=checkbox name=dtc1 %s><font size="2">Quando chamar o pr&#243;ximo ticket</font></td></tr>
#
t <tr><td></td>
c b @ <td><input type=checkbox name=dtc2 %s><font size="2">Quando gerar o relat&#243;rio</font></td></tr>
#
t <tr><td><IMG SRC=indica1.gif>Esquema de chamada: </td>
t <td><select name=dsch>
c b A <option value=161 %s>10 x 1</option><option value=145 %s>9 x 1</option><option value=129 %s>8 x 1</option>
c b B <option value=113 %s>7 x 1</option><option value=97 %s>6 x 1</option><option value=81 %s>5 x 1</option>
c b C <option value=65 %s>4 x 1</option><option value=49 %s>3 x 1</option><option value=33 %s>2 x 1</option>
c b D <option value=17 %s>1 x 1</option><option value=18 %s>1 x 2</option><option value=19 %s>1 x 3</option>
c b E <option value=20 %s>1 x 4</option><option value=21 %s>1 x 5</option><option value=22 %s>1 x 6</option>
c b F <option value=23 %s>1 x 7</option><option value=24 %s>1 x 8</option><option value=25 %s>1 x 9</option>
c b G <option value=26 %s>1 x 10</option>
t </select><font size="2">(Normal X Prefer&#234;ncial)</font>
t </td></tr>
#
t <tr><td><IMG SRC=indica1.gif>Ajudar departamentos: </td>
t <td><select name=dhelp1>
c b H <option value="0" %s>Nenhum</option><option %s>1</option><option %s>2</option><option %s>3</option>
c b I <option %s>4</option><option %s>5</option><option %s>6</option><option %s>7</option><option %s>8</option>
c b J <option %s>9</option><option %s>10</option><option %s>11</option><option %s>12</option>
t </select>&#32;ou&#32;
t <select name=dhelp2>
c b K <option value="0" %s>Nenhum</option><option %s>1</option><option %s>2</option><option %s>3</option>
c b L <option %s>4</option><option %s>5</option><option %s>6</option><option %s>7</option><option %s>8</option>
c b M <option %s>9</option><option %s>10</option><option %s>11</option><option %s>12</option>
t </select><font size="2">(Dpto1 ou Dpto2)</font>
t </td></tr>
#
t <tr><td></td>
c b N <td><input type=checkbox name=dhelp3 onclick="disable(this,cgi.elements['xmod'].value)" %s>
t <font size="2">Ignorar dptos de ajuda e procurar maior fila</font></td></tr>
#
t <tr><td><IMG SRC=indica1.gif>Esquema de ajuda: </td>
t <td><select name=dhelp4>
c b O <option value=161 %s>10 x 1</option><option value=145 %s>9 x 1</option><option value=129 %s>8 x 1</option>
c b P <option value=113 %s>7 x 1</option><option value=97 %s>6 x 1</option><option value=81 %s>5 x 1</option>
c b Q <option value=65 %s>4 x 1</option><option value=49 %s>3 x 1</option><option value=33 %s>2 x 1</option>
c b R <option value=17 %s>1 x 1</option><option value=18 %s>1 x 2</option><option value=19 %s>1 x 3</option>
c b S <option value=20 %s>1 x 4</option><option value=21 %s>1 x 5</option><option value=22 %s>1 x 6</option>
c b T <option value=23 %s>1 x 7</option><option value=24 %s>1 x 8</option><option value=25 %s>1 x 9</option>
c b U <option value=26 %s>1 x 10</option>
t </select><font size="2">(Dpto local X Dpto de ajuda)</font>
t </td></tr>
#
t <tr><td></td>
c b V <td><input type=checkbox name=dhelp5 onclick="disable(this,cgi.elements['xmod'].value)" %s>
t <font size="2">Ajudar quando acabarem os tickets do Dpto local.</font></td></tr>
#
t <tr><td></td>
c b W <td><input type=checkbox name=dhelp6 onclick="disable(this,cgi.elements['xmod'].value)" %s>
t <font size="2">Antes de ajudar, verificar o tempo de espera local.</font></td></tr>
#
t <tr><td></td>
c b X <td><input type=checkbox name=dhelp7 onclick="disable(this,cgi.elements['xmod'].value)" %s>
t <font size="2">Alternar o esquema de ajuda por usuários.</font></td></tr>
#
t <tr><td></td>&#32;<td></td></tr>
t </font></table>
#
# ------------------------------------------------------ #
# -----------  USUÁRIOS DO DEPARTAMENTO ---------------- #
# ------------------------------------------------------ #
#
t <div id="divRadios">
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
t <th width=100%>Usu&#225;rios do departamento</th></tr>
#
t <tr><td align=center>
c b Y <input type=checkbox name=du1 %s>01<input type=checkbox name=du2 %s>02<input type=checkbox name=du3 %s>03
c b Z <input type=checkbox name=du4 %s>04<input type=checkbox name=du5 %s>05<input type=checkbox name=du6 %s>06
c b [ <input type=checkbox name=du7 %s>07<input type=checkbox name=du8 %s>08<input type=checkbox name=du9 %s>09
c b \ <input type=checkbox name=du10 %s>10<input type=checkbox name=du11 %s>11<input type=checkbox name=du12 %s>12
t </td></tr>
t <tr><td align=center>
c b ] <input type=checkbox name=du13 %s>13<input type=checkbox name=du14 %s>14<input type=checkbox name=du15 %s>15
c b ^ <input type=checkbox name=du16 %s>16<input type=checkbox name=du17 %s>17<input type=checkbox name=du18 %s>18
c b _ <input type=checkbox name=du19 %s>19<input type=checkbox name=du20 %s>20<input type=checkbox name=du21 %s>21
c b ` <input type=checkbox name=du22 %s>22<input type=checkbox name=du23 %s>23<input type=checkbox name=du24 %s>24
t </td></tr>
t <tr><td align=center>
c b a <input type=checkbox name=du25 %s>25<input type=checkbox name=du26 %s>26<input type=checkbox name=du27 %s>27
c b b <input type=checkbox name=du28 %s>28<input type=checkbox name=du29 %s>29<input type=checkbox name=du30 %s>30
c b c <input type=checkbox name=du31 %s>31<input type=checkbox name=du32 %s>32<input type=checkbox name=du33 %s>33
c b d <input type=checkbox name=du34 %s>34<input type=checkbox name=du35 %s>35<input type=checkbox name=du36 %s>36
t </td></tr>
t <tr><td align=center>
c b e <input type=checkbox name=du37 %s>37<input type=checkbox name=du38 %s>38<input type=checkbox name=du39 %s>39
c b f <input type=checkbox name=du40 %s>40<input type=checkbox name=du41 %s>41<input type=checkbox name=du42 %s>42
c b g <input type=checkbox name=du43 %s>43<input type=checkbox name=du44 %s>44<input type=checkbox name=du45 %s>45
c b h <input type=checkbox name=du46 %s>46<input type=checkbox name=du47 %s>47<input type=checkbox name=du48 %s>48
t </td></tr>
t <tr><td align=center>
c b i <input type=checkbox name=du49 %s>49<input type=checkbox name=du50 %s>50<input type=checkbox name=du51 %s>51
c b j <input type=checkbox name=du52 %s>52<input type=checkbox name=du53 %s>53<input type=checkbox name=du54 %s>54
c b k <input type=checkbox name=du55 %s>55<input type=checkbox name=du56 %s>56<input type=checkbox name=du57 %s>57
c b l <input type=checkbox name=du58 %s>58<input type=checkbox name=du59 %s>59<input type=checkbox name=du60 %s>60
t </td></tr>
t <tr><td align=center>
c b m <input type=checkbox name=du61 %s>61<input type=checkbox name=du62 %s>62<input type=checkbox name=du63 %s>63
c b n <input type=checkbox name=du64 %s>64<input type=checkbox name=du65 %s>65<input type=checkbox name=du66 %s>66
c b o <input type=checkbox name=du67 %s>67<input type=checkbox name=du68 %s>68<input type=checkbox name=du69 %s>69
c b p <input type=checkbox name=du70 %s>70<input type=checkbox name=du71 %s>71<input type=checkbox name=du72 %s>72
t </td></tr>
t <tr><td align=center>
c b q <input type=checkbox name=du73 %s>73<input type=checkbox name=du74 %s>74<input type=checkbox name=du75 %s>75
c b r <input type=checkbox name=du76 %s>76<input type=checkbox name=du77 %s>77<input type=checkbox name=du78 %s>78
c b s <input type=checkbox name=du79 %s>79<input type=checkbox name=du80 %s>80<input type=checkbox name=du81 %s>81
c b t <input type=checkbox name=du82 %s>82<input type=checkbox name=du83 %s>83<input type=checkbox name=du84 %s>84
t </td></tr>
t <tr><td align=center>
c b u <input type=checkbox name=du85 %s>85<input type=checkbox name=du86 %s>86<input type=checkbox name=du87 %s>87
c b v <input type=checkbox name=du88 %s>88<input type=checkbox name=du89 %s>89<input type=checkbox name=du90 %s>90
c b w <input type=checkbox name=du91 %s>91<input type=checkbox name=du92 %s>92<input type=checkbox name=du93 %s>93
c b x <input type=checkbox name=du94 %s>94<input type=checkbox name=du95 %s>95<input type=checkbox name=du96 %s>96
t </td></tr>
#
t <tr><td></td>&#32;<td></td></tr>
t </font></table></div>
#
# ------------------------------------------------- #
# -----------  PARÂMETROS OPCIONAIS --------------- #
# ------------------------------------------------- #
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
t <th width=100%>Outros Par&#226;metros</th></tr>
t </font></table>
#
t <table border=0 width=99%><font size="3">
#
t <tr><td width=40%><IMG SRC=indica1.gif>IP do display de chamada: </td>
c b y <td width=60%><input type=text name=dpnl value="%d.%d.%d.%d" size=15 maxlength=15></td></tr>
#
t <tr><td width=40%><IMG SRC=indica1.gif>IP do teclado de opinião: </td>
c b { <td width=60%><input type=text name=dtop value="%d.%d.%d.%d" size=15 maxlength=15>&nbsp;
c b } <input type=checkbox name=epq onclick="tctr(this)" %s>Habilitar</td></tr>
#
t <tr><td><IMG SRC=indica1.gif>Linha do display de chamada: </td>
t <td><select name=dplin>
c b ~ <option %s>1</option><option %s>2</option><option %s>3</option><option %s>4</option><option %s>5</option>
c b ! <option %s>6</option><option %s>7</option><option %s>8</option><option %s>9</option><option %s>10</option>
t </select>
t </td></tr>
#
c b % <input type=hidden name=xmod value=%d>
#
t </font></table>
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
