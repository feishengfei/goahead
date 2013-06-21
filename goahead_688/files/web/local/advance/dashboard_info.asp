<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>
<link href="images/expert.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<style>
td {white-space: nowrap;}
</style>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script type="text/javascript" src="/DST.js"></script>
<script>
Butterlate.setTextDomain("main");
</script>
<script type="text/JavaScript">
<!--
function ssid(ssid)
{
	var i = 0;
	var s;
	var s1;
	var tmp_ssid = new Array();
	for(i = 0; i < ssid.length; i++ )
	{
		s = ssid.charAt(i);
		s1 = ssid.charCodeAt(i);
		if(s1 == 32)
			tmp_ssid[i] = "&nbsp;";
		else
			tmp_ssid[i] = s;
	}
				
	 document.write(tmp_ssid.join(""));
}

function MM_swapImgRestore() { //v3.0
  var i,x,a=document.MM_sr; for(i=0;a&&i<a.length&&(x=a[i])&&x.oSrc;i++) x.src=x.oSrc;
}

function MM_preloadImages() { //v3.0
  var d=document; if(d.images){ if(!d.MM_p) d.MM_p=new Array();
    var i,j=d.MM_p.length,a=MM_preloadImages.arguments; for(i=0; i<a.length; i++)
    if (a[i].indexOf("#")!=0){ d.MM_p[j]=new Image; d.MM_p[j++].src=a[i];}}
}

function MM_findObj(n, d) { //v4.01
  var p,i,x;  if(!d) d=document; if((p=n.indexOf("?"))>0&&parent.frames.length) {
    d=parent.frames[n.substring(p+1)].document; n=n.substring(0,p);}
  if(!(x=d[n])&&d.all) x=d.all[n]; for (i=0;!x&&i<d.forms.length;i++) x=d.forms[i][n];
  for(i=0;!x&&d.layers&&i<d.layers.length;i++) x=MM_findObj(n,d.layers[i].document);
  if(!x && d.getElementById) x=d.getElementById(n); return x;
}

function MM_swapImage() { //v3.0
  var i,j=0,x,a=MM_swapImage.arguments; document.MM_sr=new Array; for(i=0;i<(a.length-2);i+=3)
   if ((x=MM_findObj(a[i]))!=null){document.MM_sr[j++]=x; if(!x.oSrc) x.oSrc=x.src; x.src=a[i+2];}
}

function MM_showHideLayers() { //v6.0
  var i,p,v,obj,args=MM_showHideLayers.arguments;
  for (i=0; i<(args.length-2); i+=3) if ((obj=MM_findObj(args[i]))!=null) { v=args[i+2];
    if (obj.style) { obj=obj.style; v=(v=='show')?'visible':(v=='hide')?'hidden':v; }
    obj.visibility=v; }
}
//-->

var nv_OP_Mode = <% getCfgZero(1, "OP_Mode"); %>;
var nv_WDS_Mode = <% getCfgZero(1, "WdsEnable"); %>;
var nv_WDS1_Mode = <% getCfgZero(1, "Wds5GEnable"); %>;
var wan3G = <% get3GSetting(); %>;
var wan_ipv6_proto = '<% getCfgZero(1, "wan_ipv6_proto"); %>';
var board_type = "<% getCfgZero(1, "board_model"); %>";
var bridge_mode = "<% getCfgZero(1, "brg_mode"); %>";

function initTranslation()
{
	var e;

	e = document.getElementById("dash_infoTitle");
	e.innerHTML = _("dash_info title");
	e = document.getElementById("dash_infoItem");
	e.innerHTML = _("dash_info item");
	e = document.getElementById("dash_infoData");
	e.innerHTML = _("dash_info data");
	e = document.getElementById("dash_infoHostName");
	e.innerHTML = _("dash_info hostname");
	e = document.getElementById("dash_infoFwVer");
	e.innerHTML = _("dash_info fwver");
	e = document.getElementById("dash_infoSysOpmode");
	e.innerHTML = _("dash_info sys opmode");

	if ((nv_OP_Mode == 0)||(nv_OP_Mode == 6)||(nv_OP_Mode == 7)){
		e = document.getElementById("dash_infoWanInfo");
		e.innerHTML = _("dash_info wan info");
 		if((wan3G)&&(nv_OP_Mode == 0)){
 			e = document.getElementById("dash_infoSIMstatus");
			e.innerHTML = _("dash_info wan3G simstatus");
			e = document.getElementById("dash_infoWan3GIp");
			e.innerHTML = _("dash_info wan ip");
			e = document.getElementById("dash_infoWan3GSubmask");
			e.innerHTML = _("dash_info wan mask");
			e = document.getElementById("dash_infoWan3GDHCP");
			e.innerHTML = _("dash_info lan dhcp");
 		}else{
			e = document.getElementById("dash_infoWanMac");
			e.innerHTML = _("dash_info wan mac");
			e = document.getElementById("dash_infoWanIp");
			e.innerHTML = _("dash_info wan ip");
			e = document.getElementById("dash_infoWanSubmask");
			e.innerHTML = _("dash_info wan mask");
			e = document.getElementById("dash_infoWanContype");
    		e.innerHTML = _("dash_info lan dhcp");
		}
	}

	e = document.getElementById("dash_infoLanInfo");
	e.innerHTML = _("dash_info lan info");
	e = document.getElementById("dash_infoLanMac");
	e.innerHTML = _("dash_info lan mac");
	e = document.getElementById("dash_infoLanIp");
	e.innerHTML = _("dash_info lan ip");
	e = document.getElementById("dash_infoLanSubmask");
	e.innerHTML = _("dash_info lan mask");
	e = document.getElementById("dash_infoLanDhcp");
	e.innerHTML = _("dash_info lan dhcp");
	if (board_type == "0A22")
	{
		e = document.getElementById("dash_infoWlanInfo");
		e.innerHTML = _("dash_info wlan-1 info");
		e = document.getElementById("dash_infoWlanOpmode");
		e.innerHTML = _("dash_info wlan-1 opmode");
	}
	else
	{
		e = document.getElementById("dash_infoWlanInfo");
		e.innerHTML = _("dash_info wlan info");
		e = document.getElementById("dash_infoWlanOpmode");
		e.innerHTML = _("dash_info wlan opmode");
	}
	e = document.getElementById("dash_infoWlanMac");
	e.innerHTML = _("dash_info wlan mac");
	e = document.getElementById("dash_infoWlanStatus");
	e.innerHTML = _("dash_info wlan status");
	if (board_type == "0A52")
	{
		e = document.getElementById("dash1_infoWlanInfo");
		e.innerHTML = _("dash_info wlan1 info");
		e = document.getElementById("dash1_infoWlanOpmode");
		e.innerHTML = _("dash_info wlan1 opmode");
		e = document.getElementById("dash1_infoWlanMac");
		e.innerHTML = _("dash_info wlan mac");
		e = document.getElementById("dash1_infoWlanStatus");
		e.innerHTML = _("dash_info wlan status");
	}
	else if (board_type == "0A22")
	{
		e = document.getElementById("dash1_infoWlanInfo");
		e.innerHTML = _("dash_info wlan-2 info");
		e = document.getElementById("dash1_infoWlanOpmode");
		e.innerHTML = _("dash_info wlan-2 opmode");
		e = document.getElementById("dash1_infoWlanMac");
		e.innerHTML = _("dash_info wlan mac");
		e = document.getElementById("dash1_infoWlanStatus");
		e.innerHTML = _("dash_info wlan status");
	}
	
	if (nv_OP_Mode != 6){
		if (nv_OP_Mode != 5) {
			e = document.getElementById("dash_infoWlanChan");
			e.innerHTML = _("dash_info wlan chan");
		}
		e = document.getElementById("dash_infoWlanOpchan");
		e.innerHTML = _("dash_info wlan opchan");
		if ((board_type == "0A22") || (board_type == "0A52"))
		{
			e = document.getElementById("dash1_infoWlanChan");
			e.innerHTML = _("dash_info wlan chan");
			e = document.getElementById("dash1_infoWlanOpchan");
			e.innerHTML = _("dash_info wlan opchan");
		}
	}
	// add nv_OP_Mode == 6 for wisp mode when 2.4g or 5g is ap mode
	if(nv_OP_Mode == 6){
	      if("<% getCfgZero(1, "wisp2.4g_mode"); %>" == "ap"){
       		e = document.getElementById("dash_infoWlanChan");
       		e.innerHTML = _("dash_info wlan chan");
       		e = document.getElementById("dash_infoWlanOpchan");
       		e.innerHTML = _("dash_info wlan opchan");
       		e = document.getElementById("dash_infoWlan80211mode");
       		e.innerHTML = _("dash_info wlan 80211mode");
	      }
	      if ((board_type == "0A22") || (board_type == "0A52"))
	      {
	      	if("<% getCfgZero(1, "wisp5g_mode"); %>" == "ap"){
        		e = document.getElementById("dash1_infoWlanChan");
        		e.innerHTML = _("dash_info wlan chan");
        		e = document.getElementById("dash1_infoWlanOpchan");
        		e.innerHTML = _("dash_info wlan opchan");
        		e = document.getElementById("dash1_infoWlan80211mode");
        		e.innerHTML = _("dash_info wlan 80211mode");
			}
		}
	}
	e = document.getElementById("dash_infoWlanSec");
	e.innerHTML = _("dash_info wlan sec");
	if ((board_type == "0A22") || (board_type == "0A52"))
	{
		e = document.getElementById("dash1_infoWlanSec");
		e.innerHTML = _("dash_info wlan sec");
	}

	if (nv_OP_Mode != 6){
		e = document.getElementById("dash_infoWlan80211mode");
		e.innerHTML = _("dash_info wlan 80211mode");
		if ((board_type == "0A22") || (board_type == "0A52"))
		{
			e = document.getElementById("dash1_infoWlan80211mode");
			e.innerHTML = _("dash_info wlan 80211mode");
		}
	}

	e = document.getElementById("dash_infoIfstatTitle");
	e.innerHTML = _("dash_info ifstat title");
	e = document.getElementById("dash_infoIfstatif");
	e.innerHTML = _("dash_info ifstat if");
	e = document.getElementById("dash_infoIfstatStat");
	e.innerHTML = _("dash_info ifstat stat");
	e = document.getElementById("dash_infoIfstatRate");
	e.innerHTML = _("dash_info ifstat rate");
	
	if (nv_OP_Mode == 0){
 		if(wan3G){
			e = document.getElementById("dash_infoIfstatWan3G");
			e.innerHTML = _("dash_info ifstat wan3G");
 		}else{
			e = document.getElementById("dash_infoIfstatWan");
			e.innerHTML = _("dash_info ifstat wan");
		}
	}

	if (board_type == "0A22")
	{
		e = document.getElementById("dash_infoIfstatWlan");
		e.innerHTML = _("dash_info ifstat-1 wlan");
	}
	else
	{
		e = document.getElementById("dash_infoIfstatWlan");
		e.innerHTML = _("dash_info ifstat wlan");
	}
	if (board_type == "0A52")
	{
		e = document.getElementById("dash1_infoIfstatWlan");
		e.innerHTML = _("dash_info ifstat1 wlan");
	}
	else if (board_type == "0A22")
	{
		e = document.getElementById("dash1_infoIfstatWlan");
		e.innerHTML = _("dash_info ifstat-2 wlan");
	}
	e = document.getElementById("dash_infoSystatTitle");
	e.innerHTML = _("dash_info systat title");
	e = document.getElementById("dash_infoSystatItem");
	e.innerHTML = _("dash_info systat item");
	e = document.getElementById("dash_infoSystatData");
	e.innerHTML = _("dash_info systat data");
	e = document.getElementById("dash_infoSystatSysuptime");
	e.innerHTML = _("dash_info systat sysuptime");
	e = document.getElementById("dash_infoSystatCurrtime");
	e.innerHTML = _("dash_info systat currtime");
	e = document.getElementById("dash_infoSystatSysres");
	e.innerHTML = _("dash_info systat sysres");
	e = document.getElementById("dash_infoSystatCpu");
	e.innerHTML = _("dash_info systat cpu");
	e = document.getElementById("dash_infoSystatMem");
	e.innerHTML = _("dash_info systat mem");
	//e = document.getElementById("dash_infoSystatSetting");
	//e.innerHTML = _("dash_info systat setting");
	
	if ((nv_OP_Mode == 0)||(nv_OP_Mode == 6) ||(nv_OP_Mode == 7)){
		e = document.getElementById("dash_infoSystatSettingFw");
		e.innerHTML = _("dash_info systat setting fw");
		//e = document.getElementById("dash_infoSystatBm");
		//e.innerHTML = _("dash_info systat bm");
	}

	//e = document.getElementById("dash_infoSystatConfmode");
	//e.innerHTML = _("dash_info systat confmode");
	//e = document.getElementById("dash_infoSystatConfmodeExp");
	//e.innerHTML = _("dash_info systat confmode exp");

	if ((nv_OP_Mode == 0)){
		if(wan3G){
			e = document.getElementById("dash_infoSum3GTitle");
			e.innerHTML = _("dash_info sum3G title");
			e = document.getElementById("dash_info3GItem");
			e.innerHTML = _("dash_info systat item");
			e = document.getElementById("dash_info3GData");
			e.innerHTML = _("dash_info systat data");
			//e = document.getElementById("dash_info3GConnStatus");
			//e.innerHTML = _("dash_info conn3G status");
			e = document.getElementById("dash_info3GISP");
			e.innerHTML = _("dash_info wan3G ISP");
			e = document.getElementById("dash_info3GSignalStrength");
			e.innerHTML = _("dash_info wan3G SSI");
			e = document.getElementById("dash_info3GSSIunit");
			e.innerHTML = _("dash_info wan3G SSIunit");		
			e = document.getElementById("dash_info3GConnUpTime");
			e.innerHTML = _("dash_info wan3G uptime");
			e = document.getElementById("dash_info3GManufacturer");
			e.innerHTML = _("dash_info wan3G Manufacturer");
			e = document.getElementById("dash_info3GModel");
			e.innerHTML = _("dash_info wan3G Model");		
			e = document.getElementById("dash_info3GFW");
			e.innerHTML = _("dash_info wan3G firmware");
			e = document.getElementById("dash_info3GIMEI");
			e.innerHTML = _("dash_info wan3G IMEI");
			//e = document.getElementById("dash_infoSIMIMEI");
			//e.innerHTML = _("dash_info wan3G SIM_IMEI");
		}
	}
	
	//Steve add for IPv6 Status
	if ((nv_OP_Mode == 0)||(nv_OP_Mode == 6)||(nv_OP_Mode == 7)){
	
		e = document.getElementById("dash_infoIPv6Title");
		e.innerHTML = _("dash_info ipv6 title");
		e = document.getElementById("dash_infoIPv6Item");
		e.innerHTML = _("dash_info systat item");
		e = document.getElementById("dash_infoIPv6Data");
		e.innerHTML = _("dash_info systat data");
		e = document.getElementById("dash_infoIPv6ConnType");
		e.innerHTML = _("dash_info ipv6 conn type");
		if((wan_ipv6_proto == '0') || (wan_ipv6_proto == '1')){ 
			e = document.getElementById("dash_infoIPv6WanAddress");
			e.innerHTML = _("dash_info ipv6 wan address");	
			e = document.getElementById("dash_infoIPv6WanGateway");
			e.innerHTML = _("dash_info ipv6 wan gateway");
			e = document.getElementById("dash_infoIPv6WanFirstDns");
			e.innerHTML = _("dash_info ipv6 wan first dns");
			e = document.getElementById("dash_infoIPv6WanSecondDns");
			e.innerHTML = _("dash_info ipv6 wan second dns");
			e = document.getElementById("dash_infoIPv6LanAddress");
			e.innerHTML = _("dash_info ipv6 lan address");
		}
		e = document.getElementById("dash_infoIPv6LanLinkLocal");
		e.innerHTML = _("dash_info ipv6 lan link local");	
	}

	e = document.getElementById("dash_infoSumTitle");
	e.innerHTML = _("dash_info sum title");
	
	if ((nv_OP_Mode == 0)||(nv_OP_Mode == 6)||(nv_OP_Mode == 7)){
		//e = document.getElementById("dash_infoSumBw");
		//e.innerHTML = _("dash_info sum bw");
		//e = document.getElementById("dash_infoSumBwDet");
		//e.innerHTML = _("dash_info sum bw det");
		e = document.getElementById("dash_infoSumDhcp");
		e.innerHTML = _("dash_info sum dhcp");
		e = document.getElementById("dash_infoSumDhcpDet");
		e.innerHTML = _("dash_info sum dhcp det");
	}
	
	e = document.getElementById("dash_infoSumPktstat");
	e.innerHTML = _("dash_info sum pktstat");
	e = document.getElementById("dash_infoSumPktstatDet");
	e.innerHTML = _("dash_info sum pktstat det");
	
	//if ((nv_OP_Mode == 0)||(nv_OP_Mode == 1)||(nv_OP_Mode == 5)){
	//	e = document.getElementById("dash_infoSumWlansta");
	//	e.innerHTML = _("dash_info sum wlan");
	//      e = document.getElementById("dash_infoSumWlanstaDet");
	//	e.innerHTML = _("dash_info sum wlan det");
	//}
}

