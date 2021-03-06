<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Expires" content="-1">
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<meta http-equiv="Pragma" content="no-cache">
<script type="text/javascript" src="/lang/b28n.js"></script>
<script type="text/javascript" src="/channel_sel.js"></script>
<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />

<style>
.on {display:on}
.off {display:none}
td {white-space: nowrap;}
</style>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("wireless");

var wirelessmode  = <% getCfgZero(1, "OP_Mode"); %>;
var wispmode  = <% getCfgZero(1, "wisp_mode"); %>;
var apcli_include = '<% getWlanApcliBuilt(); %>';

var wlan_guest_en  = <% getCfgZero(1, "WlanGuestEnable"); %>;
var wlan_guest_bw_en  = <% getCfgZero(1, "WlanGuestBWEnable"); %>;
var wlan_guest_bw_priority  = <% getCfgZero(1, "WlanGuestBWPriority"); %>;

var bm_en = <% getCfgZero(1, "RL_QoSEnable"); %>;

var uBW = "<% QoSGetInfo("RL_QoSUploadBw"); %>";
var dBW = "<% QoSGetInfo("RL_QoSDownloadBw"); %>";

var channelBGN = <% Channellist(); %>;

var changed = 0;
var old_MBSSID;
var defaultShownMBSSID = 0;
var SSID = new Array();
var PreAuth = new Array();
var AuthMode = new Array();
var EncrypType = new Array();
var DefaultKeyID = new Array();
var Key1Type = new Array();
var Key1Str = new Array();
var Key2Type = new Array();
var Key2Str = new Array();
var Key3Type = new Array();
var Key3Str = new Array();
var Key4Type = new Array();
var Key4Str = new Array();
var WPAPSK = new Array();
var RekeyMethod = new Array();
var RekeyInterval = new Array();
var PMKCachePeriod = new Array();
var IEEE8021X = new Array();
var RADIUS_Server = new Array();
var RADIUS_Port = new Array();
var RADIUS_Key = new Array();
var session_timeout_interval = new Array();
var AccessPolicy = new Array();
var AccessControlList = new Array();
var DefaultWEPKey = new Array();

function show_div(show,id) {
	if(show)
		document.getElementById(id).className  = "on" ;
	else
		document.getElementById(id).className  = "off" ;
}



function checkMac(str){
	var len = str.length;
	if(len!=17)
		return false;

	for (var i=0; i<str.length; i++) {
		if((i%3) == 2){
			if(str.charAt(i) == ':')
				continue;
		}else{
			if (    (str.charAt(i) >= '0' && str.charAt(i) <= '9') ||
					(str.charAt(i) >= 'a' && str.charAt(i) <= 'f') ||
					(str.charAt(i) >= 'A' && str.charAt(i) <= 'F') )
			continue;
		}
		return false;
	}
	return true;
}

function checkRange(str, num, min, max)
{
       var k = 0;
	 for (var i=0; i<str.length; i++) {
		if (str.charAt(i) == '.')
			k = k+1;
		continue;
	}
	if(k > 3){
             return false;
	}
	d = atoi(str,num);
	if(d > max || d < min)
		return false;
	return true;
}

function checkIpAddr(field)
{
	if(field.value == "")
	return false;

	if ( checkAllNum(field.value) == 0)
		return false;

	if( (!checkRange(field.value,1,0,239)) ||
		(!checkRange(field.value,2,0,255)) ||
		(!checkRange(field.value,3,0,255)) ||
		(!checkRange(field.value,4,1,254)) ){
			return false;
	}
	return true;
}

function isAllNum(str)
{
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9') || (str.charAt(i) == '.' ))
			continue;
		return 0;
	}
	return 1;
}

function checkIpAddrX(field, ismask)
{
	if (field.value == "") {
		alert("Error. IP address is empty.");
		field.value = field.defaultValue;
		field.focus();
		return false;
	}

	if (isAllNum(field.value) == 0) {
		alert('It should be a [0-9] number.');
		field.value = field.defaultValue;
		field.focus();
		return false;
	}
    /*
	if (ismask) {
		if ((!checkRange(field.value, 1, 0, 256)) ||
				(!checkRange(field.value, 2, 0, 256)) ||
				(!checkRange(field.value, 3, 0, 256)) ||
				(!checkRange(field.value, 4, 0, 256)))
		{
			alert('IP adress format error.');
			field.value = field.defaultValue;
			field.focus();
			return false;
		}
	}
	*/
	//aron modify to limit max to 255
	if (ismask) {
		if ((!checkRange(field.value, 1, 0, 255)) ||
				(!checkRange(field.value, 2, 0, 255)) ||
				(!checkRange(field.value, 3, 0, 255)) ||
				(!checkRange(field.value, 4, 0, 255)))
		{
			alert('IP adress format error.');
			field.value = field.defaultValue;
			field.focus();
			return false;
		}
	}
	else {
		if ((!checkRange(field.value, 1, 0, 255)) ||
				(!checkRange(field.value, 2, 0, 255)) ||
				(!checkRange(field.value, 3, 0, 255)) ||
				(!checkRange(field.value, 4, 1, 254)))
		{
			alert('IP adress format error.');
			field.value = field.defaultValue;
			field.focus();
			return false;
		}
	}
	return true;
}

function atoi(str, num)
{
	i=1;
	if(num != 1 ){
		while (i != num && str.length != 0){
			if(str.charAt(0) == '.'){
			i++;
		}
		str = str.substring(1);
	}
	
        if(i != num )
		return -1;
	}

	for(i=0; i<str.length; i++){
		if(str.charAt(i) == '.'){
			str = str.substring(0, i);
			break;
		}
	}
	
	if(str.length == 0)
		return -1;

	return parseInt(str, 10);
}

function checkSubnet(ip, mask, client)
{ 
  ip_d = atoi(ip, 1);
  mask_d = atoi(mask, 1);
  client_d = atoi(client, 1);
  if ( (ip_d & mask_d) != (client_d & mask_d ) )
	return false;

  ip_d = atoi(ip, 2);
  mask_d = atoi(mask, 2);
  client_d = atoi(client, 2);
  if ( (ip_d & mask_d) != (client_d & mask_d ) )
	return false;

  ip_d = atoi(ip, 3);
  mask_d = atoi(mask, 3);
  client_d = atoi(client, 3);
  if ( (ip_d & mask_d) != (client_d & mask_d ) )
	return false;

  ip_d = atoi(ip, 4);
  mask_d = atoi(mask, 4);
  client_d = atoi(client, 4);
  if ( (ip_d & mask_d) != (client_d & mask_d ) )
	return false;

  return true;
}

function checkHex(str){
	var len = str.length;

	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9') ||
			(str.charAt(i) >= 'a' && str.charAt(i) <= 'f') ||
			(str.charAt(i) >= 'A' && str.charAt(i) <= 'F') ){
				continue;
		}else
	        return false;
	}
	return true;
}

function checkInjection(str)
{
	var len = str.length;
	for (var i=0; i<str.length; i++) {
		if (str.charAt(i) == '\r' || str.charAt(i) == '\n' || str.charAt(i) == '^' || str.charAt(i) == '|'|| str.charAt(i) == '$' || str.charAt(i) == '`' || str.charAt(i) == ',' || str.charAt(i) == '"'||str.charAt(i) == '%'||str.charAt(i) == '&'|| str.charAt(i) == ';'){
			return false;

		}else
	        continue;
	}
	return true;
}

function checkStrictInjection(str)
{
	var len = str.length;
	for (var i=0; i<str.length; i++) {
		if ( str.charAt(i) == ';' || str.charAt(i) == ',' ||
			 str.charAt(i) == '\r' || str.charAt(i) == '\n'){
				return false;
		}else
	        continue;
	}
	return true;
}

function checkAllNum(str)
{
	for (var i=0; i<str.length; i++){
		if((str.charAt(i) >= '0' && str.charAt(i) <= '9') || (str.charAt(i) == '.' ))
			continue;
		return false;
	}
	return true;
}

function style_display_on()
{
	if (window.ActiveXObject) { // IE
		return "block";
	}
	else if (window.XMLHttpRequest) { // Mozilla, Safari,...
		return "table-row";
	}
}

var http_request = false;
function makeRequest(url, content, handler) {
	http_request = false;
	if (window.XMLHttpRequest) { // Mozilla, Safari,...
		http_request = new XMLHttpRequest();
		if (http_request.overrideMimeType) {
			http_request.overrideMimeType('text/xml');
		}
	} else if (window.ActiveXObject) { // IE
		try {
			http_request = new ActiveXObject("Msxml2.XMLHTTP");
		} catch (e) {
			try {
			http_request = new ActiveXObject("Microsoft.XMLHTTP");
			} catch (e) {}
		}
	}
	if (!http_request) {
		alert('Giving up :( Cannot create an XMLHTTP instance');
		return false;
	}
	http_request.onreadystatechange = handler;
	http_request.open('POST', url, true);
	http_request.send(content);
}

function securityHandler() {
	if (http_request.readyState == 4) {
		if (http_request.status == 200) {
			parseAllData(http_request.responseText);
			UpdateMBSSIDList();
			LoadFields(defaultShownMBSSID);
		} else {
			alert('There was a problem with the request.');
		}
	}
}

function deleteAccessPolicyListHandler()
{
	window.location.reload(false);
}


function parseAllData(str)
{
	var wepdefaultkey = "<% getCfgZero(1, "DefaultWEPKey"); %>";
	var all_str = new Array();
	all_str = str.split("\n");

	defaultShownMBSSID = parseInt(all_str[0]);
	for (var i=0; i<all_str.length-2; i++) {
		var fields_str = new Array();
		fields_str = all_str[i+1].split("\r");

		SSID[i] = fields_str[0];
		PreAuth[i] = fields_str[1];
		AuthMode[i] = fields_str[2];
		EncrypType[i] = fields_str[3];
		DefaultKeyID[i] = fields_str[4];
		Key1Type[i] = fields_str[5];
		Key1Str[i] = fields_str[6];
		Key2Type[i] = fields_str[7];
		Key2Str[i] = fields_str[8];
		Key3Type[i] = fields_str[9];
		Key3Str[i] = fields_str[10];
		Key4Type[i] = fields_str[11];
		Key4Str[i] = fields_str[12];
		WPAPSK[i] = fields_str[13];
		RekeyMethod[i] = fields_str[14];
		RekeyInterval[i] = fields_str[15];
		PMKCachePeriod[i] = fields_str[16];
		IEEE8021X[i] = fields_str[17];
		RADIUS_Server[i] = fields_str[18];
		RADIUS_Port[i] = fields_str[19];
		RADIUS_Key[i] = fields_str[20];
		session_timeout_interval[i] = fields_str[21];
		AccessPolicy[i] = fields_str[22];
		AccessControlList[i] = fields_str[23];

		DefaultWEPKey[i] = wepdefaultkey.split(";",i+1);

		/* !!!! IMPORTANT !!!!*/
		if(IEEE8021X[i] == "1")
			AuthMode[i] = "IEEE8021X";

		if(AuthMode[i] == "OPEN" && EncrypType[i] == "NONE" && IEEE8021X[i] == "0")
			AuthMode[i] = "Disable";
	}
}

function clickAutoChannel()
{
	if (document.wireless_basic.Auto_Channel.checked == true) {
		document.wireless_basic.sz11gChannel.disabled = true;
		show_div(false, "div_extension_channel");	
		document.wireless_basic.n_extcha.disabled = true;
	}
	else {
		document.wireless_basic.sz11gChannel.disabled = false;	
		if (document.wireless_basic.n_bandwidth.options.selectedIndex == 1)
		{
			show_div(true, "div_extension_channel");	
			document.wireless_basic.n_extcha.disabled = false;
		}
	}
}

