<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<meta http-equiv="Pragma" content="no-cache">
<head>
<link rel="stylesheet" title="Standard" href="ez_css.css" type="text/css" media="screen" />
<link href="index_css.css" rel="stylesheet" type="text/css" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
</head>


<body>
<form method="post" name="web_login" action="/goform/web_login">
<div id="wrapper">
<div class="index_bg" id="index_bg"></div>
<div class="frame_left"></div>
<div class="frame_right"></div>
<div class="home_image"></div>

<div class="home_note">
<ul>
	<li class="modelname" id="modelname">General Platform</li>
	<li class="welcome_text" id="mantitle">
		Welcome to the device configuration interface. 
		Enter the password and click 'Login'.</li>
	<li><table width="350" border="0" cellpadding="0" cellspacing="0">
		<tr>
			<td>&nbsp;</td>
			<td width="92">&nbsp;</td>
			<td width="117">&nbsp;</td>
			<td width="137">&nbsp;</td>
		</tr>
		<tr>
			<td>&nbsp;</td>
			<td><span class="password_word" id="manAdmUser">User Name</span><span class="password_word" id="manAdmuser"> :&nbsp</span></td>
			<td><input name="Loginusername" style="width:120px;" maxlength="30" value="" class=""/></td>
		</tr>
		<tr>
			<td>&nbsp;</td>
			<td><span class="password_word" id="manAdmPasswd">Password</span><span class="password_word" id="manAdmPasswd"> :&nbsp</span></td>
			<td><input name="LoginPassword" style="width:120px;" maxlength="30" value="" type="password" class=""/></td>
		</tr>
		<tr>
			<td>&nbsp;</td>
		</tr>
		<tr>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
			<td><div><ul>
				<li class="button3"><input type="hidden" name="set_flag" size="1" value="0">
					<a href="#"><span id=manlogin>Login</span></a>
				</li>
			</ul></div></td>
		</tr>
		<tr>
		<td>&nbsp;</td>
		<td colspan="3"><span class="m" id=manpassword>( max. 30 alphanumeric, printable characters and no spaces )</span></td>
		</tr>
		<tr>
			<td>&nbsp;</td>
		</tr>

		<tr>
			<td>&nbsp;</td>
			<td>
				<span class="password_word" id="manSelectLang">Language</span>
				<span class="password_word"> :&nbsp</span>
			</td>
			<td><span class="green">
				<select name="langSelection" id="langSelection">
				</select>
				</span>
			</td>
			<td>&nbsp;</td>
		</tr>
		<tr>
			<td width="4">&nbsp;</td>
			<td colspan="3"></td>
		</tr>
	</table></li>
</ul>
</div>

<span class="off">
<div class="copyright" id="copyright">(C) Copyright  by HP Communications Corp.<img src="/goahead1.gif"></div>
<div id="set_pupup1">

