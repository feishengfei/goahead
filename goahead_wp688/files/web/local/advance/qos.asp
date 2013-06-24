<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Expires" content="-1">
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<meta http-equiv="Pragma" content="no-cache">
<script type="text/javascript" src="/lang/b28n.js"></script>
<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />

<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("wireless");

var wirelessmode  = <% getCfgZero(1, "OP_Mode"); %>;
var wispmode  = <% getCfgZero(1, "wisp_mode"); %>;
function isNum(str)
{
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9'))
			continue;
		return 0;
	}
	return 1;
}
function checkOutOfRange(str)
{
	if(str.length>=3)
	     return  1;
 	if(str.length==2) {
		   if(str.charAt(0) >= '7')
		   return 1;
		   if(str.charAt(0) == '6' && str.charAt(1) >= '4')
		   return 1;
	}
	return 0;
}
function isEqual(str1,str2)
{
    if((str1 == "") && (str2 == ""))
	   return 0;
    if(str1 == str2)
	{
	   return 1;
	}
	else
	   return 0;
}

function selectIndexisEqual(d1,d2)
{
    if(d1==d2)
	{
	   return 1;
	}
	else
	{
	   return 0;
	}
}
function check_dscp_value()
{
if(!isNum(document.wireless_advanced.DSCP_value_1.value)||!isNum(document.wireless_advanced.DSCP_value_2.value)||!isNum(document.wireless_advanced.DSCP_value_3.value)||!isNum(document.wireless_advanced.DSCP_value_4.value)||!isNum(document.wireless_advanced.DSCP_value_5.value)||!isNum(document.wireless_advanced.DSCP_value_6.value)||!isNum(document.wireless_advanced.DSCP_value_7.value)||!isNum(document.wireless_advanced.DSCP_value_8.value))
{
 	alert("Input the value of DSCP in decimal!");
		return false;
}


if(checkOutOfRange(document.wireless_advanced.DSCP_value_1.value)||checkOutOfRange(document.wireless_advanced.DSCP_value_2.value)||checkOutOfRange(document.wireless_advanced.DSCP_value_3.value)||checkOutOfRange(document.wireless_advanced.DSCP_value_4.value)||checkOutOfRange(document.wireless_advanced.DSCP_value_5.value)||checkOutOfRange(document.wireless_advanced.DSCP_value_6.value)||checkOutOfRange(document.wireless_advanced.DSCP_value_7.value)||checkOutOfRange(document.wireless_advanced.DSCP_value_8.value))
{
    	alert("The value of DSCP is greater than 63!");
		return false;
}
 
 if(isEqual(document.wireless_advanced.DSCP_value_1.value,document.wireless_advanced.DSCP_value_2.value)||isEqual(document.wireless_advanced.DSCP_value_1.value,document.wireless_advanced.DSCP_value_3.value)||isEqual(document.wireless_advanced.DSCP_value_1.value,document.wireless_advanced.DSCP_value_4.value)||isEqual(document.wireless_advanced.DSCP_value_1.value,document.wireless_advanced.DSCP_value_5.value)||isEqual(document.wireless_advanced.DSCP_value_1.value,document.wireless_advanced.DSCP_value_6.value)||isEqual(document.wireless_advanced.DSCP_value_1.value,document.wireless_advanced.DSCP_value_7.value)||isEqual(document.wireless_advanced.DSCP_value_1.value,document.wireless_advanced.DSCP_value_8.value)||isEqual(document.wireless_advanced.DSCP_value_2.value,document.wireless_advanced.DSCP_value_3.value)||isEqual(document.wireless_advanced.DSCP_value_2.value,document.wireless_advanced.DSCP_value_4.value)||isEqual(document.wireless_advanced.DSCP_value_2.value,document.wireless_advanced.DSCP_value_5.value)||isEqual(document.wireless_advanced.DSCP_value_2.value,document.wireless_advanced.DSCP_value_6.value)||isEqual(document.wireless_advanced.DSCP_value_2.value,document.wireless_advanced.DSCP_value_7.value)||isEqual(document.wireless_advanced.DSCP_value_2.value,document.wireless_advanced.DSCP_value_8.value)||isEqual(document.wireless_advanced.DSCP_value_3.value,document.wireless_advanced.DSCP_value_4.value)||isEqual(document.wireless_advanced.DSCP_value_3.value,document.wireless_advanced.DSCP_value_5.value)||isEqual(document.wireless_advanced.DSCP_value_3.value,document.wireless_advanced.DSCP_value_6.value)||isEqual(document.wireless_advanced.DSCP_value_3.value,document.wireless_advanced.DSCP_value_7.value)||isEqual(document.wireless_advanced.DSCP_value_3.value,document.wireless_advanced.DSCP_value_8.value)||isEqual(document.wireless_advanced.DSCP_value_4.value,document.wireless_advanced.DSCP_value_5.value)||isEqual(document.wireless_advanced.DSCP_value_4.value,document.wireless_advanced.DSCP_value_6.value)||isEqual(document.wireless_advanced.DSCP_value_4.value,document.wireless_advanced.DSCP_value_7.value)||isEqual(document.wireless_advanced.DSCP_value_4.value,document.wireless_advanced.DSCP_value_8.value)||isEqual(document.wireless_advanced.DSCP_value_5.value,document.wireless_advanced.DSCP_value_6.value)||isEqual(document.wireless_advanced.DSCP_value_5.value,document.wireless_advanced.DSCP_value_7.value)||isEqual(document.wireless_advanced.DSCP_value_5.value,document.wireless_advanced.DSCP_value_7.value)||isEqual(document.wireless_advanced.DSCP_value_6.value,document.wireless_advanced.DSCP_value_7.value)||isEqual(document.wireless_advanced.DSCP_value_6.value,document.wireless_advanced.DSCP_value_8.value)||isEqual(document.wireless_advanced.DSCP_value_7.value,document.wireless_advanced.DSCP_value_8.value))
 {
 	alert("Input is invalid!");
		return false;
 }
if((selectIndexisEqual(document.wireless_advanced.DSCP_Priority_Select_9.selectedIndex,document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex) && (document.wireless_advanced.DSCP_value_1.value!=""))||(selectIndexisEqual(document.wireless_advanced.DSCP_Priority_Select_9.selectedIndex,document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex) && (document.wireless_advanced.DSCP_value_2.value!=""))||(selectIndexisEqual(document.wireless_advanced.DSCP_Priority_Select_9.selectedIndex,document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex) && (document.wireless_advanced.DSCP_value_3.value!=""))||(selectIndexisEqual(document.wireless_advanced.DSCP_Priority_Select_9.selectedIndex,document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex) && (document.wireless_advanced.DSCP_value_4.value!=""))||(selectIndexisEqual(document.wireless_advanced.DSCP_Priority_Select_9.selectedIndex,document.wireless_advanced.DSCP_Priority_Select_5.selectedIndex) && (document.wireless_advanced.DSCP_value_5.value!=""))||(selectIndexisEqual(document.wireless_advanced.DSCP_Priority_Select_9.selectedIndex,document.wireless_advanced.DSCP_Priority_Select_6.selectedIndex) && (document.wireless_advanced.DSCP_value_6.value!=""))||(selectIndexisEqual(document.wireless_advanced.DSCP_Priority_Select_9.selectedIndex,document.wireless_advanced.DSCP_Priority_Select_7.selectedIndex) && (document.wireless_advanced.DSCP_value_7.value!=""))(selectIndexisEqual(document.wireless_advanced.DSCP_Priority_Select_9.selectedIndex,document.wireless_advanced.DSCP_Priority_Select_8.selectedIndex) && (document.wireless_advanced.DSCP_value_8.value!="")))
{
  	alert("Input is invalid!");
		return false;
}
}
function submit_apply()
{
    if(check_dscp_value()==false)
	    return false;
	//if ((wirelessmode == 2) || ((wirelessmode == 6) && (wispmode == 0))){
	//alert("You CANNOT select WISP or Client mode to configure this page");
	//	return false;
	//}
	showWebMessage(2, "");
	return true;

}