function Channel_BandWidth_onChange()
{
	if (document.wireless_basic.n_bandwidth.options.selectedIndex == 0)
	{
		show_div(false, "div_extension_channel");	
		document.wireless_basic.n_extcha.disabled = true;
	}
	else
	{
		if (document.wireless_basic.Auto_Channel.checked == false) {
			show_div(true, "div_extension_channel");	
			document.wireless_basic.n_extcha.disabled = false;
			refreshExtChannel();
		}
	}
	parent.adjustMyFrameHeight();
}

function wirelessModeChange()
{
	show_div(false, "div_ht_phy_1_3");
	show_div(false, "div_extension_channel");
	show_div(false, "div_networkmode_id");

	
	if (wirelessmode == 3){ // WDS Mode
		show_div(false, "div_ht_phy_1_3");
		show_div(false, "div_extension_channel");
	}else{
		show_div(true, "div_ht_phy_1_3");
		show_div(true, "div_extension_channel");
	    show_div(true, "div_networkmode_id");
	}

	Channel_BandWidth_onChange();
	parent.adjustMyFrameHeight();
}

function checkData()
{

	return true;//Arthur Chow 2009-06-09
	
	var securitymode;
	securitymode = document.wireless_basic.security_mode.value;

	if (securitymode == "OPEN" || securitymode == "SHARED" ||securitymode == "WEPAUTO")
	{
		if(! check_Wep(securitymode) )
			return false;
	}else if (securitymode == "WPAPSK" || securitymode == "WPA2PSK" || securitymode == "WPAPSKWPA2PSK" /* || security_mode == 5 */){
		var keyvalue = document.wireless_basic.passphrase.value;

		if (keyvalue.length == 0){
			alert('Please input wpapsk key!');
			return false;
		}

		if (keyvalue.length < 8){
			alert('Please input at least 8 character of wpapsk key!');
			return false;
		}
		
		if(checkInjection(document.wireless_basic.passphrase.value) == false){
			alert('Invalid characters in Pass Phrase.');
			return false;
		}

		if(checkAllNum(document.wireless_basic.keyRenewalInterval.value) == false){
			alert('Please input a valid key renewal interval');
			return false;
		}
		if(document.wireless_basic.keyRenewalInterval.value < 60){
			alert('Warning: A short key renewal interval.');
		}
		if(check_wpa() == false)
			return false;
	}
	//802.1x
	else if (securitymode == "IEEE8021X") // 802.1x
	{
		if( document.wireless_basic.ieee8021x_wep[0].checked == false &&
			document.wireless_basic.ieee8021x_wep[1].checked == false){
			alert('Please choose the 802.1x WEP option.');
			return false;
		}
		if(check_radius() == false)
			return false;
	}else if (securitymode == "WPA" || securitymode == "WPA1WPA2") //     WPA or WPA1WP2 mixed mode
	{
		if(check_wpa() == false)
			return false;
		if(check_radius() == false)
			return false;
	}else if (securitymode == "WPA2") // WPA2
	{
		if(check_wpa() == false)
			return false;
		if( document.wireless_basic.PreAuthentication[0].checked == false &&
			document.wireless_basic.PreAuthentication[1].checked == false){
			alert('Please choose the Pre-Authentication options.');
			return false;
		}

		if(!document.wireless_basic.PMKCachePeriod.value.length){
			alert('Please input the PMK Cache Period.');
			return false;
		}
		if(check_radius() == false)
			return false;
	}

	return true;
}

function check_wpa()
{
		if(checkAllNum(document.wireless_basic.keyRenewalInterval.value) == false){
			alert('Please input a valid key renewal interval');
			return false;
		}
		if(document.wireless_basic.keyRenewalInterval.value < 60){
			alert('Warning: A short key renewal interval.');
		}
		return true;
}

function check_radius()
{
	if(!document.wireless_basic.RadiusServerIP.value.length){
		alert('Please input the radius server ip address.');
		return false;		
	}
	if(!document.wireless_basic.RadiusServerPort.value.length){
		alert('Please input the radius server port number.');
		return false;		
	}
	if(!document.wireless_basic.RadiusServerSecret.value.length){
		alert('Please input the radius server shared secret.');
		return false;		
	}

	if(checkIpAddr(document.wireless_basic.RadiusServerIP) == false){
		alert('Please input a valid radius server ip address.');
		return false;		
	}
	if( (checkRange(document.wireless_basic.RadiusServerPort.value, 1, 1, 65535)==false) ||
		(checkAllNum(document.wireless_basic.RadiusServerPort.value)==false)){
		alert('Please input a valid radius server port number.');
		return false;		
	}
	if(checkStrictInjection(document.wireless_basic.RadiusServerSecret.value)==false){
		alert('The shared secret contains invalid characters.');
		return false;		
	}

	if(document.wireless_basic.RadiusServerSessionTimeout.value.length){
		if(checkAllNum(document.wireless_basic.RadiusServerSessionTimeout.value)==false){
			alert('Please input a valid session timeout number or u may left it empty.');
			return false;	
		}	
	}

	return true;
}

function securityMode(c_f)
{
	var security_mode;


	changed = c_f;

	hideWep();

	show_div(false, "div_wpapsk_compatible");
	show_div(false, "div_wpa_compatible");	
	
	show_div(false, "wpa_passphrase");
	show_div(false, "wpa_key_renewal_interval");
	show_div(false, "wpa_PMK_Cache_Period");
	show_div(false, "wpa_preAuthentication");
	
	document.wireless_basic.passphrase.disabled = true;
	document.wireless_basic.keyRenewalInterval.disabled = true;
	document.wireless_basic.PMKCachePeriod.disabled = true;
	document.wireless_basic.PreAuthentication.disabled = true;

	// 802.1x
	show_div(false, "div_radius_server");
	show_div(false, "div_8021x_wep");
	document.wireless_basic.ieee8021x_wep.disable = true;
	document.wireless_basic.RadiusServerIP.disable = true;
	document.wireless_basic.RadiusServerPort.disable = true;
	document.wireless_basic.RadiusServerSecret.disable = true;	
	document.wireless_basic.RadiusServerSessionTimeout.disable = true;
	document.wireless_basic.RadiusServerIdleTimeout.disable = true;	

	security_mode = document.wireless_basic.security_mode.value;

	if (security_mode == "OPEN" || security_mode == "SHARED" ||security_mode == "WEPAUTO"){
		showWep(security_mode);
	}else if (security_mode == "WPAPSK" || security_mode == "WPA2PSK" || security_mode == "WPAPSKWPA2PSK"){
		<!-- WPA -->
		
		if (security_mode == "WPA2PSK" || security_mode == "WPAPSKWPA2PSK"){	
			show_div(true, "div_wpapsk_compatible");
		}	
		show_div(true, "wpa_passphrase");
		document.wireless_basic.passphrase.disabled = false;

		show_div(true, "wpa_key_renewal_interval");
		document.wireless_basic.keyRenewalInterval.disabled = false;
	}else if (security_mode == "WPA" || security_mode == "WPA2" || security_mode == "WPA1WPA2") //wpa enterprise
	{
	
		if (security_mode == "WPA2" || security_mode == "WPA1WPA2"){ //wpa enterprise	
	 	    show_div(true, "div_wpa_compatible");			
		}
		
		show_div(true, "wpa_key_renewal_interval");
		document.wireless_basic.keyRenewalInterval.disabled = false;
	
		<!-- 802.1x -->
		show_div(true, "div_radius_server");
		document.wireless_basic.RadiusServerIP.disable = false;
		document.wireless_basic.RadiusServerPort.disable = false;
		document.wireless_basic.RadiusServerSecret.disable = false;	
		document.wireless_basic.RadiusServerSessionTimeout.disable = false;
		document.wireless_basic.RadiusServerIdleTimeout.disable = false;	

		if(security_mode == "WPA2"){
			show_div(true, "wpa_preAuthentication");
			document.wireless_basic.PreAuthentication.disabled = false;
			show_div(true, "wpa_PMK_Cache_Period");
			document.wireless_basic.PMKCachePeriod.disabled = false;
		}
	}else if (security_mode == "IEEE8021X"){ // 802.1X-WEP
		show_div(true, "div_8021x_wep");
		show_div(true, "div_radius_server");
		document.wireless_basic.ieee8021x_wep.disable = false;
		document.wireless_basic.RadiusServerIP.disable = false;
		document.wireless_basic.RadiusServerPort.disable = false;
		document.wireless_basic.RadiusServerSecret.disable = false;	
		document.wireless_basic.RadiusServerSessionTimeout.disable = false;
	}
	
	show_div(true, "div_note2_id");
	parent.adjustMyFrameHeight();
}


function hideWep()
{
	show_div(false, "div_wep");
}

function showWep(mode)
{
	show_div(true, "div_wep");
}


