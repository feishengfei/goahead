function getSelectSelected(selectbox)
{       
	return selectbox.options[selectbox.selectedIndex].value;
} 

function removeAllFromTheList(list){
	while(list.length>0)
		list.options[0] = null;
	return true;
}

function addToTheList(list,str,selected){
	var currentLength = list.length;
	list.length++;
	list.options[currentLength] = new Option(str, str);

	return list.options[currentLength].selected=selected;
}

function addValueTextToList(list,optValue,optText,optSelected){
	var currentLength = list.length;
	list.length++;
	list.options[currentLength] = new Option(optText,optValue);

	return list.options[currentLength].selected=optSelected;
}

function SetChannelIdx(value)
{
	var i;
	var channel_table;

	switch(i=getSelectSelected(document.wireless_basic.wirelessmode)){
		case '1':
		case '4':
		case '6':
			break;
		case '9'://11b/g/n
			channel_table = channelBGN;
			break;
		default:
			alert("Error: Unknown Wireless Mode ("+i+")");
			return false;
	}


	for(i = 0; i < channel_table.length - 1; i++)
	{
		if(channel_table[i][2] == value)
		{
			channel_table[i][1] = "SELECTED";
			updateChannel();

			return true;
		}
	}
	return false;
}

function useExtChan()
{
	/* Can not set HT20 HT40 */
	if(document.wireless_basic.n_bandwidth.disabled == true)
	{  
		document.wireless_basic.n_extcha.disabled = true;
		return false;
	}

	/* HT40 selected */
	if(eval(document.wireless_basic.n_bandwidth.value) == 1)
	{  
		document.wireless_basic.n_extcha.disabled = false;
		return true;
	}
	else
	{  
		document.wireless_basic.n_extcha.disabled = true;
		return false;
	}
}

function refreshExtensionChanList(chanTab, selChan, ext_u, ext_l){
	var ret=false;
	var idx=0;
	removeAllFromTheList(document.wireless_basic.n_extcha);

	/* if SmartSelect selected, no extension channels */
	if(eval(selChan) == 0)
		return false;

	for(idx=0; idx<chanTab.length-1; idx++){
		if(eval(ext_u) && ((eval(selChan) - 4) == eval(chanTab[idx][2])))
		{  
			addValueTextToList(document.wireless_basic.n_extcha, "-1", chanTab[idx][0], chanTab[idx][1]);
			ret = true;
			continue;
		}
		if(eval(ext_l) && ((eval(selChan) + 4) == eval(chanTab[idx][2])))
		{  
			addValueTextToList(document.wireless_basic.n_extcha, "1", chanTab[idx][0], chanTab[idx][1]);
			ret = true;
			break;
		}
	}
	return ret;
}

function refreshChanList(channel)
{
	var been_selected=false;
	removeAllFromTheList(document.wireless_basic.sz11gChannel);

	for(i=0;i<channel.length-1;i++){
		if(!(eval(document.wireless_basic.n_bandwidth.value) == 1) || (eval(channel[i][3]) == 1 || eval(channel[i][4]) == 1))
		{
			tmp=addValueTextToList(document.wireless_basic.sz11gChannel,channel[i][2], channel[i][0],channel[i][1]=="SELECTED");
			been_selected=been_selected||tmp;
		}
	}
	if(!been_selected)document.wireless_basic.sz11gChannel.options[document.wireless_basic.sz11gChannel.length-1].selected=true;
}

function refreshExtChannel()
{
	var i;
	var channel;
	var select_channel;
	var chan_idx = 0;

	/* enable / disable extension channel according to radion band selection */
	if(useExtChan() != true)
	{
		removeAllFromTheList(document.wireless_basic.n_extcha);
		return false;
	}

	switch(i=getSelectSelected(document.wireless_basic.wirelessmode)){
		case '1':
		case '4':
		case '6':
			break;
		case '9':
			channel = channelBGN;
			break;
		default:
			alert("Error: Unknown Wireless Mode ("+i+")");
			return false;
	}
	
	select_channel = eval(document.wireless_basic.sz11gChannel.selectedIndex);

	for(i = 0; i < channel.length - 1; i++)
	{
		if(!(eval(document.wireless_basic.n_bandwidth.value) == 1) || (eval(channel[i][3]) == 1 || eval(channel[i][4]) == 1))
			chan_idx++;

		if(chan_idx == select_channel + 1)
			break;
	}

	/* refresh extension channel list */
	if(chan_idx == select_channel + 1)
	{
		if(refreshExtensionChanList(channel, channel[i][2], channel[i][3], channel[i][4]) != true)
			document.wireless_basic.n_extcha.disabled = true;
	}
	else /* channel SmartSelect in HT40 */
	{
		removeAllFromTheList(document.wireless_basic.n_extcha);
		document.wireless_basic.n_extcha.disabled = true;
	}

	return true;
}


function updateChannel(){
	tmpWMDisabled=document.wireless_basic.wirelessmode.disabled;
	tmpCHDisabled=document.wireless_basic.sz11gChannel.disabled;
	document.wireless_basic.wirelessmode.disabled=true;
	document.wireless_basic.sz11gChannel.disabled=true;


	switch(i=getSelectSelected(document.wireless_basic.wirelessmode)){
		case '1':
		case '4':
		case '6':
			break;
		case '9'://11b/g/n
			refreshChanList(channelBGN);
			break;
		default:
			alert("Error: Unknown Wireless Mode ("+i+")");
			return false;
	}
	document.wireless_basic.wirelessmode.disabled=tmpWMDisabled;
	document.wireless_basic.sz11gChannel.disabled=tmpCHDisabled;
}
