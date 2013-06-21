<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html;charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<style>
td {white-space: nowrap;}
</style>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("firewall");

var sys_port_list="<% get_sys_sock_port_list(); %>";

function find_www_port(port){
	var port_array = new Array();
	
	port_array = sys_port_list.split(":");
	
	for (i = 0; i < port_array.length; i ++) {
			if(port_array[i] == port)
					return true;	
	}
	
	return false;
}

function initTranslation(){

	<!-- var e = document.getElementById("RemoteManagementServerPort"); -->
	<!-- e.innerHTML = _("remote management server port"); -->
	<!-- e = document.getElementById("RemoteManagementServerAccess"); -->
	<!-- e.innerHTML = _("remote management server access"); -->
	<!-- e = document.getElementById("RemoteManagementSecuredIP"); -->
	<!-- e.innerHTML = _("remote management secured ip"); -->
	<!-- e = document.getElementById("RemoteManagementSecuredIPAll"); -->
	<!-- e.innerHTML = _("remote management secured ip all"); -->
	<!-- e = document.getElementById("RemoteManagementSecuredIPSel"); -->
	<!-- e.innerHTML = _("remote management secured ip sel"); -->
	<!-- e = document.getElementById("natApply"); -->
	<!-- e.value = _("nat apply"); -->
	<!-- e = document.getElementById("natReset"); -->
	<!-- e.value = _("nat reset"); -->
	
	<!-- e = document.getElementById("remoteManagementWWW"); -->
	<!-- e.innerHTML = _("remote management www"); -->
}
function atoi(str, num){
	i = 1;
	if (num != 1) {
		while (i != num && str.length != 0) {
			if (str.charAt(0) == '.') {
				i++;
			}
			str = str.substring(1);
		}
		if (i != num)
			return -1;
	}
	for (i=0; i<str.length; i++) {
		if (str.charAt(i) == '.') {
			str = str.substring(0, i);
			break;
		}
	}
	if (str.length == 0)
		return -1;
	return parseInt(str, 10);
} 
function checkRange(str, num, min, max){
       var k = 0;
	 for (var i=0; i<str.length; i++) {
		if (str.charAt(i) == '.')
			k = k+1;
		continue;
	}
	if(k > 3){
	      alert("Error. IP address is not valid.");
             return false;
	}
	d = atoi(str, num);
	if (d > max || d < min)
		return false;
	return true;
}
function isAllNum(str){
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9') || (str.charAt(i) == '.' ))
			continue;
		return 0;
	}
	return 1;
}