<div class="close1"><a href="#"></a></div>
<ul>
	<li style="margin-top:10px; font-weight:bold;" id="ChangeUnit">Change Unit</li>
	<li>
		<select name="degree_select">
			<option value='f' id=tempc_select>&deg;F</option>
			<option value='c' id=tempf_select>&deg;C</option>
		</select>
	</li>
	<li style="font-weight:bold;" id="Changelocation">Change location</li>
	<li class="list">
		<select name="city_select" size="2">
			<option value='AUXX0025' id='Austria - Vienna'>Austria - Vienna</option>
			<option value='CHXX0008' id='China - Beijing'>China - Beijing</option>
			<option value='CSXX0009' id='Costa Rica - San Jose'>Costa Rica - San Jose</option>
			<option value='EZXX0012' id='Czech - Prague'>Czech - Prague</option>
			<option value='DAXX0009' id='Denmark - Copenhagen'>Denmark - Copenhagen</option>
			<option value='FIXX0002' id='Finland - Helsinki'>Finland - Helsinki</option>
			<option value='FRXX0076' id='France - Paris'>France - Paris</option>
			<option value='GMXX0007' id='Germany - Berlin'>Germany - Berlin</option>
			<option value='GRXX0004' id='Greece - Athens'>Greece - Athens</option>
			<option value='INXX0096' id='India - New Delhi'>India - New Delhi</option>
			<option value='IDXX0022' id='Indonesia - Jakarta'>Indonesia - Jakarta</option>
			<option value='ITXX0067' id='Italy - Roma'>Italy - Roma</option>
			<option value='JAXX0085' id='Japan - Tokyo'>Japan - Tokyo</option>
			<option value='MYXX0008' id='Malaysia - Kuala Lumpur'>Malaysia - Kuala Lumpur</option>
			<option value='NLXX0002' id='Netherlands - Amsterdam'>Netherlands - Amsterdam</option>
			<option value='NOXX0029' id='Norway - Oslo'>Norway - Oslo</option>
			<option value='RSXX0063' id='Russia - Moscow'>Russia - Moscow</option>
			<option value='SNXX0006' id='Singapore - Singapore'>Singapore - Singapore</option>
			<option value='SPXX0050' id='Spain - Madrid'>Spain - Madrid</option>
			<option value='SWXX0031' id='Sweden - Stockholm'>Sweden - Stockholm</option>
			<option value='SZXX0006' id='Switzerland - Bern'>Switzerland - Bern</option>
			<option value='TWXX0021' id='Taiwan - Taipei'>Taiwan - Taipei</option>
			<option value='THXX0002' id='Thailand - Bangkok'>Thailand - Bangkok</option>
			<option value='TUXX0002' id='Turkey - Ankara'>Turkey - Ankara</option>
			<option value='UKXX0085' id='UK - London'>UK - London</option>
			<option value='UKXX1428' id='UK - Greenwich'>UK - Greenwich</option>
			<option value='USNY0996' id='USA - New York'>USA - New York</option>
			<option value='USDC0001' id='USA - Washington D.C.'>USA - Washington D.C.</option>
			<option value='USCA0638' id='USA - Los Angeles'>USA - Los Angeles</option>
			<option value='VMXX0006' id='Vietnam - Hanoi'>Vietnam - Hanoi</option>
		</select>
	</li>
	<li class="button3">
		<div align="right" style="margin-right:10px;">
			<a id="Finish" href="#"><span >Finish</span></a>  
		</div>
	</li>
</ul>
</div>

