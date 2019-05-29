t <html><head><title>Chamada no painel</title>
t <link rel="icon" type="image/bmp" href="W.bmp" sizes="16x16">
t
t <script language=JavaScript type="text/javascript" src="xml_http.js"></script>
t <script language=JavaScript type="text/javascript">
t
t var formUpdate = new periodicObj("callnext.cgx", 6000);
t
t function periodicUpdate() {
t   updateMultiple(formUpdate);
t   periodicFormTime = setTimeout(periodicUpdate, formUpdate.period);
t }
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
t  window.localStorage.setItem('atd', cgi.elements['callm'].value);
t  window.localStorage.setItem('dep', cgi.elements['calld'].value);
t  window.localStorage.setItem('func', cgi.elements['callf'].value);
t }
t
t function recovery ()
t {
t  cgi.elements['callm'].value = window.localStorage.getItem('atd');
t  cgi.elements['calld'].value = window.localStorage.getItem('dep');
t  cgi.elements['callf'].value = window.localStorage.getItem('func');
t }
t
t </script></head>
t
t <style>
t
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
t </style>
t
t <body onload="recovery();periodicUpdate()" bgColor=#ffffff leftMargin=0 topMargin=10 marginwidth="0" 
t marginheight="0">
t <div align=center style="width: 640; height: 253">
t <table style="border: 1px solid #000080" cellSpacing=0 cellPadding=0 width=635 height=254>
t <tbody>
t <tr bgColor=#ffffff>
t <td>
t </td>
t <td width="100%" style="border-bottom: 1px solid #000080" vAlign="center" margin="20">
t <h2 align="center"><font face="verdana" color="#143c5c">Chamada no Painel</font></h2>
t </td>
t </tr>
t <tr bgColor=#E8E8E8>
t <td colSpan=5 width=630 height=130 vAlign=top>
t <div align=center>
t <center><font face="verdana">
t <table width="90%" border=0 align=center>
t <tbody>
t <tr>
t <td width="95%">
#
t <form action=callnext.cgi method=post name=cgi autocomplete="off">
t <input type=hidden value="callnext" name=pg>
#
t <table border=0 width=99%>
t <font size="3">
t <input type=hidden name=callf id=calljf value=0>
#
t <font size="3">
#
t <tr><td width=60%>
t N&#250;mero do atendente: 
t </td>
t <td width=20%>
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
t <td width=15%>
t </td>
t </tr>
#
t <tr>
t <td width=60%>
t Escolha o departamento: 
t </td>
t <td width=20%>Fila:
t </td>
t </td>
t <td width=15%>Chamando:
t </td>
t </tr>
#
c u A <tr><td width=60%><input type="radio" name="calld" value=1 %s>%s</td>
c u B <td width=20%><input type="text" class="info" id="f1" value="%d"></td>
c u C <td width=15%><input type="text" class="info" id="t1" value="%c%03d"></td></tr>
c u D <tr><td width=60%><input type="radio" name="calld" value=2 %s>%s</td>
c u E <td width=20%><input type="text" class="info" id="f2" value="%d"></td>
c u F <td width=15%><input type="text" class="info" id="t2" value="%c%03d"></td></tr>
c u G <tr><td width=60%><input type="radio" name="calld" value=3 %s>%s</td>
c u H <td width=20%><input type="text" class="info" id="f3" value="%d"></td>
c u I <td width=15%><input type="text" class="info" id="t3" value="%c%03d"></td></tr>
c u J <tr><td width=60%><input type="radio" name="calld" value=4 %s>%s</td>
c u K <td width=20%><input type="text" class="info" id="f4" value="%d"></td>
c u L <td width=15%><input type="text" class="info" id="t4" value="%c%03d"></td></tr>
c u M <tr><td width=60%><input type="radio" name="calld" value=5 %s>%s</td>
c u N <td width=20%><input type="text" class="info" id="f5" value="%d"></td>
c u O <td width=15%><input type="text" class="info" id="t5" value="%c%03d"></td></tr>
c u P <tr><td width=60%><input type="radio" name="calld" value=6 %s>%s</td>
c u Q <td width=20%><input type="text" class="info" id="f6" value="%d"></td>
c u R <td width=15%><input type="text" class="info" id="t6" value="%c%03d"></td></tr>
c u S <tr><td width=60%><input type="radio" name="calld" value=7 %s>%s</td>
c u T <td width=20%><input type="text" class="info" id="f7" value="%d"></td>
c u U <td width=15%><input type="text" class="info" id="t7" value="%c%03d"></td></tr>
c u V <tr><td width=60%><input type="radio" name="calld" value=8 %s>%s</td>
c u W <td width=20%><input type="text" class="info" id="f8" value="%d"></td>
c u X <td width=15%><input type="text" class="info" id="t8" value="%c%03d"></td></tr>
c u Y <tr><td width=60%><input type="radio" name="calld" value=9 %s>%s</td>
c u Z <td width=20%><input type="text" class="info" id="f9" value="%d"></td>
c u 0 <td width=15%><input type="text" class="info" id="t9" value="%c%03d"></td></tr>
c u 1 <tr><td width=60%><input type="radio" name="calld" value=10 %s>%s</td>
c u 2 <td width=20%><input type="text" class="info" id="f10" value="%d"></td>
c u 3 <td width=15%><input type="text" class="info" id="t10" value="%c%03d"></td></tr>
c u 4 <tr><td width=60%><input type="radio" name="calld" value=11 %s>%s</td>
c u 5 <td width=20%><input type="text" class="info" id="f11" value="%d"></td>
c u 6 <td width=15%><input type="text" class="info" id="t11" value="%c%03d"></td></tr>
c u 7 <tr><td width=60%><input type="radio" name="calld" value=12 %s>%s</td>
c u 8 <td width=20%><input type="text" class="info" id="f12" value="%d"></td>
c u 9 <td width=15%><input type="text" class="info" id="t12" value="%c%03d"></td></tr>
#
t </font>
t </table>
#
t <p align=center>
#
t <input type=button id="next" class="button" value="CHAMAR" onclick="changeConfirm(this.form,1)">
t <input type=button id="repeat" class="button" value="REPETIR" onclick="changeConfirm(this.form,2)">
t <input type=button id="close" class="button" value="ENCERRAR" onclick="changeConfirm(this.form,3)">
t <input type=button id="cancel" class="button" value="CANCELAR" onclick="changeConfirm(this.form,4)">
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