function initTranslation()
{
	var e = document.getElementById("QoSTitle");
	e.innerHTML = _("qos qostitle");

	e = document.getElementById("QoSWMMEnable");
	e.innerHTML = _("qos wmm enable");
	
	e = document.getElementById("GeneralApply");
	e.value = _("general apply");
	
	e = document.getElementById("GeneralReset");
	e.value = _("general reset");
}

function initValue()
{
	initTranslation();

	var WMMEnable  = <% getCfgZero(1, "WmmCapable"); %>;
	
	if (WMMEnable == 1)
		document.wireless_advanced.wmm_capable.checked = true;
	else
		document.wireless_advanced.wmm_capable.checked = false;

	//if ((wirelessmode == 2) || ((wirelessmode == 6) && (wispmode == 0))){
	//    alert("You CANNOT select WISP or Client mode to configure this page");
	//}
	var   dscpmode = "<% getCfgGeneral(1, "dscp_mode"); %>";
	var   dscpother = "<% getCfgGeneral(1, "dscp_other"); %>";
	
	if(dscpmode == "1")
	{
	    document.wireless_advanced.DSCP_mode_Select.selectedIndex=1;
	}
	else
	{
	document.wireless_advanced.DSCP_mode_Select.selectedIndex=0;
    document.wireless_advanced.DSCP_value_1.value= "56";
	document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=2;
	document.wireless_advanced.DSCP_value_1.disabled=true;
	document.wireless_advanced.DSCP_Priority_Select_1.disabled=true;
	document.wireless_advanced.DSCP_value_2.value= "48";
	document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=2;
	document.wireless_advanced.DSCP_value_2.disabled=true;
	document.wireless_advanced.DSCP_Priority_Select_2.disabled=true;
	document.wireless_advanced.DSCP_value_3.value= "46";
	document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=2;
	document.wireless_advanced.DSCP_value_3.disabled=true;
	document.wireless_advanced.DSCP_Priority_Select_3.disabled=true;
    document.wireless_advanced.DSCP_value_4.value= "40";
	document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=1;
	document.wireless_advanced.DSCP_value_4.disabled=true;
	document.wireless_advanced.DSCP_Priority_Select_4.disabled=true;
	document.wireless_advanced.DSCP_value_5.value= "32";
	document.wireless_advanced.DSCP_Priority_Select_5.selectedIndex=1;
	document.wireless_advanced.DSCP_value_5.disabled=true;
	document.wireless_advanced.DSCP_Priority_Select_5.disabled=true;
	document.wireless_advanced.DSCP_value_6.value= "";
	document.wireless_advanced.DSCP_Priority_Select_6.selectedIndex=0;
	document.wireless_advanced.DSCP_value_6.disabled=true;
	document.wireless_advanced.DSCP_Priority_Select_6.disabled=true;
    document.wireless_advanced.DSCP_value_7.value= "";
	document.wireless_advanced.DSCP_Priority_Select_7.selectedIndex=0;
	document.wireless_advanced.DSCP_value_7.disabled=true;
	document.wireless_advanced.DSCP_Priority_Select_7.disabled=true;
    document.wireless_advanced.DSCP_value_8.value= "";
	document.wireless_advanced.DSCP_Priority_Select_8.selectedIndex=0;
	document.wireless_advanced.DSCP_value_8.disabled=true;
	document.wireless_advanced.DSCP_Priority_Select_8.disabled=true;
	document.wireless_advanced.DSCP_Priority_Select_9.selectedIndex=0;
	document.wireless_advanced.DSCP_Priority_Select_9.disabled=true;
		return true;
	}
	
    if(dscpother == "2")
	{
	  document.wireless_advanced.DSCP_Priority_Select_9.selectedIndex=2;
	}
	else if(dscpother == "1")
	{
	  document.wireless_advanced.DSCP_Priority_Select_9.selectedIndex=1;
	}
	else
	{
	  document.wireless_advanced.DSCP_Priority_Select_9.selectedIndex=0;
	}
	
	var   tosvoice = "<% getCfgGeneral(1, "tos_voice"); %>";
    var   tosvideo = "<% getCfgGeneral(1, "tos_video"); %>";
	var   tosdata  = "<% getCfgGeneral(1, "tos_data"); %>";

	var name;
	var index;
	var length_total;
	var array_tmp=new Array();
	var array_tmp_1=new Array();
	var array_tmp_2=new Array();
	
	array_tmp.length=0;
	array_tmp_1.length=0;
	array_tmp_2.length=0;
	
	if(isEqual(tosvideo,"none"))
	{
	  tosvideo="";
	}
	if(isEqual(tosvoice,"none"))
	{
	  tosvoice="";	  
	}
	if(tosvideo!="")
	{
	 array_tmp_1 = tosvideo.split(",");
	}
		
	if(tosvoice!="")
	{ 
	 array_tmp = tosvoice.split(",");	 
	
	}
	if(tosdata!="")
	{ 
	 array_tmp_2 = tosdata.split(",");	 
	
	}
    if(dscpother=="2")
	{
	  if(tosvideo!="" && tosdata!="")
	  {
	    length_total=array_tmp_1.length+array_tmp_2.length;
	  }
	  if(tosvideo=="" && tosdata!="")
	  {
	    length_total=array_tmp_2.length;
	  }
	  if(tosvideo!="" && tosdata=="")
	  {
	    length_total=array_tmp_1.length;
	  }
	  if(tosvideo=="" && tosdata=="")
	  {
	    length_total=0;
	  }
	  {
	  if(8 == length_total)
	  {
	    for(var i=0;i<8;i++)
		{
		    index = i+1;
		   
			if(i<array_tmp_1.length)
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_1[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=2;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp_1[i]/4);
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=2;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp_1[i]/4);
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=2;
			   }
			   else if(4==index)
			   {
			     document.wireless_advanced.DSCP_value_4.value= parseInt(array_tmp_1[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=2;
			   }
			   else if(5==index)
			   {
			     document.wireless_advanced.DSCP_value_5.value= parseInt(array_tmp_1[i]/4);
				 document.wireless_advanced.DSCP_Priority_Select_5.selectedIndex=2;
			   }
			   else if(6==index)
			   {
			     document.wireless_advanced.DSCP_value_6.value= parseInt(array_tmp_1[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_6.selectedIndex=2;
			   }
			   else if(7==index)
			   {
			     document.wireless_advanced.DSCP_value_7.value= parseInt(array_tmp_1[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_7.selectedIndex=2;
			   }
			   else if(8==index)
			   {
			     document.wireless_advanced.DSCP_value_8.value= parseInt(array_tmp_1[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_8.selectedIndex=2;
			   }
			}
			else
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=1;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=1;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=1;
			   }
			   else if(4==index)
			   {
			     document.wireless_advanced.DSCP_value_4.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=1;
			   }
			   else if(5==index)
			   {
			     document.wireless_advanced.DSCP_value_5.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_5.selectedIndex=1;
			   }
			   else if(6==index)
			   {
			     document.wireless_advanced.DSCP_value_6.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_6.selectedIndex=1;
			   }
			   else if(7==index)
			   {
			     document.wireless_advanced.DSCP_value_7.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_7.selectedIndex=1;
			   }
			   else if(8==index)
			   {
			     document.wireless_advanced.DSCP_value_8.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_8.selectedIndex=1;
			   }
			}
		}
	  }
	  if(7 == length_total)
	  {
	    for(var i=0;i<7;i++)
		{
		    index = i+1;
		   
			if(i<array_tmp_1.length)
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_1[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=2;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp_1[i]/4);
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=2;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp_1[i]/4);
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=2;
			   }
			   else if(4==index)
			   {
			     document.wireless_advanced.DSCP_value_4.value= parseInt(array_tmp_1[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=2;
			   }
			   else if(5==index)
			   {
			     document.wireless_advanced.DSCP_value_5.value= parseInt(array_tmp_1[i]/4);
				 document.wireless_advanced.DSCP_Priority_Select_5.selectedIndex=2;
			   }
			   else if(6==index)
			   {
			     document.wireless_advanced.DSCP_value_6.value= parseInt(array_tmp_1[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_6.selectedIndex=2;
			   }
			   else if(7==index)
			   {
			     document.wireless_advanced.DSCP_value_7.value= parseInt(array_tmp_1[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_7.selectedIndex=2;
			   }
			}
			else
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=1;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=1;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=1;
			   }
			   else if(4==index)
			   {
			     document.wireless_advanced.DSCP_value_4.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=1;
			   }
			   else if(5==index)
			   {
			     document.wireless_advanced.DSCP_value_5.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_5.selectedIndex=1;
			   }
			   else if(6==index)
			   {
			     document.wireless_advanced.DSCP_value_6.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_6.selectedIndex=1;
			   }
			   else if(7==index)
			   {
			     document.wireless_advanced.DSCP_value_7.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_7.selectedIndex=1;
			   }
			}
		}
	  }
	  if(6 == length_total)
	  {
	    for(var i=0;i<6;i++)
		{
		    index = i+1;
		   
             if(i<array_tmp_1.length)
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_1[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=2;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp_1[i]/4);
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=2;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp_1[i]/4);
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=2;
			   }
			   else if(4==index)
			   {
			     document.wireless_advanced.DSCP_value_4.value= parseInt(array_tmp_1[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=2;
			   }
			   else if(5==index)
			   {
			     document.wireless_advanced.DSCP_value_5.value= parseInt(array_tmp_1[i]/4);
				 document.wireless_advanced.DSCP_Priority_Select_5.selectedIndex=2;
			   }
			   else if(6==index)
			   {
			     document.wireless_advanced.DSCP_value_6.value= parseInt(array_tmp_1[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_6.selectedIndex=2;
			   }
			}
			else
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=1;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=1;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=1;
			   }
			   else if(4==index)
			   {
			     document.wireless_advanced.DSCP_value_4.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=1;
			   }
			   else if(5==index)
			   {
			     document.wireless_advanced.DSCP_value_5.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_5.selectedIndex=1;
			   }
			   else if(6==index)
			   {
			     document.wireless_advanced.DSCP_value_6.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_6.selectedIndex=1;
			   }
			}
		}
	  }
	  if(5 == length_total)
	  {
	    for(var i=0;i<5;i++)
		{
		    index = i+1;
		   
		    if(i<array_tmp_1.length)
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_1[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=2;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp_1[i]/4);
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=2;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp_1[i]/4);
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=2;
			   }
			   else if(4==index)
			   {
			     document.wireless_advanced.DSCP_value_4.value= parseInt(array_tmp_1[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=2;
			   }
			   else if(5==index)
			   {
			     document.wireless_advanced.DSCP_value_5.value= parseInt(array_tmp_1[i]/4);
				 document.wireless_advanced.DSCP_Priority_Select_5.selectedIndex=2;
			   }
			}
			else
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=1;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=1;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=1;
			   }
			   else if(4==index)
			   {
			     document.wireless_advanced.DSCP_value_4.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=1;
			   }
			   else if(5==index)
			   {
			     document.wireless_advanced.DSCP_value_5.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_5.selectedIndex=1;
			   }
			}
		}
	  }
	  if(4 == length_total)
	  {
	    for(var i=0;i<4;i++)
		{
		    index = i+1;
		   
            if(i<array_tmp_1.length)
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_1[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=2;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp_1[i]/4);
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=2;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp_1[i]/4);
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=2;
			   }
			   else if(4==index)
			   {
			     document.wireless_advanced.DSCP_value_4.value= parseInt(array_tmp_1[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=2;
			   }
			}
			else
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=1;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=1;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=1;
			   }
			   else if(4==index)
			   {
			     document.wireless_advanced.DSCP_value_4.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=1;
			   }
			}
		}
	  }

	  if(3 == length_total)
	  {
	    for(var i=0;i<3;i++)
		{
		    index = i+1;
		   
            if(i<array_tmp_1.length)
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_1[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=2;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp_1[i]/4);
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=2;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp_1[i]/4);
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=2;
			   }
			}
			else
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=1;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=1;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=1;
			   }
			}
		}
	  }

	  if(2 == length_total)
	  {
	    
		
	    for(var i=0;i<2;i++)
		{
		    index = i+1;
		   
            if(i<array_tmp_1.length)
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_1[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=2;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp_1[i]/4);
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=2;
			   }
			}
			else
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=1;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=1;
			   }
			}
		}
	  }

      if(1 == length_total)
	  {
	    for(var i=0;i<1;i++)
		{
		    index = i+1;
		   
           	if(i<array_tmp_1.length)
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_1[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=2;
			   }
			}
			else
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_2[i-array_tmp_1.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=1;
			   }
			}
		}
	  }

	}
	}
	else if(dscpother=="1")
	{
	  if(tosvoide!="" && tosdata!="")
	  {
	    length_total=array_tmp.length+array_tmp_2.length;
	  }
	  if(tosvoice=="" && tosdata!="")
	  {
	    length_total=array_tmp_2.length;
	  }
	  if(tosvoice!="" && tosdata=="")
	  {
	    length_total=array_tmp.length;
	  }
	  if(tosvoice=="" && tosdata=="")
	  {
	    length_total=0;
	  }
	  {
	  if(8 == length_total)
	  {
	    for(var i=0;i<8;i++)
		{
		    index = i+1;
		   
			if(i<array_tmp.length)
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=2;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp[i]/4);
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=2;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp[i]/4);
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=2;
			   }
			   else if(4==index)
			   {
			     document.wireless_advanced.DSCP_value_4.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=2;
			   }
			   else if(5==index)
			   {
			     document.wireless_advanced.DSCP_value_5.value= parseInt(array_tmp[i]/4);
				 document.wireless_advanced.DSCP_Priority_Select_5.selectedIndex=2;
			   }
			   else if(6==index)
			   {
			     document.wireless_advanced.DSCP_value_6.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_6.selectedIndex=2;
			   }
			   else if(7==index)
			   {
			     document.wireless_advanced.DSCP_value_7.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_7.selectedIndex=2;
			   }
			   else if(8==index)
			   {
			     document.wireless_advanced.DSCP_value_8.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_8.selectedIndex=2;
			   }
			}
			else
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=1;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=1;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=1;
			   }
			   else if(4==index)
			   {
			     document.wireless_advanced.DSCP_value_4.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=1;
			   }
			   else if(5==index)
			   {
			     document.wireless_advanced.DSCP_value_5.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_5.selectedIndex=1;
			   }
			   else if(6==index)
			   {
			     document.wireless_advanced.DSCP_value_6.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_6.selectedIndex=1;
			   }
			   else if(7==index)
			   {
			     document.wireless_advanced.DSCP_value_7.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_7.selectedIndex=1;
			   }
			   else if(8==index)
			   {
			     document.wireless_advanced.DSCP_value_8.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_8.selectedIndex=1;
			   }
			}
		}
	  }
	  if(7 == length_total)
	  {
	    for(var i=0;i<7;i++)
		{
		    index = i+1;
		   
			if(i<array_tmp.length)
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=2;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp[i]/4);
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=2;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp[i]/4);
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=2;
			   }
			   else if(4==index)
			   {
			     document.wireless_advanced.DSCP_value_4.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=2;
			   }
			   else if(5==index)
			   {
			     document.wireless_advanced.DSCP_value_5.value= parseInt(array_tmp[i]/4);
				 document.wireless_advanced.DSCP_Priority_Select_5.selectedIndex=2;
			   }
			   else if(6==index)
			   {
			     document.wireless_advanced.DSCP_value_6.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_6.selectedIndex=2;
			   }
			   else if(7==index)
			   {
			     document.wireless_advanced.DSCP_value_7.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_7.selectedIndex=2;
			   }
			}
			else
			{
			    if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=1;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=1;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=1;
			   }
			   else if(4==index)
			   {
			     document.wireless_advanced.DSCP_value_4.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=1;
			   }
			   else if(5==index)
			   {
			     document.wireless_advanced.DSCP_value_5.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_5.selectedIndex=1;
			   }
			   else if(6==index)
			   {
			     document.wireless_advanced.DSCP_value_6.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_6.selectedIndex=1;
			   }
			   else if(7==index)
			   {
			     document.wireless_advanced.DSCP_value_7.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_7.selectedIndex=1;
			   }
			}
		}
	  }
	  if(6 == length_total)
	  {
	    for(var i=0;i<6;i++)
		{
		    index = i+1;
		   
            if(i<array_tmp.length)
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=2;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=2;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=2;
			   }
			   else if(4==index)
			   {
			     document.wireless_advanced.DSCP_value_4.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=2;
			   }
			   else if(5==index)
			   {
			     document.wireless_advanced.DSCP_value_5.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_5.selectedIndex=2;
			   }
			   else if(6==index)
			   {
			     document.wireless_advanced.DSCP_value_6.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_6.selectedIndex=2;
			   }
			}
			else
			{
			 if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=1;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=1;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=1;
			   }
			   else if(4==index)
			   {
			     document.wireless_advanced.DSCP_value_4.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=1;
			   }
			   else if(5==index)
			   {
			     document.wireless_advanced.DSCP_value_5.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_5.selectedIndex=1;
			   }
			   else if(6==index)
			   {
			     document.wireless_advanced.DSCP_value_6.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_6.selectedIndex=1;
			   }
			}
		}
	  }
	  if(5 == length_total)
	  {
	    for(var i=0;i<5;i++)
		{
		    index = i+1;
		   
			if(i<array_tmp.length)
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=2;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=2;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=2;
			   }
			   else if(4==index)
			   {
			     document.wireless_advanced.DSCP_value_4.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=2;
			   }
			   else if(5==index)
			   {
			     document.wireless_advanced.DSCP_value_5.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_5.selectedIndex=2;
			   }
			}
			else
			{
               if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=1;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=1;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=1;
			   }
			   else if(4==index)
			   {
			     document.wireless_advanced.DSCP_value_4.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=1;
			   }
			   else if(5==index)
			   {
			     document.wireless_advanced.DSCP_value_5.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_5.selectedIndex=1;
			   }
			}
		}
	  }
	  if(4 == length_total)
	  {
	    for(var i=0;i<4;i++)
		{
		    index = i+1;
		   
            if(i<array_tmp.length)
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=2;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=2;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=2;
			   }
			   else if(4==index)
			   {
			     document.wireless_advanced.DSCP_value_4.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=2;
			   }
			}
			else
			{
               if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=1;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=1;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=1;
			   }
			   else if(4==index)
			   {
			     document.wireless_advanced.DSCP_value_4.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=1;
			   }
			}
		}
	  }

	  if(3 == length_total)
	  {
	    for(var i=0;i<3;i++)
		{
		    index = i+1;
		   
            if(i<array_tmp.length)
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=2;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=2;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=2;
			   }
			}
			else
			{
               if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=1;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=1;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=1;
			   }
			}
		}
	  }

	  if(2 == length_total)
	  {
	    
		
	    for(var i=0;i<2;i++)
		{
		    index = i+1;
		   
            if(i<array_tmp.length)
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=2;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=2;
			   }

			}
			else
			{
               if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=1;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=1;
			   }
			}
		}
	  }

      if(1 == length_total)
	  {
	    for(var i=0;i<1;i++)
		{
		    index = i+1;
		   
           if(i<array_tmp.length)
			{
			   if(1==index)
			   {
			     
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=2;
			   }
			}
			else
			{
               if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_2[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=1;
			   }
			}			
		}
	  }

	}
	}
	else
	{
	  if(tosvideo!="" && tosvoice!="")
	  {
	    length_total=array_tmp.length+array_tmp_1.length;
	  }
	  	if(tosvideo=="" && tosvoice!="")
	  {
	    length_total=array_tmp.length;
	  }
	  if(tosvideo!="" && tosvoice=="")
	  {
	    length_total=array_tmp_1.length;
	  }
	  if(tosvideo=="" && tosvoice=="")
	  {
	    length_total=0;
	  }
	  {
	  if(8 == length_total)
	  {
	    for(var i=0;i<8;i++)
		{
		    index = i+1;
		   
			if(i<array_tmp.length)
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=2;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp[i]/4);
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=2;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp[i]/4);
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=2;
			   }
			   else if(4==index)
			   {
			     document.wireless_advanced.DSCP_value_4.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=2;
			   }
			   else if(5==index)
			   {
			     document.wireless_advanced.DSCP_value_5.value= parseInt(array_tmp[i]/4);
				 document.wireless_advanced.DSCP_Priority_Select_5.selectedIndex=2;
			   }
			   else if(6==index)
			   {
			     document.wireless_advanced.DSCP_value_6.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_6.selectedIndex=2;
			   }
			   else if(7==index)
			   {
			     document.wireless_advanced.DSCP_value_7.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_7.selectedIndex=2;
			   }
			   else if(8==index)
			   {
			     document.wireless_advanced.DSCP_value_8.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_8.selectedIndex=2;
			   }
			}
			else
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=1;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=1;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=1;
			   }
			   else if(4==index)
			   {
			     document.wireless_advanced.DSCP_value_4.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=1;
			   }
			   else if(5==index)
			   {
			     document.wireless_advanced.DSCP_value_5.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_5.selectedIndex=1;
			   }
			   else if(6==index)
			   {
			     document.wireless_advanced.DSCP_value_6.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_6.selectedIndex=1;
			   }
			   else if(7==index)
			   {
			     document.wireless_advanced.DSCP_value_7.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_7.selectedIndex=1;
			   }
			   else if(8==index)
			   {
			     document.wireless_advanced.DSCP_value_8.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_8.selectedIndex=1;
			   }
			}
		}
	  }
	  if(7 == length_total)
	  {
	    for(var i=0;i<7;i++)
		{
		    index = i+1;
		   
			if(i<array_tmp.length)
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=2;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp[i]/4);
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=2;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp[i]/4);
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=2;
			   }
			   else if(4==index)
			   {
			     document.wireless_advanced.DSCP_value_4.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=2;
			   }
			   else if(5==index)
			   {
			     document.wireless_advanced.DSCP_value_5.value= parseInt(array_tmp[i]/4);
				 document.wireless_advanced.DSCP_Priority_Select_5.selectedIndex=2;
			   }
			   else if(6==index)
			   {
			     document.wireless_advanced.DSCP_value_6.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_6.selectedIndex=2;
			   }
			   else if(7==index)
			   {
			     document.wireless_advanced.DSCP_value_7.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_7.selectedIndex=2;
			   }
			}
			else
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=1;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=1;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=1;
			   }
			   else if(4==index)
			   {
			     document.wireless_advanced.DSCP_value_4.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=1;
			   }
			   else if(5==index)
			   {
			     document.wireless_advanced.DSCP_value_5.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_5.selectedIndex=1;
			   }
			   else if(6==index)
			   {
			     document.wireless_advanced.DSCP_value_6.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_6.selectedIndex=1;
			   }
			   else if(7==index)
			   {
			     document.wireless_advanced.DSCP_value_7.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_7.selectedIndex=1;
			   }
			}
		}
	  }
	  if(6 == length_total)
	  {
	    for(var i=0;i<6;i++)
		{
		    index = i+1;
		   
            if(i<array_tmp.length)
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=2;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=2;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=2;
			   }
			   else if(4==index)
			   {
			     document.wireless_advanced.DSCP_value_4.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=2;
			   }
			   else if(5==index)
			   {
			     document.wireless_advanced.DSCP_value_5.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_5.selectedIndex=2;
			   }
			   else if(6==index)
			   {
			     document.wireless_advanced.DSCP_value_6.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_6.selectedIndex=2;
			   }
			}
			else
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=1;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=1;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=1;
			   }
			   else if(4==index)
			   {
			     document.wireless_advanced.DSCP_value_4.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=1;
			   }
			   else if(5==index)
			   {
			     document.wireless_advanced.DSCP_value_5.value= parseInt(array_tmp_1[i-array_tmp.length]/4);
				 document.wireless_advanced.DSCP_Priority_Select_5.selectedIndex=1;
			   }
			   else if(6==index)
			   {
			     document.wireless_advanced.DSCP_value_6.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_6.selectedIndex=1;
			   }
			}
		}
	  }
	  if(5 == length_total)
	  {
	    for(var i=0;i<5;i++)
		{
		    index = i+1;
		   
			if(i<array_tmp.length)
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=2;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=2;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=2;
			   }
			   else if(4==index)
			   {
			     document.wireless_advanced.DSCP_value_4.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=2;
			   }
			   else if(5==index)
			   {
			     document.wireless_advanced.DSCP_value_5.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_5.selectedIndex=2;
			   }
			}
			else
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=1;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=1;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=1;
			   }
			   else if(4==index)
			   {
			     document.wireless_advanced.DSCP_value_4.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=1;
			   }
			   else if(5==index)
			   {
			     document.wireless_advanced.DSCP_value_5.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_5.selectedIndex=1;
			   }
			}
		}
	  }
	  if(4 == length_total)
	  {
	    for(var i=0;i<4;i++)
		{
		    index = i+1;
		   
            if(i<array_tmp.length)
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=2;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=2;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=2;
			   }
			   else if(4==index)
			   {
			     document.wireless_advanced.DSCP_value_4.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=2;
			   }
			}
			else
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=1;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=1;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=1;
			   }
			   else if(4==index)
			   {
			     document.wireless_advanced.DSCP_value_4.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=1;
			   }
			   
			}
		}
	  }

	  if(3 == length_total)
	  {
	    for(var i=0;i<3;i++)
		{
		    index = i+1;
		   
            if(i<array_tmp.length)
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=2;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=2;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=2;
			   }
			}
			else
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=1;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=1;
			   }
			   else if(3==index)
			   {
			     document.wireless_advanced.DSCP_value_3.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=1;
			   }
			}
		}
	  }

	  if(2 == length_total)
	  {
	    
		
	    for(var i=0;i<2;i++)
		{
		    index = i+1;
		   
            if(i<array_tmp.length)
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=2;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=2;
			   }

			}
			else
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=1;
			   }
			   else if(2==index)
			   {
			     document.wireless_advanced.DSCP_value_2.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=1;
			   }
			}
		}
	  }

      if(1 == length_total)
	  {
	    for(var i=0;i<1;i++)
		{
		    index = i+1;
		   
            if(i<array_tmp.length)
			{
			   if(1==index)
			   {
			     
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp[i])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=2;
			   }
			}
			else
			{
			   if(1==index)
			   {
			     document.wireless_advanced.DSCP_value_1.value= parseInt(array_tmp_1[i-array_tmp.length])/4;
				 document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=1;
			   }
			}			
		}
	  }

	}
	}
	
	if(tosvideo=="" && tosvoice=="" && tosdata=="")
	{
	 length_total= 0;
	 return false;
	}
		
}