<div id="set_pupup2">
<div class="close1"><a href="#" ></a></div>
<ul><li style="font-weight:bold; margin-top:10px;" id="timezone">Change time zone
</li><li class="list">
	<select name="time_zone" size="2">
		<option value="UCT_-11" id="manNTPMidIsland">(GMT-11:00) Midway Island, Samoa</option>
		<option value="UCT_-10" id="manNTPHawaii">(GMT-10:00) Hawaii</option>
		<option value="NAS_-09" id="manNTPAlaska">(GMT-09:00) Alaska</option>
		<option value="PST_-08" id="manNTPPacific">(GMT-08:00) Pacific Time (US &amp; Canada), Tijuana</option>
		<option value="MST_-07" id="manNTPArizona">(GMT-07:00) Arizona</option>
		<option value="MST_-07" id="manNTPMountain">(GMT-07:00) Mountain Time (US &amp; Canada)</option>
		<option value="CST_-06" id="manNTPCentralAmerica">(GMT-06:00) Central America</option>
		<option value="UCT_-06" id="manNTPCentralTime">(GMT-06:00) Central Time (US &amp; Canada)</option>
		<option value="UCT_-05" id="manNTPBogota">(GMT-05:00) Bogota, Lima, Quito</option>
		<option value="EST_-05" id="manNTPEasternTime">(GMT-05:00) Eastern Time (US &amp; Canada)</option>
		<option value="AST_-04" id="manNTPAltlantic">(GMT-04:00) Altlantic Time (Canada)</option>
		<option value="UCT_-04" id="manNTPCaracas">(GMT-04:00) Caracas, La Poz, Santiago</option>
		<option value="UCT_-03" id="manNTPBrasilia">(GMT-03:00) Brasilia, Greenland</option>
		<option value="EBS_-03" id="manNTPBuenos">(GMT-03:00) Buenos Aires, Georgetown</option>
		<option value="NOR_-02" id="manNTPMid-Atlantic">(GMT-02:00) Mid-Atlantic</option>
		<option value="EUT_-01" id="manNTPAzores">(GMT-01:00) Azores, Cape Verde Is.</option>
		<option value="UCT_000" id="manNTPCasablanca">(GMT) Casablanca, Monrovia</option>
		<option value="GMT_000" id="manNTPGreenwich">(GMT) Greenwich Mean Time : Dublin, Edinburgh, Lisbon, London</option>
		<option value="MET_001" id="manNTPAmsterdam">(GMT+01:00) Amsterdam, Berlin, Bern, Rome, Stockholm, Vienna</option>
		<option value="MEZ_001" id="manNTPBelgrade">(GMT+01:00) Belgrade, Bratislava, Budapest, Ljubljana, Prague</option>
		<option value="UCT_001" id="manNTPBrussels">(GMT+01:00) Brussels, Copenhagen, Madrid, Paris</option>
		<option value="EET_002" id="manNTPAthens">(GMT+02:00) Athens, Beirut, Istanbul, Minsk</option>
		<option value="SAS_002" id="manNTPBucharest">(GMT+02:00) Bucharest, Cairo, Harare, Pretoria</option>
		<option value="IST_003" id="manNTPBaghdad">(GMT+03:00) Baghdad, Kuwait, Riyadh, Nairobi</option>
		<option value="MSK_003" id="manNTPMoscow">(GMT+03:00) Moscow, St. Petersburg, Volgograd</option>
		<option value="UCT_004" id="manNTPMuscat">(GMT+04:00) Abu Dhabi, Muscat</option>
		<option value="UCT_005" id="manNTPEkaterinburg">(GMT+05:00) Ekaterinburg</option>
		<option value="UCT_006" id="manNTPAlmaty">(GMT+06:00) Almaty, Novosibirsk</option>
		<option value="UCT_007" id="manNTPBangkok">(GMT+07:00) Bangkok, Hanoi, Jakarta</option>
		<option value="CST_008" id="manNTPBeijing">(GMT+08:00) Beijing, Chongqing, Hong Kong, Urumqi</option>
		<option value="CCT_008" id="manNTPIrkutsk">(GMT+08:00) Irkutsk, Ulaan Bataar</option>
		<option value="SST_008" id="manNTPSingapore">(GMT+08:00) Kuala Lumpur, Singapore</option>
		<option value="AWS_008" id="manNTPTaipei">(GMT+08:00) Perth, Taipei</option>
		<option value="JST_009" id="manNTPTokyo">(GMT+09:00) Osaka, Sapporo, Tokyo</option>
		<option value="KST_009" id="manNTPYakutsk">(GMT+09:00) Seoul, Yakutsk</option>
		<option value="UCT_010" id="manNTPVladivostok">(GMT+10:00) Brisbane, Vladivostok</option>
		<option value="AES_010" id="manNTPCanberra">(GMT+10:00) Canberra, Melbourne, Sydney</option>
		<option value="UCT_011" id="manNTPMagadan">(GMT+11:00) Magadan, Solomon Is., New Caledonia</option>
		<option value="UCT_012" id="manNTPAuckland">(GMT+12:00) Auckland, Wellington</option>
		<option value="NZS_012" id="manNTPFiji">(GMT+12:00) Fiji, Kamchatka, Marshall Is.</option>
	</select>
	</li><li class="button3">
<div align="right" style="margin-right:10px;"><a id="Finish" href="#"><span >Finish</span></a>  </div>
</li>
</ul></div>


<div class="infomation">
	<table width="100%" height="82" border="0" cellpadding="0" cellspacing="0">
	<tr>
		<td width="104" class="weather" id=jpg_display></td>
		<td width="130"><div class="weather_word">
			<ul>
				<li class="thermograph" id=temp_display></li>
				<li class="city" id=city_display></li>
			</ul>
		</div></td>
		<td width="127"><div class="clock">
			<ul>
				<li class="time" id="mtenCurrent_Time"></li>
				<li class="date" id="mtenCurrent_Date"></li>
			</ul>
		</div></td>
	</tr>
	</table>
</div>


</span>
</div>
</form>
<form method="post" name="NTP" action="/goform/NTP">
	<input type="hidden" name="mtenCurrent_Hour" 	value="<% getCurrentHour();%>" />
	<input type="hidden" name="mtenCurrent_Min" 	value="<% getCurrentMin();%>" />
	<input type="hidden" name="mtenCurrent_Sec" 	value="<% getCurrentSec();%>" />
	<input type="hidden" name="mtenCurrent_Year" 	value="<% getCurrentYear();%>" />

	<input type="hidden" name="mtenCurrent_Mon" 	value="<% getCurrentMon();%>" />

	<input type="hidden" name="mtenCurrent_Day" 	value="<% getCurrentDay();%>" />
</form>

<iframe name='setlang' src='/temp.html' width='0' height='0' marginwidth='0' marginheight='0' frameborder='0' scrolling='no'>             
</iframe>

</body>
</html>