function wlan24g_encrypt_mode(){
      var ieee802_1x = "<% getCfgZero(1, 'IEEE8021X'); %>"
	var Auth = "<% getCfgZero(1, 'AuthMode'); %>"
	var Encry = "<% getCfgZero(1, 'EncrypType'); %>"

	if (ieee802_1x.charAt(0) == 1 ){
		document.write("<span class='table_font2'>802.1x</span>");
	}else if ((Auth.split(";",1) == "OPEN") && (Encry.split(";",1) == "NONE")){
		document.write("<span class='table_font2'><font id='dash_infoWlanSecNosec2'></font></span>");
		e = document.getElementById("dash_infoWlanSecNosec2");
		e.innerHTML = _("dash_info wlan sec nosec");
	}else if (Auth.split(";",1) == "SHARED"){
		document.write("<span class='table_font2'><font id='dash_infoWlanSecWEP1'></font></span>");
		e = document.getElementById("dash_infoWlanSecWEP1");
		e.innerHTML = _("dash_info wlan sec wep");
	}else if (Auth.split(";",1) == "WPAPSK"){
		document.write("<span class='table_font2'><font id='dash_infoWlanSecWPAPSK1'></font></span>");
		e = document.getElementById("dash_infoWlanSecWPAPSK1");
		e.innerHTML = _("dash_info wlan sec wpapsk");
	}else if (Auth.split(";",1) == "WPA2PSK"){
		document.write("<span class='table_font2'><font id='dash_infoWlanSecWPA2PSK1'></font></span>");
		e = document.getElementById("dash_infoWlanSecWPA2PSK1");
		e.innerHTML = _("dash_info wlan sec wpa2psk");
	}else if (Auth.split(";",1) == "WPAPSKWPA2PSK"){
		document.write("<span class='table_font2'><font id='dash_infoWlanSecWPAPSKWPA2PSK'></font></span>");
		e = document.getElementById("dash_infoWlanSecWPAPSKWPA2PSK");
		e.innerHTML = _("dash_info wlan sec wpapskwpa2psk");
	}else if (Auth.split(";",1) == "WPAWPA2"){
		document.write("<span class='table_font2'><font id='dash_infoWlanSecWPAWPA2'></font></span>");
		e = document.getElementById("dash_infoWlanSecWPAWPA2");
		e.innerHTML = _("dash_info wlan sec wpawpa2");
	}else{
		document.write("<span class='table_font2'>"+Auth.split(";",1)+"</span>");
	}
}
function wlan24g_80211_mode(){
      	document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWlan80211mode'></font>:</span></td><td>");
	if (<% getCfgZero(1, "WirelessMode"); %> == 0){
		document.write("<span class='table_font2'>802.11b/g</span>");
	}else if (<% getCfgZero(1, "WirelessMode"); %> == 1){
		document.write("<span class='table_font2'>802.11b</span>");
	}else if (<% getCfgZero(1, "WirelessMode"); %> == 4){
		document.write("<span class='table_font2'>802.11g</span>");
	}else if (<% getCfgZero(1, "WirelessMode"); %> == 6){
		document.write("<span class='table_font2'>802.11n</span>");
	}else if (<% getCfgZero(1, "WirelessMode"); %> == 9){
		document.write("<span class='table_font2'>802.11b/g/n</span>");
	}
	document.write("</td></tr>");
}
function wlan24g_channel_mode(){
	  if (nv_OP_Mode != 5) {
		  document.write("<tr >");
		  document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWlanChan'></font>:</span></td>");
		  document.write("<td >");

		  if (<% getCfgZero(1, "Channel"); %> == 0 ){
			   document.write("<span class='table_font2'><font id='dash_infoWlanChanAuto'></font></span>");
			   e = document.getElementById("dash_infoWlanChanAuto");
			   e.innerHTML = _("dash_info wlan chan auto");
		  }else
		  {
			   document.write("<span class='table_font2'><font id='dash_infoWlanChanfixed'></font></span>");
			   e = document.getElementById("dash_infoWlanChanfixed");
			   e.innerHTML = _("dash_info wlan chan fixed");
		  }
	  
		  //document.write("<span class='table_font2'><% getOpChannleASP(); %></span>");      			
		  document.write("</td>");
		  document.write("</tr>");
	  }
	  
      document.write("<tr >");
      document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWlanOpchan'></font>:</span></td>");
      document.write("<td ><span class='table_font2'><% getOpChannleASP(); %></span></td>");
      document.write("</tr>");
}

