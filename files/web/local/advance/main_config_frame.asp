<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html;charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>
<link href="images/inside_main_frame.css" rel="stylesheet" type="text/css" />
<link href="images/dtree.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="/lang/b28n.js"></script>
<script>
Butterlate.setTextDomain("main");
var prev_menu_id=2;
var prev_submenu_index=1;
var is_wps_init_page=1;

var site_survey_ssid = "";
var site_survey_security_infra_mode = 0;
var site_survey_security_adhoc_mode = 0;
var site_survey_cipher = 0;
var site_survey_network_type = 0;
var site_survey_channel = 0;
var site_survey_setting = 0;
var opmode  = "<% getCfgZero(1, "OP_Mode"); %>";
var wisp_mode = "<% getCfgZero(1, "wisp_mode"); %>";
var modelname = "<% getCfgGeneral(1, "SystemName"); %>";
var bridge_mode = "<% getCfgZero(1, "brg_mode"); %>";
var board_type = "<% getCfgZero(1, "board_model"); %>";
</script>
<script type="text/javascript" src="images/dtree.js"></script>
<script type="text/JavaScript">

function show_radar()
{
		var nv_radio_off = "<% getRadioStatusASP5G(); %>"; 
		var radar_status = "<% getwifi5GRadarStatus(); %>";
       if (nv_radio_off == "ON")
       {
                 show_div(false,"menu_33_1");
                 show_div(false,"menu_33_1_radar");
                 show_div(false,"menu_26_1");
                 show_div(false,"menu_26_1_radar");

     	   if((radar_status == 2) || (radar_status == 1))
     	   {
                 if(opmode == 5)
                 {
                 show_div(false,"menu_33_1");
                 show_div(true,"menu_33_1_radar");
                 show_div(false,"menu_26_1");
                 show_div(false,"menu_26_1_radar");
                 }else
                 {
                 show_div(false,"menu_26_1");
                 show_div(true,"menu_26_1_radar");
                 show_div(false,"menu_33_1");
                 show_div(false,"menu_33_1_radar");
                 }
          }else{
                 if(opmode == 5){
                 show_div(true,"menu_33_1");
                 show_div(false,"menu_33_1_radar");
                 show_div(false,"menu_26_1");
                 show_div(false,"menu_26_1_radar");
                 }else{
                 show_div(true,"menu_26_1");
                 show_div(false,"menu_26_1_radar");
                 show_div(false,"menu_33_1");
                 show_div(false,"menu_33_1_radar");
                 }
     	   }
       }
       else
       {
                if(opmode == 5){
                show_div(true,"menu_33_1");
                show_div(false,"menu_33_1_radar");
                show_div(false,"menu_26_1");
                show_div(false,"menu_26_1_radar");
                }else{
                show_div(true,"menu_26_1");
                show_div(false,"menu_26_1_radar");
                show_div(false,"menu_33_1");
                show_div(false,"menu_33_1_radar");
                }
       }
 }
 
function initTranslation()
{
	var e = document.getElementById("id_Home");
	e.innerHTML = _("Home");
	e = document.getElementById("id_Logout");
	e.innerHTML = _("Logout");
	e = document.getElementById("id_About");
	e.innerHTML = _("About");
	//e = document.getElementById("id_EasyMode");
	//e.innerHTML = _("EasyMode");
	e = document.getElementById("id_WelcomeAdmin");
	e.innerHTML = _("WelcomeAdmin");
	e = document.getElementById("id_openall");
	e.innerHTML = _("openall");
	e = document.getElementById("id_closeall");
	e.innerHTML = _("closeall");

			e = document.getElementById("g2-2No");
	e.innerHTML = _("g2-2 No");
	e = document.getElementById("g2-2Yes");
	e.innerHTML = _("g2-2 Yes");
		e = document.getElementById("ask_logout");
	e.innerHTML = _("dash logout mes");
		e = document.getElementById("log_out");
	e.innerHTML = _("dash logout title");
		e = document.getElementById("message_header");
	e.innerHTML = _("message_header");

	e = document.getElementById("dashAboutCheck1");
	e.innerHTML = _("dash about check1");
	e = document.getElementById("dashAboutCheck2");
	e.innerHTML = _("dash about check2");
	e = document.getElementById("dashAboutClose");
	e.innerHTML = _("dash about close");
	e = document.getElementById("modelname");
	e.innerHTML = modelname;
}

<!--
function MM_findObj(n, d) { //v4.01
  var p,i,x;  if(!d) d=document; if((p=n.indexOf("?"))>0&&parent.frames.length) {
    d=parent.frames[n.substring(p+1)].document; n=n.substring(0,p);}
  if(!(x=d[n])&&d.all) x=d.all[n]; for (i=0;!x&&i<d.forms.length;i++) x=d.forms[i][n];
  for(i=0;!x&&d.layers&&i<d.layers.length;i++) x=MM_findObj(n,d.layers[i].document);
  if(!x && d.getElementById) x=d.getElementById(n); return x;
}

function MM_showHideLayers() { //v6.0
  var i,p,v,obj,args=MM_showHideLayers.arguments;
  for (i=0; i<(args.length-2); i+=3) if ((obj=MM_findObj(args[i]))!=null) { v=args[i+2];
    if (obj.style) { obj=obj.style; v=(v=='show')?'visible':(v=='hide')?'hidden':v; }
    obj.visibility=v; }
}
//-->

</script>
<style>
.on {display:on}
.off {display:none}
</style>
</head>
<body onresize="onResizeWindow()"; scroll="no" onLoad="initTranslation()">
<div id="wrapper">
  <div id="top"><div class="top_position"><div class="logo"></div>
  <div class="modelname" id="modelname"><% getCfgGeneral(1, "SystemName"); %></div>
  <div class="quick_link">
    <ul>
	<!--
      <li class="easy" title="Easy Mode"><a href="../networkmap.asp" id=id_EasyMode></a></li>
	-->
      <li class="about" onclick="MM_showHideLayers('popup','','show','popup4_table','','show')"><a href="#" title="About" id=id_About></a></li>
        <li class="home"><a href="../index_login.asp" title="Home" target="_parent" id=id_Home></a></li>
      <li onclick="MM_showHideLayers('popup3_table','','show','popup','','show')"><a href="#" title="Logout" id=id_Logout></a></li>
      <li class="line"></li>
      <li><font id=id_WelcomeAdmin></font></li>
    </ul>
  </div>
  </div></div>
  <div id="blueline"></div>
</div>
<div id="contentframe">
<div class="contentframe"><ul>
<li><div class="submenu"><ul> <li class="i_dashboard" title="Dashboard"><a href="dashboard.asp" title="Dashboard"></a></li> 
<li class="i_monitor"><a href="main_monitor_frame.asp" title="Monitor"></a></li><li class="i_config" title="Configuration"></li> <li class="i_maintenance"><a href="main_maintenance_frame.asp" title="Maintenance"></a></li></ul>  <br class="clearfloat" /></div></li>
<li><div class="menu"> <br class="clearfloat" />
  <ul>
