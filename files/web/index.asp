<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<meta http-equiv="Pragma" content="no-cache">
<head>
<link rel="stylesheet" title="Standard" href="ez_css.css" type="text/css" media="screen" />
<link href="index_css.css" rel="stylesheet" type="text/css" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("admin");
var modelname = "<% getCfgGeneral(1, "SystemName"); %>";
function initTranslation()
{
	var lang="<% getCfgGeneral(1, "Language"); %>";
	var cook = "en";
	
	if ((lang!="en") && (lang!="de") && (lang!="fr") && (lang!="es") && (lang!="zhtw") && (lang!="zhcn") && (lang!="it") && (lang!="turk"))
		lang = "en";

	if (document.cookie.length > 0) {
		var s = document.cookie.indexOf("language=");
		var e = document.cookie.indexOf(";", s);
		if (s != -1) {
			if (e == -1)
				cook = document.cookie.substring(s+9);
			else
				cook = document.cookie.substring(s+9, e);
		}
	}
	
	document.cookie="language="+lang+"; path=/";

	if (cook != lang)
		self.location.reload();
	
	var e = document.getElementById("manAdmPasswd");
	e.innerHTML = _("man admin passwd");
	e = document.getElementById("manAdmUser");
	e.innerHTML = _("man admin username");
	e = document.getElementById("manSelectLang");
	e.innerHTML = _("man select language");

  e = document.getElementById("modelname");
	e.innerHTML = modelname;
	e = document.getElementById("mantitle");
	e.innerHTML = _("login loginmsg");
	e = document.getElementById("manpassword");
	e.innerHTML = _("login passmsg");
	e = document.getElementById("manlogin");
	e.innerHTML = _("login login");

}

function initValue() {
	var lang_element = document.getElementById("langSelection");
	var lang_en = "<% getLangBuilt("en"); %>";
	var lang_de = "<% getLangBuilt("de"); %>";
	var lang_fr = "<% getLangBuilt("fr"); %>";
	var lang_es = "<% getLangBuilt("es"); %>";
	var lang_zhtw = "<% getLangBuilt("zhtw"); %>";
	var lang_it = "<% getLangBuilt("it"); %>";
	var lang_zhcn = "<% getLangBuilt("zhcn"); %>";
	var lang_turk = "<% getLangBuilt("turk"); %>";

	initTranslation();
	//lang_element.options.length = 0;
	if (lang_en == "1")
		lang_element.options[lang_element.length] = new Option('English', 'en');
	if (lang_de == "1")
		lang_element.options[lang_element.length] = new Option('Deutsch', 'de');
	if (lang_fr == "1")
		lang_element.options[lang_element.length] = new Option('Français', 'fr');
	if (lang_es == "1")
		lang_element.options[lang_element.length] = new Option('Español', 'es');
	if (lang_zhtw == "1")
		lang_element.options[lang_element.length] = new Option('繁體中文', 'zhtw');
	if (lang_it == "1")
		lang_element.options[lang_element.length] = new Option('Italiano', 'it');
	if (lang_zhcn == "1")
		lang_element.options[lang_element.length] = new Option('简体中文', 'zhcn');
	if (lang_turk == "1")
		lang_element.options[lang_element.length] = new Option('Turkçe', 'turk');

	if (document.cookie.length > 0) {
		var s = document.cookie.indexOf("language=");
		var e = document.cookie.indexOf(";", s);
		var lang = "en";
		var i;

		if (s != -1) {
			if (e == -1)
				lang = document.cookie.substring(s+9);
			else
				lang = document.cookie.substring(s+9, e);
		}
		for (i=0; i<lang_element.options.length; i++) {
			if (lang == lang_element.options[i].value) {
				lang_element.options.selectedIndex = i;
				break;
			}
		}
	}
}
function setLanguage()
{
	document.cookie="language="+document.forms[0].langSelection.value+"; path=/";
	//self.location.reload();
	setlang.location.href = '/set_language.asp?'+document.forms[0].langSelection.value;
	return true;
}
</script>
</head>
<body onLoad="initValue()">
<form method="post" name="web_login" action="/goform/web_login">
<div id="wrapper">
<div class="index_bg" id="index_bg"></div>
<div class="frame_left"></div>
<div class="frame_right"></div>
<!--div class="logo" id="logo"></div-->
<div class="home_image"></div>
<div class="home_note">
  <ul><li class="modelname" id="modelname"><% getCfgGeneral(1, "SystemName"); %></li><li class="welcome_text" id="mantitle">Welcome to the device configuration interface. 
Enter the password and click 'Login'.</li><li>
  <table width="350" border="0" cellpadding="0" cellspacing="0">
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
	<tr><td>&nbsp;</td></tr>
	<tr>
      <td>&nbsp;</td>
	  <td>&nbsp;</td>
      <td><div><ul><li class="button3"><input type="hidden" name="set_flag" size="1" value="0"><a href="javascript: document.web_login.submit();"><span id=manlogin>Login</span></a></li></ul></div></td>
    </tr>
    <tr>
      <td>&nbsp;</td>
      <td colspan="3"><span class="m" id=manpassword>( max. 30 alphanumeric, printable characters and no spaces )</span></td>
      </tr>
    <tr>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
    </tr>
    
    <tr>
      <td>&nbsp;</td>
      <td><span class="password_word" id="manSelectLang">Language</span><span class="password_word"> :&nbsp</span></td>
      <td><span class="green">
			<select name="langSelection" id="langSelection" onchange="return setLanguage()">
			</select>
      </span></td>
      <td>&nbsp;</td>
    </tr>
    <tr>
      <td width="4">&nbsp;</td>
      <td colspan="3">	 </td>
      </tr>
  </table>
</li></ul>
</div>
<span class="off">
<div class="copyright" id="copyright">(C) Copyright  by HP Communications Corp.<img src="/goahead1.gif"></div>


  <div class="infomation">
  <table width="100%" height="82" border="0" cellpadding="0" cellspacing="0">
    <tr>
      <td width="104" class="weather" id=jpg_display></td>
      <td width="130"><div class="weather_word"><ul><li class="thermograph" id=temp_display></li>
      <li class="city" id=city_display></li>
      </ul></div></td>
      <td width="127"><div class="clock"><ul>
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
         <input type="hidden" name="mtenCurrent_Hour" value="<% getCurrentHour();%>" />
         <input type="hidden" name="mtenCurrent_Min" value="<% getCurrentMin();%>" />
         <input type="hidden" name="mtenCurrent_Sec" value="<% getCurrentSec();%>" />
         <input type="hidden" name="mtenCurrent_Year" value="<% getCurrentYear();%>" />

         <input type="hidden" name="mtenCurrent_Mon" value="<% getCurrentMon();%>" />

         <input type="hidden" name="mtenCurrent_Day" value="<% getCurrentDay();%>" />
</form>
</body>
</html>