function getCfgGeneralSsid(){
     document.write("<tr >");
     document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWlanStatusNamessid'>(SSID)</font>:</span></td>");
     document.write("<td ><span class='table_font2'><% getCfgGeneral(1, "SSID1"); %></span></td>");
     document.write("</tr>");
     e = document.getElementById("dash_infoWlanStatusNamessid");
     e.innerHTML = _("dash_info wlan status namessid");
}
//for 5g interface as follow
function getCfg5gGeneralSsid(){
     document.write("<tr >");
     document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash1_infoWlanStatusNamessid'>(SSID)</font>:</span></td>");
     document.write("<td ><span class='table_font2'><% getCfgGeneral(1, "SSID5G1"); %></span></td>");
     document.write("</tr>");
     e = document.getElementById("dash1_infoWlanStatusNamessid");
     e.innerHTML = _("dash_info wlan status namessid");
}
function wlan5g_channel_mode(){
      document.write("<tr >");
	document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash1_infoWlanChan'></font>:</span></td>");
	document.write("<td >");
	if (<% getCfgZero(1, "Channel_5G"); %> == 0 ){
		document.write("<span class='table_font2'><font id='dash1_infoWlanChanAuto'></font></span>");
		e = document.getElementById("dash1_infoWlanChanAuto");
		e.innerHTML = _("dash_info wlan chan auto");
	}else
	{
		document.write("<span class='table_font2'><font id='dash1_infoWlanChanfixed'></font></span>");
		e = document.getElementById("dash1_infoWlanChanfixed");
		e.innerHTML = _("dash_info wlan chan fixed");
	}
	//document.write("<span class='table_font2'><% getOpChannleASP5G(); %></span>");
			
	document.write("</td>");
	document.write("</tr>");
	document.write("<tr >");
	document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash1_infoWlanOpchan'></font>:</span></td>");
	document.write("<td ><span class='table_font2'><% getOpChannleASP5G(); %></span></td>");
	document.write("</tr>");
}
function wlan5g_encrypt_mode(){
      var ieee802_1x = "<% getCfgZero(1, 'IEEE8021X'); %>"
	var Auth = "<% getCfgZero(1, 'Auth5GMode'); %>"
	var Encry = "<% getCfgZero(1, 'Encryp5GType'); %>"

	if (ieee802_1x.charAt(0) == 1 ){
		document.write("<span class='table_font2'>802.1x</span>");
	}else if ((Auth.split(";",1) == "OPEN") && (Encry.split(";",1) == "NONE")){
		document.write("<span class='table_font2'><font id='dash1_infoWlanSecNosec2'></font></span>");
		e = document.getElementById("dash1_infoWlanSecNosec2");
		e.innerHTML = _("dash_info wlan sec nosec");
	}else if (Auth.split(";",1) == "SHARED"){
		document.write("<span class='table_font2'><font id='dash1_infoWlanSecWEP1'></font></span>");
		e = document.getElementById("dash1_infoWlanSecWEP1");
		e.innerHTML = _("dash_info wlan sec wep");
	}else if (Auth.split(";",1) == "WPAPSK"){
		document.write("<span class='table_font2'><font id='dash1_infoWlanSecWPAPSK1'></font></span>");
		e = document.getElementById("dash1_infoWlanSecWPAPSK1");
		e.innerHTML = _("dash_info wlan sec wpapsk");
	}else if (Auth.split(";",1) == "WPA2PSK"){
		document.write("<span class='table_font2'><font id='dash1_infoWlanSecWPA2PSK1'></font></span>");
		e = document.getElementById("dash1_infoWlanSecWPA2PSK1");
		e.innerHTML = _("dash_info wlan sec wpa2psk");
	}else if (Auth.split(";",1) == "WPAPSKWPA2PSK"){
		document.write("<span class='table_font2'><font id='dash1_infoWlanSecWPAPSKWPA2PSK'></font></span>");
		e = document.getElementById("dash1_infoWlanSecWPAPSKWPA2PSK");
		e.innerHTML = _("dash_info wlan sec wpapskwpa2psk");
	}else if (Auth.split(";",1) == "WPAWPA2"){
		document.write("<span class='table_font2'><font id='dash1_infoWlanSecWPAWPA2'></font></span>");
		e = document.getElementById("dash1_infoWlanSecWPAWPA2");
		e.innerHTML = _("dash_info wlan sec wpawpa2");
	}else{
		document.write("<span class='table_font2'>"+Auth.split(";",1)+"</span>");
	}
}
function wlan5g_80211_mode(){
      document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash1_infoWlan80211mode'></font>:</span></td><td>");
	if (<% getCfgZero(1, "Wireless5GMode"); %> == 8){
	    document.write("<span class='table_font2'>802.11a/n</span>");
	}else if (<% getCfgZero(1, "Wireless5GMode"); %> == 2){
	    document.write("<span class='table_font2'>802.11a</span>");
	}else if (<% getCfgZero(1, "Wireless5GMode"); %> == 11){
	   document.write("<span class='table_font2'>802.11n</span>");
	}
	 document.write("</td></tr>");
}

</script>
</head>
<body onload="initTranslation();">
<form method="post" name="NTP" action="/goform/NTP">
<div id="popup"></div>
<div id="contentframe">
  <div>
    <table width="100%" border="0" cellspacing="0" cellpadding="0"><tr>
        <td width="1" bgcolor="#779bc8"><img src="../images/air.gif" width="1" height="38" /></td>
        <td width="100%"  valign="top" bgcolor="#39639e">
<!-- Arthur Chow 2008-12-31
		<div class="contentdata"><div class="content_title">DASHBOARD</div></div> <br class="clearfloat" /><div class="vd"><ul><li class="vd_left"></li> <li class="vd_center"><span class="vd_title">Virtual Device</span><span class="vd_flash"></span></li>
		<li class="vd_right"></li></ul> </div> <br/><br class="clearfloat" />-->
        <div class="pannel2">
		<div class="leftpannel">

        <div class="w_text3">
   <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="table_frame">
     <tr>
       <td><table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
         <tr>
           <td><table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
             <tr>
               <td><table width="100%" cellpadding="0" cellspacing="0" >
                 <tr>
                   <td height="26" colspan="2"><table width="100%" border="0" cellspacing="0" cellpadding="0">
                       <tr>
                         <td height="26" background="images/table_top_center.gif" class="table_title"><font id="dash_infoTitle"></font></td>
                       </tr>
                   </table></td>
                 </tr>
                 <tr >
                   <td width="60%" valign="top"><span class="top_font"><font id="dash_infoItem"></font></span></td>
                   <td width="50%"><span class="top_font"><font id="dash_infoData"></font></span></td>
                 </tr>
                 <tr >
                   <td valign="top"><span class="table_font"><font id="dash_infoHostName"></font>:</span></td>
                   <td ><span class="table_font2"><% getCfgGeneral(1, 'SystemName'); %></span></td>
                 </tr>

                 <tr >
                   <td valign="top"><span class="table_font"><font id="dash_infoFwVer"></font>: </span></td>
                   <td ><span class="table_font2"><% getFirmwareVersion(); %></span></td>
                 </tr>
                 <tr >
                   <td valign="top"><span class="table_font"><font id="dash_infoSysOpmode"></font>: </span></td>
                   <td >
<script>
	var e;
	if (nv_OP_Mode == 0){
		document.write("<span class='table_font2'><font id='dash_infoSysRouter'></font></span>");
		e = document.getElementById("dash_infoSysRouter");
		e.innerHTML = _("dash_info sys router");
	}
	
	if (nv_OP_Mode == 1){
		document.write("<span class='table_font2'><font id='dash_infoSysAp'></font></span>");
		e = document.getElementById("dash_infoSysAp");
		e.innerHTML = _("dash_info sys ap");
	}

	if (nv_OP_Mode == 2){
		document.write("<span class='table_font2'><font id='dash_infoSysClient'></font></span>");
		e = document.getElementById("dash_infoSysClient");
		e.innerHTML = _("dash_info sys clinet");
	}

	if (nv_OP_Mode == 3){
		document.write("<span class='table_font2'><font id='dash_infoSysWds'></font></span>");
		e = document.getElementById("dash_infoSysWds");
		e.innerHTML = _("dash_info sys wds");
	}

	if (nv_OP_Mode == 4){
		document.write("<span class='table_font2'><font id='dash_infoSysApwds'></font></span>");
		e = document.getElementById("dash_infoSysApwds");
		e.innerHTML = _("dash_info sys apwds");
	}

	if (nv_OP_Mode == 5){
		document.write("<span class='table_font2'><font id='dash_infoSysApcli'></font></span>");
		e = document.getElementById("dash_infoSysApcli");
		e.innerHTML = _("dash_info sys apcli");
	}

	if (nv_OP_Mode == 6){
		document.write("<span class='table_font2'><font id='dash_infoSysWisp'></font></span>");
		e = document.getElementById("dash_infoSysWisp");
		e.innerHTML = _("dash_info sys wisp");
	}
	
	if (nv_OP_Mode == 7){
		document.write("<span class='table_font2'><font id='dash_infoSysWispUR'></font></span>");
		e = document.getElementById("dash_infoSysWispUR");
		e.innerHTML = _("dash_info sys wisp ur");
	}
</script>
</td>
</tr>