function check_Wep(securitymode)
{
	var defaultid, i;
	for (i=0; i<=3; i++){	
		if (document.wireless_basic.DefWEPKey[i].checked == true){
			defaultid = i;
		}
	}	
	defaultid = defaultid+1;

	if ( defaultid == 1 )
		var keyvalue = document.wireless_basic.wep_key_1.value;
	else if (defaultid == 2)
		var keyvalue = document.wireless_basic.wep_key_2.value;
	else if (defaultid == 3)
		var keyvalue = document.wireless_basic.wep_key_3.value;
	else if (defaultid == 4)
		var keyvalue = document.wireless_basic.wep_key_4.value;

	if (keyvalue.length == 0 &&  (securitymode == "SHARED" || securitymode == "OPEN")){ // shared wep  || md5
		alert('Please input wep key'+defaultid+' !');
		return false;
	}

	var keylength = document.wireless_basic.wep_key_1.value.length;
	if (keylength != 0){
		if (document.wireless_basic.WEPKey_Code[0].checked == true){ //ASCII
			if (document.getElementById("wep_encry").selectedIndex == 0 ){ // 64-bits (ASCII)
				if(keylength != 5) {
					alert('Please input 5 characters of wep key1 !');
					return false;
				}
				if(checkInjection(document.wireless_basic.wep_key_1.value)== false){
					alert('Wep key1 contains invalid characters.');
					return false;
				}
			}else{ // 128-bits (ASCII)
				if(keylength != 13) {
					alert('Please input 13 characters of wep key1 !');
					return false;
				}
				if(checkInjection(document.wireless_basic.wep_key_1.value)== false){
					alert('Wep key1 contains invalid characters.');
					return false;
				}
			}
		}else{ //HEX
			if (document.getElementById("wep_encry").selectedIndex == 0 ){ // 64-bits (HEX)
				if(keylength != 10) {
					alert('Please input 10 characters of wep key1 !');
					return false;
				}
				if(checkHex(document.wireless_basic.wep_key_1.value) == false){
					alert('Invalid Wep key1 format!');
					return false;
				}			
			}else{ // 128-bits (HEX)
				if(keylength != 26) {
					alert('Please input 26 characters of wep key1 !');
					return false;
				}
				if(checkHex(document.wireless_basic.wep_key_1.value) == false){
					alert('Invalid Wep key1 format!');
					return false;
				}							
			}		
		
		}
	}

	var keylength = document.wireless_basic.wep_key_2.value.length;
	if (keylength != 0){
		if (document.wireless_basic.WEPKey_Code[0].checked == true){ //ASCII
			if (document.getElementById("wep_encry").selectedIndex == 0 ){ // 64-bits (ASCII)
				if(keylength != 5) {
					alert('Please input 5 characters of wep key2 !');
					return false;
				}
				if(checkInjection(document.wireless_basic.wep_key_2.value)== false){
					alert('Wep key2 contains invalid characters.');
					return false;
				}
			}else{ // 128-bits (ASCII)
				if(keylength != 13) {
					alert('Please input 13 characters of wep key2 !');
					return false;
				}
				if(checkInjection(document.wireless_basic.wep_key_2.value)== false){
					alert('Wep key2 contains invalid characters.');
					return false;
				}
			}
		}else{ //HEX
			if (document.getElementById("wep_encry").selectedIndex == 0 ){ // 64-bits (HEX)
				if(keylength != 10) {
					alert('Please input 10 characters of wep key2 !');
					return false;
				}
				if(checkHex(document.wireless_basic.wep_key_2.value) == false){
					alert('Invalid Wep key2 format!');
					return false;
				}			
			}else{ // 128-bits (HEX)
				if(keylength != 26) {
					alert('Please input 26 characters of wep key2 !');
					return false;
				}
				if(checkHex(document.wireless_basic.wep_key_2.value) == false){
					alert('Invalid Wep key2 format!');
					return false;
				}							
			}		
		
		}
	}

	var keylength = document.wireless_basic.wep_key_3.value.length;
	if (keylength != 0){
		if (document.wireless_basic.WEPKey_Code[0].checked == true){ //ASCII
			if (document.getElementById("wep_encry").selectedIndex == 0 ){ // 64-bits (ASCII)
				if(keylength != 5) {
					alert('Please input 5 characters of wep key3 !');
					return false;
				}
				if(checkInjection(document.wireless_basic.wep_key_3.value)== false){
					alert('Wep key3 contains invalid characters.');
					return false;
				}
			}else{ // 128-bits (ASCII)
				if(keylength != 13) {
					alert('Please input 13 characters of wep key3 !');
					return false;
				}
				if(checkInjection(document.wireless_basic.wep_key_3.value)== false){
					alert('Wep key3 contains invalid characters.');
					return false;
				}
			}
		}else{ //HEX
			if (document.getElementById("wep_encry").selectedIndex == 0 ){ // 64-bits (HEX)
				if(keylength != 10) {
					alert('Please input 10 characters of wep key3 !');
					return false;
				}
				if(checkHex(document.wireless_basic.wep_key_3.value) == false){
					alert('Invalid Wep key3 format!');
					return false;
				}			
			}else{ // 128-bits (HEX)
				if(keylength != 26) {
					alert('Please input 26 characters of wep key3 !');
					return false;
				}
				if(checkHex(document.wireless_basic.wep_key_3.value) == false){
					alert('Invalid Wep key3 format!');
					return false;
				}							
			}		
		
		}
	}	

	var keylength = document.wireless_basic.wep_key_4.value.length;
	if (keylength != 0){
		if (document.wireless_basic.WEPKey_Code[0].checked == true){ //ASCII
			if (document.getElementById("wep_encry").selectedIndex == 0 ){ // 64-bits (ASCII)
				if(keylength != 5) {
					alert('Please input 5 characters of wep key4 !');
					return false;
				}
				if(checkInjection(document.wireless_basic.wep_key_4.value)== false){
					alert('Wep key4 contains invalid characters.');
					return false;
				}
			}else{ // 128-bits (ASCII)
				if(keylength != 13) {
					alert('Please input 13 characters of wep key4 !');
					return false;
				}
				if(checkInjection(document.wireless_basic.wep_key_4.value)== false){
					alert('Wep key4 contains invalid characters.');
					return false;
				}
			}
		}else{ //HEX
			if (document.getElementById("wep_encry").selectedIndex == 0 ){ // 64-bits (HEX)
				if(keylength != 10) {
					alert('Please input 10 characters of wep key4 !');
					return false;
				}
				if(checkHex(document.wireless_basic.wep_key_4.value) == false){
					alert('Invalid Wep key4 format!');
					return false;
				}			
			}else{ // 128-bits (HEX)
				if(keylength != 26) {
					alert('Please input 26 characters of wep key4 !');
					return false;
				}
				if(checkHex(document.wireless_basic.wep_key_4.value) == false){
					alert('Invalid Wep key4 format!');
					return false;
				}							
			}		
		
		}
	}	


	return true;
}

function check_same_ssid(){
      for (var i = 1; i < 8; i++){
		if (eval("document.wireless_basic.mssid_"+i).value != ""){
			if (i == 1){
				if (eval(document.wireless_basic.ssid).value == eval(document.wireless_basic.mssid_1).value ){
					return false;	
				}
				if (eval(document.wireless_basic.ssid).value == eval(document.wireless_basic.mssid_2).value ){
					return false;	
				}
				if (eval(document.wireless_basic.ssid).value == eval(document.wireless_basic.mssid_3).value ){
					return false;	
				}	
				if (eval(document.wireless_basic.ssid).value == eval(document.wireless_basic.mssid_4).value ){
					return false;	
				}	
				if (eval(document.wireless_basic.ssid).value == eval(document.wireless_basic.mssid_5).value ){
					return false;	
				}	
				if (eval(document.wireless_basic.ssid).value == eval(document.wireless_basic.mssid_6).value ){
					return false;	
				}	
				if (eval(document.wireless_basic.ssid).value == eval(document.wireless_basic.mssid_7).value ){
					return false;	
				}	
			}
			else if (i == 2){
				if (eval(document.wireless_basic.mssid_1).value == eval(document.wireless_basic.mssid_2).value ){
					return false;	
				}
				if (eval(document.wireless_basic.mssid_1).value == eval(document.wireless_basic.mssid_3).value ){
					return false;	
				}			
				if (eval(document.wireless_basic.mssid_1).value == eval(document.wireless_basic.mssid_4).value ){
					return false;	
				}			
				if (eval(document.wireless_basic.mssid_1).value == eval(document.wireless_basic.mssid_5).value ){
					return false;	
				}			
				if (eval(document.wireless_basic.mssid_1).value == eval(document.wireless_basic.mssid_6).value ){
					return false;	
				}			
				if (eval(document.wireless_basic.mssid_1).value == eval(document.wireless_basic.mssid_7).value ){
					return false;	
				}			
			}
			else if (i == 3){
				if (eval(document.wireless_basic.mssid_2).value == eval(document.wireless_basic.mssid_3).value ){
					return false;	
				}
				if (eval(document.wireless_basic.mssid_2).value == eval(document.wireless_basic.mssid_4).value ){
					return false;	
				}
				if (eval(document.wireless_basic.mssid_2).value == eval(document.wireless_basic.mssid_5).value ){
					return false;	
				}
				if (eval(document.wireless_basic.mssid_2).value == eval(document.wireless_basic.mssid_6).value ){
					return false;	
				}
				if (eval(document.wireless_basic.mssid_2).value == eval(document.wireless_basic.mssid_7).value ){
					return false;	
				}
			}
			else if (i == 4){
				if (eval(document.wireless_basic.mssid_3).value == eval(document.wireless_basic.mssid_4).value ){
					return false;	
				}
				if (eval(document.wireless_basic.mssid_3).value == eval(document.wireless_basic.mssid_5).value ){
					return false;	
				}
				if (eval(document.wireless_basic.mssid_3).value == eval(document.wireless_basic.mssid_6).value ){
					return false;	
				}
				if (eval(document.wireless_basic.mssid_3).value == eval(document.wireless_basic.mssid_7).value ){
					return false;	
				}
			}
			else if (i == 5){
				if (eval(document.wireless_basic.mssid_4).value == eval(document.wireless_basic.mssid_5).value ){
					return false;	
				}
				if (eval(document.wireless_basic.mssid_4).value == eval(document.wireless_basic.mssid_6).value ){
					return false;	
				}
				if (eval(document.wireless_basic.mssid_4).value == eval(document.wireless_basic.mssid_7).value ){
					return false;	
				}
			}
			else if (i == 6){
				if (eval(document.wireless_basic.mssid_5).value == eval(document.wireless_basic.mssid_6).value ){
					return false;	
				}
				if (eval(document.wireless_basic.mssid_5).value == eval(document.wireless_basic.mssid_7).value ){
					return false;	
				}
			}
			else if (i == 7){
				if (eval(document.wireless_basic.mssid_6).value == eval(document.wireless_basic.mssid_7).value ){
					return false;	
				}
			}
		}
	}
       return true;
}