function dscp_mode_select_change()
{
  if(document.wireless_advanced.DSCP_mode_Select.selectedIndex==0)
  {
    document.wireless_advanced.DSCP_value_1.value= "56";
	document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=2;
	document.wireless_advanced.DSCP_value_1.disabled=true;
	document.wireless_advanced.DSCP_Priority_Select_1.disabled=true;
	document.wireless_advanced.DSCP_value_2.value= "48";
	document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=2;
	document.wireless_advanced.DSCP_value_2.disabled=true;
	document.wireless_advanced.DSCP_Priority_Select_2.disabled=true;
	document.wireless_advanced.DSCP_value_3.value= "46";
	document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=2;
	document.wireless_advanced.DSCP_value_3.disabled=true;
	document.wireless_advanced.DSCP_Priority_Select_3.disabled=true;
    document.wireless_advanced.DSCP_value_4.value= "40";
	document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=1;
	document.wireless_advanced.DSCP_value_4.disabled=true;
	document.wireless_advanced.DSCP_Priority_Select_4.disabled=true;
	document.wireless_advanced.DSCP_value_5.value= "32";
	document.wireless_advanced.DSCP_Priority_Select_5.selectedIndex=1;
	document.wireless_advanced.DSCP_value_5.disabled=true;
	document.wireless_advanced.DSCP_Priority_Select_5.disabled=true;
	document.wireless_advanced.DSCP_value_6.value= "";
	document.wireless_advanced.DSCP_Priority_Select_6.selectedIndex=0;
	document.wireless_advanced.DSCP_value_6.disabled=true;
	document.wireless_advanced.DSCP_Priority_Select_6.disabled=true;
    document.wireless_advanced.DSCP_value_7.value= "";
	document.wireless_advanced.DSCP_Priority_Select_7.selectedIndex=0;
	document.wireless_advanced.DSCP_value_7.disabled=true;
	document.wireless_advanced.DSCP_Priority_Select_7.disabled=true;
    document.wireless_advanced.DSCP_value_8.value= "";
	document.wireless_advanced.DSCP_Priority_Select_8.selectedIndex=0;
	document.wireless_advanced.DSCP_value_8.disabled=true;
	document.wireless_advanced.DSCP_Priority_Select_8.disabled=true;
	document.wireless_advanced.DSCP_Priority_Select_9.selectedIndex=0;
	document.wireless_advanced.DSCP_Priority_Select_9.disabled=true;
  }
  else
  {
    document.wireless_advanced.DSCP_value_1.value= "";
	document.wireless_advanced.DSCP_Priority_Select_1.selectedIndex=0;
	document.wireless_advanced.DSCP_value_1.disabled=false;
	document.wireless_advanced.DSCP_Priority_Select_1.disabled=false;
	document.wireless_advanced.DSCP_value_2.value= "";
	document.wireless_advanced.DSCP_Priority_Select_2.selectedIndex=0;
	document.wireless_advanced.DSCP_value_2.disabled=false;
	document.wireless_advanced.DSCP_Priority_Select_2.disabled=false;
	document.wireless_advanced.DSCP_value_3.value= "";
	document.wireless_advanced.DSCP_Priority_Select_3.selectedIndex=0;
	document.wireless_advanced.DSCP_value_3.disabled=false;
	document.wireless_advanced.DSCP_Priority_Select_3.disabled=false;
    document.wireless_advanced.DSCP_value_4.value= "";
	document.wireless_advanced.DSCP_Priority_Select_4.selectedIndex=0;
	document.wireless_advanced.DSCP_value_4.disabled=false;
	document.wireless_advanced.DSCP_Priority_Select_4.disabled=false;
	document.wireless_advanced.DSCP_value_5.value= "";
	document.wireless_advanced.DSCP_Priority_Select_5.selectedIndex=0;
	document.wireless_advanced.DSCP_value_5.disabled=false;
	document.wireless_advanced.DSCP_Priority_Select_5.disabled=false;
	document.wireless_advanced.DSCP_value_6.value= "";
	document.wireless_advanced.DSCP_Priority_Select_6.selectedIndex=0;
	document.wireless_advanced.DSCP_value_6.disabled=false;
	document.wireless_advanced.DSCP_Priority_Select_6.disabled=false;
    document.wireless_advanced.DSCP_value_7.value= "";
	document.wireless_advanced.DSCP_Priority_Select_7.selectedIndex=0;
	document.wireless_advanced.DSCP_value_7.disabled=false;
	document.wireless_advanced.DSCP_Priority_Select_7.disabled=false;
    document.wireless_advanced.DSCP_value_8.value= "";
	document.wireless_advanced.DSCP_Priority_Select_8.selectedIndex=0;
    document.wireless_advanced.DSCP_value_8.disabled=false;
	document.wireless_advanced.DSCP_Priority_Select_8.disabled=false;
	document.wireless_advanced.DSCP_Priority_Select_9.selectedIndex=0;
	document.wireless_advanced.DSCP_Priority_Select_9.disabled=false;
  }
  
}
</script>
</head>
<body onLoad="initValue()">
<form method=post name="wireless_advanced" action="/goform/WiFiWMM" onSubmit="return submit_apply()">
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul>
<li class="title" id="QoSTitle"> Qos Configuration </li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><input name="wmm_capable" type="checkbox" value="1" /><input name="AAA" type="hidden" value=1/>
<font id ="QoSWMMEnable">Enable QoS</font>
</td>
</tr>
</table>
</li>
<li class="w_text2">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><table width="100%" cellpadding="0" cellspacing="0" >
<tr>
<td height="26" colspan="3"><table width="100%" border="0" cellspacing="0" cellpadding="0">
<tr>
<td width="25%" valign="top"><center>
<span class="top_left">
<table background="images/table_top_center.gif" width="100%" border="0" cellpadding="0">
<td align="left"><font id ="DSCPConfiguration"> DSCP Configuration </font></td>
<td align="right" class="table_title">
<select name="DSCP_mode_Select" size="1"  onchange='dscp_mode_select_change()'>
  <option value=0 >default </option>
  <option value=1 >custom</option>