<script>                              
	if ((nv_OP_Mode == 0)||(nv_OP_Mode == 6)||(nv_OP_Mode == 7)){
		document.write("<tr ><td valign='top'><span class='table_font'><font id='dash_infoWanInfo'></font> :</span></td>");
		document.write("<td ><span class='table_font2'></span></td></tr>");
 		if((wan3G)&&(nv_OP_Mode == 0)){
			document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;  - <font id='dash_infoSIMstatus'></font>:</span></td>");
			if ("<% getSIMstatus(); %>" == "1" )
				document.write("<td valign='top'><span class='table_font2'>"+_("dash_info wan3G ready")+"</span></td></tr>");
			else
				document.write("<td valign='top'><span class='table_font2'>"+_("dash_info wan3G not_ready")+"</span></td></tr>");	
			
			document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWan3GIp'></font>: </span></td>");
			document.write("<td ><span class='table_font2'><% getWanIp(); %></span></td></tr>");
			document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWan3GSubmask'></font>:</span></td>");
			document.write("<td ><span class='table_font2'><% getWanNetmask(); %></span></td></tr>");
			document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWan3GDHCP'></font>:</span></td>");	
			if ("<% getCfgGeneral(1, "wanConnectionMode"); %>" == "DHCP" )
				document.write("<td valign='top'><span class='table_font2'>"+_("Client")+"</span></td></tr>");
			else
				document.write("<td valign='top'><span class='table_font2'>"+_("dash ref none")+"</span></td></tr>");
		}else{
			document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;  - <font id='dash_infoWanMac'></font>:</span></td>");
			document.write("<td ><span class='table_font2'><% getWanMac(); %></span></td></tr>");
			document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWanIp'></font>: </span></td>");
			document.write("<td ><span class='table_font2'><% getWanIp(); %></span></td></tr>");
			document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWanSubmask'></font>:</span></td>");
			document.write("<td ><span class='table_font2'><% getWanNetmask(); %></span></td></tr>");
			document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWanGateway'>"+_('g2-2 Gateway1')+"</font>:</span></td>");
			var wan_gateway="<% getWanGateway(); %>";
			if (wan_gateway.length==0)
				document.write("<td ><span class='table_font2'>0.0.0.0</span></td></tr>");
			else	
				document.write("<td ><span class='table_font2'>"+wan_gateway+"</span></td></tr>");
	
			document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWanContype'>DHCP</font>:</span></td>");
			if ("<% getCfgGeneral(1, "wanConnectionMode"); %>" == "DHCP" )
				document.write("<td valign='top'><span class='table_font2'>"+_("Client")+"</span></td></tr>");
			else
				document.write("<td valign='top'><span class='table_font2'>"+_("dash ref none")+"</span></td></tr>");
 		}                 
	}                 
</script>
<tr >
  <td valign="top"><span class="table_font"><font id="dash_infoLanInfo"></font>:</span></td>
  <td ><span class="table_font2"></span></td>
</tr>
<tr >
  <td valign="top"><span class="table_font">&nbsp;&nbsp;- <font id="dash_infoLanMac"></font>:</span></td>
  <td ><span class="table_font2"><% getLanMac(); %></span></td>
</tr>
<tr >
  <td valign="top"><span class="table_font">&nbsp;&nbsp;- <font id="dash_infoLanIp"></font>: </span></td>
  <td ><span class="table_font2"><% getLanIp(); %></span></td>
</tr>
<tr >
  <td valign="top"><span class="table_font">&nbsp;&nbsp;- <font id="dash_infoLanSubmask"></font>:</span></td>
  <td ><span class="table_font2"><% getLanNetmask(); %></span></td>
</tr>
<script>
	if (nv_OP_Mode == 1){
		var lan_gw="<% getLanIfGateway(); %>";
		document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoLanGateway'>"+_('g2-2 Gateway1')+"</font>:</span></td>");
		document.write("<td ><span class='table_font2'>");
		if (lan_gw.length==0)
			document.write("0.0.0.0");
		else	
			document.write(lan_gw);
		document.write("</span></td></tr>");
	}
</script>
<tr>
<td valign="top"><span class="table_font">&nbsp;&nbsp;- <font id="dash_infoLanDhcp"></font>:</span></td>
<td >
<script>
	var e;
	if ((nv_OP_Mode == 0) || (nv_OP_Mode == 6) || (nv_OP_Mode == 7)){
		if (<% getCfgZero(1, "dhcpEnabled"); %>){
			document.write("<span class='table_font2'><font id='dash_infoLanDhcpSrv'></font></span>");
			e = document.getElementById("dash_infoLanDhcpSrv");
			e.innerHTML = _("dash_info lan dhcp srv");
		}else{
			document.write("<span class='table_font2'><font id='dash_infoLanDhcpDis'>"+_("dash_info lan dhcp dis")+"</font></span>");
			e = document.getElementById("dash_infoLanDhcpDis");
			e.innerHTML = _("dash_info lan dhcp dis");
		}
	}

	if ((nv_OP_Mode == 1) || (nv_OP_Mode == 5)){
		var lanIpmode = '<% getCfgZero(1, "lanIp_mode"); %>';
		if (lanIpmode=="0"){//DHCP
			document.write("<span class='table_font2'><font id=ap_dhcp>"+_("Client")+"</font></span>");
		}else{
			document.write("<span class='table_font2'><font id=ap_dhcp>"+_("dash ref none")+"</font></span>");
		}
	}
</script>
</td>
</tr>
<tr>
<td valign="top"><span class="table_font"><font id="dash_infoWlanInfo"></font>:</span></td>
<td><span class="table_font2"></span></td>
</tr>
<tr>
<td valign="top"><span class="table_font">&nbsp;&nbsp;- <font id="dash_infoWlanOpmode"></font>: </span></td>
<td>
<script>
	var e;
	if (nv_OP_Mode == 0){
		if (nv_WDS_Mode==0){
			document.write("<span class='table_font2'><font id='dash_infoWlanAp0'></font></span>");
			e = document.getElementById("dash_infoWlanAp0");
			e.innerHTML = _("dash_info wlan ap0");
		}
		
		if (nv_WDS_Mode==2){
			document.write("<span class='table_font2'><font id='dash_infoWlanAp0'></font></span>");
			e = document.getElementById("dash_infoWlanAp0");
			e.innerHTML = _("dash_info sys wds");
		}
		
		if (nv_WDS_Mode==1){
			document.write("<span class='table_font2'><font id='dash_infoWlanAp0'></font></span>");
			e = document.getElementById("dash_infoWlanAp0");
			e.innerHTML = _("dash_info sys apwds");
		}
	}
	
	if (nv_OP_Mode == 1){
		if (nv_WDS_Mode==0){
			document.write("<span class='table_font2'><font id='dash_infoWlanAp1'></font></span>");
			e = document.getElementById("dash_infoWlanAp1");
			e.innerHTML = _("dash_info wlan ap1");
		}
		
		if (nv_WDS_Mode==2){
			document.write("<span class='table_font2'><font id='dash_infoWlanAp1'></font></span>");
			e = document.getElementById("dash_infoWlanAp1");
			e.innerHTML = _("dash_info sys wds");
		}
		if (nv_WDS_Mode==1){
			document.write("<span class='table_font2'><font id='dash_infoWlanAp1'></font></span>");
			e = document.getElementById("dash_infoWlanAp1");
			e.innerHTML = _("dash_info sys apwds");
		}
	}
	
	if (nv_OP_Mode == 2){
		document.write("<span class='table_font2'><font id='dash_infoWlanCli'></font></span>");
		e = document.getElementById("dash_infoWlanCli");
		e.innerHTML = _("dash_info wlan cli");
	}
	
	if (nv_OP_Mode == 3){
		document.write("<span class='table_font2'><font id='dash_infoWlanWds'></font></span>");
		e = document.getElementById("dash_infoWlanWds");
		e.innerHTML = _("dash_info wlan wds");
	}
	
	if (nv_OP_Mode == 4){
		document.write("<span class='table_font2'><font id='dash_infoWlanApwds'></font></span>");
		e = document.getElementById("dash_infoWlanApwds");
		e.innerHTML = _("dash_info wlan apwds");
	}
	
	if (nv_OP_Mode == 5){
		document.write("<span class='table_font2'><font id='dash_infoWlanApcli'></font></span>");
		e = document.getElementById("dash_infoWlanApcli");
		if (board_type == "0A52" || board_type == "0A22")
		{
			if (bridge_mode == 1)
				e.innerHTML = _("dash_info wlan cli");
			else
				e.innerHTML = _("dash_info wlan ap0");
		}
		else
		{
			if (bridge_mode == 1)
				e.innerHTML = _("dash_info wlan apcli");
			else
				e.innerHTML = _("dash_info wlan cli");
		}
	}
	
	if (nv_OP_Mode == 6){
	       if("<% getCfgZero(1, "wisp2.4g_mode"); %>" == "wisp"){
	     		document.write("<span class='table_font2'><font id='dash_infoWlanWisp'></font></span>");
	     		e = document.getElementById("dash_infoWlanWisp");
	     		e.innerHTML = _("dash_info wlan wisp");
	       }else{
			if (nv_WDS_Mode==0){
				document.write("<span class='table_font2'><font id='dash_infoWlanAp1'></font></span>");
				e = document.getElementById("dash_infoWlanAp1");
				e.innerHTML = _("dash_info wlan ap1");
			}
			
			if (nv_WDS_Mode==2){
				document.write("<span class='table_font2'><font id='dash_infoWlanAp1'></font></span>");
				e = document.getElementById("dash_infoWlanAp1");
				e.innerHTML = _("dash_info sys wds");
			}
			if (nv_WDS_Mode==1){
				document.write("<span class='table_font2'><font id='dash_infoWlanAp1'></font></span>");
				e = document.getElementById("dash_infoWlanAp1");
				e.innerHTML = _("dash_info sys apwds");
			}
	       }
	}
	
	if (nv_OP_Mode == 7){
		document.write("<span class='table_font2'><font id='dash_infoWlanWispUR'></font></span>");
		e = document.getElementById("dash_infoWlanWispUR");
		e.innerHTML = _("dash_info wlan wisp ur");
	}
</script>
</td>
</tr>
<tr >
  <td valign="top"><span class="table_font">&nbsp;&nbsp;- <font id="dash_infoWlanMac"></font>:</span></td>
  <td >
  	<script>
		if ( nv_OP_Mode==1 ) {
			document.write("<span class='table_font2'><% getWlanMac('rai0'); %></span>");	
		} else if (nv_OP_Mode==5 || nv_OP_Mode==6) {
			document.write("<span class='table_font2'><% getWlanMac('apclii0'); %></span>");
		}

	</script>
  </td>
</tr>
<tr >
  <td valign="top"><span class="table_font">&nbsp;&nbsp;- <font id="dash_infoWlanStatus"></font>:</span></td>
  <td >
<script>
	var e;
	var wlanstatus;
	if (nv_OP_Mode==1) {
		wlanstatus="<% getRadioStatusASP('rai0'); %>";
	} else if (nv_OP_Mode==5 || nv_OP_Mode==6) {
		wlanstatus="<% getRadioStatusASP('apclii0'); %>";
	}

	if (wlanstatus=="OFF"){
		document.write("<span class='table_font2'><font id='dash_infoWlanStatusOff'></font></span>");
		e = document.getElementById("dash_infoWlanStatusOff");
		e.innerHTML = _("dash_info wlan status off");
	}else if (wlanstatus=="ON") {
		document.write("<span class='table_font2'><font id='dash_infoWlanStatusOn'></font></span>");
		e = document.getElementById("dash_infoWlanStatusOn");
		e.innerHTML = _("dash_info wlan status on");
	} else {
		document.write("<span class='table_font2'><font id='dash_infoWlanStatusUnkown'></font></span>");
		e = document.getElementById("dash_infoWlanStatusUnkown");
		e.innerHTML = "unkown";
	}
