t <html><head><title>Vers&#227;o</title>
t <link rel="icon" type="image/bmp" href="W.bmp" sizes="16x16">
t <script language=JavaScript>
t function changeConfirm(f){
t  if(!confirm('A senha de acesso a configuracao esta sendo alterada. \nConfirma alteracao?')) return;
t  f.submit();
t }
t function disablepassword(obj) {
t if(obj==1){ 
t 	cgi.elements['pw'].disabled = false;
t 	cgi.elements['pw2'].disabled = false;}
t if(obj==0){ 
t 	cgi.elements['pw'].disabled = true;
t 	cgi.elements['pw2'].disabled = true;}
t } 
t </script></head>
#
#
#
#
t <body onload="disablepassword(cgi.auth.value)">
i pg_header.inc
t <h2 align=center><br>Vers&#227;o</h2>
t <form action=index.htm method=post name=cgi>
t <input type=hidden value="ver" name=pg>
#
t <table border=0 width=99%><font size="3">
#
t <tr bgcolor=#C8C8C8><th width=40%>Item</th><th width=60%>Par&#226;metros</th></tr>
# -------------------------------------------------- #
# --- Informa o modelo do dispositivo -------------- #
# -------------------------------------------------- #
c d 3 <tr><td><img src=indica1.gif>Modelo:</TD><TD><b>%s</b></td></tr>
# -------------------------------------------------- #
# --- Informa a versão do dispositivo -------------- #
# -------------------------------------------------- #
c d 0 <tr><td><img src=indica1.gif>Vers&#227;o</TD><TD><b>%s</b></td></tr>
# -------------------------------------------------- #
# --- Indicador de status de uso da autenticação --- #
# -------------------------------------------------- #
t <tr><td><img src=indica1.gif>Autentica&#231;&#227;o</TD>
t <td><select name="auth" onChange="disablepassword(this.value)">
c d 1 <option value=0 %s>Desabilitado</option><option value=1 %s>Habilitado</option></select></td></tr>
# -------------------------------------------------- #
# --------- Text Box para digitar a senha ---------- #
# -------------------------------------------------- #
t <tr><td><img src=indica1.gif>Digite sua senha:</td>
c d 2 <td><input type=password name=pw size=10 maxlength=10 value="%s"></td></tr>
# -------------------------------------------------- #
# ---- Text Box para digitar a senha novamente ----- #
# -------------------------------------------------- #
t <tr><td><img src=indica1.gif>Digite novamente sua senha:</td>
c d 2 <td><input type=password name=pw2 size=10 maxlength=10 value="%s"></td></tr>
t </font></table>
# -------------------------------------------------- #
# ------------ Definição dos botões ---------------- #
# -------------------------------------------------- #
t <p align=center>
t <input type=button name=set value="Alterar" onclick="changeConfirm(this.form)">
t <input type=reset value="Desfazer" onclick="history.go()">
t </p></form>
i pg_footer.inc
. End of script must be closed with period.