function submit_apply()
{
	var submit_ssid_num;
	
	if ((wirelessmode == 2) || ((wirelessmode == 6) && (wispmode == 0))){
	    alert("You CANNOT select WISP or Client mode to configure this page");	
	    return false;
	}
	
	if (((document.wireless_basic.mssid_1.value == "") && (document.wireless_basic.hidemssid_1.checked == true)) ||
	    ((document.wireless_basic.mssid_2.value == "") && (document.wireless_basic.hidemssid_2.checked == true)) ||
	    ((document.wireless_basic.mssid_3.value == "") && (document.wireless_basic.hidemssid_3.checked == true)) ||
	    ((document.wireless_basic.mssid_4.value == "") && (document.wireless_basic.hidemssid_4.checked == true)) ||
	    ((document.wireless_basic.mssid_5.value == "") && (document.wireless_basic.hidemssid_5.checked == true)) ||
	    ((document.wireless_basic.mssid_6.value == "") && (document.wireless_basic.hidemssid_6.checked == true)) ||
	    ((document.wireless_basic.mssid_7.value == "") && (document.wireless_basic.hidemssid_7.checked == true)) 
		 ){
		alert("SSID field cannot empty when you select hide function");
		return false;
	}	
		
	if (((document.wireless_basic.mssid_1.value == "") && (document.wireless_basic.IntraBSS1.checked == true)) ||
	    ((document.wireless_basic.mssid_2.value == "") && (document.wireless_basic.IntraBSS2.checked == true)) ||
	    ((document.wireless_basic.mssid_3.value == "") && (document.wireless_basic.IntraBSS3.checked == true)) ||
	    ((document.wireless_basic.mssid_4.value == "") && (document.wireless_basic.IntraBSS4.checked == true)) ||
	    ((document.wireless_basic.mssid_5.value == "") && (document.wireless_basic.IntraBSS5.checked == true)) ||
	    ((document.wireless_basic.mssid_6.value == "") && (document.wireless_basic.IntraBSS6.checked == true)) ||
	    ((document.wireless_basic.mssid_7.value == "") && (document.wireless_basic.IntraBSS7.checked == true)) 
		 ){
		alert("SSID field cannot empty");
		return false;
	}
		
	if (document.wireless_basic.ssid.value == "")
	{
		alert("Please enter SSID!");
		document.wireless_basic.ssid.focus();
		document.wireless_basic.ssid.select();
		return false;
	}
	if(checkInjection(document.wireless_basic.ssid.value) == false){
		alert('System do not support specific \^\|\$ \,\"\`\%\&\; characters.');
		return false;
	}
	if(checkInjection(document.wireless_basic.mssid_1.value) == false){
		alert('System do not support specific \^\|\$ \,\"\`\%\&\; characters.');
		return false;
	}
	if(checkInjection(document.wireless_basic.mssid_2.value) == false){
		alert('System do not support specific \^\|\$ \,\"\`\%\&\; characters.');
		return false;
	}
	if(checkInjection(document.wireless_basic.mssid_3.value) == false){
		alert('System do not support specific \^\|\$ \,\"\`\%\&\; characters.');
		return false;
	}	
	if(checkInjection(document.wireless_basic.mssid_4.value) == false){
		alert('System do not support specific \^\|\$ \,\"\`\%\&\; characters.');
		return false;
	}	
	if(checkInjection(document.wireless_basic.mssid_5.value) == false){
		alert('System do not support specific \^\|\$ \,\"\`\%\&\; characters.');
		return false;
	}	
	if(checkInjection(document.wireless_basic.mssid_6.value) == false){
		alert('System do not support specific \^\|\$ \,\"\`\%\&\; characters.');
		return false;
	}	
	if(checkInjection(document.wireless_basic.mssid_7.value) == false){
		alert('System do not support specific \^\|\$ \,\"\`\%\&\; characters.');
		return false;
	}	
	if(check_same_ssid() == false){
		alert("Duplicate ssid names!");	
             return false;
	}
	submit_ssid_num = 1;

	for (i = 1; i < 8; i++)
	{
		if (eval("document.wireless_basic.mssid_"+i).value != "")
		{
			if (i == 8)
			{
				if (1*apcli_include == 0)
					submit_ssid_num++;
					
			}
			else
				submit_ssid_num++;
		}
	}

	document.wireless_basic.bssid_num.value = submit_ssid_num;		


	if(document.wireless_basic.mssid_1.value == "")
		document.wireless_basic.bssid_num1.value = "0";
	else
		document.wireless_basic.bssid_num1.value = "1";
	
	if(document.wireless_basic.mssid_2.value == "")
		document.wireless_basic.bssid_num2.value = "0";
	else
		document.wireless_basic.bssid_num2.value = "1";

	if(document.wireless_basic.mssid_3.value == "")
		document.wireless_basic.bssid_num3.value = "0";
	else
		document.wireless_basic.bssid_num3.value = "1";	

	if(document.wireless_basic.mssid_4.value == "")
		document.wireless_basic.bssid_num4.value = "0";
	else
		document.wireless_basic.bssid_num4.value = "1";	

	if(document.wireless_basic.mssid_5.value == "")
		document.wireless_basic.bssid_num5.value = "0";
	else
		document.wireless_basic.bssid_num5.value = "1";	

	if(document.wireless_basic.mssid_6.value == "")
		document.wireless_basic.bssid_num6.value = "0";
	else
		document.wireless_basic.bssid_num6.value = "1";	

	if(document.wireless_basic.mssid_7.value == "")
		document.wireless_basic.bssid_num7.value = "0";
	else
		document.wireless_basic.bssid_num7.value = "1";	


	if(	(document.wireless_basic.ssid.value == "") || 
		(document.wireless_basic.mssid_1.value == "") || 
		(document.wireless_basic.mssid_2.value == "") || 
		(document.wireless_basic.mssid_3.value == "") ||
		(document.wireless_basic.mssid_4.value == "") ||
		(document.wireless_basic.mssid_5.value == "") ||
		(document.wireless_basic.mssid_6.value == "") ||
		(document.wireless_basic.mssid_7.value == "") 
	){
		alert("Please enter SSID!");
		return false;
	}

	if ((wirelessmode == 0) && (document.wireless_basic.wlanguest.checked == true)){
		if(document.wireless_basic.wlanguestip.value == ""){
			alert("Error. IP address is empty.");
			return false;
		}
		if(!checkIpAddrX(document.wireless_basic.wlanguestip, false)){
			alert('IP address format error.');
			return false;
		}
		
		if(document.wireless_basic.wlanguestmask.value == ""){
			alert("Error. Invalid Subnet Mask.");
			return false;
		}
	    if (!checkIpAddrX(document.wireless_basic.wlanguestmask, true)){
	    	alert("Error. Invalid Subnet Mask.");
		    return false;
		}
		
		if (document.wireless_basic.guestmaxbw.value == "" ){
			alert("The value of bandwitdth can not accept empty.");
			return false;
		}
		
		
		//
		if(document.wireless_basic.wlanguestBW.checked == true){
			if(bm_en == 0){
				alert("Please Enable Bandwidth Management");
				return false;
			}
			
			dguest=atoi(document.wireless_basic.guestmaxbw.value, 1);
			ddbw=atoi(dBW, 1);
			dubw=atoi(uBW, 1);	
			if(ddbw >= dubw)
			 	aaa=dubw;
			else
				aaa=ddbw;	
			if (aaa == 64){
				alert("Please set value to 64");
				return false;
			}
			if (dguest < 64 || dguest > aaa ){
			alert("Please set value between 64~"+ aaa);
			return false;
			}
			/*
			if (document.wireless_basic.guestmaxbw.value < 64 || document.wireless_basic.guestmaxbw.value > 32768 ){
			alert("Please set value between 64~32768");
			return false;
			}*/
		}
		
		if ( checkSubnet(document.wireless_basic.wlanguestip.value, document.wireless_basic.wlanguestmask.value, document.wireless_basic.lanIP.value)) {
        		alert('Invalid  ip address!');
        		return false;
    	}
	
	}

	if (checkData() == true){
		changed = 0;
		
		showWebMessage(2, "");
		document.wireless_basic.submit();
	}
}

function LoadFields(MBSSID)
{
	var result;
	sp_select = document.getElementById("security_mode");

	sp_select.options.length = 0;
    
	wpsenable = <% getWPSModeASP(); %>;

	if(wpsenable ==0){
		// disable WPS
		sp_select.options[sp_select.length] = new Option(_("general nosecurity"),"Disable",	false, AuthMode[MBSSID] == "Disable");
		sp_select.options[sp_select.length] = new Option(_("general static wep"),"SHARED", 	false, AuthMode[MBSSID] == "SHARED");
    		sp_select.options[sp_select.length] = new Option(_("general wpapsk"), "WPAPSK",	false, AuthMode[MBSSID] == "WPAPSK");
    		sp_select.options[sp_select.length] = new Option(_("general wpa"),	"WPA",		false, AuthMode[MBSSID] == "WPA");        
    		sp_select.options[sp_select.length] = new Option(_("general wpa2psk"),"WPA2PSK",	false, AuthMode[MBSSID] == "WPA2PSK");
    		sp_select.options[sp_select.length] = new Option(_("general wpa2"),	"WPA2",		false, AuthMode[MBSSID] == "WPA2");
	}else{
		// enable WPS
		sp_select.options[sp_select.length] = new Option(_("general nosecurity"),"Disable",	false, AuthMode[MBSSID] == "Disable");
		sp_select.options[sp_select.length] = new Option(_("general static wep"),"SHARED", 	false, AuthMode[MBSSID] == "SHARED");
		sp_select.options[sp_select.length] = new Option(_("general wpapsk"), "WPAPSK",	false, AuthMode[MBSSID] == "WPAPSK");
    		sp_select.options[sp_select.length] = new Option(_("general wpa2psk"),"WPA2PSK",	false, AuthMode[MBSSID] == "WPA2PSK");
	}

	// WEP
	document.getElementById("WEP1").value = Key1Str[MBSSID];
	document.getElementById("WEP2").value = Key2Str[MBSSID];
	document.getElementById("WEP3").value = Key3Str[MBSSID];
	document.getElementById("WEP4").value = Key4Str[MBSSID];

	// Now, 4 Keys type are same, so only get the first key to display HEX or ASCII. It's OK
	if (Key1Type[MBSSID] == "0"){
		document.wireless_basic.WEPKey_Code[1].checked = true; //hex

		if (Key1Str[MBSSID] != ""){
			if (Key1Str[MBSSID].length > 10){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;
			}
		}else if (Key2Str[MBSSID] != ""){
			if (Key2Str[MBSSID].length > 10){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;
			}
		}else if (Key3Str[MBSSID] != ""){
			if (Key3Str[MBSSID].length > 10){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;
			}
		}else if (Key4Str[MBSSID] != ""){
			if (Key4Str[MBSSID].length > 10){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;
			}		
		}
	}else{
		document.wireless_basic.WEPKey_Code[0].checked = true; //ASCII

		if (Key1Str[MBSSID] != ""){
			if (Key1Str[MBSSID].length > 5){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;	
			}
		}else if (Key2Str[MBSSID] != ""){
			if (Key2Str[MBSSID].length > 5){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;	
			}
		}else if (Key3Str[MBSSID] != ""){
			if (Key3Str[MBSSID].length > 5){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;	
			}
		}else if (Key4Str[MBSSID] != ""){
			if (Key4Str[MBSSID].length > 5){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;	
			}		
		}
	}

	document.wireless_basic.DefWEPKey[0].checked= false;
	document.wireless_basic.DefWEPKey[1].checked= false;
	document.wireless_basic.DefWEPKey[2].checked= false;
	document.wireless_basic.DefWEPKey[3].checked= false;
	
	var defkey_index = parseInt(DefaultKeyID[MBSSID]) - 1;
//	var defkey_index = parseInt(DefaultWEPKey[MBSSID]) - 1;
	
	if ( (defkey_index >= 0) && (defkey_index <= 3) ){
	     document.wireless_basic.DefWEPKey[defkey_index].checked= true;
	}else {
	     document.wireless_basic.DefWEPKey[0].checked= true;	
	}     

	// Static WEP 	
	sp_select = document.getElementById("security_mode");	

	if (AuthMode[MBSSID] == "WEPAUTO")
		document.getElementById("security_mode").selectedIndex = 1;

// +++ Let default WEP authentication method from Shared to Auto
var LastAuthMode = "<% getCfgZero(1, "LastWEPAuthMethod"); %>";
if  ((LastAuthMode == "WEPAUTO") || (LastAuthMode == "SHARED")){
	if(LastAuthMode == "WEPAUTO"){
		document.getElementById("auth_method").selectedIndex = 0; // Auto
	}else if (LastAuthMode == "SHARED"){
		document.getElementById("auth_method").selectedIndex = 1; // Shared Key
	}else{
		document.getElementById("auth_method").selectedIndex = 0; // default Auto
	}	
}else{
	if(AuthMode[MBSSID] == "WEPAUTO")
		document.getElementById("auth_method").selectedIndex = 0; // Auto
	else if (AuthMode[MBSSID] == "SHARED")
		document.getElementById("auth_method").selectedIndex = 1; // Shared Key
	else
		document.getElementById("auth_method").selectedIndex = 0; // default Auto
}
// --- Let default WEP authentication method from Shared to Auto
 	
 	// WPA Compatible and WPA-PSK Compatible	
	if(AuthMode[MBSSID] == "WPAPSKWPA2PSK"){
		if(wpsenable ==0){	
		document.getElementById("security_mode").selectedIndex = 4;	
	   	document.wireless_basic.wpapsk_compatible.checked = true;	
	   	}else{
			document.getElementById("security_mode").selectedIndex = 3;
	   		document.wireless_basic.wpapsk_compatible.checked = true;	   	
	   	}
	}
 	
	if(AuthMode[MBSSID] == "WPA1WPA2"){
	   document.getElementById("security_mode").selectedIndex = 5;		
	   document.wireless_basic.wpa_compatible.checked = true;	
	}
 	
	document.getElementById("passphrase").value = WPAPSK[MBSSID];
	document.getElementById("keyRenewalInterval").value = RekeyInterval[MBSSID];
	document.getElementById("PMKCachePeriod").value = PMKCachePeriod[MBSSID];
	
	if(PreAuth[MBSSID] == "0")
		document.wireless_basic.PreAuthentication[0].checked = true;
	else
		document.wireless_basic.PreAuthentication[1].checked = true;

	//802.1x wep
	if(IEEE8021X[MBSSID] == "1"){
		if(EncrypType[MBSSID] == "WEP")
			document.wireless_basic.ieee8021x_wep[1].checked = true;
		else
			document.wireless_basic.ieee8021x_wep[0].checked = true;
	}
	
	document.getElementById("RadiusServerIP").value = RADIUS_Server[MBSSID];
	document.getElementById("RadiusServerPort").value = RADIUS_Port[MBSSID];
	document.getElementById("RadiusServerSecret").value = RADIUS_Key[MBSSID];			
	document.getElementById("RadiusServerSessionTimeout").value = session_timeout_interval[MBSSID];
	
	securityMode(0);

}