</script>
</td>
</tr>
<script>
	var e;

	if (nv_OP_Mode == 5 || nv_OP_Mode == 6){
		document.write("<tr >");
		document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWlanStatusNameEssid'></font>:</span></td>");
		var associate_status="<% getWISPapcli0ConnectStatus(); %>";
		if (associate_status=="None")
			document.write("<td ><span class='table_font2'> </span></td>");
		else {
			document.write("<td ><span class='table_font2'>");
			<% getWISPapcli0ConnectESSID('apclii0'); %>
			document.write("<% getWISPapcli0ConnectESSIDMAC('apclii0'); %></span></td>");
		}
		
		document.write("</tr>");
		e = document.getElementById("dash_infoWlanStatusNameEssid");
		e.innerHTML = _("dash_info wlan status namessid");
		document.write("<tr >");
		document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWlanWISPConnectionStatus'></font>:</span></td>");
		
		if (associate_status=="None")
			document.write("<td ><span class='table_font2'>"+_('Disassociated')+"</span></td>");
		if (associate_status=="Connected")
			document.write("<td ><span class='table_font2'>"+_('Associated')+"</span></td>");
		document.write("</tr>");
		e = document.getElementById("dash_infoWlanWISPConnectionStatus");
		e.innerHTML = _("Connect Status");
		if(associate_status=="Connected") {
			document.write("<tr>");
			document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWlanWISPRSSI'></font>:</span></td>");
			document.write("<td ><span class='table_font2'><% getwifiRSSI(); %></span></td>");
			document.write("</tr>");
			e = document.getElementById("dash_infoWlanWISPRSSI");
			e.innerHTML=_("dash_info rssi");
		}
	}else{
		getCfgGeneralSsid();
	}
</script>
<script>
	var e;
	if (nv_OP_Mode != 6){
            wlan24g_channel_mode();
      }
</script>
<tr>
<td valign="top"><span class="table_font">&nbsp;&nbsp;- <font id="dash_infoWlanSec"></font>:</span></td>
<td>
<script>
	var op_mode = <% getCfgZero(1, "OP_Mode"); %>;
	if (op_mode == 6){
             if("<% getCfgZero(1, "wisp2.4g_mode"); %>" == "wisp"){
        		var associate_status="<% getWISPapcli0ConnectStatus(); %>";		
        		if (associate_status=="None"){
        			document.write("<span class='table_font2'><font id='dash_infoWlanSecNosec'></font></span>");
        			e = document.getElementById("dash_infoWlanSecNosec");
        			e.innerHTML = _("dash_info wlan sec nosec");
        		}else{
        			var Auth = "<% getCfgGeneral(1, 'ApCliAuthMode'); %>";
        			var wepKey1 = "<% getCfgGeneral(1, 'ApCliKey1Str'); %>";
        			var wepKey2 = "<% getCfgGeneral(1, 'ApCliKey2Str'); %>";
        			var wepKey3 = "<% getCfgGeneral(1, 'ApCliKey3Str'); %>";
        			var wepKey4 = "<% getCfgGeneral(1, 'ApCliKey4Str'); %>";
        			
        			if ((Auth == "OPEN") && (wepKey1 == "") && (wepKey2 == "") && (wepKey3 == "") && (wepKey4 == "")){
        				document.write("<span class='table_font2'><font id='dash_infoWlanSecNosec1'></font></span>");
        				e = document.getElementById("dash_infoWlanSecNosec1");
        				e.innerHTML = _("dash_info wlan sec nosec");			
        			}else if ((Auth == "OPEN") || (Auth == "SHARED")){
        				document.write("<span class='table_font2'><font id='dash_infoWlanSecWEP'></font></span>");
        				e = document.getElementById("dash_infoWlanSecWEP");
        				e.innerHTML = _("dash_info wlan sec wep");
        			}else if (Auth == "WPAPSK"){
        				document.write("<span class='table_font2'><font id='dash_infoWlanSecWPAPSK'></font></span>");
        				e = document.getElementById("dash_infoWlanSecWPAPSK");
        				e.innerHTML = _("dash_info wlan sec wpapsk");
        			}else if (Auth == "WPA2PSK"){
        				document.write("<span class='table_font2'><font id='dash_infoWlanSecWPA2PSK'></font></span>");
        				e = document.getElementById("dash_infoWlanSecWPA2PSK");
        				e.innerHTML = _("dash_info wlan sec wpa2psk");
        			}
        		}
	       }else{
                    wlan24g_encrypt_mode();
                    wlan24g_channel_mode();
                    wlan24g_80211_mode();                    
	       }
	}else{
              wlan24g_encrypt_mode();
	}
</script>
</td>
</tr>
<script>
	if (nv_OP_Mode != 6){
	wlan24g_80211_mode();
}
</script>
<script>
/************
var e;
if (nv_OP_Mode == 5) {
	document.write("<tr >");
	document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoWlanStationStatus'>WLAN Station Status</font>:</span></td>");
	var associate_status="<% getAPClientConnectionStatus(); %>";
	if (associate_status=="None")
		document.write("<td ><span class='table_font2'>"+_('Disassociated')+"</span></td>");
	if (associate_status=="Connected")
		document.write("<td ><span class='table_font2'><% getAPClientConnectionESSID(); %><% getAPClientConnectionESSIDMAC(); %></span></td>");
	document.write("</tr>");
	e = document.getElementById("dash_infoWlanStationStatus");
	if (board_type == "0A22")
		e.innerHTML = _("dash_info sum wlan-1");
	else
		e.innerHTML = _("dash_info sum wlan");
}
************/
</script>