<li class="menuline" id=menuline_height><div><ul><li class="space4"><script>document.write(_("CONFIGURATION_TITLE"))</script></li><li class="space"><p><a href="javascript: d.openAll();" class="fastbutton" id=id_openall></a><a>&nbsp; | &nbsp;</a><a href="javascript: d.closeAll();" id=id_closeall></a></p></li><li>
    <div class="dtree">
      <script type="text/javascript">
		
		d = new dTree('d');

		d.add(0,-1,'');
		d.add(1,0,_('Network'),'');
		
		if (board_type == "0A52")
		{
			var nv_radio_off = "<% getRadioStatusASP5G(); %>"; 
			var radar_status = "<% getwifi5GRadarStatus(); %>";
		}
		
		if ((opmode==0) || (opmode==1)){ // AP and Router Mode
			if (board_type == "0A22")
			{
				d.add(44,1,_('Wireless LAN 2.4G-1'),'/local/advance/wlan.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G-1')+' > '+_('DownLink'), 'InfoFrame');
			}
			else
			{
				d.add(20,1,_('Wireless LAN 2.4G'),'/local/advance/wlan.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('DownLink'), 'InfoFrame');
			}
			if (board_type == "0A22" || board_type == "0A52")
			{
				if (board_type == "0A52")
				{
			      if(nv_radio_off == "ON"){
			         if((radar_status == 2) || (radar_status == 1))
			         {
			           d.add(26,1,_('Wireless LAN 5G'),'/local/advance/radar.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('DownLink'), 'InfoFrame');
			         }else{
			           d.add(26,1,_('Wireless LAN 5G'),'/local/advance/wlan_5g.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('DownLink'), 'InfoFrame');
			         }
			       }else{
			           d.add(26,1,_('Wireless LAN 5G'),'/local/advance/wlan_5g.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('DownLink'), 'InfoFrame');
			    	}
				    
		 		}
		 		else
			    {
			    	d.add(45,1,_('Wireless LAN 2.4G-2'),'/local/advance/wlan.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G-2')+' > '+_('DownLink'), 'InfoFrame');
			    }
			}
			if (opmode==0) //Router Mode
			{
			   //d.add(36,1,_('IPv6'),'/local/advance/ipv6.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('IPv6')+' > '+_('General'), 'InfoFrame');
            }
        }
		else if (opmode==2){ // Client
			d.add(21,1,_('Wireless LAN 2.4G'),'/local/advance/wisp.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('General'), 'InfoFrame');
		}
		else if (opmode==6){ // WISP Mode
	        if(board_type == "0A52"){
	        	if(wisp_mode == 0)
	        	{
	        		d.add(21,1,_('Wireless LAN'),'/local/advance/apcli_wisp.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('UpLink'), 'InfoFrame');
	        	}
	        	else
	        	{
					d.add(27,1,_('Wireless LAN'),'/local/advance/apcli_wisp_5g.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('UpLink'), 'InfoFrame');
				}
	        }else if(board_type == "0A22"){
				//d.add(20,1,_('Wireless LAN'),'/local/advance/wlan.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('General'), 'InfoFrame');
				d.add(17,1,_('Wireless LAN'),'/local/advance/apcli_wisp.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('UpLink'), 'InfoFrame');
	        }
	        else
	        {
	        	d.add(18,1,_('Wireless LAN'),'/local/advance/apcli_wisp.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('UpLink'), 'InfoFrame');
	        }
		   // d.add(37,1,_('IPv6'),'/local/advance/ipv6.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('IPv6')+' > '+_('General'), 'InfoFrame');
		}		
		else if (opmode==3){ // WDS
			d.add(22,1,_('Wireless LAN 2.4G'),'/local/advance/wlan.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('General'), 'InfoFrame');
			d.add(28,1,_('Wireless LAN 5G'),'/local/advance/wlan_5g.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('General'), 'InfoFrame');
		}
		else if (opmode==4){ // AP + WDS
			d.add(23,1,_('Wireless LAN 2.4G'),'/local/advance/wlan.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('General'), 'InfoFrame');
			d.add(29,1,_('Wireless LAN 5G'),'/local/advance/wlan_5g.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('General'), 'InfoFrame');
		}
		else if (opmode==5){ // AP + Client
			if (bridge_mode == 0)
			{
				if (board_type == "0A52")
				{
					d.add(55,1,_('Wireless LAN'),'/local/advance/ur_5g.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('UpLink'), 'InfoFrame');
				}
				else if (board_type == "0A22")
				{
					d.add(54,1,_('Wireless LAN'),'/local/advance/ur.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('UpLink'), 'InfoFrame');
				}
				else
				{
					d.add(24,1,_('Wireless LAN'),'/local/advance/ur.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('UpLink'), 'InfoFrame');
				}
			}
			else
			{
				if (board_type == "0A52")
				{
					d.add(56,1,_('Wireless LAN'),'/local/advance/ur.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('UpLink'), 'InfoFrame');
				}
				else
				{
					d.add(54,1,_('Wireless LAN'),'/local/advance/ur.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('UpLink'), 'InfoFrame');
				}
			}
			/*5G
			if (board_type != 0 && bridge_mode != 0)
			{
			    if (nv_radio_off == "ON"){
				 if((radar_status == 2) || (radar_status == 1))
				 {
				   	d.add(33,1,_('Wireless LAN 5G'),'/local/advance/radar.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('General'), 'InfoFrame');
				 }else{
				   	d.add(33,1,_('Wireless LAN 5G'),'/local/advance/wlan_5g.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('General'), 'InfoFrame');
				 }
			   }else{
				   d.add(33,1,_('Wireless LAN 5G'),'/local/advance/wlan_5g.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('General'), 'InfoFrame');
			   }
		   }*/
		}
		else if (opmode==7){ // WISP + UR
			d.add(25,1,_('Wireless LAN 2.4G'),'/local/advance/wlan.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('General'), 'InfoFrame');
			d.add(34,1,_('Wireless LAN 5G'),'/local/advance/wlan_5g.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('General'), 'InfoFrame');
			d.add(38,1,_('IPv6'),'/local/advance/ipv6.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('IPv6')+' > '+_('General'), 'InfoFrame');
		}
		
		if (opmode ==0 ){ // Router mode
		d.add(30,1,_('WAN'),'/local/advance/wan.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('WAN')+' > '+_('Internet Connection'), 'InfoFrame');
		}
		else if (opmode ==6 ){ // Router mode
		d.add(31,1,_('WAN'),'/local/advance/wan.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('WAN')+' > '+_('Internet Connection'), 'InfoFrame');
		}
		else if (opmode ==7 ){ // Router mode
		d.add(32,1,_('WAN'),'/local/advance/wan.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('WAN')+' > '+_('Internet Connection'), 'InfoFrame');
		}
		d.add(3,1,_('LAN'),'/local/advance/lan.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('LAN')+' > '+_('IP'), 'InfoFrame');
		
		if ((opmode==0) || (opmode==6) || (opmode==7)){ // Router and WISP
		d.add(12,1,_('DHCP Server'),'/local/advance/lan_dhcp.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('DHCP Server')+' > '+_('General'), 'InfoFrame');
		d.add(5,1,_('NAT'),'/local/advance/nat.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('NAT')+' > '+_('General'), 'InfoFrame');
		//d.add(7,1,_('DDNS'),'/local/advance/ddns.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('DDNS')+' > '+_('General'), 'InfoFrame');
		d.add(6,1,_('Static Route'),'/local/advance/sr.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('Static Route')+' > '+_('IP Static Route'), 'InfoFrame');
		//d.add(11,1,_('RIP'),'/local/advance/rip.asp', _('CONFIGURATION')+' > '+_('Network')+' > '+_('Routing')+' > '+_('RIP'), 'InfoFrame');
		d.add(9,0,_('Security'),'');
		d.add(10,9,_('Firewall'),'firewall.asp', _('CONFIGURATION')+' > '+_('Security')+' > '+_('Firewall')+' > '+_('SPI Firewall'), 'InfoFrame');
		d.add(15,9,_('Content Filter'),'SCFilter.asp', _('CONFIGURATION')+' > '+_('Security')+' > '+_('Content Filter'), 'InfoFrame');
		//d.add(16,9,_('OpenDNS'),'opendns.asp', _('CONFIGURATION')+' > '+_('Security')+' > '+_('OpenDNS'), 'InfoFrame');
		//william add for ipsec
		//d.add(41,9,_('IPsec'),'vpn_ipsec_general.asp', _('CONFIGURATION')+' > '+_('Security')+' > '+_('IPsec')+' > '+_('General'), 'InfoFrame');
		//d.add(42,9,_('PPTP Server'),'vpn_pptpd_general.asp', _('CONFIGURATION')+' > '+_('Security')+' > '+_('PPTP Server')+' > '+_('General'), 'InfoFrame');
		d.add(43,9,_('VPN Passthrough'),'vpn_passthrough.asp', _('CONFIGURATION')+' > '+_('Security')+' > '+_('VPN Passthrough')+' > '+_('General'), 'InfoFrame');

		d.add(13,0,_('Management'),'');
		//d.add(14,13,_('Bandwidth Management'),'bm.asp', _('CONFIGURATION')+' > '+_('Management')+' > '+_('Bandwidth Management')+' > '+_('General'), 'InfoFrame');
		d.add(35,13,_('Remote Management'),'remote_web.asp', _('CONFIGURATION')+' > '+_('Management')+' > '+_('Remote Management')+' > '+_('WWW'), 'InfoFrame');
		d.add(8,13,_('UPnP'),'/local/advance/upnp.asp', _('CONFIGURATION')+' > '+_('Management')+' > '+_('UPnP')+' > '+_('General'), 'InfoFrame');
		}
		else {
			d.add(13,0,_('Management'),'');
		}
		d.add(39,13,_('Service'),'service.asp', _('CONFIGURATION')+' > '+_('Management')+' > '+_('Service')+' > '+_('Service'), 'InfoFrame');
		d.add(40,13,_('System Utilities'),'ad_tools.asp', _('CONFIGURATION')+' > '+_('Management')+' > '+_('System Utilities')+' > '+_('System Utilities'), 'InfoFrame');
		document.write(d);
		if(board_type == "0A52")
		{
			show_radar();
		}

		//-->
	</script>
</div></li></ul> 
  </div></li> 
<!--   
  <li class="floor">  <div class="r4"></div>
      <div class="r3"></div>
      <div class="r2"></div>
      <div class="r1"></div>
	  </li>
-->	  
	  </ul> <br class="clearfloat" /></div>
</li></ul> <br class="clearfloat" /></div>
<div class="contenleft">
  <div class="path">
    <ul>
      <li class="i_path"><a class="w_path" id="pathTitle"></a></li>
    </ul>
    <br class="clearfloat" />
  </div>
<span id=message_show class='off'>  
  <div class="barcontent" id=message_position>
    <ul>
      <li class="i_message">
      <table width="100%" border="0" cellspacing="0" cellpadding="0"><tr><td nowrap>&nbsp;&nbsp;&nbsp;
      <a class="message_title" style="color:#30466d;" id="message_header">Message</a>:
      </td></tr></table>
      </li>
			<li class="message_word" id=message_len>
			<table width="100%" border="0" cellspacing="0" cellpadding="0"><tr><td nowrap>
			<a style="color:#3d8900;" id=message_str>Ready</a>
			</td></tr></table>
			</li>
    </ul>
    <br class="clearfloat" />
  </div>
</span>  
  <div class="contentmain">
    <div id="tabs">
      <ul>
<!-- WAN -->    
<!--- Router mode -->    
<span id = "menu_30_1_hightline" class = "off" >
        <li class="hightline"><a title="Internet Connection"><span><script>document.write(_("Internet Connection"))</script></span></a></li>
</span>    
<span id = "menu_30_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wan.asp" title="Internet Connection" onclick="onChangeHeadMenu(30, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('WAN')+' > '+_('Internet Connection'))"><span><script>document.write(_("Internet Connection"))</script></span></a></li>
</span>    
<span id = "menu_30_2_hightline" class = "off" >
        <li class="hightline"><a  title="Advanced"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>
<span id = "menu_30_2" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wan_advance.asp" title="Advanced" onclick="onChangeHeadMenu(30, 2, _('CONFIGURATION')+' > '+_('Network')+' > '+_('WAN')+' > '+_('Advanced'))"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>   
<!--<span id = "menu_30_3_hightline" class = "off" >
        <li class="hightline"><a  title="IGMP Snooping"><span><script>document.write(_("IGMP Snooping"))</script></span></a></li>
</span>
<span id = "menu_30_3" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/igmp_snooping.asp" title="IGMP Snooping" onclick="onChangeHeadMenu(30, 3, _('CONFIGURATION')+' > '+_('Network')+' > '+_('WAN')+' > '+_('IGMP Snooping'))"><span><script>document.write(_("IGMP Snooping"))</script></span></a></li>
</span>   -->    

<!--- WISP mode -->
<span id = "menu_31_1_hightline" class = "off" >
        <li class="hightline"><a title="Internet Connection"><span><script>document.write(_("Internet Connection"))</script></span></a></li>
</span>    
<span id = "menu_31_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wan.asp" title="Internet Connection" onclick="onChangeHeadMenu(31, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('WAN')+' > '+_('Internet Connection'))"><span><script>document.write(_("Internet Connection"))</script></span></a></li>
</span>
<!-- <span id = "menu_31_2_hightline" class = "off" >
        <li class="hightline"><a  title="Advanced"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>
<span id = "menu_31_2" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wan_advance.asp" title="Advanced" onclick="onChangeHeadMenu(31, 2, _('CONFIGURATION')+' > '+_('Network')+' > '+_('WAN')+' > '+_('Advanced'))"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span> -->
<!--- WISP +UR mode -->
<span id = "menu_32_1_hightline" class = "off" >
        <li class="hightline"><a title="Internet Connection"><span><script>document.write(_("Internet Connection"))</script></span></a></li>
</span>    
<span id = "menu_32_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wan.asp" title="Internet Connection" onclick="onChangeHeadMenu(32, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('WAN')+' > '+_('Internet Connection'))"><span><script>document.write(_("Internet Connection"))</script></span></a></li>
</span>
<span id = "menu_32_2_hightline" class = "off" >
        <li class="hightline"><a  title="Advanced"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>
<span id = "menu_32_2" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wan_advance.asp" title="Advanced" onclick="onChangeHeadMenu(32, 2, _('CONFIGURATION')+' > '+_('Network')+' > '+_('WAN')+' > '+_('Advanced'))"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>  
<!-- LAN -->    
<span id = "menu_3_1_hightline" class = "off" >
        <li class="hightline"><a title="IP"><span><script>document.write(_("IP"))</script></span></a></li>
</span>    
<span id = "menu_3_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/lan.asp" title="IP" onclick="onChangeHeadMenu(3, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('LAN')+' > '+_('IP'))"><span><script>document.write(_("IP"))</script></span></a></li>
</span>
<!--//hide ip_alias and spanning tree    
<span id = "menu_3_2_hightline" class = "off" >
        <li class="hightline"><a  title="IP Alias"><span><script>document.write(_("IP Alias"))</script></span></a></li>
</span>    
<span id = "menu_3_2" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/ip_alias.asp" title="IP Alias" onclick="onChangeHeadMenu(3, 2, _('CONFIGURATION')+' > '+_('Network')+' > '+_('LAN')+' > '+_('IP Alias'))"><span><script>document.write(_("IP Alias"))</script></span></a></li>
</span>
<span id = "menu_3_3_hightline" class = "off" >
        <li class="hightline"><a  title="Spanning Tree Protocol"><span><script>document.write(_("Spanning Tree Protocol"))</script></span></a></li>
</span>    
<span id = "menu_3_3" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/stp.asp" title="Spanning Tree Protocol" onclick="onChangeHeadMenu(3, 3, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Configure')+' > '+_('Spanning Tree Protocol'))"><span><script>document.write(_("Spanning Tree Protocol"))</script></span></a></li>
</span>
-->   
<!-- remove igmp snooping -->
<!--
<span id = "menu_3_4_hightline" class = "off" >
        <li class="hightline"><a  title="IGMP snooping"><span><script>document.write(_("IGMP snooping"))</script></span></a></li>
</span>
<span id = "menu_3_4" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/hw_snooping.asp" title="IGMP snooping" onclick="onChangeHeadMenu(3, 4, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Configure')+' > '+_('IGMP snooping'))"><span><script>document.write(_("IGMP snooping"))</script></span></a></li>
</span>
--> 
<!-- Wireless LAN -->    
<span id = "menu_4_1_hightline" class = "off" >
        <li class="hightline"><a title="General"><span><script>document.write(_("General"))</script></span></a></li>
</span>    
<span id = "menu_4_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan.asp" title="General" onclick="onChangeHeadMenu(4, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('General'))"><span><script>document.write(_("General"))</script></span></a></li>
</span>    

<span id = "menu_4_2_hightline" class = "off" >
        <li class="hightline"><a title="Security"><span><script>document.write(_("Security"))</script></span></a></li>
</span>    
<span id = "menu_4_2" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/security.asp" title="Security" onclick="onChangeHeadMenu(4, 2, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('Security'))"><span><script>document.write(_("Security"))</script></span></a></li>
</span>

<!--
<span id = "menu_4_3_hightline" class = "off" >
        <li class="hightline"><a title="MAC Filter"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>   
<span id = "menu_4_3" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/mf.asp" title="MAC Filter" onclick="onChangeHeadMenu(4, 3, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('MAC Filter'))"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>
-->
<span id = "menu_4_3_hightline" class = "off" >
        <li class="hightline"><a title="Advanced"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>    
<span id = "menu_4_3" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan_advance.asp" title="Advanced" onclick="onChangeHeadMenu(4, 3, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('Advanced'))"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span> 
<span id = "menu_4_4_hightline" class = "off" >
        <li class="hightline"><a title="QoS"><span><script>document.write(_("QoS"))</script></span></a></li>
</span>    
<span id = "menu_4_4" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/qos.asp" title="QoS" onclick="onChangeHeadMenu(4, 4, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('QoS'))"><span><script>document.write(_("QoS"))</script></span></a></li>
</span> 
<!--
<span id = "menu_4_6_hightline" class = "off" >
        <li class="hightline"><a title="WPS"><span><script>document.write(_("WPS"))</script></span></a></li>
</span>    
<span id = "menu_4_6" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wps.asp" title="WPS" onclick="onChangeHeadMenu(4, 6, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('WPS'))"><span><script>document.write(_("WPS"))</script></span></a></li>
</span> 
<span id = "menu_4_7_hightline" class = "off" >
        <li class="hightline"><a title="WPS Station"><span><script>document.write(_("WPS Station"))</script></span></a></li>
</span>    
<span id = "menu_4_7" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wps_station.asp" title="WPS Station" onclick="is_wps_init_page=1; onChangeHeadMenu(4, 7, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('WPS Station'))"><span><script>document.write(_("WPS Station"))</script></span></a></li>
</span> 
<span id = "menu_4_8_hightline" class = "off" >
        <li class="hightline"><a title="Scheduling"><span><script>document.write(_("Scheduling"))</script></span></a></li>
</span>    
<span id = "menu_4_8" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/scheduling.asp" title="Scheduling" onclick="onChangeHeadMenu(4, 8, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('Scheduling'))"><span><script>document.write(_("Scheduling"))</script></span></a></li>
</span>
-->

<span id = "menu_4_5_hightline" class = "off" >
        <li class="hightline"><a title="Universal Repeater"><span><script>document.write(_("Universal Repeater"))</script></span></a></li>
</span>    
<span id = "menu_4_5" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/apcli.asp" title="Universal Repeater" onclick="onChangeHeadMenu(4, 5, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('Universal Repeater'))"><span><script>document.write(_("Universal Repeater"))</script></span></a></li>
</span>

<!--
<span id = "menu_4_10_hightline" class = "off" >
        <li class="hightline"><a title="WDS"><span><script>document.write(_("WDS"))</script></span></a></li>
</span>    
<span id = "menu_4_10" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wds.asp" title="WDS" onclick="onChangeHeadMenu(4, 10, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('WDS'))"><span><script>document.write(_("WDS"))</script></span></a></li>
</span>
-->

<span id = "menu_4_6_hightline" class = "off" >
        <li class="hightline"><a title="Client"><span><script>document.write(_("Client"))</script></span></a></li>
</span>    
<span id = "menu_4_6" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wisp.asp" title="Client" onclick="onChangeHeadMenu(4, 6, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('Client'))"><span><script>document.write(_("Client"))</script></span></a></li>
</span>
<!-- NAT -->    
<span id = "menu_5_1_hightline" class = "off" >
        <li class="hightline"><a title="General"><span><script>document.write(_("General"))</script></span></a></li>
</span>    
<span id = "menu_5_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/nat.asp" title="General" onclick="onChangeHeadMenu(5, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('NAT')+' > '+_('General'))"><span><script>document.write(_("General"))</script></span></a></li>
</span>    
<span id = "menu_5_2_hightline" class = "off" >
        <li class="hightline"><a title="Application"><span><script>document.write(_("Application"))</script></span></a></li>
</span>    
<span id = "menu_5_2" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/nat_application.asp" title="Application" onclick="onChangeHeadMenu(5, 2, _('CONFIGURATION')+' > '+_('Network')+' > '+_('NAT')+' > '+_('Application'))"><span><script>document.write(_("Application"))</script></span></a></li>
</span>    
<span id = "menu_5_3_hightline" class = "off" >
        <li class="hightline"><a title="Advanced"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>    
<span id = "menu_5_3" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/nat_advance.asp" title="Advanced" onclick="onChangeHeadMenu(5, 3, _('CONFIGURATION')+' > '+_('Network')+' > '+_('NAT')+' > '+_('Advanced'))"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>    
<!-- NAT -->    
<span id = "menu_6_1_hightline" class = "off" >
        <li class="hightline"><a title="IP Static Route"><span><script>document.write(_("IP Static Route"))</script></span></a></li>
</span>    
<span id = "menu_6_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/sr.asp" title="IP Static Route" onclick="onChangeHeadMenu(6, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Static Route')+' > '+_('IP Static Route'))"><span><script>document.write(_("IP Static Route"))</script></span></a></li>
</span>    
<!-- DDNS -->    
<!--<span id = "menu_7_1_hightline" class = "off" >
        <li class="hightline"><a title="DDNS"><span><script>document.write(_("General"))</script></span></a></li>
</span>    
<span id = "menu_7_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/ddns.asp" title="General" onclick="onChangeHeadMenu(7, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('DDNS')+' > '+_('General'))"><span><script>document.write(_("General"))</script></span></a></li>
</span>-->    
<!-- UPnP -->    
<span id = "menu_8_1_hightline" class = "off" >
        <li class="hightline"><a title="UPnP"><span><script>document.write(_("General"))</script></span></a></li>
</span>    
<span id = "menu_8_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/upnp.asp" title="General" onclick="onChangeHeadMenu(8, 1, _('CONFIGURATION')+' > '+_('Management')+' > '+_('UPnP')+' > '+_('General'))"><span><script>document.write(_("General"))</script></span></a></li>
</span>    
<!-- Firewall -->    
<span id = "menu_10_1_hightline" class = "off" >
        <li class="hightline"><a title="SPI Firewall"><span><script>document.write(_("SPI Firewall"))</script></span></a></li>
</span>    
<span id = "menu_10_1" class = "off" >
        <li><a target="InfoFrame" href="firewall.asp" title="SPI Firewall" onclick="onChangeHeadMenu(10, 1, _('CONFIGURATION')+' > '+_('Security')+' > '+_('Firewall')+' > '+_('SPI Firewall'))"><span><script>document.write(_("SPI Firewall"))</script></span></a></li>
</span>    
<span id = "menu_10_2_hightline" class = "off" >
        <li class="hightline"><a title="Filter"><span><script>document.write(_("Filter"))</script></span></a></li>
</span>    
<span id = "menu_10_2" class = "off" >
        <li><a target="InfoFrame" href="fw_s2.asp" title="Filter" onclick="onChangeHeadMenu(10, 2, _('CONFIGURATION')+' > '+_('Security')+' > '+_('Firewall')+' > '+_('Filter'))"><span><script>document.write(_("Filter"))</script></span></a></li>
</span>    

<!-- IPsec -->    
<span id = "menu_41_1_hightline" class = "off" >
        <li class="hightline"><a title="General"><span><script>document.write(_("General"))</script></span></a></li>
</span>    
<span id = "menu_41_1" class = "off" >
        <li><a target="InfoFrame" href="vpn_ipsec_general.asp" title="General" onclick="onChangeHeadMenu(41, 1, _('CONFIGURATION')+' > '+_('Security')+' > '+_('IPsec')+' > '+_('General'))"><span><script>document.write(_("General"))</script></span></a></li>
</span>    

<span id = "menu_41_2_hightline" class = "off" >
        <li class="hightline"><a title="IPsec"><span><script>document.write(_("User Rule"))</script></span></a></li>
</span>    
<span id = "menu_41_2" class = "off" >
        <li><a target="InfoFrame" href="vpn_ipsec.asp" title="User Rule" onclick="onChangeHeadMenu(41, 2, _('CONFIGURATION')+' > '+_('Security')+' > '+_('IPsec')+' > '+_('User Rule'))"><span><script>document.write(_("User Rule"))</script></span></a></li>
</span>    

<!-- PPTP Server -->    
<span id = "menu_42_1_hightline" class = "off" >
        <li class="hightline"><a title="General"><span><script>document.write(_("General"))</script></span></a></li>
</span>    

<span id = "menu_42_1" class = "off" >
        <li><a target="InfoFrame" href="vpn_pptpd_general.asp" title="General" onclick="onChangeHeadMenu(42, 1, _('CONFIGURATION')+' > '+_('Security')+' > '+_('PPTP Server')+' > '+_('General'))"><span><script>document.write(_("General"))</script></span></a></li>
</span>    

<span id = "menu_42_2_hightline" class = "off" >
        <li class="hightline"><a title=""><span><script>document.write(_("User Rule"))</script></span></a></li>
</span>    

<span id = "menu_42_2" class = "off" >
        <li><a target="InfoFrame" href="vpn_pptpd.asp" title="PPTP Server" onclick="onChangeHeadMenu(42, 2, _('CONFIGURATION')+' > '+_('Security')+' > '+_('PPTP Server')+' > '+_('User Rule'))"><span><script>document.write(_("User Rule"))</script></span></a></li>
</span>    
<!-- vpn passthrough -->
<span id = "menu_43_1_hightline" class = "off" >
        <li class="hightline"><a title="General"><span><script>document.write(_("General"))</script></span></a></li>
</span>    

<span id = "menu_43_1" class = "off" >
        <li><a target="InfoFrame" href="vpn_passthrough.asp" title="General" onclick="onChangeHeadMenu(43, 1, _('CONFIGURATION')+' > '+_('Security')+' > '+_('VPN Passthrough')+' > '+_('General'))"><span><script>document.write(_("General"))</script></span></a></li>
</span>    

<!--
<span id = "menu_10_3_hightline" class = "off" >
        <li class="hightline"><a title="SPI"><span><script>document.write(_("SPI"))</script></span></a></li>
</span>    
<span id = "menu_10_3" class = "off" >
        <li><a target="InfoFrame" href="spi.asp" title="SPI" onclick="onChangeHeadMenu(10, 3, _('CONFIGURATION')+' > '+_('Security')+' > '+_('Firewall')+' > '+_('SPI'))"><span><script>document.write(_("SPI"))</script></span></a></li>
</span>
-->    
<!-- RIP -->    
<span id = "menu_11_1_hightline" class = "off" >
        <li class="hightline"><a title="RIP"><span><script>document.write(_("RIP"))</script></span></a></li>
</span>    
<span id = "menu_11_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/rip.asp" title="RIP" onclick="onChangeHeadMenu(11, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Routing')+' > '+_('RIP'))"><span><script>document.write(_("RIP"))</script></span></a></li>
</span>
<!-- DHCP Server -->    
<span id = "menu_12_1_hightline" class = "off" >
        <li class="hightline"><a title="General"><span><script>document.write(_("General"))</script></span></a></li>
</span>    
<span id = "menu_12_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/lan_dhcp.asp" title="General" onclick="onChangeHeadMenu(12, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('DHCP Server')+' > '+_('General'))"><span><script>document.write(_("General"))</script></span></a></li>
</span> 
<span id = "menu_12_2_hightline" class = "off" >
        <li class="hightline"><a title="Advanced"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>    
<span id = "menu_12_2" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/lan_dhcp_advance.asp" title="Advanced" onclick="onChangeHeadMenu(12, 2, _('CONFIGURATION')+' > '+_('Network')+' > '+_('DHCP Server')+' > '+_('Advanced'))"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>    
<!-- Bandwidth Management -->  
<!--<span id = "menu_14_1_hightline" class = "off" >
        <li class="hightline"><a title="General"><span><script>document.write(_("General"))</script></span></a></li>
</span>    
<span id = "menu_14_1" class = "off" >
        <li><a target="InfoFrame" href="bm.asp" title="General" onclick="onChangeHeadMenu(14, 1, _('CONFIGURATION')+' > '+_('Management')+' > '+_('Bandwidth Management')+' > '+_('General'))"><span><script>document.write(_("General"))</script></span></a></li>
</span>    
<span id = "menu_14_2_hightline" class = "off" >
        <li class="hightline"><a title="Advanced"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>    
<span id = "menu_14_2" class = "off" >
        <li><a target="InfoFrame" href="qos_adv.asp" title="Advanced" onclick="onChangeHeadMenu(14, 2, _('CONFIGURATION')+' > '+_('Utility')+' > '+_('Bandwidth Management')+' > '+_('Advanced'))"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>    
<span id = "menu_14_3_hightline" class = "off" >
        <li class="hightline"><a title="Monitor"><span><script>document.write(_("Monitor"))</script></span></a></li>
</span>
<span id = "menu_14_3" class = "off" >
        <li><a target="InfoFrame" href="bm_monitor.asp" title="Advanced" onclick="onChangeHeadMenu(14, 3, _('CONFIGURATION')+' > '+_('Utility')+' > '+_('Bandwidth Management')+' > '+_('Monitor'))"><span><script>document.write(_("Monitor"))</script></span></a></li>
</span>-->

<!-- Parental Control -->    
<span id = "menu_15_1_hightline" class = "off" >
        <li class="hightline"><a title="Content Filter"><span><script>document.write(_("Content Filter"))</script></span></a></li>
</span>    
<span id = "menu_15_1" class = "off" >
        <li><a target="InfoFrame" href="SCFilter.asp" title="Content Filter" onclick="onChangeHeadMenu(15, 1, _('CONFIGURATION')+' > '+_('Utility')+' > '+_('Parental Control')+' > '+_('Content Filter'))"><span><script>document.write(_("Content Filter"))</script></span></a></li>
</span>
<!--OPEN DNS-->
<span id = "menu_16_1_hightline" class = "off" >
        <li class="hightline"><a title="OpenDNS"><span><script>document.write(_("OpenDNS"))</script></span></a></li>
</span>    
<span id = "menu_16_1" class = "off" >
        <li><a target="InfoFrame" href="opendns.asp" title="OpenDNS" onclick="onChangeHeadMenu(16, 1, _('CONFIGURATION')+' > '+_('Utility')+' > '+_('Parental Control')+' > '+_('OpenDNS'))"><span><script>document.write(_("OpenDNS"))</script></span></a></li>
</span>

<!-- AP and Router Mode -->    
<span id = "menu_20_1_hightline" class = "off" >
        <li class="hightline"><a title="DownLink"><span><script>document.write(_("DownLink"))</script></span></a></li>
</span>    
<span id = "menu_20_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan.asp" title="DownLink" onclick="onChangeHeadMenu(20, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('DownLink'))"><span><script>document.write(_("DownLink"))</script></span></a></li>
</span>    

<span id = "menu_20_2_hightline" class = "off" >
        <li class="hightline"><a title="Security" id="Security_title0"><span><script>document.write(_("Security"))</script></span></a></li>
</span>    
<span id = "menu_20_2" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/security.asp" title="Security" id="Security_title" onclick="onChangeHeadMenu(20, 2, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('Security'))"><span><script>document.write(_("Security"))</script></span></a></li>
</span>

<span id = "menu_20_3_hightline" class = "off" >
        <li class="hightline"><a title="MAC Filter"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>    
<span id = "menu_20_3" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/mf.asp" title="MAC Filter" onclick="onChangeHeadMenu(20, 3, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('MAC Filter'))"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>

<span id = "menu_20_4_hightline" class = "off" >
        <li class="hightline"><a title="Advanced"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>    
<span id = "menu_20_4" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan_advance.asp" title="Advanced" onclick="onChangeHeadMenu(20, 4, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('Advanced'))"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span> 

<span id = "menu_20_5_hightline" class = "off" >
        <li class="hightline"><a title="QoS"><span><script>document.write(_("QoS"))</script></span></a></li>
</span>    
<span id = "menu_20_5" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/qos.asp" title="QoS" onclick="onChangeHeadMenu(20, 5, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('QoS'))"><span><script>document.write(_("QoS"))</script></span></a></li>
</span> 

<!--
<span id = "menu_20_6_hightline" class = "off" >
        <li class="hightline"><a title="WPS"><span><script>document.write(_("WPS"))</script></span></a></li>
</span>    
<span id = "menu_20_6" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wps.asp" title="WPS" onclick="onChangeHeadMenu(20, 6, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('WPS'))"><span><script>document.write(_("WPS"))</script></span></a></li>
</span> 

<span id = "menu_20_7_hightline" class = "off" >
        <li class="hightline"><a title="WPS Station"><span><script>document.write(_("WPS Station"))</script></span></a></li>
</span>    
<span id = "menu_20_7" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wps_station.asp" title="WPS Station" onclick="is_wps_init_page=1; onChangeHeadMenu(20, 7, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('WPS Station'))"><span><script>document.write(_("WPS Station"))</script></span></a></li>
</span> 

<span id = "menu_20_8_hightline" class = "off" >
        <li class="hightline"><a title="Scheduling"><span><script>document.write(_("Scheduling"))</script></span></a></li>
</span>    
<span id = "menu_20_8" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/scheduling.asp" title="Scheduling" onclick="onChangeHeadMenu(20, 8, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('Scheduling'))"><span><script>document.write(_("Scheduling"))</script></span></a></li>
</span>

<span id = "menu_20_9_hightline" class = "off" >
        <li class="hightline"><a title="WDS"><span><script>document.write(_("WDS"))</script></span></a></li>
</span>    
<span id = "menu_20_9" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wds.asp" title="WDS" onclick="onChangeHeadMenu(20, 9, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('WDS'))"><span><script>document.write(_("WDS"))</script></span></a></li>
</span>
-->

<span id = "menu_44_1_hightline" class = "off" >
        <li class="hightline"><a title="DownLink"><span><script>document.write(_("DownLink"))</script></span></a></li>
</span>    
<span id = "menu_44_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan.asp" title="DownLink" onclick="onChangeHeadMenu(44, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G-1')+' > '+_('DownLink'))"><span><script>document.write(_("DownLink"))</script></span></a></li>
</span>    

<span id = "menu_44_2_hightline" class = "off" >
        <li class="hightline"><a title="Security" id="Security_title0"><span><script>document.write(_("Security"))</script></span></a></li>
</span>    
<span id = "menu_44_2" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/security.asp" title="Security" id="Security_title" onclick="onChangeHeadMenu(44, 2, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G-1')+' > '+_('Security'))"><span><script>document.write(_("Security"))</script></span></a></li>
</span>

<span id = "menu_44_3_hightline" class = "off" >
        <li class="hightline"><a title="MAC Filter"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>    
<span id = "menu_44_3" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/mf.asp" title="MAC Filter" onclick="onChangeHeadMenu(44, 3, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G-1')+' > '+_('MAC Filter'))"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>

<span id = "menu_44_4_hightline" class = "off" >
        <li class="hightline"><a title="Advanced"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>    
<span id = "menu_44_4" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan_advance.asp" title="Advanced" onclick="onChangeHeadMenu(44, 4, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G-1')+' > '+_('Advanced'))"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span> 

<span id = "menu_44_5_hightline" class = "off" >
        <li class="hightline"><a title="QoS"><span><script>document.write(_("QoS"))</script></span></a></li>
</span>    
<span id = "menu_44_5" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/qos.asp" title="QoS" onclick="onChangeHeadMenu(44, 5, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G-1')+' > '+_('QoS'))"><span><script>document.write(_("QoS"))</script></span></a></li>
</span>

<!--AP and Router 2.4G-2-->
<span id = "menu_45_1_hightline" class = "off" >
        <li class="hightline"><a title="DownLink"><span><script>document.write(_("DownLink"))</script></span></a></li>
</span>    
<span id = "menu_45_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan.asp" title="DownLink" onclick="onChangeHeadMenu(45, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G-2')+' > '+_('DownLink'))"><span><script>document.write(_("DownLink"))</script></span></a></li>
</span>    

<span id = "menu_45_2_hightline" class = "off" >
        <li class="hightline"><a title="Security" id="Security_title0"><span><script>document.write(_("Security"))</script></span></a></li>
</span>    
<span id = "menu_45_2" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/security.asp" title="Security" id="Security_title" onclick="onChangeHeadMenu(45, 2, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G-2')+' > '+_('Security'))"><span><script>document.write(_("Security"))</script></span></a></li>
</span>

<span id = "menu_45_3_hightline" class = "off" >
        <li class="hightline"><a title="MAC Filter"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>    
<span id = "menu_45_3" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/mf.asp" title="MAC Filter" onclick="onChangeHeadMenu(45, 3, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G-2')+' > '+_('MAC Filter'))"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>

<span id = "menu_45_4_hightline" class = "off" >
        <li class="hightline"><a title="Advanced"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>    
<span id = "menu_45_4" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan_advance.asp" title="Advanced" onclick="onChangeHeadMenu(45, 4, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G-2')+' > '+_('Advanced'))"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span> 

<span id = "menu_45_5_hightline" class = "off" >
        <li class="hightline"><a title="QoS"><span><script>document.write(_("QoS"))</script></span></a></li>
</span>    
<span id = "menu_45_5" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/qos.asp" title="QoS" onclick="onChangeHeadMenu(45, 5, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G-2')+' > '+_('QoS'))"><span><script>document.write(_("QoS"))</script></span></a></li>
</span>

<!-- AP and Router Mode 5G -->    
<span id = "menu_26_1_hightline" class = "off" >
        <li class="hightline"><a title="DownLink"><span><script>document.write(_("DownLink"))</script></span></a></li>
</span>    
<span id = "menu_26_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan_5g.asp" title="DownLink" onclick="onChangeHeadMenu(26, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('DownLink'))"><span><script>document.write(_("DownLink"))</script></span></a></li>
</span>    
<span id = "menu_26_1_radar" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/radar.asp" title="DownLink" onclick="onChangeHeadMenu(26, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('DownLink'))"><span><script>document.write(_("DownLink"))</script></span></a></li>
</span>    

<span id = "menu_26_2_hightline" class = "off" >
        <li class="hightline"><a title="Security" id="Security_title0"><span><script>document.write(_("Security"))</script></span></a></li>
</span>    
<span id = "menu_26_2" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/security_5g.asp" title="Security" id="Security_title" onclick="onChangeHeadMenu(26, 2, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('Security'))"><span><script>document.write(_("Security"))</script></span></a></li>
</span>

<span id = "menu_26_3_hightline" class = "off" >
        <li class="hightline"><a title="MAC Filter"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>    
<span id = "menu_26_3" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/mf_5g.asp" title="MAC Filter" onclick="onChangeHeadMenu(26, 3, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('MAC Filter'))"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>

<span id = "menu_26_4_hightline" class = "off" >
        <li class="hightline"><a title="Advanced"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>    
<span id = "menu_26_4" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan_advance_5g.asp" title="Advanced" onclick="onChangeHeadMenu(26, 4, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('Advanced'))"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span> 

<span id = "menu_26_5_hightline" class = "off" >
        <li class="hightline"><a title="QoS"><span><script>document.write(_("QoS"))</script></span></a></li>
</span>    
<span id = "menu_26_5" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/qos_5g.asp" title="QoS" onclick="onChangeHeadMenu(26, 5, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('QoS'))"><span><script>document.write(_("QoS"))</script></span></a></li>
</span> 

<!--
<span id = "menu_26_6_hightline" class = "off" >
        <li class="hightline"><a title="WPS"><span><script>document.write(_("WPS"))</script></span></a></li>
</span>    
<span id = "menu_26_6" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wps_5g.asp" title="WPS" onclick="onChangeHeadMenu(26, 6, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('WPS'))"><span><script>document.write(_("WPS"))</script></span></a></li>
</span> 

<span id = "menu_26_7_hightline" class = "off" >
        <li class="hightline"><a title="WPS Station"><span><script>document.write(_("WPS Station"))</script></span></a></li>
</span>    
<span id = "menu_26_7" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wps_station_5g.asp" title="WPS Station" onclick="is_wps_init_page=1; onChangeHeadMenu(26, 7, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('WPS Station'))"><span><script>document.write(_("WPS Station"))</script></span></a></li>
</span> 

<span id = "menu_26_8_hightline" class = "off" >
        <li class="hightline"><a title="Scheduling"><span><script>document.write(_("Scheduling"))</script></span></a></li>
</span>    
<span id = "menu_26_8" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/scheduling_5g.asp" title="Scheduling" onclick="onChangeHeadMenu(26, 8, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('Scheduling'))"><span><script>document.write(_("Scheduling"))</script></span></a></li>
</span>

<span id = "menu_26_9_hightline" class = "off" >
        <li class="hightline"><a title="WDS"><span><script>document.write(_("WDS"))</script></span></a></li>
</span>    
<span id = "menu_26_9" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wds_5g.asp" title="WDS" onclick="onChangeHeadMenu(26, 9, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('WDS'))"><span><script>document.write(_("WDS"))</script></span></a></li>
</span>
-->

<!-- Client and WISP Mode -->
<!--2.4G as Wan, 2.4G as Lan-->
<span id = "menu_17_1_hightline" class = "off" >
        <li class="hightline"><a title="UpLink"><span><script>document.write(_("UpLink"))</script></span></a></li>
</span>    
<span id = "menu_17_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/apcli_wisp.asp" title="UpLink" onclick="onChangeHeadMenu(17, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('UpLink'))"><span><script>document.write(_("UpLink"))</script></span></a></li>
</span>
<span id = "menu_17_2_hightline" class = "off" >
        <li class="hightline"><a title="Site Survey"><span><script>document.write(_("Site Survey"))</script></span></a></li>
</span>    
<span id = "menu_17_2" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/apcli_site_waiting.asp" title="Site Survey" onclick="onChangeHeadMenu(17, 2, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('Site Survey'))"><span><script>document.write(_("Site Survey"))</script></span></a></li>
</span>
<span id = "menu_17_3_hightline" class = "off" >
        <li class="hightline"><a title="DownLink"><span><script>document.write(_("DownLink"))</script></span></a></li>
</span>    
<span id = "menu_17_3" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan.asp" title="DownLink" onclick="onChangeHeadMenu(17, 3, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('DownLink'))"><span><script>document.write(_("DownLink"))</script></span></a></li>
</span>    
<span id = "menu_17_4_hightline" class = "off" >
        <li class="hightline"><a title="Security" id="Security_title0"><span><script>document.write(_("Security"))</script></span></a></li>
</span>    
<span id = "menu_17_4" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/security.asp" title="Security" id="Security_title" onclick="onChangeHeadMenu(17, 4, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('Security'))"><span><script>document.write(_("Security"))</script></span></a></li>
</span>

<span id = "menu_17_5_hightline" class = "off" >
        <li class="hightline"><a title="MAC Filter"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>    
<span id = "menu_17_5" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/mf.asp" title="MAC Filter" onclick="onChangeHeadMenu(17, 5, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('MAC Filter'))"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>

<span id = "menu_17_6_hightline" class = "off" >
        <li class="hightline"><a title="Advanced"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>    
<span id = "menu_17_6" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan_advance.asp" title="Advanced" onclick="onChangeHeadMenu(17, 6, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('Advanced'))"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span> 

<span id = "menu_17_7_hightline" class = "off" >
        <li class="hightline"><a title="QoS"><span><script>document.write(_("QoS"))</script></span></a></li>
</span>    
<span id = "menu_17_7" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/qos.asp" title="QoS" onclick="onChangeHeadMenu(17, 7, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('QoS'))"><span><script>document.write(_("QoS"))</script></span></a></li>
</span> 

<span id = "menu_18_1_hightline" class = "off" >
        <li class="hightline"><a title="UpLink"><span><script>document.write(_("UpLink"))</script></span></a></li>
</span>    
<span id = "menu_18_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/apcli_wisp.asp" title="UpLink" onclick="onChangeHeadMenu(18, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('UpLink'))"><span><script>document.write(_("UpLink"))</script></span></a></li>
</span>
<span id = "menu_18_2_hightline" class = "off" >
        <li class="hightline"><a title="Site Survey"><span><script>document.write(_("Site Survey"))</script></span></a></li>
</span>    
<span id = "menu_18_2" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/apcli_site_waiting.asp" title="Site Survey" onclick="onChangeHeadMenu(18, 2, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('Site Survey'))"><span><script>document.write(_("Site Survey"))</script></span></a></li>
</span>

<span id = "menu_18_3_hightline" class = "off" >
        <li class="hightline"><a title="Advanced"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>    
<span id = "menu_18_3" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan_advance.asp" title="Advanced" onclick="onChangeHeadMenu(18, 3, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('Advanced'))"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>

<span id = "menu_18_4_hightline" class = "off" >
        <li class="hightline"><a title="QoS"><span><script>document.write(_("QoS"))</script></span></a></li>
</span>    
<span id = "menu_18_4" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/qos.asp" title="QoS" onclick="onChangeHeadMenu(18, 4, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('QoS'))"><span><script>document.write(_("QoS"))</script></span></a></li>
</span> 

<!--Wisp 2.4G as Wan, 5G as Lan-->
<span id = "menu_21_1_hightline" class = "off" >
        <li class="hightline"><a title="UpLink"><span><script>document.write(_("UpLink"))</script></span></a></li>
</span>    
<span id = "menu_21_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/apcli_wisp.asp" title="UpLink" onclick="onChangeHeadMenu(21, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('UpLink'))"><span><script>document.write(_("UpLink"))</script></span></a></li>
</span>
<span id = "menu_21_2_hightline" class = "off" >
        <li class="hightline"><a title="Site Survey"><span><script>document.write(_("Site Survey"))</script></span></a></li>
</span>    
<span id = "menu_21_2" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/apcli_site_waiting.asp" title="Site Survey" onclick="onChangeHeadMenu(21, 2, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('Site Survey'))"><span><script>document.write(_("Site Survey"))</script></span></a></li>
</span>
<span id = "menu_21_3_hightline" class = "off" >
        <li class="hightline"><a title="DownLink"><span><script>document.write(_("DownLink"))</script></span></a></li>
</span>    
<span id = "menu_21_3" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan_5g.asp" title="DownLink" onclick="onChangeHeadMenu(21, 3, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('DownLink'))"><span><script>document.write(_("DownLink"))</script></span></a></li>
</span>    
<span id = "menu_21_4_hightline" class = "off" >
        <li class="hightline"><a title="Security" id="Security_title0"><span><script>document.write(_("Security"))</script></span></a></li>
</span>    
<span id = "menu_21_4" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/security_5g.asp" title="Security" id="Security_title" onclick="onChangeHeadMenu(21, 4, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('Security'))"><span><script>document.write(_("Security"))</script></span></a></li>
</span>

<span id = "menu_21_5_hightline" class = "off" >
        <li class="hightline"><a title="MAC Filter"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>    
<span id = "menu_21_5" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/mf.asp" title="MAC Filter" onclick="onChangeHeadMenu(21, 5, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('MAC Filter'))"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>

<span id = "menu_21_6_hightline" class = "off" >
        <li class="hightline"><a title="Advanced"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>    
<span id = "menu_21_6" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan_advance_5g.asp" title="Advanced" onclick="onChangeHeadMenu(21, 6, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('Advanced'))"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span> 

<span id = "menu_21_7_hightline" class = "off" >
        <li class="hightline"><a title="QoS"><span><script>document.write(_("QoS"))</script></span></a></li>
</span>    
<span id = "menu_21_7" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/qos.asp" title="QoS" onclick="onChangeHeadMenu(21, 7, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('QoS'))"><span><script>document.write(_("QoS"))</script></span></a></li>
</span> 
<!-- Client and WISP Mode 5g -->
<span id = "menu_27_1_hightline" class = "off" >
        <li class="hightline"><a title="UpLink"><span><script>document.write(_("UpLink"))</script></span></a></li>
</span>    
<span id = "menu_27_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/apcli_wisp_5g.asp" title="UpLink" onclick="onChangeHeadMenu(27, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('UpLink'))"><span><script>document.write(_("UpLink"))</script></span></a></li>
</span>
<span id = "menu_27_2_hightline" class = "off" >
        <li class="hightline"><a title="Site Survey"><span><script>document.write(_("Site Survey"))</script></span></a></li>
</span>    
<span id = "menu_27_2" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/apcli_site_waiting_5g.asp" title="Site Survey" onclick="onChangeHeadMenu(27, 2, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('Site Survey'))"><span><script>document.write(_("Site Survey"))</script></span></a></li>
</span>
<span id = "menu_27_3_hightline" class = "off" >
        <li class="hightline"><a title="DownLink"><span><script>document.write(_("DownLink"))</script></span></a></li>
</span>    
<span id = "menu_27_3" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan.asp" title="DownLink" onclick="onChangeHeadMenu(27, 3, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('DownLink'))"><span><script>document.write(_("DownLink"))</script></span></a></li>
</span>    
<span id = "menu_27_4_hightline" class = "off" >
        <li class="hightline"><a title="Security" id="Security_title0"><span><script>document.write(_("Security"))</script></span></a></li>
</span>    
<span id = "menu_27_4" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/security.asp" title="Security" id="Security_title" onclick="onChangeHeadMenu(27, 4, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('Security'))"><span><script>document.write(_("Security"))</script></span></a></li>
</span>

<span id = "menu_27_5_hightline" class = "off" >
        <li class="hightline"><a title="MAC Filter"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>    
<span id = "menu_27_5" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/mf.asp" title="MAC Filter" onclick="onChangeHeadMenu(27, 5, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('MAC Filter'))"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>

<span id = "menu_27_6_hightline" class = "off" >
        <li class="hightline"><a title="Advanced"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>    
<span id = "menu_27_6" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan_advance.asp" title="Advanced" onclick="onChangeHeadMenu(27, 6, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('Advanced'))"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span> 

<span id = "menu_27_7_hightline" class = "off" >
        <li class="hightline"><a title="QoS"><span><script>document.write(_("QoS"))</script></span></a></li>
</span>    
<span id = "menu_27_7" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/qos.asp" title="QoS" onclick="onChangeHeadMenu(27, 7, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('QoS'))"><span><script>document.write(_("QoS"))</script></span></a></li>
</span> 
<!--
<span id = "menu_21_2_hightline" class = "off" >
        <li class="hightline"><a title="Advanced"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>    
<span id = "menu_21_2" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wisp_advance.asp" title="Advanced" onclick="onChangeHeadMenu(21, 2, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('Advance'))"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>

<span id = "menu_21_3_hightline" class = "off" >
        <li class="hightline"><a title="Site Survey"><span><script>document.write(_("Site Survey"))</script></span></a></li>
</span>    
<span id = "menu_21_3" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wisp_site_survey.asp" title="Site Survey" onclick="onChangeHeadMenu(21, 3, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('Site Survey'))"><span><script>document.write(_("Site Survey"))</script></span></a></li>
</span>
-->
<!-- WDS -->
<span id = "menu_22_1_hightline" class = "off" >
        <li class="hightline"><a title="General"><span><script>document.write(_("General"))</script></span></a></li>
</span>    
<span id = "menu_22_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan.asp" title="General" onclick="onChangeHeadMenu(22, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('General'))"><span><script>document.write(_("General"))</script></span></a></li>
</span>    

<span id = "menu_22_2_hightline" class = "off" >
        <li class="hightline"><a title="Advanced"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>    
<span id = "menu_22_2" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan_advance.asp" title="Advanced" onclick="onChangeHeadMenu(22, 2, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('Advanced'))"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span> 

<span id = "menu_22_3_hightline" class = "off" >
        <li class="hightline"><a title="WDS"><span><script>document.write(_("WDS"))</script></span></a></li>
</span>    
<span id = "menu_22_3" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wds.asp" title="WDS" onclick="onChangeHeadMenu(22, 3, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('WDS'))"><span><script>document.write(_("WDS"))</script></span></a></li>
</span>
<!-- 5g WDS-->
<span id = "menu_28_1_hightline" class = "off" >
        <li class="hightline"><a title="General"><span><script>document.write(_("General"))</script></span></a></li>
</span>    
<span id = "menu_28_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan_5g.asp" title="General" onclick="onChangeHeadMenu(28, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('General'))"><span><script>document.write(_("General"))</script></span></a></li>
</span>    

<span id = "menu_28_2_hightline" class = "off" >
        <li class="hightline"><a title="Advanced"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>    
<span id = "menu_28_2" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan_advance_5g.asp" title="Advanced" onclick="onChangeHeadMenu(28, 2, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('Advanced'))"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span> 

<span id = "menu_28_3_hightline" class = "off" >
        <li class="hightline"><a title="WDS"><span><script>document.write(_("WDS"))</script></span></a></li>
</span>    
<span id = "menu_28_3" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wds_5g.asp" title="WDS" onclick="onChangeHeadMenu(28, 3, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('WDS'))"><span><script>document.write(_("WDS"))</script></span></a></li>
</span>

<!-- AP + WDS -->
<span id = "menu_23_1_hightline" class = "off" >
        <li class="hightline"><a title="General"><span><script>document.write(_("General"))</script></span></a></li>
</span>    
<span id = "menu_23_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan.asp" title="General" onclick="onChangeHeadMenu(23, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('General'))"><span><script>document.write(_("General"))</script></span></a></li>
</span>    

<span id = "menu_23_2_hightline" class = "off" >
        <li class="hightline"><a title="Security"><span><script>document.write(_("Security"))</script></span></a></li>
</span>    
<span id = "menu_23_2" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/security.asp" title="Security" onclick="onChangeHeadMenu(23, 2, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('Security'))"><span><script>document.write(_("Security"))</script></span></a></li>
</span>

<span id = "menu_23_3_hightline" class = "off" >
        <li class="hightline"><a title="MAC Filter"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>    
<span id = "menu_23_3" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/mf.asp" title="MAC Filter" onclick="onChangeHeadMenu(23, 3, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('MAC Filter'))"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>
<span id = "menu_23_4_hightline" class = "off" >
        <li class="hightline"><a title="Advanced"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>    
<span id = "menu_23_4" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan_advance.asp" title="Advanced" onclick="onChangeHeadMenu(23, 4, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('Advanced'))"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span> 
<span id = "menu_23_5_hightline" class = "off" >
        <li class="hightline"><a title="QoS"><span><script>document.write(_("QoS"))</script></span></a></li>
</span>    
<span id = "menu_23_5" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/qos.asp" title="QoS" onclick="onChangeHeadMenu(23, 5, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('QoS'))"><span><script>document.write(_("QoS"))</script></span></a></li>
</span> 
<span id = "menu_23_6_hightline" class = "off" >
        <li class="hightline"><a title="WPS"><span><script>document.write(_("WPS"))</script></span></a></li>
</span>    
<span id = "menu_23_6" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wps.asp" title="WPS" onclick="onChangeHeadMenu(23, 6, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('WPS'))"><span><script>document.write(_("WPS"))</script></span></a></li>
</span> 
<span id = "menu_23_7_hightline" class = "off" >
        <li class="hightline"><a title="WPS Station"><span><script>document.write(_("WPS Station"))</script></span></a></li>
</span>    
<span id = "menu_23_7" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wps_station.asp" title="WPS Station" onclick="is_wps_init_page=1; onChangeHeadMenu(23, 7, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('WPS Station'))"><span><script>document.write(_("WPS Station"))</script></span></a></li>
</span> 
<span id = "menu_23_8_hightline" class = "off" >
        <li class="hightline"><a title="Scheduling"><span><script>document.write(_("Scheduling"))</script></span></a></li>
</span>    
<span id = "menu_23_8" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/scheduling.asp" title="Scheduling" onclick="onChangeHeadMenu(23, 8, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('Scheduling'))"><span><script>document.write(_("Scheduling"))</script></span></a></li>
</span>

<span id = "menu_23_9_hightline" class = "off" >
        <li class="hightline"><a title="WDS"><span><script>document.write(_("WDS"))</script></span></a></li>
</span>    
<span id = "menu_23_9" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wds.asp" title="WDS" onclick="onChangeHeadMenu(23, 9, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('WDS'))"><span><script>document.write(_("WDS"))</script></span></a></li>
</span>
<!--5g AP + WDS -->
<span id = "menu_29_1_hightline" class = "off" >
        <li class="hightline"><a title="General"><span><script>document.write(_("General"))</script></span></a></li>
</span>    
<span id = "menu_29_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan_5g.asp" title="General" onclick="onChangeHeadMenu(29, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('General'))"><span><script>document.write(_("General"))</script></span></a></li>
</span>    

<span id = "menu_29_2_hightline" class = "off" >
        <li class="hightline"><a title="Security"><span><script>document.write(_("Security"))</script></span></a></li>
</span>    
<span id = "menu_29_2" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/security_5g.asp" title="Security" onclick="onChangeHeadMenu(29, 2, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('Security'))"><span><script>document.write(_("Security"))</script></span></a></li>
</span>

<span id = "menu_29_3_hightline" class = "off" >
        <li class="hightline"><a title="MAC Filter"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>    
<span id = "menu_29_3" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/mf_5g.asp" title="MAC Filter" onclick="onChangeHeadMenu(29, 3, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('MAC Filter'))"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>
<span id = "menu_29_4_hightline" class = "off" >
        <li class="hightline"><a title="Advanced"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>    
<span id = "menu_29_4" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan_advance_5g.asp" title="Advanced" onclick="onChangeHeadMenu(29, 4, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('Advanced'))"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span> 
<span id = "menu_29_5_hightline" class = "off" >
        <li class="hightline"><a title="QoS"><span><script>document.write(_("QoS"))</script></span></a></li>
</span>    
<span id = "menu_29_5" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/qos_5g.asp" title="QoS" onclick="onChangeHeadMenu(29, 5, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('QoS'))"><span><script>document.write(_("QoS"))</script></span></a></li>
</span> 
<span id = "menu_29_6_hightline" class = "off" >
        <li class="hightline"><a title="WPS"><span><script>document.write(_("WPS"))</script></span></a></li>
</span>    
<span id = "menu_29_6" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wps_5g.asp" title="WPS" onclick="onChangeHeadMenu(29, 6, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('WPS'))"><span><script>document.write(_("WPS"))</script></span></a></li>
</span> 
<span id = "menu_29_7_hightline" class = "off" >
        <li class="hightline"><a title="WPS Station"><span><script>document.write(_("WPS Station"))</script></span></a></li>
</span>    
<span id = "menu_29_7" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wps_station_5g.asp" title="WPS Station" onclick="is_wps_init_page=1; onChangeHeadMenu(29, 7, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('WPS Station'))"><span><script>document.write(_("WPS Station"))</script></span></a></li>
</span> 
<span id = "menu_29_8_hightline" class = "off" >
        <li class="hightline"><a title="Scheduling"><span><script>document.write(_("Scheduling"))</script></span></a></li>
</span>    
<span id = "menu_29_8" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/scheduling_5g.asp" title="Scheduling" onclick="onChangeHeadMenu(29, 8, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('Scheduling'))"><span><script>document.write(_("Scheduling"))</script></span></a></li>
</span>

<span id = "menu_29_9_hightline" class = "off" >
        <li class="hightline"><a title="WDS"><span><script>document.write(_("WDS"))</script></span></a></li>
</span>    
<span id = "menu_29_9" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wds_5g.asp" title="WDS" onclick="onChangeHeadMenu(29, 9, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('WDS'))"><span><script>document.write(_("WDS"))</script></span></a></li>
</span>

<!-- AP + Client -->
<span id = "menu_24_1_hightline" class = "off" >
        <li class="hightline"><a title="UpLink"><span><script>document.write(_("UpLink"))</script></span></a></li>
</span>    
<span id = "menu_24_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/ur.asp" title="UpLink" onclick="onChangeHeadMenu(24, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('UpLink'))"><span><script>document.write(_("UpLink"))</script></span></a></li>
</span>

<span id = "menu_24_2_hightline" class = "off" >
        <li class="hightline"><a title="Site Survey"><span><script>document.write(_("Site Survey"))</script></span></a></li>
</span>    
<span id = "menu_24_2" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/ur_site_waiting.asp" title="Site Survey" onclick="onChangeHeadMenu(24, 2, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('Site Survey'))"><span><script>document.write(_("Site Survey"))</script></span></a></li>
</span>
<!--
<span id = "menu_24_3_hightline" class = "off" >
        <li class="hightline"><a title="MAC Filter"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>    
<span id = "menu_24_3" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/mf.asp" title="MAC Filter" onclick="onChangeHeadMenu(24, 3, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('MAC Filter'))"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>

-->
<span id = "menu_24_3_hightline" class = "off" >
        <li class="hightline"><a title="Advanced"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>    
<span id = "menu_24_3" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan_advance.asp" title="Advanced" onclick="onChangeHeadMenu(24, 3, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('Advanced'))"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>

<span id = "menu_24_4_hightline" class = "off" >
        <li class="hightline"><a title="QoS"><span><script>document.write(_("QoS"))</script></span></a></li>
</span>    
<span id = "menu_24_4" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/qos.asp" title="QoS" onclick="onChangeHeadMenu(24, 4, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('QoS'))"><span><script>document.write(_("QoS"))</script></span></a></li>
</span>
<!-- 
<span id = "menu_24_6_hightline" class = "off" >
        <li class="hightline"><a title="WPS"><span><script>document.write(_("WPS"))</script></span></a></li>
</span>    
<span id = "menu_24_6" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wps.asp" title="WPS" onclick="onChangeHeadMenu(24, 6, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('WPS'))"><span><script>document.write(_("WPS"))</script></span></a></li>
</span> 
<span id = "menu_24_7_hightline" class = "off" >
        <li class="hightline"><a title="WPS Station"><span><script>document.write(_("WPS Station"))</script></span></a></li>
</span>    
<span id = "menu_24_7" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wps_station.asp" title="WPS Station" onclick="is_wps_init_page=1; onChangeHeadMenu(24, 7, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('WPS Station'))"><span><script>document.write(_("WPS Station"))</script></span></a></li>
</span> 
<span id = "menu_24_8_hightline" class = "off" >
        <li class="hightline"><a title="Scheduling"><span><script>document.write(_("Scheduling"))</script></span></a></li>
</span>    
<span id = "menu_24_8" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/scheduling.asp" title="Scheduling" onclick="onChangeHeadMenu(24, 8, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('Scheduling'))"><span><script>document.write(_("Scheduling"))</script></span></a></li>
</span>

<span id = "menu_24_9_hightline" class = "off" >
        <li class="hightline"><a title="Universal Repeater"><span><script>document.write(_("Universal Repeater"))</script></span></a></li>
</span>    
<span id = "menu_24_9" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/apcli.asp" title="Universal Repeater" onclick="onChangeHeadMenu(24, 9, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('Universal Repeater'))"><span><script>document.write(_("Universal Repeater"))</script></span></a></li>
</span>

<span id = "menu_24_5_hightline" class = "off" >
        <li class="hightline"><a title="Downlink"><span><script>document.write(_("Downlink"))</script></span></a></li></span>   
<span id = "menu_24_5" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan.asp" title="Downlink" onclick="onChangeHeadMenu(24, 5, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('Downlink'))"><span><script>document.write(_("Downlink"))</script></span></a></li></span>

<span id = "menu_24_6_hightline" class = "off" >
        <li class="hightline"><a title="Security"><span><script>document.write(_("Security"))</script></span></a></li></span>
<span id = "menu_24_6" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/security.asp" title="Security" onclick="onChangeHeadMenu(24, 6, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('Security'))"><span><script>document.write(_("Security"))</script></span></a></li></span>


<script type="text/javascript">
if(bridge_mode != 0)
{
    //Downlink
	document.write("<span id = 'menu_24_5_hightline' class = 'off' ><li class='hightline'><a title='Downlink'><span>Downlink</span></a></li></span>");
	document.write("<span id = 'menu_24_5' class = 'off' ><li><a target='InfoFrame' href='/local/advance/wlan.asp' title='Downlink' onclick='onChangeHeadMenu(24, 5, CONFIGURATION + > + Network + > + Wireless LAN 2.4G + >  + Downlink)'><span>Downlink</span></a></li></span>");
    //security
	document.write("<span id = 'menu_24_6_hightline' class = 'off' ><li class='hightline'><a title='Security'><span>Security</span></a></li></span>");
	document.write("<span id = 'menu_24_6' class = 'off' ><li><a target='InfoFrame' href='/local/advance/security.asp' title='Security' onclick='onChangeHeadMenu(24, 6, _(CONFIGURATION) + > + _(Network) + > + _(Wireless LAN 2.4G) + > + _(Security))'><span>Security</span></a></li></span>");
}
</script>
-->

<span id = "menu_54_1_hightline" class = "off" >
        <li class="hightline"><a title="UpLink"><span><script>document.write(_("UpLink"))</script></span></a></li>
</span>    
<span id = "menu_54_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/ur.asp" title="UpLink" onclick="onChangeHeadMenu(54, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('UpLink'))"><span><script>document.write(_("UpLink"))</script></span></a></li>
</span>

<span id = "menu_54_2_hightline" class = "off" >
        <li class="hightline"><a title="Site Survey"><span><script>document.write(_("Site Survey"))</script></span></a></li>
</span>    
<span id = "menu_54_2" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/ur_site_waiting.asp" title="Site Survey" onclick="onChangeHeadMenu(54, 2, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('Site Survey'))"><span><script>document.write(_("Site Survey"))</script></span></a></li>
</span>

<span id = "menu_54_3_hightline" class = "off" >
        <li class="hightline"><a title="DownLink"><span><script>document.write(_("DownLink"))</script></span></a></li></span>   
<span id = "menu_54_3" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan.asp" title="DownLink" onclick="onChangeHeadMenu(54, 3, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('DownLink'))"><span><script>document.write(_("DownLink"))</script></span></a></li></span>

<span id = "menu_54_4_hightline" class = "off" >
        <li class="hightline"><a title="Security"><span><script>document.write(_("Security"))</script></span></a></li></span>
<span id = "menu_54_4" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/security.asp" title="Security" onclick="onChangeHeadMenu(54, 4, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('Security'))"><span><script>document.write(_("Security"))</script></span></a></li></span>

<span id = "menu_54_5_hightline" class = "off" >
        <li class="hightline"><a title="MAC Filter"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>    
<span id = "menu_54_5" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/mf.asp" title="MAC Filter" onclick="onChangeHeadMenu(54, 5, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('MAC Filter'))"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>

<span id = "menu_54_6_hightline" class = "off" >
        <li class="hightline"><a title="Advanced"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>    
<span id = "menu_54_6" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan_advance.asp" title="Advanced" onclick="onChangeHeadMenu(54, 6, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('Advanced'))"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span> 
<span id = "menu_54_7_hightline" class = "off" >
        <li class="hightline"><a title="QoS"><span><script>document.write(_("QoS"))</script></span></a></li>
</span>    
<span id = "menu_54_7" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/qos.asp" title="QoS" onclick="onChangeHeadMenu(54, 7, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('QoS'))"><span><script>document.write(_("QoS"))</script></span></a></li>
</span>


		
<span id = "menu_55_1_hightline" class = "off" >
        <li class="hightline"><a title="UpLink"><span><script>document.write(_("UpLink"))</script></span></a></li>
</span>    
<span id = "menu_55_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/ur_5g.asp" title="UpLink" onclick="onChangeHeadMenu(55, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('UpLink'))"><span><script>document.write(_("UpLink"))</script></span></a></li>
</span>

<span id = "menu_55_2_hightline" class = "off" >
        <li class="hightline"><a title="Site Survey"><span><script>document.write(_("Site Survey"))</script></span></a></li>
</span>    
<span id = "menu_55_2" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/ur_site_waiting_5g.asp" title="Site Survey" onclick="onChangeHeadMenu(55, 2, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('Site Survey'))"><span><script>document.write(_("Site Survey"))</script></span></a></li>
</span>

<span id = "menu_55_3_hightline" class = "off" >
        <li class="hightline"><a title="DownLink"><span><script>document.write(_("DownLink"))</script></span></a></li></span>   
<span id = "menu_55_3" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan.asp" title="DownLink" onclick="onChangeHeadMenu(55, 3, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('DownLink'))"><span><script>document.write(_("DownLink"))</script></span></a></li></span>

<span id = "menu_55_4_hightline" class = "off" >
        <li class="hightline"><a title="Security"><span><script>document.write(_("Security"))</script></span></a></li></span>
<span id = "menu_55_4" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/security.asp" title="Security" onclick="onChangeHeadMenu(55, 4, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('Security'))"><span><script>document.write(_("Security"))</script></span></a></li></span>

<span id = "menu_55_5_hightline" class = "off" >
        <li class="hightline"><a title="MAC Filter"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>    
<span id = "menu_55_5" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/mf.asp" title="MAC Filter" onclick="onChangeHeadMenu(55, 5, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('MAC Filter'))"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>

<span id = "menu_55_6_hightline" class = "off" >
        <li class="hightline"><a title="Advanced"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>    
<span id = "menu_55_6" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan_advance.asp" title="Advanced" onclick="onChangeHeadMenu(55, 6, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('Advanced'))"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span> 
<span id = "menu_55_7_hightline" class = "off" >
        <li class="hightline"><a title="QoS"><span><script>document.write(_("QoS"))</script></span></a></li>
</span>    
<span id = "menu_55_7" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/qos.asp" title="QoS" onclick="onChangeHeadMenu(55, 7, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('QoS'))"><span><script>document.write(_("QoS"))</script></span></a></li>
</span>

<span id = "menu_56_1_hightline" class = "off" >
        <li class="hightline"><a title="UpLink"><span><script>document.write(_("UpLink"))</script></span></a></li>
</span>    
<span id = "menu_56_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/ur.asp" title="UpLink" onclick="onChangeHeadMenu(56, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('UpLink'))"><span><script>document.write(_("UpLink"))</script></span></a></li>
</span>

<span id = "menu_56_2_hightline" class = "off" >
        <li class="hightline"><a title="Site Survey"><span><script>document.write(_("Site Survey"))</script></span></a></li>
</span>    
<span id = "menu_56_2" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/ur_site_waiting.asp" title="Site Survey" onclick="onChangeHeadMenu(56, 2, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('Site Survey'))"><span><script>document.write(_("Site Survey"))</script></span></a></li>
</span>

<span id = "menu_56_3_hightline" class = "off" >
        <li class="hightline"><a title="DownLink"><span><script>document.write(_("DownLink"))</script></span></a></li></span>   
<span id = "menu_56_3" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan_5g.asp" title="DownLink" onclick="onChangeHeadMenu(56, 3, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('DownLink'))"><span><script>document.write(_("DownLink"))</script></span></a></li></span>

<span id = "menu_56_4_hightline" class = "off" >
        <li class="hightline"><a title="Security"><span><script>document.write(_("Security"))</script></span></a></li></span>
<span id = "menu_56_4" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/security_5g.asp" title="Security" onclick="onChangeHeadMenu(56, 4, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('Security'))"><span><script>document.write(_("Security"))</script></span></a></li></span>

<span id = "menu_56_5_hightline" class = "off" >
        <li class="hightline"><a title="MAC Filter"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>    
<span id = "menu_56_5" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/mf.asp" title="MAC Filter" onclick="onChangeHeadMenu(56, 5, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('MAC Filter'))"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>

<span id = "menu_56_6_hightline" class = "off" >
        <li class="hightline"><a title="Advanced"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>    
<span id = "menu_56_6" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan_advance_5g.asp" title="Advanced" onclick="onChangeHeadMenu(56, 6, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('Advanced'))"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span> 
<span id = "menu_56_7_hightline" class = "off" >
        <li class="hightline"><a title="QoS"><span><script>document.write(_("QoS"))</script></span></a></li>
</span>    
<span id = "menu_56_7" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/qos.asp" title="QoS" onclick="onChangeHeadMenu(56, 7, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('QoS'))"><span><script>document.write(_("QoS"))</script></span></a></li>
</span>



<!-- 5g AP + Client -->
<span id = "menu_33_1_hightline" class = "off" >
        <li class="hightline"><a title="General"><span><script>document.write(_("General"))</script></span></a></li>
</span>    
<span id = "menu_33_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan_5g.asp" title="General" onclick="onChangeHeadMenu(33, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('General'))"><span><script>document.write(_("General"))</script></span></a></li>
</span>

<span id = "menu_33_1_radar" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/radar.asp" title="General" onclick="onChangeHeadMenu(33, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('General'))"><span><script>document.write(_("General"))</script></span></a></li>
</span> 

<span id = "menu_33_2_hightline" class = "off" >
        <li class="hightline"><a title="Security"><span><script>document.write(_("Security"))</script></span></a></li>
</span>    
<span id = "menu_33_2" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/security_5g.asp" title="Security" onclick="onChangeHeadMenu(33, 2, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('Security'))"><span><script>document.write(_("Security"))</script></span></a></li>
</span>

<span id = "menu_33_3_hightline" class = "off" >
        <li class="hightline"><a title="MAC Filter"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>    
<span id = "menu_33_3" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/mf_5g.asp" title="MAC Filter" onclick="onChangeHeadMenu(33, 3, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('MAC Filter'))"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>

<span id = "menu_33_4_hightline" class = "off" >
        <li class="hightline"><a title="Advanced"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>    
<span id = "menu_33_4" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan_advance_5g.asp" title="Advanced" onclick="onChangeHeadMenu(33, 4, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('Advanced'))"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span> 
<span id = "menu_33_5_hightline" class = "off" >
        <li class="hightline"><a title="QoS"><span><script>document.write(_("QoS"))</script></span></a></li>
</span>    
<span id = "menu_33_5" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/qos_5g.asp" title="QoS" onclick="onChangeHeadMenu(33, 5, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('QoS'))"><span><script>document.write(_("QoS"))</script></span></a></li>
</span>
<!-- 
<span id = "menu_33_6_hightline" class = "off" >
        <li class="hightline"><a title="WPS"><span><script>document.write(_("WPS"))</script></span></a></li>
</span>    
<span id = "menu_33_6" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wps_5g.asp" title="WPS" onclick="onChangeHeadMenu(33, 6, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('WPS'))"><span><script>document.write(_("WPS"))</script></span></a></li>
</span> 
<span id = "menu_33_7_hightline" class = "off" >
        <li class="hightline"><a title="WPS Station"><span><script>document.write(_("WPS Station"))</script></span></a></li>
</span>    
<span id = "menu_33_7" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wps_station_5g.asp" title="WPS Station" onclick="is_wps_init_page=1; onChangeHeadMenu(33, 7, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('WPS Station'))"><span><script>document.write(_("WPS Station"))</script></span></a></li>
</span> 
<span id = "menu_33_8_hightline" class = "off" >
        <li class="hightline"><a title="Scheduling"><span><script>document.write(_("Scheduling"))</script></span></a></li>
</span>    
<span id = "menu_33_8" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/scheduling_5g.asp" title="Scheduling" onclick="onChangeHeadMenu(33, 8, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('Scheduling'))"><span><script>document.write(_("Scheduling"))</script></span></a></li>
</span>
-->
<span id = "menu_33_6_hightline" class = "off" >
        <li class="hightline"><a title="Universal Repeater"><span><script>document.write(_("Universal Repeater"))</script></span></a></li>
</span>    
<span id = "menu_33_6" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/ur_5g.asp" title="Universal Repeater" onclick="onChangeHeadMenu(33, 6, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('Universal Repeater'))"><span><script>document.write(_("Universal Repeater"))</script></span></a></li>
</span>

<span id = "menu_33_7_hightline" class = "off" >
        <li class="hightline"><a title="Site Survey"><span><script>document.write(_("Site Survey"))</script></span></a></li>
</span>    
<span id = "menu_33_7" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/ur_site_waiting_5g.asp" title="Site Survey" onclick="onChangeHeadMenu(33, 7, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('Site Survey'))"><span><script>document.write(_("Site Survey"))</script></span></a></li>
</span>
<!-- WISP + UR-->
<span id = "menu_25_1_hightline" class = "off" >
        <li class="hightline"><a title="General"><span><script>document.write(_("General"))</script></span></a></li>
</span>    
<span id = "menu_25_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan.asp" title="General" onclick="onChangeHeadMenu(25, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('General'))"><span><script>document.write(_("General"))</script></span></a></li>
</span>    

<span id = "menu_25_2_hightline" class = "off" >
        <li class="hightline"><a title="Security"><span><script>document.write(_("Security"))</script></span></a></li>
</span>    
<span id = "menu_25_2" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/security.asp" title="Security" onclick="onChangeHeadMenu(25, 2, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('Security'))"><span><script>document.write(_("Security"))</script></span></a></li>
</span>

<span id = "menu_25_3_hightline" class = "off" >
        <li class="hightline"><a title="MAC Filter"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>    
<span id = "menu_25_3" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/mf.asp" title="MAC Filter" onclick="onChangeHeadMenu(25, 3, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('MAC Filter'))"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>
<span id = "menu_25_4_hightline" class = "off" >
        <li class="hightline"><a title="Advanced"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>    
<span id = "menu_25_4" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan_advance.asp" title="Advanced" onclick="onChangeHeadMenu(25, 4, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('Advanced'))"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span> 
<span id = "menu_25_5_hightline" class = "off" >
        <li class="hightline"><a title="QoS"><span><script>document.write(_("QoS"))</script></span></a></li>
</span>    
<span id = "menu_25_5" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/qos.asp" title="QoS" onclick="onChangeHeadMenu(25, 5, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('QoS'))"><span><script>document.write(_("QoS"))</script></span></a></li>
</span> 
<span id = "menu_25_6_hightline" class = "off" >
        <li class="hightline"><a title="WPS"><span><script>document.write(_("WPS"))</script></span></a></li>
</span>    
<span id = "menu_25_6" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wps.asp" title="WPS" onclick="onChangeHeadMenu(25, 6, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('WPS'))"><span><script>document.write(_("WPS"))</script></span></a></li>
</span> 
<span id = "menu_25_7_hightline" class = "off" >
        <li class="hightline"><a title="WPS Station"><span><script>document.write(_("WPS Station"))</script></span></a></li>
</span>    
<span id = "menu_25_7" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wps_station.asp" title="WPS Station" onclick="is_wps_init_page=1; onChangeHeadMenu(25, 7, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('WPS Station'))"><span><script>document.write(_("WPS Station"))</script></span></a></li>
</span> 
<span id = "menu_25_8_hightline" class = "off" >
        <li class="hightline"><a title="Scheduling"><span><script>document.write(_("Scheduling"))</script></span></a></li>
</span>    
<span id = "menu_25_8" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/scheduling.asp" title="Scheduling" onclick="onChangeHeadMenu(25, 8, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('Scheduling'))"><span><script>document.write(_("Scheduling"))</script></span></a></li>
</span>
<span id = "menu_25_9_hightline" class = "off" >
        <li class="hightline"><a title="Universal Repeater"><span><script>document.write(_("Universal Repeater"))</script></span></a></li>
</span>    
<span id = "menu_25_9" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/apcli_wisp.asp" title="Universal Repeater" onclick="onChangeHeadMenu(25, 9, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('Universal Repeater'))"><span><script>document.write(_("Universal Repeater"))</script></span></a></li>
</span>

<span id = "menu_25_10_hightline" class = "off" >
        <li class="hightline"><a title="Site Survey"><span><script>document.write(_("Site Survey"))</script></span></a></li>
</span>    
<span id = "menu_25_10" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/apcli_wisp_ur_site_survey.asp" title="Site Survey" onclick="onChangeHeadMenu(25, 10, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('Site Survey'))"><span><script>document.write(_("Site Survey"))</script></span></a></li>
</span>

<!-- 5g WISP + UR-->
<span id = "menu_34_1_hightline" class = "off" >
        <li class="hightline"><a title="General"><span><script>document.write(_("General"))</script></span></a></li>
</span>    
<span id = "menu_34_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan_5g.asp" title="General" onclick="onChangeHeadMenu(34, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('General'))"><span><script>document.write(_("General"))</script></span></a></li>
</span>    

<span id = "menu_34_2_hightline" class = "off" >
        <li class="hightline"><a title="Security"><span><script>document.write(_("Security"))</script></span></a></li>
</span>    
<span id = "menu_34_2" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/security_5g.asp" title="Security" onclick="onChangeHeadMenu(34, 2, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('Security'))"><span><script>document.write(_("Security"))</script></span></a></li>
</span>

<span id = "menu_34_3_hightline" class = "off" >
        <li class="hightline"><a title="MAC Filter"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>    
<span id = "menu_34_3" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/mf_5g.asp" title="MAC Filter" onclick="onChangeHeadMenu(34, 3, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('MAC Filter'))"><span><script>document.write(_("MAC Filter"))</script></span></a></li>
</span>
<span id = "menu_34_4_hightline" class = "off" >
        <li class="hightline"><a title="Advanced"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span>    
<span id = "menu_34_4" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wlan_advance_5g.asp" title="Advanced" onclick="onChangeHeadMenu(34, 4, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('Advanced'))"><span><script>document.write(_("Advanced"))</script></span></a></li>
</span> 
<span id = "menu_34_5_hightline" class = "off" >
        <li class="hightline"><a title="QoS"><span><script>document.write(_("QoS"))</script></span></a></li>
</span>    
<span id = "menu_34_5" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/qos_5g.asp" title="QoS" onclick="onChangeHeadMenu(34, 5, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('QoS'))"><span><script>document.write(_("QoS"))</script></span></a></li>
</span> 
<span id = "menu_34_6_hightline" class = "off" >
        <li class="hightline"><a title="WPS"><span><script>document.write(_("WPS"))</script></span></a></li>
</span>    
<span id = "menu_34_6" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wps_5g.asp" title="WPS" onclick="onChangeHeadMenu(34, 6, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('WPS'))"><span><script>document.write(_("WPS"))</script></span></a></li>
</span> 
<span id = "menu_34_7_hightline" class = "off" >
        <li class="hightline"><a title="WPS Station"><span><script>document.write(_("WPS Station"))</script></span></a></li>
</span>    
<span id = "menu_34_7" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/wps_station_5g.asp" title="WPS Station" onclick="is_wps_init_page=1; onChangeHeadMenu(34, 7, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('WPS Station'))"><span><script>document.write(_("WPS Station"))</script></span></a></li>
</span> 
<span id = "menu_34_8_hightline" class = "off" >
        <li class="hightline"><a title="Scheduling"><span><script>document.write(_("Scheduling"))</script></span></a></li>
</span>    
<span id = "menu_34_8" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/scheduling_5g.asp" title="Scheduling" onclick="onChangeHeadMenu(34, 8, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('Scheduling'))"><span><script>document.write(_("Scheduling"))</script></span></a></li>
</span>
<span id = "menu_34_9_hightline" class = "off" >
        <li class="hightline"><a title="Universal Repeater"><span><script>document.write(_("Universal Repeater"))</script></span></a></li>
</span>    
<span id = "menu_34_9" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/apcli_wisp_5g.asp" title="Universal Repeater" onclick="onChangeHeadMenu(34, 9, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('Universal Repeater'))"><span><script>document.write(_("Universal Repeater"))</script></span></a></li>
</span>

<span id = "menu_34_10_hightline" class = "off" >
        <li class="hightline"><a title="Site Survey"><span><script>document.write(_("Site Survey"))</script></span></a></li>
</span>    
<span id = "menu_34_10" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/apcli_wisp_ur_site_survey_5g.asp" title="Site Survey" onclick="onChangeHeadMenu(34, 10, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 5G')+' > '+_('Site Survey'))"><span><script>document.write(_("Site Survey"))</script></span></a></li>
</span>

<!-- Remote Management -->    
<span id = "menu_35_1_hightline" class = "off" >
        <li class="hightline"><a title="WWW"><span><script>document.write(_("WWW"))</script></span></a></li>
</span>    
<span id = "menu_35_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/remote_web.asp" title="WWW" onclick="onChangeHeadMenu(35, 1, _('CONFIGURATION')+' > '+_('Management')+' > '+_('Remote Management')+' > '+_('WWW'))"><span><script>document.write(_("WWW"))</script></span></a></li>
</span>    
<span id = "menu_39_1_hightline" class = "off" >
        <li class="hightline"><a title="Service"><span><script>document.write(_("Service"))</script></span></a></li>
</span>    
<span id = "menu_39_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/service.asp" title="Service" onclick="onChangeHeadMenu(39, 1, _('CONFIGURATION')+' > '+_('Management')+' > '+_('Service')+' > '+_('Service'))"><span><script>document.write(_("Service"))</script></span></a></li>
</span>    
<span id = "menu_40_1_hightline" class = "off" >
        <li class="hightline"><a title="System Utilities"><span><script>document.write(_("System Utilities"))</script></span></a></li>
</span>    
<span id = "menu_40_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/ad_tools.asp" title="System Utilities" onclick="onChangeHeadMenu(40, 1, _('CONFIGURATION')+' > '+_('Management')+' > '+_('System Utilities')+' > '+_('System Utilities'))"><span><script>document.write(_("System Utilities"))</script></span></a></li>
</span>    

<!-- AP and Router Mode -->
<!--<span id = "menu_36_1_hightline" class = "off" >
        <li class="hightline"><a title="IPv6"><span><script>document.write(_("IPv6"))</script></span></a></li>
</span>
<span id = "menu_36_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/ipv6.asp" title="IPv6" onclick="onChangeHeadMenu(36, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('IPv6')+' > '+_('IPv6'))"><span><script>document.write(_("IPv6"))</script></span></a></li>
</span>-->
<!-- AP + Client -->
<!--<span id = "menu_37_1_hightline" class = "off" >
        <li class="hightline"><a title="IPv6"><span><script>document.write(_("IPv6"))</script></span></a></li>
</span>
<span id = "menu_37_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/ipv6.asp" title="IPv6" onclick="onChangeHeadMenu(37, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('IPv6')+' > '+_('IPv6'))"><span><script>document.write(_("IPv6"))</script></span></a></li>
</span>-->
<!-- WISP + UR -->
<!--<span id = "menu_38_1_hightline" class = "off" >
        <li class="hightline"><a title="IPv6"><span><script>document.write(_("IPv6"))</script></span></a></li>
</span>
<span id = "menu_38_1" class = "off" >
        <li><a target="InfoFrame" href="/local/advance/ipv6.asp" title="IPv6" onclick="onChangeHeadMenu(38, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('IPv6')+' > '+_('IPv6'))"><span><script>document.write(_("IPv6"))</script></span></a></li>
</span>-->

      </ul>
      <br class="clearfloat" />
    </div>
    <div id="table">
      <ul>
        <li class="table_top"></li>
        <li class="table_content"><br />
            <div class="data">
              <ul>
<table width="100%" border="0" cellspacing="0" cellpadding="0"><tr><td>
<script>
var current_url=location.href;
var current_url_len=current_url.length;
var index=current_url.charAt(current_url_len-1);
if (index==1)
document.write("<iframe id='myFrame' name='InfoFrame' src='/local/advance/wps.asp' align='LEFT' width='100%' height='100%' marginwidth='0' marginheight='0' frameborder='0' scrolling='auto' onload='adjustMyFrameHeight();'>");              
else
if (index==2)
document.write("<iframe id='myFrame' name='InfoFrame' src='/local/advance/upnp.asp' align='LEFT' width='100%' height='100%' marginwidth='0' marginheight='0' frameborder='0' scrolling='auto' onload='adjustMyFrameHeight();'>");              
else
{
if ((opmode==0) || (opmode==1)) // AP and Router Mode
	document.write("<iframe id='myFrame' name='InfoFrame' src='/local/advance/wlan.asp' align='LEFT' width='100%' height='100%' marginwidth='0' marginheight='0' frameborder='0' scrolling='auto' onload='adjustMyFrameHeight();'>");              
else if (opmode==2) // Client
	document.write("<iframe id='myFrame' name='InfoFrame' src='/local/advance/wisp.asp' align='LEFT' width='100%' height='100%' marginwidth='0' marginheight='0' frameborder='0' scrolling='auto' onload='adjustMyFrameHeight();'>");              
else if (opmode==6) // WISP Mode
{
	if (board_type == "0A52")
	{
	    if(wisp_mode == 0){
			document.write("<iframe id='myFrame' name='InfoFrame' src='/local/advance/apcli_wisp.asp' align='LEFT' width='100%' height='100%' marginwidth='0' marginheight='0' frameborder='0' scrolling='auto' onload='adjustMyFrameHeight();'>");
	    }else{
		document.write("<iframe id='myFrame' name='InfoFrame' src='/local/advance/apcli_wisp_5g.asp' align='LEFT' width='100%' height='100%' marginwidth='0' marginheight='0' frameborder='0' scrolling='auto' onload='adjustMyFrameHeight();'>");
	    }
	}
	else
		document.write("<iframe id='myFrame' name='InfoFrame' src='/local/advance/apcli_wisp.asp' align='LEFT' width='100%' height='100%' marginwidth='0' marginheight='0' frameborder='0' scrolling='auto' onload='adjustMyFrameHeight();'>");
}	
else if (opmode==3) // WDS
	document.write("<iframe id='myFrame' name='InfoFrame' src='/local/advance/wlan.asp' align='LEFT' width='100%' height='100%' marginwidth='0' marginheight='0' frameborder='0' scrolling='auto' onload='adjustMyFrameHeight();'>");              
else if (opmode==4) // AP + WDS
	document.write("<iframe id='myFrame' name='InfoFrame' src='/local/advance/wlan.asp' align='LEFT' width='100%' height='100%' marginwidth='0' marginheight='0' frameborder='0' scrolling='auto' onload='adjustMyFrameHeight();'>");              
else if (opmode==5) // AP + Client
	{
		if (board_type == "0A52" && bridge_mode == 0)
			document.write("<iframe id='myFrame' name='InfoFrame' src='/local/advance/ur_5g.asp' align='LEFT' width='100%' height='100%' marginwidth='0' marginheight='0' frameborder='0' scrolling='auto' onload='adjustMyFrameHeight();'>");
		else
			document.write("<iframe id='myFrame' name='InfoFrame' src='/local/advance/ur.asp' align='LEFT' width='100%' height='100%' marginwidth='0' marginheight='0' frameborder='0' scrolling='auto' onload='adjustMyFrameHeight();'>");
	}
else if (opmode==7) // WISP + UR
	document.write("<iframe id='myFrame' name='InfoFrame' src='/local/advance/wlan.asp' align='LEFT' width='100%' height='100%' marginwidth='0' marginheight='0' frameborder='0' scrolling='auto' onload='adjustMyFrameHeight();'>");            
}
</script>
</iframe>
</td></tr>
</table>
        </ul>
        </div>
        </li>
      </ul>
      <br class="clearfloat" />
    </div>
<br /><br /><br /><br />
  </div>
</div>
</div>
<div id="popup3_table">
  <table width="60%" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td><table width="100%" height="37" border="0" cellpadding="0" cellspacing="0">
      <tr>
        <td width="19" height="37"><img src="images/popup_top_left.gif" width="19" height="45" /></td>
        <td background="images/popup_top_center.gif" class="table_title" id=log_out>Log Out</td>
        <td width="15" background="images/popup_top_center.gif"><a href="#" class="iclose" onclick="MM_showHideLayers('popup3_table','','hide','popup','','hide')"></a></td>
        <td width="19" height="45"><img src="images/popup_top_right.gif" width="19" height="45" /></td>
      </tr>
    </table></td>
  </tr>
  <tr>
    <td><table width="100%" border="0" cellpadding="0" cellspacing="0">
      <tr>
        <td width="19" background="images/popup_main_left.gif">&nbsp;</td>
        <td bgcolor="#FFFFFF"><span class="popupcontent"> 
          <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
              <td width="27%"><center>
                <img src="images/i_info.jpg" width="50" height="49" />
              </center>
              </td>
              <td width="73%" id=ask_logout>Are you sure you want to log out?</td>
            </tr>
          </table>
         
        </span></td>
        <td width="19" background="images/popup_main_right.gif">&nbsp;</td>
      </tr>
    </table></td>
  </tr>
  <tr>
    <td><table width="100%" height="37" border="0" cellpadding="0" cellspacing="0">
      <tr>
        <td width="19" height="48"><img src="images/popup_down_left.gif" width="19" height="48" /></td>
        <td background="images/popup_down_center.gif"><div class="button3">
          <ul>   
    <li><a href="#" title="No"><span id="g2-2No" onclick="MM_showHideLayers('popup3_table','','hide','popup','','hide')">No</span></a></li>
	<li><a href="/logout.html" title="Yes"><span id="g2-2Yes">Yes</span></a></li>
	</ul>
        </div></td>
        <td width="19" height="37"><img src="images/popup_down_right.gif" width="19" height="48" /></td>
      </tr>
    </table></td>
  </tr>
</table></div>
<div id="popup4_table">
  <table width="60%" border="0" cellspacing="0" cellpadding="0">
    <tr>
      <td><table width="100%" height="37" border="0" cellpadding="0" cellspacing="0">
        <tr>
          <td width="19" height="37"><img src="images/popup_top_left.gif" width="19" height="45" /></td>
          <td background="images/popup_top_center.gif" class="table_title">About</td>
          <td width="15" background="images/popup_top_center.gif"><a href="#" class="iclose" onclick="MM_showHideLayers('popup','','hide','popup4_table','','hide')"></a></td>
          <td width="19" height="45"><img src="images/popup_top_right.gif" width="19" height="45" /></td>
        </tr>
      </table></td>
    </tr>
    <tr>
      <td><table width="100%" border="0" cellpadding="0" cellspacing="0">
        <tr>
          <td width="19" background="images/popup_main_left.gif">&nbsp;</td>
          <td bgcolor="#FFFFFF"><span class="popupcontent">
            <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
              <tr>
                <td width="27%"><center>
                  <img src="images/i_info.jpg" width="50" height="49" />
                </center></td>
                <td width="73%"><span class="blod"><% getCfgGeneral(1, "SystemName"); %></span> <br />
                  (C) Copyright by Lite-On Communications Corp.<br />
                  <br />
                  <font id="dashAboutCheck1"></font> <a href="http://www.liteon.com" target="_blank">www.liteon.com</a> <font id="dashAboutCheck2"></font></td>
              </tr>
              <tr>
                <td height="20" colspan="2"><center>
                </center></td>
              </tr>
            </table>
          </span></td>
          <td width="19" background="images/popup_main_right.gif">&nbsp;</td>
        </tr>
      </table></td>
    </tr>
    <tr>
      <td><table width="100%" height="37" border="0" cellpadding="0" cellspacing="0">
        <tr>
          <td width="19" height="48"><img src="images/popup_down_left.gif" width="19" height="48" /></td>
          <td background="images/popup_down_center.gif"><div class="button3">
            <ul>
              <li><a href="#" title="Close"><span onclick="MM_showHideLayers('popup','','hide','popup4_table','','hide')"><font id="dashAboutClose"></font></span></a></li>
            </ul>
          </div></td>
          <td width="19" height="37"><img src="images/popup_down_right.gif" width="19" height="48" /></td>
        </tr>
      </table></td>
    </tr>
  </table>
</div>
<div id="popup"></div>
</body>
<script>
var current_url=location.href;
var current_url_len=current_url.length;
var index=current_url.charAt(current_url_len-1);
if ((opmode==0) || (opmode==1)) // AP and Router Mode
	if(board_type == "0A22")
	{
		current_menu_id=44;
	}
	else
	{
		current_menu_id=20;
	}
else if ((opmode==2) || (opmode==6)) // Client and WISP Mode
{
	if(opmode == 6)
	{
		if(board_type == "0A52")
		{
			if (wisp_mode == 0)
				current_menu_id=21;
			else
				current_menu_id=27;
		}
		else if(board_type == "0A22")
		{
			current_menu_id=17;
		}
		else
		{
			current_menu_id=18;
		}
	}
	else
	{
	   if(wisp_mode==0){
	      current_menu_id=21;
	   }else{
	      current_menu_id=20;
	   }
   }
}
else if (opmode==3) // WDS
	current_menu_id=22;
else if (opmode==4) // AP + WDS
	current_menu_id=23;
else if (opmode==5) // AP + Client
{
	if (bridge_mode == 0)
	{
		if( board_type == "0A52")
			current_menu_id=55;
		else if (board_type == "0A22")
			current_menu_id=54;
		else
			current_menu_id=24;
	}
	else
	{
		if( board_type == "0A52")
			current_menu_id=56;
		else
			current_menu_id=54;
	}
}
else if (opmode==7) // WISP+UR
	current_menu_id=25;
var current_submenu_index=1;
if (index==1)
{
if ((opmode==0) || (opmode==1)) // AP and Router Mode
	current_menu_id=20;
else if ((opmode==2) || (opmode==6)) // Client and WISP Mode
{      
   if(wisp_mode==0){
      current_menu_id=21;
   }else{
      current_menu_id=20;
   }
}
else if (opmode==3) // WDS
	current_menu_id=22;
else if (opmode==4) // AP + WDS
	current_menu_id=23;
else if (opmode==5) // AP + Client
	current_menu_id=24;
else if (opmode==7) // WISP + UR
	current_menu_id=25;
current_submenu_index=5;
onChangeHeadMenu(current_menu_id, current_submenu_index, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('WPS'))
d.s(2);
}
else
if (index==2)
{
current_menu_id=8;
current_submenu_index=1;
onChangeHeadMenu(current_menu_id, current_submenu_index, _('CONFIGURATION')+' > '+_('Management')+' > '+_('UPnP')+' > '+_('General'))
d.s(21);
}
else
{
	if(opmode == 1)
	{
		if(board_type == "0A22")
		{
			onChangeHeadMenu(current_menu_id, current_submenu_index, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G-1')+' > '+_('DownLink'));
		}
		else
		{
			onChangeHeadMenu(current_menu_id, current_submenu_index, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN 2.4G')+' > '+_('DownLink'));
		}
	}
	else
	{
		onChangeHeadMenu(current_menu_id, current_submenu_index, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('UpLink'));
	}
	d.s(2);
}

function onResizeWindow()
{
adjustMyFrameHeight();
}
</script>	
<script>
function alert(str)
{
	showWebMessage(1, str);
}
showWebMessage(<% getWebMessageFlag(); %>, _("<% getWebMessage(); %>"));
</script> 
</html>