function selectMBSSIDChanged()
{
	// check if any security settings changed
	if(changed){
		ret = confirm("Are you sure to ignore changed?");
		if(!ret){
			document.wireless_basic.ssidIndex.options.selectedIndex = old_MBSSID;
			return false;
		}
		else
			changed = 0;
	}

	var selected = document.wireless_basic.ssidIndex.options.selectedIndex;

	// backup for user cancel action
	old_MBSSID = selected;

	MBSSIDChange(selected);
}

/*
 * When user select the different SSID, this function would be called.
 */ 
function MBSSIDChange(selected)
{
	// load wep/wpa/802.1x table for MBSSID[selected]
	LoadFields(selected);

	// radio button special case
	WPAAlgorithms = EncrypType[selected];
	IEEE8021XWEP = IEEE8021X[selected];
	PreAuthentication = PreAuth[selected];

	return true;
}


function delap(mbssid, num)
{
	makeRequest("/goform/APDeleteAccessPolicyList", mbssid+ "," +num, deleteAccessPolicyListHandler);
}

function initTranslation()
{

	var e = document.getElementById("GeneralWirelessSetup");
	e.innerHTML = _("general wireless setup");

	e = document.getElementById("GeneralWirelessLAN");
	e.innerHTML = _("general wireless lan");
	
	e = document.getElementById("GeneralWirelessSSID");
	e.innerHTML = _("general wireless ssid");
	
	e = document.getElementById("GeneralHideSSID1");
	//e.innerHTML = _("general hide ssid1");
	e.innerHTML = _("general mssid hide");
	
	e = document.getElementById("GeneralmSSIDHide1");
	e.innerHTML = _("general mssid hide");
	e = document.getElementById("GeneralmSSIDHide2");
	e.innerHTML = _("general mssid hide");
	e = document.getElementById("GeneralmSSIDHide3");
	e.innerHTML = _("general mssid hide");
	e = document.getElementById("GeneralmSSIDHide4");
	e.innerHTML = _("general mssid hide");
	e = document.getElementById("GeneralmSSIDHide5");
	e.innerHTML = _("general mssid hide");
	e = document.getElementById("GeneralmSSIDHide6");
	e.innerHTML = _("general mssid hide");
	e = document.getElementById("GeneralmSSIDHide7");
	e.innerHTML = _("general mssid hide");

	e = document.getElementById("AdvanceNetMode");
	e.innerHTML = _("advance net mode");

	e = document.getElementById("AdvanceHTChannelBW");
	e.innerHTML = _("advance ht channelbw");
	
	e = document.getElementById("GeneralChannelSelect");
	e.innerHTML = _("general channel select");

	e = document.getElementById("AdvanceHTExtChannel");
	e.innerHTML = _("advance ht extchannel");
	
	e = document.getElementById("GeneralAutoChannelSelect");
	e.innerHTML = _("general autochannel select");
	
	e = document.getElementById("GeneralOperatingChannel");
	e.innerHTML = _("general operating channel");
		
	e = document.getElementById("GeneralSSIDChoice");
	e.innerHTML = _("general ssid choice");

	e = document.getElementById("GeneralSecurityTitle");
	e.innerHTML = _("general security title");
	
	e = document.getElementById("GeneralSecureMode");
	e.innerHTML = _("general security mode");

	e = document.getElementById("GeneralWEPPassPhrase");
	e.innerHTML = _("general wep passphrase");

	e = document.getElementById("GeneralWEPEncryp");
	e.innerHTML = _("general wep encryp");

	e = document.getElementById("GeneralAuthMethod");
	e.innerHTML = _("general auth method");

	e = document.getElementById("GeneralWEPGenerate");
	e.value = _("general wep generate");

	e = document.getElementById("GeneralWEPEncryp64");
	e.innerHTML = _("general wep encryp64");
	
	e = document.getElementById("GeneralWEPEncryp128");
	e.innerHTML = _("general wep encryp128");

	e = document.getElementById("GeneralEncrypSharedKey");
	e.innerHTML = _("general encryp sharedkey");

	e = document.getElementById("GeneralEncrypAuto");
	e.innerHTML = _("general encryp auto");

	e = document.getElementById("GeneralNote1_1");
	e.innerHTML = _("general note1_1");
	
	e = document.getElementById("GeneralNote1_2");
	e.innerHTML = _("general note1_2");
	
	e = document.getElementById("GeneralNote1_3");
	e.innerHTML = _("general note1_3");
	
	e = document.getElementById("GeneralNote1_4");
	e.innerHTML = _("general note1_4");

	e = document.getElementById("GeneralWEPASCII");
	e.innerHTML = _("general wep ascii");
	
	e = document.getElementById("GeneralWEPHex");
	e.innerHTML = _("general wep hex");

	e = document.getElementById("GeneralWEPKEY1");
	e.innerHTML = _("general wep key1");

	e = document.getElementById("GeneralWEPKEY2");
	e.innerHTML = _("general wep key2");
	
	e = document.getElementById("GeneralWEPKEY3");
	e.innerHTML = _("general wep key3");
		
	e = document.getElementById("GeneralWEPKEY4");
	e.innerHTML = _("general wep key4");

	e = document.getElementById("GeneralWPAPSKCompatible");
	e.innerHTML = _("general wpapsk compatible");

	e = document.getElementById("GeneralWPACompatible");
	e.innerHTML = _("general wpa compatible");

	e = document.getElementById("GeneralWPAPSKPreSharedKey");
	e.innerHTML = _("general wpapsk presharedkey");	

	e = document.getElementById("GeneralWPAKeyRenewInterval");
	e.innerHTML = _("general wpa keyrenewinterval");
	
	e = document.getElementById("GeneralWPAPMKCachePeriod");
	e.innerHTML = _("general wpa pmkcacheperiod");
	
	e = document.getElementById("GeneralWPAkeyRenewalIntervalUnit");
	e.innerHTML = _("general wpa keyrenewalintervalunit");

	e = document.getElementById("GeneralWPAPMKCachePeriodUnit");
	e.innerHTML = _("general wpa pmkcacheperiodunit");

	e = document.getElementById("GeneralWPAPreAuth");
	e.innerHTML = _("general wpa preauth");

	e = document.getElementById("GeneralWPAPreAuthDisable");
	e.innerHTML = _("general wpa preauthdisable");

	e = document.getElementById("GeneralWPAPreAuthEnable");
	e.innerHTML = _("general wpa preauthenable");

	e = document.getElementById("GeneralIEEE8021xSettings");
	e.innerHTML = _("general ieee8021x settings");

	e = document.getElementById("General1XWEP");
	e.innerHTML = _("general ieee8021x wep");

	e = document.getElementById("General1XWEPDisable");
	e.innerHTML = _("general ieee8021x wepdisable");

	e = document.getElementById("General1XWEPEnable");
	e.innerHTML = _("general ieee8021x wepenable");

	e = document.getElementById("GeneralAuthServer");
	e.innerHTML = _("general auth server");

	e = document.getElementById("GeneralRadiusIPAddr");
	e.innerHTML = _("general radius ipaddr");

	e = document.getElementById("GeneralRadiusPort");
	e.innerHTML = _("general radius port");

	e = document.getElementById("GeneralRadiusSharedSecret");
	e.innerHTML = _("general radius sharedsecret");

	e = document.getElementById("GeneralRadiusSessionTimeout");
	e.innerHTML = _("general radius sessiontimeout");

	e = document.getElementById("GeneralRadiusIdleTimeout");
	e.innerHTML = _("general radius idletimeout");

	e = document.getElementById("GeneralNote2");
	e.innerHTML = _("general note_2");	

	e = document.getElementById("GeneralApply");
	e.value = _("general apply");

	e = document.getElementById("GeneralReset");
	e.value = _("general reset");

	e = document.getElementById("IntraBSS_0");
	e.innerHTML = _("advance intra bss");	
	e = document.getElementById("IntraBSS_1");
	e.innerHTML = _("advance intra bss");	
	e = document.getElementById("IntraBSS_2");
	e.innerHTML = _("advance intra bss");	
	e = document.getElementById("IntraBSS_3");
	e.innerHTML = _("advance intra bss");	
	e = document.getElementById("IntraBSS_4");
	e.innerHTML = _("advance intra bss");	
	e = document.getElementById("IntraBSS_5");
	e.innerHTML = _("advance intra bss");	
	e = document.getElementById("IntraBSS_6");
	e.innerHTML = _("advance intra bss");	
	e = document.getElementById("IntraBSS_7");
	e.innerHTML = _("advance intra bss");	

	e = document.getElementById("Main_IntraBSS");
	e.innerHTML = _("main advance intra bss");		
	e = document.getElementById("GeneralWirelessSSID1");
	e.innerHTML = _("general wireless ssid1");
	e = document.getElementById("GeneralWirelessSSID2");
	e.innerHTML = _("general wireless ssid2");
	e = document.getElementById("GeneralWirelessSSID3");
	e.innerHTML = _("general wireless ssid3");	
	e = document.getElementById("GeneralWirelessSSID4");
	e.innerHTML = _("general wireless ssid4");	
	e = document.getElementById("GeneralWirelessSSID5");
	e.innerHTML = _("general wireless ssid5");	
	e = document.getElementById("GeneralWirelessSSID6");
	e.innerHTML = _("general wireless ssid6");	
	e = document.getElementById("GeneralWirelessSSID7");
	e.innerHTML = _("general wireless ssid7");	
	
	e = document.getElementById("enable_guest_wlan");
	e.innerHTML = _("wlan guest enable");	
	e = document.getElementById("wlanguestIP");
	e.innerHTML = _("wlan guest IP");
	e = document.getElementById("wlanguestMASK");
	e.innerHTML = _("wlan guest Subnet");
	e = document.getElementById("Guest_BWControl");
	e.innerHTML = _("wlan guest BWControl");
	//e = document.getElementById("guestpriority");
	//e.innerHTML = _("wlan guest priority");	
	//e = document.getElementById("guestHigh");
	//e.innerHTML = _("wlan guest High");	
	//e = document.getElementById("guestMiddle");
	//e.innerHTML = _("wlan guest Middle");	
	//e = document.getElementById("guestLow");
	//e.innerHTML = _("wlan guest Low");	
	e = document.getElementById("guetmaxBW");
	e.innerHTML = _("wlan guest maxBW");
	e = document.getElementById("guetBW_unit");
	e.innerHTML = _("wlan guest BW_unit");
	
	e = document.getElementById("GeneralWirelessEnable1");
	e.innerHTML = _("wireless enable");
	e = document.getElementById("GeneralWirelessEnable2");
	e.innerHTML = _("wireless enable");
	e = document.getElementById("GeneralWirelessEnable3");
	e.innerHTML = _("wireless enable");
	e = document.getElementById("GeneralWirelessEnable4");
	e.innerHTML = _("wireless enable");
	e = document.getElementById("GeneralWirelessEnable5");
	e.innerHTML = _("wireless enable");
	e = document.getElementById("GeneralWirelessEnable6");
	e.innerHTML = _("wireless enable");
	e = document.getElementById("GeneralWirelessEnable7");
	e.innerHTML = _("wireless enable");
}