<script>
if ((board_type == "0A22") || (board_type == "0A52"))
{
	document.write("<tr>");
	document.write("<td valign='top'><span class='table_font'><font id='dash1_infoWlanInfo'></font>:</span></td>");
	document.write("<td><span class='table_font2'></span></td>");
	document.write("</tr>");
	document.write("<tr>");
	document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash1_infoWlanOpmode'></font>: </span></td>");
	document.write("<td>");
	var e;
	if (nv_OP_Mode == 0){
		if (nv_WDS1_Mode==0){
			document.write("<span class='table_font2'><font id='dash1_infoWlanAp0'></font></span>");
			e = document.getElementById("dash1_infoWlanAp0");
			e.innerHTML = _("dash_info wlan ap0");
		}
		
		if (nv_WDS1_Mode==2){
			document.write("<span class='table_font2'><font id='dash1_infoWlanAp0'></font></span>");
			e = document.getElementById("dash1_infoWlanAp0");
			e.innerHTML = _("dash_info sys wds");
		}
		
		if (nv_WDS1_Mode==1){
			document.write("<span class='table_font2'><font id='dash1_infoWlanAp0'></font></span>");
			e = document.getElementById("dash1_infoWlanAp0");
			e.innerHTML = _("dash_info sys apwds");
		}
	}
	
	if (nv_OP_Mode == 1){
		if (nv_WDS1_Mode==0){
			document.write("<span class='table_font2'><font id='dash1_infoWlanAp1'></font></span>");
			e = document.getElementById("dash1_infoWlanAp1");
			e.innerHTML = _("dash_info wlan ap1");
		}
		
		if (nv_WDS1_Mode==2){
			document.write("<span class='table_font2'><font id='dash1_infoWlanAp1'></font></span>");
			e = document.getElementById("dash1_infoWlanAp1");
			e.innerHTML = _("dash_info sys wds");
		}
		if (nv_WDS1_Mode==1){
			document.write("<span class='table_font2'><font id='dash1_infoWlanAp1'></font></span>");
			e = document.getElementById("dash1_infoWlanAp1");
			e.innerHTML = _("dash_info sys apwds");
		}
	}
	
	if (nv_OP_Mode == 2){
		document.write("<span class='table_font2'><font id='dash1_infoWlanCli'></font></span>");
		e = document.getElementById("dash1_infoWlanCli");
		e.innerHTML = _("dash_info wlan cli");
	}
	
	if (nv_OP_Mode == 3){
		document.write("<span class='table_font2'><font id='dash1_infoWlanWds'></font></span>");
		e = document.getElementById("dash1_infoWlanWds");
		e.innerHTML = _("dash_info wlan wds");
	}
	
	if (nv_OP_Mode == 4){
		document.write("<span class='table_font2'><font id='dash1_infoWlanApwds'></font></span>");
		e = document.getElementById("dash1_infoWlanApwds");
		e.innerHTML = _("dash_info wlan apwds");
	}
	
	if (nv_OP_Mode == 5){
		document.write("<span class='table_font2'><font id='dash1_infoWlanApcli'></font></span>");
		e = document.getElementById("dash1_infoWlanApcli");
		if (board_type == "0A52" || board_type == "0A22")
		{
			if (bridge_mode == 1)
				e.innerHTML = _("dash_info wlan ap1");
			else
				e.innerHTML = _("dash_info wlan cli");
		}
	}
	
	if (nv_OP_Mode == 6){
	       if("<% getCfgZero(1, "wisp5g_mode"); %>" == "wisp"){
			document.write("<span class='table_font2'><font id='dash1_infoWlanWisp'></font></span>");
			e = document.getElementById("dash1_infoWlanWisp");
			e.innerHTML = _("dash_info wlan wisp");
	       }else{
			if (nv_WDS1_Mode==0){
				document.write("<span class='table_font2'><font id='dash1_infoWlanAp1'></font></span>");
				e = document.getElementById("dash1_infoWlanAp1");
				e.innerHTML = _("dash_info wlan ap1");
			}			
			if (nv_WDS1_Mode==2){
				document.write("<span class='table_font2'><font id='dash1_infoWlanAp1'></font></span>");
				e = document.getElementById("dash1_infoWlanAp1");
				e.innerHTML = _("dash_info sys wds");
			}
			if (nv_WDS1_Mode==1){
				document.write("<span class='table_font2'><font id='dash1_infoWlanAp1'></font></span>");
				e = document.getElementById("dash1_infoWlanAp1");
				e.innerHTML = _("dash_info sys apwds");
			}
	       }
	}
	
	if (nv_OP_Mode == 7){
		document.write("<span class='table_font2'><font id='dash1_infoWlanWispUR'></font></span>");
		e = document.getElementById("dash1_infoWlanWispUR");
		e.innerHTML = _("dash_info wlan wisp ur");
	}

	document.write("</td>");
	document.write("</tr>");
	document.write("<tr >");
	document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash1_infoWlanMac'></font>:</span></td>");
	document.write("<td ><span class='table_font2'><% getWlan5GMac(); %></span></td>");
	document.write("</tr>");
	document.write("<tr >");
	document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash1_infoWlanStatus'></font>:</span></td>");
	document.write("<td >");
	var wlanstatus="<% getRadioStatusASP5G(); %>";
	if (wlanstatus=="OFF"){
		document.write("<span class='table_font2'><font id='dash1_infoWlanStatusOff'></font></span>");
		e = document.getElementById("dash1_infoWlanStatusOff");
		e.innerHTML = _("dash_info wlan status off");
	}else{
		document.write("<span class='table_font2'><font id='dash1_infoWlanStatusOn'></font></span>");
		e = document.getElementById("dash1_infoWlanStatusOn");
		e.innerHTML = _("dash_info wlan status on");
	}
	document.write("</td>");
	document.write("</tr>");
	if (nv_OP_Mode == 6){
		if("<% getCfgZero(1, "wisp5g_mode"); %>" == "wisp"){
             	       document.write("<tr >");
              	document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash1_infoWlanStatusNameEssid'></font>:</span></td>");
              	document.write("<td ><span class='table_font2'><% getWISP1apcli0ConnectESSID(); %><% getWISP1apcli0ConnectESSIDMAC(); %></span></td>");
              	document.write("</tr>");
              	e = document.getElementById("dash1_infoWlanStatusNameEssid");
              	e.innerHTML = _("dash_info wlan status namessid");
       		document.write("<tr >");
       		document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash1_infoWlanWISPConnectionStatus'></font>:</span></td>");
       		var associate_status="<% getWISP1apcli0ConnectStatus(); %>";
       		if (associate_status=="None")
       			document.write("<td ><span class='table_font2'>"+_('Disassociated')+"</span></td>");
       			
       		if (associate_status=="Connected")
       			document.write("<td ><span class='table_font2'>"+_('Associated')+"</span></td>");
       			
       		document.write("</tr>");
       		e = document.getElementById("dash1_infoWlanWISPConnectionStatus");
       		e.innerHTML = _("Connect Status");
		}else{
		     getCfg5gGeneralSsid();
		}
	}else{
           getCfg5gGeneralSsid();
      }
	if (nv_OP_Mode != 6){
	    wlan5g_channel_mode();
	}
	document.write("</tr>");
	document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash1_infoWlanSec'></font>:</span></td>");
	document.write("<td>");
	var op_mode = <% getCfgZero(1, "OP_Mode"); %>;
	if (op_mode == 6){
	       if("<% getCfgZero(1, "wisp5g_mode"); %>" == "wisp"){
       		var associate_status="<% getWISP1apcli0ConnectStatus(); %>";		
       		if (associate_status=="None"){
       			document.write("<span class='table_font2'><font id='dash1_infoWlanSecNosec'></font></span>");
       			e = document.getElementById("dash1_infoWlanSecNosec");
       			e.innerHTML = _("dash_info wlan sec nosec");
       		}else{
       			var Auth = "<% getCfgGeneral(1, 'ApCli5GAuthMode'); %>";
       			var wepKey1 = "<% getCfgGeneral(1, 'ApCli5GKey1Str'); %>";
       			var wepKey2 = "<% getCfgGeneral(1, 'ApCli5GKey2Str'); %>";
       			var wepKey3 = "<% getCfgGeneral(1, 'ApCli5GKey3Str'); %>";
       			var wepKey4 = "<% getCfgGeneral(1, 'ApCli5GKey4Str'); %>";
       			
       			if ((Auth == "OPEN") && (wepKey1 == "") && (wepKey2 == "") && (wepKey3 == "") && (wepKey4 == "")){
       				document.write("<span class='table_font2'><font id='dash1_infoWlanSecNosec1'></font></span>");
       				e = document.getElementById("dash1_infoWlanSecNosec1");
       				e.innerHTML = _("dash_info wlan sec nosec");			
       			}else if ((Auth == "OPEN") || (Auth == "SHARED")){
       				document.write("<span class='table_font2'><font id='dash1_infoWlanSecWEP'></font></span>");
       				e = document.getElementById("dash1_infoWlanSecWEP");
       				e.innerHTML = _("dash_info wlan sec wep");
       			}else if (Auth == "WPAPSK"){
       				document.write("<span class='table_font2'><font id='dash1_infoWlanSecWPAPSK'></font></span>");
       				e = document.getElementById("dash1_infoWlanSecWPAPSK");
       				e.innerHTML = _("dash_info wlan sec wpapsk");
       			}else if (Auth == "WPA2PSK"){
       				document.write("<span class='table_font2'><font id='dash1_infoWlanSecWPA2PSK'></font></span>");
       				e = document.getElementById("dash1_infoWlanSecWPA2PSK");
       				e.innerHTML = _("dash_info wlan sec wpa2psk");
       			}
       		}
		}else{
		      //add for wisp mode when 5g radio is ap mode-----
                    wlan5g_encrypt_mode();
                    wlan5g_channel_mode();
                    wlan5g_80211_mode();                    
	     }
	}else{
           wlan5g_encrypt_mode();
	}
	document.write("</td>");
	document.write("</tr>");
	if (nv_OP_Mode != 6){
           wlan5g_80211_mode();
      }
	if (nv_OP_Mode == 5) {
		document.write("<tr >");
		document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash1_infoWlanStationStatus'>WLAN Station Status</font>:</span></td>");
		var associate_status="<% getAPClient5GConnectionStatus(); %>";
		if (associate_status=="None")
			document.write("<td ><span class='table_font2'>"+_('Disassociated')+"</span></td>");
		if (associate_status=="Connected")
			document.write("<td ><span class='table_font2'><% getAPClient5GConnectionESSID(); %><% getAPClient5GConnectionESSIDMAC(); %></span></td>");
		document.write("</tr>");
		e = document.getElementById("dash1_infoWlanStationStatus");
		if (board_type == "0A52")
			e.innerHTML = _("dash_info sum wlan1");
		else
			e.innerHTML = _("dash_info sum wlan-2");
	}
}
</script>
<!--<script>
	var wps_unconfigured = <% getWPSunconfigured(); %>;
	if (nv_OP_Mode != 6){
		document.write("<tr >");
		document.write("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- WPS:</span></td>");
		if (wps_unconfigured==1)
			document.write("<td ><span class='table_font2'><a target='_parent' href='/local/advance/main_config_frame.asp?1'><font id='dash_infoWlan80211modeConfig'>"+_("Unconfigured")+"</font></a></span></td>");
		else
			document.write("<td ><span class='table_font2'><a target='_parent' href='/local/advance/main_config_frame.asp?1'><font id='dash_infoWlan80211modeConfig'>"+_("Configured")+"</font></a></span></td>");
		
		document.write("</tr>");
		e = document.getElementById("dash_infoWlan80211modeConfig");
	}
</script>-->
</table></td>
</tr>
</table></td>
</tr>
</table></td>
</tr>
</table>
</div>
<script>
if ((board_type == "0A22") || (board_type == "0A52"))
{
	document.write("<br/>");
	document.write("<br/>");
}
</script>
 
		  <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
              <td></td>
            </tr>
          </table>
		</div>
		<div class="rightpannel">
		<div class="w_text3">
		    <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="table_frame">
              <tr>
                <td><table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td><table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
                          <tr>
                            <td><table width="100%" cellpadding="0" cellspacing="0" >
                                <tr>
                                  <td height="26" colspan="3"><table width="100%" border="0" cellspacing="0" cellpadding="0">
                                      <tr>
                                        <td height="26" background="images/table_top_center.gif" class="table_title"><font id="dash_infoIfstatTitle"></font> 	</td>
                                      </tr>
                                  </table></td>
                                </tr>
                                <tr >
                                  <td width="" valign="top"><span class="top_font"><font id="dash_infoIfstatif"></font></span></td>
                                  <td width="" valign="top"><span class="top_font"><font id="dash_infoIfstatStat"></font></span></td>
                                  <td width=""><span class="top_font"><font id="dash_infoIfstatRate"></font></span></td>
                                </tr>
