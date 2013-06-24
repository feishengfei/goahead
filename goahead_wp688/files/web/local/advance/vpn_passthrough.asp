<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("firewall"); 

function formCheck()
{
	showWebMessage(2, "");
	return true;
}

</script>
</head>
<body onload="">
<form method="post" name="vpn_passthrough" action="/goform/vpn_passthrough" >
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul><li class="title" id="firewallSetup" style="white-space:nowrap" >VPN Passthrough Setup</li>

				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
						<tr>
						<td width="10%" id="" style="white-space:nowrap" ><% lang("IPsec Passthrough"); %> </td>
						<td width="3%" >
						<input type="radio" value="0" name="ipsec_enable" onclick="init()"
                        <%nvg_attr_match("ipsec_enable", "smb_vpn_passthrough_rule","0","ipsec_enable","0", "checked");%>>
						</input>
						  </td>
						<td width="5%"><%lang("Enable");%>
						<td width="3%" ><input type="radio" value="1" name="ipsec_enable" onclick="init()"
						<%nvg_attr_match("ipsec_enable", "smb_vpn_passthrough_rule","0","ipsec_enable","1", "checked");%>>
						</input>
						</td>
						<td width="5%"><%lang("Disable");%>
						  <td width="77%"></td>
						</tr>
					</table>
				</li>

				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
						<tr>
						<td width="10%" id="" style="white-space:nowrap" ><% lang("PPTP Passthrough"); %> </td>
						<td width="3%" >
						<input type="radio" value="0" name="pptp_enable" onclick="init()"
                        <%nvg_attr_match("pptp_enable", "smb_vpn_passthrough_rule","0","pptp_enable","0", "checked");%>>
						</input>
						  </td>
						<td width="5%"><%lang("Enable");%>
						<td width="3%" ><input type="radio" value="1" name="pptp_enable" onclick="init()"
						<%nvg_attr_match("pptp_enable", "smb_vpn_passthrough_rule","0","pptp_enable","1", "checked");%>>
						</input>
						</td>
						<td width="5%"><%lang("Disable");%>
						  <td width="77%"></td>
						</tr>
					</table>
				</li>

				<li class="w_text">
					<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
						<tr>
						<td width="10%" id="" style="white-space:nowrap" ><% lang("L2TP Passthrough"); %> </td>
						<td width="3%" >
						<input type="radio" value="0" name="l2tp_enable" onclick="init()"
                        <%nvg_attr_match("l2tp_enable", "smb_vpn_passthrough_rule","0","l2tp_enable","0", "checked");%>>
						</input>
						  </td>
						<td width="5%"><%lang("Enable");%>
						<td width="3%" ><input type="radio" value="1" name="l2tp_enable" onclick="init()"
						<%nvg_attr_match("l2tp_enable", "smb_vpn_passthrough_rule","0","l2tp_enable","1", "checked");%>>
						</input>
						</td>
						<td width="5%"><%lang("Disable");%>
						  <td width="77%"></td>
						</tr>
					</table>
				</li>

<li></li>
<li></li>
</ul>
</div>
</li>
<center>
<li class="table_button">
<input type="submit" value="Apply" id="Apply" name="" onClick="return formCheck()"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input type="reset" value="Reset" id="Reset" name="reset" onClick="window.location.reload()">
<input type=hidden value="/local/advance/vpn_passthrough.asp" name="vpn_pass_url">
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