</select></td>
</table>
 </span>
</center></td>
</tr>
</table></td>
</tr>

<tr>
<td width="25%" valign="top"><center>
<span class="top_left" id ="DSCPValue">DSCP Value(0..63) </span>
</center></td>
<td width="55%"><center>
<span class="top_right" id ="DSCPPriority">Priority </span>
</center></td>
</tr>

<tr >
<td valign="top"><center>
<span class="table_left">
<input type=text maxlength="15" size="15" value="" name="DSCP_value_1" /> 
</center></td>

<td ><center>
<span class="table_right">
<select name="DSCP_Priority_Select_1" size="1" onchange="">
<option value=00 >data </option>
<option value=01 >video </option>
<option value=02 >voice </option>
</select>
</span>
</center></td>
</tr>

<tr >
<td valign="top"><center>
<span class="table_left">
<input type=text maxlength="15" size="15" value="" name="DSCP_value_2" /> 
</center></td>
<td><center>
<span class="table_right">
<select name="DSCP_Priority_Select_2" size="1" onchange="">
<option value=00 >data </option>
<option value=01 >video </option>
<option value=02 >voice </option>
</select>
</span>
</center></td>
</tr>
<tr >
<td valign="top"><center>
<span class="table_left">
<input type=text maxlength="15" size="15" value="" name="DSCP_value_3" /> 
</center></td>