<script>
	if (nv_OP_Mode == 0){
 		if (wan3G){
			document.write("<tr ><td valign='top'><span class='table_font'><font id='dash_infoIfstatWan3G'></font></span></td>");
			document.write("<td valign='top'><span class='table_font'>");
			var wan3G_status="<% getWAN3GLinkStatus(); %>";
			if (wan3G_status=="Down"){
				document.write("<font id='dash_infoIfstatWan3GDown'></font>");
				e = document.getElementById("dash_infoIfstatWan3GDown");
				e.innerHTML = _("dash_info ifstat wan3G down");
			}else{
				document.write("<font id='dash_infoIfstatWan3GUp'></font>");
				e = document.getElementById("dash_infoIfstatWan3GUp");
				e.innerHTML = _("dash_info ifstat wan3G up");
			}
	
			document.write("</span></td>");
			document.write("<td valign='top'><span class='table_font'>");
	
			if (wan3G_status=="Down")
				document.write("&nbsp");
			else
				document.write("7.2Mbps");
			document.write("</span></td></tr>");
 		}else{
			document.write("<tr ><td valign='top'><span class='table_font'><font id='dash_infoIfstatWan'></font></span></td>");
			document.write("<td valign='top'><span class='table_font'>");
			var wan_status="<% getWANLinkStatus(); %>";
			if (wan_status=="Down"){
				document.write("<font id='dash_infoIfstatWanDown'></font>");
				e = document.getElementById("dash_infoIfstatWanDown");
				e.innerHTML = _("dash_info ifstat wan down");
			}else{
				document.write("<font id='dash_infoIfstatWanUp'></font>");
				e = document.getElementById("dash_infoIfstatWanUp");
				e.innerHTML = _("dash_info ifstat wan up");
			}
			document.write("</span></td>");
			document.write("<td valign='top'><span class='table_font'>");
			if (wan_status=="Down")
				document.write("&nbsp");
			else
				document.write(wan_status);
			document.write("</span></td></tr>");

            document.write("<tr ><td valign='top'><span class='table_font'>LAN1</span></td>");
                        document.write("<td valign='top'><span class='table_font'>");
                        var Lan1_status="<% getLANLinkStatus(1,"1"); %>";
                        if (Lan1_status=="Down"){
                                document.write("<font id='dash_infoIfstatLan1Down'></font>");
                                e = document.getElementById("dash_infoIfstatLan1Down");
                                e.innerHTML = _("dash_info ifstat wan down");
                        }else{
                                document.write("<font id='dash_infoIfstatLan1Up'></font>");
                                e = document.getElementById("dash_infoIfstatLan1Up");
                                e.innerHTML = _("dash_info ifstat wan up");
                        }
                        document.write("</span></td>");
                        document.write("<td valign='top'><span class='table_font'>");
                        if (Lan1_status=="Down")
                                document.write("&nbsp");
                        else
                                document.write(Lan1_status);
                        document.write("</span></td></tr>");
                        
                        document.write("<tr ><td valign='top'><span class='table_font'>LAN2</span></td>");
                        document.write("<td valign='top'><span class='table_font'>");
                        var Lan2_status="<% getLANLinkStatus(1,"2"); %>";
                        if (Lan2_status=="Down"){
                                document.write("<font id='dash_infoIfstatLan2Down'></font>");
                                e = document.getElementById("dash_infoIfstatLan2Down");
                                e.innerHTML = _("dash_info ifstat wan down");
                        }else{
                                document.write("<font id='dash_infoIfstatLan2Up'></font>");
                                e = document.getElementById("dash_infoIfstatLan2Up");
                                e.innerHTML = _("dash_info ifstat wan up");
                        }
                        document.write("</span></td>");
                        document.write("<td valign='top'><span class='table_font'>");
                        if (Lan2_status=="Down")
                                document.write("&nbsp");
                        else
                                document.write(Lan2_status);
                        document.write("</span></td></tr>");
                        
                         document.write("<tr ><td valign='top'><span class='table_font'>LAN3</span></td>");
                        document.write("<td valign='top'><span class='table_font'>");
                        var Lan3_status="<% getLANLinkStatus(1,"3"); %>";
                        if (Lan3_status=="Down"){
                                document.write("<font id='dash_infoIfstatLan3Down'></font>");
                                e = document.getElementById("dash_infoIfstatLan3Down");
                                e.innerHTML = _("dash_info ifstat wan down");
                        }else{
                                document.write("<font id='dash_infoIfstatLan3Up'></font>");
                                e = document.getElementById("dash_infoIfstatLan3Up");
                                e.innerHTML = _("dash_info ifstat wan up");
                        }
                        document.write("</span></td>");
                        document.write("<td valign='top'><span class='table_font'>");
                        if (Lan3_status=="Down")
                                document.write("&nbsp");
                        else
                                document.write(Lan3_status);
                        document.write("</span></td></tr>");
                        
                         document.write("<tr ><td valign='top'><span class='table_font'>LAN4</span></td>");
                        document.write("<td valign='top'><span class='table_font'>");
                        var Lan4_status="<% getLANLinkStatus(1,"4"); %>";
                        if (Lan4_status=="Down"){
                                document.write("<font id='dash_infoIfstatLan4Down'></font>");
                                e = document.getElementById("dash_infoIfstatLan4Down");
                                e.innerHTML = _("dash_info ifstat wan down");
                        }else{
                                document.write("<font id='dash_infoIfstatLan4Up'></font>");
                                e = document.getElementById("dash_infoIfstatLan4Up");
                                e.innerHTML = _("dash_info ifstat wan up");
                        }

                        document.write("</span></td>");
                        document.write("<td valign='top'><span class='table_font'>");
                        if (Lan4_status=="Down")
                                document.write("&nbsp");
                        else
                                document.write(Lan4_status);
                        document.write("</span></td></tr>");
		}
	}
</script>	
<tr >
  <td valign="top"><span class="table_font"><font id="dash_infoIfstatWlan"></font></span></td>
  <td valign="top"><span class="table_font">
<script>
	var wlanstatus;
	
	wlanstatus="unkown";
	if (nv_OP_Mode==1)
		wlanstatus="<% getRadioStatusASP('rai0'); %>";
	else if (nv_OP_Mode==5 || nv_OP_Mode==6)
		wlanstatus="<% getRadioStatusASP('apclii0'); %>";
	else
		wlanstatus="<% getRadioStatusASP('rai0'); %>";
		
	if (wlanstatus=="ON"){
		document.write("<font id='dash_infoIfstatWlanUp'></font>");
		e = document.getElementById("dash_infoIfstatWlanUp");
		e.innerHTML = _("dash_info ifstat wlan up");
	}else if (wlanstatus=="OFF"){
		document.write("<font id='dash_infoIfstatWlanDown'></font>");
		e = document.getElementById("dash_infoIfstatWlanDown");
		e.innerHTML = _("dash_info ifstat wlan down");	
	}else {
		document.write("<font id='dash_infoIfstatWlanUnkown'></font>");
		e = document.getElementById("dash_infoIfstatWlanUnkown");
		e.innerHTML = "unkown";	
	}
</script>
</span></td>
<td valign="top"><span class="table_font">
<script>
	var wlan_status="<% getWLANLinkStatus(); %>";
	var wlan_rate="<% getWLANLinkRate(); %>";
	if (wlan_status=="Down")
		document.write("&nbsp");
	else
		document.write(wlan_rate);
</script>
                                  </span></td>
                                </tr>
<script>
	if ((board_type == "0A22") || (board_type == "0A52"))
	{
		document.write("<tr >");
		document.write("<td valign='top'><span class='table_font'><font id='dash1_infoIfstatWlan'></font></span></td>");
		document.write("<td valign='top'><span class='table_font'>");
		var wlan_status="<% getRadioStatusASP5G(); %>";
		if (wlan_status=="ON"){
			document.write("<font id='dash1_infoIfstatWlanUp'></font>");
			e = document.getElementById("dash1_infoIfstatWlanUp");
			e.innerHTML = _("dash_info ifstat wlan up");
		}else{
			document.write("<font id='dash1_infoIfstatWlanDown'></font>");
			e = document.getElementById("dash1_infoIfstatWlanDown");
			e.innerHTML = _("dash_info ifstat wlan down");	
		}
		document.write("</span></td>");
		document.write("<td valign='top'><span class='table_font'>");
		var wlan_status="<% getWLAN5GLinkStatus(); %>";
		var wlan5g_rate="<% getWLAN5GLinkRate(); %>";
		if (wlan_status=="Down")
			document.write("&nbsp");
		else
			document.write(wlan5g_rate);
		document.write("</span></td>");
		document.write("</tr>");
    }
</script>

                            </table></td>
                          </tr>
                      </table></td>
                    </tr>
                </table></td>
              </tr>
            </table>
		  </div>
		  <br/>
		  <br/>
		  <div class="w_text3">
		    <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="table_frame">
              <tr>
                <td><table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td><table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
                          <tr>
                            <td><table width="100%" cellpadding="0" cellspacing="0" >
                                <tr>
                                  <td height="26" colspan="2"><table width="100%" border="0" cellspacing="0" cellpadding="0">
                                      <tr>
                                        <td height="26" background="images/table_top_center.gif" class="table_title"><font id="dash_infoSystatTitle"></font></td>
                                      </tr>
                                  </table></td>
                                </tr>
                                <tr >
                                  <td width="60%" valign="top"><span class="top_font"><font id="dash_infoSystatItem"></font></span></td>
                                  <td width="50%"><span class="top_font"><font id="dash_infoSystatData"></font></span></td>
                                </tr>
                                <tr >
                                  <td valign="top"><span class="table_font"><font id="dash_infoSystatSysuptime"></font>:</span></td>
                                  <td ><span class="table_font2"><% getSysUptime(); %></span></td>
                                </tr>
                                <tr >
                                  <td valign="top"><span class="table_font"><font id="dash_infoSystatCurrtime"></font>:</span></td>
                                  <td><span class="table_font2"><a id="mtenCurrent_Date"></a>&nbsp/&nbsp<a id="mtenCurrent_Time"></a></span></td>
                                  <input type="hidden" name="mtenCurrent_Hour" value="<% getCurrentHour();%>" />
                                  <input type="hidden" name="mtenCurrent_Min" value="<% getCurrentMin();%>" />
                                  <input type="hidden" name="mtenCurrent_Sec" value="<% getCurrentSec();%>" />
                                  <input type="hidden" name="mtenCurrent_Year" value="<% getCurrentYear();%>" />
                                  <input type="hidden" name="mtenCurrent_Mon" value="<% getCurrentMon();%>" />
                                  <input type="hidden" name="mtenCurrent_Day" value="<% getCurrentDay();%>" />
                                </tr>
                                <tr >
                                  <td valign="top"><span class="table_font"><font id="dash_infoSystatSysres"></font>:</span></td>
                                  <td ><span class="table_font2"></span></td>
                                </tr>
<% getSysResource(); %>
                                <!--tr >
                                  <td valign="top"><span class="table_font"><font id="dash_infoSystatSetting"></font>:</span></td>
                                  <td ><span class="table_font2"></span></td>
                                </tr-->
<script>
if ((nv_OP_Mode == 0)||(nv_OP_Mode == 6)||(nv_OP_Mode == 7)){
	document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoSystatSettingFw'></font>:</span></td>");
	document.write("<td >");
	if (<% getCfgZero(1, "FIREWALLEnabled"); %>){
		document.write("<span class='table_font2'><font id='dash_infoSystatSettingEn'></font></span>");
		e = document.getElementById("dash_infoSystatSettingEn");
		e.innerHTML = _("dash_info systat setting en");
	}else{
		document.write("<span class='table_font2'><font id='dash_infoSystatSettingDis'></font></span>");
		e = document.getElementById("dash_infoSystatSettingDis");
		e.innerHTML = _("dash_info systat setting dis");
	}
	document.write("</td></tr>");
	//document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoSystatBm'></font>: </span></td>");
	//document.write("<td >");
	//if (<% getCfgZero(1, "RL_QoSEnable"); %>){
	//	document.write("<span class='table_font2'><font id='dash_infoSystatBmEn'></font></span>");
	//	e = document.getElementById("dash_infoSystatBmEn");
	//	e.innerHTML = _("dash_info systat bm en");
	//}else{
	//	document.write("<span class='table_font2'><font id='dash_infoSystatBmDis'></font></span>");
	//	e = document.getElementById("dash_infoSystatBmDis");
	//	e.innerHTML = _("dash_info systat bm dis");
	//}
	//document.write("</td></tr>");
	document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- UPnP:</span></td>");
	document.write("<td >");
	if (<% getCfgZero(1, "upnpEnabled"); %>){
		document.write("<span class='table_font2'><font id='dash_infoSystatUpnpEn'></font></span>");
		e = document.getElementById("dash_infoSystatUpnpEn");
		e.innerHTML = _("dash_info systat upnp en");
	}else{
		document.write("<span class='table_font2'><font id='dash_infoSystatUpnpDis'></font></span>");
		e = document.getElementById("dash_infoSystatUpnpDis");
		e.innerHTML = _("dash_info systat upnp dis");
	}
	document.write("</td></tr>");
}
</script>
                                <!--tr >
                                  <td valign="top"><span class="table_font">&nbsp;&nbsp;- <font id="dash_infoSystatConfmode"></font>:</span></td>
                                  <td ><span class="table_font2"><font id="dash_infoSystatConfmodeExp"></font></span></td>
                                </tr-->
                            </table></td>
                          </tr>
                      </table></td>
                    </tr>
                </table></td>
              </tr>
            </table>
		  </div>
		  <br />
		  <br />
