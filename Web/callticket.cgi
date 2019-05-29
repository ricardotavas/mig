t <html><head><title>Chamada de ticket no painel</title>
t <link rel="icon" type="image/bmp" href="W.bmp" sizes="16x16">
t
t <script language=JavaScript>
t
t function changeConfirm(f, func)
t {
t  cgi.elements['callf'].value = func;
t  save ();
t  f.submit();
t }
t
t function save ()
t {
t  window.localStorage.setItem('letra', cgi.elements['calll'].value);
t  window.localStorage.setItem('senha', cgi.elements['calls'].value);
t  window.localStorage.setItem('mesa', cgi.elements['callm'].value);
t  window.localStorage.setItem('linha', cgi.elements['callr'].value);
t  window.localStorage.setItem('funcao', cgi.elements['callf'].value);
t }
t
t function recovery ()
t {
t  cgi.elements['calll'].value = window.localStorage.getItem('letra');
t  cgi.elements['calls'].value = window.localStorage.getItem('senha');
t  cgi.elements['callm'].value = window.localStorage.getItem('mesa');
t  cgi.elements['callr'].value = window.localStorage.getItem('linha');
t  cgi.elements['callf'].value = window.localStorage.getItem('funcao');
t }
t
t function inc ()
t {
t  var valor = parseInt(cgi.elements['calls'].value);
t  cgi.elements['calls'].value = valor + 1;
t }
t
t </script></head>
t
t <style>
t .button 
t {
t  background-color: #005580;
t  border: none;
t  color: white;
t  width: 130px;
t  height: 50px;
t  border-radius: 8px;
t  text-align: center;
t  text-decoration: none;
t  font-size: 20px;
t  font-weight: bold;
t  margin: 4px 2px;
t  cursor: pointer;
t  box-shadow: 0 9px #999;
t }
t
t .button:hover {background-color: #00334d}
t
t .button:active 
t {
t  background-color: #00334d;
t  box-shadow: 0 5px #666;
t  transform: translateY(4px);
t }
t </style>
t
t <body onload="recovery()" bgColor=#ffffff leftMargin=0 topMargin=10 marginwidth="0" marginheight="0">
t <div align=center style="width: 640; height: 280">
t <table style="border: 1px solid #000080" cellSpacing=0 cellPadding=0 width=635 height=276>
t <tbody>
t <tr bgColor=#ffffff>
t <td style="border-bottom: 1px solid #000080" vAlign=center noWrap width=80 height=58>
t <a href="http://www.wyma.com"><img border="0" src="WYMA.bmp" align="left" width=78 height=56></a>
t </td>
t <td style="border-bottom: 1px solid #000080" vAlign=bottom noWrap margin=50 width=552 height=58>
t <h2 align="center"><font face="verdana" color="#143c5c">Chamada de ticket no painel</font></h2>
t </td>
t </tr>
t <tr bgColor=#E8E8E8>
t <td colSpan=5 width=630 height=196 vAlign=top>
t <div align=center>
t <center><font face="verdana">
t <table width="90%" border=0 align=center>
t <tbody>
t <tr>
t <td width="95%">
#
t <form action=callticket.cgi method=post name=cgi autocomplete="off">
t <input type=hidden value="callticket" name=pg>
#
t <table border=0 width=99%>
t <font size="3">
t <input type=hidden name=callf id=calljf value=0>
t <tr>
t <td width=50%>
t <img src=indica1.gif>Letra do departamento: 
t </td>
t <td width=50%>
t <select name="calll">
t <option>A</option><option>B</option><option>C</option><option>D</option><option>E</option>
t <option>F</option><option>G</option><option>H</option><option>I</option><option>J</option>
t <option>K</option><option>L</option><option>M</option><option>N</option><option>O</option>
t <option>P</option><option>Q</option><option>R</option><option>S</option><option>T</option>
t <option>U</option><option>V</option><option>W</option><option>X</option><option>Y</option>
t <option>Z</option>
t </select>
t </td>
t </tr>
#
t <tr>
t <td width=50%><img src=indica1.gif>Senha: </td>
t <td width=50%>
t <input type=text name=calls size=4 maxlength=4>
t <input type=button name=incr value="INCREMENTA" onclick="inc()">
t </td>
t </tr>
#
t <tr>
t <td width=50%><img src=indica1.gif>N&#250;mero do atendente: </td>
t <td width=50%>
t <select name="callm">
t <option>1</option><option>2</option><option>3</option><option>4</option><option>5</option>
t <option>6</option><option>7</option><option>8</option><option>9</option><option>10</option>
t <option>11</option><option>12</option><option>13</option><option>14</option><option>15</option>
t <option>16</option><option>17</option><option>18</option><option>19</option><option>20</option>
t <option>21</option><option>22</option><option>23</option><option>24</option><option>25</option>
t <option>26</option><option>27</option><option>28</option><option>29</option><option>30</option>
t <option>31</option><option>32</option><option>33</option><option>34</option><option>35</option>
t <option>36</option><option>37</option><option>38</option><option>39</option><option>40</option>
t <option>41</option><option>42</option><option>43</option><option>44</option><option>45</option>
t <option>46</option><option>47</option><option>48</option><option>49</option><option>50</option>
t <option>51</option><option>52</option><option>53</option><option>54</option><option>55</option>
t <option>56</option><option>57</option><option>58</option><option>59</option><option>60</option>
t <option>61</option><option>62</option><option>63</option><option>64</option><option>65</option>
t <option>66</option><option>67</option><option>68</option><option>69</option><option>70</option>
t <option>71</option><option>72</option><option>73</option><option>74</option><option>75</option>
t <option>76</option><option>77</option><option>78</option><option>79</option><option>80</option>
t <option>81</option><option>82</option><option>83</option><option>84</option><option>85</option>
t <option>86</option><option>87</option><option>88</option><option>89</option><option>90</option>
t <option>91</option><option>92</option><option>93</option><option>94</option><option>95</option>
t <option>96</option><option>97</option><option>98</option><option>99</option>
t </select>
t </td>
t </tr>
#
t <tr>
t <td width=50%><img src=indica1.gif>Linha do painel: </td>
t <td width=50%>
t <select name="callr">
t <option value=1>Linha 1</option>
t <option value=2>Linha 2</option>
t <option value=3>Linha 3</option>
t <option value=4>Linha 4</option>
t <option value=5>Linha 5</option>
t </select>
t </td>
t </tr>
#
t </font>
t </table>
#
# Here begin button definitions
#
t <p align=center>
#
t <input type=button name="ticket" class="button" value="CHAMAR" onclick="changeConfirm(this.form,5)">
t <input type=button name="close" class="button" value="ENCERRAR" onclick="changeConfirm(this.form,3)">
t <input type=button name="cancel" class="button" value="CANCELAR" onclick="changeConfirm(this.form,4)">
#
t </p>
t </form>
#
t </td>
t </tr></tbody></table>
t </font></center>
t </div>
t </td>
t </tr>
t <tr bgColor=#C8C8C8>
t <td style="border-top: 1px solid #000080" colSpan=5 width=630 height=20>
t <p align=center><font face="Verdana" size="1">Direitos autorais reservados 
t <a href="http://www.wyma.com/">WYMA Tecnologia</a></font></p>
t </td>
t </tr></tbody></table>
t </div></body>
t </html>
#
. End of script must be closed with period.


