t <html><head><title>Formatar memoria</title>
t <link rel="icon" type="image/bmp" href="W.bmp" sizes="16x16">
t <script language=JavaScript>
t  function formatConfirm(f){
t   if(!confirm('Voce tem certeza?')) return;
t   f.submit();
t  }
t </script></head>
i pg_header.inc
t <h2 align=center><br>Formatar</h2>
t <form action=index.htm method=post name=form1>
#
t <p align="center">
t <input type=checkbox name=format>Formatar o sistema de arquivos para otimização.
t </p>
#
t <p align="center">
t <input type=checkbox name=finit>Restaurar a configuração de fábrica do dispositivo.
t </p>
#
t <p align="center">
t <input type=checkbox name=ftkt>Inicializar a numeração dos tickets dos departamentos.
t </p>
#
t <p align="center">
t  <input type=button value="Confirmar" onclick="formatConfirm(this.form)">
t </p></form>
i pg_footer.inc
. End of script must be closed with period.