function initAll()
{

	if ((wirelessmode == 2) || ((wirelessmode == 6) && (wispmode == 0))){
	    alert("You CANNOT select WISP or Client mode to configure this page");	
	}
	
	initTranslation();
	
//	var Enable_SSID  = '<% getCfgZero(1, "EnableSSID" ); %>';
	var Enable_SSID1 = '<% getCfgZero(1, "EnableSSID1"); %>';
	var Enable_SSID2 = '<% getCfgZero(1, "EnableSSID2"); %>';	
	var Enable_SSID3 = '<% getCfgZero(1, "EnableSSID3"); %>';	
	var Enable_SSID4 = '<% getCfgZero(1, "EnableSSID4"); %>';	
	var Enable_SSID5 = '<% getCfgZero(1, "EnableSSID5"); %>';	
	var Enable_SSID6 = '<% getCfgZero(1, "EnableSSID6"); %>';	
	var Enable_SSID7 = '<% getCfgZero(1, "EnableSSID7"); %>';	
	var Intra_BSS  = '<% getCfgZero(1, "NoForwarding"); %>';
	var Intra_BSS1 = '<% getCfgZero(1, "NoForwarding1"); %>';
	var Intra_BSS2 = '<% getCfgZero(1, "NoForwarding2"); %>';	
	var Intra_BSS3 = '<% getCfgZero(1, "NoForwarding3"); %>';
	var Intra_BSS4 = '<% getCfgZero(1, "NoForwarding4"); %>';
	var Intra_BSS5 = '<% getCfgZero(1, "NoForwarding5"); %>';
	var Intra_BSS6 = '<% getCfgZero(1, "NoForwarding6"); %>';
	var Intra_BSS7 = '<% getCfgZero(1, "NoForwarding7"); %>';
	var broadcastssidEnable   = '<% getCfgZero(1, "HideSSID"); %>';
	var broadcastssidEnable1  = '<% getCfgZero(1, "HideSSID1"); %>';
	var broadcastssidEnable2  = '<% getCfgZero(1, "HideSSID2"); %>';
	var broadcastssidEnable3  = '<% getCfgZero(1, "HideSSID3"); %>';
	var broadcastssidEnable4  = '<% getCfgZero(1, "HideSSID4"); %>';
	var broadcastssidEnable5  = '<% getCfgZero(1, "HideSSID5"); %>';
	var broadcastssidEnable6  = '<% getCfgZero(1, "HideSSID6"); %>';
	var broadcastssidEnable7  = '<% getCfgZero(1, "HideSSID7"); %>';
	var nv_channel = <% getCfgZero(1, "Channel"); %>;
	var MainIntra_BSS = "<% getCfgZero(1, "MainIntra_BSS"); %>";
	var nv_radio_off = "<% getRadioStatusASP(); %>";
	var PhyMode  = '<% getCfgZero(1, "WirelessMode"); %>';
	var ht_bw = '<% getCfgZero(1, "HT_BW"); %>';

	/* HT20 / HT20_40 */
	if (1*ht_bw == 0)
	{
		document.wireless_basic.n_bandwidth.options.selectedIndex = 0;
		show_div(false, "div_extension_channel");		
		document.wireless_basic.n_extcha.disabled = true;
	}
	else
	{
		document.wireless_basic.n_bandwidth.options.selectedIndex = 1;
		show_div(true, "div_extension_channel");		
		document.wireless_basic.n_extcha.disabled = false;
	}

	/* 11bgn / 11an */
	PhyMode = 1*PhyMode;
	if (PhyMode == 9)
		document.wireless_basic.wirelessmode.options.selectedIndex = 0;

	var t = document.getElementById("GeneralRadioStatus");		
	if (nv_radio_off == "ON")
		t.innerHTML = _("general on");
	else
		t.innerHTML = _("general off");
		
/*
	if (Enable_SSID == "1"){
		document.wireless_basic.enablessid.checked = true;
	}else{
		document.wireless_basic.enablessid.checked = false;
	}
*/

	if (Enable_SSID1 == "1"){
		document.wireless_basic.enablessid1.checked = true;
	}else{
		document.wireless_basic.enablessid1.checked = false;
	}
	if (Enable_SSID2 == "1"){
		document.wireless_basic.enablessid2.checked = true;
	}else{
		document.wireless_basic.enablessid2.checked = false;
	}
	if (Enable_SSID3 == "1"){
		document.wireless_basic.enablessid3.checked = true;
	}else{
		document.wireless_basic.enablessid3.checked = false;
	}
	if (Enable_SSID4 == "1"){
		document.wireless_basic.enablessid4.checked = true;
	}else{
		document.wireless_basic.enablessid4.checked = false;
	}
	if (Enable_SSID5 == "1"){
		document.wireless_basic.enablessid5.checked = true;
	}else{
		document.wireless_basic.enablessid5.checked = false;
	}
	if (Enable_SSID6 == "1"){
		document.wireless_basic.enablessid6.checked = true;
	}else{
		document.wireless_basic.enablessid6.checked = false;
	}
	if (Enable_SSID7 == "1"){
		document.wireless_basic.enablessid7.checked = true;
	}else{
		document.wireless_basic.enablessid7.checked = false;
	}
		
	if (broadcastssidEnable == "1"){
		document.wireless_basic.hidessid.checked = true;
	}else{
		document.wireless_basic.hidessid.checked = false;
	}
	if (broadcastssidEnable1 == "1"){
		document.wireless_basic.hidemssid_1.checked = true;
	}else{
		document.wireless_basic.hidemssid_1.checked = false;
	}
	if (broadcastssidEnable2 == "1"){
		document.wireless_basic.hidemssid_2.checked = true;
	}else{
		document.wireless_basic.hidemssid_2.checked = false;
	}
	if (broadcastssidEnable3 == "1"){
		document.wireless_basic.hidemssid_3.checked = true;
	}else{
		document.wireless_basic.hidemssid_3.checked = false;
	}
	if (broadcastssidEnable4 == "1"){
		document.wireless_basic.hidemssid_4.checked = true;
	}else{
		document.wireless_basic.hidemssid_4.checked = false;
	}
	if (broadcastssidEnable5 == "1"){
		document.wireless_basic.hidemssid_5.checked = true;
	}else{
		document.wireless_basic.hidemssid_5.checked = false;
	}
	if (broadcastssidEnable6 == "1"){
		document.wireless_basic.hidemssid_6.checked = true;
	}else{
		document.wireless_basic.hidemssid_6.checked = false;
	}
	if (broadcastssidEnable7 == "1"){
		document.wireless_basic.hidemssid_7.checked = true;
	}else{
		document.wireless_basic.hidemssid_7.checked = false;
	}
	
	if (Intra_BSS == "1"){
		document.wireless_basic.IntraBSS.checked = true;
	}else{
		document.wireless_basic.IntraBSS.checked = false;
	}
	
	if (Intra_BSS1 == "1"){
		document.wireless_basic.IntraBSS1.checked = true;
	}else{
		document.wireless_basic.IntraBSS1.checked = false;
	}
	if (Intra_BSS2 == "1"){
		document.wireless_basic.IntraBSS2.checked = true;
	}else{
		document.wireless_basic.IntraBSS2.checked = false;
	}
	if (Intra_BSS3 == "1"){
		document.wireless_basic.IntraBSS3.checked = true;
	}else{
		document.wireless_basic.IntraBSS3.checked = false;
	}
	if (Intra_BSS4 == "1"){
		document.wireless_basic.IntraBSS4.checked = true;
	}else{
		document.wireless_basic.IntraBSS4.checked = false;
	}
	if (Intra_BSS5 == "1"){
		document.wireless_basic.IntraBSS5.checked = true;
	}else{
		document.wireless_basic.IntraBSS5.checked = false;
	}
	if (Intra_BSS6 == "1"){
		document.wireless_basic.IntraBSS6.checked = true;
	}else{
		document.wireless_basic.IntraBSS6.checked = false;
	}
	if (Intra_BSS7 == "1"){
		document.wireless_basic.IntraBSS7.checked = true;
	}else{
		document.wireless_basic.IntraBSS7.checked = false;
	}

	if (MainIntra_BSS == "1"){
		document.wireless_basic.MainIntraBSS.checked = true;
	}else{
		document.wireless_basic.MainIntraBSS.checked = false;
	}


	if (nv_channel == 0){
		document.wireless_basic.Auto_Channel.checked = true;
		document.wireless_basic.sz11gChannel.disabled = true;
	}else{
		document.wireless_basic.Auto_Channel.checked = false;
	}	

	if (nv_channel != 0)
	    document.wireless_basic.sz11gChannel.options.selectedIndex = nv_channel-1;
/*	    	
	if (1*apcli_include == 1)
	{
		document.wireless_basic.mssid_7.disabled = true;
	}	    	
*/
		
// WLAN Guest
	if (wlan_guest_en == 1)
		document.wireless_basic.wlanguest.checked = true;
	else
		document.wireless_basic.wlanguest.checked = false;
		
	if (wlan_guest_bw_en == 0)
		document.wireless_basic.wlanguestBW.checked = false;
	else
		document.wireless_basic.wlanguestBW.checked = true;		
		
	//document.wireless_basic.guest_priority.selectedIndex = wlan_guest_bw_priority - 1;
	
	//route mode
	if (wirelessmode == 0){
		show_div(false, "div_wlanguest_id");
		if (wlan_guest_en == 1){
			show_div(false, "div_wlanguest_id");
			show_div(true, "div_wlanguestIP_id");
			show_div(true, "div_wlanguestMASK_id");
			show_div(false, "div_wlanguestBW_id");
			//show_div(true, "div_guestpriority_id");
			show_div(false, "div_guetmaxBW_id");
		}else{
			//show_div(false, "div_wlanguest_id");
			show_div(false, "div_wlanguestIP_id");
			show_div(false, "div_wlanguestMASK_id");
			show_div(false, "div_wlanguestBW_id");
			//show_div(false, "div_guestpriority_id");
			show_div(false, "div_guetmaxBW_id");
		}
	}
	else{
		show_div(false, "div_wlanguest_id");
		
		show_div(false, "div_wlanguestIP_id");
		show_div(false, "div_wlanguestMASK_id");
		show_div(false, "div_wlanguestBW_id");
		//show_div(false, "div_guestpriority_id");
		show_div(false, "div_guetmaxBW_id");
	}	
		
	show_div(true, "div_ssidchoice");	

	show_div(true, "div_note2_id");

	wirelessModeChange();
	updateChannel();
	SetChannelIdx(nv_channel)
	refreshExtChannel();
	
	makeRequest("/goform/wirelessGetSecurity", "n/a", securityHandler);
	parent.adjustMyFrameHeight();
}

function UpdateMBSSIDList()
{
	document.wireless_basic.ssidIndex.length = 0;

	for(var i=0; i<SSID.length; i++){
		var j = document.wireless_basic.ssidIndex.options.length;	
		document.wireless_basic.ssidIndex.options[j] = new Option(SSID[i], i, false, false);
	}
	
	document.wireless_basic.ssidIndex.options.selectedIndex = defaultShownMBSSID;
	old_MBSSID = defaultShownMBSSID;
}

function setChange(c){
	changed = c;
}

var WPAAlgorithms;
function onWPAAlgorithmsClick(type)
{
	if(type == 0 && WPAAlgorithms == "TKIP") return;
	if(type == 1 && WPAAlgorithms == "AES") return;
	if(type == 2 && WPAAlgorithms == "TKIPAES") return;
	setChange(1);
}

var IEEE8021XWEP;
function onIEEE8021XWEPClick(type)
{
	if(type == 0 && IEEE8021XWEP == false) return;
	if(type == 1 && IEEE8021XWEP == true) return;
	setChange(1);
}

