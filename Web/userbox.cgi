t <html><head><title>Escolha do box do usu&#225;rio</title>
t <link rel="icon" type="image/bmp" href="W.bmp" sizes="16x16">
t <script language=JavaScript>
t function changePage(){
t document.location.href="index.htm";
t }
t function FormSubmit(f){
t f.submit();
t }
t </script></head>
#
i pg_header.inc
t <h2 align=center><br>Escolha do box do usu&#225;rio</h2>
t <form action=userbox.cgi method=post name=cgi>
t <input type=hidden value="seluser" name=pg>
#
# ----------------------------------------------- #
# -----------  NÚMERO DO USUÁRIO ---------------- #
# ----------------------------------------------- #
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
t <th width=100%>Selecione o usu&#225;rio: </th>
#
t <tr>
t <td align=center><select name=unum>
c p 0 <option %s>1</option><option %s>2</option><option %s>3</option><option %s>4</option><option %s>5</option>
c p 1 <option %s>6</option><option %s>7</option><option %s>8</option><option %s>9</option><option %s>10</option>
c p 2 <option %s>11</option><option %s>12</option><option %s>13</option><option %s>14</option><option %s>15</option>
c p 3 <option %s>16</option><option %s>17</option><option %s>18</option><option %s>19</option><option %s>20</option>
c p 4 <option %s>21</option><option %s>22</option><option %s>23</option><option %s>24</option><option %s>25</option>
c p 5 <option %s>26</option><option %s>27</option><option %s>28</option><option %s>29</option><option %s>30</option>
c p 6 <option %s>31</option><option %s>32</option><option %s>33</option><option %s>34</option><option %s>35</option>
c p 7 <option %s>36</option><option %s>37</option><option %s>38</option><option %s>39</option><option %s>40</option>
c p 8 <option %s>41</option><option %s>42</option><option %s>43</option><option %s>44</option><option %s>45</option>
c p 9 <option %s>46</option><option %s>47</option><option %s>48</option><option %s>49</option><option %s>50</option>
c p : <option %s>51</option><option %s>52</option><option %s>53</option><option %s>54</option><option %s>55</option>
c p ; <option %s>56</option><option %s>57</option><option %s>58</option><option %s>59</option><option %s>60</option>
c p < <option %s>61</option><option %s>62</option><option %s>63</option><option %s>64</option><option %s>65</option>
c p = <option %s>66</option><option %s>67</option><option %s>68</option><option %s>69</option><option %s>70</option>
c p > <option %s>71</option><option %s>72</option><option %s>73</option><option %s>74</option><option %s>75</option>
c p ? <option %s>76</option><option %s>77</option><option %s>78</option><option %s>79</option><option %s>80</option>
c p @ <option %s>81</option><option %s>82</option><option %s>83</option><option %s>84</option><option %s>85</option>
c p A <option %s>86</option><option %s>87</option><option %s>88</option><option %s>89</option><option %s>90</option>
c p B <option %s>91</option><option %s>92</option><option %s>93</option><option %s>94</option><option %s>95</option>
c p C <option %s>96</option><option %s>97</option><option %s>98</option><option %s>99</option>
t </select></td></tr>
#
t </font></table>
#
# -------------------------------------------- #
# -----------  BOX DO USUÁRIO ---------------- #
# -------------------------------------------- #
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
t <th width=100%>Configure o box do usu&#225;rio: </th>
#
t <tr>
t <td align=center><select name=ubox>
c p D <option %s>1</option><option %s>2</option><option %s>3</option><option %s>4</option><option %s>5</option>
c p E <option %s>6</option><option %s>7</option><option %s>8</option><option %s>9</option><option %s>10</option>
c p F <option %s>11</option><option %s>12</option><option %s>13</option><option %s>14</option><option %s>15</option>
c p G <option %s>16</option><option %s>17</option><option %s>18</option><option %s>19</option><option %s>20</option>
c p H <option %s>21</option><option %s>22</option><option %s>23</option><option %s>24</option><option %s>25</option>
c p I <option %s>26</option><option %s>27</option><option %s>28</option><option %s>29</option><option %s>30</option>
c p J <option %s>31</option><option %s>32</option><option %s>33</option><option %s>34</option><option %s>35</option>
c p K <option %s>36</option><option %s>37</option><option %s>38</option><option %s>39</option><option %s>40</option>
c p L <option %s>41</option><option %s>42</option><option %s>43</option><option %s>44</option><option %s>45</option>
c p M <option %s>46</option><option %s>47</option><option %s>48</option><option %s>49</option><option %s>50</option>
c p N <option %s>51</option><option %s>52</option><option %s>53</option><option %s>54</option><option %s>55</option>
c p O <option %s>56</option><option %s>57</option><option %s>58</option><option %s>59</option><option %s>60</option>
c p P <option %s>61</option><option %s>62</option><option %s>63</option><option %s>64</option><option %s>65</option>
c p Q <option %s>66</option><option %s>67</option><option %s>68</option><option %s>69</option><option %s>70</option>
c p R <option %s>71</option><option %s>72</option><option %s>73</option><option %s>74</option><option %s>75</option>
c p S <option %s>76</option><option %s>77</option><option %s>78</option><option %s>79</option><option %s>80</option>
c p T <option %s>81</option><option %s>82</option><option %s>83</option><option %s>84</option><option %s>85</option>
c p U <option %s>86</option><option %s>87</option><option %s>88</option><option %s>89</option><option %s>90</option>
c p V <option %s>91</option><option %s>92</option><option %s>93</option><option %s>94</option><option %s>95</option>
c p W <option %s>96</option><option %s>97</option><option %s>98</option><option %s>99</option>
t </select></td></tr>
t <tr><td align=center>
t <input type=button name=func1 value="Confirmar" onclick="FormSubmit(this.form)">
t </td></tr>
#
t </font></table>
#
# ------------------------------------------------- #
# -----------  LISTA USUÁRIO X BOX ---------------- #
# ------------------------------------------------- #
#
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#C8C8C8>
t <th width=100%>Configura&#231;&#227;o atual: Box X Usu&#225;rio</th>
#
t <tr>
t <td align=center>
c p X Usu&#225;rio 1= Box %d<br>Usu&#225;rio 2= Box %d<br>Usu&#225;rio 3= Box %d<br>Usu&#225;rio 4= Box %d<br>
c p Y Usu&#225;rio 5= Box %d<br>Usu&#225;rio 6= Box %d<br>Usu&#225;rio 7= Box %d<br>Usu&#225;rio 8= Box %d<br>
c p Z Usu&#225;rio 9= Box %d<br>Usu&#225;rio 10= Box %d<br>Usu&#225;rio 11= Box %d<br>Usu&#225;rio 12= Box %d<br>
c p [ Usu&#225;rio 13= Box %d<br>Usu&#225;rio 14= Box %d<br>Usu&#225;rio 15= Box %d<br>Usu&#225;rio 16= Box %d<br>
c p \ Usu&#225;rio 17= Box %d<br>Usu&#225;rio 18= Box %d<br>Usu&#225;rio 19= Box %d<br>Usu&#225;rio 20= Box %d<br>
c p ] Usu&#225;rio 21= Box %d<br>Usu&#225;rio 22= Box %d<br>Usu&#225;rio 23= Box %d<br>Usu&#225;rio 24= Box %d<br>
c p ^ Usu&#225;rio 25= Box %d<br>Usu&#225;rio 26= Box %d<br>Usu&#225;rio 27= Box %d<br>Usu&#225;rio 28= Box %d<br>
c p _ Usu&#225;rio 29= Box %d<br>Usu&#225;rio 30= Box %d<br>Usu&#225;rio 31= Box %d<br>Usu&#225;rio 32= Box %d<br>
c p ` Usu&#225;rio 33= Box %d<br>Usu&#225;rio 34= Box %d<br>Usu&#225;rio 35= Box %d<br>Usu&#225;rio 36= Box %d<br>
c p a Usu&#225;rio 37= Box %d<br>Usu&#225;rio 38= Box %d<br>Usu&#225;rio 39= Box %d<br>Usu&#225;rio 40= Box %d<br>
c p b Usu&#225;rio 41= Box %d<br>Usu&#225;rio 42= Box %d<br>Usu&#225;rio 43= Box %d<br>Usu&#225;rio 44= Box %d<br>
c p c Usu&#225;rio 45= Box %d<br>Usu&#225;rio 46= Box %d<br>Usu&#225;rio 47= Box %d<br>Usu&#225;rio 48= Box %d<br>
c p d Usu&#225;rio 49= Box %d<br>Usu&#225;rio 50= Box %d<br>Usu&#225;rio 51= Box %d<br>Usu&#225;rio 52= Box %d<br>
c p e Usu&#225;rio 53= Box %d<br>Usu&#225;rio 54= Box %d<br>Usu&#225;rio 55= Box %d<br>Usu&#225;rio 56= Box %d<br>
c p f Usu&#225;rio 57= Box %d<br>Usu&#225;rio 58= Box %d<br>Usu&#225;rio 59= Box %d<br>Usu&#225;rio 60= Box %d<br>
c p g Usu&#225;rio 61= Box %d<br>Usu&#225;rio 62= Box %d<br>Usu&#225;rio 63= Box %d<br>Usu&#225;rio 64= Box %d<br>
c p h Usu&#225;rio 65= Box %d<br>Usu&#225;rio 66= Box %d<br>Usu&#225;rio 67= Box %d<br>Usu&#225;rio 68= Box %d<br>
c p i Usu&#225;rio 69= Box %d<br>Usu&#225;rio 70= Box %d<br>Usu&#225;rio 71= Box %d<br>Usu&#225;rio 72= Box %d<br>
c p j Usu&#225;rio 73= Box %d<br>Usu&#225;rio 74= Box %d<br>Usu&#225;rio 75= Box %d<br>Usu&#225;rio 76= Box %d<br>
c p k Usu&#225;rio 77= Box %d<br>Usu&#225;rio 78= Box %d<br>Usu&#225;rio 79= Box %d<br>Usu&#225;rio 80= Box %d<br>
c p l Usu&#225;rio 81= Box %d<br>Usu&#225;rio 82= Box %d<br>Usu&#225;rio 83= Box %d<br>Usu&#225;rio 84= Box %d<br>
c p m Usu&#225;rio 85= Box %d<br>Usu&#225;rio 86= Box %d<br>Usu&#225;rio 87= Box %d<br>Usu&#225;rio 88= Box %d<br>
c p n Usu&#225;rio 89= Box %d<br>Usu&#225;rio 90= Box %d<br>Usu&#225;rio 91= Box %d<br>Usu&#225;rio 92= Box %d<br>
c p o Usu&#225;rio 93= Box %d<br>Usu&#225;rio 94= Box %d<br>Usu&#225;rio 95= Box %d<br>Usu&#225;rio 96= Box %d<br>
c p p Usu&#225;rio 97= Box %d<br>Usu&#225;rio 98= Box %d<br>Usu&#225;rio 99= Box %d<br>
t </td></tr>
#
t </font></table>
t <p align=center>
#
t <input type=button name=func1 value="Voltar" onclick="changePage()">
#
t </p></form>
#
i pg_footer.inc
#
. Fim do script precisa ter um ponto final.