<td><center>
<span class="table_right">
<select name="DSCP_Priority_Select_3" size="1" onchange="">
<option value=00 >data </option>
<option value=01 >video </option>
<option value=02 >voice </option>
</select>
</span>
</center></td>
</tr>

<tr >
<td valign="top"><center>
<span class="table_left">
<input type=text maxlength="15" size="15" value="" name="DSCP_value_4" /> 
</center></td>

<td><center>
<span class="table_right">
<select name="DSCP_Priority_Select_4" size="1" onchange="">
<option value=00 >data </option>
<option value=01 >video </option>
<option value=02 >voice </option>
</select>
</span>
</center></td>
</tr>

<tr >
<td valign="top"><center>
<span class="table_left">
<input type=text maxlength="15" size="15" value="" name="DSCP_value_5" /> 
</center></td>

<td><center>
<span class="table_right">
<select name="DSCP_Priority_Select_5" size="1" onchange="">
<option value=00 >data </option>
<option value=01 >video </option>
<option value=02 >voice </option>
</select>
</span>
</center></td>
</tr>

<tr >
<td valign="top"><center>
<span class="table_left">
<input type=text maxlength="15" size="15" value="" name="DSCP_value_6" /> 
</center></td>

<td><center>
<span class="table_right">
<select name="DSCP_Priority_Select_6" size="1" onchange="">
<option value=00 >data </option>
<option value=01 >video </option>
<option value=02 >voice </option>
</select>
</span>
</center></td>
</tr>