var PreAuthentication;
function onPreAuthenticationClick(type)
{
	if(type == 0 && PreAuthentication == false) return;
	if(type == 1 && PreAuthentication == true) return;
	setChange(1);
}

function parse40WEPkey(str)
{
	var fields_str = new Array();
	fields_str = str.split("\r");

	document.wireless_basic.wep_key_1.value = fields_str[0];
	document.wireless_basic.wep_key_2.value = fields_str[1];
	document.wireless_basic.wep_key_3.value = fields_str[2];
	document.wireless_basic.wep_key_4.value = fields_str[3];
}

function parse128WEPkey(str)
{
	var fields_str = new Array();
	fields_str = str.split("\r");

	document.wireless_basic.wep_key_1.value = fields_str[0];
	document.wireless_basic.wep_key_2.value = fields_str[0];
	document.wireless_basic.wep_key_3.value = fields_str[0];
	document.wireless_basic.wep_key_4.value = fields_str[0];
}

function get40wepeyHandler(){
	if (http_request.readyState == 4) {
		if (http_request.status == 200) {
			parse40WEPkey(http_request.responseText);
		} else {
			alert('There was a problem with the request.');
		}
	}
}

function get128wepeyHandler(){
	if (http_request.readyState == 4) {
		if (http_request.status == 200) {
			parse128WEPkey(http_request.responseText);
		} else {
			alert('There was a problem with the request.');
	    }
	} 
}

function generate_wep()
{
	var passphrase;
	passphrase = document.wireless_basic.wep_passphrase.value;

	document.wireless_basic.WEPKey_Code[1].checked = true; //Hex	
	if (document.getElementById("wep_encry").selectedIndex == 1){ // get 128 bits WEP KEY
		makeRequest("/goform/wifiget128wepkey", passphrase, get128wepeyHandler);
	}else{ // get 40 bits WEP KEY
		makeRequest("/goform/wifiget40wepkey", passphrase, get40wepeyHandler);
	}
}

function clickwlanguest()
{
	if (document.wireless_basic.wlanguest.checked == true){
		show_div(true, "div_wlanguestIP_id");
		show_div(true, "div_wlanguestMASK_id");
		show_div(false, "div_wlanguestBW_id");
		//show_div(true, "div_guestpriority_id");
		show_div(false, "div_guetmaxBW_id");
	}else{
		show_div(false, "div_wlanguestIP_id");
		show_div(false, "div_wlanguestMASK_id");
		show_div(false, "div_wlanguestBW_id");
		//show_div(false, "div_guestpriority_id");
		show_div(false, "div_guetmaxBW_id");
	}
	parent.adjustMyFrameHeight();
}

</script>
</head>
<body onload="initAll()">
<form method="post" name="wireless_basic" action="/goform/wifiAPGeneral">
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul>

<li class="title" id="GeneralWirelessSetup">Wireless Setup</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="180" id="GeneralWirelessLAN">Wireless LAN :</td>
<td width="150" id=GeneralRadioStatus></td>
<td width="*">&nbsp;</td>
</tr>
</table>
</li>
             

<input type="hidden" name="bssid_num" value="1">

<input type="hidden" name="bssid_num1" value="">
<input type="hidden" name="bssid_num2" value="">
<input type="hidden" name="bssid_num3" value="">
<input type="hidden" name="bssid_num4" value="">
<input type="hidden" name="bssid_num5" value="">
<input type="hidden" name="bssid_num6" value="">
<input type="hidden" name="bssid_num7" value="">


<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="180" nowrap>
<!--<font id="xxx">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</font>-->
<font id="xxx"></font>
<!--
<font id="GeneralWirelessEnable">Enable</font>
<input type="checkbox" name=enablessid value="1">
-->
<font id="GeneralWirelessSSID">Network Name(SSID) :</font></td>
<td width="150">
<input type=text name=ssid size=20 maxlength=32 value="<% getCfgGeneral(1, "SSID"); %>" />
</td>        
<td width="*"><input type="checkbox" name=hidessid value="1" />
<font id ="GeneralHideSSID1"> Hide</font>
<!-- Intra-BSS/SSID checkbox -->
<input name="IntraBSS" type="checkbox" value="0" />
<font id ="IntraBSS_0"> Enable Intra-BSS Traffic</font> 
</td>
</tr>
</table>
</li>                      


<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="60" nowrap >
<input type="checkbox" name=enablessid1 value="1" />
<font id="GeneralWirelessEnable1">Enable </font>
</td>
<td width="120">
<font id="GeneralWirelessSSID1">Name(SSID1) :</font></td>
<td width="150" nowrap >
<input type=text name=mssid_1 size=20 maxlength=32 value="<% getCfgGeneral(1, "SSID1"); %>" />
</td>
<td><input type="checkbox" name=hidemssid_1 value="1" />
<font id ="GeneralmSSIDHide1">Hide</font>
<!-- Intra-BSS/SSID checkbox -->
<input name="IntraBSS1" type="checkbox" value="0" />
<font id ="IntraBSS_1"> Enable Intra-BSS Traffic</font> 
</td>
</tr>
</table>
</li>
                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="60" nowrap>
<input type="checkbox" name=enablessid2 value="1" >
<font id="GeneralWirelessEnable2">Enable</font>
</td>
<td width="120">
<font id="GeneralWirelessSSID2"> Name(SSID2) :</font></td>
<td width="150" nowrap >
<input type=text name=mssid_2 size=20 maxlength=32 value="<% getCfgGeneral(1, "SSID2"); %>" />
</td>
<td><input type="checkbox" name=hidemssid_2 value="1" />
<font id ="GeneralmSSIDHide2">Hide</font>
<!-- Intra-BSS/SSID checkbox -->
<input name="IntraBSS2" type="checkbox" value="0" />
<font id ="IntraBSS_2"> Enable Intra-BSS Traffic</font> 
</td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="60" nowrap >
<input type="checkbox" name=enablessid3 value="1" />
<font id="GeneralWirelessEnable3">Enable</font>
</td>
<td width="120">
<font id="GeneralWirelessSSID3">Name(SSID3) :</font></td>
<td width="150" nowrap >
<input type=text name=mssid_3 size=20 maxlength=32 value="<% getCfgGeneral(1, "SSID3"); %>" />
</td>
<td><input type="checkbox" name=hidemssid_3 value="1" />
<font id ="GeneralmSSIDHide3">Hide</font>
<!-- Intra-BSS/SSID checkbox -->
<input name="IntraBSS3" type="checkbox" value="0" />
<font id ="IntraBSS_3"> Enable Intra-BSS Traffic</font> 
</td>
</tr>
</table>
</li>


<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="60" nowrap >
<input type="checkbox" name=enablessid4 value="1" />
<font id="GeneralWirelessEnable4">Enable</font>
</td>
<td width="120">
<font id="GeneralWirelessSSID4">Name(SSID4) :</font></td>
<td width="150" nowrap >
<input type=text name=mssid_4 size=20 maxlength=32 value="<% getCfgGeneral(1, "SSID4"); %>" />
</td>
<td><input type="checkbox" name=hidemssid_4 value="1" />
<font id ="GeneralmSSIDHide4">Hide</font>
<!-- Intra-BSS/SSID checkbox -->
<input name="IntraBSS4" type="checkbox" value="0" />
<font id ="IntraBSS_4"> Enable Intra-BSS Traffic</font> 
</td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="60" nowrap >
<input type="checkbox" name=enablessid5 value="1" />
<font id="GeneralWirelessEnable5">Enable</font>
</td>
<td width="120">
<font id="GeneralWirelessSSID5">Name(SSID5) :</font></td>
<td width="150" nowrap >
<input type=text name=mssid_5 size=20 maxlength=32 value="<% getCfgGeneral(1, "SSID5"); %>" />
</td>
<td><input type="checkbox" name=hidemssid_5 value="1" />
<font id ="GeneralmSSIDHide5">Hide</font>
<!-- Intra-BSS/SSID checkbox -->
<input name="IntraBSS5" type="checkbox" value="0" />
<font id ="IntraBSS_5"> Enable Intra-BSS Traffic</font> 
</td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="60" nowrap >
<input type="checkbox" name=enablessid6 value="1" />
<font id="GeneralWirelessEnable6">Enable</font>
</td>
<td width="120">
<font id="GeneralWirelessSSID6">Name(SSID6) :</font></td>
<td width="150" nowrap >
<input type=text name=mssid_6 size=20 maxlength=32 value="<% getCfgGeneral(1, "SSID6"); %>" />
</td>
<td><input type="checkbox" name=hidemssid_6 value="1" />
<font id ="GeneralmSSIDHide6">Hide</font>
<!-- Intra-BSS/SSID checkbox -->
<input name="IntraBSS6" type="checkbox" value="0" />
<font id ="IntraBSS_6"> Enable Intra-BSS Traffic</font> 
</td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="60" nowrap >
<input type="checkbox" name=enablessid7 value="1" />
<font id="GeneralWirelessEnable7">Enable</font>
</td>
<td width="120">
<font id="GeneralWirelessSSID7">Name(SSID7) :</font></td>
<td width="150" nowrap >
<input type=text name=mssid_7 size=20 maxlength=32 value="<% getCfgGeneral(1, "SSID7"); %>" />
</td>
<td><input type="checkbox" name=hidemssid_7 value="1" />
<font id ="GeneralmSSIDHide7">Hide</font>
<!-- Intra-BSS/SSID checkbox -->
<input name="IntraBSS7" type="checkbox" value="0" />
<font id ="IntraBSS_7"> Enable Intra-BSS Traffic</font> 
</td>
</tr>
</table>
</li>

<li></li>
<!-- add WLAN Guest -->
<span id="div_wlanguest_id" class="off">
<input type="checkbox" name=wlanguest value="1" onclick="clickwlanguest()"/>
<font id ="enable_guest_wlan">Enable Guest WLAN</font>
</span>

<!-- add WLAN Guest(Begin) -->
<span id="div_wlanguestIP_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="180" nowrap>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font id="wlanguestIP">IP Address : </font></td>
<td>
<input type=text name=wlanguestip size=20 maxlength=20 value="<% getCfgGeneral(1, "WlanGuestIP"); %>" />
</td>
<td></td>
</tr>
</table>
</li>
</span>

<span id="div_wlanguestMASK_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="180" nowrap>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font id="wlanguestMASK"> IP Subnet Mask : </font></td>
<td>
<input type=text name=wlanguestmask size=20 maxlength=20 value="<% getCfgGeneral(1, "WlanGuestMASK"); %>" />
</td>
</tr>
</table>
</li>
</span>
                
<span id="div_wlanguestBW_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input name="wlanguestBW" type="checkbox" value="1" />
<font id ="Guest_BWControl">Enable Bandwidth Management for Guest WLAN</font> 
</td>
</tr>
</table>
</li>                
</span>

<!--<span id="div_guestpriority_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="180" nowrap>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font id="guestpriority"> Priority : </font></td>
<td>
<select name="guest_priority" size="1">
<option value = 1 id="guestHigh">High</option>
<option value = 2 id="guestMiddle">Middle</option>
<option value = 3 id="guestLow">Low</option>
</select>
</td>
</tr>
</table>
</li>
</span>-->

<span id="div_guetmaxBW_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="180" nowrap>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font id="guetmaxBW">Maximum Bandwidth : </font></td>
<td>
<input type=text name=guestmaxbw size=6 maxlength=5 value="<% getCfgGeneral(1, "WlanGuestBWMax"); %>" />
<font id="guetBW_unit">(kbps)</font>
</td>
</tr>
</table>
</li>                
</span>
             