//0<port<=0xffff
function isvalidport(str){
	var j=0;
	for (var i=0; i<str.length; i++) {
		if (str.charAt(i) >= '0' && str.charAt(i) <= '9')
		{
			if(str.charAt(i) == '0')
				j++;
			continue;
		}
		return 0;
	}
	if(j==str.length)
		return 0;
	if(parseInt(str) > 0xffff)
		return 0;
	return 1;
}
function checkIpAddr(field, ismask){
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
function service_check_len(obj, min, max) {
if((obj.value.length>=min) && (obj.value.length<=max))
return true;
else {
obj.focus();
obj.select();
alert("Valid string length is ["+min+","+max+"]");
return false;
}
}
function myformCheck(){
var form = document.service;
if(form.snmp_enable[0].checked)
{
	document.service.snmp_version_1.value= document.service.snmp_version_1.checked ? "1" : "0";
	document.service.snmp_version_2c.value = document.service.snmp_version_2c.checked ? "1": "0";
	document.service.snmp_version_usm.value=document.service.snmp_version_usm.checked ?"1":"0";
	if(form.snmp_enable[0].checked &&
	!form.snmp_version_1.checked &&
	!form.snmp_version_2c.checked &&
	!form.snmp_version_usm.checked) {
	alert("You must choose one SNMP version");
	return false;
	}
	if(!service_check_len(form.snmp_contact,0,32))
		return false;
	if(form.snmp_version_1.checked || 
	form.snmp_version_2c.checked) { 
	if(!service_check_len(form.snmp_comm_ro,1,32))
		return false;
	if(!service_check_len(form.snmp_comm_rw,1,32))
		return false;
	if(!checkIpAddr(form.snmp_trap_ip,0))
		return false;
	}
	if(form.snmp_version_usm.checked) {
	if(!service_check_len(form.snmp_user_ro,1,32))
		return false;
	if(!service_check_len(form.snmp_user_ro_passwd,8,32))
		return false;
	if(!service_check_len(form.snmp_user_rw,1,32))
		return false;
	if(!service_check_len(form.snmp_user_rw_passwd,8,32))
		return false;
	}
}
if(form.ssh_enable[0].checked)
{
	if(!isvalidport(form.ssh_port.value))
	{
		alert("invalid ssh port number!");
		return false;
	}
	if(find_www_port(form.ssh_port.value))
  	{
  		alert("SSh port already in use");
  		return false;	
  	}
}
if(form.telnet_enable[0].checked)
{
	if(!isvalidport(form.telnet_port.value))
	{
		alert("invalid telnet port number!");
		return false;
	}
	if(find_www_port(form.telnet_port.value))
	{
  		alert("telnet port already in use");
  		return false;	
  	}
}
if(form.https_enable[0].checked)
{
	if(!isvalidport(form.https_port.value))
	{
		alert("invalid https port number!");
		return false;
	}
	if(find_www_port(form.https_port.value))
  	{
  		alert("https port already in use");
  		return false;	
  	}
}
showWebMessage(2, "");
return true;
}

function updateState(){
initTranslation();
var enable = "<% getCfgGeneral(1, "SNMP_enable"); %>";
var snmp_version_v1 = "<% getCfgGeneral(1,"SNMP_version_v1");%>";
var snmp_version_v2c = "<% getCfgGeneral(1,"SNMP_version_v2c");%>";
var snmp_version_usm = "<% getCfgGeneral(1,"SNMP_version_usm");%>";
var sshenable = "<% getCfgGeneral(1, "ssh_enable"); %>";
var telnetenable = "<% getCfgGeneral(1, "telnet_enable"); %>";
var httpsenable = "<% getCfgGeneral(1, "https_enable"); %>";
if(enable == 1) {
document.service.snmp_enable[0].checked = true;
}else
document.service.snmp_enable[1].checked = true;
if(snmp_version_v1 == 1)
document.service.snmp_version_1.checked = true;
if(snmp_version_v2c == 1)
document.service.snmp_version_2c.checked = true;
if(snmp_version_usm == 1)
document.service.snmp_version_usm.checked = true;
if(sshenable == 1) {
document.service.ssh_enable[0].checked = true;
}else
document.service.ssh_enable[1].checked = true;
if(telnetenable == 1) {
document.service.telnet_enable[0].checked = true;
}else
document.service.telnet_enable[1].checked = true;
if(httpsenable == 1) {
document.service.https_enable[0].checked = true;
}else
document.service.https_enable[1].checked = true;
radioChange();
sshchange();
telnetchange();
httpschange();
return ;
}

function radioChange(){
var form=document.service;
if(form.snmp_enable[0].checked) {
form.snmp_version_1.disabled=false;
form.snmp_version_2c.disabled=false;
form.snmp_version_usm.disabled=false;
form.snmp_contact.disabled=false;
form.snmp_comm_ro.disabled=false;
form.snmp_comm_rw.disabled=false;
form.snmp_user_ro.disabled=false;
form.snmp_user_ro_passwd.disabled=false;
form.snmp_user_rw.disabled=false;
form.snmp_user_rw_passwd.disabled=false;
form.snmp_trap_ip.disabled=false;
VersionCheck();
}else {
form.snmp_version_1.disabled=true;
form.snmp_version_2c.disabled=true;
form.snmp_version_usm.disabled=true;
form.snmp_contact.disabled=true;
form.snmp_comm_ro.disabled=true;
form.snmp_comm_rw.disabled=true;
form.snmp_user_ro.disabled=true;
form.snmp_user_ro_passwd.disabled=true;
form.snmp_user_rw.disabled=true;
form.snmp_user_rw_passwd.disabled=true;
form.snmp_trap_ip.disabled=true;
}
return true;
}

function sshchange(){
	if(document.service.ssh_enable[0].checked)
		document.service.ssh_port.disabled=false;
	else
		document.service.ssh_port.disabled=true;
}

function telnetchange(){
	if(document.service.telnet_enable[0].checked)
		document.service.telnet_port.disabled=false;
	else
		document.service.telnet_port.disabled=true;
}

function httpschange(){
	if(document.service.https_enable[0].checked)
		document.service.https_port.disabled=false;
	else
		document.service.https_port.disabled=true;
}
function VersionCheck()
{
var form=document.service;
if(form.snmp_version_usm.checked == true) {
form.snmp_user_ro.disabled=false;
form.snmp_user_ro_passwd.disabled=false;
form.snmp_user_rw.disabled=false;
form.snmp_user_rw_passwd.disabled=false;

}else {
form.snmp_user_ro.disabled=true;
form.snmp_user_ro_passwd.disabled=true;
form.snmp_user_rw.disabled=true;
form.snmp_user_rw_passwd.disabled=true;
}
if(!form.snmp_version_1.checked && !form.snmp_version_2c.checked ) {
form.snmp_comm_ro.disabled=true;
form.snmp_comm_rw.disabled=true;
form.snmp_trap_ip.disabled=true;
}else {
form.snmp_comm_ro.disabled=false;
form.snmp_comm_rw.disabled=false;
form.snmp_trap_ip.disabled=false;
}
}
</script>
</head>
<body onload="updateState()">
<form method="post" name="service" action="/goform/service" onSubmit="return myformCheck();">
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul>
<li class="title" id="remoteManagementWWW">SNMP</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id="RemoteManagementServerPort">SNMP : </td>
<td><input name="snmp_enable" onclick="radioChange()" value="1" type="radio" />&nbsp;&nbsp;&nbsp;Enable</td>

<td><input name="snmp_enable" onclick="radioChange()" value="0" type="radio" />&nbsp;&nbsp;&nbsp;Disable</td>

<td width="43%"></td>
</tr>
</table>
</li>
<li style="visibility:hidden;display:none" class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id="RemoteManagementServerPort">SNMP Version Support : </td>
<td><input name="snmp_version_1" value="1" type="checkbox" onClick="VersionCheck()" />&nbsp;&nbsp;&nbsp;SNMP v1 </td>
<td id="SNMPEnableV1"></td>
<td><input name="snmp_version_2c" value="1" type="checkbox" onClick="VersionCheck()" />&nbsp;&nbsp;&nbsp;SNMP v2 </td>
<td id="SNMPEnableV2c"></td>
<td ><input name="snmp_version_usm" value="1" type="checkbox" onClick="VersionCheck()" />&nbsp;&nbsp;&nbsp;SNMP v3</td>
<td id="SNMPEnableUsm"></td>
<td width="35%"></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id=""> Contact: </td>
<td><input name="snmp_contact" size="32" maxlength="32" value="<% getCfgGeneral(1, "SNMP_contact");%>" type="text" /></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id=""> Community Name(read only): </td>
<td><input name="snmp_comm_ro" size="32" maxlength="32" value="<% getCfgGeneral(1, "SNMP_comm_ro");%>" type="text" /></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id=""> Community Name(read/write): </td>
<td><input name="snmp_comm_rw" size="32" maxlength="32" value="<% getCfgGeneral(1, "SNMP_comm_rw");%>" type="text" /></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id=""> Trap IpAddress: </td>
<td><input name="snmp_trap_ip" size="32" maxlength="32" value="<% getCfgGeneral(1, "SNMP_trap_ip");%>" type="text" /></td>
</tr>
</table>
</li>
<li style="visibility:hidden;display:none" class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id=""> User Name(read only): </td>
<td><input name="snmp_user_ro" size="32" maxlength="32" value="<% getCfgGeneral(1, "SNMP_user_ro");%>" type="text" /></td>
</tr>
</table>
</li>
<li style="visibility:hidden;display:none" class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id=""> Password: </td>
<td><input name="snmp_user_ro_passwd" size="32" maxlength="32" value="<% getCfgGeneral(1, "SNMP_user_ro_passwd");%>" type="password" /></td>
</tr>
</table>
</li>
<li style="visibility:hidden;display:none" class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id=""> User Name(read/write): </td>
<td><input name="snmp_user_rw" size="32" maxlength="32" value="<% getCfgGeneral(1, "SNMP_user_rw");%>" type="text" /></td>
</tr>
</table>
</li>
<li style="visibility:hidden;display:none" class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id=""> Password: </td>
<td><input name="snmp_user_rw_passwd" size="32" maxlength="32" value="<% getCfgGeneral(1, "SNMP_user_rw_passwd");%>" type="password" /></td>
</tr>
</table>
</li>

</ul>
</div>
</li>

<!--SSH start-->
<li class="space3"></li>
<li class="table_content">
<div class="data">
<ul>
<li class="title" id="ManagementSSH">SSH</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id="ManagementSSHFunc">SSH : </td>
<td><input name="ssh_enable" onclick="sshchange()" value="1" type="radio" />&nbsp;&nbsp;&nbsp;Enable</td>

<td><input name="ssh_enable" onclick="sshchange()" value="0" type="radio" />&nbsp;&nbsp;&nbsp;Disable</td>

<td width="43%"></td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id=""> Server Port: </td>
<td><input name="ssh_port" size="5" maxlength="5" value="<% getCfgGeneral(1, "ssh_port");%>" type="text" /></td>
</tr>
</table>
</li>

</ul>
</div>
</li>
<!--SSH end-->

<!--TELNET start-->
<li class="space3"></li>
<li class="table_content">
<div class="data">
<ul>
<li class="title" id="ManagementTELNET">Telnet</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id="ManagementTelnetFunc">Telnet : </td>
<td><input name="telnet_enable" onclick="telnetchange()" value="1" type="radio" />&nbsp;&nbsp;&nbsp;Enable</td>

<td><input name="telnet_enable" onclick="telnetchange()" value="0" type="radio" />&nbsp;&nbsp;&nbsp;Disable</td>

<td width="43%"></td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id=""> Server Port: </td>
<td><input name="telnet_port" size="5" maxlength="5" value="<% getCfgGeneral(1, "telnet_port");%>" type="text" /></td>
</tr>
</table>
</li>

</ul>
</div>
</li>
<!--TELNET end-->

<!--https start-->
<li class="space3"></li>
<li class="table_content">
<div class="data">
<ul>
<li class="title" id="Managementhttps">HTTPS</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id="ManagementhttpsFunc">Https : </td>
<td><input name="https_enable" onclick="httpschange()" value="1" type="radio" />&nbsp;&nbsp;&nbsp;Enable</td>

<td><input name="https_enable" onclick="httpschange()" value="0" type="radio" />&nbsp;&nbsp;&nbsp;Disable</td>

<td width="43%"></td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id=""> Port: </td>
<td><input name="https_port" size="5" maxlength="5" value="<% getCfgGeneral(1, "https_port");%>" type="text" /></td>
</tr>
</table>
</li>

<li></li>
</ul>
</div>
</li>
<!--https end-->

<center>
<li class="table_button">
<input type="submit" value="Apply" id="natApply" name="add" onClick="return myformCheck()"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input type="reset" value="Cancel" id="natReset" name="reset" onClick="window.location.reload()">
<input type=hidden value="/local/advance/service.asp" name="service_url">
</li>
</center>
</ul>
</div>
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
