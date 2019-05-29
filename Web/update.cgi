t <html><head><title>Atualizar</title>
t <link rel="icon" type="image/bmp" href="W.bmp" sizes="16x16">
t <script language=JavaScript>
t function changeConfirm(f){
t  if(!confirm('Deseja reiniciar o dispositivo ?')) return;
t  f.submit();
t }
t function changePage(){
t document.location.href="uploadbin.cgi";
t }
t </script>
t </head>
#
i pg_header.inc
t <h2 align=center><br>Atualizar</h2>
#
t <form action=update.cgi method=post name=cgi>
t <input type=hidden value="update" name=pg>
#
t <p><font size="3"><br>Permite fazer uma atualiza&#231;&#227;o de firmware por uma vers&#227;o mais 
t recente, o arquivo deve ser do tipo bin&#225;rio (extens&#227;o .bin), ao pressionar o bot&#227;o 
t abaixo o dispositivo ser&#225; reiniciado e a pr&#243;xima p&#225;gina poder&#225; demorar um pouco 
t mais para ser carregada.</font></p>
#
t <p><font size="3"><br>Aten&#231;&#227;o: O arquivo deve ser de fonte confi&#225;vel ou a impressora deixar&#225; 
t de responder aos comandos.</font></p>
#
t <p><font face="verdana"><br><br>Passo 1: Clique abaixo para reiniciar o dispositivo:</font></p>
t <input type=button value="Reiniciar" onclick="changeConfirm(this.form)">
#
t <p><font face="verdana"><br><br>Passo 2: Ap&#243;s o dispositivo reiniciar clique abaixo:</font></p>
t <input type=button value="Continuar" onclick="changePage()">
#
t </form>
#
i pg_footer.inc
. End of script must be closed with period.