<script>
if ((nv_OP_Mode == 0)||(nv_OP_Mode == 6)||(nv_OP_Mode == 7)){
        document.write('<div class="w_text3">');
   		document.write('<table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="table_frame">');
     	document.write('<tr>');
       	document.write('<td><table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">');
        document.write('<tr>');
        document.write('<td><table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">');
        document.write('<tr>');
        document.write('<td><table width="100%" cellpadding="0" cellspacing="0" >');
        document.write('<tr>');
        document.write('<td height="26" colspan="2"><table width="100%" border="0" cellspacing="0" cellpadding="0">');
        document.write('<tr>');
        document.write('<td height="26" background="images/table_top_center.gif" class="table_title"><font id="dash_infoIPv6Title"></font></td>');
        document.write('</tr>');
        document.write('</table></td>');
        document.write('</tr>');
        document.write('<tr >');
        document.write('<td width="45%" valign="top"><span class="top_font"><font id="dash_infoIPv6Item"></font></span></td>');
        document.write('<td width="55%"><span class="top_font"><font id="dash_infoIPv6Data"></font></span></td>');
        document.write('</tr>');
       
////  IPv6 Connection Status    
	   
		document.write("<tr ><td valign='top'><span class='table_font'><font id='dash_infoIPv6ConnType'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% getIPv6ConnType(); %></span></td></tr>");
if((wan_ipv6_proto == '0') || (wan_ipv6_proto == '1')){ 
	
	if(wan_ipv6_proto == '0'){
		//IPv6 Static WAN Adrress
		document.write("<tr ><td valign='top'><span class='table_font'><font id='dash_infoIPv6WanAddress'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% getCfgGeneral(1, "wan_ipv6addr"); %>/<% getCfgGeneral(1, "wan_ipv6netsize"); %></span></td></tr>");
	
		//IPv6 Static WAN GATEWAY
		document.write("<tr ><td valign='top'><span class='table_font'><font id='dash_infoIPv6WanGateway'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% getCfgGeneral(1, "wan_ipv6gateway"); %>/<% getCfgGeneral(1, "wan_ipv6netsize"); %></span></td></tr>");
		
		//IPv6 Static WAN 1 dns
		document.write("<tr ><td valign='top'><span class='table_font'><font id='dash_infoIPv6WanFirstDns'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% getIPv6StaticDns(1); %></span></td></tr>");
	
		//IPv6 Static WAN 2 dns
		document.write("<tr ><td valign='top'><span class='table_font'><font id='dash_infoIPv6WanSecondDns'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% getIPv6StaticDns(2); %></span></td></tr>");
	}else if(wan_ipv6_proto == '1'){
		//IPv6 WAN Adrress
		document.write("<tr ><td valign='top'><span class='table_font'><font id='dash_infoIPv6WanAddress'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% getIPv6DhcpWanAddr(); %></span></td></tr>");
	
		//IPv6 WAN GATEWAY
		document.write("<tr ><td valign='top'><span class='table_font'><font id='dash_infoIPv6WanGateway'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% getIPv6DhcpWanDateway(); %></span></td></tr>");
		
		//IPv6 DHCP WAN 1 dns
		document.write("<tr ><td valign='top'><span class='table_font'><font id='dash_infoIPv6WanFirstDns'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% getIPv6DHCPDns(1); %></span></td></tr>");
	
		//IPv6 DHCP WAN 2 dns
		document.write("<tr ><td valign='top'><span class='table_font'><font id='dash_infoIPv6WanSecondDns'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% getIPv6DHCPDns(2); %></span></td></tr>");
	}
		//IPv6 LAN Address
		document.write("<tr ><td valign='top'><span class='table_font'><font id='dash_infoIPv6LanAddress'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% getIPv6LanAddr(); %></span></td></tr>");
}
		//IPv6 Link Local
		document.write("<tr ><td valign='top'><span class='table_font'><font id='dash_infoIPv6LanLinkLocal'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% getLinkLocalAddr(); %></span></td></tr>");


		document.write('</table></td>');
		document.write('</tr>');
		document.write('</table></td>');
		document.write('</tr>');
		document.write('</table></td>');
		document.write('</tr>');
		document.write('</table>');
		document.write('</div>');
		document.write('<br />');
		document.write('<br />');
}
</script>
<script>
if ((nv_OP_Mode == 0)){
	if(wan3G){
        document.write('<div class="w_text3">');
   		document.write('<table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="table_frame">');
     	document.write('<tr>');
       	document.write('<td><table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">');
        document.write('<tr>');
        document.write('<td><table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">');
        document.write('<tr>');
        document.write('<td><table width="100%" cellpadding="0" cellspacing="0" >');
        document.write('<tr>');
        document.write('<td height="26" colspan="2"><table width="100%" border="0" cellspacing="0" cellpadding="0">');
        document.write('<tr>');
        document.write('<td height="26" background="images/table_top_center.gif" class="table_title"><font id="dash_infoSum3GTitle"></font></td>');
        document.write('</tr>');
        document.write('</table></td>');
        document.write('</tr>');
        document.write('<tr >');
        document.write('<td width="60%" valign="top"><span class="top_font"><font id="dash_info3GItem"></font></span></td>');
        document.write('<td width="50%"><span class="top_font"><font id="dash_info3GData"></font></span></td>');
        document.write('</tr>');
       
		
////  3G Connection Status        
//		document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_info3GConnStatus'></font>:</span></td>");
//		document.write("<td ><span class='table_font2'><% get3GConnStatus(); %></span></td></tr>");

////  3G Service Provider
		document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_info3GISP'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% get3GISP(); %></span></td></tr>");
				
////  3G Signal Strength
		document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_info3GSignalStrength'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% get3GSignalStrength(); %> <font id='dash_info3GSSIunit'></font></span></td></tr>");

////  3G Last Connection Up Time
		document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_info3GConnUpTime'></font>:</span></td>");
		var wan3G_connect="<% getWAN3GLinkStatus(); %>";
			if (wan3G_connect=="Down"){
				document.write("<td ><span class='table_font2'></span></td></tr>");
			}else{
				document.write("<td ><span class='table_font2'><% get3GConnConnUpTime(); %></span></td></tr>");
			}
		
////  3G Card Manufacturer
		document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_info3GManufacturer'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% get3GManufacturer(); %></span></td></tr>");

////  3G Card Model
		document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_info3GModel'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% get3GModel(); %></span></td></tr>");
		
////  3G Card Firmware
		document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_info3GFW'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% get3GFW(); %></span></td></tr>");
		
////  3G Card IMEI
		document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_info3GIMEI'></font>:</span></td>");
		document.write("<td ><span class='table_font2'><% get3GIMEI(); %></span></td></tr>");
		
////  SIM Card IMEI
//		document.write("<tr ><td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoSIMIMEI'></font>:</span></td>");
//		document.write("<td ><span class='table_font2'><% getSIMIMEI(); %></span></td></tr>");

		document.write('</table></td>');
		document.write('</tr>');
		document.write('</table></td>');
		document.write('</tr>');
		document.write('</table></td>');
		document.write('</tr>');
		document.write('</table>');
		document.write('</div>');
		document.write('<br />');
		document.write('<br />');
	}
}
</script>  		  
		  <div class="w_text3">
            <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="table_frame">
              <tr>
                <td><table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td><table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
                          <tr>
                            <td><table width="100%" cellpadding="0" cellspacing="0" >
                              <tr>
                                <td height="26"><table width="100%" border="0" cellspacing="0" cellpadding="0">
                                    <tr>
                                      <td height="26" background="images/table_top_center.gif" class="table_title"><font id="dash_infoSumTitle"></font></td>
                                    </tr>
                                </table></td>
                              </tr>
<script>                              
if ((nv_OP_Mode == 0)||(nv_OP_Mode == 6)||(nv_OP_Mode == 7)){
	//document.write("<tr ><td width='60%' valign='top'><span class='table_font'><font id='dash_infoSumBw'></font> <a target='_parent' href='/local/advance/main_monitor_frame.asp?1'>(<font id='dash_infoSumBwDet'>Details</font>...)</a></span></td></tr>");
	document.write("<tr ><td valign='top'><span class='table_font'><font id='dash_infoSumDhcp'>DHCP Table</font> <a target='_parent' href='/local/advance/main_monitor_frame.asp?1'>(<font id='dash_infoSumDhcpDet'>Details</font>...)</a></span></td></tr>");
}
</script>                            
<tr >
<td valign="top"><span class="table_font"><font id="dash_infoSumPktstat">Packet Statistics</font> <a target='_parent' href="/local/advance/main_monitor_frame.asp?2">(<font id="dash_infoSumPktstatDet">Details</font>...)</a></span></td>
</tr>
<!--<script>                              
if ((nv_OP_Mode == 0)||(nv_OP_Mode == 1)||(nv_OP_Mode == 5)){
	document.write("<tr ><td valign='top'><span class='table_font'><font id='dash_infoSumWlansta'>WLAN Station Status</font> <a target='_parent' href='/local/advance/main_monitor_frame.asp?4'>(<font id='dash_infoSumWlanstaDet'>Details</font>...)</a></span></td></tr>");
}
</script>-->                              
                            </table></td>
                          </tr>
                      </table></td>
                    </tr>
                </table></td>
              </tr>
            </table>
		    </div>
		  </div>
		</div>
            <br class="clearfloat" />
            <br />
          <br />
        </td>
        <td width="1" bgcolor="#779bc8"><img src="../images/air.gif" width="1" height="38" /></td>
      </tr>
    </table>
  </div>
</div>
</form>
</body>
<script>
 start_Mon="<% getCfgGeneral(1, "startMon"); %>";
 start_Day="<% getCfgGeneral(1, "startDay"); %>";
 start_Hour="<% getCfgGeneral(1, "startclock"); %>";
 end_Mon="<% getCfgGeneral(1, "endMon"); %>";
 end_Day="<% getCfgGeneral(1, "endDay"); %>";
 end_Hour="<% getCfgGeneral(1, "endclock"); %>";
 enable_dst = "<% getCfgGeneral(1, "enabledaylight"); %>";

	if(enable_dst=="1")
	{
	daylightsaving();
	}
initDateAndTime();
IncreaseSec();
</script>
<script>
parent.change_iframe_height(document.body.scrollHeight);
</script>
</html>