<span id="div_networkmode_id"  class='off'>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr> 
<td id="AdvanceNetMode" width="180" nowrap>Network Mode :</td>
<td width="150">
<select name="wirelessmode" id="wirelessmode" size="1" onChange="wirelessModeChange()" >
<option value=9>11b/g/n mixed mode</option>       
</select>
</td>
<td width="*">&nbsp;</td>
</tr>
</table>
</li>
</span>

<span id="div_ht_phy_1_3" class="off">  
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0"> 
<tr>
<td id="AdvanceHTChannelBW" width="180" nowrap>Channel BandWidth :</td>
<td width="150">
<select name="n_bandwidth" id="n_bandwidth" size="1" onChange="Channel_BandWidth_onChange();updateChannel();refreshExtChannel()">
<option value=0>20 &nbsp; </option>
<option value=1>20/40 </option>
</td>
<td width="*">&nbsp;</td>
</tr>
</table>
</li>         
</span>  
                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td id ="GeneralChannelSelect" width="180" nowrap>Channel Selection :</td>
<td width="150">
	<select id="sz11gChannel" name="sz11gChannel" size="1" onChange="refreshExtChannel()" >
	</select>                        
</td>
<td width="*">
	<input type="checkbox" name="Auto_Channel" value=1 onclick="clickAutoChannel()" />
	<font id ="GeneralAutoChannelSelect">Auto Channel Selection</font> 
</td>
</tr>
</table>
</li>

<span id="div_extension_channel" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0"> 
<tr>
<td id="AdvanceHTExtChannel" width="180" nowrap>Extension Channel :</td>
<td width="150">
	<select id="n_extcha" name="n_extcha" size="1">
	</select>
</td>
<td width="*">&nbsp;</td>
</tr>
</table>
</li> 
</span>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td id ="GeneralOperatingChannel" width="180" nowrap>Operating Channel :</td>
<td width="150"> <% getOpChannleASP(); %>  </td>                      
<td width="*">&nbsp;</td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%"><input name="MainIntraBSS" type="checkbox" value=1 />
<font id ="Main_IntraBSS">Communication between wireless clients with different SSIDs</font> 
</td>
<td>&nbsp;</td>
</tr>
</table>
</li>

<span id="div_hide_security_id" class="off">
<li class="title" id="GeneralSecurityTitle">Security</li>
<span id="div_ssidchoice" class="off">                                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralSSIDChoice">SSID choice</td>
<td><select name="ssidIndex" size="1" onchange="selectMBSSIDChanged()">
<!-- ....Javascript will update options.... -->
</select>
</td>
</tr>
</table>
</li>
</span>

<li class="title" id="GeneralSecurityTitle">Security</li>                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralSecureMode">Security Mode</td>
<td>
<select name="security_mode" id="security_mode" size="1" onchange="securityMode(1)">
<!-- ....Javascript will update options.... -->
</select>
</td>
</tr>
</table>
</li>

<!-- WEP -->
<span id="div_wep" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralWEPPassPhrase">PassPhrase </td>
<td><input name="wep_passphrase" id="wep_passphrase" maxlength="26" value="" onKeyUp="setChange(1)">
<input type=button value="Generate" id="GeneralWEPGenerate" onclick="generate_wep()" ></td>		      
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralWEPEncryp">WEP Encryption</td>
<td>
<select name="wep_encry" id="wep_encry" size="1" onchange="securityMode(1)">
<option id="GeneralWEPEncryp64" value="0">64-bits </option>
<option id="GeneralWEPEncryp128" value="1">128-bits</option>
</select>
</td>
</tr>
</table>
</li>                

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralAuthMethod">Authentication Method</td>
<td>
<select name="auth_method" id="auth_method" size="1" onchange="securityMode(1)">
<option id="GeneralEncrypAuto"      value="0">Auto      </option>
<option id="GeneralEncrypSharedKey" value="1">Shared Key</option>
</select>
</td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><span class="i_note" id="GeneralNote1_1">Note:</span></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><span class="i_note_a" id="GeneralNote1_2">64-bit WEP: Enter 5 ASCII characters or 10 hexadecimal characters ("0-9", "A-F") for each Key (1-4).</span></td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><span class="i_note_a" id="GeneralNote1_3">128-bit WEP: Enter 13 ASCII characters or 26 hexadecimal characters ("0-9", "A-F") for each Key (1-4).</span></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><span class="i_note_a" id="GeneralNote1_4">(Select one WEP key as an active key to encrypt wireless data transmission.)</span></td>
</tr>
</table>
</li>                


<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="10%"></td>                    
<td width="15%"></td>
<td>
<input type="radio" name="WEPKey_Code" value="1" /><font id="GeneralWEPASCII">ASCII </font>
<input type="radio" name="WEPKey_Code" value="0" /><font id="GeneralWEPHex">Hex</font></td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">                  
<tr>
<td width="10%"></td>                    
<td width="15%" id="secureWEPKey1"><input type="radio" name="DefWEPKey" value="1"/><font id="GeneralWEPKEY1"> Key 1</font></td>
<td><input name="wep_key_1" id="WEP1" size="28" maxlength="26" value="" onKeyUp="setChange(1)"></td>
</tr>
</table>
</li>
                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="10%"></td>                    
<td width="15%" id="secureWEPKey2"><input type="radio" name="DefWEPKey" value="2" /><font id="GeneralWEPKEY2"> Key 2</font></td>
<td><input name="wep_key_2" id="WEP2" size="28" maxlength="26" value="" onKeyUp="setChange(1)"></td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="10%"></td>                    
<td width="15%" id="secureWEPKey3"><input type="radio" name="DefWEPKey" value="3" /><font id="GeneralWEPKEY3"> Key 3</font></td>
<td><input name="wep_key_3" id="WEP3" size="28" maxlength="26" value="" onKeyUp="setChange(1)"></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="10%"></td>                    
<td width="15%" id="secureWEPKey4"><input type="radio" name="DefWEPKey" value="4" /><font id="GeneralWEPKEY4"> Key 4</font></td>
<td><input name="wep_key_4" id="WEP4" size="28" maxlength="26" value="" onKeyUp="setChange(1)"></td>
</tr>
</table>
</li>
</span>

<input id==wep_key128 name=wep_key128 size=26 type=hidden>
              

<span id="div_wpapsk_compatible" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><input name="wpapsk_compatible" type="checkbox" value=1 />
<font id="GeneralWPAPSKCompatible">WPA Compatible</font>
</td>
</tr>
</table>
</li>
</span>                

<span id="div_wpa_compatible" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><input name="wpa_compatible" type="checkbox" value=1 />
<font id="GeneralWPACompatible">WPA Compatible</font>
</td>
</tr>
</table>
</li>
</span>                
                
<span id="wpa_passphrase" class="off">                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralWPAPSKPreSharedKey">Pre-Shared Key</td>
<td>
<input name="passphrase" id="passphrase" size="28" maxlength="64" value="" onKeyUp="setChange(1)">
</td>
</tr>
</table>
</li>
</span>
 
<span id="wpa_key_renewal_interval" class="off">                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralWPAKeyRenewInterval">Group Key Update Timer</td>
<td>
<input name="keyRenewalInterval" id="keyRenewalInterval" size="4" maxlength="4" value="3600" onKeyUp="setChange(1)"><font id="GeneralWPAkeyRenewalIntervalUnit"> seconds</font> 
</td>
</tr>
</table>
</li>
</span>


<span id="wpa_PMK_Cache_Period" class="off">                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralWPAPMKCachePeriod">PMK Cache Period</td>
<td>
<input name="PMKCachePeriod" id="PMKCachePeriod" size="4" maxlength="4" value="" onKeyUp="setChange(1)"><font id="GeneralWPAPMKCachePeriodUnit"> minute</font> 
</td>
</tr>
</table>
</li>
</span>

<span id="wpa_preAuthentication" class="off">                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralWPAPreAuth">Pre-Authentication</td>
<td>
<input name="PreAuthentication" id="PreAuthentication" value="0" type="radio" onClick="onPreAuthenticationClick(0)"><font id="GeneralWPAPreAuthDisable">Disable </font>
<input name="PreAuthentication" id="PreAuthentication" value="1" type="radio" onClick="onPreAuthenticationClick(1)"><font id="GeneralWPAPreAuthEnable">Enable </font>
</td>
</tr>
</table>
</li>
</span>                              


<!-- IEEE 802.1x WEP  -->
<span id="div_8021x_wep" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralIEEE8021xSettings">IEEE 802.1x Settings</td>
</tr>
</table>
</li>                
                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="General1XWEP">WEP</td>
<td>
<input name="ieee8021x_wep" id="ieee8021x_wep" value="0" type="radio" onClick="onIEEE8021XWEPClick(0)"><font id="General1XWEPDisable">Disable</font>
<input name="ieee8021x_wep" id="ieee8021x_wep" value="1" type="radio" onClick="onIEEE8021XWEPClick(1)"><font id="General1XWEPEnable">Enable</font>
</td>
</tr>
</table>
</li>
</span>                

<!-- Radius Server  -->              
<span id="div_radius_server" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralAuthServer">Authentication Server</td>
</tr>
</table>
</li>
                                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="2%">&nbsp;</td>
<td width="38%" id="GeneralRadiusIPAddr">IP Address </td>
<td>
<input name="RadiusServerIP" id="RadiusServerIP" size="16" maxlength="32" value="" onKeyUp="setChange(1)">
</td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="2%">&nbsp;</td>
<td width="38%" id="GeneralRadiusPort">Port Number</td>
<td>
<input name="RadiusServerPort" id="RadiusServerPort" size="5" maxlength="5" value="" onKeyUp="setChange(1)">
</td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="2%">&nbsp;</td>
<td width="38%" id="GeneralRadiusSharedSecret">Shared Secret </td>
<td>
<input name="RadiusServerSecret" id="RadiusServerSecret" size="16" maxlength="64" value="" onKeyUp="setChange(1)">
</td>
</tr>
</table>
</li>
                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="2%">&nbsp;</td>
<td width="38%" id="GeneralRadiusSessionTimeout">Session Timeout </td>
<td>
<input name="RadiusServerSessionTimeout" id="RadiusServerSessionTimeout" size="3" maxlength="4" value="0" onKeyUp="setChange(1)">
</td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="2%">&nbsp;</td>
<td width="38%" id="GeneralRadiusIdleTimeout">Idle Timeout </td>
<td>
<input name="RadiusServerIdleTimeout" id="RadiusServerIdleTimeout" size="3" maxlength="4" value="" onKeyUp="setChange(1)" readonly>
</td>
</tr>
</table>
</li>
</span>

<span id="div_note2_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><span id="GeneralNote2" class="i_note">Note: WPA and WPA2 can be configured when WPS disabled</span></td>
</tr>
</table>
</li>                
</span>
</span> <!-- span id="div_hide_security_id" class="off> -->

<li></li>
<li></li>
</ul>
</div>
</li>
<center>
<li class="table_button">
<input type=button value=Apply id="GeneralApply" onclick="submit_apply()" >&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input type=reset value=Reset id="GeneralReset" onClick="window.location.reload()">
<input type=hidden name="lanIP" value="<% getCfgGeneral(1, "lan_ipaddr"); %>">
</li>
</center>

</form>
</body>
<script>
function alert(str)
{
	showWebMessage(1, str);
}
showWebMessage(<% getWebMessageFlag(); %>, _("<% getWebMessage(); %>"));
</script> 
</html>
 