<tr >
<td valign="top"><center>
<span class="table_left">
<input type=text maxlength="15" size="15" value="" name="DSCP_value_7" /> 
</center></td>

<td><center>
<span class="table_right">
<select name="DSCP_Priority_Select_7" size="1" onchange="">
<option value=00 >data </option>
<option value=01 >video </option>
<option value=02 >voice </option>
</select>
</span>
</center></td>
</tr>

<tr >
<td valign="top"><center>
<span class="table_left">
<input type=text maxlength="15" size="15" value="" name="DSCP_value_8" /> 
</center></td>

<td><center>
<span class="table_right">
<select name="DSCP_Priority_Select_8" size="1" onchange="">
<option value=00 >data </option>
<option value=01 >video </option>
<option value=02 >voice </option>
</select>
</span>
</center></td>
</tr>

<tr >
<td valign="top"><center>
<span class="table_left">
All others
</center></td>

<td><center>
<span class="table_right">
<select name="DSCP_Priority_Select_9" size="1" onchange="">
<option value=00 >data </option>
<option value=01 >video </option>
<option value=02 >voice </option>
</select>
</span>
</center></td>
</tr>


<tr>
<td height="5" colspan="3"><table width="100%" height="5" border="0" cellpadding="0" cellspacing="0">
<tr>
<td width="5" height="5"><img src="images/table_bottom_left.gif" width="5" height="5" /></td>
<td height="5" background="images/table_bottom_center.gif"><img src="images/table_bottom_center.gif" width="1" height="5" /></td>
<td width="5"><img src="images/table_bottom_right.gif" width="5" height="5" /></td>
</tr>
</table></td>
</tr>
</table></td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><span class="i_note" id ="SchedulingNote">Note: 1 Input the value of DSCP in decimal. 2 Configuration will affect others enabled radio.</span></td>
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
<input type=submit value="Apply" id="GeneralApply">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input type=reset  value="Reset" id="GeneralReset" onClick="window.location.reload()">
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
