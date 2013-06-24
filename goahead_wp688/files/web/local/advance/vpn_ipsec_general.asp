<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html;charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">

<title>.::Welcome to <% getCfgGeneral(1, 'SystemName'); %>::.</title>

<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
 
<script type="text/javascript" src="/lang/b28n.js"></script>
<script type="text/javascript" src="images/common.js"></script>
<script type="text/javascript" src="images/code.js"></script>
<script type="text/javascript" src="images/svg-common.js"></script>

<script language="JavaScript" type="text/javascript">

Butterlate.setTextDomain("firewall");
var t;
 
var ipsec_objects =
[ { id: 'ipsec_table_add' },
  { id: 'ipsec_table_delete' },
  { id: 'ipsec_table_modify' },
  { id: 'ipsec_table_element' }
];

function formCheck()
{
	showWebMessage(2, "");
	return true;
}

function init() {
    F = document.getElementById("vpn_ipsec");
/*
    var enable = (document.getElementsByName("ipsec_enable")[0].checked == true) ? 1 : 0;

    sel_change(enable, 1, F, ipsec_objects);
    if (enable == 1) {
        //tbl_disable_row_edit('ipsec_table');
    }
	*/
}
</script>
<body onload="init();">
  <form name="vpn_ipsec" action="/goform/vpn_ipsec_general" method="post">
    <div id="table">
      <ul>
        <li class="table_content">
          <div class="data">
            <ul>
              <li class="title" id="securityTitle">
                IPsec
              </li>
              <span class='on'>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
				  <input type="radio" value="1" name="ipsec_enable" id='ipsec_enable'  <%nvg_match("ipsec_enable","1","checked");%>><%lang("Enable");%>
				  </input>

				  <input type="radio" value="0" name="ipsec_enable" id='ipsec_enable' <%nvg_match("ipsec_enable","0","checked");%>><%lang("Disable");%>
				  </input>

				  </table>
				  </li>
				</span>

                <li></li>
                <li></li>

            </ul>
          </div>
          </li>
    <center>
      <li class="table_button">
	    <input type="submit" value="Apply" id="natApply" name="add" onClick="return formCheck()"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <input type="reset" value="Cancel" id="" name="reset" onClick="window.location.reload()">
        <input type=hidden value="/local/advance/vpn_ipsec_general.asp" name="ipsec_gen_url">
      </li>
    </center>
      </ul>
    </div>
  </form>
</body>
<script>
  function alert(str) {
    showWebMessage(1, str);
  }
  showWebMessage( <%getWebMessageFlag(); %>, _("<% getWebMessage(); %>"));
</script>

</html>
