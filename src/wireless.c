/* vi: set sw=4 ts=4 sts=4: */
/*
 *	wireless.c -- Wireless Settings
 *
 *	Copyright (c) Ralink Technology Corporation All Rights Reserved.
 *
 *	$Id: wireless.c,v 1.91.2.4 2008-07-18 06:14:16 yy Exp $
 */

#if 1 //Steve10
#include	<sys/ioctl.h>
#include	<arpa/inet.h>
//#include	"../../autoconf.h"
#ifdef CONFIG_DEFAULTS_KERNEL_2_6_21
  #include	<linux/types.h>
  #include	<linux/socket.h>
  #include	<linux/if.h>
#endif
#include	<linux/wireless.h>

#include	"internet.h"
//#include	"nvram.h"
#include	"utils.h"
#include	"webs.h"
#include	"wireless.h"
//#include	"oid.h"
#include "nvram.h"
#include "nvram_rule.h"
//#include	"linux/autoconf.h"

// Tommy, Add syslog, 2009/10/21 04:47
#include <syslog.h>

// Tommy, decrease code size , 2009/3/17 01:28
#define Include_Ralink_Original 0
#define TMP_LEN 256 //from httpd common.h
// Tommy, 2009/2/16 10:53
int WLAN_Scheduler(int setting);
int WLAN_EZSecurity(int setting);
int getRadioStatus(void);
void WPSRestart(void); // Aaron 2009/8/13 move to here!

#define SIOCDEVPRIVATE				0x8BE0
#define SIOCIWFIRSTPRIV				SIOCDEVPRIVATE
#define RTPRIV_IOCTL_GET_MAC_TABLE		(SIOCIWFIRSTPRIV + 0x0F)
#define RTPRIV_IOCTL_GET_RADAR_STATUS		(SIOCIWFIRSTPRIV + 0x0B)

#define CHANNEL_MAX_2G 32
#define MAC_ADDR_LENGTH 18
#define STATIONLIST_MAX_2G 32

typedef struct Channel_s
{
	unsigned char chan_number;
	unsigned int frequency;
}Channel_t;

typedef struct ChannelListInfo_s
{
	int num;
	Channel_t channel_list[CHANNEL_MAX_2G];
}ChannelListInfo_t;

typedef struct StationInfo_s
{
	unsigned char Addr[MAC_ADDR_LENGTH];
	unsigned int  ConnectedTime;
}StationInfo_t;

typedef struct StationListInfo_s
{
	int sta_num;
	StationInfo_t sta_list[STATIONLIST_MAX_2G];
}StationListInfo_t;

/*
 * RT2860, RTINIC, RT2561
 */
static int default_shown_mbssid[3]  = {0,0,0};

extern int g_wsc_configured;
extern int g1_wsc_configured;

// Tommy, 2009/1/8 10:28
extern void create_crond_file(void);
// Tommy, 2009/3/13 09:57
//extern void check_scheduler(void);
static void wifiAPGeneral(webs_t wp, char_t *path, char_t *query);
static void wifiAPGeneral5G(webs_t wp, char_t *path, char_t *query);
static int  getWlan11aChannels(int eid, webs_t wp, int argc, char_t **argv);
static int  getWlan11bChannels(int eid, webs_t wp, int argc, char_t **argv);
static int  getWlan11gChannels(int eid, webs_t wp, int argc, char_t **argv);
// Tommy, 2008/12/26 03:14下午
static int  getWlan11gChannelsFreq(int eid, webs_t wp, int argc, char_t **argv);

static int  getGeneralChannelsFreq(int eid, webs_t wp, int argc, char_t **argv);
static int  getWlan11gExtChannelsFreq(int eid, webs_t wp, int argc, char_t **argv);
static int  getWlan11aExtChannelsFreq(int eid, webs_t wp, int argc, char_t **argv);

static int  getWlanApcliBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int  getWlanChannel(int eid, webs_t wp, int argc, char_t **argv);
static int  getWlanCurrentMac(int eid, webs_t wp, int argc, char_t **argv);
static int  getWlanStaInfo(int eid, webs_t wp, int argc, char_t **argv);

static int getmacrepeaterStaInfo(int eid, webs_t wp, int argc, char_t **argv);

// Tommy, 2009/1/21 05:42
static int  getwifiStaInfo(int eid, webs_t wp, int argc, char_t **argv);
static int  getwifi5GStaInfo(int eid, webs_t wp, int argc, char_t **argv);
static int  getwifi5GRadarStatus(int eid, webs_t wp, int argc, char_t **argv);
static int  getDLSBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int  getWlanM2UBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int get_channel(char channel[], char str[], char pattern[]);
// Tommy, Patch WDS for WEP, 2009/3/26 02:38
#if 0
static int  getWlanWdsEncType(int eid, webs_t wp, int argc, char_t **argv);
#endif
// Tommy, decrease code size , 2009/3/17 01:28
#if Include_Ralink_Original
static void wirelessBasic(webs_t wp, char_t *path, char_t *query);
static void wirelessAdvanced(webs_t wp, char_t *path, char_t *query);
// Tommy, Patch WDS for WEP, 2009/3/26 02:38
static void wirelessWds(webs_t wp, char_t *path, char_t *query);
#endif // #if Include_Ralink_Original

// Tommy, 2008/12/11 02:02下午
static void wifiAdvanced(webs_t wp, char_t *path, char_t *query);
static void wifiGeneral(webs_t wp, char_t *path, char_t *query);
static void wifiWDS(webs_t wp, char_t *path, char_t *query);
static void wifiApcli(webs_t wp, char_t *path, char_t *query);
static int getwifiBSSIDList(int eid, webs_t wp, int argc, char_t **argv);
static int getwifiRSSI(int eid, webs_t wp, int argc, char_t **argv);
static int getwifi5GBSSIDList(int eid, webs_t wp, int argc, char_t **argv);
static void wispwifiApcli(webs_t wp, char_t *path, char_t *query);
static void urwifiApcli(webs_t wp, char_t *path, char_t *query);
static void urwifi5GApcli(webs_t wp, char_t *path, char_t *query);
static int  getRadioStatusASP(int eid, webs_t wp, int argc, char_t **argv);
static int  getRadioStatusASP5G(int eid, webs_t wp, int argc, char_t **argv);
static int  getAPClientConnectionESSID(int eid, webs_t wp, int argc, char_t **argv);
static int getConnectedAPMode(int eid, webs_t wp, int argc, char_t **argv);
static int  getAPClientConnectionStatus(int eid, webs_t wp, int argc, char_t **argv);
static int  getAPClientConnectionESSIDMAC(int eid, webs_t wp, int argc, char_t **argv);
static int  getAPClient5GConnectionESSID(int eid, webs_t wp, int argc, char_t **argv);
static int  getAPClient5GConnectionStatus(int eid, webs_t wp, int argc, char_t **argv);
static int  getAPClient5GConnectionESSIDMAC(int eid, webs_t wp, int argc, char_t **argv);

static void wirelessApcli(webs_t wp, char_t *path, char_t *query);
static void wirelessWmm(webs_t wp, char_t *path, char_t *query);
static void wirelessGetSecurity(webs_t wp, char_t *path, char_t *query);
static void wireless5gGetSecurity(webs_t wp, char_t *path, char_t *query);
static void GetWDSSecurity(webs_t wp, char_t *path, char_t *query);
static void Get5GWDSSecurity(webs_t wp, char_t *path, char_t *query);
static void wifiAPSecurity(webs_t wp, char_t *path, char_t *query);
static void wifi5GAPSecurity(webs_t wp, char_t * path, char_t * query);
// Passphrase to WEP
#if 1
static void wifiget128wepkey(webs_t wp, char_t *path, char_t *query);
static void wifiget40wepkey(webs_t wp, char_t *path, char_t *query);
#endif
// Tommy, 2009/1/21 05:48
static void EZwifiAPSecurity(webs_t wp, char_t *path, char_t *query);
static void EZAPSecurity(webs_t wp, char_t *path, char_t *query);
static void wifiAssocList(webs_t wp, char_t *path, char_t *query);
// Tommy, decrease code size , 2009/3/17 01:28
#if Include_Ralink_Original
static void APSecurity(webs_t wp, char_t *path, char_t *query);
#endif // #if Include_Ralink_Original
static void MultiFilterSecurity(webs_t wp, char_t *path, char_t *query);
static void MultiFilter5GSecurity(webs_t wp, char_t *path, char_t *query);
static int  isWPSConfiguredASP(int eid, webs_t wp, int argc, char_t **argv);
static int  isAntennaDiversityBuilt(int eid, webs_t wp, int argc, char_t **argv);
// Tommy, WLAN Scheduling, 2008/12/9 05:02下午
static int  getOpChannleASP(int eid, webs_t wp, int argc, char_t **argv);
static int  getOpChannleASP5G(int eid, webs_t wp, int argc, char_t **argv);
static int  getOpChannleNum(int eid, webs_t wp, int argc, char_t **argv);
static int  getOpChannleNum5G(int eid, webs_t wp, int argc, char_t **argv);
static void wifischeduler(webs_t wp, char_t *path, char_t *query);
static void wifi5gscheduler(webs_t wp, char_t *path, char_t *query);
static void EZwifischeduler(webs_t wp, char_t *path, char_t *query);
static void WiFiWMM(webs_t wp, char_t *path, char_t *query);
static void WiFi5GWMM(webs_t wp, char_t *path, char_t *query);

#ifdef CONFIG_RT2860V2_AP_ANTENNA_DIVERSITY
static void AntennaDiversity(webs_t wp, char_t *path, char_t *query);
static void getAntenna(webs_t wp, char_t *path, char_t *query);
#endif

int deleteNthValueMulti(int index[], int count, char *value, char delimit);		/* for Access Policy list deletion*/
static void APDeleteAccessPolicyList(webs_t wp, char_t *path, char_t *query);
//void DeleteAccessPolicyList(int nvram, webs_t wp, char_t *path, char_t *query);
void DeleteAccessPolicyList(webs_t wp, char_t *path, char_t *query);

static void APDelete5GAccessPolicyList(webs_t wp, char_t *path, char_t *query);
//void DeleteAccessPolicyList(int nvram, webs_t wp, char_t *path, char_t *query);
void Delete5GAccessPolicyList(webs_t wp, char_t *path, char_t *query);

// Tommy
/*
 * description:  Re-configure Wireless Setting, use this procedure to replace initInternet().
 *		 Accelerated wireless Apply action
 */
void reconfig_wireless(int nvram)
{
#if 0	
	int mbssid_num = atoi(nvram_bufget(nvram, "BssidNum"));
	char *operation_mode = nvram_bufget(RT2860_NVRAM, "OP_Mode");	
	char *wdsMode = nvram_bufget(RT2860_NVRAM, "WdsEnable");
	//char *Radio_OFF = nvram_bufget(RT2860_NVRAM, "RadioOff");
	int i;

	if (nvram == RT2860_NVRAM){
#if 1	
		system("ralink_init make_wireless_config rt2860");
		/* ... Down All Interfaces ... */
		// Down all ra0...7 interfaces
		for (i = 0 ; i < mbssid_num; i++){
	     		doSystem("ifconfig ra%d down",i);
		}	     

		// Down apcli0 interface
		if (!strcmp(operation_mode, "5")){
	   		system("ifconfig apcli0 down");		
		}

		// Down all wsd0...3 interfaces
// Tommy, 2009/6/1 		
		//if (strcmp(wdsMode, "0")){
	    		for (i = 0 ; i < 4; i++){
	         		doSystem("ifconfig wds%d down",i);
	    		}     
		//}
		
		/* ... Up All Interfaces ... */
		// Up all ra0...7 interfaces
		for (i = 0 ; i < mbssid_num; i++){
	     		doSystem("ifconfig ra%d up",i);
		}	     
		
		// Up apcli0 interface
		if (!strcmp(operation_mode, "5")){
	   		system("ifconfig apcli0 up");		
		}

		// Up all wsd0...3 interfaces
		if (strcmp(wdsMode, "0")){
	    		for (i = 0 ; i < 4; i++){
	         		doSystem("ifconfig wds%d up",i);
	    		}     
		}
#else
// Original Ralink's patch code
		for(i=0; i<mbssid_num; i++){
			doSystem("ifconfig ra%d down", i);
	       	}
        	for(i=0; i<mbssid_num; i++){			
			doSystem("ifconfig ra%d up", i);
		}
#endif
		WPSRestart();
	}else if(nvram == RTINIC_NVRAM){
		system("ralink_init make_wireless_config inic");
	
		for(i=0; i<mbssid_num; i++){
			doSystem("ifconfig rai%d down", i);
	       	}
	       	
        	for(i=0; i<mbssid_num; i++){			
			doSystem("ifconfig rai%d up", i);
		}
		// WPSRestart();
	} else if(nvram == RT2561_NVRAM) {
		system("ralink_init make_wireless_config rt2561");

		for(i=0; i<mbssid_num; i++){
			doSystem("ifconfig raL%d down", i);
	       	}
        	for(i=0; i<mbssid_num; i++){			
			doSystem("ifconfig raL%d up", i);
		}
		// WPSRestart();
	}else
		printf("*** Unknown interface.\n");	
	
	// Restore WLAN status
	if (getRadioStatus() == 0){
		//doSystem("ifconfig ra0 down");
		system("iwpriv ra0 set RadioOn=0");
	}else{
		//doSystem("ifconfig ra0 up");
		system("iwpriv ra0 set RadioOn=1");
	}
#endif
}

/*
 * description: write 802.11a channels in <select> tag
 */
static int getWlan11aChannels(int eid, webs_t wp, int argc, char_t **argv)
{
#if 0
	int  idx = 0, channel;
	char *value = nvram_bufget(RT2860_NVRAM,"CountryCode");
	char *channel_s = nvram_bufget(RT2860_NVRAM, "Channel");

	channel = (channel_s == NULL)? 0 : atoi(channel_s);
	if ((value == NULL) || (strcmp(value, "") == 0) ||
			(strcmp(value, "US") == 0) || (strcmp(value, "FR") == 0) ||
			(strcmp(value, "IE") == 0) || (strcmp(value, "JP") == 0) ||
			(strcmp(value, "HK") == 0)) {
		for (idx = 0; idx < 4; idx++)
			websWrite(wp, T("%s%d %s>%d%s%d%s"), "<option value=", 36+4*idx,
					(36+4*idx == channel)? "selected" : "", 5180+20*idx,
					"MHz (Channel ", 36+4*idx, ")</option>");
	}
	if ((value == NULL) || (strcmp(value, "") == 0) ||
			(strcmp(value, "US") == 0) || (strcmp(value, "FR") == 0) ||
			(strcmp(value, "IE") == 0) || (strcmp(value, "TW") == 0) ||
			(strcmp(value, "HK") == 0)) {
		for (idx = 4; idx < 8; idx++)
			websWrite(wp, T("%s%d %s>%d%s%d%s"), "<option value=", 36+4*idx,
					(36+4*idx == channel)? "selected" : "", 5180+20*idx,
					"MHz (Channel ", 36+4*idx, ")</option>");
	}
	if ((value == NULL) || (strcmp(value, "") == 0)) {
		for (idx = 16; idx < 27; idx++)
			websWrite(wp, T("%s%d %s>%d%s%d%s"), "<option value=", 36+4*idx,
				   	(36+4*idx == channel)? "selected" : "", 5180+20*idx,
					"MHz (Channel ", 36+4*idx, ")</option>");
	}
	if ((value == NULL) || (strcmp(value, "") == 0) ||
			(strcmp(value, "US") == 0) || (strcmp(value, "TW") == 0) ||
			(strcmp(value, "CN") == 0) || (strcmp(value, "HK") == 0)) {
		for (idx = 28; idx < 32; idx++)
			websWrite(wp, T("%s%d %s>%d%s%d%s"), "<option value=",
					36+4*idx+1, (36+4*idx+1 == channel)? "selected" : "",
					5180+20*idx+5, "MHz (Channel ", 36+4*idx+1, ")</option>");
	}
	if ((value == NULL) || (strcmp(value, "") == 0) ||
			(strcmp(value, "US") == 0) || (strcmp(value, "CN") == 0) ||
			(strcmp(value, "HK") == 0)) {
		return websWrite(wp,
				T("<option value=165 %s>5825MHz (Channel 165)</option>\n"),
				(165 == channel)? "selected" : "");
	}
	return 0;
#endif
}

/*
 * description: write 802.11b channels in <select> tag
 */
static int getWlan11bChannels(int eid, webs_t wp, int argc, char_t **argv)
{
#if 0	
	int idx = 0, channel;
	char *value = nvram_bufget(RT2860_NVRAM, "CountryCode");
	char *channel_s = nvram_bufget(RT2860_NVRAM, "Channel");

	channel = (channel_s == NULL)? 0 : atoi(channel_s);
	if ((value == NULL) || (strcmp(value, "") == 0) ||
			(strcmp(value, "US") == 0) || (strcmp(value, "JP") == 0) ||
			(strcmp(value, "FR") == 0) || (strcmp(value, "IE") == 0) ||
			(strcmp(value, "TW") == 0) || (strcmp(value, "CN") == 0) ||
			(strcmp(value, "HK") == 0)) {
		for (idx = 0; idx < 11; idx++)
			websWrite(wp, T("%s%d %s>%d%s%d%s"), "<option value=", idx+1,
					(idx+1 == channel)? "selected" : "", 2412+5*idx,
					"MHz (Channel ", idx+1, ")</option>");
	}

	if ((value == NULL) || (strcmp(value, "") == 0) ||
			(strcmp(value, "JP") == 0) || (strcmp(value, "TW") == 0) ||
			(strcmp(value, "FR") == 0) ||
			(strcmp(value, "IE") == 0) || (strcmp(value, "CN") == 0) ||
			(strcmp(value, "HK") == 0)) {
		for (idx = 11; idx < 13; idx++)
			websWrite(wp, T("%s%d %s>%d%s%d%s"), "<option value=", idx+1,
					(idx+1 == channel)? "selected" : "", 2412+5*idx,
					"MHz (Channel ", idx+1, ")</option>");
	}

	if ((value == NULL) || (strcmp(value, "") == 0) ||
			(strcmp(value, "JP") == 0)) {
		return websWrite(wp,
				T("<option value=14 %s>2484MHz (Channel 14)</option>\n"),
				(14 == channel)? "selected" : "");
	}
	return 0;
#endif
}

/*
 * description: write 802.11g channels in <select> tag
 */
static int getWlan11gChannels(int eid, webs_t wp, int argc, char_t **argv)
{
#if 0	
	int idx = 0, channel;
	char *value = nvram_bufget(RT2860_NVRAM, "CountryCode");
	char *channel_s = nvram_bufget(RT2860_NVRAM, "Channel");

	channel = (channel_s == NULL)? 0 : atoi(channel_s);
	if ((value == NULL) || (strcmp(value, "") == 0) ||
			(strcmp(value, "US") == 0) || (strcmp(value, "JP") == 0) ||
			(strcmp(value, "FR") == 0) || (strcmp(value, "IE") == 0) ||
			(strcmp(value, "TW") == 0) || (strcmp(value, "CN") == 0) ||
			(strcmp(value, "HK") == 0)) {
		for (idx = 0; idx < 11; idx++)
			websWrite(wp, T("%s%d %s>%d%s%d%s"), "<option value=", idx+1,
					(idx+1 == channel)? "selected" : "", 2412+5*idx,
					"MHz (Channel ", idx+1, ")</option>");
	}                                                                           
	if ((value == NULL) || (strcmp(value, "") == 0) ||
			(strcmp(value, "JP") == 0) || (strcmp(value, "TW") == 0) ||
			(strcmp(value, "FR") == 0) || (strcmp(value, "IE") == 0) ||
			(strcmp(value, "CN") == 0) || (strcmp(value, "HK") == 0)) {
		for (idx = 11; idx < 13; idx++)
			websWrite(wp, T("%s%d %s>%d%s%d%s"), "<option value=", idx+1,
					(idx+1 == channel)? "selected" : "", 2412+5*idx,
					"MHz (Channel ", idx+1, ")</option>");
	}

	if ((value == NULL) || (strcmp(value, "") == 0)) {
		return websWrite(wp,
				T("<option value=14 %s>2484MHz (Channel 14)</option>\n"),
				(14 == channel)? "selected" : "");
	}
	return 0;
#endif
}
//wisp mode general channel
static int getGeneralChannelsFreq(int eid, webs_t wp, int argc, char_t **argv)
{
#if 1
//added by nico----------------
	char *channel_s;
	char buf[TMP_LEN]; 
	char buf_channel[32];
	char *wirelessmode;
	char *value, *tmpvalue;

	char ModeTmpBuf[32];	
	ezplib_get_attr_val("wl_mode_rule", 0, "mode", ModeTmpBuf, 32, EZPLIB_USE_CLI);
	
	channel_s = nvram_safe_get("WISP_Channel");
	strcpy(buf_channel,channel_s);
//printf("buf_channel====>>>>>%s\n",buf_channel);
//	strcpy(channel_s, nvram_safe_get("AP_Channel"));
//printf("buf_channel====>>>>>%s\n",channel_s);	


    tmpvalue = nvram_safe_get("wl_region");
#if 1 //Steve reference udps.c
	if (!strcmp (tmpvalue,"0")){
		printf("	WISP==>US\n");
        value = "US";
	}
	else if (!strcmp (tmpvalue,"1")){
		printf("	WISP==>FR\n");
		value = "FR";
	}
    else if (!strcmp (tmpvalue,"5")){
    	printf("	WISP==>JP\n");
		value = "JP";
	}else{
		printf("	WISP Other==>FR\n");
		value = "FR";
	}
#else    
    if (!strcmp (tmpvalue,"0")){
       	value = "US";
	}
	else if (!strcmp (tmpvalue,"1")){
		value = "TW";
	}
        else if (!strcmp (tmpvalue,"2")){
		value = "TW";
	}
#endif
	if (!strcmp(ModeTmpBuf, "ap")){
		ezplib_get_attr_val("wl_ap_basic_rule", 0, "net_mode", buf, TMP_LEN, EZPLIB_USE_CLI); 
	}else {
		ezplib_get_attr_val("wl_basic_rule", 0, "net_mode", buf, TMP_LEN, EZPLIB_USE_CLI); 
	}
	
	strcpy(wirelessmode, buf);
//----------------------------------							
	int idx = 0, channel;
//	char *value = nvram_bufget(RT2860_NVRAM, "CountryCode");
//	char *channel_s = nvram_bufget(RT2860_NVRAM, "Channel");
//	char *wirlessmode = nvram_bufget(RT2860_NVRAM, "WirelessMode");	

	channel = (buf_channel == NULL)? 0 : atoi(buf_channel);
	if ((value == NULL) || (strcmp(value, "") == 0) ||
			(strcmp(value, "US") == 0) || (strcmp(value, "JP") == 0) ||
			(strcmp(value, "FR") == 0) || (strcmp(value, "IE") == 0) ||
			(strcmp(value, "TW") == 0) || (strcmp(value, "CN") == 0) ||
			(strcmp(value, "HK") == 0)) {
		for (idx = 0; idx < 11; idx++)
#if 0		
			websWrite(wp, T("%s%d %s>%d%s%d%s"), "<option value=", idx+1,
					(idx+1 == channel)? "selected" : "", 2412+5*idx,
					"MHz (Channel ", idx+1, ")</option>");
#else
		    if (idx <9){				
			websWrite(wp, T("%s%d %s>%s0%d% d%s"), "<option value=", idx+1,
					(idx+1 == channel)? "selected" : "", "Channel-",idx+1,
					2412+5*idx,"MHz</option>");
		    }else{
			websWrite(wp, T("%s%d %s>%s%d% d%s"), "<option value=", idx+1,
					(idx+1 == channel)? "selected" : "", "Channel-",idx+1,
					2412+5*idx,"MHz</option>");		    
		    }
		    
		    			
#endif						
	}                                                                           
	if ((value == NULL) || (strcmp(value, "") == 0) ||
			(strcmp(value, "TW") == 0) || (strcmp(value, "JP") == 0) || 
			(strcmp(value, "FR") == 0) || (strcmp(value, "IE") == 0) ||
			(strcmp(value, "CN") == 0) || (strcmp(value, "HK") == 0)) {
		for (idx = 11; idx < 13; idx++)
#if 0		
			websWrite(wp, T("%s%d %s>%d%s%d%s"), "<option value=", idx+1,
					(idx+1 == channel)? "selected" : "", 2412+5*idx,
					"MHz (Channel ", idx+1, ")</option>");
#else
			websWrite(wp, T("%s%d %s>%s%d% d%s"), "<option value=", idx+1,
					(idx+1 == channel)? "selected" : "", "Channel-",idx+1,
					2412+5*idx,"MHz</option>");			
#endif					
	}

	if ((value == NULL) || (strcmp(value, "") == 0) ||
		((!strcmp(value, "JP")) && (!strcmp(wirelessmode, "1"))) ) {
#if 0		
		return websWrite(wp,
				T("<option value=14 %s>2484MHz (Channel 14)</option>\n"),
				(14 == channel)? "selected" : "");
#else
		return websWrite(wp,
				T("<option value=14 %s>Channel-14 2484MHz</option>\n"),
				(14 == channel)? "selected" : "");
#endif				
	}
	return 0;
#endif
}

//wisp mode 5g general channel
static int get5gGeneralChannelsFreq(int eid, webs_t wp, int argc, char_t **argv)
{
#if 1
	char *channel_s;
    	char buf[TMP_LEN]; 
	char buf_channel[32];
	char *wirelessmode;
	char *value, *tmpvalue;
	
	channel_s = nvram_safe_get("WISP1_Channel");
	strcpy(buf_channel,channel_s);

    tmpvalue = nvram_safe_get("wl_region");
#if 1 //Steve reference udps.c
	if (!strcmp (tmpvalue,"0")){
		printf("	WISP==>US\n");
        value = "US";
	}
	else if (!strcmp (tmpvalue,"1")){
		printf("	WISP==>FR\n");
		value = "FR";
	}
    else if (!strcmp (tmpvalue,"5")){
    	printf("	WISP==>JP\n");
		value = "JP";
	}else{
		printf("	WISP Other==>FR\n");
		value = "FR";
	}
#else    
    if (!strcmp (tmpvalue,"0")){
       	value = "US";
	}
	else if (!strcmp (tmpvalue,"1")){
		value = "TW";
	}
        else if (!strcmp (tmpvalue,"2")){
		value = "TW";
	}
#endif
	
	ezplib_get_attr_val("wl5g_basic_rule", 0, "net_mode", buf, TMP_LEN, EZPLIB_USE_CLI); 
	strcpy(wirelessmode, buf);

	int idx = 0, channel,i = 0,j = 0,k  = 0;
	FILE *fin;
	char str[255],c;
	char ptn[]="Channel=";
	char channels[3];
	
	if( !strcmp(channel_s,"0")){
	      //system("iwconfig ra0 > /tmp/opchannel");
	      system("/sbin/ezp-wps-set 1 1 1 0");
		fin = fopen("/tmp/opchannel","r");		
		while ((c=fgetc(fin)) != EOF){
			ungetc(c,fin);		
			readline(str,fin);
			if (get_channel(channels, str, ptn) == 0)
		   	break;
		}

		fclose(fin);
		system("rm -f /tmp/opchannel");	
		strcpy(buf_channel,channels);
	}
	channel = (buf_channel == NULL)? 36 : atoi(buf_channel);
	if ((value == NULL) || (strcmp(value, "") == 0) ||
			(strcmp(value, "US") == 0) || (strcmp(value, "JP") == 0) ||
			(strcmp(value, "FR") == 0) || (strcmp(value, "IE") == 0) ||
			(strcmp(value, "TW") == 0) || (strcmp(value, "CN") == 0) ||
			(strcmp(value, "HK") == 0)) {
		for (idx = 0; idx < 21; idx++){		
  		      if(idx < 8){
                             websWrite(wp, T("%s%d %s>%s%d% d%s"), "<option value=", idx*4+36,
                                                       	(idx*4+36 == channel)? "selected" : "", "Channel-",idx*4+36,
                                                       	5180+10*i,"MHz</option>"); 
				   i = i+2;
		     }else if(idx > 18){
                            websWrite(wp, T("%s%d %s>%s%d% d%s"), "<option value=",149+j*2,
                                                       	(149+j*2 == channel)? "selected" : "", "Channel-",149+j*2,
                                                       	5745+10*j,"MHz</option>"); 
				j = j+2;
		   }else{
       		       websWrite(wp, T("%s%d %s>%s%d% d%s"), "<option value=", 100+k*2,
                                                              	(100+k*2 == channel)? "selected" : "", "Channel-",100+k*2,
                                                         	        5500+10*k,"MHz</option>");
				if(idx == 12 )
					k = k+8;
				else
					k = k+2;
		   }
		}			
	}
	//TODO for 5g
	if ((value == NULL) || (strcmp(value, "") == 0) ||
			(strcmp(value, "TW") == 0) || (strcmp(value, "JP") == 0) || 
			(strcmp(value, "FR") == 0) || (strcmp(value, "IE") == 0) ||
			(strcmp(value, "CN") == 0) || (strcmp(value, "HK") == 0)) {
		for (idx = 11; idx < 13; idx++)
#if 0		
			websWrite(wp, T("%s%d %s>%d%s%d%s"), "<option value=", idx+1,
					(idx+1 == channel)? "selected" : "", 2412+5*idx,
					"MHz (Channel ", idx+1, ")</option>");
#else
			websWrite(wp, T("%s%d %s>%s%d% d%s"), "<option value=", idx+1,
					(idx+1 == channel)? "selected" : "", "Channel-",idx+1,
					2412+5*idx,"MHz</option>");			
#endif					
	}

	if ((value == NULL) || (strcmp(value, "") == 0) ||
		((!strcmp(value, "JP")) && (!strcmp(wirelessmode, "1"))) ) {
#if 0		
		return websWrite(wp,
				T("<option value=14 %s>2484MHz (Channel 14)</option>\n"),
				(14 == channel)? "selected" : "");
#else
		return websWrite(wp,
				T("<option value=14 %s>Channel-14 2484MHz</option>\n"),
				(14 == channel)? "selected" : "");
#endif				
	}
	return 0;
#endif
}

/*
 * description: write 802.11g channels in <select> tag
 */
static int getWlan11gChannelsFreq(int eid, webs_t wp, int argc, char_t **argv)
{
#if 1
//added by nico----------------
	char *channel_s;
	char buf[TMP_LEN]; 
	char buf_channel[32];
	char *wirelessmode;
	char *value, *tmpvalue;
	
	char ModeTmpBuf[32];	
	ezplib_get_attr_val("wl_mode_rule", 0, "mode", ModeTmpBuf, 32, EZPLIB_USE_CLI);

	channel_s = nvram_safe_get("AP_Channel");
	strcpy(buf_channel,channel_s);
//printf("buf_channel====>>>>>%s\n",buf_channel);
//	strcpy(channel_s, nvram_safe_get("AP_Channel"));
//printf("buf_channel====>>>>>%s\n",channel_s);	


        tmpvalue = nvram_safe_get("wl_region");

#if 1 //Steve reference udps.c
	if (!strcmp (tmpvalue,"0")){
		printf("	==>US\n");
        value = "US";
	}
	else if (!strcmp (tmpvalue,"1")){
		printf("	==>FR\n");
		value = "FR";
	}
    else if (!strcmp (tmpvalue,"5")){
    	printf("	==>JP\n");
		value = "JP";
	}else{
		printf("	Other==>FR\n");
		value = "FR";
	}
#else        
    if (!strcmp (tmpvalue,"0")){
        	value = "US";
	}
	else if (!strcmp (tmpvalue,"1")){
		value = "TW";
	}
        else if (!strcmp (tmpvalue,"2")){
		value = "TW";
	}
#endif
	if (!strcmp(ModeTmpBuf, "ap")){
		ezplib_get_attr_val("wl_ap_basic_rule", 0, "net_mode", buf, TMP_LEN, EZPLIB_USE_CLI); 
	}else {
		ezplib_get_attr_val("wl_basic_rule", 0, "net_mode", buf, TMP_LEN, EZPLIB_USE_CLI); 
	}
	strcpy(wirelessmode, buf);
//----------------------------------							
	int idx = 0, channel;
//	char *value = nvram_bufget(RT2860_NVRAM, "CountryCode");
//	char *channel_s = nvram_bufget(RT2860_NVRAM, "Channel");
//	char *wirlessmode = nvram_bufget(RT2860_NVRAM, "WirelessMode");	

	channel = (buf_channel == NULL)? 0 : atoi(buf_channel);
	if ((value == NULL) || (strcmp(value, "") == 0) ||
			(strcmp(value, "US") == 0) || (strcmp(value, "JP") == 0) ||
			(strcmp(value, "FR") == 0) || (strcmp(value, "IE") == 0) ||
			(strcmp(value, "TW") == 0) || (strcmp(value, "CN") == 0) ||
			(strcmp(value, "HK") == 0)) {
		for (idx = 0; idx < 11; idx++)
#if 0		
			websWrite(wp, T("%s%d %s>%d%s%d%s"), "<option value=", idx+1,
					(idx+1 == channel)? "selected" : "", 2412+5*idx,
					"MHz (Channel ", idx+1, ")</option>");
#else
		    if (idx <9){				
			websWrite(wp, T("%s%d %s>%s0%d% d%s"), "<option value=", idx+1,
					(idx+1 == channel)? "selected" : "", "Channel-",idx+1,
					2412+5*idx,"MHz</option>");
		    }else{
			websWrite(wp, T("%s%d %s>%s%d% d%s"), "<option value=", idx+1,
					(idx+1 == channel)? "selected" : "", "Channel-",idx+1,
					2412+5*idx,"MHz</option>");		    
		    }
		    
		    			
#endif						
	}                                                                           
	if ((value == NULL) || (strcmp(value, "") == 0) ||
			(strcmp(value, "TW") == 0) || (strcmp(value, "JP") == 0) || 
			(strcmp(value, "FR") == 0) || (strcmp(value, "IE") == 0) ||
			(strcmp(value, "CN") == 0) || (strcmp(value, "HK") == 0)) {
		for (idx = 11; idx < 13; idx++)
#if 0		
			websWrite(wp, T("%s%d %s>%d%s%d%s"), "<option value=", idx+1,
					(idx+1 == channel)? "selected" : "", 2412+5*idx,
					"MHz (Channel ", idx+1, ")</option>");
#else
			websWrite(wp, T("%s%d %s>%s%d% d%s"), "<option value=", idx+1,
					(idx+1 == channel)? "selected" : "", "Channel-",idx+1,
					2412+5*idx,"MHz</option>");			
#endif					
	}

	if ((value == NULL) || (strcmp(value, "") == 0) ||
		((!strcmp(value, "JP")) && (!strcmp(wirelessmode, "1"))) ) {
#if 0		
		return websWrite(wp,
				T("<option value=14 %s>2484MHz (Channel 14)</option>\n"),
				(14 == channel)? "selected" : "");
#else
		return websWrite(wp,
				T("<option value=14 %s>Channel-14 2484MHz</option>\n"),
				(14 == channel)? "selected" : "");
#endif				
	}
	return 0;
#endif
}
/*
 * description: write 802.11a channels in <select> tag
 */
static int getWlan11aChannelsFreq(int eid, webs_t wp, int argc, char_t **argv)
{
#if 1
	char *channel_s;
    	char buf[TMP_LEN]; 
	char buf_channel[32];
	char *wirelessmode;
	char *value, *tmpvalue;
	
	channel_s = nvram_safe_get("AP1_Channel");
	strcpy(buf_channel,channel_s);
//printf("buf_channel====>>>>>%s\n",buf_channel);
//	strcpy(channel_s, nvram_safe_get("AP_Channel"));
//printf("buf_channel====>>>>>%s\n",channel_s);

       tmpvalue = nvram_safe_get("wl_region");

#if 1 //Steve reference udps.c
	if (!strcmp (tmpvalue,"0")){
		printf("	==>US\n");
        value = "US";
	}
	else if (!strcmp (tmpvalue,"1")){
		printf("	==>FR\n");
		value = "FR";
	}
    else if (!strcmp (tmpvalue,"5")){
    	printf("	==>JP\n");
		value = "JP";
	}else{
		printf("	Other==>FR\n");
		value = "FR";
	}
#else        
    if (!strcmp (tmpvalue,"0")){
        	value = "US";
	}
	else if (!strcmp (tmpvalue,"1")){
		value = "TW";
	}
        else if (!strcmp (tmpvalue,"2")){
		value = "TW";
	}
#endif
	
	ezplib_get_attr_val("wl5g_basic_rule", 0, "net_mode", buf, TMP_LEN, EZPLIB_USE_CLI); 
	strcpy(wirelessmode, buf);

	int idx = 0, channel,i = 0,j = 0,k  = 0;
	FILE *fin;
	char str[255],c;
	char ptn[]="Channel=";
	char channels[3];
	
	if( !strcmp(channel_s,"0")){
	      //system("iwconfig ra0 > /tmp/opchannel");
	      system("/sbin/ezp-wps-set 1 1 1 0");
		fin = fopen("/tmp/opchannel","r");		
		while ((c=fgetc(fin)) != EOF){
			ungetc(c,fin);		
			readline(str,fin);
			if (get_channel(channels, str, ptn) == 0)
		   	break;
		}

		fclose(fin);
		system("rm -f /tmp/opchannel");	
		strcpy(buf_channel,channels);
	}
	channel = (buf_channel == NULL)? 36 : atoi(buf_channel);
	if ((value == NULL) || (strcmp(value, "") == 0) ||
			(strcmp(value, "US") == 0) || (strcmp(value, "JP") == 0) ||
			(strcmp(value, "FR") == 0) || (strcmp(value, "IE") == 0) ||
			(strcmp(value, "TW") == 0) || (strcmp(value, "CN") == 0) ||
			(strcmp(value, "HK") == 0)) {
		for (idx = 0; idx < 21; idx++){		
  		      if(idx < 8){
                             websWrite(wp, T("%s%d %s>%s%d% d%s"), "<option value=", idx*4+36,
                                                       	(idx*4+36 == channel)? "selected" : "", "Channel-",idx*4+36,
                                                       	5180+10*i,"MHz</option>"); 
				   i = i+2;
		     }else if(idx > 15){
                            websWrite(wp, T("%s%d %s>%s%d% d%s"), "<option value=",149+j*2,
                                                       	(149+j*2 == channel)? "selected" : "", "Channel-",149+j*2,
                                                       	5745+10*j,"MHz</option>"); 
				j = j+2;
		   }else{
       		       websWrite(wp, T("%s%d %s>%s%d% d%s"), "<option value=", 100+k*2,
                                                              	(100+k*2 == channel)? "selected" : "", "Channel-",100+k*2,
                                                         	        5500+10*k,"MHz</option>");
				if(idx == 12 )
					k = k+8;
				else
					k = k+2;
		   }
		}			
	}
	//TODO for 5g
	if ((value == NULL) || (strcmp(value, "") == 0) ||
			(strcmp(value, "TW") == 0) || (strcmp(value, "JP") == 0) || 
			(strcmp(value, "FR") == 0) || (strcmp(value, "IE") == 0) ||
			(strcmp(value, "CN") == 0) || (strcmp(value, "HK") == 0)) {
		for (idx = 11; idx < 13; idx++)
#if 0		
			websWrite(wp, T("%s%d %s>%d%s%d%s"), "<option value=", idx+1,
					(idx+1 == channel)? "selected" : "", 2412+5*idx,
					"MHz (Channel ", idx+1, ")</option>");
#else
			websWrite(wp, T("%s%d %s>%s%d% d%s"), "<option value=", idx+1,
					(idx+1 == channel)? "selected" : "", "Channel-",idx+1,
					2412+5*idx,"MHz</option>");			
#endif					
	}

	if ((value == NULL) || (strcmp(value, "") == 0) ||
		((!strcmp(value, "JP")) && (!strcmp(wirelessmode, "1"))) ) {
#if 0		
		return websWrite(wp,
				T("<option value=14 %s>2484MHz (Channel 14)</option>\n"),
				(14 == channel)? "selected" : "");
#else
		return websWrite(wp,
				T("<option value=14 %s>Channel-14 2484MHz</option>\n"),
				(14 == channel)? "selected" : "");
#endif				
	}
	return 0;
#endif
}


//
/*
 * description: write 802.11a ext channels in <select> tag
 */
static int getWlan11aExtChannelsFreq(int eid, webs_t wp, int argc, char_t **argv)
{
#if 1	
	int channel,ext_channel_idx;
	//char *channel_s;
	char channel_s[TMP_LEN];
    	char buf[TMP_LEN]; 
	//char *ext_channel_s;
	char ext_channel_s[TMP_LEN];
	
	char *value, *tmpvalue;
	ezplib_get_attr_val("wl5g_basic_rule", 0, "channel", buf, TMP_LEN, EZPLIB_USE_CLI); 
	strcpy(channel_s, buf);
			        
    tmpvalue = nvram_safe_get("wl_region");
    
#if 1 //Steve reference udps.c
	if (!strcmp (tmpvalue,"0")){
		printf("	Ext==>US\n");
        value = "US";
	}
	else if (!strcmp (tmpvalue,"1")){
		printf("	Ext==>FR\n");
		value = "FR";
	}
    else if (!strcmp (tmpvalue,"5")){
    	printf("	Ext==>JP\n");
		value = "JP";
	}else{
		printf("	Ext Other==>FR\n");
		value = "FR";
	}
#else
    if (!strcmp (tmpvalue,"0")){
        	value = "US";
	}
	else if (!strcmp (tmpvalue,"1")){
		value = "TW";
	}
        else if (!strcmp (tmpvalue,"2")){
		value = "TW";
	}
#endif

		
	ezplib_get_attr_val("wl1_advanced_rule", 0, "extcha", buf, TMP_LEN, EZPLIB_USE_CLI); 
	strcpy(ext_channel_s, buf);
	
	channel = atoi(channel_s);
	ext_channel_idx =  atoi(ext_channel_s);

	if (channel ==0){
	    websWrite(wp, T("<option value=0 selected>      AUTO        </option>"));
	}else if (channel >= 36 && channel <= 44){
	    websWrite(wp, T("<option value=0 selected></option>\n"));
	}else if (channel == 48){
	    websWrite(wp, T("<option value=0 selected>Channel-%d %dMHz</option>\n"),channel+4, 5180+5*(channel-32));
	}else if (channel >= 52 && channel <= 60){	
	    websWrite(wp, T("<option value=0%s>Channel-%d %dMHz</option>\n"),(ext_channel_idx == 0)? " selected" : "",channel-4, 5180+5*(channel-40));
	    websWrite(wp, T("<option value=1%s>Channel-%d %dMHz</option>\n"),(ext_channel_idx == 1)? " selected" : "",channel+4, 5200+5*(channel-36));
	}else if (channel == 64){
	    websWrite(wp, T("<option value=0 selected>Channel-%d %dMHz</option>\n"),channel-4, 5180+5*(channel-20));
	}else if (channel == 100){	
	    websWrite(wp, T("<option value=0 selected>Channel-%d %dMHz</option>\n"),channel+4, 5500+5*(104-channel));
	}else if (channel >= 104 && channel <=136){
	    if(channel == 116){
	         websWrite(wp, T("<option value=0%s>Channel-%d %dMHz</option>\n"),(ext_channel_idx == 0)? " selected" : "",channel-4, 5500+5*(channel-104));
	    }else if(channel == 132){
	         websWrite(wp, T("<option value=1%s>Channel-%d %dMHz</option>\n"),(ext_channel_idx == 1)? " selected" : "",channel+4, 5520+5*(channel-100));
	    }else{
	         websWrite(wp, T("<option value=0%s>Channel-%d %dMHz</option>\n"),(ext_channel_idx == 0)? " selected" : "",channel-4, 5500+5*(channel-104));
	         websWrite(wp, T("<option value=1%s>Channel-%d %dMHz</option>\n"),(ext_channel_idx == 1)? " selected" : "",channel+4, 5520+5*(channel-100));
	    }
	}else if (channel == 140){	
	    websWrite(wp, T("<option value=0 selected>Channel-%d %dMHz</option>\n"),channel-4, 5500+5*(channel-104));
	}else if (channel == 149){	
	    websWrite(wp, T("<option value=0 selected>Channel-%d %dMHz</option>\n"),channel+4, 5745+5*(153-channel));
	}else if (channel >= 153 && channel <= 161){	
	    websWrite(wp, T("<option value=0%s>Channel-%d %dMHz</option>\n"),(ext_channel_idx == 0)? " selected" : "",channel-4, 5745+5*(channel-153));
	    websWrite(wp, T("<option value=1%s>Channel-%d %dMHz</option>\n"),(ext_channel_idx == 1)? " selected" : "",channel+4, 5765+5*(channel-149));
	}else if (channel == 165){	
	    websWrite(wp, T("<option value=0 selected>Channel-%d %dMHz</option>\n"),channel-4, 5745+5*(channel-153));
	}
	return 0;
#endif	
}

//
/*
 * description: write 802.11g ext channels in <select> tag
 */
static int getWlan11gExtChannelsFreq(int eid, webs_t wp, int argc, char_t **argv)
{
#if 1	
	int channel,ext_channel_idx;
	//char *channel_s;
	char channel_s[TMP_LEN];
	char buf[TMP_LEN]; 
	//char *ext_channel_s;
	char ext_channel_s[TMP_LEN];
	char *value, *tmpvalue;
	char ModeTmpBuf[32];
	
	ezplib_get_attr_val("wl_mode_rule", 0, "mode", ModeTmpBuf, 32, EZPLIB_USE_CLI);
	if (!strcmp(ModeTmpBuf, "ap")){
		ezplib_get_attr_val("wl_ap_basic_rule", 0, "channel", buf, TMP_LEN, EZPLIB_USE_CLI); 
	}else {
		ezplib_get_attr_val("wl_basic_rule", 0, "channel", buf, TMP_LEN, EZPLIB_USE_CLI); 
	}
	
	strcpy(channel_s, buf);
			        
    tmpvalue = nvram_safe_get("wl_region");
    
#if 1 //Steve reference udps.c
	if (!strcmp (tmpvalue,"0")){
		printf("	Ext==>US\n");
        value = "US";
	}
	else if (!strcmp (tmpvalue,"1")){
		printf("	Ext==>FR\n");
		value = "FR";
	}
    else if (!strcmp (tmpvalue,"5")){
    	printf("	Ext==>JP\n");
		value = "JP";
	}else{
		printf("	Ext Other==>FR\n");
		value = "FR";
	}
#else
    if (!strcmp (tmpvalue,"0")){
        	value = "US";
	}
	else if (!strcmp (tmpvalue,"1")){
		value = "TW";
	}
        else if (!strcmp (tmpvalue,"2")){
		value = "TW";
	}
#endif

	if (!strcmp(ModeTmpBuf, "ap")){
		ezplib_get_attr_val("wl_ap_advanced_rule", 0, "extcha", buf, TMP_LEN, EZPLIB_USE_CLI); 
	}else {
		ezplib_get_attr_val("wl_advanced_rule", 0, "extcha", buf, TMP_LEN, EZPLIB_USE_CLI); 
	}
	strcpy(ext_channel_s, buf);
	
//	char *value = nvram_bufget(RT2860_NVRAM, "CountryCode");
//	char_t *channel_s = nvram_bufget(RT2860_NVRAM, "Channel");
//	char_t *ext_channel_s = nvram_bufget(RT2860_NVRAM, "HT_EXTCHA");	
	
	channel = atoi(channel_s);
	ext_channel_idx =  atoi(ext_channel_s);

	if (channel ==0){
	    websWrite(wp, T("<option value=0 selected>      AUTO        </option>"));
	}
	else if (channel >= 1 && channel <= 4){
	    websWrite(wp, T("<option value=0 selected>Channel-0%d %dMHz</option>\n"),
	    	channel+4, 2412+5*(channel+3));
	}
	else if (channel >= 5 && channel <= 10){
	    if (channel == 5){
		websWrite(wp, T("<option value=0%s>Channel-01 2412MHz</option>\n"),
			(ext_channel_idx == 0)? " selected" : "");
		websWrite(wp, T("<option value=1%s>Channel-09 2452MHz</option>\n"),
			(ext_channel_idx == 1)? " selected" : "");	    	
	    }
	    else if(channel >= 6 && channel <= 7){
			websWrite(wp, T("<option value=0%s>Channel-0%d %dMHz</option>\n"),
				(ext_channel_idx == 0)? " selected" : "",
				channel-4, 2412+5*(channel-5));
			websWrite(wp, T("<option value=1%s>Channel-%d %dMHz</option>\n"),
				(ext_channel_idx == 1)? " selected" : "",
				channel+4, 2412+5*(channel+3));
  	    }
  	    else if (channel >= 8 && channel <= 9){
			if ((value == NULL) || (!strcmp(value, "")) ||
				(!strcmp(value, "TW")) || (!strcmp(value, "FR")) || 
				(!strcmp(value, "IE")) || (!strcmp(value, "JP")) || 
				(!strcmp(value, "CN")) || (!strcmp(value, "HK")) ) {
					websWrite(wp, T("<option value=0%s>Channel-0%d %dMHz</option>\n"),
						(ext_channel_idx == 0)? " selected" : "",channel-4, 2412+5*(channel-5));
					websWrite(wp, T("<option value=1%s>Channel-%d %dMHz</option>\n"),
						(ext_channel_idx == 1)? " selected" : "",channel+4, 2412+5*(channel+3));
			}
			else{
				websWrite(wp, T("<option value=0%s>Channel-0%d %dMHz</option>\n"),
					(ext_channel_idx == 0)? " selected" : "",channel-4, 2412+5*(channel-5));
			}
  	    }
  	    else if (channel == 10){
  	    	if ((value == NULL) || (!strcmp(value, ""))){
  	    		websWrite(wp, T("<option value=0%s>Channel-0%d %dMHz</option>\n"),
  	    			(ext_channel_idx == 0)? " selected" : "",channel-4, 2412+5*(channel-5));
				websWrite(wp, T("<option value=1%s>Channel-%d %dMHz</option>\n"),
					(ext_channel_idx == 1)? " selected" : "",channel+4, 2412+5*(channel+3));
			}
			else{
				websWrite(wp, T("<option value=0%s>Channel-0%d %dMHz</option>\n"),
					(ext_channel_idx == 0)? " selected" : "",channel-4, 2412+5*(channel-5));
			}	
  	    }		
	}
	else if (channel >= 11 && channel <= 13){
		websWrite(wp, T("<option value=0 selected>Channel-0%d %dMHz</option>\n"),
			channel-4, 2412+5*(channel-4));
	}
	else if (channel == 14){
		websWrite(wp, T("<option value=0 selected>Channel-10 2457MHz</option>\n"));		
	}

	return 0;
#endif	
}

static int getWlanApcliBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#ifdef CONFIG_RT2860V2_AP_APCLI
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

/*
 * description: write channel number or 0 if auto-select
 */
static int getWlanChannel(int eid, webs_t wp, int argc, char_t **argv)
{
#if 0	
	char *value = nvram_bufget(RT2860_NVRAM, "AutoChannelSelect");

	if (NULL == value)
		return websWrite(wp, T("9"));
	if (!strncmp(value, "1", 2))
		return websWrite(wp, T("0"));

	value = nvram_bufget(RT2860_NVRAM, "Channel");
	if (NULL == value)
		return websWrite(wp, T("9"));
	else
		return websWrite(wp, T("%s"), value);
#endif
}


int getRadioStatus(void)
{
#if 0	
	char *Radio_OFF = nvram_bufget(RT2860_NVRAM, "RadioOff");	
	char *scheduling_enable = nvram_bufget(RT2860_NVRAM, "WiFiScheduleEnable");
	char *currRadio = nvram_bufget(RT2860_NVRAM, "SchedulerRadioOn");	
	char *operation_mode = nvram_bufget(RT2860_NVRAM, "OP_Mode");

	if (!strcmp(Radio_OFF, "0")){
		if ( (strcmp(operation_mode, "6")) && (strcmp(operation_mode, "2"))){
		if (!strcmp(scheduling_enable, "1")){
			if (!strcmp(currRadio, "0"))
				return 0;
		}
		return 1;
	}else{
			return 1;		
		}	
	}else{
		return 0;
	}
#endif		
}


// Tommy , 2009/2/21 10:45
static int getRadioStatusASP(int eid, webs_t wp, int argc, char_t **argv)
{
#if 1	
//	char *Radio_OFF = nvram_bufget(RT2860_NVRAM, "RadioOff");	
//	char *scheduling_enable = nvram_bufget(RT2860_NVRAM, "WiFiScheduleEnable");
//	char *currRadio = nvram_bufget(RT2860_NVRAM, "SchedulerRadioOn");	
//	char *operation_mode = nvram_bufget(RT2860_NVRAM, "OP_Mode");
	
//added by nico--------------------------------
	char *Radio_OFF;
	char buf[1024]; 
    int fd;
    char cmd[256];
	char_t *wr_itf;
	int radio_on_off;
	char ModeTmpBuf[32];
	
    #define RADIO_STATUS_PATH          "/tmp/status_of_2G_radio"

    memset(cmd, 0, sizeof(cmd));
    Radio_OFF = buf;
    ezplib_get_attr_val("wl_mode_rule", 0, "mode", ModeTmpBuf, 32, EZPLIB_USE_CLI);
	
	#if 0
    	ezplib_get_attr_val("wl_basic_rule", 0, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
    	
    		   strcpy(Radio_OFF, buf);
    #else

        radio_on_off = 0;
    
        if (ejArgs(argc, argv, T("%s"), &wr_itf) < 1) {
			if (!strcmp(ModeTmpBuf, "ap")){
				ezplib_get_attr_val("wl_ap_basic_rule", 0, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
			}else {
				ezplib_get_attr_val("wl_basic_rule", 0, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
			}
            
		    if (!strcmp(Radio_OFF, "0")){
                 websWrite(wp, T("ON"));			
            }else{
            	return websWrite(wp, T("OFF"));
            }	
	    }
        
        sprintf(cmd, "ifconfig %s | grep Scope:Link | awk '{print $4}' | cut -c 7-10  1> %s", wr_itf, RADIO_STATUS_PATH);
        
        system(cmd);
        printf("cmd: %s\n", cmd);
        if (( fd = open(RADIO_STATUS_PATH, O_RDONLY) ) < 0) {
            goto BadResult;
        }

        if (read(fd, Radio_OFF, 32) < 0) {
            goto BadResult;
        }

        close(fd);

        /* Deleted tmp file */
        sprintf(cmd, "rm %s", RADIO_STATUS_PATH );
        system(cmd);
        printf("--------------------------------%s\n", Radio_OFF);
        /*  ap or ap client on or off */
        //if (!strcmp(Radio_OFF, "") || !strstr(Radio_OFF, "ESSID:") || strstr(Radio_OFF, "Not-Associated"))
        if (strstr(Radio_OFF, "Link"))
            return websWrite(wp, T("ON"));
        else 
            return websWrite(wp, T("OFF"));

    #endif

    BadResult:
        printf("open or read %s fail\n", RADIO_STATUS_PATH);
        return websWrite(wp, T("OFF"));

    #undef RADIO_STATUS_PATH
#endif
}
static int getRadioStatusASP5G(int eid, webs_t wp, int argc, char_t **argv)
{
#if 1	
	char *Radio_OFF;
    	char buf[TMP_LEN]; 
	 
	
	ezplib_get_attr_val("wl5g_basic_rule", 0, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
	
		   strcpy(Radio_OFF, buf);

	if (!strcmp(Radio_OFF, "0")){
		return websWrite(wp, T("OFF"));		
	}else{
		return websWrite(wp, T("ON"));		
	}
#endif
}
static int getConnectedAPMode(int eid, webs_t wp, int argc, char_t **argv)
{
       char buf[50];
	FILE *fp = NULL;
	int len=0;

       system("iwpriv apclii0 set SiteSurvey=1");
       system("iwpriv apclii0 get_site_survey > /etc/site_survey.dat");
	system("/sbin/ezp-wps-set 2 0 0 0");
	if (NULL == (fp = fopen("/etc/wireless_mode.dat", "r")))
	{
		websWrite(wp, T("--"));
		return 0;
	}
    	len = fscanf(fp, "%s", buf);
	fclose(fp);
	system("rm /etc/wireless_mode.dat");
	if(len!=1)
             websWrite(wp, T("--"));
       else
             websWrite(wp, T("%s"), buf);
   
}
static int getAPClientConnectionStatus(int eid, webs_t wp, int argc, char_t **argv)
{
	char buf[65];
	FILE *fp = NULL;
	int len=0;
	
	//system("iwconfig apclii0 | grep 'Access Point:' | sed 's/^.*Access Point: //g' > /tmp/apclient_essid_mac");
       system("/sbin/ezp-wps-set 1 00 5 0");
	if (NULL == (fp = fopen("/tmp/apclient_essid_mac", "r")))
	{
		websWrite(wp, T("None"));
		return 0;
	}
	len = fscanf(fp, "%s", buf);
	fclose(fp);
	if (len!=1)
	{
		websWrite(wp, T("None"));
		return 0;
	}
	
	if (strlen(buf)==17)
	{
		websWrite(wp, T("Connected"));
	}
	else
	{
		websWrite(wp, T("None"));
	}
	return 0;
}

static int getAPClientConnectionESSIDMAC(int eid, webs_t wp, int argc, char_t **argv)
{
	char buf[65];
	FILE *fp = NULL;
	int len=0;
	
	//system("iwconfig apclii0 | grep 'Access Point:' | sed 's/^.*Access Point: //g' > /tmp/apclient_essid_mac");
       system("/sbin/ezp-wps-set 1 00 5 0");
	if (NULL == (fp = fopen("/tmp/apclient_essid_mac", "r")))
	{
		return 0;
	}
	len = fscanf(fp, "%s", buf);
	fclose(fp);
	if (len!=1)
	{
		return 0;
	}
	
	if (strlen(buf)==17)
	{
		websWrite(wp, T(" (%s)"), buf);
	}
	return 0;
}

static char apcli[33];
static int getAPClientConnectionESSID(int eid, webs_t wp, int argc, char_t **argv)
{
	char buf[65];
	FILE *fp = NULL;
	int len=0;
#if 0	//Ricardo added
	//char *apclissid = nvram_get(RT2860_NVRAM, "ApCliSsid");
	ezplib_get_attr_val("wl0_apcli_rule", 0, "ssid", apcli, 33, EZPLIB_USE_CLI);
	websWrite(wp, T("%s"), apcli);
#else
	system("iwconfig apclii0 | grep ESSID | sed 's/^.*ESSID:\"//g' | sed 's/\".*$//g' > /tmp/apcli_essid");
	if (NULL == (fp = fopen("/tmp/apcli_essid", "r"))) {
		websWrite(wp, T(" "));
	} else {
		fseek(fp, 0, SEEK_END);
		len = ftell(fp);
		rewind(fp);
		fgets(buf, len, fp);
		websWrite(wp, T("%s"), buf);
		fclose(fp);
	}
#endif
	return 0;
}

static int getAPClient5GConnectionStatus(int eid, webs_t wp, int argc, char_t **argv)
{
	char buf[65];
	FILE *fp = NULL;
	int len=0;
	
	//system("iwconfig apcli0 | grep 'Access Point:' | sed 's/^.*Access Point: //g' > /tmp/apclient1_essid_mac");
       system("/sbin/ezp-wps-set 1 01 5 0");
	if (NULL == (fp = fopen("/tmp/apclient1_essid_mac", "r")))
	{
		websWrite(wp, T("None"));
		return 0;
	}
	len = fscanf(fp, "%s", buf);
	fclose(fp);
	if (len!=1)
	{
		websWrite(wp, T("None"));
		return 0;
	}
	
	if (strlen(buf)==17)
	{
		websWrite(wp, T("Connected"));
	}
	else
	{
		websWrite(wp, T("None"));
	}
	return 0;
}

static int getAPClient5GConnectionESSIDMAC(int eid, webs_t wp, int argc, char_t **argv)
{
	char buf[65];
	FILE *fp = NULL;
	int len=0;
	
	//system("iwconfig apcli0 | grep 'Access Point:' | sed 's/^.*Access Point: //g' > /tmp/apclient1_essid_mac");
       system("/sbin/ezp-wps-set 1 01 5 0");
	if (NULL == (fp = fopen("/tmp/apclient1_essid_mac", "r")))
	{
		return 0;
	}
	len = fscanf(fp, "%s", buf);
	fclose(fp);
	if (len!=1)
	{
		return 0;
	}
	
	if (strlen(buf)==17)
	{
		websWrite(wp, T(" (%s)"), buf);
	}
	return 0;
}

static char apcli1[33];
static int getAPClient5GConnectionESSID(int eid, webs_t wp, int argc, char_t **argv)
{
	char buf[65];
	FILE *fp = NULL;
	int len=0;
	
	//char *apclissid = nvram_get(RT2860_NVRAM, "ApCliSsid");
	ezplib_get_attr_val("wl1_apcli_rule", 0, "ssid", apcli1, 33, EZPLIB_USE_CLI);
	websWrite(wp, T("%s"), apcli1);
	return 0;
}

/*
 * description: write MAC address from interface 'ra0'
 */
static int getWlanCurrentMac(int eid, webs_t wp, int argc, char_t **argv)
{
	char if_hw[18] = {0};

	if (-1 == getIfMac("rai0", if_hw))
		return websWrite(wp, T(" "));
	return websWrite(wp, T("%s"), if_hw);
}

typedef union _MACHTTRANSMIT_SETTING {
	struct  {
		unsigned short  MCS:7;  // MCS
		unsigned short  BW:1;   //channel bandwidth 20MHz or 40 MHz
		unsigned short  ShortGI:1;
		unsigned short  STBC:2; //SPACE
		unsigned short  rsv:3;
		unsigned short  MODE:2; // Use definition MODE_xxx.
	} field;
	unsigned short      word;
} MACHTTRANSMIT_SETTING;

typedef struct _RT_802_11_MAC_ENTRY {
	unsigned char            ApIdx;
	unsigned char            Addr[6];
	unsigned char            Aid;
	unsigned char            Psm;     // 0:PWR_ACTIVE, 1:PWR_SAVE
	unsigned char            MimoPs;  // 0:MMPS_STATIC, 1:MMPS_DYNAMIC, 3:MMPS_Enabled
	char                     AvgRssi0;
	char                     AvgRssi1;
	char                     AvgRssi2;
	unsigned int             ConnectedTime;
// Tommy , add association Time ,2009/1/20 11:38
	//unsigned long		 AssociationTime;	
	MACHTTRANSMIT_SETTING    TxRate;
// Patch EZP
	unsigned int		LastRxRate; 
	short		StreamSnr[3]; 
	short		SoundingRespSnr[3];
// Patch EZP
} RT_802_11_MAC_ENTRY;

typedef struct _RT_802_11_MAC_TABLE {
	unsigned long            Num;
	RT_802_11_MAC_ENTRY      Entry[32]; //MAX_LEN_OF_MAC_TABLE = 32 
} RT_802_11_MAC_TABLE;

//_RT1_802_11_MAC_ENTRY for 5g
typedef struct _RT1_802_11_MAC_ENTRY {
    unsigned char       ApIdx;
    unsigned char       Addr[6];
    unsigned char       Aid;
    unsigned char       Psm;     // 0:PWR_ACTIVE, 1:PWR_SAVE
    unsigned char       MimoPs;  // 0:MMPS_STATIC, 1:MMPS_DYNAMIC, 3:MMPS_Enabled
    char AvgRssi0;
    char AvgRssi1;
    char AvgRssi2;
    unsigned int ConnectedTime;
/* EZP add association Time */
    unsigned long       AssociationTime;
    MACHTTRANSMIT_SETTING TxRate;
//#ifdef RTMP_RBUS_SUPPORT
   unsigned int LastRxRate;
   short StreamSnr[3]; // BF SNR from RXWI. Units=0.25 dB. 22 dB offset removed
   short SoundingRespSnr[3]; // SNR from Sounding Response. Units=0.25 dB. 22 dB offset removed
   //SHORT TxPER; // TX PER over the last second. Percent
   //SHORT reserved;
//#endif // RTMP_RBUS_SUPPORT //
}RT1_802_11_MAC_ENTRY;
 
typedef struct _RT1_802_11_MAC_TABLE {
    unsigned long       Num;
    RT1_802_11_MAC_ENTRY Entry[32];
} RT1_802_11_MAC_TABLE;

static int getWlanStaInfo(int eid, webs_t wp, int argc, char_t **argv)
{
	int i, s;
	struct iwreq iwr;
	RT_802_11_MAC_TABLE table = {0};

	s = socket(AF_INET, SOCK_DGRAM, 0);
	strncpy(iwr.ifr_name, "rai0", IFNAMSIZ);
	iwr.u.data.pointer = (caddr_t) &table;

	if (s < 0) {
		websError(wp, 500, "ioctl sock failed!");
		return -1;
	}

	if (ioctl(s, RTPRIV_IOCTL_GET_MAC_TABLE, &iwr) < 0) {
		websError(wp, 500, "ioctl -> RTPRIV_IOCTL_GET_MAC_TABLE failed!");
		close(s);
		return -1;
	}

	for (i = 0; i < table.Num; i++) {
		websWrite(wp, T("<tr><td>%02X:%02X:%02X:%02X:%02X:%02X</td>"),
				table.Entry[i].Addr[0], table.Entry[i].Addr[1],
				table.Entry[i].Addr[2], table.Entry[i].Addr[3],
				table.Entry[i].Addr[4], table.Entry[i].Addr[5]);
		websWrite(wp, T("<td>%d</td><td>%d</td><td>%d</td>"),
				table.Entry[i].Aid, table.Entry[i].Psm, table.Entry[i].MimoPs);
		websWrite(wp, T("<td>%d</td><td>%s</td><td>%d</td><td>%d</td></tr>"),
				table.Entry[i].TxRate.field.MCS,
				(table.Entry[i].TxRate.field.BW == 0)? "20M":"40M",
				table.Entry[i].TxRate.field.ShortGI, table.Entry[i].TxRate.field.STBC);
	}
	close(s);
	return 0;
}

// Tommy, 2009/3/2 02:31 Asssociation Time
#define YEAR 1970
typedef struct
{
	int year;
	int month;
	int day;
	int hour;
	int min;
	int sec;
} date;

const int days[12]={31,28,31,30,31,30,31,31,30,31,30,31};

/* Adjust Leap Year or not? */
int isLeapYear(int year)
{
	if(((year%4==0)&&(year%100!=0)) || (year%400==0)){
		return 1;
	}
	return 0;
}

date sec2date(long sec)
{
	date d={YEAR,1,1,0,0,0};
	//int ds,sum;
	int ds,sum,i;
//printf("---> @@@<sec2date> sec = %d\n",sec);	

	ds=sec/(24*60*60);
//printf("@@@<sec2date> ds = %d\n",ds);
	sec-=ds*(24*60*60);
//printf("@@@<sec2date> sec = %d\n",sec);	
	d.hour=sec/(60*60);
//printf("@@@<sec2date> d.hour = %d\n",d.hour);
	sec-=d.hour*(60*60);
//printf("@@@<sec2date> sec = %d\n",sec);
	d.min=sec/60;
//printf("@@@<sec2date> d.min = %d\n",d.min);	
	sec-=d.min*60;
//printf("@@@<sec2date> sec = %d\n",sec);	
	d.sec=sec;
//printf("@@@<sec2date> d.sec = %d\n",d.sec);

//printf("---> day & month @@@<sec2date> ds = %d\n",ds);
	while(1){
		sum=365;
		if(isLeapYear(d.year)){
			sum+=1;
		}
		
		if(ds<sum){
			break;
		}
	
		d.year++;
		ds-=sum;
	}
//printf("@@@<sec2date> year = %d\n",d.year);	

//printf("@@@d.month = %d\n",d.month);
//printf("@@@days[d.month-1] = %d\n",days[d.month-1]);
//printf("@@@<sec2date> ds = %d\n",ds);

	ds++;
	//if (ds == 0){
	//	return d;		
	//}	
	
	
	while(1){
		sum=days[d.month-1];
		//sum=days[d.month-1];
		if(d.month==2){
			if(isLeapYear(d.year)){
				sum+=1;
			}
		}
		
		//if((ds+1) < sum){
		if(ds <= sum){			
			//d.day+=ds;
			d.day=ds;
			break;
		}
		
		d.month++;
		ds-=sum;
	}
//printf("@@@d.day = %d\n",d.day);
//printf("@@@d.month = %d\n",d.month);	
	
	return d;
}

static int getmacrepeaterStaInfo(int eid, webs_t wp, int argc, char_t **argv)
{

         char mode[10]={0}; 
         char addr_mode[10]={0}; 
         char macrepeater_enable[10]={0};

          ezplib_get_attr_val("wl0_apcli_rule", 0, "macrepeater", macrepeater_enable, 10, EZPLIB_USE_CLI);
          ezplib_get_attr_val("wl0_apcli_rule", 0, "addrmode", addr_mode, 10, EZPLIB_USE_CLI);
          ezplib_get_attr_val("wl_mode_rule", 0, "mode", mode, 10, EZPLIB_USE_CLI);
          printf("### %s %s %s ...................................................................\n",mode,addr_mode,macrepeater_enable);
          if(0==strcmp(mode,"ur") && 0==strcmp(addr_mode,"0") && 0==strcmp(macrepeater_enable,"1"))
          {
                  int i=0;
                  FILE * fp1=NULL;
                  char buff1[20]={0};
                
                 system("iwpriv rai0 stat > /etc/stat.dat ");
                 system("awk 'n==1{print} $0~/MIMOPS/{n=1}'  /etc/stat.dat > /etc/tmp.dat");
                 system("awk '{print $1}' /etc/tmp.dat > /etc/macrepeater.dat ");

                 if (NULL == (fp1 = fopen("/etc/macrepeater.dat", "r")))
            	   {
            		return 0;
            	   }

                  while(!feof(fp1))  
               {  
                        fgets(buff1,sizeof(buff1),fp1); 

                        if((buff1[2]!=':') ||(buff1[5]!=':')||(buff1[8]!=':')||(buff1[11]!=':')||(buff1[14]!=':'))
                        {
                            continue;
                        }
                        websWrite(wp, T("<li class=\"w_text\">"));
                        websWrite(wp, T("<table width=\"95%%\" border=\"0\" align=\"center\" cellpadding=\"0\" cellspacing=\"0\">"));
                        websWrite(wp, T("<tr >"));
                        websWrite(wp, T("<td  width=\"20%%\"><center><span class='table_left'>%d</span></center></td>"),i+1);           
                        websWrite(wp, T("<td  width=\"80%%\"><center><span class='table_font'>%s</span></center></td>"),buff1);
                        //websWrite(wp, T("<td  width=\"45%%\"><center><span class='table_font'>%s</span></center></td>"),buff2);       
                        websWrite(wp, T("</span></center></td>"));
                        websWrite(wp, T("</tr>"));                                                 
                        websWrite(wp, T("</table>"));
                        websWrite(wp, T("</li>"));
                        memset(buff1,0,20);
                        i++;
                }
               fclose(fp1);
               system("rm  /etc/tmp.dat");
               system("rm  /etc/macrepeater.dat");
               return 0;
             
          }
          else
          {
                  int i=0;
                  FILE * fp1=NULL;
                  FILE * fp2=NULL;
                  char buff1[20]={0};
                  char buff2[20]={0};
                  char buff3[20]={0};
                  char *lan_proto=NULL;
                
                 system("iwpriv rai0 stat > /etc/stat.dat ");
                 system("awk 'n==1{print} $0~/MAT information:/{n=1}'  /etc/stat.dat > /etc/tmp.dat");
                 system("awk '{print $2}' /etc/tmp.dat > /etc/mac.dat ");
                 system("awk '{print $3}' /etc/tmp.dat > /etc/ip.dat ");

                 lan_proto=nvram_get("lan0_proto");
                 if(0==strcmp(lan_proto,"static"))
                 {
                    ezplib_get_attr_val("lan_static_rule", 0, "ipaddr", buff3, 20, EZPLIB_USE_CLI);
                   
                 }
                 else
                 {
                   ezplib_get_attr_val("lan_dhcps_rule", 0, "lan0_fallback_ip", buff3, 20, EZPLIB_USE_CLI); 
                 }
               printf("%s %s........................\n",lan_proto,buff3);

               if (NULL == (fp1 = fopen("/etc/mac.dat", "r")))
            	{
            		return 0;
            	}
               if (NULL == (fp2 = fopen("/etc/ip.dat", "r")))
            	{
                      fclose(fp1);
            		return 0;
            	}
               
               while((!feof(fp1))&&(!feof(fp2)))  
               {  
                        fgets(buff1,sizeof(buff1),fp1); 
                        fgets(buff2,sizeof(buff2),fp2);
                        if (0==strcmp(buff2,buff3))
                        {
                            printf("3..........................\n");
                        }
                        if((buff1[2]!=':') ||(buff1[5]!=':')||(buff1[8]!=':')||(buff1[11]!=':')||(buff1[14]!=':')||(0==strcmp(buff2,buff3)))
                        {
                            continue;
                        }
                        websWrite(wp, T("<li class=\"w_text\">"));
                        websWrite(wp, T("<table width=\"95%%\" border=\"0\" align=\"center\" cellpadding=\"0\" cellspacing=\"0\">"));
                        websWrite(wp, T("<tr >"));
                        websWrite(wp, T("<td  width=\"10%%\"><center><span class='table_left'>%d</span></center></td>"),i+1);           
                        websWrite(wp, T("<td  width=\"45%%\"><center><span class='table_font'>%s</span></center></td>"),buff1);
                        websWrite(wp, T("<td  width=\"45%%\"><center><span class='table_font'>%s</span></center></td>"),buff2);       
                        websWrite(wp, T("</span></center></td>"));
                        websWrite(wp, T("</tr>"));                                                 
                        websWrite(wp, T("</table>"));
                        websWrite(wp, T("</li>"));
                        memset(buff1,0,20);
                        memset(buff2,0,20);
                        i++;
                }
               fclose(fp1);
               fclose(fp2);
               system("rm  /etc/tmp.dat");
               system("rm  /etc/mac.dat");
               system("rm  /etc/ip.dat");
               return 0;
          }
    }

/*
typedef struct StationInfo_s
{
	unsigned char Addr[MAC_ADDR_LENGTH];
	unsigned int  ConnectedTime;
}StationInfo_t;

typedef struct StationListInfo_s
{
	int sta_num;
	StationInfo_t sta_list[STATIONLIST_MAX_2G];
}StationListInfo_t;
*/
static int getTimeZone()
{
	int timezone = 0;

	char TempBuf[32];
	ezplib_get_attr_val("ntp_rule", 0, "zone", TempBuf, 32, EZPLIB_USE_CLI);

	if (!strcmp(TempBuf, "UTC12")){
		timezone = 60 * 60 * (-12);
	}else if (!strcmp(TempBuf, "UTC11")){
		timezone = 60 * 60 * (-11);
	}else if (!strcmp(TempBuf, "UTC10")){
		timezone = 60 * 60 * (-10);
	}else if (!strcmp(TempBuf, "NAST9NADT,M3.2.0/2,M11.1.0/2")){
		timezone = 60 * 60 * (-9);
	}else if (!strcmp(TempBuf, "PST8PDT,M3.2.0/2,M11.1.0/2")){
		timezone = 60 * 60 * (-8);
	}else if (!strcmp(TempBuf, "UTC7")){
		timezone = 60 * 60 * (-7);
	}else if (!strcmp(TempBuf, "UTC6")){
		timezone = 60 * 60 * (-6);
	}else if (!strcmp(TempBuf, "CST6CDT,M3.2.0/2,M11.1.0/2")){
		timezone = 60 * 60 * (-6);
	}else if (!strcmp(TempBuf, "UTC5")){
		timezone = 60 * 60 * (-5);
	}else if (!strcmp(TempBuf, "EST5EDT,M3.2.0/2,M11.1.0/2")){
		timezone = 60 * 60 * (-5);
	}else if (!strcmp(TempBuf, "UTC4")){
		timezone = 60 * 60 * (-4);
	}else if (!strcmp(TempBuf, "AST4ADT,M3.2.0/2,M11.1.0/2")){
		timezone = 60 * 60 * (-4);
	}else if (!strcmp(TempBuf, "BRWST4BRWDT,M10.3.0/0,M2.5.0/0")){
		timezone = 60 * 60 * (-4);
	}else if (!strcmp(TempBuf, "NST3:30NDT,M3.2.0/0:01,M11.1.0/0:01")){
		timezone = 60 * 60 * (-3);
	}else if (!strcmp(TempBuf, "WGST3WGDT,M3.5.6/22,M10.5.6/23")){
		timezone = 60 * 60 * (-3);
	}else if (!strcmp(TempBuf, "BRST3BRDT,M10.3.0/0,M2.5.0/0")){
		timezone = 60 * 60 * (-3);
	}else if (!strcmp(TempBuf, "UTC3")){
		timezone = 60 * 60 * (-10);
	}else if (!strcmp(TempBuf, "UTC2")){
		timezone = 60 * 60 * (-2);
	}else if (!strcmp(TempBuf, "STD1DST,M3.5.0/2,M10.5.0/2")){
		timezone = 60 * 60 * (-1);
	}else if (!strcmp(TempBuf, "UTC0")){
		timezone = 60 * 60 * 0;
	}else if (!strcmp(TempBuf, "GMT0BST,M3.5.0/2,M10.5.0/2")){
		timezone = 60 * 60 * 0;
	}else if (!strcmp(TempBuf, "UTC-1")){
		timezone = 60 * 60 * 1;
	}else if (!strcmp(TempBuf, "STD-1DST,M3.5.0/2,M10.5.0/2")){
		timezone = 60 * 60 * 1;
	}else if (!strcmp(TempBuf, "UTC-2")){
		timezone = 60 * 60 * 2;
	}else if (!strcmp(TempBuf, "STD-2DST,M3.5.0/2,M10.5.0/2")){
		timezone = 60 * 60 * 2;
	}else if (!strcmp(TempBuf, "UTC-3")){
		timezone = 60 * 60 * 3;
	}else if (!strcmp(TempBuf, "UTC-4")){
		timezone = 60 * 60 * 4;
	}else if (!strcmp(TempBuf, "UTC-5")){
		timezone = 60 * 60 * 5;
	}else if (!strcmp(TempBuf, "UTC-5:30")){
		timezone = 19800 ;
	}else if (!strcmp(TempBuf, "UTC-6")){
		timezone = 60 * 60 * 6;
	}else if (!strcmp(TempBuf, "UTC-7")){
		timezone = 60 * 60 * 7;
	}else if (!strcmp(TempBuf, "UTC-8")){
		timezone = 60 * 60 * 8;
	}else if (!strcmp(TempBuf, "UTC-9")){
		timezone = 60 * 60 * 9;
	}else if (!strcmp(TempBuf, "CST-9:30CST,M10.5.0/2,M3.5.0/3")){
		timezone = 60 * 60 * 9;
	}else if (!strcmp(TempBuf, "UTC-10")){
		timezone = 60 * 60 * 10;
	}else if (!strcmp(TempBuf, "STD-10DST,M10.5.0/2,M3.5.0/2")){
		timezone = 60 * 60 * 10;
	}else if (!strcmp(TempBuf, "UTC-11")){
		timezone = 60 * 60 * 11;
	}else if (!strcmp(TempBuf, "UTC-12")){
		timezone = 60 * 60 * 12;
	}else if (!strcmp(TempBuf, "STD-12DST,M10.5.0/2,M3.5.0/2")){
		timezone = 60 * 60 * 12;
	}
	return timezone;
}

// Tommy , 2009/1/21 05:43
static int getwifiStaInfo(int eid, webs_t wp, int argc, char_t **argv)
{
	int i;
	long sec;
	date d;
	int timezone = getTimeZone();

	StationListInfo_t sli;
	//feak
	sli.sta_num=12;
	srandom(time(NULL));
	for (i = 0; i < sli.sta_num; i++) {
		sprintf(sli.sta_list[i].Addr, "%02X:%02X:%02X:%02X:%02X:%02X", 
			random()%256, random()%256, random()%256, 
			random()%256, random()%256, random()%256 );
		sli.sta_list[i].ConnectedTime = time(NULL) + random()%24*3600;
	}

	for (i = 0; i < sli.sta_num; i++) {
		websWrite(wp, T("<li class=\"w_text\">"));
		websWrite(wp, T("<table width=\"95%%\" border=\"0\" align=\"center\" cellpadding=\"0\" cellspacing=\"0\">"));
		websWrite(wp, T("<tr >"));
		websWrite(wp, T("<td  width=\"10%%\"><center><span class='table_left'>%d</span></center></td>"),
				i+1);			
		websWrite(wp, T("<td  width=\"30%%\"><center><span class='table_font'>%s</span></center></td>"),
				sli.sta_list[i].Addr);
		websWrite(wp, T("<td  width=\"65%%\"><center><span class='table_right'>"));

		sec = sli.sta_list[i].ConnectedTime;
		// Deal with time zone	
		sec = sec + timezone;
		d=sec2date(sec);
		websWrite(wp, T("%s%d:%s%d:%s%d "), (d.hour < 10) ? "0" : "", d.hour,
				(d.min < 10) ? "0" : "", d.min,
				(d.sec < 10) ? "0" : "", d.sec);
		websWrite(wp, T("</span></center></td>"));
		websWrite(wp, T("</tr>"));  			                                   
		websWrite(wp, T("</table>"));
		websWrite(wp, T("</li>"));
	}

	return 0;
}

/* definition of radar detection */
#define RD_NORMAL_MODE				0	/* Not found radar signal */
#define RD_SWITCHING_MODE			1	/* Found radar signal, and doing channel switch */
#define RD_SILENCE_MODE				2	/* After channel switch, need to be silence a while to ensure radar not found */


static int getwifi5GRadarStatus(int eid, webs_t wp, int argc, char_t **argv)
{
	int i, s;
	struct iwreq iwr;
	int radar_status = 0;
	char TempBuf[32];
	ezplib_get_attr_val("board_model_rule", 0, "bd_model", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
	if(strcmp(TempBuf,"0A52") != 0)
	{
		websWrite(wp, T("%d") ,radar_status);
		return 0;
	}
	s = socket(AF_INET, SOCK_DGRAM, 0);
	strncpy(iwr.ifr_name, "ra0", IFNAMSIZ);
	iwr.u.data.pointer = (caddr_t) &radar_status;

	if (s < 0) {
             printf("%s====>%d\n",__func__,__LINE__);
		websError(wp, 500, "ioctl sock failed!");
		return -1;
	}
	if (ioctl(s, RTPRIV_IOCTL_GET_RADAR_STATUS, &iwr) < 0) {
            printf("%s====>%d\n",__func__,__LINE__);
		websError(wp, 500, "ioctl -> RTPRIV_IOCTL_GET_RADAR_STATUS failed!");
		close(s);
		return -1;
	}
       printf("%s====>%d radar_status=%d\n",__func__,__LINE__,radar_status);
       if(radar_status ==  RD_SWITCHING_MODE || radar_status == RD_SILENCE_MODE){

	 }else if(radar_status == RD_NORMAL_MODE){

	 }
	 websWrite(wp, T("%d") ,radar_status);
	close(s);
	return 0;
}

static int getwifi5GStaInfo(int eid, webs_t wp, int argc, char_t **argv)
{
	int updays, uphours, upminutes, upseconds;
	unsigned long conntimes;	
	int i, s;
	struct iwreq iwr;
	RT1_802_11_MAC_TABLE table = {0};
	s = socket(AF_INET, SOCK_DGRAM, 0);
	strncpy(iwr.ifr_name, "ra0", IFNAMSIZ);
	iwr.u.data.pointer = (caddr_t) &table;

	if (s < 0) {
             //printf("%s====>%d\n",__func__,__LINE__);
		websError(wp, 500, "ioctl sock failed!");
		return -1;
	}

	if (ioctl(s, RTPRIV_IOCTL_GET_MAC_TABLE, &iwr) < 0) {
            // printf("%s====>%d\n",__func__,__LINE__);
		websError(wp, 500, "ioctl -> RTPRIV_IOCTL_GET_MAC_TABLE failed!");
		close(s);
		return -1;
	}
       //printf("%s====>%d table.Num=%lu\n",__func__,__LINE__,table.Num);
#if 1
	for (i = 0; i < table.Num; i++) {
		websWrite(wp, T("<li class=\"w_text\">"));
		websWrite(wp, T("<table width=\"95%%\" border=\"0\" align=\"center\" cellpadding=\"0\" cellspacing=\"0\">"));
		websWrite(wp, T("<tr >"));
		websWrite(wp, T("<td  width=\"10%%\"><center><span class='table_left'>%d</span></center></td>"),
				i+1);			
		websWrite(wp, T("<td  width=\"30%%\"><center><span class='table_font'>%02X:%02X:%02X:%02X:%02X:%02X</span></center></td>"),
				table.Entry[i].Addr[0], table.Entry[i].Addr[1],
				table.Entry[i].Addr[2], table.Entry[i].Addr[3],
				table.Entry[i].Addr[4], table.Entry[i].Addr[5]);
#if 1
		websWrite(wp, T("<td  width=\"65%%\"><center><span class='table_right'>"));
	long sec;
	date d;
	int timezone;
	
#if 1
		char TempBuf[32];
		
		ezplib_get_attr_val("ntp_rule", 0, "zone", TempBuf, 32, EZPLIB_USE_CLI);
		
		if (!strcmp(TempBuf, "UTC12")){
			timezone = 60 * 60 * (-12);
		}else if (!strcmp(TempBuf, "UTC11")){
			timezone = 60 * 60 * (-11);
		}else if (!strcmp(TempBuf, "UTC10")){
			timezone = 60 * 60 * (-10);
		}else if (!strcmp(TempBuf, "NAST9NADT,M3.2.0/2,M11.1.0/2")){
			timezone = 60 * 60 * (-9);
		}else if (!strcmp(TempBuf, "PST8PDT,M3.2.0/2,M11.1.0/2")){
			timezone = 60 * 60 * (-8);
		}else if (!strcmp(TempBuf, "UTC7")){
			timezone = 60 * 60 * (-7);
		}else if (!strcmp(TempBuf, "UTC6")){
			timezone = 60 * 60 * (-6);
		}else if (!strcmp(TempBuf, "CST6CDT,M3.2.0/2,M11.1.0/2")){
			timezone = 60 * 60 * (-6);
		}else if (!strcmp(TempBuf, "UTC5")){
			timezone = 60 * 60 * (-5);
		}else if (!strcmp(TempBuf, "EST5EDT,M3.2.0/2,M11.1.0/2")){
			timezone = 60 * 60 * (-5);
		}else if (!strcmp(TempBuf, "UTC4")){
			timezone = 60 * 60 * (-4);
		}else if (!strcmp(TempBuf, "AST4ADT,M3.2.0/2,M11.1.0/2")){
			timezone = 60 * 60 * (-4);
		}else if (!strcmp(TempBuf, "BRWST4BRWDT,M10.3.0/0,M2.5.0/0")){
			timezone = 60 * 60 * (-4);
		}else if (!strcmp(TempBuf, "NST3:30NDT,M3.2.0/0:01,M11.1.0/0:01")){
			timezone = 60 * 60 * (-3);
		}else if (!strcmp(TempBuf, "WGST3WGDT,M3.5.6/22,M10.5.6/23")){
			timezone = 60 * 60 * (-3);
		}else if (!strcmp(TempBuf, "BRST3BRDT,M10.3.0/0,M2.5.0/0")){
			timezone = 60 * 60 * (-3);
		}else if (!strcmp(TempBuf, "UTC3")){
			timezone = 60 * 60 * (-10);
		}else if (!strcmp(TempBuf, "UTC2")){
			timezone = 60 * 60 * (-2);
		}else if (!strcmp(TempBuf, "STD1DST,M3.5.0/2,M10.5.0/2")){
			timezone = 60 * 60 * (-1);
		}else if (!strcmp(TempBuf, "UTC0")){
			timezone = 60 * 60 * 0;
		}else if (!strcmp(TempBuf, "GMT0BST,M3.5.0/2,M10.5.0/2")){
			timezone = 60 * 60 * 0;
		}else if (!strcmp(TempBuf, "UTC-1")){
			timezone = 60 * 60 * 1;
		}else if (!strcmp(TempBuf, "STD-1DST,M3.5.0/2,M10.5.0/2")){
			timezone = 60 * 60 * 1;
		}else if (!strcmp(TempBuf, "UTC-2")){
			timezone = 60 * 60 * 2;
		}else if (!strcmp(TempBuf, "STD-2DST,M3.5.0/2,M10.5.0/2")){
			timezone = 60 * 60 * 2;
		}else if (!strcmp(TempBuf, "UTC-3")){
			timezone = 60 * 60 * 3;
		}else if (!strcmp(TempBuf, "UTC-4")){
			timezone = 60 * 60 * 4;
		}else if (!strcmp(TempBuf, "UTC-5")){
			timezone = 60 * 60 * 5;
		}else if (!strcmp(TempBuf, "UTC-5:30")){
			timezone = 19800 ;
		}else if (!strcmp(TempBuf, "UTC-6")){
			timezone = 60 * 60 * 6;
		}else if (!strcmp(TempBuf, "UTC-7")){
			timezone = 60 * 60 * 7;
		}else if (!strcmp(TempBuf, "UTC-8")){
			timezone = 60 * 60 * 8;
		}else if (!strcmp(TempBuf, "UTC-9")){
			timezone = 60 * 60 * 9;
		}else if (!strcmp(TempBuf, "CST-9:30CST,M10.5.0/2,M3.5.0/3")){
			timezone = 60 * 60 * 9;
		}else if (!strcmp(TempBuf, "UTC-10")){
			timezone = 60 * 60 * 10;
		}else if (!strcmp(TempBuf, "STD-10DST,M10.5.0/2,M3.5.0/2")){
			timezone = 60 * 60 * 10;
		}else if (!strcmp(TempBuf, "UTC-11")){
			timezone = 60 * 60 * 11;
		}else if (!strcmp(TempBuf, "UTC-12")){
			timezone = 60 * 60 * 12;
		}else if (!strcmp(TempBuf, "STD-12DST,M10.5.0/2,M3.5.0/2")){
			timezone = 60 * 60 * 12;
		}
#else	
	//char *tz = nvram_bufget(RT2860_NVRAM, "TZ");
	char *tz;
	
	if (!strcmp(tz, "UCT_-11")){
		timezone = 60 * 60 * (-11);
	}else if (!strcmp(tz, "UCT_-10")){
		timezone = 60 * 60 * (-10);
	}else if (!strcmp(tz, "NAS_-09")){
		timezone = 60 * 60 * (-9);
	}else if (!strcmp(tz, "PST_-08")){
		timezone = 60 * 60 * (-8);
	}else if (!strcmp(tz, "MST_-07")){
		timezone = 60 * 60 * (-7);
	}else if ( (!strcmp(tz, "CST_-06")) || (!strcmp(tz, "UCT_-06")) ){
		timezone = 60 * 60 * (-6);
	}else if ( (!strcmp(tz, "UCT_-05")) || (!strcmp(tz, "EST_-05")) ){
		timezone = 60 * 60 * (-5);
	}else if ( (!strcmp(tz, "AST_-04")) || (!strcmp(tz, "UCT_-04")) ){
		timezone = 60 * 60 * (-4);
	}else if ( (!strcmp(tz, "UCT_-03")) || (!strcmp(tz, "EBS_-03")) ){
		timezone = 60 * 60 * (-3);
	}else if (!strcmp(tz, "NOR_-02")){
		timezone = 60 * 60 * (-2);
	}else if (!strcmp(tz, "EUT_-01")){
		timezone = 60 * 60 * (-1);
	}else if ( (!strcmp(tz, "UCT_000")) || (!strcmp(tz, "GMT_000")) ){
		timezone = 60 * 60 * 0;
	}else if ( (!strcmp(tz, "MET_001")) || (!strcmp(tz, "MEZ_001")) || (!strcmp(tz, "UCT_001")) ){
		timezone = 60 * 60 * 1;
	}else if ( (!strcmp(tz, "EET_002")) || (!strcmp(tz, "SAS_002")) ){
		timezone = 60 * 60 * 2;
	}else if ( (!strcmp(tz, "IST_003")) || (!strcmp(tz, "MSK_003")) ){
		timezone = 60 * 60 * 3;		
	}else if (!strcmp(tz, "UCT_004")){
		timezone = 60 * 60 * 4;
	}else if (!strcmp(tz, "UCT_005")){
		timezone = 60 * 60 * 5;
	}else if (!strcmp(tz, "UCT_006")){
		timezone = 60 * 60 * 6;
	}else if (!strcmp(tz, "UCT_007")){
		timezone = 60 * 60 * 7;
	}else if ( (!strcmp(tz, "CST_008")) || (!strcmp(tz, "CCT_008")) || (!strcmp(tz, "SST_008")) || (!strcmp(tz, "AWS_008")) ){
		timezone = 60 * 60 * 8;																								
	}else if ( (!strcmp(tz, "JST_009")) || (!strcmp(tz, "KST_009")) ){
		timezone = 60 * 60 * 9;
	}else if ( (!strcmp(tz, "UCT_010")) || (!strcmp(tz, "AES_010"))){
		timezone = 60 * 60 * 10;
	}else if (!strcmp(tz, "UCT_011")){
		timezone = 60 * 60 * 11;		
	}else if ( (!strcmp(tz, "UCT_012")) || (!strcmp(tz, "NZS_012")) ){
		timezone = 60 * 60 * 12;
	}
#endif
	sec = table.Entry[i].ConnectedTime - 1; // -1 : if (pEntry->StaConnectTime == 1){ } in linux-2.6.21.x\drivers\net\wireless\rt2860v2\ap\ap.c
	
// Deal with time zone	
	sec = sec + timezone;

	d=sec2date(sec);
	
#if 0
printf("%s%d:%s%d:%s%d %d/%s%d/%s%d", (d.hour < 10) ? "0" : "", d.hour,
					(d.min < 10) ? "0" : "", d.min,
					(d.sec < 10) ? "0" : "", d.sec,
					 d.year,
					(d.month < 10) ? "0" : "", d.month,
					(d.day < 10) ? "0" : "", d.day);	
#endif	
	//printf("##### The date of AssociationTime: %d-%d-%d %d:%d:%d\n",d.year,d.month,d.day,d.hour,d.min,d.sec);
	//websWrite(wp, T("%d:%d:%d %d/%d/%d"), d.hour,d.min,d.sec,d.year,d.month,d.day);
	websWrite(wp, T("%s%d:%s%d:%s%d %d/%s%d/%s%d"), (d.hour < 10) ? "0" : "", d.hour,
							(d.min < 10) ? "0" : "", d.min,
							(d.sec < 10) ? "0" : "", d.sec,
							 d.year,
							(d.month < 10) ? "0" : "", d.month,
							(d.day < 10) ? "0" : "", d.day);
       //printf("%s====>%d\n",__func__,__LINE__);
#else
// Tommy , add Sation Connected Time ,2009/1/20 11:38
		conntimes = (unsigned long)table.Entry[i].ConnectedTime;

		updays = (int) conntimes / (60*60*24);
		upminutes = (int) conntimes / 60;
		uphours = (upminutes / 60) % 24;
		upminutes %= 60;
		upseconds = ((int) conntimes)%60;
	
		websWrite(wp, T("<td  width=\"65%%\"><center><span class='table_right'>"));
		
		if (updays){
			websWrite(wp, T("%d day%s, %d hour%s, %d min%s, %d sec%s") , 
			updays, (updays != 1) ? "s" : "",
			uphours, (uphours != 1) ? "s" : "",
			upminutes, (upminutes != 1) ? "s" : "",
			upseconds, (upseconds != 1) ? "s" : "");
		}else if (uphours){
			websWrite(wp, T("%d hour%s, %d min%s, %d sec%s") , 
			uphours, (uphours != 1) ? "s" : "",
			upminutes, (upminutes != 1) ? "s" : "",
			upseconds, (upseconds != 1) ? "s" : "");
		}else if (upminutes){
	        	websWrite(wp, T("%d min%s, %d sec%s") , 
			upminutes, (upminutes != 1) ? "s" : "",
			upseconds, (upseconds != 1) ? "s" : "");
		}else{
			websWrite(wp, T("%d sec%s") , 
			upseconds, (upseconds != 1) ? "s" : "");
		}	
#endif			
		websWrite(wp, T("</span></center></td>"));
		websWrite(wp, T("</tr>"));  			                                   
		websWrite(wp, T("</table>"));
		websWrite(wp, T("</li>"));
	}
	
#else
	for (i = 0; i < table.Num; i++) {
		websWrite(wp, T("<tr><td>%d</td>"),
				i);			
		websWrite(wp, T("<td>%02X:%02X:%02X:%02X:%02X:%02X</td>"),
				table.Entry[i].Addr[0], table.Entry[i].Addr[1],
				table.Entry[i].Addr[2], table.Entry[i].Addr[3],
				table.Entry[i].Addr[4], table.Entry[i].Addr[5]);
				
printf("MAC = %02X:%02X:%02X:%02X:%02X:%02X\n",table.Entry[i].Addr[0], table.Entry[i].Addr[1],
				table.Entry[i].Addr[2], table.Entry[i].Addr[3],
				table.Entry[i].Addr[4], table.Entry[i].Addr[5]);
								
		websWrite(wp, T("<td>%d</td><td>%d</td><td>%d</td>"),
				table.Entry[i].Aid, table.Entry[i].Psm, table.Entry[i].MimoPs);
		websWrite(wp, T("<td>%d</td><td>%s</td><td>%d</td><td>%d</td></tr>"),
				table.Entry[i].TxRate.field.MCS,
				(table.Entry[i].TxRate.field.BW == 0)? "20M":"40M",
				table.Entry[i].TxRate.field.ShortGI, table.Entry[i].TxRate.field.STBC);
// Tommy , add Sation Connected Time ,2009/1/20 11:38
printf("ConnectedTime = %10d\n",table.Entry[i].ConnectedTime);			
		websWrite(wp, T("<td>%10d</td>"),
				table.Entry[i].ConnectedTime);			
// Tommy , add association Time ,2009/1/20 11:38
printf("AssociationTime = %lu\n",table.Entry[i].AssociationTime);
		websWrite(wp, T("<td>%10lu</td></tr>"),table.Entry[i].AssociationTime); // sysuptime + association time				
	}
#endif	
	close(s);
	return 0;
}


static int getDLSBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#ifdef CONFIG_RT2860V2_AP_DLS
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

static int getWlanM2UBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#ifdef CONFIG_RT2860V2_AP_IGMP_SNOOP
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

/*
 * description: write the WDS Encryp Type
 */
// Tommy, Patch WDS for WEP, 2009/3/26 02:38 
#if 0
static int getWlanWdsEncType(int eid, webs_t wp, int argc, char_t **argv)
{
	char *value = nvram_bufget(RT2860_NVRAM, "WdsEncrypType");

	if (NULL == value)
		return websWrite(wp, T("0"));
	else if (strcmp(value, "NONE") == 0)
		return websWrite(wp, T("0"));
	else if (strcmp(value, "WEP") == 0)
		return websWrite(wp, T("1"));
	else if (strcmp(value, "TKIP") == 0)
		return websWrite(wp, T("2"));
	else if (strcmp(value, "AES") == 0)
		return websWrite(wp, T("3"));
	else
		return websWrite(wp, T("0"));
}
#endif
void replace_blankssid_value(int i)
{
	char acl_num[16], acl_basic[32], acl_rule[32];
	int num = i; 	
	
	ezplib_replace_rule("wl0_ssid_rule", num, "");
	ezplib_replace_rule("wl0_basic_rule", num, "0^0^0^1^1");
	ezplib_replace_rule("wl0_sec_rule", num, "disabled");
	ezplib_replace_rule("wl0_sec_wep_rule", num, "1^^^^^^open");
	ezplib_replace_rule("wl0_sec_wpa_rule", num, "^tkip^^^1812^disable^3600^5000^3000");
	ezplib_replace_rule("wl0_sec_wpa2_rule", num, "^aes^^^1812^disable^3600^5000^0^10^3000^1");

	sprintf(acl_num, "wl0%d_acl_num", num);
	nvram_set(acl_num, "0");
	
	sprintf(acl_basic, "wl0%d_acl_basic_rule", num);
	ezplib_replace_rule(acl_basic, 0, "1^disabled");

	sprintf(acl_rule, "wl0%d_acl_rule", num);
	nvram_set(acl_rule, "");
			
}
void replace_ssid_value(int newnum, int oldnum)
{
printf("ENTER replace ssid\n");
	char SSID[TMP_LEN], basic[TMP_LEN], WEP[TMP_LEN], WPA[TMP_LEN], WPA2[TMP_LEN], SEC[TMP_LEN], *AclNum, AclBasic[TMP_LEN], *AclRule;
	char old_acl_num[16], new_acl_num[16], old_acl_basic[32], new_acl_basic[32], old_acl_rule[32], new_acl_rule[32];
	int NewNum = newnum;
	int OldNum = oldnum;

//SSID
	ezplib_get_rule("wl0_ssid_rule", OldNum, SSID, TMP_LEN);
	ezplib_replace_rule("wl0_ssid_rule", NewNum, SSID);
	ezplib_replace_rule("wl0_ssid_rule", OldNum, "");
	
//enable^hidden^rate^wme^isolation	
	ezplib_get_rule("wl0_basic_rule", OldNum, basic, TMP_LEN);
	ezplib_replace_rule("wl0_basic_rule", NewNum, basic);
	ezplib_replace_rule("wl0_basic_rule", OldNum, "0^0^0^1^1");
	
//security mode
	ezplib_get_rule("wl0_sec_rule", OldNum, SEC, TMP_LEN);
	ezplib_replace_rule("wl0_sec_rule", NewNum, SEC);
	ezplib_replace_rule("wl0_sec_rule", OldNum, "disabled");
	
//WEP
	ezplib_get_rule("wl0_sec_wep_rule", OldNum, WEP, TMP_LEN);
	ezplib_replace_rule("wl0_sec_wep_rule", NewNum, WEP);
	ezplib_replace_rule("wl0_sec_wep_rule", OldNum, "1^^^^^^open");
	
//WPA	
	ezplib_get_rule("wl0_sec_wpa_rule", OldNum, WPA, TMP_LEN);
	ezplib_replace_rule("wl0_sec_wpa_rule", NewNum, WPA);
	ezplib_replace_rule("wl0_sec_wpa_rule", OldNum, "^tkip^^^1812^disable^3600^5000^3000");
	
//WPA2
	ezplib_get_rule("wl0_sec_wpa2_rule", OldNum, WPA2, TMP_LEN);
	ezplib_replace_rule("wl0_sec_wpa2_rule", NewNum, WPA2);
	ezplib_replace_rule("wl0_sec_wpa2_rule", OldNum, "^aes^^^1812^disable^3600^5000^0^10^3000^1");
	
//MAC Filter num
	sprintf(old_acl_num, "wl0%d_acl_num", OldNum);
	sprintf(new_acl_num, "wl0%d_acl_num", NewNum);		 
	AclNum = nvram_safe_get(old_acl_num);

	nvram_set(new_acl_num, AclNum);
	nvram_set(old_acl_num, "0");	

//MAC Filter basic
	sprintf(old_acl_basic, "wl0%d_acl_basic_rule", OldNum);
	sprintf(new_acl_basic, "wl0%d_acl_basic_rule", NewNum);		 
	ezplib_get_rule(old_acl_basic, 0, AclBasic, TMP_LEN);
	ezplib_replace_rule(new_acl_basic, 0, AclBasic);
	ezplib_replace_rule(old_acl_basic, 0, "1^disabled");
	
//MAC Filter rule
	sprintf(old_acl_rule, "wl0%d_acl_rule", OldNum);
	sprintf(new_acl_rule, "wl0%d_acl_rule", NewNum);
	AclRule = nvram_safe_get(old_acl_rule);
	nvram_set(new_acl_rule, AclRule);
	nvram_set(old_acl_rule, "");	
}

static void revise_mbss_value(int old_num, int new_num)
{
#if 0
	/* {{{ The parameters that support multiple BSSID is listed as followed,
	   1.) SSID,                 char SSID[33];
	   2.) AuthMode,             char AuthMode[14];
	   3.) EncrypType,           char EncrypType[8];
	   4.) WPAPSK,               char WPAPSK[65];
	   5.) DefaultKeyID,         int  DefaultKeyID;
	   6.) Key1Type,             int  Key1Type;
	   7.) Key1Str,              char Key1Str[27];
	   8.) Key2Type,             int  Key2Type;
	   9.) Key2Str,              char Key2Str[27];
	   10.) Key3Type,            int  Key3Type;
	   11.) Key3Str,             char Key3Str[27];
	   12.) Key4Type,            int  Key4Type;
	   13.) Key4Str,             char Key4Str[27];
	   14.) AccessPolicy,
	   15.) AccessControlList,
	   16.) NoForwarding,
	   17.) IEEE8021X,           int  IEEE8021X;
	   18.) TxRate,              int  TxRate;
	   19.) HideSSID,            int  HideSSID;
	   20.) PreAuth,             int  PreAuth;
	   21.) WmmCapable
	                             int  SecurityMode;
                             	 char VlanName[20];
	                             int  VlanId;
	                             int  VlanPriority;
	}}} */
	char new_value[264], *old_value, *p;
	int i;

#define MBSS_INIT(field, default_value) \
	do { \
		old_value = nvram_bufget(RT2860_NVRAM, #field); \
		snprintf(new_value, 264, "%s", old_value); \
		p = new_value + strlen(old_value); \
		for (i = old_num; i < new_num; i++) { \
			snprintf(p, 264 - (p - new_value), ";%s", default_value); \
			p += 1 + strlen(default_value); \
		} \
		nvram_bufset(RT2860_NVRAM, #field, new_value); \
	} while (0)

#define MBSS_REMOVE(field) \
	do { \
		old_value = nvram_bufget(RT2860_NVRAM, #field); \
		snprintf(new_value, 264, "%s", old_value); \
		p = new_value; \
		for (i = 0; i < new_num; i++) { \
			if (0 == i) \
				p = strchr(p, ';'); \
			else \
				p = strchr(p+1, ';'); \
			if (NULL == p) \
				break; \
		} \
		if (p) \
			*p = '\0'; \
		nvram_bufset(RT2860_NVRAM, #field, new_value); \
	} while (0)

	if (new_num > old_num) {
		//MBSS_INIT(SSID, "ssid");
		MBSS_INIT(AuthMode, "OPEN");
		MBSS_INIT(EncrypType, "NONE");
		//MBSS_INIT(WPAPSK, "12345678");
		MBSS_INIT(DefaultKeyID, "1");
		MBSS_INIT(Key1Type, "0");
		//MBSS_INIT(Key1Str, "");
		MBSS_INIT(Key2Type, "0");
		//MBSS_INIT(Key2Str, "");
		MBSS_INIT(Key3Type, "0");
		//MBSS_INIT(Key3Str, "");
		MBSS_INIT(Key4Type, "0");
		//MBSS_INIT(Key4Str, "");
/*		MBSS_INIT(AccessPolicy0, "0");
		MBSS_INIT(AccessControlList0, "");
		MBSS_INIT(AccessPolicy1, "0");
		MBSS_INIT(AccessControlList1, "");
		MBSS_INIT(AccessPolicy2, "0");
		MBSS_INIT(AccessControlList2, "");
		MBSS_INIT(AccessPolicy3, "0");
		MBSS_INIT(AccessControlList3, ""); */
		MBSS_INIT(NoForwarding, "0");
		MBSS_INIT(NoForwardingBTNBSSID, "0");
		MBSS_INIT(IEEE8021X, "0");
		MBSS_INIT(TxRate, "0");
		//MBSS_INIT(HideSSID, "0");
		MBSS_INIT(PreAuth, "0");
		MBSS_INIT(WmmCapable, "1");
		for (i = old_num + 1; i <= new_num; i++) {
			nvram_bufset(RT2860_NVRAM, racat("WPAPSK", i), "12345678");
			nvram_bufset(RT2860_NVRAM, racat("Key1Str", i), "");
			nvram_bufset(RT2860_NVRAM, racat("Key2Str", i), "");
			nvram_bufset(RT2860_NVRAM, racat("Key3Str", i), "");
			nvram_bufset(RT2860_NVRAM, racat("Key4Str", i), "");
			// The index of AccessPolicy & AccessControlList starts at 0.
			nvram_bufset(RT2860_NVRAM, racat("AccessPolicy", i-1), "0");
			nvram_bufset(RT2860_NVRAM, racat("AccessControlList", i-1), "");
		}
	}
	else if (new_num < old_num) {
		//MBSS_REMOVE(SSID);
		MBSS_REMOVE(AuthMode);
		MBSS_REMOVE(EncrypType);
		//MBSS_REMOVE(WPAPSK);
		MBSS_REMOVE(DefaultKeyID);
		MBSS_REMOVE(Key1Type);
		//MBSS_REMOVE(Key1Str);
		MBSS_REMOVE(Key2Type);
		//MBSS_REMOVE(Key2Str);
		MBSS_REMOVE(Key3Type);
		//MBSS_REMOVE(Key3Str);
		MBSS_REMOVE(Key4Type);
		//MBSS_REMOVE(Key4Str);
/*		MBSS_REMOVE(AccessPolicy0);
		MBSS_REMOVE(AccessControlList0);
		MBSS_REMOVE(AccessPolicy1);
		MBSS_REMOVE(AccessControlList1);
		MBSS_REMOVE(AccessPolicy2);
		MBSS_REMOVE(AccessControlList2);
		MBSS_REMOVE(AccessPolicy3);
		MBSS_REMOVE(AccessControlList3); */
		MBSS_REMOVE(NoForwarding);
		MBSS_REMOVE(NoForwardingBTNBSSID);
		MBSS_REMOVE(IEEE8021X);
		MBSS_REMOVE(TxRate);
		MBSS_REMOVE(HideSSID);
		MBSS_REMOVE(PreAuth);
		MBSS_REMOVE(WmmCapable);
		for (i = new_num + 1; i <= old_num; i++) {
			nvram_bufset(RT2860_NVRAM, racat("SSID", i), "");
			nvram_bufset(RT2860_NVRAM, racat("WPAPSK", i), "");
			nvram_bufset(RT2860_NVRAM, racat("Key1Str", i), "");
			nvram_bufset(RT2860_NVRAM, racat("Key2Str", i), "");
			nvram_bufset(RT2860_NVRAM, racat("Key3Str", i), "");
			nvram_bufset(RT2860_NVRAM, racat("Key4Str", i), "");
			// The index of AccessPolicy & AccessControlList starts at 0.
			nvram_bufset(RT2860_NVRAM, racat("AccessPolicy", i-1), "0");
			nvram_bufset(RT2860_NVRAM, racat("AccessControlList", i-1), "");
		}
	}
#endif
}

/* goform/wirelessBasic */
// Tommy, decrease code size , 2009/3/17 01:28
#if Include_Ralink_Original
static void wirelessBasic(webs_t wp, char_t *path, char_t *query)
{
	char_t	*wirelessmode;
	char_t	*ssid, *mssid_1, *mssid_2, *mssid_3, *mssid_4, *mssid_5, *mssid_6,
			*mssid_7, *bssid_num, *broadcastssid, *apisolated, *mbssidapisolated;
	char_t	*sz11aChannel, *sz11bChannel, *sz11gChannel, *abg_rate;
	char_t	*wds_list, *wds_mode, *wds_phy_mode, *wds_encryp_type, *wds_encryp_key;
	char_t	*n_mode, *n_bandwidth, *n_gi, *n_mcs, *n_rdg, *n_extcha, *n_amsdu;
	char_t	*n_autoba, *n_badecline;
	char_t	*tx_stream, *rx_stream;
	char_t	*radio;
	int i = 0, is_n = 0, new_bssid_num, old_bssid_num = 1;

	radio = websGetVar(wp, T("radiohiddenButton"), T("2"));
	if (!strncmp(radio, "0", 2)) {
		//doSystem("iwpriv rai0 set RadioOn=0");
		system("/sbin/ezp-wps-set 0 0 11 0");
		nvram_set(RT2860_NVRAM, "RadioOff", "1");
		websRedirect(wp, "wireless/basic.asp");
		return;
	}
	else if (!strncmp(radio, "1", 2)) {
		//doSystem("iwpriv rai0 set RadioOn=1");
		system("/sbin/ezp-wps-set 0 0 11 1");
		nvram_set(RT2860_NVRAM, "RadioOff", "0");
		websRedirect(wp, "wireless/basic.asp");
		return;
	}

	//fetch from web input
	wirelessmode = websGetVar(wp, T("wirelessmode"), T("9")); //9: bgn mode
	ssid = websGetVar(wp, T("ssid"), T("")); 
	mssid_1 = websGetVar(wp, T("mssid_1"), T("")); 
	mssid_2 = websGetVar(wp, T("mssid_2"), T("")); 
	mssid_3 = websGetVar(wp, T("mssid_3"), T("")); 
	mssid_4 = websGetVar(wp, T("mssid_4"), T("")); 
	mssid_5 = websGetVar(wp, T("mssid_5"), T("")); 
	mssid_6 = websGetVar(wp, T("mssid_6"), T("")); 
	mssid_7 = websGetVar(wp, T("mssid_7"), T("")); 
	bssid_num = websGetVar(wp, T("bssid_num"), T("1"));
	broadcastssid = websGetVar(wp, T("broadcastssid"), T("1")); 
	apisolated = websGetVar(wp, T("apisolated"), T("0"));
	mbssidapisolated = websGetVar(wp, T("mbssidapisolated"), T("0"));
	sz11aChannel = websGetVar(wp, T("sz11aChannel"), T("")); 
	sz11bChannel = websGetVar(wp, T("sz11bChannel"), T("")); 
	sz11gChannel = websGetVar(wp, T("sz11gChannel"), T("")); 
	//abg_rate = websGetVar(wp, T("abg_rate"), T("")); 
	wds_mode = websGetVar(wp, T("wds_mode"), T("0")); 
	wds_phy_mode = websGetVar(wp, T("wds_phy_mode"), T("0")); 
	wds_encryp_type = websGetVar(wp, T("wds_encryp_type"), T("0")); 
	wds_encryp_key = websGetVar(wp, T("wds_encryp_key"), T("0")); 
	wds_list = websGetVar(wp, T("wds_list"), T("")); 
	if (strlen(wds_list) > 0)
		wds_list[strlen(wds_list) - 1] = '\0';
	n_mode = websGetVar(wp, T("n_mode"), T("0"));
	n_bandwidth = websGetVar(wp, T("n_bandwidth"), T("0"));
	n_gi = websGetVar(wp, T("n_gi"), T("0"));
	n_mcs = websGetVar(wp, T("n_mcs"), T("33"));
	n_rdg = websGetVar(wp, T("n_rdg"), T("0"));
	n_extcha = websGetVar(wp, T("n_extcha"), T("0"));
	n_amsdu = websGetVar(wp, T("n_amsdu"), T("0"));
	n_autoba = websGetVar(wp, T("n_autoba"), T("0"));
	n_badecline = websGetVar(wp, T("n_badecline"), T("0"));
	tx_stream = websGetVar(wp, T("tx_stream"), T("0"));
	rx_stream = websGetVar(wp, T("rx_stream"), T("0"));

	old_bssid_num = atoi(nvram_bufget(RT2860_NVRAM, "BssidNum"));
	new_bssid_num = atoi(bssid_num);

	nvram_bufset(RT2860_NVRAM, "WirelessMode", wirelessmode);
	if (!strncmp(wirelessmode, "4", 2))
		nvram_bufset(RT2860_NVRAM, "BasicRate", "351");
	else if (!strncmp(wirelessmode, "8", 2) || !strncmp(wirelessmode, "9", 2))
		is_n = 1;

#if 0
	i = atoi(mode);
	if (i == 1)
		nvram_bufset(RT2860_NVRAM, "BasicRate", "3");
	else if (i == 0) {
		nvram_bufset(RT2860_NVRAM, "TxPreamble","0");
		nvram_bufset(RT2860_NVRAM, "TxBurst", "0");
		nvram_bufset(RT2860_NVRAM, "PktAggregate", "0");
	}
	else if (i == 5) {
		nvram_bufset(RT2860_NVRAM, "TxPreamble","1");
		nvram_bufset(RT2860_NVRAM, "TxBurst", "1");
		nvram_bufset(RT2860_NVRAM, "PktAggregate", "1");
		nvram_bufset(RT2860_NVRAM, "DisableOLBC", "1");
	}
	if (i != 5) {
		nvram_bufset(RT2860_NVRAM, "DisableOLBC", "0");
	}
#endif

	//SSID, Multiple SSID
	if (0 == strlen(ssid)) {
		nvram_commit(RT2860_NVRAM);
		websError(wp, 403, T("'SSID' should not be empty!"));
		return;
	}
	nvram_bufset(RT2860_NVRAM, "SSID1", ssid);

//#WPS
	{
		char *wordlist= nvram_bufget(RT2860_NVRAM, "WscModeOption");
		if(wordlist){
			if (strcmp(wordlist, "0"))
				//doSystem("iwpriv rai0 set WscConfStatus=1");
				system("/sbin/ezp-wps-set 0 0 1 1");
			nvram_bufset(RT2860_NVRAM, "WscConfigured", "1");
			g_wsc_configured = 1;
		}
	}

//#WPS
	default_shown_mbssid[RT2860_NVRAM] = 0;

	i = 2;
	if (0 != strlen(mssid_1)) {
		nvram_bufset(RT2860_NVRAM, racat("SSID", i), mssid_1);
		i++;
	}
	if (0 != strlen(mssid_2)) {
		nvram_bufset(RT2860_NVRAM, racat("SSID", i), mssid_2);
		i++;
	}
	if (0 != strlen(mssid_3)) {
		nvram_bufset(RT2860_NVRAM, racat("SSID", i), mssid_3);
		i++;
	}
	if (0 != strlen(mssid_4)) {
		nvram_bufset(RT2860_NVRAM, racat("SSID", i), mssid_4);
		i++;
	}
	if (0 != strlen(mssid_5)) {
		nvram_bufset(RT2860_NVRAM, racat("SSID", i), mssid_5);
		i++;
	}
	if (0 != strlen(mssid_6)) {
		nvram_bufset(RT2860_NVRAM, racat("SSID", i), mssid_6);
		i++;
	}
	if (0 != strlen(mssid_7)) {
		nvram_bufset(RT2860_NVRAM, racat("SSID", i), mssid_7);
		i++;
	}

	nvram_bufset(RT2860_NVRAM, "BssidNum", bssid_num);
	if (new_bssid_num < 1 || new_bssid_num > 8) {
		nvram_commit(RT2860_NVRAM);
		websError(wp, 403, T("'bssid_num' %s is out of range!"), bssid_num);
		return;
	}
	revise_mbss_value(old_bssid_num, new_bssid_num);

	//Broadcast SSID
	if (new_bssid_num == 1) {
		if (!strncmp(broadcastssid, "1", 2))
			nvram_bufset(RT2860_NVRAM, "HideSSID", "0");
		else
			nvram_bufset(RT2860_NVRAM, "HideSSID", "1");
	}
	else if (new_bssid_num == 2) {
		if (!strncmp(broadcastssid, "1", 2))
			nvram_bufset(RT2860_NVRAM, "HideSSID", "0;0");
		else
			nvram_bufset(RT2860_NVRAM, "HideSSID", "1;1");
	}
	else if (new_bssid_num == 3) {
		if (!strncmp(broadcastssid, "1", 2))
			nvram_bufset(RT2860_NVRAM, "HideSSID", "0;0;0");
		else
			nvram_bufset(RT2860_NVRAM, "HideSSID", "1;1;1");
	}
	else if (new_bssid_num == 4) {
		if (!strncmp(broadcastssid, "1", 2))
			nvram_bufset(RT2860_NVRAM, "HideSSID", "0;0;0;0");
		else
			nvram_bufset(RT2860_NVRAM, "HideSSID", "1;1;1;1");
	}
	else if (new_bssid_num == 5) {
		if (!strncmp(broadcastssid, "1", 2))
			nvram_bufset(RT2860_NVRAM, "HideSSID", "0;0;0;0;0");
		else
			nvram_bufset(RT2860_NVRAM, "HideSSID", "1;1;1;1;1");
	}
	else if (new_bssid_num == 6) {
		if (!strncmp(broadcastssid, "1", 2))
			nvram_bufset(RT2860_NVRAM, "HideSSID", "0;0;0;0;0;0");
		else
			nvram_bufset(RT2860_NVRAM, "HideSSID", "1;1;1;1;1;1");
	}
	else if (new_bssid_num == 7) {
		if (!strncmp(broadcastssid, "1", 2))
			nvram_bufset(RT2860_NVRAM, "HideSSID", "0;0;0;0;0;0;0");
		else
			nvram_bufset(RT2860_NVRAM, "HideSSID", "1;1;1;1;1;1;1");
	}
	else if (new_bssid_num == 8) {
		if (!strncmp(broadcastssid, "1", 2))
			nvram_bufset(RT2860_NVRAM, "HideSSID", "0;0;0;0;0;0;0;0");
		else
			nvram_bufset(RT2860_NVRAM, "HideSSID", "1;1;1;1;1;1;1;1");
	}

	// NoForwarding and NoForwardingBTNBSSID
	nvram_bufset(RT2860_NVRAM, "NoForwarding", apisolated);
	nvram_bufset(RT2860_NVRAM, "NoForwardingBTNBSSID", mbssidapisolated);

	//11abg Channel or AutoSelect
	if ((0 == strlen(sz11aChannel)) && (0 == strlen(sz11bChannel)) &&
			(0 == strlen(sz11gChannel))) {
		nvram_commit(RT2860_NVRAM);
		websError(wp, 403, T("'Channel' should not be empty!"));
		return;
	}
	if (!strncmp(sz11aChannel, "0", 2) && !strncmp(sz11bChannel, "0", 2) &&
			!strncmp(sz11gChannel, "0", 2))
		nvram_bufset(RT2860_NVRAM, "AutoChannelSelect", "1");
	else
		nvram_bufset(RT2860_NVRAM, "AutoChannelSelect", "0");
	if (0 != strlen(sz11aChannel))
		nvram_bufset(RT2860_NVRAM, "Channel", sz11aChannel);
	if (0 != strlen(sz11bChannel))
		nvram_bufset(RT2860_NVRAM, "Channel", sz11bChannel);
	if (0 != strlen(sz11gChannel))
		nvram_bufset(RT2860_NVRAM, "Channel", sz11gChannel);

	//Rate for a, b, g
	/*
	if (strncmp(abg_rate, "", 1)) {
		int rate = atoi(abg_rate);
		if (!strncmp(wirelessmode, "0", 2) || !strncmp(wirelessmode, "2", 2) || !strncmp(wirelessmode, "4", 2)) {
			if (rate == 1 || rate == 2 || rate == 5 || rate == 11)
				nvram_bufset(RT2860_NVRAM, "FixedTxMode", "CCK");
			else
				nvram_bufset(RT2860_NVRAM, "FixedTxMode", "OFDM");

			if (rate == 1)
				nvram_bufset(RT2860_NVRAM, "HT_MCS", "0");
			else if (rate == 2)
				nvram_bufset(RT2860_NVRAM, "HT_MCS", "1");
			else if (rate == 5)
				nvram_bufset(RT2860_NVRAM, "HT_MCS", "2");
			else if (rate == 6)
				nvram_bufset(RT2860_NVRAM, "HT_MCS", "0");
			else if (rate == 9)
				nvram_bufset(RT2860_NVRAM, "HT_MCS", "1");
			else if (rate == 11)
				nvram_bufset(RT2860_NVRAM, "HT_MCS", "3");
			else if (rate == 12)
				nvram_bufset(RT2860_NVRAM, "HT_MCS", "2");
			else if (rate == 18)
				nvram_bufset(RT2860_NVRAM, "HT_MCS", "3");
			else if (rate == 24)
				nvram_bufset(RT2860_NVRAM, "HT_MCS", "4");
			else if (rate == 36)
				nvram_bufset(RT2860_NVRAM, "HT_MCS", "5");
			else if (rate == 48)
				nvram_bufset(RT2860_NVRAM, "HT_MCS", "6");
			else if (rate == 54)
				nvram_bufset(RT2860_NVRAM, "HT_MCS", "7");
			else
				nvram_bufset(RT2860_NVRAM, "HT_MCS", "33");
		}
		else if (!strncmp(wirelessmode, "1", 2)) {
			nvram_bufset(RT2860_NVRAM, "FixedTxMode", "CCK");
			if (rate == 1)
				nvram_bufset(RT2860_NVRAM, "HT_MCS", "0");
			else if (rate == 2)
				nvram_bufset(RT2860_NVRAM, "HT_MCS", "1");
			else if (rate == 5)
				nvram_bufset(RT2860_NVRAM, "HT_MCS", "2");
			else if (rate == 11)
				nvram_bufset(RT2860_NVRAM, "HT_MCS", "3");
		}
		else //shall not happen
			error(E_L, E_LOG, T("wrong configurations from web UI"));
	}
	else
		nvram_bufset(RT2860_NVRAM, "FixedTxMode", "HT");
	*/

	//WdsEnable, WdsPhyMode, WdsEncrypType, WdsKey, WdsList
	//where WdsPhyMode - 0:CCK, 1:OFDM, 2:HTMIX, 3:GREENFIELD
	//      WdsEncryptType - NONE, WEP, TKIP, AES
	nvram_bufset(RT2860_NVRAM, "WdsEnable", wds_mode);
	if (strncmp(wds_mode, "0", 2)) {
		nvram_bufset(RT2860_NVRAM, "WdsPhyMode", wds_phy_mode);
		nvram_bufset(RT2860_NVRAM, "WdsEncrypType", wds_encryp_type);
		nvram_bufset(RT2860_NVRAM, "WdsKey", wds_encryp_key);
		if (!strncmp(wds_mode, "2", 2) || !strncmp(wds_mode, "3", 2)) {
			if (0 != strlen(wds_list))
				nvram_bufset(RT2860_NVRAM, "WdsList", wds_list);
		}
	}

	//HT_OpMode, HT_BW, HT_GI, HT_MCS, HT_RDG, HT_EXTCHA, HT_AMSDU, HT_TxStream, HT_RxStream
	if (is_n) {
		nvram_bufset(RT2860_NVRAM, "HT_OpMode", n_mode);
		nvram_bufset(RT2860_NVRAM, "HT_BW", n_bandwidth);
		nvram_bufset(RT2860_NVRAM, "HT_GI", n_gi);
		nvram_bufset(RT2860_NVRAM, "HT_MCS", n_mcs);
		nvram_bufset(RT2860_NVRAM, "HT_RDG", n_rdg);
		nvram_bufset(RT2860_NVRAM, "HT_EXTCHA", n_extcha);
		nvram_bufset(RT2860_NVRAM, "HT_AMSDU", n_amsdu);
		nvram_bufset(RT2860_NVRAM, "HT_AutoBA", n_autoba);
		nvram_bufset(RT2860_NVRAM, "HT_BADecline", n_badecline);
	}
	nvram_bufset(RT2860_NVRAM, "HT_TxStream", tx_stream);
	nvram_bufset(RT2860_NVRAM, "HT_RxStream", rx_stream);

	nvram_commit(RT2860_NVRAM);
#if CONFIG_RT2860V2_AP == CONFIG_MIPS
	/* this is a workaround:
	 *  when AP is built as kernel
	 *  if more ssids are created, driver won't exe RT28xx_MBSS_Init again
	 *  therefore, we reboot to make it available
	 *  (PS. CONFIG_MIPS would be "y")
	 */
	if (new_bssid_num > old_bssid_num)
		doSystem("reboot");
#endif
	initInternet();

	//debug print
	websHeader(wp);
	websWrite(wp, T("<h2>mode: %s</h2><br>\n"), wirelessmode);
	websWrite(wp, T("ssid: %s, bssid_num: %s<br>\n"), ssid, bssid_num);
	websWrite(wp, T("mssid_1: %s, mssid_2: %s, mssid_3: %s<br>\n"),
			mssid_1, mssid_2, mssid_3);
	websWrite(wp, T("mssid_4: %s, mssid_5: %s, mssid_6: %s, mssid_7: %s<br>\n"),
			mssid_4, mssid_5, mssid_6, mssid_7);
	websWrite(wp, T("broadcastssid: %s<br>\n"), broadcastssid);
	websWrite(wp, T("sz11aChannel: %s<br>\n"), sz11aChannel);
	websWrite(wp, T("sz11bChannel: %s<br>\n"), sz11bChannel);
	websWrite(wp, T("sz11gChannel: %s<br>\n"), sz11gChannel);
	/*
	if (strncmp(abg_rate, "", 1)) {
		websWrite(wp, T("abg_rate: %s<br>\n"), abg_rate);
	}
	*/
	websWrite(wp, T("wds_mode: %s<br>\n"), wds_mode);
	if (strncmp(wds_mode, "0", 2)) {
		websWrite(wp, T("wds_phy_mode: %s<br>\n"), wds_phy_mode);
		websWrite(wp, T("wds_encryp_type: %s<br>\n"), wds_encryp_type);
		websWrite(wp, T("wds_encryp_key: %s<br>\n"), wds_encryp_key);
		if (!strncmp(wds_mode, "2", 2) || !strncmp(wds_mode, "3", 2))
			websWrite(wp, T("wds_list: %s<br>\n"), wds_list);
	}
	if (is_n) {
		websWrite(wp, T("n_mode: %s<br>\n"), n_mode);
		websWrite(wp, T("n_bandwidth: %s<br>\n"), n_bandwidth);
		websWrite(wp, T("n_gi: %s<br>\n"), n_gi);
		websWrite(wp, T("n_mcs: %s<br>\n"), n_mcs);
		websWrite(wp, T("n_rdg: %s<br>\n"), n_rdg);
		websWrite(wp, T("n_extcha: %s<br>\n"), n_extcha);
		websWrite(wp, T("n_amsdu: %s<br>\n"), n_amsdu);
		websWrite(wp, T("n_autoba: %s<br>\n"), n_autoba);
		websWrite(wp, T("n_badecline: %s<br>\n"), n_badecline);
	}
	websWrite(wp, T("tx_stream: %s<br>\n"), tx_stream);
	websWrite(wp, T("rx_stream: %s<br>\n"), rx_stream);
	websFooter(wp);
	websDone(wp, 200);
}
#endif // #if Include_Ralink_Original


/* goform/wifiWDS */
static void wifiWDS(webs_t wp, char_t *path, char_t *query)
{
	char_t	*wds_mode, *wds_phy_mode, *wds_encryp_type, *wds_encryp_key, *wds_encryp_key0,
			*wds_encryp_key1,*wds_encryp_key2, *wds_encryp_key3, *wds_list;
	char_t *wds_1,*wds_2,*wds_3,*wds_4;

	wds_mode = websGetVar(wp, T("wds_mode"), T("0")); 
	wds_phy_mode = websGetVar(wp, T("wds_phy_mode"), T(""));
	wds_encryp_type = websGetVar(wp, T("wds_encryp_type"), T(""));
	wds_encryp_key = websGetVar(wp, T("wds_encryp_key"), T(""));
	wds_encryp_key0 = wds_encryp_key;
	wds_encryp_key1 = wds_encryp_key;
	wds_encryp_key2 = wds_encryp_key;
	wds_encryp_key3 = wds_encryp_key;
	
	wds_1 = websGetVar(wp, T("wds_1"), T(""));
	wds_2 = websGetVar(wp, T("wds_2"), T(""));
	wds_3 = websGetVar(wp, T("wds_3"), T(""));
	wds_4 = websGetVar(wp, T("wds_4"), T(""));
	
// Tommy , Add syslog, 2009/10/22 09:49	
	//openlog("WDS", LOG_PID, LOG_DAEMON);
	
	wds_list = websGetVar(wp, T("wds_list"), T(""));

// Tommy , Add syslog, 2009/10/22 09:49
#if 0
	char wds_mac[128];
	char *s;
	int i;
	
	strcpy(wds_mac,wds_list);
	s = wds_mac;
	i = 0;
	while ((*s != '\0') && (i < 127)){
		if (*s == ';'){
			*s = ' ';
		}	
		s++;
		i++;
	}
	
	if (!strcmp(wds_mode,"0")){
		syslog(LOG_NOTICE, "WDS mode is disable");
	}else if (!strcmp(wds_mode,"3")){
		syslog(LOG_NOTICE, "WDS mode is AP+Bridge, set Remote MAC Address %s",wds_mac);
	}else if (!strcmp(wds_mode,"2")){
		syslog(LOG_NOTICE, "WDS mode is Bridge, set Remote MAC Address %s",wds_mac);
	}
#endif

	//nvram_bufset(RT2860_NVRAM, "WdsEnable", wds_mode);
	if (!strcmp(wds_mode, "0")){
		ezplib_replace_attr("wl_wds_rule", 0, "mode", "disabled");
	}else if (!strcmp(wds_mode, "1")){
		ezplib_replace_attr("wl_wds_rule", 0, "mode", "repeater");
	}else if (!strcmp(wds_mode, "2")){
		ezplib_replace_attr("wl_wds_rule", 0, "mode", "bridge");
	}

	if (strcmp(wds_mode, "0")){
		ezplib_replace_attr("wl0_wds_basic_rule", 0, "hwaddr", wds_1);
		ezplib_replace_attr("wl0_wds_basic_rule", 1, "hwaddr", wds_2);
		ezplib_replace_attr("wl0_wds_basic_rule", 2, "hwaddr", wds_3);
		ezplib_replace_attr("wl0_wds_basic_rule", 3, "hwaddr", wds_4);
	}

//AXIM/Holmas: don't set WDS phy mode when Bridge+AP mode
if (strcmp(wds_mode,"1")){
	if (!strcmp(wds_phy_mode, "CCK;CCK;CCK;CCK")){
		ezplib_replace_attr("wl_advanced_rule", 0, "wdstxmode", "CCK");
	}else if (!strcmp(wds_phy_mode, "OFDM;OFDM;OFDM;OFDM")){
		ezplib_replace_attr("wl_advanced_rule", 0, "wdstxmode", "OFDM");
	}else if (!strcmp(wds_phy_mode, "HTMIX;HTMIX;HTMIX;HTMIX")){
		ezplib_replace_attr("wl_advanced_rule", 0, "wdstxmode", "HTMIX");
	}else if (!strcmp(wds_phy_mode, "GREENFIELD;GREENFIELD;GREENFIELD;GREENFIELD")){
		ezplib_replace_attr("wl_advanced_rule", 0, "wdstxmode", "GREENFIELD");	
	}
}
	
	if (!strcmp(wds_encryp_type, "NONE;NONE;NONE;NONE")){
		ezplib_replace_attr("wl0_wds_basic_rule", 0, "secmode", "disabled");
		ezplib_replace_attr("wl0_wds_basic_rule", 1, "secmode", "disabled");
		ezplib_replace_attr("wl0_wds_basic_rule", 2, "secmode", "disabled");
		ezplib_replace_attr("wl0_wds_basic_rule", 3, "secmode", "disabled");
	
	}else if (!strcmp(wds_encryp_type, "WEP;WEP;WEP;WEP")){
		ezplib_replace_attr("wl0_wds_basic_rule", 0, "secmode", "wep");
		ezplib_replace_attr("wl0_wds_basic_rule", 1, "secmode", "wep");
		ezplib_replace_attr("wl0_wds_basic_rule", 2, "secmode", "wep");
		ezplib_replace_attr("wl0_wds_basic_rule", 3, "secmode", "wep");
		
		ezplib_replace_attr("wl0_wds_sec_wep_rule", 0, "key", wds_encryp_key0);
		ezplib_replace_attr("wl0_wds_sec_wep_rule", 1, "key", wds_encryp_key1);
		ezplib_replace_attr("wl0_wds_sec_wep_rule", 2, "key", wds_encryp_key2);
		ezplib_replace_attr("wl0_wds_sec_wep_rule", 3, "key", wds_encryp_key3);
		
		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 0, "key", wds_encryp_key0);
		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 1, "key", wds_encryp_key1);
		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 2, "key", wds_encryp_key2);
		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 3, "key", wds_encryp_key3);
		
		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 0, "key", wds_encryp_key0);
		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 1, "key", wds_encryp_key1);
		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 2, "key", wds_encryp_key2);
		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 3, "key", wds_encryp_key3);
		
	}else if (!strcmp(wds_encryp_type, "TKIP;TKIP;TKIP;TKIP")){
		ezplib_replace_attr("wl0_wds_basic_rule", 0, "secmode", "psk");
		ezplib_replace_attr("wl0_wds_basic_rule", 1, "secmode", "psk");
		ezplib_replace_attr("wl0_wds_basic_rule", 2, "secmode", "psk");
		ezplib_replace_attr("wl0_wds_basic_rule", 3, "secmode", "psk");
		
		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 0, "crypto", "tkip");
		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 1, "crypto", "tkip");
		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 2, "crypto", "tkip");
		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 3, "crypto", "tkip");
		
		ezplib_replace_attr("wl0_wds_sec_wep_rule", 0, "key", wds_encryp_key0);
		ezplib_replace_attr("wl0_wds_sec_wep_rule", 1, "key", wds_encryp_key1);
		ezplib_replace_attr("wl0_wds_sec_wep_rule", 2, "key", wds_encryp_key2);
		ezplib_replace_attr("wl0_wds_sec_wep_rule", 3, "key", wds_encryp_key3);
		
		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 0, "key", wds_encryp_key0);
		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 1, "key", wds_encryp_key1);
		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 2, "key", wds_encryp_key2);
		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 3, "key", wds_encryp_key3);
		
		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 0, "key", wds_encryp_key0);
		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 1, "key", wds_encryp_key1);
		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 2, "key", wds_encryp_key2);
		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 3, "key", wds_encryp_key3);
		
	}else if (!strcmp(wds_encryp_type, "AES;AES;AES;AES")){
		ezplib_replace_attr("wl0_wds_basic_rule", 0, "secmode", "psk");
		ezplib_replace_attr("wl0_wds_basic_rule", 1, "secmode", "psk");
		ezplib_replace_attr("wl0_wds_basic_rule", 2, "secmode", "psk");
		ezplib_replace_attr("wl0_wds_basic_rule", 3, "secmode", "psk");

		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 0, "crypto", "aes");
		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 1, "crypto", "aes");
		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 2, "crypto", "aes");
		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 3, "crypto", "aes");
		
		ezplib_replace_attr("wl0_wds_sec_wep_rule", 0, "key", wds_encryp_key0);
		ezplib_replace_attr("wl0_wds_sec_wep_rule", 1, "key", wds_encryp_key1);
		ezplib_replace_attr("wl0_wds_sec_wep_rule", 2, "key", wds_encryp_key2);
		ezplib_replace_attr("wl0_wds_sec_wep_rule", 3, "key", wds_encryp_key3);
		
		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 0, "key", wds_encryp_key0);
		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 1, "key", wds_encryp_key1);
		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 2, "key", wds_encryp_key2);
		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 3, "key", wds_encryp_key3);
		
		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 0, "key", wds_encryp_key0);
		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 1, "key", wds_encryp_key1);
		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 2, "key", wds_encryp_key2);
		ezplib_replace_attr("wl0_wds_sec_wpa_rule", 3, "key", wds_encryp_key3);
	}

	system("/sbin/ezp-wl0-ctrl wds");
printf("/sbin/ezp-wl0-ctrl wds");
	nvram_commit();
	
#if 0	
	if (!strncmp(wds_mode, "1", 2) || !strncmp(wds_mode, "2", 2)) {
			//if (0 != strlen(wds_list))
				//nvram_bufset(RT2860_NVRAM, "WdsList", wds_list);
	}
	else{ // if (!strncmp(wds_mode, "0", 2)) {
		//nvram_bufset(RT2860_NVRAM, "WdsEncrypType", wds_encryp_type);	
	}
#endif 

#if 1//Arthur Chow 2009-03-06		
		setWebMessage(0, NULL);
#endif		     	
	websRedirect(wp, "local/advance/wds.asp");
}



/* goform/wifi5GWDS */
static void wifi5GWDS(webs_t wp, char_t *path, char_t *query)
{
	char_t	*wds_mode, *wds_phy_mode, *wds_encryp_type, *wds_encryp_key, *wds_encryp_key0,
			*wds_encryp_key1,*wds_encryp_key2, *wds_encryp_key3, *wds_list;
	char_t *wds_1,*wds_2,*wds_3,*wds_4;

	wds_mode = websGetVar(wp, T("wds_mode"), T("0")); 
	wds_phy_mode = websGetVar(wp, T("wds_phy_mode"), T(""));
	wds_encryp_type = websGetVar(wp, T("wds_encryp_type"), T(""));
	wds_encryp_key = websGetVar(wp, T("wds_encryp_key"), T(""));
	wds_encryp_key0 = wds_encryp_key;
	wds_encryp_key1 = wds_encryp_key;
	wds_encryp_key2 = wds_encryp_key;
	wds_encryp_key3 = wds_encryp_key;
	
	wds_1 = websGetVar(wp, T("wds_1"), T(""));
	wds_2 = websGetVar(wp, T("wds_2"), T(""));
	wds_3 = websGetVar(wp, T("wds_3"), T(""));
	wds_4 = websGetVar(wp, T("wds_4"), T(""));
	
// Tommy , Add syslog, 2009/10/22 09:49	
	
	wds_list = websGetVar(wp, T("wds_list"), T(""));

	if (!strcmp(wds_mode, "0")){
		ezplib_replace_attr("wl1_wds_rule", 0, "mode", "disabled");
	}else if (!strcmp(wds_mode, "1")){
		ezplib_replace_attr("wl1_wds_rule", 0, "mode", "repeater");
	}else if (!strcmp(wds_mode, "2")){
		ezplib_replace_attr("wl1_wds_rule", 0, "mode", "bridge");
	}

	if (strcmp(wds_mode, "0")){
		ezplib_replace_attr("wl1_wds_basic_rule", 0, "hwaddr", wds_1);
		ezplib_replace_attr("wl1_wds_basic_rule", 1, "hwaddr", wds_2);
		ezplib_replace_attr("wl1_wds_basic_rule", 2, "hwaddr", wds_3);
		ezplib_replace_attr("wl1_wds_basic_rule", 3, "hwaddr", wds_4);
	}

//AXIM/Holmas: don't set WDS phy mode when Bridge+AP mode
if (strcmp(wds_mode,"1")){
	if (!strcmp(wds_phy_mode, "CCK;CCK")){
		ezplib_replace_attr("wl1_advanced_rule", 0, "wdstxmode", "CCK");
	}else if (!strcmp(wds_phy_mode, "OFDM;OFDM")){
		ezplib_replace_attr("wl1_advanced_rule", 0, "wdstxmode", "OFDM");
	}else if (!strcmp(wds_phy_mode, "HTMIX;HTMIX")){
		ezplib_replace_attr("wl1_advanced_rule", 0, "wdstxmode", "HTMIX");
	}else if (!strcmp(wds_phy_mode, "GREENFIELD;GREENFIELD")){
		ezplib_replace_attr("wl1_advanced_rule", 0, "wdstxmode", "GREENFIELD");	
	}
}
	
	if (!strcmp(wds_encryp_type, "NONE;NONE;NONE;NONE")){
		ezplib_replace_attr("wl1_wds_basic_rule", 0, "secmode", "disabled");
		ezplib_replace_attr("wl1_wds_basic_rule", 1, "secmode", "disabled");
		ezplib_replace_attr("wl1_wds_basic_rule", 2, "secmode", "disabled");
		ezplib_replace_attr("wl1_wds_basic_rule", 3, "secmode", "disabled");
	
	}else if (!strcmp(wds_encryp_type, "WEP;WEP;WEP;WEP")){
		ezplib_replace_attr("wl1_wds_basic_rule", 0, "secmode", "wep");
		ezplib_replace_attr("wl1_wds_basic_rule", 1, "secmode", "wep");
		ezplib_replace_attr("wl1_wds_basic_rule", 2, "secmode", "wep");
		ezplib_replace_attr("wl1_wds_basic_rule", 3, "secmode", "wep");
		
		ezplib_replace_attr("wl1_wds_sec_wep_rule", 0, "key", wds_encryp_key0);
		ezplib_replace_attr("wl1_wds_sec_wep_rule", 1, "key", wds_encryp_key1);
		ezplib_replace_attr("wl1_wds_sec_wep_rule", 2, "key", wds_encryp_key2);
		ezplib_replace_attr("wl1_wds_sec_wep_rule", 3, "key", wds_encryp_key3);
		
		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 0, "key", wds_encryp_key0);
		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 1, "key", wds_encryp_key1);
		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 2, "key", wds_encryp_key2);
		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 3, "key", wds_encryp_key3);
		
		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 0, "key", wds_encryp_key0);
		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 1, "key", wds_encryp_key1);
		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 2, "key", wds_encryp_key2);
		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 3, "key", wds_encryp_key3);
		
	}else if (!strcmp(wds_encryp_type, "TKIP;TKIP;TKIP;TKIP")){
		ezplib_replace_attr("wl1_wds_basic_rule", 0, "secmode", "psk");
		ezplib_replace_attr("wl1_wds_basic_rule", 1, "secmode", "psk");
		ezplib_replace_attr("wl1_wds_basic_rule", 2, "secmode", "psk");
		ezplib_replace_attr("wl1_wds_basic_rule", 3, "secmode", "psk");
		
		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 0, "crypto", "tkip");
		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 1, "crypto", "tkip");
		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 2, "crypto", "tkip");
		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 3, "crypto", "tkip");
		
		ezplib_replace_attr("wl1_wds_sec_wep_rule", 0, "key", wds_encryp_key0);
		ezplib_replace_attr("wl1_wds_sec_wep_rule", 1, "key", wds_encryp_key1);
		ezplib_replace_attr("wl1_wds_sec_wep_rule", 2, "key", wds_encryp_key2);
		ezplib_replace_attr("wl1_wds_sec_wep_rule", 3, "key", wds_encryp_key3);
		
		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 0, "key", wds_encryp_key0);
		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 1, "key", wds_encryp_key1);
		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 2, "key", wds_encryp_key2);
		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 3, "key", wds_encryp_key3);
		
		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 0, "key", wds_encryp_key0);
		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 1, "key", wds_encryp_key1);
		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 2, "key", wds_encryp_key2);
		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 3, "key", wds_encryp_key3);
		
	}else if (!strcmp(wds_encryp_type, "AES;AES;AES;AES")){
		ezplib_replace_attr("wl1_wds_basic_rule", 0, "secmode", "psk");
		ezplib_replace_attr("wl1_wds_basic_rule", 1, "secmode", "psk");
		ezplib_replace_attr("wl1_wds_basic_rule", 2, "secmode", "psk");
		ezplib_replace_attr("wl1_wds_basic_rule", 3, "secmode", "psk");

		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 0, "crypto", "aes");
		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 1, "crypto", "aes");
		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 2, "crypto", "aes");
		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 3, "crypto", "aes");
		
		ezplib_replace_attr("wl1_wds_sec_wep_rule", 0, "key", wds_encryp_key0);
		ezplib_replace_attr("wl1_wds_sec_wep_rule", 1, "key", wds_encryp_key1);
		ezplib_replace_attr("wl1_wds_sec_wep_rule", 2, "key", wds_encryp_key2);
		ezplib_replace_attr("wl1_wds_sec_wep_rule", 3, "key", wds_encryp_key3);
		
		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 0, "key", wds_encryp_key0);
		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 1, "key", wds_encryp_key1);
		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 2, "key", wds_encryp_key2);
		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 3, "key", wds_encryp_key3);
		
		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 0, "key", wds_encryp_key0);
		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 1, "key", wds_encryp_key1);
		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 2, "key", wds_encryp_key2);
		ezplib_replace_attr("wl1_wds_sec_wpa_rule", 3, "key", wds_encryp_key3);
	}
      //TODO ezp-wl1-ctrl
	system("/sbin/ezp-wl1-ctrl wds");
       printf("/sbin/ezp-wl1-ctrl wds");
	nvram_commit();
	
#if 1//Arthur Chow 2009-03-06		
		setWebMessage(0, NULL);
#endif		     	
	websRedirect(wp, "local/advance/wds_5g.asp");
}

// Tommy, 2008/12/11 02:04下午
/* goform/wifiAdvanced */
static void wifiAdvanced(webs_t wp, char_t *path, char_t *query)
{
	char_t *dtim;
	char_t *tx_power;
	char_t *bcn_int;

	dtim = websGetVar(wp,T("dtim"),T("1"));
	tx_power = websGetVar(wp, T("tx_power"), T("0"));
	bcn_int = websGetVar(wp, T("bcn_int"), T("100"));

	char ModeTmpBuf[32];
    ezplib_get_attr_val("wl_mode_rule", 0, "mode", ModeTmpBuf, 32, EZPLIB_USE_CLI);
	if (!strcmp(ModeTmpBuf, "ap")){
		ezplib_replace_attr("wl_ap_advanced_rule", 0, "dtim", dtim);
		ezplib_replace_attr("wl_ap_basic_rule", 0, "txpower", tx_power);
		ezplib_replace_attr("wl_ap_advanced_rule", 0, "bcn", bcn_int);
	}
	else {
		ezplib_replace_attr("wl_advanced_rule", 0, "dtim", dtim);
		ezplib_replace_attr("wl_basic_rule", 0, "txpower", tx_power);
		ezplib_replace_attr("wl_advanced_rule", 0, "bcn", bcn_int);
	}

	nvram_commit();	
	//TODO by Frank Zhou

#if 1//Arthur Chow 2009-03-06		
		setWebMessage(0, NULL);
#endif			
	websRedirect(wp, "local/advance/wlan_advance.asp");	
}	


/* goform/wifi5GAdvanced */
static void wifi5GAdvanced(webs_t wp, char_t *path, char_t *query)
{
#if 1	
	char_t	*n_mode, *n_bandwidth, *n_gi, *n_mcs, *n_rdg, *n_extcha, *n_amsdu;
	char_t	*n_autoba, *n_badecline;
	char_t	*tx_stream, *rx_stream;
	char_t     *dtim1,*antenna_mode;

	int is_n = 0;


	char_t *rts = websGetVar(wp, T("rts"), T("2346"));

	char_t *fragment = websGetVar(wp, T("fragment"), T("2346"));
		
	char_t *wIntraBSS = websGetVar(wp, T("IntraBSS"), T(""));
	if (0 == strlen(wIntraBSS)) {
		wIntraBSS = "1";
	}else{
		wIntraBSS = "0";
	}

	char_t *tx_power = websGetVar(wp, T("tx_power"), T("100"));

	char_t *wirelessmode = websGetVar(wp, T("wirelessmode"), T("9")); //9: bgn mode
	
	char *wifimode = atoi(wirelessmode);

	n_mode = websGetVar(wp, T("n_mode"), T("0"));
	n_bandwidth = websGetVar(wp, T("n_bandwidth"), T("0"));
	n_gi = websGetVar(wp, T("n_gi"), T("0"));
	n_extcha = websGetVar(wp, T("n_extcha"), T("0"));
	dtim1 = websGetVar(wp,T("dtim"),T("1"));
	antenna_mode = websGetVar(wp,T("antenna_mode"),T("1"));

	ezplib_replace_attr("wl1_advanced_rule", 0, "opmode", n_mode);

	ezplib_replace_attr("wl1_advanced_rule", 0, "htbw", n_bandwidth);
	
	ezplib_replace_attr("wl1_advanced_rule", 0, "gi", n_gi);

	ezplib_replace_attr("wl1_advanced_rule", 0, "extcha", n_extcha);

	ezplib_replace_attr("wl1_advanced_rule", 0, "rts", rts);	

	ezplib_replace_attr("wl1_advanced_rule", 0, "frag", fragment);
	
       ezplib_replace_attr("wl1_advanced_rule", 0, "dtim", dtim1);	
	   
       ezplib_replace_attr("wl1_advanced_rule", 0, "antennamode", antenna_mode);	

	ezplib_replace_attr("wl5g_basic_rule", 0, "bisolation", wIntraBSS);	

	ezplib_replace_attr("wl5g_basic_rule", 0, "txpower", tx_power);
	
	ezplib_replace_attr("wl5g_basic_rule", 0, "net_mode", wirelessmode);
	
// Holmas say that need to add the following statement for WiFi
	system("/sbin/ezp-wl1-ctrl advance");	
	
	system("/sbin/ezp-wl1-ctrl basic");
	nvram_commit();	

#if 1//Arthur Chow 2009-03-06		
		setWebMessage(0, NULL);
#endif			
	websRedirect(wp, "local/advance/wlan_advance_5g.asp");	
#endif	
}	


/* goform/wirelessAdvanced */
// Tommy, decrease code size , 2009/3/17 01:28
#if Include_Ralink_Original
static void wirelessAdvanced(webs_t wp, char_t *path, char_t *query)
{
	char_t	*bg_protection, *basic_rate, *beacon, *dtim, *fragment, *rts,
			*tx_power, *short_preamble, *short_slot, *tx_burst, *pkt_aggregate,
			*ieee_80211h, *wmm_capable, *apsd_capable, *dls_capable, *countrycode;
	char_t	*m2u_enable;
	int		i, ssid_num, wlan_mode;
	char	wmm_enable[8];

	//fetch from web input
	bg_protection = websGetVar(wp, T("bg_protection"), T("0"));
	basic_rate = websGetVar(wp, T("basic_rate"), T("15"));
	beacon = websGetVar(wp, T("beacon"), T("100"));
	dtim = websGetVar(wp, T("dtim"), T("1"));
	fragment = websGetVar(wp, T("fragment"), T("2346"));
	rts = websGetVar(wp, T("rts"), T("2346"));
	tx_power = websGetVar(wp, T("tx_power"), T("100"));
	short_preamble = websGetVar(wp, T("short_preamble"), T("0"));
	short_slot = websGetVar(wp, T("short_slot"), T("0"));
	tx_burst = websGetVar(wp, T("tx_burst"), T("0"));
	pkt_aggregate = websGetVar(wp, T("pkt_aggregate"), T("0"));
	ieee_80211h = websGetVar(wp, T("ieee_80211h"), T("0"));
	wmm_capable = websGetVar(wp, T("wmm_capable"), T("0"));
	apsd_capable = websGetVar(wp, T("apsd_capable"), T("0"));
	dls_capable = websGetVar(wp, T("dls_capable"), T("0"));
	m2u_enable = websGetVar(wp, T("m2u_enable"), T("0"));
	countrycode = websGetVar(wp, T("country_code"), T("NONE"));

	if (NULL != nvram_bufget(RT2860_NVRAM, "BssidNum"))
		ssid_num = atoi(nvram_bufget(RT2860_NVRAM, "BssidNum"));
	else
		ssid_num = 1;
	wlan_mode = atoi(nvram_bufget(RT2860_NVRAM, "WirelessMode"));

	//set to nvram
	nvram_bufset(RT2860_NVRAM, "BGProtection", bg_protection);
	nvram_bufset(RT2860_NVRAM, "BasicRate", basic_rate);
	nvram_bufset(RT2860_NVRAM, "BeaconPeriod", beacon);
	nvram_bufset(RT2860_NVRAM, "DtimPeriod", dtim);
	nvram_bufset(RT2860_NVRAM, "FragThreshold", fragment);
	nvram_bufset(RT2860_NVRAM, "RTSThreshold", rts);
	nvram_bufset(RT2860_NVRAM, "TxPower", tx_power);
	nvram_bufset(RT2860_NVRAM, "TxPreamble", short_preamble);
	nvram_bufset(RT2860_NVRAM, "ShortSlot", short_slot);
	nvram_bufset(RT2860_NVRAM, "TxBurst", tx_burst);
	nvram_bufset(RT2860_NVRAM, "PktAggregate", pkt_aggregate);
	nvram_bufset(RT2860_NVRAM, "IEEE80211H", ieee_80211h);
	nvram_bufset(RT2860_NVRAM, "WmmCapable", wmm_capable);
	nvram_bufset(RT2860_NVRAM, "APSDCapable", apsd_capable);
	nvram_bufset(RT2860_NVRAM, "DLSCapable", dls_capable);
	nvram_bufset(RT2860_NVRAM, "M2UEnabled", m2u_enable);

	bzero(wmm_enable, sizeof(char)*8);
	for (i = 0; i < ssid_num; i++)
	{
		sprintf(wmm_enable+strlen(wmm_enable), "%d", atoi(wmm_capable));
		sprintf(wmm_enable+strlen(wmm_enable), "%c", ';');
	}
	wmm_enable[strlen(wmm_enable) - 1] = '\0';

	if (!strncmp(wmm_capable, "1", 2)) {
		if (wlan_mode < 5)
			nvram_bufset(RT2860_NVRAM, "TxBurst", "0");
	}

	nvram_bufset(RT2860_NVRAM, "CountryCode", countrycode);
	if (!strncmp(countrycode, "US", 3)) {
		nvram_bufset(RT2860_NVRAM, "CountryRegion", "0");
		nvram_bufset(RT2860_NVRAM, "CountryRegionABand", "0");
	}
	else if (!strncmp(countrycode, "JP", 3)) {
		nvram_bufset(RT2860_NVRAM, "CountryRegion", "5");
		nvram_bufset(RT2860_NVRAM, "CountryRegionABand", "6");
	}
	else if (!strncmp(countrycode, "FR", 3)) {
		nvram_bufset(RT2860_NVRAM, "CountryRegion", "1");
		nvram_bufset(RT2860_NVRAM, "CountryRegionABand", "2");
	}
	else if (!strncmp(countrycode, "TW", 3)) {
		nvram_bufset(RT2860_NVRAM, "CountryRegion", "1");
		nvram_bufset(RT2860_NVRAM, "CountryRegionABand", "3");
	}
	else if (!strncmp(countrycode, "IE", 3)) {
		nvram_bufset(RT2860_NVRAM, "CountryRegion", "1");
		nvram_bufset(RT2860_NVRAM, "CountryRegionABand", "2");
	}
	else if (!strncmp(countrycode, "HK", 3)) {
		nvram_bufset(RT2860_NVRAM, "CountryRegion", "1");
		nvram_bufset(RT2860_NVRAM, "CountryRegionABand", "0");
	}
	else {
		nvram_bufset(RT2860_NVRAM, "CountryCode", "");
	}

	nvram_commit(RT2860_NVRAM);
	initInternet();

#ifdef CONFIG_RT2860V2_AP_IGMP_SNOOP
	if (!strncmp(m2u_enable, "1", 2))
		//doSystem("iwpriv rai0 set IgmpSnEnable=1");
		system("/sbin/ezp-wps-set 0 0 12 1");
	else
		//doSystem("iwpriv rai0 set IgmpSnEnable=0");
		system("/sbin/ezp-wps-set 0 0 12 0");
#endif

	//debug print
	websHeader(wp);
	websWrite(wp, T("bg_protection: %s<br>\n"), bg_protection);
    websWrite(wp, T("basic_rate: %s<br>\n"), basic_rate);
    websWrite(wp, T("beacon: %s<br>\n"), beacon);
    websWrite(wp, T("dtim: %s<br>\n"), dtim);
    websWrite(wp, T("fragment: %s<br>\n"), fragment);
    websWrite(wp, T("rts: %s<br>\n"), rts);
    websWrite(wp, T("tx_power: %s<br>\n"), tx_power);
    websWrite(wp, T("short_preamble: %s<br>\n"), short_preamble);
    websWrite(wp, T("short_slot: %s<br>\n"), short_slot);
    websWrite(wp, T("tx_burst: %s<br>\n"), tx_burst);
    websWrite(wp, T("pkt_aggregate: %s<br>\n"), pkt_aggregate);
    websWrite(wp, T("ieee_80211h: %s<br>\n"), ieee_80211h);
    websWrite(wp, T("wmm_capable: %s<br>\n"), wmm_capable);
    websWrite(wp, T("apsd_capable: %s<br>\n"), apsd_capable);
    websWrite(wp, T("dls_capable: %s<br>\n"), dls_capable);
    websWrite(wp, T("countrycode: %s<br>\n"), countrycode);
#ifdef CONFIG_RT2860V2_AP_IGMP_SNOOP
    websWrite(wp, T("m2u_enable: %s<br>\n"), m2u_enable);
#endif
	websFooter(wp);
	websDone(wp, 200);
}
#endif // #if Include_Ralink_Original

// Tommy, 2008/12/16 02:29下午
/* goform/wifiGeneral */
static void wifiGeneral(webs_t wp, char_t *path, char_t *query)
{
#if 0	
	char_t	*ssid, *mssid_1, *mssid_2, *mssid_3, *mssid_4, *mssid_5, *mssid_6,
		*mssid_7, *bssid_num, *broadcastssid;
	char_t	*sz11gChannel;
	int i = 0, is_n = 0, new_bssid_num, old_bssid_num = 1;
	char_t	*auto_channel;
	char_t	*hidessid, *hidemssid_1, *hidemssid_2, *hidemssid_3, *hidemssid_4, *hidemssid_5, *hidemssid_6,*hidemssid_7;
	char_t	*enablessid, *enablemssid_1, *enablemssid_2, *enablemssid_3, *enablemssid_4, *enablemssid_5, *enablemssid_6,*enablemssid_7;
	char 	hidessid_str[17];

 char *operation_mode = nvram_bufget(RT2860_NVRAM, "OP_Mode");
 
 if (!strcmp(operation_mode, "3")){ // if (!strcmp(operation_mode, "3")){
 	
	sz11gChannel = websGetVar(wp, T("sz11gChannel"), T(""));	
	
	auto_channel = websGetVar(wp, T("Auto_Channel"), T(""));

        if (!strncmp(auto_channel, "1", 2))
		sz11gChannel = "0";
	
        if (!strncmp(sz11gChannel, "0", 2))
		nvram_bufset(RT2860_NVRAM, "AutoChannelSelect", "1");
	else
		nvram_bufset(RT2860_NVRAM, "AutoChannelSelect", "0");
			
	nvram_bufset(RT2860_NVRAM, "Channel", sz11gChannel);

 } else { // if (!strcmp(operation_mode, "3")){
	//fetch from web input
	//wirelessmode = websGetVar(wp, T("wirelessmode"), T("9")); //9: bgn mode
	ssid = websGetVar(wp, T("ssid"), T("")); 
	mssid_1 = websGetVar(wp, T("mssid_1"), T("")); 
	mssid_2 = websGetVar(wp, T("mssid_2"), T("")); 
	mssid_3 = websGetVar(wp, T("mssid_3"), T("")); 
	mssid_4 = websGetVar(wp, T("mssid_4"), T("")); 
	mssid_5 = websGetVar(wp, T("mssid_5"), T("")); 
	mssid_6 = websGetVar(wp, T("mssid_6"), T("")); 
	mssid_7 = websGetVar(wp, T("mssid_7"), T("")); 
	bssid_num = websGetVar(wp, T("bssid_num"), T("1"));
	
	enablessid = websGetVar(wp, T("enablessid"), T("")); 
	enablemssid_1 = websGetVar(wp, T("enablemssid_1"), T("")); 
	enablemssid_2 = websGetVar(wp, T("enablemssid_2"), T("")); 
	enablemssid_3 = websGetVar(wp, T("enablemssid_3"), T("")); 
	enablemssid_4 = websGetVar(wp, T("enablemssid_4"), T("")); 
	enablemssid_5 = websGetVar(wp, T("enablemssid_5"), T("")); 
	enablemssid_6 = websGetVar(wp, T("enablemssid_6"), T("")); 
	enablemssid_7 = websGetVar(wp, T("enablemssid_7"), T("")); 

// Tommy, SSID enable check box, 2009/1/13 03:04	
	nvram_bufset(RT2860_NVRAM, "SSID_Enable",   atoi(enablessid)   ? "1" : "0" );
	nvram_bufset(RT2860_NVRAM, "mSSID1_Enable",   atoi(enablemssid_1)   ? "1" : "0" );
	nvram_bufset(RT2860_NVRAM, "mSSID2_Enable",   atoi(enablemssid_2)   ? "1" : "0" );
	nvram_bufset(RT2860_NVRAM, "mSSID3_Enable",   atoi(enablemssid_3)   ? "1" : "0" );
	nvram_bufset(RT2860_NVRAM, "mSSID4_Enable",   atoi(enablemssid_4)   ? "1" : "0" );
	nvram_bufset(RT2860_NVRAM, "mSSID5_Enable",   atoi(enablemssid_5)   ? "1" : "0" );
	nvram_bufset(RT2860_NVRAM, "mSSID6_Enable",   atoi(enablemssid_6)   ? "1" : "0" );
	nvram_bufset(RT2860_NVRAM, "mSSID7_Enable",   atoi(enablemssid_7)   ? "1" : "0" );

// Tommy, 2009/1/6 02:45 per-SSID hide
#if 0	
	broadcastssid = websGetVar(wp, T("broadcastssid"), T(""));
	
        if (!strncmp(broadcastssid, "1", 2)){
        	broadcastssid="1";
        }else{	
		broadcastssid="0";        
	}	 
#else
	hidessid = websGetVar(wp, T("hidessid"), T("")); 
	hidemssid_1 = websGetVar(wp, T("hidemssid_1"), T("")); 
	hidemssid_2 = websGetVar(wp, T("hidemssid_2"), T("")); 
	hidemssid_3 = websGetVar(wp, T("hidemssid_3"), T("")); 
	hidemssid_4 = websGetVar(wp, T("hidemssid_4"), T("")); 
	hidemssid_5 = websGetVar(wp, T("hidemssid_5"), T("")); 
	hidemssid_6 = websGetVar(wp, T("hidemssid_6"), T("")); 
	hidemssid_7 = websGetVar(wp, T("hidemssid_7"), T("")); 
#endif

	sz11gChannel = websGetVar(wp, T("sz11gChannel"), T(""));	
	
	auto_channel = websGetVar(wp, T("Auto_Channel"), T(""));

        if (!strncmp(auto_channel, "1", 2))
		sz11gChannel = "0";
	
        if (!strncmp(sz11gChannel, "0", 2))
		nvram_bufset(RT2860_NVRAM, "AutoChannelSelect", "1");
	else
		nvram_bufset(RT2860_NVRAM, "AutoChannelSelect", "0");
			
	nvram_bufset(RT2860_NVRAM, "Channel", sz11gChannel);
	
	/* Reset Ext Channle */
	//if ((sz11gChannel >= 1 && sz11gChannel<=4) || (sz11gChannel >= 11 && sz11gChannel<=14)){
		nvram_bufset(RT2860_NVRAM, "HT_EXTCHA", "0");		
	//}
	
#if 0	
	apisolated = websGetVar(wp, T("apisolated"), T("0"));
	mbssidapisolated = websGetVar(wp, T("mbssidapisolated"), T("0"));
	sz11aChannel = websGetVar(wp, T("sz11aChannel"), T("")); 
	sz11bChannel = websGetVar(wp, T("sz11bChannel"), T("")); 
	sz11gChannel = websGetVar(wp, T("sz11gChannel"), T("")); 
	abg_rate = websGetVar(wp, T("abg_rate"), T("")); 
	wds_mode = websGetVar(wp, T("wds_mode"), T("0")); 
	wds_phy_mode = websGetVar(wp, T("wds_phy_mode"), T("0")); 
	wds_encryp_type = websGetVar(wp, T("wds_encryp_type"), T("0")); 
	wds_encryp_key = websGetVar(wp, T("wds_encryp_key"), T("0")); 
	wds_list = websGetVar(wp, T("wds_list"), T("")); 
	if (strlen(wds_list) > 0)
		wds_list[strlen(wds_list) - 1] = '\0';
	n_mode = websGetVar(wp, T("n_mode"), T("0"));
	n_bandwidth = websGetVar(wp, T("n_bandwidth"), T("0"));
	n_gi = websGetVar(wp, T("n_gi"), T("0"));
	n_mcs = websGetVar(wp, T("n_mcs"), T("33"));
	n_rdg = websGetVar(wp, T("n_rdg"), T("0"));
	n_extcha = websGetVar(wp, T("n_extcha"), T("0"));
	n_amsdu = websGetVar(wp, T("n_amsdu"), T("0"));
	n_autoba = websGetVar(wp, T("n_autoba"), T("0"));
	n_badecline = websGetVar(wp, T("n_badecline"), T("0"));
	tx_stream = websGetVar(wp, T("tx_stream"), T("0"));
	rx_stream = websGetVar(wp, T("rx_stream"), T("0"));
#endif
	old_bssid_num = atoi(nvram_bufget(RT2860_NVRAM, "BssidNum"));
	new_bssid_num = atoi(bssid_num);

#if 0
	nvram_bufset(RT2860_NVRAM, "WirelessMode", wirelessmode);
	if (!strncmp(wirelessmode, "4", 2))
		nvram_bufset(RT2860_NVRAM, "BasicRate", "351");
		
	else if (!strncmp(wirelessmode, "8", 2) || !strncmp(wirelessmode, "9", 2))
		is_n = 1;
#endif

	//SSID, Multiple SSID
	if (0 == strlen(ssid)) {
		nvram_commit(RT2860_NVRAM);
		websError(wp, 403, T("'SSID' should not be empty!"));
		return;
	}
	nvram_bufset(RT2860_NVRAM, "SSID1", ssid);

//#WPS
	{
		char *wordlist= nvram_bufget(RT2860_NVRAM, "WscModeOption");
		if(wordlist){
			if (strcmp(wordlist, "0"))
				doSystem("iwpriv ra0 set WscConfStatus=1");
			nvram_bufset(RT2860_NVRAM, "WscConfigured", "1");
			g_wsc_configured = 1;
		}
	}

//#WPS
	default_shown_mbssid[RT2860_NVRAM] = 0;

	i = 2;
	if (0 != strlen(mssid_1)) {
		nvram_bufset(RT2860_NVRAM, racat("SSID", i), mssid_1);
		i++;
	}
	if (0 != strlen(mssid_2)) {
		nvram_bufset(RT2860_NVRAM, racat("SSID", i), mssid_2);
		i++;
	}
	if (0 != strlen(mssid_3)) {
		nvram_bufset(RT2860_NVRAM, racat("SSID", i), mssid_3);
		i++;
	}
	if (0 != strlen(mssid_4)) {
		nvram_bufset(RT2860_NVRAM, racat("SSID", i), mssid_4);
		i++;
	}
	if (0 != strlen(mssid_5)) {
		nvram_bufset(RT2860_NVRAM, racat("SSID", i), mssid_5);
		i++;
	}
	if (0 != strlen(mssid_6)) {
		nvram_bufset(RT2860_NVRAM, racat("SSID", i), mssid_6);
		i++;
	}
	if (0 != strlen(mssid_7)) {
		nvram_bufset(RT2860_NVRAM, racat("SSID", i), mssid_7);
		i++;
	}

	nvram_bufset(RT2860_NVRAM, "BssidNum", bssid_num);
	if (new_bssid_num < 1 || new_bssid_num > 8) {
		nvram_commit(RT2860_NVRAM);
		websError(wp, 403, T("'bssid_num' %s is out of range!"), bssid_num);
		return;
	}
	
	revise_mbss_value(old_bssid_num, new_bssid_num);

//Tommy, Add per SSID hidden , 2009/1/6 01:56 
#if 0 // per-SSID hidden
	//Broadcast SSID
	if (new_bssid_num == 1) {
		if (!strncmp(broadcastssid, "1", 2))
			nvram_bufset(RT2860_NVRAM, "HideSSID", "0");
		else
			nvram_bufset(RT2860_NVRAM, "HideSSID", "1");
	}
	else if (new_bssid_num == 2) {
		if (!strncmp(broadcastssid, "1", 2))
			nvram_bufset(RT2860_NVRAM, "HideSSID", "0;0");
		else
			nvram_bufset(RT2860_NVRAM, "HideSSID", "1;1");
	}
	else if (new_bssid_num == 3) {
		if (!strncmp(broadcastssid, "1", 2))
			nvram_bufset(RT2860_NVRAM, "HideSSID", "0;0;0");
		else
			nvram_bufset(RT2860_NVRAM, "HideSSID", "1;1;1");
	}
	else if (new_bssid_num == 4) {
		if (!strncmp(broadcastssid, "1", 2))
			nvram_bufset(RT2860_NVRAM, "HideSSID", "0;0;0;0");
		else
			nvram_bufset(RT2860_NVRAM, "HideSSID", "1;1;1;1");
	}
	else if (new_bssid_num == 5) {
		if (!strncmp(broadcastssid, "1", 2))
			nvram_bufset(RT2860_NVRAM, "HideSSID", "0;0;0;0;0");
		else
			nvram_bufset(RT2860_NVRAM, "HideSSID", "1;1;1;1;1");
	}
	else if (new_bssid_num == 6) {
		if (!strncmp(broadcastssid, "1", 2))
			nvram_bufset(RT2860_NVRAM, "HideSSID", "0;0;0;0;0;0");
		else
			nvram_bufset(RT2860_NVRAM, "HideSSID", "1;1;1;1;1;1");
	}
	else if (new_bssid_num == 7) {
		if (!strncmp(broadcastssid, "1", 2))
			nvram_bufset(RT2860_NVRAM, "HideSSID", "0;0;0;0;0;0;0");
		else
			nvram_bufset(RT2860_NVRAM, "HideSSID", "1;1;1;1;1;1;1");
	}
	else if (new_bssid_num == 8) {
		if (!strncmp(broadcastssid, "1", 2))
			nvram_bufset(RT2860_NVRAM, "HideSSID", "0;0;0;0;0;0;0;0");
		else
			nvram_bufset(RT2860_NVRAM, "HideSSID", "1;1;1;1;1;1;1;1");
	}
#else // per-SSID hidden

		if (!strncmp(hidessid, "1", 2)){
			hidessid_str[0] = '1';
		}else{
			hidessid_str[0] = '0';
		}			
			hidessid_str[1] = ';';

		if (!strncmp(hidemssid_1, "1", 2)){
			hidessid_str[2] = '1';
		}else{
			hidessid_str[2] = '0';
		}
			hidessid_str[3] = ';';
			
		if (!strncmp(hidemssid_2, "1", 2)){
			hidessid_str[4] = '1';
		}else{
			hidessid_str[4] = '0';
		}
			hidessid_str[5] = ';';
			
		if (!strncmp(hidemssid_3, "1", 2)){
			hidessid_str[6] = '1';
		}else{
			hidessid_str[6] = '0';
		}
			hidessid_str[7] = ';';
			
		if (!strncmp(hidemssid_4, "1", 2)){
			hidessid_str[8] = '1';
		}else{
			hidessid_str[8] = '0';
		}
			hidessid_str[9] = ';';
			
		if (!strncmp(hidemssid_5, "1", 2)){
			hidessid_str[10] = '1';
		}else{
			hidessid_str[10] = '0';
		}
			hidessid_str[11] = ';';
			
		if (!strncmp(hidemssid_6, "1", 2)){
			hidessid_str[12] = '1';
		}else{
			hidessid_str[12] = '0';
		}
			hidessid_str[13] = ';';
			
		if (!strncmp(hidemssid_7, "1", 2)){
			hidessid_str[14] = '1';
		}else{
			hidessid_str[14] = '0';
		}
			hidessid_str[15] = ';';
			
		hidessid_str[new_bssid_num*2 - 1] = '\0';
		nvram_bufset(RT2860_NVRAM, "HideSSID", hidessid_str);		
#endif // per-SSID hidden


#if 0
	// NoForwarding and NoForwardingBTNBSSID
	nvram_bufset(RT2860_NVRAM, "NoForwarding", apisolated);
	nvram_bufset(RT2860_NVRAM, "NoForwardingBTNBSSID", mbssidapisolated);
#endif

#if 0
	//11abg Channel or AutoSelect
	if ((0 == strlen(sz11aChannel)) && (0 == strlen(sz11bChannel)) &&
			(0 == strlen(sz11gChannel))) {
		nvram_commit(RT2860_NVRAM);
		websError(wp, 403, T("'Channel' should not be empty!"));
		return;
	}
	if (!strncmp(sz11aChannel, "0", 2) && !strncmp(sz11bChannel, "0", 2) &&
			!strncmp(sz11gChannel, "0", 2))
		nvram_bufset(RT2860_NVRAM, "AutoChannelSelect", "1");
	else
		nvram_bufset(RT2860_NVRAM, "AutoChannelSelect", "0");
	if (0 != strlen(sz11aChannel))
		nvram_bufset(RT2860_NVRAM, "Channel", sz11aChannel);
	if (0 != strlen(sz11bChannel))
		nvram_bufset(RT2860_NVRAM, "Channel", sz11bChannel);
	if (0 != strlen(sz11gChannel))
		nvram_bufset(RT2860_NVRAM, "Channel", sz11gChannel);



	//WdsEnable, WdsPhyMode, WdsEncrypType, WdsKey, WdsList
	//where WdsPhyMode - 0:CCK, 1:OFDM, 2:HTMIX, 3:GREENFIELD
	//      WdsEncryptType - NONE, WEP, TKIP, AES
	nvram_bufset(RT2860_NVRAM, "WdsEnable", wds_mode);
	if (strncmp(wds_mode, "0", 2)) {
		nvram_bufset(RT2860_NVRAM, "WdsPhyMode", wds_phy_mode);
		nvram_bufset(RT2860_NVRAM, "WdsEncrypType", wds_encryp_type);
		nvram_bufset(RT2860_NVRAM, "WdsKey", wds_encryp_key);
		if (!strncmp(wds_mode, "2", 2) || !strncmp(wds_mode, "3", 2)) {
			if (0 != strlen(wds_list))
				nvram_bufset(RT2860_NVRAM, "WdsList", wds_list);
		}
	}
#endif	


#if CONFIG_RT2860V2_AP == CONFIG_MIPS
	/* this is a workaround:
	 *  when AP is built as kernel
	 *  if more ssids are created, driver won't exe RT28xx_MBSS_Init again
	 *  therefore, we reboot to make it available
	 *  (PS. CONFIG_MIPS would be "y")
	 */
	if (new_bssid_num > old_bssid_num)
		doSystem("reboot");
#endif
 } //if (!strcmp(operation_mode, "3")){

	nvram_commit(RT2860_NVRAM);

#if 0	
// Tommy, Fixed Multiple SSID or AP Client module(apcli0) cannot rmmod rt2880_ap.ko 
	for (i = 0 ; i < old_bssid_num	; i++){
	     doSystem("ifconfig ra%d down",i);
	}	     

	//char *operation_mode = nvram_bufget(RT2860_NVRAM, "OP_Mode");
	
	if (!strcmp(operation_mode, "5")){
	   doSystem("ifconfig apcli0 down");		
	}
	// down all WDS interface wsd0 - wsd3
	char *wdsMode = nvram_bufget(RT2860_NVRAM, "WdsEnable");
	
	if (strcmp(wdsMode, "0")){
	    for (i = 0 ; i < 4; i++){
	         doSystem("ifconfig wds%d down",i);
	    }     
	}			
#endif	
	initInternet();

	websRedirect(wp, "local/advance/wlan.asp");
#endif
}


void ConverterStringToDisplay(char *str)
{
    int  len, i;
    char buffer[193];
    char *pOut;

    memset(buffer,0,193);
    len = strlen(str);
    pOut = &buffer[0];

    for (i = 0; i < len; i++) {
		switch (str[i]) {
			case '&':
				strcpy (pOut, "&amp;");
				pOut += 5;
				break;

			case '<': 
				strcpy (pOut, "&lt;");
				pOut += 4;
				break;

			case '>': 
				strcpy (pOut, "&gt;");
				pOut += 4;
				break;

			case '"':
				strcpy (pOut, "&quot;");
				pOut += 6;
				break;

				//case ' ':
				//strcpy (pOut, "&nbsp;");
				//pOut += 6;
				//break;

			default:
				if ((str[i]>=0) && (str[i]<=31)) {
					//Device Control Characters
					sprintf(pOut, "&#%02d;", str[i]);
					pOut += 5;
				} else if ((str[i]==39) || (str[i]==47) || (str[i]==59) || (str[i]==92)) {
					// ' / ; (backslash)
					sprintf(pOut, "&#%02d;", str[i]);
					pOut += 5;
				} else if (str[i]>=127) {
					//Device Control Characters
					sprintf(pOut, "&#%03d;", str[i]);
					pOut += 6;
				} else {
					*pOut = str[i];
					pOut++;
				}
				break;
		}
    }
    *pOut = '\0';
    strcpy(str, buffer);
}
void delspace(char *Buf1, char* Buf2)
{
	while(*Buf1 != '\0')
	{
		if((*Buf1) == 0x20)
		{
			++Buf1; // if space skip 
		}
		else
		*Buf2++ = *Buf1++; // if not space copy to buf 2
	}
	*Buf2 = '\0'; //NULL Terminate

}

void delquote(char *Buf1, char* Buf2)
{
	int ssid_len = 32, i = 0, len = 0;
	char *p, *q, *tmp_p = NULL;

	if ( (Buf1 == NULL)  || (Buf2 == NULL))
		return;
	
	p = Buf1;
	while( (p != NULL) && (ssid_len > 0))
	{
		q = memchr(p, '"', ssid_len);	
		if ( (q != NULL) && (p != q)) {
			ssid_len -= (q -p);
			p = q + 1;
			tmp_p = q;
		} else {
			break;
		} 
	}
	
	p = Buf1;
	#if 1
	if (tmp_p != NULL) {
		len = tmp_p - Buf1;
		memcpy(Buf2, Buf1, (tmp_p - p));
		Buf2[len + 1] = '\0';
	} else if ( Buf1[0] == '"') {
		Buf2[0] = '\0';
	} else  {
	  	memcpy(Buf2, Buf1, ssid_len);
		Buf2[ssid_len] = '\0';
	}
	#endif
}


/*funciton@special character replace with 
 *parameter: source src str,sub str,replace str
 *eg: aa<bb <  &#60;
 *result: aa$#60;bb $#60; replace < char
 *reason: html not support < > & " characters
 */
char *sub_replace(char *source,char *sub,char *rep)
{
  char *result;
  char *pc1,*pc2,*pc3;
  int isource,isub,irep;
  isub = strlen(sub);
  irep = strlen(rep);
  isource = strlen(source);
  if(strlen(sub) == 0) //sub[0]=='\0'; is empty str or not!
      return strdup(source);
  result = (char *)malloc(( (irep > isub) ? (float)strlen(source) / isub* irep+ 1:isource ) * sizeof(char));
  pc1 = result;
  while(*source != '\0')
  {
     pc2 = source;
     pc3 = sub;
     while(*pc2 == *pc3 && *pc3 != '\0' && *pc2 != '\0')
           pc2++, pc3++;
     if(strlen(pc3) == 0)
     {
        pc3 = rep;
        while(*pc3 != '\0')
             *pc1++ = *pc3++;
        pc2--;
        source = pc2;
     }
     else
        *pc1++ = *source;
         source++;
     }
   *pc1 = '\0';
   return result;
}

char *get_translate_char(int flag,char * sour_ssid){
	char special_char[5]={'&','<','>','"','\0'};
	char *ssid_tmp;
	char *ssid_value = sour_ssid,sub[5],rep[8];
       int k;

	if(!flag){
              for(k = 0;k< strlen(special_char);k++){
                   switch(k){
                        case 0:
                           strcpy(rep,"&#38;");
                           strcpy(sub,"&");
       			ssid_tmp = sub_replace(ssid_value,sub,rep);
				strcpy(ssid_value,ssid_tmp);
				free(ssid_tmp);
                           break;
                         case 1:
                              strcpy(rep,"&#60;");
                              strcpy(sub,"<");
       			   ssid_tmp = sub_replace(ssid_value,sub,rep);
				   strcpy(ssid_value,ssid_tmp);
				free(ssid_tmp);
                             break;
                          case 2:
                              strcpy(rep,"&#62;");
                              strcpy(sub,">");
       			   ssid_tmp = sub_replace(ssid_value,sub,rep);
				   strcpy(ssid_value,ssid_tmp);
				   free(ssid_tmp);
                              break;
                          case 3:
                              strcpy(rep,"&#34;");
                              strcpy(sub,"\"");
       			    ssid_tmp = sub_replace(ssid_value,sub,rep);
				    strcpy(ssid_value,ssid_tmp);
				    free(ssid_tmp);
                               break;
                          default:
                              break;
                      }
    	          }
          }
	return sour_ssid;
}

static int getwifiRSSI(int eid, webs_t wp, int argc, char_t **argv) {
	FILE *fp, *tmp;
	char *pRssiList = (char*)malloc(16 * sizeof (char));
	char tmpRssi1[16], tmpRssi2[16];
	int  Rssi1, Rssi2;
	system("iwpriv apclii0 stat | grep RSSI | awk '{print $3, $4}' > /tmp/rssi.dat");
	if (NULL == (fp = fopen("/tmp/rssi.dat", "r")))
	{
		websWrite(wp, T("-- / --"));
	} else {
		memset(tmpRssi1, 0x00, sizeof(tmpRssi1));
		memset(tmpRssi2, 0x00, sizeof(tmpRssi2));
		fgets(pRssiList, 16, fp);
		snprintf(tmpRssi1, 4, "%s", pRssiList); 
		delspace(tmpRssi1, &tmpRssi1[0]);
		Rssi1 = atoi(tmpRssi1);
		snprintf(tmpRssi2, 4, "%s", pRssiList+4); 
		delspace(tmpRssi2, &tmpRssi2[0]);
		Rssi2 = atoi(tmpRssi2);
		if(Rssi1 == 0 && Rssi2 == 0) {
			websWrite(wp, T("-- / --"));
		} else if(Rssi1 == 0) {
			websWrite(wp, T("-- / %d dBm"), Rssi2);
		} else if(Rssi2 == 0) {
			websWrite(wp, T("%d dBm / --"), Rssi1);
		} else {
			websWrite(wp, T("%d dBm / %d dBm"), Rssi1, Rssi2);
		}
	}
	free(pRssiList);
	fclose(fp);
	return 0; 
}


static int getwifiBSSIDList(int eid, webs_t wp, int argc, char_t **argv)
{
	FILE *fp, *tmp;
	int num = 0, i, j=0;
	int NetworkType;
	char *pBssidList = (char*)malloc(65536 * sizeof (char));
	char result[256];
	char set_bssid[28], TMPset_bssid[28], SSID[128];
	char TMPssid[33], TMPrssi[16], TMPchannel[16], TMPauth[20], TMPbssid[28];
	unsigned char tmpRadio[188], tmpBSSID[28], tmpSSID[64], tmp_SSID[128], *p_ssid, *p_SSID, tmpRSSI[16], tmpChannel[16],  
		tmpAuth[32], tmpEncry[20], tmpNetworkType[24], tmpImg[40];
	unsigned char radiocheck[8];
	unsigned char tmpSSIDII[28];
	unsigned char BssidQuery[6];
	char *ssid_value;
	int space_length;
	
	system("/sbin/ezp-wps-set 0 00 13 1");
	system("/sbin/ezp-wps-set 0 00 14 0");

	if (NULL == (fp = fopen("/etc/site_survey.dat", "r")))
	{
		websWrite(wp, T("0"));
	}
	else
	{
		fgets(pBssidList, 65536, fp);
		fgets(pBssidList, 65536, fp);

		while(!feof(fp)){
			memset(radiocheck, 0x00, sizeof(radiocheck));
			memset(tmpRadio, 0x00, sizeof(tmpRadio));
			memset(tmpBSSID, 0x00, sizeof(tmpBSSID));
			memset(tmpRSSI, 0x00, sizeof(tmpRSSI));
			memset(tmpSSID, 0x00, sizeof(tmpSSID));
			memset(tmpChannel, 0x00, sizeof(tmpChannel));
			memset(tmpAuth, 0x00, sizeof(tmpAuth));
			memset(tmpEncry, 0x00, sizeof(tmpEncry));
			memset(tmpNetworkType, 0x00, sizeof(tmpNetworkType));
			memset(tmpImg, 0x00, sizeof(tmpImg));
			memset(tmpSSIDII, 0x00, sizeof(tmpSSIDII));
			memset(TMPssid, 0x00, sizeof(TMPssid));

			num++;
			fgets(pBssidList, 65536, fp);

			snprintf(tmpChannel, 4, "%s", pBssidList); 
			delspace(tmpChannel, &TMPchannel[0]);
			atoi(TMPchannel);
			if (atoi(TMPchannel) ==  0){
				break;
			}

			snprintf(tmpSSID, 35, "%s", pBssidList+4);
			delquote(tmpSSID+1, &TMPssid[0]);

			snprintf(tmpBSSID, 19, "%s", pBssidList+39);
			delspace(tmpBSSID, &TMPbssid[0]);

			int i;
			for(i=0;i<sizeof(TMPbssid);i++)
				TMPbssid[i]=toupper(TMPbssid[i]);

			snprintf(tmpAuth, 23, "%s", pBssidList+59);
			delspace(tmpAuth, &TMPauth[0]);

			snprintf(tmpRSSI, 9, "%s", pBssidList+82);
			delspace(tmpRSSI, &TMPrssi[0]);
			
			snprintf(tmpNetworkType, 3, "%s", pBssidList+106);

			if (memcmp(BssidQuery, TMPbssid, 6) == 0){
				sprintf((char *)tmpImg, "<img src=\"images/checkmrk.gif\"> ");
			}else{
				sprintf((char *)tmpImg, " ");
			}

			system("/sbin/ezp-wps-set 1 00 4 0");
			if (NULL == (tmp = fopen("/tmp/wisp_essid_mac", "r"))){
				websWrite(wp, T("None"));
				return 0;
			}else{
				fgets(set_bssid, 28, tmp);
				set_bssid[strlen(set_bssid)-4] = '\0';
				delspace(set_bssid, &TMPset_bssid[0]);
				if(!strcmp(TMPbssid, set_bssid)){
					sprintf((char *)tmpImg, "<img src=\"images/checkmrk.gif\"> ");
				}
				else{
					sprintf((char *)tmpImg, " ");
				}
			}

			if (strcmp((char *)TMPssid, "") == 0){
				memset(SSID, 0, 128);
				//sprintf((char *)SSID, "%s%s", tmpImg, "&nbsp;");
				sprintf((char *)SSID, "%s",  "&nbsp;");
			} else {
				int j = 0;
				int flag = 0;
				do {
					if (TMPssid[j] < 32 || TMPssid[j] > 126 || TMPssid[j] == 13) // 13 is string end of Dos
					{
						strcpy((char *)TMPssid, "&nbsp;");
						flag = 1;
						break;
					}
					j++;
				} while(j < strlen(TMPssid)-1);
				
				ssid_value = get_translate_char(flag,TMPssid);
				memset(SSID, 0, 128);
				//sprintf((char *)SSID, "%s%s", tmpImg, ssid_value);
				sprintf((char *)SSID, "%s", ssid_value);
			}

			if(!strcmp(TMPauth, "WPAPSK/AES")){
				sprintf(tmpAuth, "WPAPSK");
				sprintf(tmpEncry, "AES");	
			}else if(!strcmp(TMPauth, "WPAPSK/TKIP")){
				sprintf(tmpAuth, "WPAPSK");
				sprintf(tmpEncry, "TKIP");	
			}else if(!strcmp(TMPauth, "WPA1PSKWPA2PSK/TKIPAES")){
				sprintf(tmpAuth, "WPA1PSKWPA2PSK");
				sprintf(tmpEncry, "TKIPAES");
			}else if(!strcmp(TMPauth, "WPA2PSK/AES")){
				sprintf(tmpAuth, "WPA2PSK");
				sprintf(tmpEncry, "AES");
			}else if(!strcmp(TMPauth, "WPA2PSK/TKIP")){
				sprintf(tmpAuth, "WPA2PSK");
				sprintf(tmpEncry, "TKIP");
			}else if(!strcmp(TMPauth, "WEP")){
				sprintf(tmpAuth, "Unknown");
				sprintf(tmpEncry, "WEP");
			}else if(!strcmp(TMPauth, "NONE")){
				sprintf(tmpAuth, "NONE");
				sprintf(tmpEncry, "Not Use");
			}else if(!strcmp(TMPauth, "WPA1PSKWPA2PSK/AES")){
				sprintf(tmpAuth, "WPA1PSKWPA2PSK");
				sprintf(tmpEncry, "AES");
			}else if(!strcmp(TMPauth, "WPA1PSKWPA2PSK/TKIP")){
				sprintf(tmpAuth, "WPA1PSKWPA2PSK");
				sprintf(tmpEncry, "TKIP");
			}else if(!strcmp(TMPauth, "WPA/TKIP")){
				sprintf(tmpAuth, "WPA");
				sprintf(tmpEncry, "TKIP");
			}else if(!strcmp(TMPauth, "WPA/AES")){
				sprintf(tmpAuth, "WPA");
				sprintf(tmpEncry, "AES");
			}else if(!strcmp(TMPauth, "WPA/TKIPAES")){
				sprintf(tmpAuth, "WPA");
				sprintf(tmpEncry, "TKIPAES");
			}else if(!strcmp(TMPauth, "WPA2/TKIP")){
				sprintf(tmpAuth, "WPA2");
				sprintf(tmpEncry, "TKIP");
			}else if(!strcmp(TMPauth, "WPA2/AES")){
				sprintf(tmpAuth, "WPA2");
				sprintf(tmpEncry, "AES");
			}else if(!strcmp(TMPauth, "WPA2/TKIPAES")){
				sprintf(tmpAuth, "WPA2");
				sprintf(tmpEncry, "TKIPAES");
			}else if(!strcmp(TMPauth, "WPA1WPA2/TKIPAES")){
				sprintf(tmpAuth, "WPA1WPA2");
				sprintf(tmpEncry, "TKIPAES");
			}else if(!strcmp(TMPauth, "WPA2PSK/TKIPAES")){
				sprintf(tmpAuth, "WPA2PSK");
				sprintf(tmpEncry, "TKIPAES");
			}else if(!strcmp(TMPauth, "WPA1WPA2/AES")){
				sprintf(tmpAuth, "WPA1WPA2");
				sprintf(tmpEncry, "AES");
			}else if(!strcmp(TMPauth, "WPA1WPA2/TKIP")){
				sprintf(tmpAuth, "WPA1WPA2");
				sprintf(tmpEncry, "TKIP");
			}else if(!strcmp(TMPauth, "WPAPSK/TKIPAES")){
				sprintf(tmpAuth, "WPAPSK");
				sprintf(tmpEncry, "TKIPAES");
			}

			strcpy(radiocheck, "");

			if(!strcmp(tmpNetworkType, "In")){
				NetworkType = 1;
			}else if (!strcmp(tmpNetworkType, "Ad")){
				NetworkType = 0;
			}

			/* Added by Bruce Liu, 2013-4.27 */
			/* Begin: Process character: '\' */
							memset(tmp_SSID, 0, 128);
							p_ssid = tmp_SSID;
							p_SSID = SSID;
							j = 0;
							while (  ( j < 32) && (p_ssid != NULL) && ( (p_ssid + 5) != NULL)  )  {
								if (SSID[j] == 92) {
									memcpy(p_ssid, "\\\\", 4 );
									p_ssid += 2;
								} else if (SSID[j] == 39){
									memcpy(p_ssid, "\\\'", 4 );
									p_ssid += 2;
								} else {
									memcpy(p_ssid, p_SSID, 1);
									p_ssid =  p_ssid + 1;
								}
								p_SSID ++;
								j ++;
							}
			/* End: '\' */
			/* Ended by Bruce Liu */

			//sprintf((char *)tmpRadio, "<input type=radio name=selectedSSID %s onClick=\"selectedSSIDChange('%s','%s',%d,%d,'%s','%s')\">", radiocheck, TMPssid, TMPbssid, NetworkType, atoi(TMPchannel), tmpEncry, tmpAuth);
			sprintf((char *)tmpRadio, "<input type=radio name=selectedSSID %s onClick=\"selectedSSIDChange('%s','%s',%d,%d,'%s','%s')\">", radiocheck, tmp_SSID, TMPbssid, NetworkType, atoi(TMPchannel), tmpEncry, tmpAuth);

			websWrite(wp, "<td width=\"5%%\"><center>\n");
			websWrite(wp, "<span class=\"table_left\">\n");
			websWrite(wp, "%s\n", tmpRadio);
			websWrite(wp, "</span></center></td>\n");

			websWrite(wp, "<td width=\"30%%\"><center>\n");
			websWrite(wp, "<span class=\"table_font\">\n");
			/* Added by Bruce Liu, 2013-4-24, for multi space of ssid */
			//memset(tmp_SSID, 0, sizeof(tmp_SSID) );
			#if 0
			/* Solution 1: */
			#define SPACE "&nbsp;"
			memset(tmp_SSID, 0, 128);
			p_ssid = tmp_SSID;
			p_SSID = SSID;
			j = 0;
			space_length = 0;
			space_length = strlen(SPACE);
			printf("<<<<<<<<<<<<<<<<<<space's length: %d \n", space_length );
			while (  ( j < 32) && (p_ssid != NULL) && ( (p_ssid + 5) != NULL)  )  {
				printf("----------%c,    %d\n", SSID[j],  SSID[j]);
				//if (isblank(p_SSID[j])) 
				if (SSID[j] == 32) {
					memcpy(p_ssid, SPACE, space_length );
					p_ssid += space_length;
					printf("******************%s\n", tmp_SSID);
				} else {
					memcpy(p_ssid, p_SSID, 1);
					p_ssid =  p_ssid + 1;
				}
				p_SSID ++;
				j ++;
			}
			websWrite(wp, "%s\n", tmp_SSID);
			printf("----------%s\n", tmp_SSID);
			printf("----------%s\n", SSID);
			#undef SPACE
			#else
			/* Solution 2: */
				#if 0
				printf("<<<<<<<<<<<<<<<<<<<<<<<");
				for (i = 0; i < 32; i ++)
					printf("%c", tmp_SSID[i]);
				printf("\n");
				#endif
				printf("<<<<<<<<<<<<<<<<<<<<<<<%s\n", SSID);
				websWrite(wp, "%s", tmpImg);
				//websWrite(wp, "<script>ssid(\"%s\") </script>\n", SSID);
				websWrite(wp, "<script>ssid(\"%s\") </script>\n", tmp_SSID);
			#endif
			/* Ended by Bruce Liu */
			websWrite(wp, "</span></center></td>\n");

			websWrite(wp, "<td width=\"17%%\">\n");
			websWrite(wp, "<span class=\"table_font\">\n");
			websWrite(wp, "%s\n", TMPbssid);
			websWrite(wp, "</span></td>\n");

			websWrite(wp, "<td width=\"7%%\"><center>\n");
			websWrite(wp, "<span class=\"table_font\">\n");
			websWrite(wp, "%s%%\n", TMPrssi);
			websWrite(wp, "</span></center></td>\n");

			websWrite(wp, "<td width=\"5%%\"><center>\n");
			websWrite(wp, "<span class=\"table_font\">\n");
			websWrite(wp, "%s\n", TMPchannel);
			websWrite(wp, "</span></center></td>\n");

			websWrite(wp, "<td width=\"10%%\"><center>\n");
			websWrite(wp, "<span class=\"table_font\">\n");
			websWrite(wp, "%s\n", tmpEncry);
			websWrite(wp, "</span></center></td>\n");

			websWrite(wp, "<td width=\"15%%\"><center>\n");
			websWrite(wp, "<span class=\"table_font\">\n");
			websWrite(wp, "%s\n", tmpAuth);
			websWrite(wp, "</span></center></td>\n");

			websWrite(wp, "<td width=\"10%%\"><center>\n");
			websWrite(wp, "<span class=\"table_right\">\n");
			websWrite(wp, "%s\n", tmpNetworkType);
			websWrite(wp, "</span></center></td>\n");
			websWrite(wp, T("</tr>\n"));
		}
	}

	free(pBssidList);

	fclose(fp);
	return 0; 	
}

static int getwifi5GBSSIDList(int eid, webs_t wp, int argc, char_t **argv)
{
	FILE *fp, *tmp;
	int num = 0, i;
	int NetworkType;
	char *pBssidList = (char*)malloc(65536 * sizeof (char));
	char result[256];
	char set_bssid[28], TMPset_bssid[28], SSID[128];
	char TMPssid[33], TMPrssi[16], TMPchannel[16], TMPauth[20], TMPbssid[28];
	unsigned char tmpRadio[188], tmpBSSID[28], tmpSSID[64], tmpRSSI[16], tmpChannel[16],  
		      tmpAuth[32], tmpEncry[20], tmpNetworkType[24], tmpImg[40];
	unsigned char radiocheck[8];
	unsigned char tmpSSIDII[28];
	unsigned char BssidQuery[6];
	char *ssid_value;
	
	//doSystem("iwpriv ra0 set SiteSurvey=1");
	system("/sbin/ezp-wps-set 0 1 13 1");
	Sleep(4);
	//doSystem("iwpriv ra0 get_site_survey > /etc/site1_survey.dat");
	system("/sbin/ezp-wps-set 0 1 14 0");
	
	if (NULL == (fp = fopen("/etc/site1_survey.dat", "r")))
	{
		websWrite(wp, T("0"));
	}
	else {
			fgets(pBssidList, 65536, fp);
			fgets(pBssidList, 65536, fp);
		
		while(!feof(fp)){
			memset(radiocheck, 0x00, sizeof(radiocheck));
			memset(tmpRadio, 0x00, sizeof(tmpRadio));
			memset(tmpBSSID, 0x00, sizeof(tmpBSSID));
			memset(tmpBSSID, 0x00, sizeof(SSID));
			memset(tmpRSSI, 0x00, sizeof(tmpRSSI));
			memset(tmpSSID, 0x00, sizeof(tmpSSID));
			memset(tmpChannel, 0x00, sizeof(tmpChannel));
			memset(tmpAuth, 0x00, sizeof(tmpAuth));
			memset(tmpEncry, 0x00, sizeof(tmpEncry));
			memset(tmpNetworkType, 0x00, sizeof(tmpNetworkType));
			memset(tmpImg, 0x00, sizeof(tmpImg));
			memset(tmpSSIDII, 0x00, sizeof(tmpSSIDII));
			
			num++;
			fgets(pBssidList, 65536, fp);
			
			snprintf(tmpChannel, 4, "%s", pBssidList); 

			delspace(tmpChannel, &TMPchannel[0]);

			atoi(TMPchannel);
	
			if (atoi(TMPchannel) ==  0){
				break;
			}
			snprintf(tmpSSID, 33, "%s", pBssidList+4);
			pBssidList[36] =  '\0';
			
			delspace(tmpSSID, &TMPssid[0]);  
			
			snprintf(tmpBSSID, 19, "%s", pBssidList+37);
			delspace(tmpBSSID, &TMPbssid[0]);
			

			int i;
			for(i=0;i<sizeof(TMPbssid);i++)
				TMPbssid[i]=toupper(TMPbssid[i]);

			snprintf(tmpAuth, 23, "%s", pBssidList+57);
			delspace(tmpAuth, &TMPauth[0]);

			snprintf(tmpRSSI, 9, "%s", pBssidList+80);
			
			delspace(tmpRSSI, &TMPrssi[0]);
			
			snprintf(tmpNetworkType, 3, "%s", pBssidList+104);			

                    //result[i] = *pBssidList;
			if (memcmp(BssidQuery, TMPbssid, 6) == 0){
                          //printf("Enter BssidQuery=====%s  tmpBSSID====%s\n",BssidQuery,tmpBSSID);
				sprintf((char *)tmpImg, "<img src=\"images/checkmrk.gif\"> ");
			}else{
                           //printf("Enter BssidQuery!!! image empty tmpBSSID====%s\n",tmpBSSID);
				sprintf((char *)tmpImg, " ");
			}
			//match ssid
	//TODO iwconfig apcli0 for 5g
	//system("iwconfig apcli0 | grep 'Access Point:' | sed 's/^.*Access Point: //g' > /tmp/wisp1_essid_mac");	
	system("/sbin/ezp-wps-set 1 01 4 0");
	
	if (NULL == (tmp = fopen("/tmp/wisp1_essid_mac", "r")))
	{
		websWrite(wp, T("None"));
		return 0;
	}else{
		fgets(set_bssid, 28, tmp);

		set_bssid[strlen(set_bssid)-4] = '\0';
              //printf("set bssid ======%s#########\n",set_bssid);
		delspace(set_bssid, &TMPset_bssid[0]);
             //printf("set bssid ======%s@@@@@@@@@\n",set_bssid);
		
		if(!strcmp(TMPbssid, set_bssid)){

			sprintf((char *)tmpImg, "<img src=\"images/checkmrk.gif\"> ");	
                    printf("tmpImg match======%s@@@@@\n",tmpImg);		
			}
		else{
		     sprintf((char *)tmpImg, " ");
                   printf("tmpImg  not match======%s@@@@@@@\n",tmpImg);
		}
	}

		if (strcmp((char *)TMPssid, "") == 0)
		{
			// Tommy,
			sprintf((char *)SSID, "%s%s", tmpImg, "&nbsp;");
                    printf("TMPssid  before=====%s@@@@\n", TMPssid);
		}else {
			int j = 0;
			int flag = 0;
			do {
				if (TMPssid[j] < 32 || TMPssid[j] > 126 || TMPssid[j] == 13) // 13 is string end of Dos
				{
					strcpy((char *)TMPssid, "&nbsp;");
					flag = 1;
					break;
				}
				j++;
			} while(j < strlen(TMPssid)-1);
			// Tommy,
			ssid_value = get_translate_char(flag,TMPssid);
			sprintf((char *)SSID, "%s%s", tmpImg, ssid_value);
                    printf("TMPssid   after=====%s@@@@\n", SSID);
		}
                    printf("TMPssid ????????????  after=====%s@@@@\n", ssid_value);

			if(!strcmp(TMPauth, "WPAPSK/AES")){
				sprintf(tmpAuth, "WPAPSK");
				sprintf(tmpEncry, "AES");				
			}else if(!strcmp(TMPauth, "WPAPSK/TKIP")){
				sprintf(tmpAuth, "WPAPSK");
				sprintf(tmpEncry, "TKIP");				
			}else if(!strcmp(TMPauth, "WPA1PSKWPA2PSK/TKIPAES")){
				sprintf(tmpAuth, "WPA1PSKWPA2PSK");
				sprintf(tmpEncry, "TKIPAES");				
			}else if(!strcmp(TMPauth, "WPA2PSK/AES")){
				sprintf(tmpAuth, "WPA2PSK");
				sprintf(tmpEncry, "AES");				
			}else if(!strcmp(TMPauth, "WPA2PSK/TKIP")){
				sprintf(tmpAuth, "WPA2PSK");
				sprintf(tmpEncry, "TKIP");				
			}else if(!strcmp(TMPauth, "WEP")){
				sprintf(tmpAuth, "Unknown");
				sprintf(tmpEncry, "WEP");				
			}else if(!strcmp(TMPauth, "NONE")){
				sprintf(tmpAuth, "NONE");
				sprintf(tmpEncry, "Not Use");				
			}else if(!strcmp(TMPauth, "WPA1PSKWPA2PSK/AES")){
				sprintf(tmpAuth, "WPA1PSKWPA2PSK");
				sprintf(tmpEncry, "AES");				
			}else if(!strcmp(TMPauth, "WPA1PSKWPA2PSK/TKIP")){
				sprintf(tmpAuth, "WPA1PSKWPA2PSK");
				sprintf(tmpEncry, "TKIP");				
			}else if(!strcmp(TMPauth, "WPA/TKIP")){
				sprintf(tmpAuth, "WPA");
				sprintf(tmpEncry, "TKIP");				
			}else if(!strcmp(TMPauth, "WPA/AES")){
				sprintf(tmpAuth, "WPA");
				sprintf(tmpEncry, "AES");				
			}else if(!strcmp(TMPauth, "WPA/TKIPAES")){
				sprintf(tmpAuth, "WPA");
				sprintf(tmpEncry, "TKIPAES");
			}else if(!strcmp(TMPauth, "WPA2/TKIP")){
				sprintf(tmpAuth, "WPA2");
				sprintf(tmpEncry, "TKIP");
			}else if(!strcmp(TMPauth, "WPA2/AES")){
				sprintf(tmpAuth, "WPA2");
				sprintf(tmpEncry, "AES");
			}else if(!strcmp(TMPauth, "WPA2/TKIPAES")){
				sprintf(tmpAuth, "WPA2");
				sprintf(tmpEncry, "TKIPAES");
			}else if(!strcmp(TMPauth, "WPA1WPA2/TKIPAES")){
				sprintf(tmpAuth, "WPA1WPA2");
				sprintf(tmpEncry, "TKIPAES");
			}else if(!strcmp(TMPauth, "WPA2PSK/TKIPAES")){
				sprintf(tmpAuth, "WPA2PSK");
				sprintf(tmpEncry, "TKIPAES");
			}else if(!strcmp(TMPauth, "WPA1WPA2/AES")){
				sprintf(tmpAuth, "WPA1WPA2");
				sprintf(tmpEncry, "AES");
			}else if(!strcmp(TMPauth, "WPA1WPA2/TKIP")){
				sprintf(tmpAuth, "WPA1WPA2");
				sprintf(tmpEncry, "TKIP");
			}else if(!strcmp(TMPauth, "WPAPSK/TKIPAES")){
				sprintf(tmpAuth, "WPAPSK");
				sprintf(tmpEncry, "TKIPAES");
			}
				
			strcpy(radiocheck, "");
			
			if(!strcmp(tmpNetworkType, "In")){
				NetworkType = 1;				
			}else if (!strcmp(tmpNetworkType, "Ad")){
				NetworkType = 0;
			}

			sprintf((char *)tmpRadio, "<input type=radio name=selectedSSID %s onClick=\"selectedSSIDChange('%s','%s',%d,%d,'%s','%s')\">", radiocheck, TMPssid, TMPbssid, NetworkType, atoi(TMPchannel), tmpEncry, tmpAuth);
			websWrite(wp, "<td width=\"5%%\"><center>\n");
			websWrite(wp, "<span class=\"table_left\">\n");
			websWrite(wp, "%s\n", tmpRadio);
			websWrite(wp, "</span></center></td>\n");

			websWrite(wp, "<td width=\"30%%\"><center>\n");
			websWrite(wp, "<span class=\"table_font\">\n");
			websWrite(wp, "%s\n", SSID);
			websWrite(wp, "</span></center></td>\n");
								
			websWrite(wp, "<td width=\"17%%\">\n");
			websWrite(wp, "<span class=\"table_font\">\n");
			websWrite(wp, "%s\n", TMPbssid);
			websWrite(wp, "</span></td>\n");
						
			websWrite(wp, "<td width=\"7%%\"><center>\n");
			websWrite(wp, "<span class=\"table_font\">\n");
			websWrite(wp, "%s%%\n", TMPrssi);
			websWrite(wp, "</span></center></td>\n");

			websWrite(wp, "<td width=\"5%%\"><center>\n");
			websWrite(wp, "<span class=\"table_font\">\n");
			websWrite(wp, "%s\n", TMPchannel);
			websWrite(wp, "</span></center></td>\n");

			websWrite(wp, "<td width=\"10%%\"><center>\n");
			websWrite(wp, "<span class=\"table_font\">\n");
			websWrite(wp, "%s\n", tmpEncry);
			websWrite(wp, "</span></center></td>\n");
			
			websWrite(wp, "<td width=\"15%%\"><center>\n");
			websWrite(wp, "<span class=\"table_font\">\n");
			websWrite(wp, "%s\n", tmpAuth);
			websWrite(wp, "</span></center></td>\n");

			websWrite(wp, "<td width=\"10%%\"><center>\n");
			websWrite(wp, "<span class=\"table_right\">\n");
			websWrite(wp, "%s\n", tmpNetworkType);
			websWrite(wp, "</span></center></td>\n");
			websWrite(wp, T("</tr>\n"));

			}

	}

	free(pBssidList);
	fclose(fp);
	return 0; 	
}

/* goform/wifiApcli */
static void wifiApcli(webs_t wp, char_t *path, char_t *query)
{
	char_t	*apclienable, *ssid, *bssid, *mode, *enc, *wpapsk, *keyid, *keytype,
			*key1, *key2, *key3, *key4;

	//fetch from web input
	apclienable = websGetVar(wp, T("apcli_enable"), T("0"));
	ssid = websGetVar(wp, T("apcli_ssid"), T(""));
	bssid = websGetVar(wp, T("apcli_bssid"), T(""));
	mode = websGetVar(wp, T("apcli_mode"), T("OPEN"));
	enc = websGetVar(wp, T("apcli_enc"), T("NONE"));
	wpapsk = websGetVar(wp, T("apcli_wpapsk"), T("12345678"));
	keyid = websGetVar(wp, T("apcli_default_key"), T("1"));
	keytype = websGetVar(wp, T("apcli_key1type"), T("1"));
	key1 = websGetVar(wp, T("apcli_key1"), T(""));
	key2 = websGetVar(wp, T("apcli_key2"), T(""));
	key3 = websGetVar(wp, T("apcli_key3"), T(""));
	key4 = websGetVar(wp, T("apcli_key4"), T(""));

	if (gstrlen(ssid) == 0) {
		websError(wp, 200, "SSID is empty");
		return;
	}

	ezplib_replace_attr("wl0_apcli_rule", 0, "enable", apclienable);	
	ezplib_replace_attr("wl0_apcli_rule", 0, "ssid", ssid);	
	ezplib_replace_attr("wl0_apcli_rule", 0, "bssid", bssid);
	if (!strcmp(mode, "OPEN")){
		if ( (!strcmp(key1, "")) && (!strcmp(key2, "")) && (!strcmp(key3, "")) &&(!strcmp(key4, ""))){
			ezplib_replace_attr("wl0_apcli_rule", 0, "secmode", "disabled");
		}else{
			ezplib_replace_attr("wl0_apcli_rule", 0, "secmode", "wep");
			ezplib_replace_attr("wl0_apcli_sec_wep_rule", 0, "encmode", "open"); // OPEN & WEP
		}
	}else if (!strcmp(mode, "SHARED")){
		ezplib_replace_attr("wl0_apcli_rule", 0, "secmode", "wep");
		ezplib_replace_attr("wl0_apcli_sec_wep_rule", 0, "encmode", "shared"); // SHARED KEY & WEP
	}else if (!strcmp(mode, "WPAPSK")){
		ezplib_replace_attr("wl0_apcli_rule", 0, "secmode", "psk");
		if (!strcmp(enc, "TKIP")){
			ezplib_replace_attr("wl0_apcli_sec_wpa_rule", 0, "crypto", "tkip");
		}else if (!strcmp(enc, "AES")){
			ezplib_replace_attr("wl0_apcli_sec_wpa_rule", 0, "crypto", "aes");
		}	
		ezplib_replace_attr("wl0_apcli_sec_wpa_rule", 0, "key", wpapsk);
	}else if (!strcmp(mode, "WPA2PSK")){
		ezplib_replace_attr("wl0_apcli_rule", 0, "secmode", "psk2");
		if (!strcmp(enc, "TKIP")){
			ezplib_replace_attr("wl0_apcli_sec_wpa2_rule", 0, "crypto", "tkip");
		}else if (!strcmp(enc, "AES")){
			ezplib_replace_attr("wl0_apcli_sec_wpa2_rule", 0, "crypto", "aes");
		}	
		ezplib_replace_attr("wl0_apcli_sec_wpa2_rule", 0, "key", wpapsk);
	}
	ezplib_replace_attr("wl0_apcli_sec_wep_rule", 0, "key_index", keyid);
	ezplib_replace_attr("wl0_apcli_sec_wep_rule", 0, "keytype", keytype);
	ezplib_replace_attr("wl0_apcli_sec_wep_rule", 0, "key1", key1);
	ezplib_replace_attr("wl0_apcli_sec_wep_rule", 0, "key2", key2);
	ezplib_replace_attr("wl0_apcli_sec_wep_rule", 0, "key3", key3);
	ezplib_replace_attr("wl0_apcli_sec_wep_rule", 0, "key4", key4);

	system("/sbin/ezp-wl0-ctrl ur");
	//doSystem("iwpriv rai0 set WscConfStatus=2");
	system("/sbin/ezp-wps-set 0 0 1 2");
	nvram_commit();

#if 0
// Tommy, Fixed Multiple SSID or AP Client module(apcli0) cannot rmmod rt2880_ap.ko 
	int i;
	int old_bssid_num = atoi(nvram_bufget(RT2860_NVRAM, "BssidNum"));
	for (i = 0 ; i < old_bssid_num	; i++){
	     doSystem("ifconfig ra%d down",i);
	}	     

	char *operation_mode = nvram_bufget(RT2860_NVRAM, "OP_Mode");
	
	if (!strcmp(operation_mode, "5")){
	   doSystem("ifconfig apcli0 down");		
	}
	
	// down all WDS interface wsd0 - wsd3
	char *wdsMode = nvram_bufget(RT2860_NVRAM, "WdsEnable");
	
	if (strcmp(wdsMode, "0")){
	    for (i = 0 ; i < 4; i++){
	         doSystem("ifconfig wds%d down",i);
	    }     
	}	
#endif		
	//initInternet();

	//debug print
#if 0	
	websHeader(wp);
	websWrite(wp, T("ssid: %s<br>\n"), ssid);
	websWrite(wp, T("bssid: %s<br>\n"), bssid);
	websWrite(wp, T("mode: %s<br>\n"), mode);
	websWrite(wp, T("enc: %s<br>\n"), enc);
	if (!strcmp(mode, "WPAPSK") || !strcmp(mode, "WPA2PSK")) {
		websWrite(wp, T("wpapsk: %s<br>\n"), wpapsk);
	}
	if (!strcmp(mode, "OPEN") || !strcmp(mode, "SHARED")) {
		websWrite(wp, T("keyid: %s<br>\n"), keyid);
		websWrite(wp, T("keytype: %s<br>\n"), keytype);
		websWrite(wp, T("key1: %s<br>\n"), key1);
		websWrite(wp, T("key2: %s<br>\n"), key2);
		websWrite(wp, T("key3: %s<br>\n"), key3);
		websWrite(wp, T("key4: %s<br>\n"), key4);
	}
	websFooter(wp);
	websDone(wp, 200);
#endif	
#if 1//Arthur Chow 2009-03-06		
		setWebMessage(0, NULL);
#endif
	websRedirect(wp, "local/advance/apcli.asp");		
}

/* goform/wispwifiApcli */
static void wispwifiApcli(webs_t wp, char_t *path, char_t *query)
{
	char_t	*apclienable, *ssid, *bssid, *mode, *enc, *wpapsk, *keyid, *keytype,
			*key1, *key2, *key3, *key4, *tmp, *auth_method, *channel;
	char_t *wpa_auth, *wpa_user,*wpa_passwd,*wpa_crypto;

	//fetch from web input
	channel = websGetVar(wp, T("apcli_channel"), T(""));
//	apclienable = websGetVar(wp, T("apcli_enable"), T("0"));
	ssid = websGetVar(wp, T("apcli_ssid"), T(""));
	bssid = websGetVar(wp, T("apcli_bssid"), T(""));
	mode = websGetVar(wp, T("apcli_mode"), T("OPEN"));
	auth_method = websGetVar(wp, T("auth_method"), T(""));
	enc = websGetVar(wp, T("cipher"), T("NONE"));
	wpapsk = websGetVar(wp, T("apcli_wpapsk"), T("12345678"));
	keyid = websGetVar(wp, T("DefWEPKey"), T("1"));
	keytype = websGetVar(wp, T("WEPKey_Code"), T("1"));
	key1 = websGetVar(wp, T("apcli_key1"), T(""));
	key2 = websGetVar(wp, T("apcli_key2"), T(""));
	key3 = websGetVar(wp, T("apcli_key3"), T(""));
	key4 = websGetVar(wp, T("apcli_key4"), T(""));
	wpa_auth = websGetVar(wp, T("cipher2"), T("1"));
	wpa_user = websGetVar(wp, T("apcli_wpa_username"), T(""));
	wpa_passwd = websGetVar(wp, T("apcli_wpa_password"), T(""));
      wpa_crypto = websGetVar(wp, T("cipher3"), T(""));
//printf("mode    ===========%s\n", mode);
//printf("enc    ===========%s\n", enc);
//printf("auth_method    ===========%s\n", auth_method);

	if (gstrlen(ssid) == 0) {
		websError(wp, 200, "SSID is empty");
		return;
	}

	if(!strcmp(enc, "0")){
		tmp = "TKIP";
	}else if (!strcmp(enc, "1")){
		tmp = "AES";
	}
	if (0 != strlen(ssid)) {
		ezplib_replace_attr("wl0_apcli_rule", 0, "enable", "1");		
		}
//	ezplib_replace_attr("wl0_apcli_rule", 0, "enable", apclienable);	
	ezplib_replace_attr("wl0_apcli_rule", 0, "ssid", ssid);

	nvram_set("WISP_Channel", channel);	
	ezplib_replace_attr("wl_basic_rule", 0, "channel", channel);

	ezplib_replace_attr("wl0_apcli_rule", 0, "bssid", bssid);
	if (!strcmp(mode, "OPEN")){
		if ( (!strcmp(key1, "")) && (!strcmp(key2, "")) && (!strcmp(key3, "")) &&(!strcmp(key4, ""))){
			ezplib_replace_attr("wl0_apcli_rule", 0, "secmode", "disabled");
		}else{
			ezplib_replace_attr("wl0_apcli_rule", 0, "secmode", "wep");
			ezplib_replace_attr("wl0_apcli_sec_wep_rule", 0, "encmode", "open"); // OPEN & WEP
		}
	}else if (!strcmp(mode, "SHARED")){
		ezplib_replace_attr("wl0_apcli_rule", 0, "secmode", "wep");
			if (!strcmp(auth_method, "1")){
				ezplib_replace_attr("wl0_apcli_sec_wep_rule", 0, "encmode", "shared"); // SHARED KEY & WEP
			}else {
				ezplib_replace_attr("wl0_apcli_sec_wep_rule", 0, "encmode", "open");
			}
	}else if (!strcmp(mode, "WPAPSK")){
		ezplib_replace_attr("wl0_apcli_rule", 0, "secmode", "psk");
		if (!strcmp(tmp, "TKIP")){
			ezplib_replace_attr("wl0_apcli_sec_wpa_rule", 0, "crypto", "tkip");
		}else if (!strcmp(tmp, "AES")){
			ezplib_replace_attr("wl0_apcli_sec_wpa_rule", 0, "crypto", "aes");
		}	
		ezplib_replace_attr("wl0_apcli_sec_wpa_rule", 0, "key", wpapsk);
	}else if (!strcmp(mode, "WPA2PSK")){
		ezplib_replace_attr("wl0_apcli_rule", 0, "secmode", "psk2");
		if (!strcmp(tmp, "TKIP")){
			ezplib_replace_attr("wl0_apcli_sec_wpa2_rule", 0, "crypto", "tkip");
		}else if (!strcmp(tmp, "AES")){
			ezplib_replace_attr("wl0_apcli_sec_wpa2_rule", 0, "crypto", "aes");
		}	
		ezplib_replace_attr("wl0_apcli_sec_wpa2_rule", 0, "key", wpapsk);
	}else if (!strcmp(mode, "WPA2")){
		ezplib_replace_attr("wl0_apcli_rule", 0, "secmode", "wpa2");
		ezplib_replace_attr("wl0_wpa_auth_rule", 0, "wpa_auth", wpa_auth);
		ezplib_replace_attr("wl0_wpa_auth_rule", 0, "wpa_user", wpa_user);
		ezplib_replace_attr("wl0_wpa_auth_rule", 0, "wpa_passwd", wpa_passwd);
              if (!strcmp(wpa_crypto, "0")){
			ezplib_replace_attr("wl0_wpa_auth_rule", 0, "wpa_crypto", "tkip");
                     ezplib_replace_attr("wl0_apcli_sec_wpa_rule", 0, "crypto", "TKIP");                   
		}else if (!strcmp(wpa_crypto, "1")){
			ezplib_replace_attr("wl0_wpa_auth_rule", 0, "wpa_crypto", "aes");
                     ezplib_replace_attr("wl0_apcli_sec_wpa_rule", 0, "crypto", "AES");   
		}
	}else if (!strcmp(mode, "WPA")){
		ezplib_replace_attr("wl0_apcli_rule", 0, "secmode", "wpa");
		ezplib_replace_attr("wl0_wpa_auth_rule", 0, "wpa_auth", wpa_auth);
		ezplib_replace_attr("wl0_wpa_auth_rule", 0, "wpa_user", wpa_user);
		ezplib_replace_attr("wl0_wpa_auth_rule", 0, "wpa_passwd", wpa_passwd);
              if (!strcmp(wpa_crypto, "0")){
			ezplib_replace_attr("wl0_wpa_auth_rule", 0, "wpa_crypto", "tkip");
                     ezplib_replace_attr("wl0_apcli_sec_wpa_rule", 0, "crypto", "TKIP");                   
		}else if (!strcmp(wpa_crypto, "1")){
			ezplib_replace_attr("wl0_wpa_auth_rule", 0, "wpa_crypto", "aes");
                     ezplib_replace_attr("wl0_apcli_sec_wpa_rule", 0, "crypto", "AES");   
		}
	}
	ezplib_replace_attr("wl0_apcli_sec_wep_rule", 0, "key_index", keyid);
	ezplib_replace_attr("wl0_apcli_sec_wep_rule", 0, "keytype", keytype);
	ezplib_replace_attr("wl0_apcli_sec_wep_rule", 0, "key1", key1);
	ezplib_replace_attr("wl0_apcli_sec_wep_rule", 0, "key2", key2);
	ezplib_replace_attr("wl0_apcli_sec_wep_rule", 0, "key3", key3);
	ezplib_replace_attr("wl0_apcli_sec_wep_rule", 0, "key4", key4);
	nvram_commit();
	system("/sbin/ezp-wl0-ctrl wisp");
	//system("/sbin/ezp-wl0-ctrl basic");  //Disable useless code


#if 0
// Tommy, Fixed Multiple SSID or AP Client module(apcli0) cannot rmmod rt2880_ap.ko 
	int i;
	int old_bssid_num = atoi(nvram_bufget(RT2860_NVRAM, "BssidNum"));
	for (i = 0 ; i < old_bssid_num	; i++){
	     doSystem("ifconfig ra%d down",i);
	}	     

	char *operation_mode = nvram_bufget(RT2860_NVRAM, "OP_Mode");
	
	if (!strcmp(operation_mode, "5")){
	   doSystem("ifconfig apcli0 down");		
	}
	
	// down all WDS interface wsd0 - wsd3
	char *wdsMode = nvram_bufget(RT2860_NVRAM, "WdsEnable");
	
	if (strcmp(wdsMode, "0")){
	    for (i = 0 ; i < 4; i++){
	         doSystem("ifconfig wds%d down",i);
	    }     
	}	
#endif		
	//initInternet();

	//debug print
#if 0	
	websHeader(wp);
	websWrite(wp, T("ssid: %s<br>\n"), ssid);
	websWrite(wp, T("bssid: %s<br>\n"), bssid);
	websWrite(wp, T("mode: %s<br>\n"), mode);
	websWrite(wp, T("enc: %s<br>\n"), enc);
	if (!strcmp(mode, "WPAPSK") || !strcmp(mode, "WPA2PSK")) {
		websWrite(wp, T("wpapsk: %s<br>\n"), wpapsk);
	}
	if (!strcmp(mode, "OPEN") || !strcmp(mode, "SHARED")) {
		websWrite(wp, T("keyid: %s<br>\n"), keyid);
		websWrite(wp, T("keytype: %s<br>\n"), keytype);
		websWrite(wp, T("key1: %s<br>\n"), key1);
		websWrite(wp, T("key2: %s<br>\n"), key2);
		websWrite(wp, T("key3: %s<br>\n"), key3);
		websWrite(wp, T("key4: %s<br>\n"), key4);
	}
	websFooter(wp);
	websDone(wp, 200);
#endif	
#if 1//Arthur Chow 2009-03-06		
		setWebMessage(0, NULL);
#endif
	websRedirect(wp, "local/advance/apcli_wisp.asp");		
}


/* goform/wispwifi5GApcli */
static void wispwifi5GApcli(webs_t wp, char_t *path, char_t *query)
{
	char_t	*apclienable, *ssid, *bssid, *mode, *enc, *wpapsk, *keyid, *keytype,
			*key1, *key2, *key3, *key4, *tmp, *auth_method, *channel;
	char_t *wpa_auth, *wpa_user,*wpa_passwd;

	channel = websGetVar(wp, T("apcli_channel"), T(""));
	ssid = websGetVar(wp, T("apcli_ssid"), T(""));
	bssid = websGetVar(wp, T("apcli_bssid"), T(""));
	mode = websGetVar(wp, T("apcli_mode"), T("OPEN"));
	auth_method = websGetVar(wp, T("auth_method"), T(""));
	enc = websGetVar(wp, T("cipher"), T("NONE"));
	wpapsk = websGetVar(wp, T("apcli_wpapsk"), T("12345678"));
	keyid = websGetVar(wp, T("DefWEPKey"), T("1"));
	keytype = websGetVar(wp, T("WEPKey_Code"), T("1"));
	key1 = websGetVar(wp, T("apcli_key1"), T(""));
	key2 = websGetVar(wp, T("apcli_key2"), T(""));
	key3 = websGetVar(wp, T("apcli_key3"), T(""));
	key4 = websGetVar(wp, T("apcli_key4"), T(""));
	wpa_auth = websGetVar(wp, T("cipher2"), T("1"));
	wpa_user = websGetVar(wp, T("apcli_wpa_username"), T(""));
	wpa_passwd = websGetVar(wp, T("apcli_wpa_password"), T(""));

	if (gstrlen(ssid) == 0) {
		websError(wp, 200, "SSID is empty");
		return;
	}

	if(!strcmp(enc, "0")){
		tmp = "TKIP";
	}else if (!strcmp(enc, "1")){
		tmp = "AES";
	}
	if (0 != strlen(ssid)) {
		ezplib_replace_attr("wl1_apcli_rule", 0, "enable", "1");		
	}
	ezplib_replace_attr("wl1_apcli_rule", 0, "ssid", ssid);

	nvram_set("WISP1_Channel", channel);	
	ezplib_replace_attr("wl5g_basic_rule", 0, "channel", channel);

	ezplib_replace_attr("wl1_apcli_rule", 0, "bssid", bssid);
	if (!strcmp(mode, "OPEN")){
		if ( (!strcmp(key1, "")) && (!strcmp(key2, "")) && (!strcmp(key3, "")) &&(!strcmp(key4, ""))){
			ezplib_replace_attr("wl1_apcli_rule", 0, "secmode", "disabled");
		}else{
			ezplib_replace_attr("wl1_apcli_rule", 0, "secmode", "wep");
			ezplib_replace_attr("wl1_apcli_sec_wep_rule", 0, "encmode", "open"); // OPEN & WEP
		}
	}else if (!strcmp(mode, "SHARED")){
		ezplib_replace_attr("wl1_apcli_rule", 0, "secmode", "wep");
			if (!strcmp(auth_method, "1")){
				ezplib_replace_attr("wl1_apcli_sec_wep_rule", 0, "encmode", "shared"); // SHARED KEY & WEP
			}else {
				ezplib_replace_attr("wl1_apcli_sec_wep_rule", 0, "encmode", "open");
			}
	}else if (!strcmp(mode, "WPAPSK")){
		ezplib_replace_attr("wl1_apcli_rule", 0, "secmode", "psk");
		if (!strcmp(tmp, "TKIP")){
			ezplib_replace_attr("wl1_apcli_sec_wpa_rule", 0, "crypto", "tkip");
		}else if (!strcmp(tmp, "AES")){
			ezplib_replace_attr("wl1_apcli_sec_wpa_rule", 0, "crypto", "aes");
		}	
		ezplib_replace_attr("wl1_apcli_sec_wpa_rule", 0, "key", wpapsk);
	}else if (!strcmp(mode, "WPA2PSK")){
		ezplib_replace_attr("wl1_apcli_rule", 0, "secmode", "psk2");
		if (!strcmp(tmp, "TKIP")){
			ezplib_replace_attr("wl1_apcli_sec_wpa2_rule", 0, "crypto", "tkip");
		}else if (!strcmp(tmp, "AES")){
			ezplib_replace_attr("wl1_apcli_sec_wpa2_rule", 0, "crypto", "aes");
		}	
		ezplib_replace_attr("wl1_apcli_sec_wpa2_rule", 0, "key", wpapsk);
	}else if (!strcmp(mode, "WPA2")){
		ezplib_replace_attr("wl1_apcli_rule", 0, "secmode", "wpa2");
		ezplib_replace_attr("wl1_wpa_auth_rule", 0, "wpa_auth", wpa_auth);
		ezplib_replace_attr("wl1_wpa_auth_rule", 0, "wpa_user", wpa_user);
		ezplib_replace_attr("wl1_wpa_auth_rule", 0, "wpa_passwd", wpa_passwd);
	}else if (!strcmp(mode, "WPA")){
		ezplib_replace_attr("wl1_apcli_rule", 0, "secmode", "wpa");
		ezplib_replace_attr("wl1_wpa_auth_rule", 0, "wpa_auth", wpa_auth);
		ezplib_replace_attr("wl1_wpa_auth_rule", 0, "wpa_user", wpa_user);
		ezplib_replace_attr("wl1_wpa_auth_rule", 0, "wpa_passwd", wpa_passwd);
	}
	ezplib_replace_attr("wl1_apcli_sec_wep_rule", 0, "key_index", keyid);
	ezplib_replace_attr("wl1_apcli_sec_wep_rule", 0, "keytype", keytype);
	ezplib_replace_attr("wl1_apcli_sec_wep_rule", 0, "key1", key1);
	ezplib_replace_attr("wl1_apcli_sec_wep_rule", 0, "key2", key2);
	ezplib_replace_attr("wl1_apcli_sec_wep_rule", 0, "key3", key3);
	ezplib_replace_attr("wl1_apcli_sec_wep_rule", 0, "key4", key4);
	nvram_commit();
	//TODO /sbin/ezp-wl1-ctrl for 5g
	system("/sbin/ezp-wl1-ctrl wisp");
	system("/sbin/ezp-wl1-ctrl basic");

#if 1//Arthur Chow 2009-03-06		
		setWebMessage(0, NULL);
#endif
	websRedirect(wp, "local/advance/apcli_wisp_5g.asp");		
}

/* goform/urwifiApcli */
static void urwifiApcli(webs_t wp, char_t *path, char_t *query)
{
	char_t	*apclienable, *ssid, *bssid, *mode, *enc, *wpapsk, *keyid, *keytype,
			*key1, *key2, *key3, *key4, *tmp, *auth_method, *channel,*apcliaddrmode,*apclilockmac,*apclimacrepeater;
	char_t *wpa_auth, *wpa_user,*wpa_passwd,*wpa_crypto;
	char_t *tmp_SSID[128], *p_ssid, *p_SSID;
	int j = 0;

	channel = websGetVar(wp, T("apcli_channel"), T(""));
	apclienable = websGetVar(wp, T("apcli_enable"), T("0"));
	ssid = websGetVar(wp, T("apcli_ssid"), T(""));
	bssid = websGetVar(wp, T("apcli_Bssid"), T(""));
	mode = websGetVar(wp, T("apcli_mode"), T("OPEN"));
	auth_method = websGetVar(wp, T("auth_method"), T(""));
	enc = websGetVar(wp, T("cipher"), T("NONE"));
	wpapsk = websGetVar(wp, T("apcli_wpapsk"), T("12345678"));
	keyid = websGetVar(wp, T("DefWEPKey"), T("1"));
	keytype = websGetVar(wp, T("WEPKey_Code"), T("1"));
	key1 = websGetVar(wp, T("apcli_key1"), T(""));
	key2 = websGetVar(wp, T("apcli_key2"), T(""));
	key3 = websGetVar(wp, T("apcli_key3"), T(""));
	key4 = websGetVar(wp, T("apcli_key4"), T(""));
	apcliaddrmode = websGetVar(wp, T("apcli_wmode"), T("0"));
	apclilockmac = websGetVar(wp, T("LockMac"), T("0"));
       apclimacrepeater = websGetVar(wp, T("MacRepeater"), T("0"));
	wpa_auth = websGetVar(wp, T("cipher2"), T("1"));
	wpa_user = websGetVar(wp, T("apcli_wpa_username"), T(""));
	wpa_passwd = websGetVar(wp, T("apcli_wpa_password"), T(""));
       wpa_crypto = websGetVar(wp, T("cipher3"), T(""));
     

       if(0==strcmp(apclienable,"0"))
	{
            ezplib_replace_attr("wl0_apcli_rule", 0, "enable", apclienable); 
            nvram_commit();
            
            system("/sbin/ezp-wl0-ctrl ur_switch_ap");
	
	     setWebMessage(0, NULL);
	     websRedirect(wp, "local/advance/ur.asp");
            return;
       }

	if (gstrlen(ssid) == 0) {
		websError(wp, 200, "SSID is empty");
		return;
	}

	if(!strcmp(enc, "0")){
		tmp = "TKIP";
	}else if (!strcmp(enc, "1")){
		tmp = "AES";
	}

	ezplib_replace_attr("wl0_apcli_rule", 0, "enable", apclienable);	
	ezplib_replace_attr("wl0_apcli_rule", 0, "addrmode", apcliaddrmode);	
	#if 0
	/* Added by Bruce Liu, 2013-4-27 for special character */
		memset(tmp_SSID, 0, 128);
		p_ssid = tmp_SSID;
		p_SSID = ssid;
		j = 0;
		while (  ( j < 32) && (p_ssid != NULL) && ( (p_ssid + 5) != NULL)  )  {
				if (ssid[j] == 92) {
						memcpy(p_ssid, "\\\\", 4 );
						p_ssid += 2;
				} else if (ssid[j] == 34) {
						memcpy(p_ssid, "\\\"", 4 );
						p_ssid += 2;
				} else {
						memcpy(p_ssid, p_SSID, 1);
						p_ssid =  p_ssid + 1;
				}
				p_SSID ++;
				j ++;
		}
	/* Ended by Bruce Liu */
	ezplib_replace_attr("wl0_apcli_rule", 0, "ssid", tmp_SSID);
	#endif
	ezplib_replace_attr("wl0_apcli_rule", 0, "ssid", ssid);
        ezplib_replace_attr("wl0_apcli_rule", 0, "lockmac", apclilockmac);
        ezplib_replace_attr("wl0_apcli_rule", 0, "macrepeater", apclimacrepeater);
	if( (bssid != NULL) && strcmp(bssid,""))
		ezplib_replace_attr("wl0_apcli_rule", 0, "bssid", bssid);

//	nvram_set("WISP_Channel", channel);
	nvram_set("AP_Channel", channel);
	ezplib_replace_attr("wl_basic_rule", 0, "channel", channel);

	if (!strcmp(mode, "OPEN")){
		if ( (!strcmp(key1, "")) && (!strcmp(key2, "")) && (!strcmp(key3, "")) &&(!strcmp(key4, ""))){
			ezplib_replace_attr("wl0_apcli_rule", 0, "secmode", "disabled");
		}else{
			ezplib_replace_attr("wl0_apcli_rule", 0, "secmode", "wep");
			ezplib_replace_attr("wl0_apcli_sec_wep_rule", 0, "encmode", "open"); // OPEN & WEP
		}
	}else if (!strcmp(mode, "SHARED")){
		ezplib_replace_attr("wl0_apcli_rule", 0, "secmode", "wep");
			if (!strcmp(auth_method, "1")){
				ezplib_replace_attr("wl0_apcli_sec_wep_rule", 0, "encmode", "shared"); // SHARED KEY & WEP
			}else {
				ezplib_replace_attr("wl0_apcli_sec_wep_rule", 0, "encmode", "open");
			}
	}else if (!strcmp(mode, "WPAPSK")){
		ezplib_replace_attr("wl0_apcli_rule", 0, "secmode", "psk");
		if (!strcmp(tmp, "TKIP")){
			ezplib_replace_attr("wl0_apcli_sec_wpa_rule", 0, "crypto", "tkip");
		}else if (!strcmp(tmp, "AES")){
			ezplib_replace_attr("wl0_apcli_sec_wpa_rule", 0, "crypto", "aes");
		}	
		ezplib_replace_attr("wl0_apcli_sec_wpa_rule", 0, "key", wpapsk);
	}else if (!strcmp(mode, "WPA2PSK")){
		ezplib_replace_attr("wl0_apcli_rule", 0, "secmode", "psk2");
		if (!strcmp(tmp, "TKIP")){
			ezplib_replace_attr("wl0_apcli_sec_wpa2_rule", 0, "crypto", "tkip");
		}else if (!strcmp(tmp, "AES")){
			ezplib_replace_attr("wl0_apcli_sec_wpa2_rule", 0, "crypto", "aes");
		}	
		ezplib_replace_attr("wl0_apcli_sec_wpa2_rule", 0, "key", wpapsk);
	}else if (!strcmp(mode, "WPA2")){
		ezplib_replace_attr("wl0_apcli_rule", 0, "secmode", "wpa2");
		ezplib_replace_attr("wl0_wpa_auth_rule", 0, "wpa_auth", wpa_auth);
		ezplib_replace_attr("wl0_wpa_auth_rule", 0, "wpa_user", wpa_user);
		ezplib_replace_attr("wl0_wpa_auth_rule", 0, "wpa_passwd", wpa_passwd);
              if (!strcmp(wpa_crypto, "0")){
			ezplib_replace_attr("wl0_wpa_auth_rule", 0, "wpa_crypto", "tkip");
                     ezplib_replace_attr("wl0_apcli_sec_wpa_rule", 0, "crypto", "TKIP");                   
		}else if (!strcmp(wpa_crypto, "1")){
			ezplib_replace_attr("wl0_wpa_auth_rule", 0, "wpa_crypto", "aes");
                     ezplib_replace_attr("wl0_apcli_sec_wpa_rule", 0, "crypto", "AES");   
		}
	}else if (!strcmp(mode, "WPA")){
		ezplib_replace_attr("wl0_apcli_rule", 0, "secmode", "wpa");
		ezplib_replace_attr("wl0_wpa_auth_rule", 0, "wpa_auth", wpa_auth);
		ezplib_replace_attr("wl0_wpa_auth_rule", 0, "wpa_user", wpa_user);
		ezplib_replace_attr("wl0_wpa_auth_rule", 0, "wpa_passwd", wpa_passwd);
              if (!strcmp(wpa_crypto, "0")){
			ezplib_replace_attr("wl0_wpa_auth_rule", 0, "wpa_crypto", "tkip");
                     ezplib_replace_attr("wl0_apcli_sec_wpa_rule", 0, "crypto", "TKIP");   
		}else if (!strcmp(wpa_crypto, "1")){
			ezplib_replace_attr("wl0_wpa_auth_rule", 0, "wpa_crypto", "aes");
                     ezplib_replace_attr("wl0_apcli_sec_wpa_rule", 0, "crypto", "AES");   
		}
	}
	ezplib_replace_attr("wl0_apcli_sec_wep_rule", 0, "key_index", keyid);
	ezplib_replace_attr("wl0_apcli_sec_wep_rule", 0, "keytype", keytype);
	ezplib_replace_attr("wl0_apcli_sec_wep_rule", 0, "key1", key1);
	ezplib_replace_attr("wl0_apcli_sec_wep_rule", 0, "key2", key2);
	ezplib_replace_attr("wl0_apcli_sec_wep_rule", 0, "key3", key3);
	ezplib_replace_attr("wl0_apcli_sec_wep_rule", 0, "key4", key4);
	nvram_commit();
    system("/sbin/ezp-wl0-ctrl ur_switch_ap");
	//doSystem("iwpriv rai0 set WscConfStatus=2");
	//system("/sbin/ezp-wps-set 0 0 1 2");

#if 1//Arthur Chow 2009-03-06		
		setWebMessage(0, NULL);
#endif
printf("*******************************************>>>>>>>>>>>>>>>>>>>>>>>>>> ur.asp");
	websRedirect(wp, "local/advance/ur.asp");		
}


/* goform/urwifi5GApcli */
static void urwifi5GApcli(webs_t wp, char_t *path, char_t *query)
{
	char_t	*apclienable, *ssid, *bssid, *mode, *enc, *wpapsk, *keyid, *keytype,
			*key1, *key2, *key3, *key4, *tmp, *auth_method, *channel,*apcliaddrmode, *apclilockmac;
	char_t *wpa_auth, *wpa_user,*wpa_passwd;

	channel = websGetVar(wp, T("apcli_channel"), T(""));
	apclienable = websGetVar(wp, T("apcli_enable"), T("0"));
	ssid = websGetVar(wp, T("apcli_ssid"), T(""));
	bssid = websGetVar(wp, T("apcli_Bssid"), T(""));
	mode = websGetVar(wp, T("apcli_mode"), T("OPEN"));
	auth_method = websGetVar(wp, T("auth_method"), T(""));
	enc = websGetVar(wp, T("cipher"), T("NONE"));
	wpapsk = websGetVar(wp, T("apcli_wpapsk"), T("12345678"));
	keyid = websGetVar(wp, T("DefWEPKey"), T("1"));
	keytype = websGetVar(wp, T("WEPKey_Code"), T("1"));
	key1 = websGetVar(wp, T("apcli_key1"), T(""));
	key2 = websGetVar(wp, T("apcli_key2"), T(""));
	key3 = websGetVar(wp, T("apcli_key3"), T(""));
	key4 = websGetVar(wp, T("apcli_key4"), T(""));
	apcliaddrmode = websGetVar(wp, T("apcli_wmode"), T("0"));
	apclilockmac = websGetVar(wp, T("LockMac"), T("0"));
	wpa_auth = websGetVar(wp, T("cipher2"), T("1"));
	wpa_user = websGetVar(wp, T("apcli_wpa_username"), T(""));
	wpa_passwd = websGetVar(wp, T("apcli_wpa_password"), T(""));

	if (gstrlen(ssid) == 0) {
		websError(wp, 200, "SSID is empty");
		return;
	}

	if(!strcmp(enc, "0")){
		tmp = "TKIP";
	}else if (!strcmp(enc, "1")){
		tmp = "AES";
	}
	
	ezplib_replace_attr("wl1_apcli_rule", 0, "enable", apclienable);	
	ezplib_replace_attr("wl1_apcli_rule", 0, "addrmode", apcliaddrmode);	
	ezplib_replace_attr("wl1_apcli_rule", 0, "ssid", ssid);
	ezplib_replace_attr("wl1_apcli_rule", 0, "lockmac", apclilockmac);

	nvram_set("AP1_Channel", channel);
	ezplib_replace_attr("wl5g_basic_rule", 0, "channel", channel);

	ezplib_replace_attr("wl1_apcli_rule", 0, "bssid", bssid);
	if (!strcmp(mode, "OPEN")){
		if ( (!strcmp(key1, "")) && (!strcmp(key2, "")) && (!strcmp(key3, "")) &&(!strcmp(key4, ""))){
			ezplib_replace_attr("wl1_apcli_rule", 0, "secmode", "disabled");
		}else{
			ezplib_replace_attr("wl1_apcli_rule", 0, "secmode", "wep");
			ezplib_replace_attr("wl1_apcli_sec_wep_rule", 0, "encmode", "open"); // OPEN & WEP
		}
	}else if (!strcmp(mode, "SHARED")){
		ezplib_replace_attr("wl1_apcli_rule", 0, "secmode", "wep");
			if (!strcmp(auth_method, "1")){
				ezplib_replace_attr("wl1_apcli_sec_wep_rule", 0, "encmode", "shared"); // SHARED KEY & WEP
			}else {
				ezplib_replace_attr("wl1_apcli_sec_wep_rule", 0, "encmode", "open");
			}
	}else if (!strcmp(mode, "WPAPSK")){
		ezplib_replace_attr("wl1_apcli_rule", 0, "secmode", "psk");
		if (!strcmp(tmp, "TKIP")){
			ezplib_replace_attr("wl1_apcli_sec_wpa_rule", 0, "crypto", "tkip");
		}else if (!strcmp(tmp, "AES")){
			ezplib_replace_attr("wl1_apcli_sec_wpa_rule", 0, "crypto", "aes");
		}	
		ezplib_replace_attr("wl1_apcli_sec_wpa_rule", 0, "key", wpapsk);
	}else if (!strcmp(mode, "WPA2PSK")){
		ezplib_replace_attr("wl1_apcli_rule", 0, "secmode", "psk2");
		if (!strcmp(tmp, "TKIP")){
			ezplib_replace_attr("wl1_apcli_sec_wpa2_rule", 0, "crypto", "tkip");
		}else if (!strcmp(tmp, "AES")){
			ezplib_replace_attr("wl1_apcli_sec_wpa2_rule", 0, "crypto", "aes");
		}	
		ezplib_replace_attr("wl1_apcli_sec_wpa2_rule", 0, "key", wpapsk);
	}else if (!strcmp(mode, "WPA2")){
		ezplib_replace_attr("wl1_apcli_rule", 0, "secmode", "wpa2");
		ezplib_replace_attr("wl1_wpa_auth_rule", 0, "wpa_auth", wpa_auth);
		ezplib_replace_attr("wl1_wpa_auth_rule", 0, "wpa_user", wpa_user);
		ezplib_replace_attr("wl1_wpa_auth_rule", 0, "wpa_passwd", wpa_passwd);
	}else if (!strcmp(mode, "WPA")){
		ezplib_replace_attr("wl1_apcli_rule", 0, "secmode", "wpa");
		ezplib_replace_attr("wl1_wpa_auth_rule", 0, "wpa_auth", wpa_auth);
		ezplib_replace_attr("wl1_wpa_auth_rule", 0, "wpa_user", wpa_user);
		ezplib_replace_attr("wl1_wpa_auth_rule", 0, "wpa_passwd", wpa_passwd);
	}
	ezplib_replace_attr("wl1_apcli_sec_wep_rule", 0, "key_index", keyid);
	ezplib_replace_attr("wl1_apcli_sec_wep_rule", 0, "keytype", keytype);
	ezplib_replace_attr("wl1_apcli_sec_wep_rule", 0, "key1", key1);
	ezplib_replace_attr("wl1_apcli_sec_wep_rule", 0, "key2", key2);
	ezplib_replace_attr("wl1_apcli_sec_wep_rule", 0, "key3", key3);
	ezplib_replace_attr("wl1_apcli_sec_wep_rule", 0, "key4", key4);
	nvram_commit();
	//TODO /sbin/ezp-wl1-ctrl ru for 5g
	system("/sbin/ezp-wl1-ctrl ur");
	//doSystem("iwpriv ra0 set WscConfStatus=2");
       system("/sbin/ezp-wps-set 0 1 1 2");

#if 1//Arthur Chow 2009-03-06		
		setWebMessage(0, NULL);
#endif
printf("*******************************************>>>>>>>>>>>>>>>>>>>>>>>>>> ur.asp");
	websRedirect(wp, "local/advance/ur_5g.asp");		
}

/* goform/wirelessWds */
#if Include_Ralink_Original
static void wirelessWds(webs_t wp, char_t *path, char_t *query)
{
	char_t	*wds_mode, *wds_phy_mode, *wds_encryp_type, *wds_encryp_key0,
			*wds_encryp_key1,*wds_encryp_key2, *wds_encryp_key3, *wds_list;

	wds_mode = websGetVar(wp, T("wds_mode"), T("0"));
	wds_phy_mode = websGetVar(wp, T("wds_phy_mode"), T(""));
	wds_encryp_type = websGetVar(wp, T("wds_encryp_type"), T(""));
	wds_encryp_key0 = websGetVar(wp, T("wds_encryp_key0"), T(""));
	wds_encryp_key1 = websGetVar(wp, T("wds_encryp_key1"), T(""));
	wds_encryp_key2 = websGetVar(wp, T("wds_encryp_key2"), T(""));
	wds_encryp_key3 = websGetVar(wp, T("wds_encryp_key3"), T(""));
	wds_list = websGetVar(wp, T("wds_list"), T(""));

	nvram_bufset(RT2860_NVRAM, "WdsEnable", wds_mode);
	if (strncmp(wds_mode, "0", 2)) {
		nvram_bufset(RT2860_NVRAM, "WdsPhyMode", wds_phy_mode);
		nvram_bufset(RT2860_NVRAM, "WdsEncrypType", wds_encryp_type);
		nvram_bufset(RT2860_NVRAM, "Wds0Key", wds_encryp_key0);
		nvram_bufset(RT2860_NVRAM, "Wds1Key", wds_encryp_key1);
		nvram_bufset(RT2860_NVRAM, "Wds2Key", wds_encryp_key2);
		nvram_bufset(RT2860_NVRAM, "Wds3Key", wds_encryp_key3);
		if (!strncmp(wds_mode, "2", 2) || !strncmp(wds_mode, "3", 2)) {
			if (0 != strlen(wds_list))
				nvram_bufset(RT2860_NVRAM, "WdsList", wds_list);
		}
	}
	nvram_commit(RT2860_NVRAM);

	initInternet();

	//debug print
	websHeader(wp);
	websWrite(wp, T("wds_mode: %s<br>\n"), wds_mode);
	websWrite(wp, T("wds_phy_mode: %s<br>\n"), wds_phy_mode);
	websWrite(wp, T("wds_encryp_type: %s<br>\n"), wds_encryp_type);
	websWrite(wp, T("wds_encryp_key0: %s<br>\n"), wds_encryp_key0);
	websWrite(wp, T("wds_encryp_key1: %s<br>\n"), wds_encryp_key1);
	websWrite(wp, T("wds_encryp_key2: %s<br>\n"), wds_encryp_key2);
	websWrite(wp, T("wds_encryp_key3: %s<br>\n"), wds_encryp_key3);
	websWrite(wp, T("wds_list: %s<br>\n"), wds_list);
	websFooter(wp);
	websDone(wp, 200);
}
#endif

/* goform/wirelessApcli */
static void wirelessApcli(webs_t wp, char_t *path, char_t *query)
{
#if 0	
	char_t	*ssid, *bssid, *mode, *enc, *wpapsk, *keyid, *keytype,
			*key1, *key2, *key3, *key4;

	//fetch from web input
	ssid = websGetVar(wp, T("apcli_ssid"), T(""));
	bssid = websGetVar(wp, T("apcli_bssid"), T(""));
	mode = websGetVar(wp, T("apcli_mode"), T("OPEN"));
	enc = websGetVar(wp, T("apcli_enc"), T("NONE"));
	wpapsk = websGetVar(wp, T("apcli_wpapsk"), T("12345678"));
	keyid = websGetVar(wp, T("apcli_default_key"), T("1"));
	keytype = websGetVar(wp, T("apcli_key1type"), T("1"));
	key1 = websGetVar(wp, T("apcli_key1"), T(""));
	key2 = websGetVar(wp, T("apcli_key2"), T(""));
	key3 = websGetVar(wp, T("apcli_key3"), T(""));
	key4 = websGetVar(wp, T("apcli_key4"), T(""));

	if (gstrlen(ssid) == 0) {
		websError(wp, 200, "SSID is empty");
		return;
	}

	nvram_bufset(RT2860_NVRAM, "ApCliEnable", "1");
	nvram_bufset(RT2860_NVRAM, "ApCliSsid", ssid);
	nvram_bufset(RT2860_NVRAM, "ApCliBssid", bssid);
	nvram_bufset(RT2860_NVRAM, "ApCliAuthMode", mode);
	nvram_bufset(RT2860_NVRAM, "ApCliEncrypType", enc);
	nvram_bufset(RT2860_NVRAM, "ApCliWPAPSK", wpapsk);
	nvram_bufset(RT2860_NVRAM, "ApCliDefaultKeyId", keyid);
	nvram_bufset(RT2860_NVRAM, "ApCliKey1Type", keytype);
	nvram_bufset(RT2860_NVRAM, "ApCliKey1Str", key1);
	nvram_bufset(RT2860_NVRAM, "ApCliKey2Type", keytype);
	nvram_bufset(RT2860_NVRAM, "ApCliKey2Str", key2);
	nvram_bufset(RT2860_NVRAM, "ApCliKey3Type", keytype);
	nvram_bufset(RT2860_NVRAM, "ApCliKey3Str", key3);
	nvram_bufset(RT2860_NVRAM, "ApCliKey4Type", keytype);
	nvram_bufset(RT2860_NVRAM, "ApCliKey4Str", key4);
	nvram_commit(RT2860_NVRAM);
	initInternet();

	//debug print
	websHeader(wp);
	websWrite(wp, T("ssid: %s<br>\n"), ssid);
	websWrite(wp, T("bssid: %s<br>\n"), bssid);
	websWrite(wp, T("mode: %s<br>\n"), mode);
	websWrite(wp, T("enc: %s<br>\n"), enc);
	if (!strcmp(mode, "WPAPSK") || !strcmp(mode, "WPA2PSK")) {
		websWrite(wp, T("wpapsk: %s<br>\n"), wpapsk);
	}
	if (!strcmp(mode, "OPEN") || !strcmp(mode, "SHARED")) {
		websWrite(wp, T("keyid: %s<br>\n"), keyid);
		websWrite(wp, T("keytype: %s<br>\n"), keytype);
		websWrite(wp, T("key1: %s<br>\n"), key1);
		websWrite(wp, T("key2: %s<br>\n"), key2);
		websWrite(wp, T("key3: %s<br>\n"), key3);
		websWrite(wp, T("key4: %s<br>\n"), key4);
	}
	websFooter(wp);
	websDone(wp, 200);
#endif
}

/* goform/wirelessWmm */
static void wirelessWmm(webs_t wp, char_t *path, char_t *query)
{
#if 0
	char_t	*ap_aifsn_all, *ap_cwmin_all, *ap_cwmax_all, *ap_txop_all,
			*ap_acm_all, *ap_ackpolicy_all,
			*sta_aifsn_all, *sta_cwmin_all, *sta_cwmax_all, *sta_txop_all,
			*sta_acm_all;

	ap_aifsn_all = websGetVar(wp, T("ap_aifsn_all"), T(""));
	ap_cwmin_all = websGetVar(wp, T("ap_cwmin_all"), T(""));
	ap_cwmax_all = websGetVar(wp, T("ap_cwmax_all"), T(""));
	ap_txop_all = websGetVar(wp, T("ap_txop_all"), T(""));
	ap_acm_all = websGetVar(wp, T("ap_acm_all"), T(""));
	ap_ackpolicy_all = websGetVar(wp, T("ap_ackpolicy_all"), T(""));
	sta_aifsn_all = websGetVar(wp, T("sta_aifsn_all"), T(""));
	sta_cwmin_all = websGetVar(wp, T("sta_cwmin_all"), T(""));
	sta_cwmax_all = websGetVar(wp, T("sta_cwmax_all"), T(""));
	sta_txop_all = websGetVar(wp, T("sta_txop_all"), T(""));
	sta_acm_all = websGetVar(wp, T("sta_acm_all"), T(""));

	if (0 != strlen(ap_aifsn_all))
		nvram_bufset(RT2860_NVRAM, "APAifsn", ap_aifsn_all);
	if (0 != strlen(ap_cwmin_all))
		nvram_bufset(RT2860_NVRAM, "APCwmin", ap_cwmin_all);
	if (0 != strlen(ap_cwmax_all))
		nvram_bufset(RT2860_NVRAM, "APCwmax", ap_cwmax_all);
	if (0 != strlen(ap_txop_all))
		nvram_bufset(RT2860_NVRAM, "APTxop", ap_txop_all);
	if (0 != strlen(ap_acm_all))
		nvram_bufset(RT2860_NVRAM, "APACM", ap_acm_all);
	if (0 != strlen(ap_ackpolicy_all))
		nvram_bufset(RT2860_NVRAM, "AckPolicy", ap_ackpolicy_all);
	if (0 != strlen(sta_aifsn_all))
		nvram_bufset(RT2860_NVRAM, "BSSAifsn", sta_aifsn_all);
	if (0 != strlen(sta_cwmin_all))
		nvram_bufset(RT2860_NVRAM, "BSSCwmin", sta_cwmin_all);
	if (0 != strlen(sta_cwmax_all))
		nvram_bufset(RT2860_NVRAM, "BSSCwmax", sta_cwmax_all);
	if (0 != strlen(sta_txop_all))
		nvram_bufset(RT2860_NVRAM, "BSSTxop", sta_txop_all);
	if (0 != strlen(sta_acm_all))
		nvram_bufset(RT2860_NVRAM, "BSSACM", sta_acm_all);

	nvram_commit(RT2860_NVRAM);

	doSystem("ifconfig ra0 down");
	doSystem("ralink_init make_wireless_config rt2860");
	doSystem("ifconfig ra0 up");
	//after ra0 down&up we must restore WPS status
	WPSRestart();

	websHeader(wp);
	websWrite(wp, T("ap_aifsn_all: %s<br>\n"), ap_aifsn_all);
	websWrite(wp, T("ap_cwmin_all: %s<br>\n"), ap_cwmin_all);
	websWrite(wp, T("ap_cwmax_all: %s<br>\n"), ap_cwmax_all);
	websWrite(wp, T("ap_txop_all: %s<br>\n"), ap_txop_all);
	websWrite(wp, T("ap_acm_all: %s<br>\n"), ap_acm_all);
	websWrite(wp, T("ap_ackpolicy_all: %s<br>\n"), ap_ackpolicy_all);
	websWrite(wp, T("sta_aifsn_all: %s<br>\n"), sta_aifsn_all);
	websWrite(wp, T("sta_cwmin_all: %s<br>\n"), sta_cwmin_all);
	websWrite(wp, T("sta_cwmax_all: %s<br>\n"), sta_cwmax_all);
	websWrite(wp, T("sta_txop_all: %s<br>\n"), sta_txop_all);
	websWrite(wp, T("sta_acm_all: %s<br>\n"), sta_acm_all);
	websFooter(wp);
	websDone(wp, 200);
#endif
}

void restart8021XDaemon(int nvram)
{
#if 0	
	int i, num, apd_flag = 0;
	char *auth_mode = nvram_bufget(nvram, "AuthMode");
	char *ieee8021x = nvram_bufget(nvram, "IEEE8021X");
	char *num_s = nvram_bufget(nvram, "BssidNum");
	if(!num_s)
		return;
	num = atoi(num_s);

	if(nvram == RT2860_NVRAM)
		doSystem("killall rt2860apd 1>/dev/null 2>&1");
	else if(nvram == RTINIC_NVRAM)
		doSystem("killall rtinicapd 1>/dev/null 2>&1");
	else if(nvram == RT2561_NVRAM)
		doSystem("killall rt61apd 1>/dev/null 2>&1");
	
	/*
	 * In fact we only support mbssid[0] to use 802.1x radius settings.
	 */
	for(i=0; i<num; i++){
		char tmp_auth[128];
		if( getNthValueSafe(i, auth_mode, ';', tmp_auth, 128) != -1){
			if(!strcmp(tmp_auth, "WPA") || !strcmp(tmp_auth, "WPA2") || !strcmp(tmp_auth, "WPA1WPA2")){
				apd_flag = 1;
				break;
			}
		}

		if( getNthValueSafe(i, ieee8021x, ';', tmp_auth, 128) != -1){
			if(!strcmp(tmp_auth, "1")){
				apd_flag = 1;
				break;
			}
		}
	}

	if(apd_flag){
		if(nvram == RT2860_NVRAM)
			doSystem("rt2860apd");	
		else if(nvram == RTINIC_NVRAM)
			doSystem("rtinicapd");
		else if(nvram == RT2561_NVRAM)
			doSystem("rt61apd");
	}
#endif
}


/* LFF means "Load From Flash" ...*/
#if 0
#define LFF(result, nvram, x, n)	\
							do{		char tmp[128];										\
									if(! ( x  = nvram_bufget(nvram, #x)) )				\
										tmp[0] = '\0';									\
									else{												\
										if( getNthValueSafe(n, x, ';', tmp, 128) != -1){\
											gstrncat(result, tmp, 4096);				\
										}												\
									}gstrncat(result, "\r", 4096);						\
							}while(0)
#endif
/* Load from Web */
#define LFW(x, y)	do{												\
						if(! ( x = websGetVar(wp, T(#y), T(""))))	\
							return;									\
					}while(0)



static	char reValue[256];
void getSecurity(webs_t wp, char_t *path, char_t *query)
{
	#define RESULT_SIZE 4096
	int  i;
	char_t result[RESULT_SIZE];
	char_t *wpapsk;
	char *value;
    	char buf[TMP_LEN]; 
	int ret;   
	int ssid_num; 
        char *wpa_compatible;   

	memset(result, 0, RESULT_SIZE);
        ssid_num = atoi(nvram_safe_get("wlv_rule_max"));     
	result[0] = '\0';
	sprintf(result, "%d\n",  default_shown_mbssid[0]);	

	for(i=0; i<ssid_num; i++) {
		ezplib_get_attr_val("wl0_ssid_rule", i, "ssid", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;
		
	gstrncat(result, value, RESULT_SIZE);	// SSID
	gstrncat(result, "\r", RESULT_SIZE);

		ezplib_get_attr_val("wl0_sec_wpa2_rule", i, "preauth", buf, TMP_LEN, EZPLIB_USE_CLI);			
		strcpy(reValue, buf);
		value = reValue;	
	
	gstrncat(result, value, RESULT_SIZE);	//  PreAuth  for wpa2
	gstrncat(result, "\r", RESULT_SIZE);
			
		ezplib_get_attr_val("wl0_sec_rule", i, "secmode", buf, TMP_LEN, EZPLIB_USE_CLI);	
	
	if(!strcmp(buf , "disabled")){
		strcpy(buf,"Disable");
	}else if(!strcmp(buf , "wep")){
		ezplib_get_attr_val("wl0_sec_wep_rule", i, "encmode", buf, TMP_LEN, EZPLIB_USE_CLI);
		if(!strcmp(buf , "auto")){
			strcpy(buf,"WEPAUTO");
		}else if(!strcmp(buf , "shared")){
			strcpy(buf,"SHARED");
		}else if(!strcmp(buf , "open")){
			strcpy(buf,"OPEN");
		}		
	}else if(!strcmp(buf , "wpa")){
		strcpy(buf,"WPA");
	}else if(!strcmp(buf , "wpa2")){
		ezplib_get_attr_val("wl0_sec_wpa2_rule", i, "wpacap", buf, TMP_LEN, EZPLIB_USE_CLI);
		if(!strcmp(buf , "1")){
			strcpy(buf, "WPA1WPA2");
		}else
		strcpy(buf,"WPA2");
	}else if(!strcmp(buf , "psk")){
		strcpy(buf,"WPAPSK");
	}else if(!strcmp(buf , "psk2")){
		ezplib_get_attr_val("wl0_sec_wpa2_rule", i, "wpacap", buf, TMP_LEN, EZPLIB_USE_CLI);
		if(!strcmp(buf , "1")){
			strcpy(buf, "WPAPSKWPA2PSK");
		}else
		strcpy(buf,"WPA2PSK");
	}
		strcpy(reValue, buf);
		value = reValue;
	   	
	gstrncat(result, value, RESULT_SIZE);	//  AuthMode
	gstrncat(result, "\r", RESULT_SIZE);
	
	ezplib_get_attr_val("wl0_sec_rule", i, "secmode", buf, TMP_LEN, EZPLIB_USE_CLI);
	if(!strcmp(buf , "wep")){
		strcpy(reValue, "WEP");
		value = reValue;
		gstrncat(result, value, RESULT_SIZE);	//  EncrypType for wep
		gstrncat(result, "\r", RESULT_SIZE);	
	}else {		
	gstrncat(result, "", RESULT_SIZE);	        //  EncrypType  
	gstrncat(result, "\r", RESULT_SIZE);
	}

		ezplib_get_attr_val("wl0_sec_wep_rule", i, "key_index", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
		value = reValue;
	
	gstrncat(result, value, RESULT_SIZE);	        //  defaultkey   
	gstrncat(result, "\r", RESULT_SIZE);  
	
		ezplib_get_attr_val("wl0_sec_wep_rule", i, "keytype", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
		value = reValue;	
	
	gstrncat(result, value, RESULT_SIZE);	         //  Key1Type
	gstrncat(result, "\r", RESULT_SIZE);

		ezplib_get_attr_val("wl0_sec_wep_rule", i, "key1", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
		value = reValue;
	   	
	gstrncat(result, value, RESULT_SIZE); 	         //  key1str 
        gstrncat(result, "\r", RESULT_SIZE);
        
        	ezplib_get_attr_val("wl0_sec_wep_rule", i, "keytype", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
		value = reValue;
       
	gstrncat(result, value, RESULT_SIZE); 	         //  Key2Type
	gstrncat(result, "\r", RESULT_SIZE);
	
		ezplib_get_attr_val("wl0_sec_wep_rule", i, "key2", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;
	
	gstrncat(result, value, RESULT_SIZE);            // key2str  
	gstrncat(result, "\r", RESULT_SIZE);  
	
		ezplib_get_attr_val("wl0_sec_wep_rule", i, "keytype", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
		value = reValue;
	
	gstrncat(result, value, RESULT_SIZE);	          // Key3Type
	gstrncat(result, "\r", RESULT_SIZE);
	
		ezplib_get_attr_val("wl0_sec_wep_rule", i, "key3", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
		value = reValue;

	gstrncat(result, value, RESULT_SIZE);            //  key3str 
	gstrncat(result, "\r", RESULT_SIZE);
	
		ezplib_get_attr_val("wl0_sec_wep_rule", i, "keytype", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
		value = reValue;
	
	gstrncat(result, value, RESULT_SIZE);           //  Key4Type
	gstrncat(result, "\r", RESULT_SIZE);
	
		ezplib_get_attr_val("wl0_sec_wep_rule", i, "key4", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;

	gstrncat(result, value, RESULT_SIZE);            //  key4str
	gstrncat(result, "\r", RESULT_SIZE);
	
	ezplib_get_attr_val("wl0_sec_rule", i, "secmode", buf, TMP_LEN, EZPLIB_USE_CLI);	

	if(!strcmp(buf , "psk")){
		
	      	ezplib_get_attr_val("wl0_sec_wpa_rule", i, "key", buf, TMP_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
		value = reValue;

	gstrncat(result, value, RESULT_SIZE);            //  WPAPSK = passphrase
	gstrncat(result, "\r", RESULT_SIZE);				

	}else if(!strcmp(buf , "psk2")){
		
	   	ezplib_get_attr_val("wl0_sec_wpa2_rule", i, "key", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
		value = reValue;

	gstrncat(result, value, RESULT_SIZE);             //  WPAPSK2 = passphrase
	gstrncat(result, "\r", RESULT_SIZE);
			
	}else {  
	   	ezplib_get_attr_val("wl0_sec_wpa_rule", i, "key", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
		value = reValue;
		
	gstrncat(result, value, RESULT_SIZE);                //  WPAPSK for disable
	gstrncat(result, "\r", RESULT_SIZE);
		
	}
			
	gstrncat(result, "DISABLE", RESULT_SIZE);          //RekeyMethod
	gstrncat(result, "\r", RESULT_SIZE);

	ezplib_get_attr_val("wl0_sec_rule", i, "secmode", buf, TMP_LEN, EZPLIB_USE_CLI);
	
	if((!strcmp(buf , "wpa")) || (!strcmp(buf , "psk"))){		
		ezplib_get_attr_val("wl0_sec_wpa_rule", i, "rekey_time_interval", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;
		
	gstrncat(result, value, RESULT_SIZE);               //RekeyInterval for wpa
	gstrncat(result, "\r", RESULT_SIZE);
	
	}else if((!strcmp(buf , "wpa2")) || (!strcmp(buf , "psk2"))){	
		ezplib_get_attr_val("wl0_sec_wpa2_rule", i, "rekey_time_interval", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;
		
	gstrncat(result, value, RESULT_SIZE);               //RekeyInterval for wpa2
	gstrncat(result, "\r", RESULT_SIZE);
	
	}else{
	
	gstrncat(result, "", RESULT_SIZE);               //RekeyInterval for Null
	gstrncat(result, "\r", RESULT_SIZE);
	
	}

	
	ezplib_get_attr_val("wl0_sec_wpa2_rule", i, "pmkperiod", buf, TMP_LEN, EZPLIB_USE_CLI);	
		strcpy(reValue, buf);
		value = reValue;	
	gstrncat(result, value, RESULT_SIZE);               //PMKCachePeriod ==> PMK Cache Period = ??minutes   ??????????????????????????
	gstrncat(result, "\r", RESULT_SIZE);


	gstrncat(result, "0", RESULT_SIZE);	             //IEEE8021X   
	gstrncat(result, "\r", RESULT_SIZE);
	
	ezplib_get_attr_val("wl0_sec_rule", i, "secmode", buf, TMP_LEN, EZPLIB_USE_CLI);

	if(!strcmp(buf , "wpa")){	
		ezplib_get_attr_val("wl0_sec_wpa_rule", i, "radius_ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;
		
	gstrncat(result, value, RESULT_SIZE);	              //RADIUS_Server for wpa
	gstrncat(result, "\r", RESULT_SIZE);
		
	}else if(!strcmp(buf , "wpa2")){	
			
		ezplib_get_attr_val("wl0_sec_wpa2_rule", i, "radius_ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;
		 
	gstrncat(result, value, RESULT_SIZE);	              //RADIUS_Server for wpa2
	gstrncat(result, "\r", RESULT_SIZE);
		
	}else{
	
	gstrncat(result, "", RESULT_SIZE);	                //RADIUS_Server for Null
	gstrncat(result, "\r", RESULT_SIZE);
	
	}
	
	ezplib_get_attr_val("wl0_sec_rule", i, "secmode", buf, TMP_LEN, EZPLIB_USE_CLI);

	if(!strcmp(buf , "wpa")){	
		
		ezplib_get_attr_val("wl0_sec_wpa_rule", i, "radius_port", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;
	
	gstrncat(result, value, RESULT_SIZE);	               //RADIUS_Port for wpa
	gstrncat(result, "\r", RESULT_SIZE);		
			
	}else if(!strcmp(buf , "wpa2")){	
			
		ezplib_get_attr_val("wl0_sec_wpa2_rule", i, "radius_port", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;
		
	gstrncat(result, value, RESULT_SIZE);	               //RADIUS_Port for wpa2
	gstrncat(result, "\r", RESULT_SIZE);
	
	}else {	
		
		ezplib_get_attr_val("wl0_sec_wpa_rule", i, "radius_port", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;
	
	gstrncat(result, value, RESULT_SIZE);	               //RADIUS_Port for disable
	gstrncat(result, "\r", RESULT_SIZE);		
			
	}
	
	ezplib_get_attr_val("wl0_sec_rule", i, "secmode", buf, TMP_LEN, EZPLIB_USE_CLI);

	if(!strcmp(buf , "wpa")){	
		
		ezplib_get_attr_val("wl0_sec_wpa_rule", i, "radius_key", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;
		
	gstrncat(result, value, RESULT_SIZE);	               //RADIUS_Key for wpa
	gstrncat(result, "\r", RESULT_SIZE);
		
	}else if(!strcmp(buf , "wpa2")){	
		
		ezplib_get_attr_val("wl0_sec_wpa2_rule", i, "radius_key", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;
		
	gstrncat(result, value, RESULT_SIZE);	               //RADIUS_Key for wpa2
	gstrncat(result, "\r", RESULT_SIZE);
	
	}else {	
		
	gstrncat(result, "", RESULT_SIZE);	               //RADIUS_Key for disable
	gstrncat(result, "\r", RESULT_SIZE);
		
	}
	
	ezplib_get_attr_val("wl0_sec_rule", i, "secmode", buf, TMP_LEN, EZPLIB_USE_CLI);

	if(!strcmp(buf , "wpa")){
			
		ezplib_get_attr_val("wl0_sec_wpa_rule", i, "session_timeout", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;
		
	gstrncat(result, value, RESULT_SIZE);	               //session_timeout_interval for wpa
	gstrncat(result, "\r", RESULT_SIZE);
		
	}else if(!strcmp(buf , "wpa2")){
		
		ezplib_get_attr_val("wl0_sec_wpa2_rule", i, "session_timeout", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;

	gstrncat(result, value, RESULT_SIZE);	                //session_timeout_interval for wpa2
	gstrncat(result, "\r", RESULT_SIZE);
			
	}else {
		
		ezplib_get_attr_val("wl0_sec_wpa_rule", i, "session_timeout", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;

	gstrncat(result, value, RESULT_SIZE);	                //session_timeout_interval for disable
	gstrncat(result, "\r", RESULT_SIZE);
			
	}
	
	if (i == 0){
        int rule_num0;     
        rule_num0 = atoi(nvram_safe_get("wl00_acl_num"));  		
		ezplib_get_attr_val("wl00_acl_rule", 0, "policy", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;
	gstrncat(result, value, RESULT_SIZE);	               //AccessPolicy   ???????????????????????
	gstrncat(result, "\r", RESULT_SIZE);
	}else if (i == 1){
        int rule_num1;     
        rule_num1 = atoi(nvram_safe_get("wl01_acl_num"));
		ezplib_get_attr_val("wl01_acl_rule", 0, "policy", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;	
	gstrncat(result, value, RESULT_SIZE);	               //AccessPolicy   ???????????????????????
	gstrncat(result, "\r", RESULT_SIZE);
	}else if (i == 2){
        int rule_num2;     
        rule_num2 = atoi(nvram_safe_get("wl02_acl_num"));	
		ezplib_get_attr_val("wl02_acl_rule", 0, "policy", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;	
	gstrncat(result, value, RESULT_SIZE);	               //AccessPolicy   ???????????????????????
	gstrncat(result, "\r", RESULT_SIZE);
	}else if (i == 3){
        int rule_num3;     
        rule_num3 = atoi(nvram_safe_get("wl03_acl_num"));		
		ezplib_get_attr_val("wl03_acl_rule", 0, "policy", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;	
	gstrncat(result, value, RESULT_SIZE);	               //AccessPolicy   ???????????????????????
	gstrncat(result, "\r", RESULT_SIZE);
	}	


	if (i == 0){
		
        int x, rule_num0;
        char result0[4096] = {0};
        char tmp[TMP_LEN];        
        rule_num0 = atoi(nvram_safe_get("wl00_acl_num"));

		for(x=0; x<rule_num0; x++){	
			ezplib_get_attr_val("wl00_acl_rule", x, "mac", buf, TMP_LEN, EZPLIB_USE_CLI); 		

            		if(x==0){
                	strcat(result0, buf);            
           		}
           		else{
                	sprintf(tmp,";%s", buf);
                	strcat(result0, tmp);              
            		} 
		}

	gstrncat(result, result0, RESULT_SIZE);	               //AccessControlList   ????????????????????
	gstrncat(result, "\r", RESULT_SIZE);
	}
	else if (i == 1){
		
        int x, rule_num1;
        char result1[4096] = {0};
        char tmp[TMP_LEN];        
        rule_num1 = atoi(nvram_safe_get("wl01_acl_num"));
	
		for(x=0; x<rule_num1; x++){			
		ezplib_get_attr_val("wl01_acl_rule", x, "mac", buf, TMP_LEN, EZPLIB_USE_CLI); 		

            		if(x==0){
                	strcat(result1, buf);            
           		}
            		else{
                	sprintf(tmp,";%s", buf);
                	strcat(result1, tmp);              
            		} 
		}
	
	gstrncat(result, result1, RESULT_SIZE);	               //AccessControlList   ????????????????????
	gstrncat(result, "\r", RESULT_SIZE);
	}
	else if (i == 2){
		
        int x, rule_num2;
        char result2[4096] = {0};
        char tmp[TMP_LEN];        
        rule_num2 = atoi(nvram_safe_get("wl02_acl_num"));

		for(x=0; x<rule_num2; x++){				
			ezplib_get_attr_val("wl02_acl_rule", x, "mac", buf, TMP_LEN, EZPLIB_USE_CLI); 		

            		if(x==0){
                	strcat(result2, buf);            
           		}
            		else{
                	sprintf(tmp,";%s", buf);
                	strcat(result2, tmp);              
            		} 
		}
	
	gstrncat(result, result2, RESULT_SIZE);	               //AccessControlList   ????????????????????
	gstrncat(result, "\r", RESULT_SIZE);	
	}
	else if (i == 3){

        int x, rule_num3;
        char result3[4096] = {0};
        char tmp[TMP_LEN];        
        rule_num3 = atoi(nvram_safe_get("wl03_acl_num"));

		for(x=0; x<rule_num3; x++){		
			ezplib_get_attr_val("wl03_acl_rule", x, "mac", buf, TMP_LEN, EZPLIB_USE_CLI); 		

            		if(x==0){
                	strcat(result3, buf);            
           		}
            		else{
                	sprintf(tmp,";%s", buf);
                	strcat(result3, tmp);              
            		} 
		}
		
	gstrncat(result, result3, RESULT_SIZE);	               //AccessControlList   ????????????????????
	gstrncat(result, "\r", RESULT_SIZE);			
	}

	gstrncat(result, "\n", sizeof(result));
	}

#undef RESULT_SIZE
	websWrite(wp, T("HTTP/1.1 200 OK\nContent-type: text/plain\nPragma: no-cache\nCache-Control: no-cache\n\n"));
	websWrite(wp, T("%s"), result);
	websDone(wp, 200);
}

void getSecurity_5g(webs_t wp, char_t *path, char_t *query)
{
	int  i;
	char_t result[4096];
	char_t *wpapsk;
	char *value;
    	char buf[TMP_LEN]; 
	int ret;   
	int ssid_num; 
        char *wpa_compatible;   
        ssid_num = atoi(nvram_safe_get("wlv_rule_max"));     
	result[0] = '\0';
	sprintf(result, "%d\n",  default_shown_mbssid[0]);	

	for(i=0; i<ssid_num; i++) {
		ezplib_get_attr_val("wl1_ssid_rule", i, "ssid", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;
	      		
	gstrncat(result, value, 4096);	// SSID
	gstrncat(result, "\r", 4096);

		ezplib_get_attr_val("wl1_sec_wpa2_rule", i, "preauth", buf, TMP_LEN, EZPLIB_USE_CLI);			
		strcpy(reValue, buf);
		value = reValue;	
	
	gstrncat(result, value, 4096);	//  PreAuth  for wpa2
	gstrncat(result, "\r", 4096);
			
		ezplib_get_attr_val("wl1_sec_rule", i, "secmode", buf, TMP_LEN, EZPLIB_USE_CLI);	
	
	if(!strcmp(buf , "disabled")){
		strcpy(buf,"Disable");
	}else if(!strcmp(buf , "wep")){
		ezplib_get_attr_val("wl1_sec_wep_rule", i, "encmode", buf, TMP_LEN, EZPLIB_USE_CLI);
		if(!strcmp(buf , "auto")){
			strcpy(buf,"WEPAUTO");
		}else if(!strcmp(buf , "shared")){
			strcpy(buf,"SHARED");
		}else if(!strcmp(buf , "open")){
			strcpy(buf,"OPEN");
		}		
	}else if(!strcmp(buf , "wpa")){
		strcpy(buf,"WPA");
	}else if(!strcmp(buf , "wpa2")){
		ezplib_get_attr_val("wl1_sec_wpa2_rule", i, "wpacap", buf, TMP_LEN, EZPLIB_USE_CLI);
		if(!strcmp(buf , "1")){
			strcpy(buf, "WPA1WPA2");
		}else
		strcpy(buf,"WPA2");
	}else if(!strcmp(buf , "psk")){
		strcpy(buf,"WPAPSK");
	}else if(!strcmp(buf , "psk2")){
		ezplib_get_attr_val("wl1_sec_wpa2_rule", i, "wpacap", buf, TMP_LEN, EZPLIB_USE_CLI);
		if(!strcmp(buf , "1")){
			strcpy(buf, "WPAPSKWPA2PSK");
		}else
		strcpy(buf,"WPA2PSK");
	}
		strcpy(reValue, buf);
		value = reValue;
	   	
	gstrncat(result, value, 4096);	//  AuthMode
	gstrncat(result, "\r", 4096);
	
	ezplib_get_attr_val("wl1_sec_rule", i, "secmode", buf, TMP_LEN, EZPLIB_USE_CLI);
	if(!strcmp(buf , "wep")){
		strcpy(reValue, "WEP");
		value = reValue;
		gstrncat(result, value, 4096);	//  EncrypType for wep
		gstrncat(result, "\r", 4096);	
	}else {		
	gstrncat(result, "", 4096);	        //  EncrypType  
	gstrncat(result, "\r", 4096);
	}

		ezplib_get_attr_val("wl1_sec_wep_rule", i, "key_index", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
		value = reValue;
	
	gstrncat(result, value, 4096);	        //  defaultkey   
	gstrncat(result, "\r", 4096);  
	
		ezplib_get_attr_val("wl1_sec_wep_rule", i, "keytype", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
		value = reValue;	
	
	gstrncat(result, value, 4096);	         //  Key1Type
	gstrncat(result, "\r", 4096);

		ezplib_get_attr_val("wl1_sec_wep_rule", i, "key1", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
		value = reValue;
	   	
	gstrncat(result, value, 4096); 	         //  key1str 
        gstrncat(result, "\r", 4096);
        
        	ezplib_get_attr_val("wl1_sec_wep_rule", i, "keytype", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
		value = reValue;
       
	gstrncat(result, value, 4096); 	         //  Key2Type
	gstrncat(result, "\r", 4096);
	
		ezplib_get_attr_val("wl1_sec_wep_rule", i, "key2", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;
	
	gstrncat(result, value, 4096);            // key2str  
	gstrncat(result, "\r", 4096);  
	
		ezplib_get_attr_val("wl1_sec_wep_rule", i, "keytype", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
		value = reValue;
	
	gstrncat(result, value, 4096);	          // Key3Type
	gstrncat(result, "\r", 4096);
	
		ezplib_get_attr_val("wl1_sec_wep_rule", i, "key3", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
		value = reValue;

	gstrncat(result, value, 4096);            //  key3str 
	gstrncat(result, "\r", 4096);
	
		ezplib_get_attr_val("wl1_sec_wep_rule", i, "keytype", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
		value = reValue;
	
	gstrncat(result, value, 4096);           //  Key4Type
	gstrncat(result, "\r", 4096);
	
		ezplib_get_attr_val("wl1_sec_wep_rule", i, "key4", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;

	gstrncat(result, value, 4096);            //  key4str
	gstrncat(result, "\r", 4096);
	
	ezplib_get_attr_val("wl1_sec_rule", i, "secmode", buf, TMP_LEN, EZPLIB_USE_CLI);	

	if(!strcmp(buf , "psk")){
		
	      	ezplib_get_attr_val("wl1_sec_wpa_rule", i, "key", buf, TMP_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
		value = reValue;

	gstrncat(result, value, 4096);            //  WPAPSK = passphrase
	gstrncat(result, "\r", 4096);				

	}else if(!strcmp(buf , "psk2")){
		
	   	ezplib_get_attr_val("wl1_sec_wpa2_rule", i, "key", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
		value = reValue;

	gstrncat(result, value, 4096);             //  WPAPSK2 = passphrase
	gstrncat(result, "\r", 4096);
			
	}else {  
	   	ezplib_get_attr_val("wl1_sec_wpa_rule", i, "key", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
		value = reValue;
		
	gstrncat(result, value, 4096);                //  WPAPSK for disable
	gstrncat(result, "\r", 4096);
		
	}
			
	gstrncat(result, "DISABLE", 4096);          //RekeyMethod
	gstrncat(result, "\r", 4096);

	ezplib_get_attr_val("wl1_sec_rule", i, "secmode", buf, TMP_LEN, EZPLIB_USE_CLI);
	
	if((!strcmp(buf , "wpa")) || (!strcmp(buf , "psk"))){		
		ezplib_get_attr_val("wl1_sec_wpa_rule", i, "rekey_time_interval", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;
		
	gstrncat(result, value, 4096);               //RekeyInterval for wpa
	gstrncat(result, "\r", 4096);
	
	}else if((!strcmp(buf , "wpa2")) || (!strcmp(buf , "psk2"))){	
		ezplib_get_attr_val("wl1_sec_wpa2_rule", i, "rekey_time_interval", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;
		
	gstrncat(result, value, 4096);               //RekeyInterval for wpa2
	gstrncat(result, "\r", 4096);
	
	}else{
	
	gstrncat(result, "", 4096);               //RekeyInterval for Null
	gstrncat(result, "\r", 4096);
	
	}

	
	ezplib_get_attr_val("wl1_sec_wpa2_rule", i, "pmkperiod", buf, TMP_LEN, EZPLIB_USE_CLI);	
		strcpy(reValue, buf);
		value = reValue;	
	gstrncat(result, value, 4096);               //PMKCachePeriod ==> PMK Cache Period = ??minutes   ??????????????????????????
	gstrncat(result, "\r", 4096);


	gstrncat(result, "0", 4096);	             //IEEE8021X   
	gstrncat(result, "\r", 4096);
	
	ezplib_get_attr_val("wl1_sec_rule", i, "secmode", buf, TMP_LEN, EZPLIB_USE_CLI);

	if(!strcmp(buf , "wpa")){	
		ezplib_get_attr_val("wl1_sec_wpa_rule", i, "radius_ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;
		
	gstrncat(result, value, 4096);	              //RADIUS_Server for wpa
	gstrncat(result, "\r", 4096);
		
	}else if(!strcmp(buf , "wpa2")){	
			
		ezplib_get_attr_val("wl1_sec_wpa2_rule", i, "radius_ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;
		 
	gstrncat(result, value, 4096);	              //RADIUS_Server for wpa2
	gstrncat(result, "\r", 4096);
		
	}else{
	
	gstrncat(result, "", 4096);	                //RADIUS_Server for Null
	gstrncat(result, "\r", 4096);
	
	}
	
	ezplib_get_attr_val("wl1_sec_rule", i, "secmode", buf, TMP_LEN, EZPLIB_USE_CLI);

	if(!strcmp(buf , "wpa")){	
		
		ezplib_get_attr_val("wl1_sec_wpa_rule", i, "radius_port", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;
	
	gstrncat(result, value, 4096);	               //RADIUS_Port for wpa
	gstrncat(result, "\r", 4096);		
			
	}else if(!strcmp(buf , "wpa2")){	
			
		ezplib_get_attr_val("wl1_sec_wpa2_rule", i, "radius_port", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;
		
	gstrncat(result, value, 4096);	               //RADIUS_Port for wpa2
	gstrncat(result, "\r", 4096);
	
	}else {	
		
		ezplib_get_attr_val("wl1_sec_wpa_rule", i, "radius_port", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;
	
	gstrncat(result, value, 4096);	               //RADIUS_Port for disable
	gstrncat(result, "\r", 4096);		
			
	}
	
	ezplib_get_attr_val("wl1_sec_rule", i, "secmode", buf, TMP_LEN, EZPLIB_USE_CLI);

	if(!strcmp(buf , "wpa")){	
		
		ezplib_get_attr_val("wl1_sec_wpa_rule", i, "radius_key", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;
		
	gstrncat(result, value, 4096);	               //RADIUS_Key for wpa
	gstrncat(result, "\r", 4096);
		
	}else if(!strcmp(buf , "wpa2")){	
		
		ezplib_get_attr_val("wl1_sec_wpa2_rule", i, "radius_key", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;
		
	gstrncat(result, value, 4096);	               //RADIUS_Key for wpa2
	gstrncat(result, "\r", 4096);
	
	}else {	
		
	gstrncat(result, "", 4096);	               //RADIUS_Key for disable
	gstrncat(result, "\r", 4096);
		
	}
	
	ezplib_get_attr_val("wl1_sec_rule", i, "secmode", buf, TMP_LEN, EZPLIB_USE_CLI);

	if(!strcmp(buf , "wpa")){
			
		ezplib_get_attr_val("wl1_sec_wpa_rule", i, "session_timeout", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;
		
	gstrncat(result, value, 4096);	               //session_timeout_interval for wpa
	gstrncat(result, "\r", 4096);
		
	}else if(!strcmp(buf , "wpa2")){
		
		ezplib_get_attr_val("wl1_sec_wpa2_rule", i, "session_timeout", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;

	gstrncat(result, value, 4096);	                //session_timeout_interval for wpa2
	gstrncat(result, "\r", 4096);
			
	}else {
		
		ezplib_get_attr_val("wl1_sec_wpa_rule", i, "session_timeout", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;

	gstrncat(result, value, 4096);	                //session_timeout_interval for disable
	gstrncat(result, "\r", 4096);
			
	}
	
	if (i == 0){
        int rule_num0;     
        rule_num0 = atoi(nvram_safe_get("wl10_acl_num"));  		
		ezplib_get_attr_val("wl10_acl_rule", 0, "policy", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;
	gstrncat(result, value, 4096);	               //AccessPolicy   ???????????????????????
	gstrncat(result, "\r", 4096);
	}else if (i == 1){
        int rule_num1;     
        rule_num1 = atoi(nvram_safe_get("wl11_acl_num"));
		ezplib_get_attr_val("wl11_acl_rule", 0, "policy", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;	
	gstrncat(result, value, 4096);	               //AccessPolicy   ???????????????????????
	gstrncat(result, "\r", 4096);
	}else if (i == 2){
        int rule_num2;     
        rule_num2 = atoi(nvram_safe_get("wl12_acl_num"));	
		ezplib_get_attr_val("wl12_acl_rule", 0, "policy", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;	
	gstrncat(result, value, 4096);	               //AccessPolicy   ???????????????????????
	gstrncat(result, "\r", 4096);
	}else if (i == 3){
        int rule_num3;     
        rule_num3 = atoi(nvram_safe_get("wl13_acl_num"));		
		ezplib_get_attr_val("wl13_acl_rule", 0, "policy", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;	
	gstrncat(result, value, 4096);	               //AccessPolicy   ???????????????????????
	gstrncat(result, "\r", 4096);
	}	


	if (i == 0){
		
        int x, rule_num0;
        char result0[4096] = {0};
        char tmp[TMP_LEN];        
        rule_num0 = atoi(nvram_safe_get("wl10_acl_num"));

		for(x=0; x<rule_num0; x++){	
			ezplib_get_attr_val("wl10_acl_rule", x, "mac", buf, TMP_LEN, EZPLIB_USE_CLI); 		

            		if(x==0){
                	strcat(result0, buf);            
           		}
           		else{
                	sprintf(tmp,";%s", buf);
                	strcat(result0, tmp);              
            		} 
		}

	gstrncat(result, result0, 4096);	               //AccessControlList   ????????????????????
	gstrncat(result, "\r", 4096);
	}
	else if (i == 1){
		
        int x, rule_num1;
        char result1[4096] = {0};
        char tmp[TMP_LEN];        
        rule_num1 = atoi(nvram_safe_get("wl11_acl_num"));
	
		for(x=0; x<rule_num1; x++){			
		ezplib_get_attr_val("wl11_acl_rule", x, "mac", buf, TMP_LEN, EZPLIB_USE_CLI); 		

            		if(x==0){
                	strcat(result1, buf);            
           		}
            		else{
                	sprintf(tmp,";%s", buf);
                	strcat(result1, tmp);              
            		} 
		}
	
	gstrncat(result, result1, 4096);	               //AccessControlList   ????????????????????
	gstrncat(result, "\r", 4096);
	}
	else if (i == 2){
		
        int x, rule_num2;
        char result2[4096] = {0};
        char tmp[TMP_LEN];        
        rule_num2 = atoi(nvram_safe_get("wl12_acl_num"));

		for(x=0; x<rule_num2; x++){				
			ezplib_get_attr_val("wl12_acl_rule", x, "mac", buf, TMP_LEN, EZPLIB_USE_CLI); 		

            		if(x==0){
                	strcat(result2, buf);            
           		}
            		else{
                	sprintf(tmp,";%s", buf);
                	strcat(result2, tmp);              
            		} 
		}
	
	gstrncat(result, result2, 4096);	               //AccessControlList   ????????????????????
	gstrncat(result, "\r", 4096);	
	}
	else if (i == 3){

        int x, rule_num3;
        char result3[4096] = {0};
        char tmp[TMP_LEN];        
        rule_num3 = atoi(nvram_safe_get("wl13_acl_num"));

		for(x=0; x<rule_num3; x++){		
			ezplib_get_attr_val("wl13_acl_rule", x, "mac", buf, TMP_LEN, EZPLIB_USE_CLI); 		

            		if(x==0){
                	strcat(result3, buf);            
           		}
            		else{
                	sprintf(tmp,";%s", buf);
                	strcat(result3, tmp);              
            		} 
		}
		
	gstrncat(result, result3, 4096);	               //AccessControlList   ????????????????????
	gstrncat(result, "\r", 4096);			
	}

	gstrncat(result, "\n", 4096);
	}
	
	websWrite(wp, T("HTTP/1.1 200 OK\nContent-type: text/plain\nPragma: no-cache\nCache-Control: no-cache\n\n"));
	websWrite(wp, T("%s"), result);
	websDone(wp, 200);
}

static void GetWDSSecurity(webs_t wp, char_t *path, char_t *query)
{
return getWDSSecurity(  wp, path, query);
}
static void Get5GWDSSecurity(webs_t wp, char_t *path, char_t *query)
{
return get5gWDSSecurity(  wp, path, query);
}
static void wirelessGetSecurity(webs_t wp, char_t *path, char_t *query)
{
return getSecurity(  wp, path, query);
}
static void wireless5gGetSecurity(webs_t wp, char_t *path, char_t *query)
{
return getSecurity_5g(  wp, path, query);
}

void getWDSSecurity(webs_t wp, char_t *path, char_t *query)
{
	char reValue[256];	
	char *value;
    	char buf[TMP_LEN];
    	
		ezplib_get_attr_val("wl0_wds_sec_wep_rule", 0, "key", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;

	websWrite(wp, T("HTTP/1.1 200 OK\nContent-type: text/plain\nPragma: no-cache\nCache-Control: no-cache\n\n"));
	websWrite(wp, T("%s"), value);
	websDone(wp, 200);    		
}
void get5gWDSSecurity(webs_t wp, char_t *path, char_t *query)
{
	char reValue[256];	
	char *value;
    	char buf[TMP_LEN];
    	
		ezplib_get_attr_val("wl1_wds_sec_wep_rule", 0, "key", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(reValue, buf);
		value = reValue;

	websWrite(wp, T("HTTP/1.1 200 OK\nContent-type: text/plain\nPragma: no-cache\nCache-Control: no-cache\n\n"));
	websWrite(wp, T("%s"), value);
	websDone(wp, 200);    		
}
// Passphrase to WEP
static	char key40_1[11];
static	char key40_2[11];
static	char key40_3[11];
static	char key40_4[11];
static	u_char keys[21];
extern void Gen40BitsKey(u_char *genkey, char *keystr);
static void wifiget40wepkey(webs_t wp, char_t *path, char_t *query)
{
	char_t result[4096];
	char Temp[6];
	int i;
	
	char passphrase[64];
	
	sscanf(query, "%s", passphrase);
		
	memset(key40_1,0,sizeof(key40_1));
	memset(key40_2,0,sizeof(key40_2));
	memset(key40_3,0,sizeof(key40_3));
	memset(key40_4,0,sizeof(key40_4));
	Gen40BitsKey(keys, passphrase);
	
	//produce 4 40bits WEP string format 
#if 1    
    for (i=0; i<5;i++){
    	sprintf(Temp,"%02x",keys[i]);
    	strcat(key40_1,Temp);
    }
    strcat(key40_1,"\0");
    
    memset(Temp,0,sizeof(Temp));
    for (i=5; i<10;i++){
    	sprintf(Temp,"%02x",keys[i]);
    	strcat(key40_2,Temp);
    }
    strcat(key40_2,"\0");
    
    memset(Temp,0,sizeof(Temp));
    for (i=10; i<15;i++){
    	sprintf(Temp,"%02x",keys[i]);
    	strcat(key40_3,Temp);
    }
    strcat(key40_3,"\0");
    
    memset(Temp,0,sizeof(Temp));
    for (i=15; i<20;i++){
    	sprintf(Temp,"%02x",keys[i]);
    	strcat(key40_4,Temp);
    }
    strcat(key40_4,"\0");
#endif

	memset(result,0,sizeof(result));
	gstrncat(result,key40_1 , 4096);
	gstrncat(result, "\r", 4096);
	gstrncat(result,key40_2 , 4096);
	gstrncat(result, "\r", 4096);
	gstrncat(result,key40_3 , 4096);
	gstrncat(result, "\r", 4096);
	gstrncat(result,key40_4 , 4096);
	gstrncat(result, "\r", 4096);
	gstrncat(result, "\n", 4096);	

	websWrite(wp, T("HTTP/1.1 200 OK\nContent-type: text/plain\nPragma: no-cache\nCache-Control: no-cache\n\n"));
	websWrite(wp, T("%s"), result);
	websDone(wp, 200);	
}

// Passphrase to WEP
static char key128[27];
extern void Gen128BitsKey(u_char *genkey, char *keystr);
static void wifiget128wepkey(webs_t wp, char_t *path, char_t *query)
{
	char_t result[4096];
	char Temp[27];
	int i;
	char passphrase[64];
	
	sscanf(query, "%s", passphrase);

	memset(key128,0,sizeof(key128));
	Gen128BitsKey(keys, passphrase);
	
	memset(Temp,0,sizeof(Temp));
	//produce 1 128bits WEP string format 
#if 1    
    for (i=0; i<13;i++){
    	sprintf( Temp,"%02x",keys[i] );
    	strcat(key128,Temp);
    }
    strcat(key128,"\0");
#endif
	
	memset(result,0,sizeof(result));
	gstrncat(result,key128 , 4096);
	gstrncat(result, "\r", 4096);
	gstrncat(result, "\n", 4096);

	websWrite(wp, T("HTTP/1.1 200 OK\nContent-type: text/plain\nPragma: no-cache\nCache-Control: no-cache\n\n"));
	websWrite(wp, T("%s"), result);
	websDone(wp, 200);	
}

/*
 *   TODO:   move to util.c?
 */
static void STFs(int nvram, int index, char *flash_key, char *value)
{
#if 0	
	char *result;
	char *tmp = nvram_bufget(nvram, flash_key);
	if(!tmp)
		tmp = "";
	result = setNthValue(index, tmp, value);
	nvram_bufset(nvram, flash_key, result);
	return ;
#endif
}

//#define STF(nvram, index, flash_key)	STFs(nvram, index, #flash_key, flash_key)

void updateFlash8021x(int nvram)
{
#if 0	
	char lan_if_addr[16];
	char *RADIUS_Server;
	char *operation_mode;

	// if(! (operation_mode = nvram_bufget(nvram, "OperationMode")))
	if(! (operation_mode = nvram_bufget(RT2860_NVRAM, "OperationMode")))
		return;

	if(! (RADIUS_Server = nvram_bufget(nvram, "RADIUS_Server")))
		return;

	if(!strlen(RADIUS_Server))
		return;

	if(*operation_mode == '0'){ // Bridge
		if (getIfIp(getLanIfName(), lan_if_addr) == -1)
			return;
		nvram_bufset(nvram, "own_ip_addr", lan_if_addr);
		if ((RT2860_NVRAM == nvram) || (RTINIC_NVRAM == nvram)) {
			nvram_bufset(nvram, "EAPifname", getLanIfName());
			nvram_bufset(nvram, "PreAuthifname", getLanIfName());
		} else if (RT2561_NVRAM == nvram)
			nvram_bufset(nvram, "Ethifname", getLanIfName());
	}else if(*operation_mode == '1'){	// Gateway
		if (getIfIp(getLanIfName(), lan_if_addr) == -1)
			return;
		nvram_bufset(nvram, "own_ip_addr", lan_if_addr);
		if ((RT2860_NVRAM == nvram) || (RTINIC_NVRAM == nvram)) {
			nvram_bufset(nvram, "EAPifname", getLanIfName());
			nvram_bufset(nvram, "PreAuthifname", getLanIfName());
		} else if (RT2561_NVRAM == nvram)
			nvram_bufset(nvram, "Ethifname", getLanIfName());
	}else if(*operation_mode == '2'){	// Wireless gateway
		if (getIfIp(getLanIfName(), lan_if_addr) == -1)
			return;
		nvram_bufset(nvram, "own_ip_addr", lan_if_addr);
		if ((RT2860_NVRAM == nvram) || (RTINIC_NVRAM == nvram)) {
			nvram_bufset(nvram, "EAPifname", getLanIfName());
			nvram_bufset(nvram, "PreAuthifname", getLanIfName());
		} else if (RT2561_NVRAM == nvram)
			nvram_bufset(nvram, "Ethifname", getLanIfName());
	}else{
		printf("not op mode\n");
		return;
	}
	nvram_commit(nvram);
#endif	
}


//int AccessPolicy5GHandle(int nvram, webs_t wp, int mbssid)
int AccessPolicy5GHandle( webs_t wp, int mbssid)
{
#if 1	
	char_t *apselect, *newap_list;
	char str[32];
	char ap_list[2048];
	char buf[TMP_LEN],rule[TMP_LEN],tmp[TMP_LEN];
	int i;

	if(mbssid > 8 || mbssid < 0)
		return -1;
	sprintf(str, "apselect_%d", mbssid);	// UI on web page

	apselect = websGetVar(wp, str, T(""));

	if(!apselect){
		printf("cant find %s\n", apselect);
		return -1;
	}

	sprintf(str, "AccessPolicy%d", mbssid);

//	nvram_bufset(nvram, str, apselect);

	sprintf(str, "newap_text_%d", mbssid);

	newap_list = websGetVar(wp, str, T(""));

	
	if(mbssid == 0){
		
        int rt_num0 = atoi(nvram_safe_get("wl10_acl_num"));

		for(i=0;i<=rt_num0;i++){

			ezplib_replace_attr("wl10_acl_rule", i, "policy", apselect);	
		}
		ezplib_replace_attr("wl10_acl_basic_rule", 0, "defpolicy", apselect);                   
		if(!newap_list)
			return -1;
		if(!gstrlen(newap_list))
			return 0;
		            
        rt_num0++;
        sprintf(tmp,"%d", rt_num0);

        nvram_set("wl10_acl_num", tmp); //update rule numbers
        
        snprintf(rule, sizeof(rule),
                     "^%s^%s^%s",
                      "1", newap_list, apselect);
                      
	ezplib_append_rule("wl10_acl_rule", rule);
	}
	else if(mbssid == 1){ 
		
        int rt_num1 = atoi(nvram_safe_get("wl11_acl_num"));

		for(i=0;i<=rt_num1;i++){

			ezplib_replace_attr("wl11_acl_rule", i, "policy", apselect);	
		}
		ezplib_replace_attr("wl11_acl_basic_rule", 0, "defpolicy", apselect);	
		if(!newap_list)
			return -1;
		if(!gstrlen(newap_list))
			return 0;
				     
        rt_num1++;
        sprintf(tmp,"%d", rt_num1);
        nvram_set("wl11_acl_num", tmp); //update rule numbers
        
        snprintf(rule, sizeof(rule),
                     "^%s^%s^%s",
                      "1", newap_list, apselect);
                      
	ezplib_append_rule("wl11_acl_rule", rule);
	}
	else if(mbssid == 2){ 
		
        int rt_num2 = atoi(nvram_safe_get("wl12_acl_num"));

		for(i=0;i<=rt_num2;i++){

			ezplib_replace_attr("wl12_acl_rule", i, "policy", apselect);	
		}
		ezplib_replace_attr("wl12_acl_basic_rule", 0, "defpolicy", apselect);	
		if(!newap_list)
			return -1;
		if(!gstrlen(newap_list))
			return 0;
				              
        rt_num2++;
        sprintf(tmp,"%d", rt_num2);

        nvram_set("wl12_acl_num", tmp); //update rule numbers
        
        snprintf(rule, sizeof(rule),
                     "^%s^%s^%s",
                      "1", newap_list, apselect);
                      
	ezplib_append_rule("wl12_acl_rule", rule);
	}
	else if(mbssid == 3){ 
		
        int rt_num3 = atoi(nvram_safe_get("wl13_acl_num"));

		for(i=0;i<=rt_num3;i++){

			ezplib_replace_attr("wl13_acl_rule", i, "policy", apselect);	
		}
		ezplib_replace_attr("wl13_acl_basic_rule", 0, "defpolicy", apselect);	
		if(!newap_list)
			return -1;
		if(!gstrlen(newap_list))
			return 0;
				     
        rt_num3++;
        sprintf(tmp,"%d", rt_num3);

        nvram_set("wl13_acl_num", tmp); //update rule numbers
        
        snprintf(rule, sizeof(rule),
                     "^%s^%s^%s",
                      "1", newap_list, apselect);
                     
	ezplib_append_rule("wl13_acl_rule", rule);
	}			                   

	return 0;
#endif
}



//int AccessPolicyHandle(int nvram, webs_t wp, int mbssid)
int AccessPolicyHandle( webs_t wp, int mbssid)
{
#if 1	
	char_t *apselect, *newap_list;
	char str[32];
	char ap_list[2048];
	char buf[TMP_LEN],rule[TMP_LEN],tmp[TMP_LEN];
	int i;

	if(mbssid > 8 || mbssid < 0)
		return -1;
	sprintf(str, "apselect_%d", mbssid);	// UI on web page

	apselect = websGetVar(wp, str, T(""));

	if(!apselect){
		printf("cant find %s\n", apselect);
		return -1;
	}

	sprintf(str, "AccessPolicy%d", mbssid);

//	nvram_bufset(nvram, str, apselect);

	sprintf(str, "newap_text_%d", mbssid);

	newap_list = websGetVar(wp, str, T(""));

	
	if(mbssid == 0){
		
        int rt_num0 = atoi(nvram_safe_get("wl00_acl_num"));

		for(i=0;i<=rt_num0;i++){

			ezplib_replace_attr("wl00_acl_rule", i, "policy", apselect);	
		}
		ezplib_replace_attr("wl00_acl_basic_rule", 0, "defpolicy", apselect);                   
		if(!newap_list)
			return -1;
		if(!gstrlen(newap_list))
			return 0;
		            
        rt_num0++;
        sprintf(tmp,"%d", rt_num0);

        nvram_set("wl00_acl_num", tmp); //update rule numbers
        
        snprintf(rule, sizeof(rule),
                     "^%s^%s^%s",
                      "1", newap_list, apselect);
                      
	ezplib_append_rule("wl00_acl_rule", rule);
	}
	else if(mbssid == 1){ 
		
        int rt_num1 = atoi(nvram_safe_get("wl01_acl_num"));

		for(i=0;i<=rt_num1;i++){

			ezplib_replace_attr("wl01_acl_rule", i, "policy", apselect);	
		}
		ezplib_replace_attr("wl01_acl_basic_rule", 0, "defpolicy", apselect);	
		if(!newap_list)
			return -1;
		if(!gstrlen(newap_list))
			return 0;
				     
        rt_num1++;
        sprintf(tmp,"%d", rt_num1);
        nvram_set("wl01_acl_num", tmp); //update rule numbers
        
        snprintf(rule, sizeof(rule),
                     "^%s^%s^%s",
                      "1", newap_list, apselect);
                      
	ezplib_append_rule("wl01_acl_rule", rule);
	}
	else if(mbssid == 2){ 
		
        int rt_num2 = atoi(nvram_safe_get("wl02_acl_num"));

		for(i=0;i<=rt_num2;i++){

			ezplib_replace_attr("wl02_acl_rule", i, "policy", apselect);	
		}
		ezplib_replace_attr("wl02_acl_basic_rule", 0, "defpolicy", apselect);	
		if(!newap_list)
			return -1;
		if(!gstrlen(newap_list))
			return 0;
				              
        rt_num2++;
        sprintf(tmp,"%d", rt_num2);

        nvram_set("wl02_acl_num", tmp); //update rule numbers
        
        snprintf(rule, sizeof(rule),
                     "^%s^%s^%s",
                      "1", newap_list, apselect);
                      
	ezplib_append_rule("wl02_acl_rule", rule);
	}
	else if(mbssid == 3){ 
		
        int rt_num3 = atoi(nvram_safe_get("wl03_acl_num"));

		for(i=0;i<=rt_num3;i++){

			ezplib_replace_attr("wl03_acl_rule", i, "policy", apselect);	
		}
		ezplib_replace_attr("wl03_acl_basic_rule", 0, "defpolicy", apselect);	
		if(!newap_list)
			return -1;
		if(!gstrlen(newap_list))
			return 0;
				     
        rt_num3++;
        sprintf(tmp,"%d", rt_num3);

        nvram_set("wl03_acl_num", tmp); //update rule numbers
        
        snprintf(rule, sizeof(rule),
                     "^%s^%s^%s",
                      "1", newap_list, apselect);
                     
	ezplib_append_rule("wl03_acl_rule", rule);
	}			                   

	return 0;
#endif
}


//void conf8021x(int nvram, webs_t wp, int mbssid)
void conf8021x(webs_t wp, int mbssid)
{
#if 1	
	char_t *RadiusServerIP, *RadiusServerPort, *RadiusServerSecret, *RadiusServerSessionTimeout;//, *RadiusServerIdleTimeout;
	char buf[TMP_LEN];
	char_t *security_mode;
	LFW(RadiusServerIP, RadiusServerIP);
	LFW(RadiusServerPort, RadiusServerPort);
	LFW(RadiusServerSecret, RadiusServerSecret);
	LFW(RadiusServerSessionTimeout, RadiusServerSessionTimeout);
	// LFW(RadiusServerIdleTimeout, RadiusServerIdleTimeout);
	if(!gstrlen(RadiusServerSessionTimeout))
		RadiusServerSessionTimeout = "0";

//	STFs(nvram, mbssid, "RADIUS_Server", RadiusServerIP);
//	STFs(nvram, mbssid, "RADIUS_Port", RadiusServerPort);
//	STFs(nvram, mbssid, "RADIUS_Key", RadiusServerSecret);
//	STFs(nvram, mbssid, "session_timeout_interval", RadiusServerSessionTimeout);

	LFW(security_mode, security_mode);
	if(!strcmp(security_mode , "WPA")){	

		ezplib_replace_attr("wl0_sec_wpa_rule", mbssid, "radius_port", RadiusServerPort);
		ezplib_replace_attr("wl0_sec_wpa_rule", mbssid, "radius_ipaddr", RadiusServerIP);	
		ezplib_replace_attr("wl0_sec_wpa_rule", mbssid, "radius_key", RadiusServerSecret);
		ezplib_replace_attr("wl0_sec_wpa_rule", mbssid, "session_timeout", RadiusServerSessionTimeout);

	}else if(!strcmp(security_mode , "WPA2")){	

		ezplib_replace_attr("wl0_sec_wpa2_rule", mbssid, "radius_port", RadiusServerPort);
		ezplib_replace_attr("wl0_sec_wpa2_rule", mbssid, "radius_ipaddr", RadiusServerIP);
		ezplib_replace_attr("wl0_sec_wpa2_rule", mbssid, "radius_key", RadiusServerSecret);
		ezplib_replace_attr("wl0_sec_wpa2_rule", mbssid, "session_timeout", RadiusServerSessionTimeout);

	}
	

//	updateFlash8021x(nvram);
//	restart8021XDaemon(nvram);
#endif
}

//void conf5g8021x(int nvram, webs_t wp, int mbssid)
void conf5g8021x(webs_t wp, int mbssid)
{
#if 1	
	char_t *RadiusServerIP, *RadiusServerPort, *RadiusServerSecret, *RadiusServerSessionTimeout;//, *RadiusServerIdleTimeout;
	char buf[TMP_LEN];
	char_t *security_mode;
	LFW(RadiusServerIP, RadiusServerIP);
	LFW(RadiusServerPort, RadiusServerPort);
	LFW(RadiusServerSecret, RadiusServerSecret);
	LFW(RadiusServerSessionTimeout, RadiusServerSessionTimeout);
	if(!gstrlen(RadiusServerSessionTimeout))
		RadiusServerSessionTimeout = "0";

	LFW(security_mode, security_mode);
	if(!strcmp(security_mode , "WPA")){	

		ezplib_replace_attr("wl1_sec_wpa_rule", mbssid, "radius_port", RadiusServerPort);
		ezplib_replace_attr("wl1_sec_wpa_rule", mbssid, "radius_ipaddr", RadiusServerIP);	
		ezplib_replace_attr("wl1_sec_wpa_rule", mbssid, "radius_key", RadiusServerSecret);
		ezplib_replace_attr("wl1_sec_wpa_rule", mbssid, "session_timeout", RadiusServerSessionTimeout);

	}else if(!strcmp(security_mode , "WPA2")){	

		ezplib_replace_attr("wl1_sec_wpa2_rule", mbssid, "radius_port", RadiusServerPort);
		ezplib_replace_attr("wl1_sec_wpa2_rule", mbssid, "radius_ipaddr", RadiusServerIP);
		ezplib_replace_attr("wl1_sec_wpa2_rule", mbssid, "radius_key", RadiusServerSecret);
		ezplib_replace_attr("wl1_sec_wpa2_rule", mbssid, "session_timeout", RadiusServerSessionTimeout);

	}
#endif
}

void confWEP(int nvram, webs_t wp, int mbssid)
{
#if 0	
	char_t *DefaultKeyID, *Key1Type, *Key1Str, *Key2Type, *Key2Str, *Key3Type, *Key3Str, *Key4Type, *Key4Str;

	LFW(DefaultKeyID, wep_default_key);
	LFW(Key1Str, wep_key_1);
	LFW(Key2Str, wep_key_2);
	LFW(Key3Str, wep_key_3);
	LFW(Key4Str, wep_key_4);
	LFW(Key1Type, WEP1Select);
	LFW(Key2Type, WEP2Select);
	LFW(Key3Type, WEP3Select);
	LFW(Key4Type, WEP4Select);

	STF(nvram, mbssid, DefaultKeyID);
	STF(nvram, mbssid, Key1Type);
	STF(nvram, mbssid, Key2Type);
	STF(nvram, mbssid, Key3Type);
	STF(nvram, mbssid, Key4Type);
	if ((RT2860_NVRAM == nvram) || (RTINIC_NVRAM == nvram)){
		nvram_bufset(nvram, racat("Key1Str", mbssid+1), Key1Str);
		nvram_bufset(nvram, racat("Key2Str", mbssid+1), Key2Str);
		nvram_bufset(nvram, racat("Key3Str", mbssid+1), Key3Str);
		nvram_bufset(nvram, racat("Key4Str", mbssid+1), Key4Str);
	} else if (RT2561_NVRAM == nvram) {
		// nvram_bufset(nvram, "Key1Str", Key1Str);
		// nvram_bufset(nvram, "Key2Str", Key2Str);
		// nvram_bufset(nvram, "Key3Str", Key3Str);
		// nvram_bufset(nvram, "Key4Str", Key4Str);
		STF(nvram, mbssid, Key1Str);
		STF(nvram, mbssid, Key2Str);
		STF(nvram, mbssid, Key3Str);
		STF(nvram, mbssid, Key4Str);
	}
#endif
}

//void wificonfWEP(int nvram, webs_t wp, int mbssid)
void wificonfWEP(webs_t wp, int mbssid)
{
#if 1	
	char_t *DefaultKeyID, *Key1Type, *Key1Str, *Key2Type, *Key2Str, *Key3Type, *Key3Str, *Key4Type, *Key4Str;
	char_t *DefaultWEPKey;

	LFW(DefaultKeyID, DefWEPKey);
// Tommy , Store WEP default WEP and use for Web page WEP setting	
	DefaultWEPKey = DefaultKeyID;
	LFW(Key1Str, wep_key_1);
	LFW(Key2Str, wep_key_2);
	LFW(Key3Str, wep_key_3);
	LFW(Key4Str, wep_key_4);
	LFW(Key1Type, WEPKey_Code);
	LFW(Key2Type, WEPKey_Code);
	LFW(Key3Type, WEPKey_Code);
	LFW(Key4Type, WEPKey_Code);

//	STF(nvram, mbssid, DefaultKeyID);
	ezplib_replace_attr("wl0_sec_wep_rule", mbssid, "key_index", DefaultKeyID);
// Tommy , Store WEP default WEP and use for Web page WEP setting
//	STF(nvram, mbssid, DefaultWEPKey);	
//	STF(nvram, mbssid, Key1Type);
//	STF(nvram, mbssid, Key2Type);
//	STF(nvram, mbssid, Key3Type);
//	STF(nvram, mbssid, Key4Type);

	ezplib_replace_attr("wl0_sec_wep_rule", mbssid, "key_index", DefaultWEPKey);	
	ezplib_replace_attr("wl0_sec_wep_rule", mbssid, "keytype", Key1Type);	
	ezplib_replace_attr("wl0_sec_wep_rule", mbssid, "keytype", Key2Type);	
	ezplib_replace_attr("wl0_sec_wep_rule", mbssid, "keytype", Key3Type);
	ezplib_replace_attr("wl0_sec_wep_rule", mbssid, "keytype", Key4Type);

/*			
	if ((RT2860_NVRAM == nvram) || (RTINIC_NVRAM == nvram)){
		nvram_bufset(nvram, racat("Key1Str", mbssid+1), Key1Str);
		nvram_bufset(nvram, racat("Key2Str", mbssid+1), Key2Str);
		nvram_bufset(nvram, racat("Key3Str", mbssid+1), Key3Str);
		nvram_bufset(nvram, racat("Key4Str", mbssid+1), Key4Str);
	} else if (RT2561_NVRAM == nvram) {
		// nvram_bufset(nvram, "Key1Str", Key1Str);
		// nvram_bufset(nvram, "Key2Str", Key2Str);
		// nvram_bufset(nvram, "Key3Str", Key3Str);
		// nvram_bufset(nvram, "Key4Str", Key4Str);
		STF(nvram, mbssid, Key1Str);
		STF(nvram, mbssid, Key2Str);
		STF(nvram, mbssid, Key3Str);
		STF(nvram, mbssid, Key4Str);
	}			
*/
	ezplib_replace_attr("wl0_sec_wep_rule", mbssid, "key1", Key1Str);
	ezplib_replace_attr("wl0_sec_wep_rule", mbssid, "key2", Key2Str);	
	ezplib_replace_attr("wl0_sec_wep_rule", mbssid, "key3", Key3Str);
	ezplib_replace_attr("wl0_sec_wep_rule", mbssid, "key4", Key4Str);

#endif	
}


//void wifi5gconfWEP(int nvram, webs_t wp, int mbssid)
void wifi5gconfWEP(webs_t wp, int mbssid)
{
#if 1	
	char_t *DefaultKeyID, *Key1Type, *Key1Str, *Key2Type, *Key2Str, *Key3Type, *Key3Str, *Key4Type, *Key4Str;
	char_t *DefaultWEPKey;

	LFW(DefaultKeyID, DefWEPKey);
// Tommy , Store WEP default WEP and use for Web page WEP setting	
	DefaultWEPKey = DefaultKeyID;
	LFW(Key1Str, wep_key_1);
	LFW(Key2Str, wep_key_2);
	LFW(Key3Str, wep_key_3);
	LFW(Key4Str, wep_key_4);
	LFW(Key1Type, WEPKey_Code);
	LFW(Key2Type, WEPKey_Code);
	LFW(Key3Type, WEPKey_Code);
	LFW(Key4Type, WEPKey_Code);

	ezplib_replace_attr("wl1_sec_wep_rule", mbssid, "key_index", DefaultKeyID);

	ezplib_replace_attr("wl1_sec_wep_rule", mbssid, "key_index", DefaultWEPKey);	
	ezplib_replace_attr("wl1_sec_wep_rule", mbssid, "keytype", Key1Type);	
	ezplib_replace_attr("wl1_sec_wep_rule", mbssid, "keytype", Key2Type);	
	ezplib_replace_attr("wl1_sec_wep_rule", mbssid, "keytype", Key3Type);
	ezplib_replace_attr("wl1_sec_wep_rule", mbssid, "keytype", Key4Type);

	ezplib_replace_attr("wl1_sec_wep_rule", mbssid, "key1", Key1Str);
	ezplib_replace_attr("wl1_sec_wep_rule", mbssid, "key2", Key2Str);	
	ezplib_replace_attr("wl1_sec_wep_rule", mbssid, "key3", Key3Str);
	ezplib_replace_attr("wl1_sec_wep_rule", mbssid, "key4", Key4Str);

#endif	
}

void confWPAGeneral(int nvram, webs_t wp, int mbssid)
{
#if 0	
	char *cipher_str;
	char *key_renewal_interval;

	LFW(cipher_str, cipher);
	LFW(key_renewal_interval, keyRenewalInterval);

	switch(cipher_str[0]){
	case '0':
		STFs(nvram, mbssid, "EncrypType", "TKIP");
		break;
	case '1':
		STFs(nvram, mbssid, "EncrypType", "AES");
		break;
	case '2':
		STFs(nvram, mbssid, "EncrypType", "TKIPAES");
		return;							// there is no tkip-aes mixed mode in WPA-PSK.
	}
	STFs(nvram, mbssid, "DefaultKeyID", "2");	// DefaultKeyID is 2
	STFs(nvram, mbssid, "RekeyInterval", key_renewal_interval);
	STFs(nvram, mbssid, "RekeyMethod", "TIME");		
	STFs(nvram, mbssid, "IEEE8021X", "0");
#endif
}


//void wificonfWPAGeneral(int nvram, webs_t wp, int mbssid, int chipher)
void wificonfWPAGeneral(webs_t wp, int mbssid, int chipher)
{
#if 1	
	char *cipher_str;
	char *key_renewal_interval;
	char_t *security_mode;	

	LFW(key_renewal_interval, keyRenewalInterval);

	if (chipher == 0){
//		STFs(nvram, mbssid, "EncrypType", "TKIP");	//wpa	
		ezplib_replace_attr("wl0_sec_wpa_rule", mbssid, "crypto", "tkip");
	}else if (chipher == 1){
//		STFs(nvram, mbssid, "EncrypType", "AES");	//wpa2	
		ezplib_replace_attr("wl0_sec_wpa2_rule", mbssid, "crypto", "aes");
	}else if (chipher == 2){
//		STFs(nvram, mbssid, "EncrypType", "TKIPAES");	//mixed	
		ezplib_replace_attr("wl0_sec_wpa2_rule", mbssid, "crypto", "mixed");
	}				

	
// Tommy ???? : Why (Bug ????)
//	STFs(nvram, mbssid, "DefaultKeyID", "2");	
//	STFs(nvram, mbssid, "RekeyInterval", key_renewal_interval);
//	STFs(nvram, mbssid, "RekeyMethod", "TIME");	
//	STFs(nvram, mbssid, "IEEE8021X", "0");
	
//	ezplib_replace_attr("wl0_sec_wep_rule", mbssid, "key_index", "2");
	
	LFW(security_mode, security_mode);
	
	if((!strcmp(security_mode , "WPA")) || (!strcmp(security_mode , "WPAPSK"))){	

	ezplib_replace_attr("wl0_sec_wpa_rule", mbssid, "rekey_time_interval", key_renewal_interval);	//group key timer for wpa / psk
	
	}
	else if((!strcmp(security_mode , "WPA2")) || (!strcmp(security_mode , "WPA2PSK"))){
	
	ezplib_replace_attr("wl0_sec_wpa2_rule", mbssid, "rekey_time_interval", key_renewal_interval);   //group key timer for wpa2 / psk2
	
	}
#endif
}


//void wificonf5GWPAGeneral(int nvram, webs_t wp, int mbssid, int chipher)
void wificonf5GWPAGeneral(webs_t wp, int mbssid, int chipher)
{
#if 1	
	char *cipher_str;
	char *key_renewal_interval;
	char_t *security_mode;	

	LFW(key_renewal_interval, keyRenewalInterval);

	if (chipher == 0){
		ezplib_replace_attr("wl1_sec_wpa_rule", mbssid, "crypto", "tkip");
	}else if (chipher == 1){
		ezplib_replace_attr("wl1_sec_wpa2_rule", mbssid, "crypto", "aes");
	}else if (chipher == 2){
		ezplib_replace_attr("wl1_sec_wpa2_rule", mbssid, "crypto", "mixed");
	}				

	LFW(security_mode, security_mode);
	
	if((!strcmp(security_mode , "WPA")) || (!strcmp(security_mode , "WPAPSK"))){	

	ezplib_replace_attr("wl1_sec_wpa_rule", mbssid, "rekey_time_interval", key_renewal_interval);	//group key timer for wpa / psk
	
	}
	else if((!strcmp(security_mode , "WPA2")) || (!strcmp(security_mode , "WPA2PSK"))){
	
	ezplib_replace_attr("wl1_sec_wpa2_rule", mbssid, "rekey_time_interval", key_renewal_interval);   //group key timer for wpa2 / psk2
	
	}
#endif
}

inline void clearRadiusSetting( int mbssid)
{
#if 1	
	char RADIUS_Server[TMP_LEN],  RADIUS_Key[TMP_LEN], *session_timeout_interval = NULL;
	char buf[TMP_LEN]; 
	char RADIUS_Port[TMP_LEN];
//	RADIUS_Server = nvram_bufget(nvram, "RADIUS_Server");
//	RADIUS_Port = nvram_bufget(nvram, "RADIUS_Port");
//	RADIUS_Key = nvram_bufget(nvram, "RADIUS_Key");
	//session_timeout_interval = nvram_bufget(nvram, "session_timeout_interval");

	ezplib_get_attr_val("wl0_sec_rule", mbssid, "secmode", buf, TMP_LEN, EZPLIB_USE_CLI);

	if(!strcmp(buf , "wpa")){	

		ezplib_get_attr_val("wl0_sec_wpa_rule", mbssid, "radius_port", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(RADIUS_Port, buf);
		strcpy(RADIUS_Port, "1812");
		ezplib_replace_attr("wl0_sec_wpa_rule", mbssid, "radius_port", RADIUS_Port);
			
	}else if(!strcmp(buf , "wpa2")){	
		ezplib_get_attr_val("wl0_sec_wpa2_rule", mbssid, "radius_port", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(RADIUS_Port, buf);
		strcpy(RADIUS_Port, "1812");
		ezplib_replace_attr("wl0_sec_wpa2_rule", mbssid, "radius_port", RADIUS_Port);
	}
	
	ezplib_get_attr_val("wl0_sec_rule", mbssid, "secmode", buf, TMP_LEN, EZPLIB_USE_CLI);

	if(!strcmp(buf , "wpa")){	
		ezplib_get_attr_val("wl0_sec_wpa_rule", mbssid, "radius_ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(RADIUS_Server, buf);
		strcpy(RADIUS_Server, "0");
		ezplib_replace_attr("wl0_sec_wpa_rule", mbssid, "radius_ipaddr", RADIUS_Server);
			
	}else if(!strcmp(buf , "wpa2")){		
		ezplib_get_attr_val("wl0_sec_wpa2_rule", mbssid, "radius_ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(RADIUS_Server, buf);
		strcpy(RADIUS_Server, "0");
		ezplib_replace_attr("wl0_sec_wpa2_rule", mbssid, "radius_ipaddr", RADIUS_Server);
	}

	ezplib_get_attr_val("wl0_sec_rule", mbssid, "secmode", buf, TMP_LEN, EZPLIB_USE_CLI);

	if(!strcmp(buf , "wpa")){	
		ezplib_get_attr_val("wl0_sec_wpa_rule", mbssid, "radius_key", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(RADIUS_Key, buf);
		strcpy(RADIUS_Key, "");
		ezplib_replace_attr("wl0_sec_wpa_rule", mbssid, "radius_key", RADIUS_Key);
	
	}else if(!strcmp(buf , "wpa2")){	
		ezplib_get_attr_val("wl0_sec_wpa2_rule", mbssid, "radius_key", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(RADIUS_Key, buf);
		strcpy(RADIUS_Key, "");
		ezplib_replace_attr("wl0_sec_wpa2_rule", mbssid, "radius_key", RADIUS_Key);

	}
printf("enter radius clear setting :\nRADIUS_Port=%s\nRADIUS_Server=%s\nRADIUS_Key=%s\n",RADIUS_Port,RADIUS_Server,RADIUS_Key);
//	nvram_bufset(nvram, "RADIUS_Server", setNthValue(mbssid, RADIUS_Server, "0"));
//	nvram_bufset(nvram, "RADIUS_Port", setNthValue(mbssid, RADIUS_Port, "1812"));


// Tommy,  set Shared Secret = "", 2009/3/21 02:41
#if 0	
	nvram_bufset(nvram, "RADIUS_Key", setNthValue(mbssid, RADIUS_Key, "ralink"));
//#else
	nvram_bufset(nvram, "RADIUS_Key", setNthValue(mbssid, RADIUS_Key, ""));
#endif	
	//nvram_bufset(nvram, "session_timeout_interval", setNthValue(mbssid, session_timeout_interval, ""));
            
	return;
#endif
}


inline void clear5GRadiusSetting( int mbssid)
{
#if 1	
	char RADIUS_Server[TMP_LEN],  RADIUS_Key[TMP_LEN], *session_timeout_interval = NULL;
	char buf[TMP_LEN]; 
	char RADIUS_Port[TMP_LEN];

	ezplib_get_attr_val("wl1_sec_rule", mbssid, "secmode", buf, TMP_LEN, EZPLIB_USE_CLI);

	if(!strcmp(buf , "wpa")){	

		ezplib_get_attr_val("wl1_sec_wpa_rule", mbssid, "radius_port", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(RADIUS_Port, buf);
		strcpy(RADIUS_Port, "1812");
		ezplib_replace_attr("wl1_sec_wpa_rule", mbssid, "radius_port", RADIUS_Port);
			
	}else if(!strcmp(buf , "wpa2")){	
		ezplib_get_attr_val("wl1_sec_wpa2_rule", mbssid, "radius_port", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(RADIUS_Port, buf);
		strcpy(RADIUS_Port, "1812");
		ezplib_replace_attr("wl1_sec_wpa2_rule", mbssid, "radius_port", RADIUS_Port);
	}
	
	ezplib_get_attr_val("wl1_sec_rule", mbssid, "secmode", buf, TMP_LEN, EZPLIB_USE_CLI);

	if(!strcmp(buf , "wpa")){	
		ezplib_get_attr_val("wl1_sec_wpa_rule", mbssid, "radius_ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(RADIUS_Server, buf);
		strcpy(RADIUS_Server, "0");
		ezplib_replace_attr("wl1_sec_wpa_rule", mbssid, "radius_ipaddr", RADIUS_Server);
			
	}else if(!strcmp(buf , "wpa2")){		
		ezplib_get_attr_val("wl1_sec_wpa2_rule", mbssid, "radius_ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(RADIUS_Server, buf);
		strcpy(RADIUS_Server, "0");
		ezplib_replace_attr("wl1_sec_wpa2_rule", mbssid, "radius_ipaddr", RADIUS_Server);
	}

	ezplib_get_attr_val("wl1_sec_rule", mbssid, "secmode", buf, TMP_LEN, EZPLIB_USE_CLI);

	if(!strcmp(buf , "wpa")){	
		ezplib_get_attr_val("wl1_sec_wpa_rule", mbssid, "radius_key", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(RADIUS_Key, buf);
		strcpy(RADIUS_Key, "");
		ezplib_replace_attr("wl1_sec_wpa_rule", mbssid, "radius_key", RADIUS_Key);
	
	}else if(!strcmp(buf , "wpa2")){	
		ezplib_get_attr_val("wl1_sec_wpa2_rule", mbssid, "radius_key", buf, TMP_LEN, EZPLIB_USE_CLI); 		
		strcpy(RADIUS_Key, buf);
		strcpy(RADIUS_Key, "");
		ezplib_replace_attr("wl1_sec_wpa2_rule", mbssid, "radius_key", RADIUS_Key);

	}
printf("enter radius clear setting :\nRADIUS_Port=%s\nRADIUS_Server=%s\nRADIUS_Key=%s\n",RADIUS_Port,RADIUS_Server,RADIUS_Key);
// Tommy,  set Shared Secret = "", 2009/3/21 02:41
#if 0	
	nvram_bufset(nvram, "RADIUS_Key", setNthValue(mbssid, RADIUS_Key, "ralink"));
//#else
	nvram_bufset(nvram, "RADIUS_Key", setNthValue(mbssid, RADIUS_Key, ""));
#endif	

	return;
#endif
}


//aron patch for giga
char* mask2prefix2(char *mask)
{
    if (!strcmp(mask, "0.0.0.0"))
          mask="0";
    else if (!strcmp(mask, "128.0.0.0"))
          mask="1";
    else if (!strcmp(mask, "192.0.0.0"))
          mask="2";
    else if (!strcmp(mask, "224.0.0.0"))
          mask="3";
    else if (!strcmp(mask, "240.0.0.0"))
          mask="4";
    else if (!strcmp(mask, "248.0.0.0"))
          mask="5";
    else if (!strcmp(mask, "252.0.0.0"))
          mask="6";
    else if (!strcmp(mask, "254.0.0.0"))
          mask="7";
    else if (!strcmp(mask, "255.0.0.0"))
          mask="8";
    else if (!strcmp(mask, "255.128.0.0"))
          mask="9";
    else if (!strcmp(mask, "255.192.0.0"))
          mask="10";
    else if (!strcmp(mask, "255.224.0.0"))
          mask="11";
    else if (!strcmp(mask, "255.240.0.0"))
          mask="12";
    else if (!strcmp(mask, "255.248.0.0"))
          mask="13";
    else if (!strcmp(mask, "255.252.0.0"))
          mask="14";
    else if (!strcmp(mask, "255.254.0.0"))
          mask="15";
    else if (!strcmp(mask, "255.255.0.0"))
          mask="16";
    else if (!strcmp(mask, "255.255.128.0"))
          mask="17";
    else if (!strcmp(mask, "255.255.192.0"))
          mask="18";
    else if (!strcmp(mask, "255.255.224.0"))
          mask="19";
    else if (!strcmp(mask, "255.255.240.0"))
          mask="20";
    else if (!strcmp(mask, "255.255.248.0"))
          mask="21";
    else if (!strcmp(mask, "255.255.252.0"))
          mask="22";
    else if (!strcmp(mask, "255.255.254.0"))
          mask="23";
    else if (!strcmp(mask, "255.255.255.0"))
          mask="24";
    else if (!strcmp(mask, "255.255.255.128"))
          mask="25";
    else if (!strcmp(mask, "255.255.255.192"))
          mask="26";
    else if (!strcmp(mask, "255.255.255.224"))
          mask="27";
    else if (!strcmp(mask, "255.255.255.240"))
          mask="28";
    else if (!strcmp(mask, "255.255.255.248"))
          mask="29";
    else if (!strcmp(mask, "255.255.255.252"))
          mask="30";
    else if (!strcmp(mask, "255.255.255.254"))
          mask="31";
    else if (!strcmp(mask, "255.255.255.255"))
          mask="32";
    else
          mask="";
          
    return mask;
}

static int Channellist(int eid, webs_t wp,int argc, char_t **argv)
{
	ChannelListInfo_t t;
	memset(&t, 0, sizeof(ChannelListInfo_t));

	int num;
	Channel_t *channel_list;

	//TODO feak channel_list
	t.num = 11;
	int i = 0;
	for ( i = 0; i< t.num; i++) {
		t.channel_list[i].chan_number = i+1;
		t.channel_list[i].frequency = 2412 + i*5;
	}

	/* get selected channel */

	/* op mode : normal/ap/ur/wisp/wisp_ur*/
	char buf[32];
	ezplib_get_attr_val("wl_mode_rule", 0, "mode", buf, 32, EZPLIB_USE_CLI);
	if (!strcmp(buf, "ap")){
		ezplib_get_attr_val("wl_ap_basic_rule", 0, 
			"channel", buf, 32, EZPLIB_USE_CLI);
	}else {
		ezplib_get_attr_val("wl_basic_rule", 0, 
			"channel", buf, 32, EZPLIB_USE_CLI);
	}
	int chan = atoi(buf);

	websWrite(wp, T("new Array(\n"));
	for ( i = 0; i< t.num; i++) {
		websWrite(wp, T("new Array(\"Channel-%d %dMHz\",\"\",\"%d\",\"%d\",\"%d\",\"0\"),\n"), 
			t.channel_list[i].chan_number,
			t.channel_list[i].frequency,
			t.channel_list[i].chan_number,
			t.channel_list[i].chan_number >=5 ? 1 : 0,
			t.channel_list[i].chan_number <=7 ? 1 : 0 );
	}
	websWrite(wp, T("new Array(\"End of Channel\",\"\",\"0\",\"0\",\"0\",\"0\"))\n"));

/* channel, selected, index, ext_u, ext_l, dfs
	
	var channelBGN=new Array(
		new Array("2412MHz (Channel 1)","","1","0","1","0"),
		new Array("2417MHz (Channel 2)","","2","0","1","0"),
		new Array("2422MHz (Channel 3)","","3","0","1","0"),
		new Array("2427MHz (Channel 4)","","4","0","1","0"),
		new Array("2432MHz (Channel 5)","","5","1","1","0"),
		new Array("2437MHz (Channel 6)","","6","1","1","0"),
		new Array("2442MHz (Channel 7)","","7","1","1","0"),
		new Array("2447MHz (Channel 8)","","8","1","0","0"),
		new Array("2452MHz (Channel 9)","","9","1","0","0"),
		new Array("2457MHz (Channel 10)","","10","1","0","0"),
		new Array("2462MHz (Channel 11)","","11","1","0","0"),
		new Array("SmartSelect","","0","0","0","0"), 
		new Array("End of Channel","","0","0","0","0"))
*/
	return 0;
}

void APGeneral(webs_t wp, char_t *path, char_t *query)
{
	char_t *ssid, *bssid_num, *set_bssid_num;
	char_t *mssid_1, *mssid_2, *mssid_3, *mssid_4;
	char_t *mssid_5, *mssid_6, *mssid_7; 
	char_t *hidessid, *hidemssid_1, *hidemssid_2, *hidemssid_3; 
	char_t *hidemssid_4, *hidemssid_5, *hidemssid_6, *hidemssid_7;
	char_t *intra_bss, *intra_bss1, *intra_bss2, *intra_bss3;
	char_t *intra_bss4, *intra_bss5, *intra_bss6, *intra_bss7;
	char_t *enablessid, *enablessid1, *enablessid2, *enablessid3;
	char_t *enablessid4, *enablessid5, *enablessid6, *enablessid7;

	char_t *main_intra_bss;
	
	int mbssid, mbssid_num, i, ssid_num;
	int new_bssid_num, old_bssid_num;
	char_t *wirelessmode;
	char_t *sz11gChannel, *auto_channel;	
	char_t *n_extcha;
	char_t *n_bandwidth;

	char wordlist[5];
	char TempSSID[128];
	
// Tommy: Implement WLAN Guest function
#if 1
	char TempBuf[32];
	char *wlan_guest = websGetVar(wp, T("wlanguest"), T("0"));
	//Steve add 2011/06/30
	char OldBuf_en[32];
	char OldBuf_ip[32];
	char OldBuf_mask[32];
	char OldBuf_bm_en[32];
	char OldBuf_bm_val[32];
	int GuestNoChange=0;
	ezplib_get_attr_val("guest_lan_rule", 0, "enable", 
		OldBuf_en, 32, EZPLIB_USE_CLI);
	ezplib_replace_attr("guest_lan_rule", 0, "enable", 
		wlan_guest); 

	// Router mode 
	/*TODO:lan static interface update need to be implemented*/
	ezplib_get_attr_val("wl_mode_rule", 0, "mode", 
		TempBuf, 32, EZPLIB_USE_CLI);
	if ( (!strcmp(TempBuf, "normal")) && (!strcmp(wlan_guest, "1")) ) { 
		char *wlan_guest_ip = websGetVar(wp, T("wlanguestip"), T(""));
		char *wlan_guest_mask = websGetVar(wp, T("wlanguestmask"), T(""));
		char *wlan_guestBW_enable = websGetVar(wp, T("wlanguestBW"), T("0"));
		char *wlan_guest_priority = websGetVar(wp, T("guest_priority"), T("1"));
		char *wlan_guest_max_bw = websGetVar(wp, T("guestmaxbw"), T(""));
		char *mask;
		
		mask = mask2prefix2(wlan_guest_mask);
		ezplib_get_attr_val("guest_lan_rule", 0, "ipaddr", OldBuf_ip, 32, EZPLIB_USE_CLI);
		ezplib_get_attr_val("guest_lan_rule", 0, "netmask", OldBuf_mask, 32, EZPLIB_USE_CLI);
		ezplib_get_attr_val("guest_lan_rule", 0, "bwen", OldBuf_bm_en, 32, EZPLIB_USE_CLI);
		ezplib_get_attr_val("guest_lan_rule", 0, "bw", OldBuf_bm_val, 32, EZPLIB_USE_CLI);
		if(	(!strcmp(OldBuf_en, wlan_guest)) && 
			(!strcmp(OldBuf_ip, wlan_guest_ip)) && 
			(!strcmp(OldBuf_mask, mask)) && 
			(!strcmp(OldBuf_bm_en, wlan_guestBW_enable)) && 
			(!strcmp(OldBuf_bm_val, wlan_guest_max_bw)) ){
			GuestNoChange=1;
		}
		else{
			GuestNoChange=0;
		}
		//wlan_guest_ip
		ezplib_replace_attr("guest_lan_rule", 0, 
			"ipaddr", wlan_guest_ip); 
		//wlan_guest_mask, Translate AboCom's netmask to AXIM's nvram format
    	mask = mask2prefix2(wlan_guest_mask);
		ezplib_replace_attr("guest_lan_rule", 0, 
			"netmask", mask); 
		//wlan_guestBW_enable
		ezplib_replace_attr("guest_lan_rule", 0, "bwen", 
			wlan_guestBW_enable); 
		//wlan_guest_priority
		ezplib_replace_attr("guest_lan_rule", 0, "prio", 
			wlan_guest_priority); 
		//wlan_guest_max_bw
		ezplib_replace_attr("guest_lan_rule", 0, "bw", 
			wlan_guest_max_bw); 
	}	

	ezplib_get_attr_val("wl0_ssid_rule", 0, "ssid", 
		TempSSID, 128, EZPLIB_USE_CLI);
	
	ssid    = websGetVar(wp, T("ssid"),    T("")); 
	mssid_1 = websGetVar(wp, T("mssid_1"), T("")); 
	mssid_2 = websGetVar(wp, T("mssid_2"), T("")); 
	mssid_3 = websGetVar(wp, T("mssid_3"), T(""));
	mssid_4 = websGetVar(wp, T("mssid_4"), T(""));
	mssid_5 = websGetVar(wp, T("mssid_5"), T("")); 
	mssid_6 = websGetVar(wp, T("mssid_6"), T("")); 
	mssid_7 = websGetVar(wp, T("mssid_7"), T("")); 	 
	
	/*Enable, Hide, Enable Intra-BSS Traffic*/
//	enablessid  = websGetVar(wp, T("enablessid"),  T(""));
	enablessid1 = websGetVar(wp, T("enablessid1"), T(""));
	enablessid2 = websGetVar(wp, T("enablessid2"), T(""));
	enablessid3 = websGetVar(wp, T("enablessid3"), T(""));
	enablessid4 = websGetVar(wp, T("enablessid4"), T(""));
	enablessid5 = websGetVar(wp, T("enablessid5"), T(""));
	enablessid6 = websGetVar(wp, T("enablessid6"), T(""));
	enablessid7 = websGetVar(wp, T("enablessid7"), T(""));
	hidessid    = websGetVar(wp, T("hidessid"),    T("")); 
	hidemssid_1 = websGetVar(wp, T("hidemssid_1"), T("")); 
	hidemssid_2 = websGetVar(wp, T("hidemssid_2"), T("")); 
	hidemssid_3 = websGetVar(wp, T("hidemssid_3"), T(""));
	hidemssid_4 = websGetVar(wp, T("hidemssid_4"), T("")); 
	hidemssid_5 = websGetVar(wp, T("hidemssid_5"), T("")); 
	hidemssid_6 = websGetVar(wp, T("hidemssid_6"), T("")); 
	hidemssid_7 = websGetVar(wp, T("hidemssid_7"), T("")); 	
	intra_bss  = websGetVar(wp, T("IntraBSS"),  T("")); 
	intra_bss1 = websGetVar(wp, T("IntraBSS1"), T("")); 
	intra_bss2 = websGetVar(wp, T("IntraBSS2"), T("")); 
	intra_bss3 = websGetVar(wp, T("IntraBSS3"), T(""));
	intra_bss4 = websGetVar(wp, T("IntraBSS4"), T("")); 
	intra_bss5 = websGetVar(wp, T("IntraBSS5"), T(""));
	intra_bss6 = websGetVar(wp, T("IntraBSS6"), T("")); 
	intra_bss7 = websGetVar(wp, T("IntraBSS7"), T(""));

	/*SSID count*/
	bssid_num = websGetVar(wp, T("bssid_num"), T("1"));
	new_bssid_num = atoi(bssid_num);	
	strcpy(TempBuf, nvram_get("wlv_rule_max"));
	if (new_bssid_num < 1 || new_bssid_num > atoi(TempBuf)) {
		nvram_commit();
		websError(wp, 403, T("'bssid_num' %s is out of range!"), bssid_num);
		return;
	}

	/* SSID */
	if (0 == strlen(ssid)){
		nvram_commit();
		websError(wp, 403, T("'SSID' should not be empty!"));
		return;
	}

	/* SSID */
	if (0 != strlen(ssid)) {
		ezplib_replace_attr("wl0_ssid_rule", 0, "ssid", ssid);
		//enable ssid
		// AXIM/Holmas say: First SSID must be enable.
		ezplib_replace_attr("wl0_basic_rule", 0, "enable", "1");	
		// Hide SSID	
		if (!strncmp(hidessid, "1", 2)){
			ezplib_replace_attr("wl0_basic_rule", 0, "hidden", "1");
		}else{
			ezplib_replace_attr("wl0_basic_rule", 0, "hidden", "0");
		}			
		// Intra-BSS
		if (!strncmp(intra_bss, "0", 2)){
			ezplib_replace_attr("wl0_basic_rule", 0, "isolation", "1");
		}else{
			ezplib_replace_attr("wl0_basic_rule", 0, "isolation", "0");
		}
	}	

	/* SSID 1 */	
	ezplib_replace_attr("wl0_ssid_rule", 1, "ssid", mssid_1);
	//enable ssid1
	if (!strncmp(enablessid1, "1", 2)){
		ezplib_replace_attr("wl0_basic_rule", 1, "enable", "1");	
	}else{
		ezplib_replace_attr("wl0_basic_rule", 1, "enable", "0");	
	}
	// Hide SSID
	if (!strncmp(hidemssid_1, "1", 2)){
		ezplib_replace_attr("wl0_basic_rule", 1, "hidden", "1");
	}else{
		ezplib_replace_attr("wl0_basic_rule", 1, "hidden", "0");
	}
	// Intra-BSS
	if (!strncmp(intra_bss1, "0", 2)){
		ezplib_replace_attr("wl0_basic_rule", 1, "isolation", "1");	
	}else{
		ezplib_replace_attr("wl0_basic_rule", 1, "isolation", "0");	
	}

	/* SSID 2 */	
	ezplib_replace_attr("wl0_ssid_rule", 2, "ssid", mssid_2);

	//enable ssid2
	if (!strncmp(enablessid2, "1", 2)){
		ezplib_replace_attr("wl0_basic_rule", 2, "enable", "1");
	}else{
		ezplib_replace_attr("wl0_basic_rule", 2, "enable", "0");
	}
	// Hide SSID
	if (!strncmp(hidemssid_2, "1", 2)){
		ezplib_replace_attr("wl0_basic_rule", 2, "hidden", "1");
	}else{
		ezplib_replace_attr("wl0_basic_rule", 2, "hidden", "0");
	}
	// Intra-BSS
	if (!strncmp(intra_bss2, "0", 2)){
		ezplib_replace_attr("wl0_basic_rule", 2, "isolation", "1");
	}else{
		ezplib_replace_attr("wl0_basic_rule", 2, "isolation", "0");
	}		

	/* SSID 3 */	
	ezplib_replace_attr("wl0_ssid_rule", 3, "ssid", mssid_3);
	//enable ssid3
	if (!strncmp(enablessid3, "1", 2)){
		ezplib_replace_attr("wl0_basic_rule", 3, "enable", "1");
	}else{
		ezplib_replace_attr("wl0_basic_rule", 3, "enable", "0");
	}
	// Hide SSID
	if (!strncmp(hidemssid_3, "1", 2)){
		ezplib_replace_attr("wl0_basic_rule", 3, "hidden", "1");
	}else{
		ezplib_replace_attr("wl0_basic_rule", 3, "hidden", "0");
	}
	// Intra-BSS
	if (!strncmp(intra_bss3, "0", 2)){
		ezplib_replace_attr("wl0_basic_rule", 3, "isolation", "1");
	}else{
		ezplib_replace_attr("wl0_basic_rule", 3, "isolation", "0");
	}		

	/* SSID 4 */	
	ezplib_replace_attr("wl0_ssid_rule", 4, "ssid", mssid_4);
	//enable ssid4
	if (!strncmp(enablessid4, "1", 2)){
		ezplib_replace_attr("wl0_basic_rule", 4, "enable", "1");
	}else{
		ezplib_replace_attr("wl0_basic_rule", 4, "enable", "0");
	}
	// Hide SSID
	if (!strncmp(hidemssid_4, "1", 2)){
		ezplib_replace_attr("wl0_basic_rule", 4, "hidden", "1");
	}else{
		ezplib_replace_attr("wl0_basic_rule", 4, "hidden", "0");
	}
	// Intra-BSS
	if (!strncmp(intra_bss4, "0", 2)){
		ezplib_replace_attr("wl0_basic_rule", 4, "isolation", "1");
	}else{
		ezplib_replace_attr("wl0_basic_rule", 4, "isolation", "0");
	}		

	/* SSID 5 */	
	ezplib_replace_attr("wl0_ssid_rule", 5, "ssid", mssid_5);
	//enable ssid5
	if (!strncmp(enablessid5, "1", 2)){
		ezplib_replace_attr("wl0_basic_rule", 5, "enable", "1");
	}else{
		ezplib_replace_attr("wl0_basic_rule", 5, "enable", "0");
	}
	// Hide SSID
	if (!strncmp(hidemssid_5, "1", 2)){
		ezplib_replace_attr("wl0_basic_rule", 5, "hidden", "1");
	}else{
		ezplib_replace_attr("wl0_basic_rule", 5, "hidden", "0");
	}
	// Intra-BSS
	if (!strncmp(intra_bss5, "0", 2)){
		ezplib_replace_attr("wl0_basic_rule", 5, "isolation", "1");
	}else{
		ezplib_replace_attr("wl0_basic_rule", 5, "isolation", "0");
	}		

	/* SSID 6 */	
	ezplib_replace_attr("wl0_ssid_rule", 6, "ssid", mssid_6);
	//enable ssid6
	if (!strncmp(enablessid6, "1", 2)){
		ezplib_replace_attr("wl0_basic_rule", 6, "enable", "1");
	}else{
		ezplib_replace_attr("wl0_basic_rule", 6, "enable", "0");
	}
	// Hide SSID
	if (!strncmp(hidemssid_6, "1", 2)){
		ezplib_replace_attr("wl0_basic_rule", 6, "hidden", "1");
	}else{
		ezplib_replace_attr("wl0_basic_rule", 6, "hidden", "0");
	}
	// Intra-BSS
	if (!strncmp(intra_bss6, "0", 2)){
		ezplib_replace_attr("wl0_basic_rule", 6, "isolation", "1");
	}else{
		ezplib_replace_attr("wl0_basic_rule", 6, "isolation", "0");
	}		

	/* SSID 7 */	
	ezplib_replace_attr("wl0_ssid_rule", 7, "ssid", mssid_7);
	//enable ssid7
	if (!strncmp(enablessid7, "1", 2)){
		ezplib_replace_attr("wl0_basic_rule", 7, "enable", "1");
	}else{
		ezplib_replace_attr("wl0_basic_rule", 7, "enable", "0");
	}
	// Hide SSID
	if (!strncmp(hidemssid_7, "1", 2)){
		ezplib_replace_attr("wl0_basic_rule", 7, "hidden", "1");
	}else{
		ezplib_replace_attr("wl0_basic_rule", 7, "hidden", "0");
	}
	// Intra-BSS
	if (!strncmp(intra_bss7, "0", 2)){
		ezplib_replace_attr("wl0_basic_rule", 7, "isolation", "1");
	}else{
		ezplib_replace_attr("wl0_basic_rule", 7, "isolation", "0");
	}		

	/* op mode : normal/ap/ur/wisp/wisp_ur*/
	char ModeTmpBuf[32];
	ezplib_get_attr_val("wl_mode_rule", 0, "mode", ModeTmpBuf, 32, EZPLIB_USE_CLI);

	/* Network Mode 11bgn / 11an */
	wirelessmode = websGetVar(wp, T("wirelessmode"), T("0"));
	if (!strcmp(ModeTmpBuf, "ap")){
		ezplib_replace_attr("wl_ap_basic_rule", 0, "net_mode", wirelessmode);
	} else {
		ezplib_replace_attr("wl_basic_rule", 0, "net_mode", wirelessmode);
	}
	
	/* Channel Bandwidth HT20 / HT20_40*/
	n_bandwidth = websGetVar(wp, T("n_bandwidth"), T("0"));	
	if (!strcmp(ModeTmpBuf, "ap")){
		ezplib_replace_attr("wl_ap_advanced_rule", 0, "htbw", n_bandwidth); 
	} else {
		ezplib_replace_attr("wl_advanced_rule", 0, "htbw", n_bandwidth); 
	}
	
	/* Channel Selection 1~11, 0 stands for auto*/
	sz11gChannel = websGetVar(wp, T("sz11gChannel"), T(""));		
	auto_channel = websGetVar(wp, T("Auto_Channel"), T(""));
	if (!strncmp(auto_channel, "1", 2))
	sz11gChannel = "0";
	nvram_set("AP_Channel", sz11gChannel);
	if (!strcmp(ModeTmpBuf, "ap")){
		ezplib_replace_attr("wl_ap_basic_rule", 0, "channel", sz11gChannel);
	} else {
		ezplib_replace_attr("wl_basic_rule", 0, "channel", sz11gChannel);
	}

	/* Extension Channel */
	n_extcha = websGetVar(wp, T("n_extcha"), T("0"));
	char extcha_str[4] = {};
	if(	!strcmp(n_extcha, "-1")) {
		sprintf(extcha_str, "%d", atoi(sz11gChannel) - 4 );
	}
	else if( !strcmp(n_extcha, "1")) {
		sprintf(extcha_str, "%d", atoi(sz11gChannel) + 4 );
	}

	if (strcmp(n_extcha, "0") && !strcmp(ModeTmpBuf, "ap")){
		ezplib_replace_attr("wl_ap_advanced_rule", 0, "extcha", extcha_str);
	} else {
		ezplib_replace_attr("wl_advanced_rule", 0, "extcha", extcha_str);
	}

	/*Communicate between wireless clients with diff SSIDs*/
	main_intra_bss = websGetVar(wp, T("MainIntraBSS"), T("0"));
	if (!strcmp(ModeTmpBuf, "ap")){
		ezplib_replace_attr("wl_ap_basic_rule", 0, "bisolation", main_intra_bss);		
	}else {
		ezplib_replace_attr("wl_basic_rule", 0, "bisolation", main_intra_bss);		
	}

		

//	default_shown_mbssid[nvram] = mbssid;

//# WPS
	{
		if (strcmp(TempSSID, ssid)){
			//char *wordlist= nvram_bufget(RT2860_NVRAM, "WscModeOption");
			ezplib_get_attr_val("wl_wps_rule", 0, "configured", wordlist, sizeof(wordlist), EZPLIB_USE_CLI);
			if(wordlist){
				if (strcmp(wordlist, "0"))
				ezplib_replace_attr("wl_wps_rule", 0, "configured" ,"1");
				g_wsc_configured = 1;
			}
		}
	}
//# WPS


	// AXIMCOM/Holmas: WLAN Guest need to execute this script
	ezplib_get_attr_val("wl_mode_rule", 0, "mode", TempBuf, 32, EZPLIB_USE_CLI);
	if (!strcmp(TempBuf, "normal")){ // Router mode
		/**script invoke**/
	}
	
	/* Configuration Done */
	nvram_commit();

	// AXIMCOM/Holmas: need to reboot
	// Router mode
	if ( (!strcmp(TempBuf, "normal")) && (!strcmp(wlan_guest, "1"))){ 
		//Steve add 2011/06/30
		if(GuestNoChange==1){
			setWebMessage(0, NULL);
			websRedirect(wp, "local/advance/wlan.asp");
		}else{
        	system("sleep 3 && reboot &");
			websHeader(wp);

			websWrite(wp, T("<script>\n"));	
			websWrite(wp, T("function waitingpage(){\n"));
			websWrite(wp, T("top.location.href = '/local/advance/proceeding.asp';\n"));
			websWrite(wp, T("}\n"));
			websWrite(wp, T("waitingpage();\n"));
			websWrite(wp, T("</script>\n"));
			websFooter(wp);
			websDone(wp, 200);
		}
	}else{
	
		if((!strcmp(wlan_guest, "0")) && (!strcmp(OldBuf_en, "1"))){
			system("sleep 3 && reboot &");
			websHeader(wp);

			websWrite(wp, T("<script>\n"));	
			websWrite(wp, T("function waitingpage(){\n"));
			websWrite(wp, T("top.location.href = '/local/advance/proceeding.asp';\n"));
			websWrite(wp, T("}\n"));
			websWrite(wp, T("waitingpage();\n"));
			websWrite(wp, T("</script>\n"));
			websFooter(wp);
			websDone(wp, 200);
		}else{
			setWebMessage(0, NULL);
			websRedirect(wp, "local/advance/wlan.asp");
		}
	}

#endif
}

//void APGeneral5G(int nvram, webs_t wp, char_t *path, char_t *query) //kissth add 2011-11-17
void APGeneral5G(webs_t wp, char_t *path, char_t *query)
{
#if 1

	char_t *ssid,*mssid_1, *mssid_2, *mssid_3, *mssid_4, *mssid_5, *mssid_6,*mssid_7,*bssid_num, *set_bssid_num;
	char_t *hidessid, *hidemssid_1, *hidemssid_2, *hidemssid_3, *hidemssid_4, *hidemssid_5, *hidemssid_6,*hidemssid_7;
	char_t *main_intra_bss, *intra_bss, *intra_bss1, *intra_bss2, *intra_bss3, *intra_bss4, *intra_bss5, *intra_bss6,*intra_bss7;
	char_t *enablessid, *enablessid1, *enablessid2, *enablessid3;
	char_t *sz11gChannel;
	char_t	*auto_channel;	
	char_t *bssid_num1, *bssid_num2, *bssid_num3;
	
	int mbssid, mbssid_num, i, ssid_num;

	int new_bssid_num, old_bssid_num;
		
	char 	*hidessid_str = malloc(sizeof(char)) , *hidessid_str1 = malloc(sizeof(char)),*hidessid_str2 = malloc(sizeof(char)),*hidessid_str3 = malloc(sizeof(char));
	char 	*intra_bss_str = malloc(sizeof(char)),*intra_bss_str1 = malloc(sizeof(char)),*intra_bss_str2 = malloc(sizeof(char)),*intra_bss_str3 = malloc(sizeof(char)), *intra_bss_str_main = malloc(sizeof(char));
	char 	*enablessid_str = malloc(sizeof(char)) , *enablessid_str1 = malloc(sizeof(char)),*enablessid_str2 = malloc(sizeof(char)),*enablessid_str3 = malloc(sizeof(char));
	char wordlist[5];
	char TempSSID[128];
	
// Tommy: Implement WLAN Guest function
#if 1
	char TempBuf[32];
	char *wlan_guest = websGetVar(wp, T("wlanguest"), T("0"));
	//Steve add 2011/06/30
	char OldBuf_en[32];
	char OldBuf_ip[32];
	char OldBuf_mask[32];
	char OldBuf_bm_en[32];
	char OldBuf_bm_val[32];
	int GuestNoChange=0;
	
	//Steve add 2011/06/30
	ezplib_get_attr_val("guest1_lan_rule", 0, "enable", OldBuf_en, 32, EZPLIB_USE_CLI);
	
	ezplib_replace_attr("guest1_lan_rule", 0, "enable", wlan_guest); // Need to implement 
		
	ezplib_get_attr_val("wl1_mode_rule", 0, "mode", TempBuf, 32, EZPLIB_USE_CLI);
	if ( (!strcmp(TempBuf, "normal")) && (!strcmp(wlan_guest, "1"))){ // Router mode
		char *wlan_guest_ip = websGetVar(wp, T("wlanguestip"), T(""));
		char *wlan_guest_mask = websGetVar(wp, T("wlanguestmask"), T(""));
		char *wlan_guestBW_enable = websGetVar(wp, T("wlanguestBW"), T("0"));
		char *wlan_guest_priority = websGetVar(wp, T("guest_priority"), T("1"));
		char *wlan_guest_max_bw = websGetVar(wp, T("guestmaxbw"), T(""));
		char *mask;
		
		//Steve add 2011/06/30
		mask = mask2prefix2(wlan_guest_mask);
		ezplib_get_attr_val("guest1_lan_rule", 0, "ipaddr", OldBuf_ip, 32, EZPLIB_USE_CLI);
		ezplib_get_attr_val("guest1_lan_rule", 0, "netmask", OldBuf_mask, 32, EZPLIB_USE_CLI);
		ezplib_get_attr_val("guest1_lan_rule", 0, "bwen", OldBuf_bm_en, 32, EZPLIB_USE_CLI);
		ezplib_get_attr_val("guest1_lan_rule", 0, "bw", OldBuf_bm_val, 32, EZPLIB_USE_CLI);
		printf("OldBuf_5g_ip=%s\n",OldBuf_ip);
		printf("OldBuf_5g_mask=%s\n",OldBuf_mask);
		printf("OldBuf_bm_en=%s\n",OldBuf_bm_en);
		printf("OldBuf_bm_val=%s\n",OldBuf_bm_val);
		if((!strcmp(OldBuf_en, wlan_guest)) && (!strcmp(OldBuf_ip, wlan_guest_ip)) && (!strcmp(OldBuf_mask, mask)) && (!strcmp(OldBuf_bm_en, wlan_guestBW_enable)) && (!strcmp(OldBuf_bm_val, wlan_guest_max_bw))){
			printf("GuestNoChange=1");
			GuestNoChange=1;
		}else{
			printf("GuestNoChange=0");
			GuestNoChange=0;
		}
		
		//wlan_guest_ip
		ezplib_replace_attr("guest1_lan_rule", 0, "ipaddr", wlan_guest_ip); // Need to implement 
		
		//wlan_guest_mask
		//Translate AboCom's netmask to AXIM's nvram format
    	mask = mask2prefix2(wlan_guest_mask);
		ezplib_replace_attr("guest1_lan_rule", 0, "netmask", mask); // Need to implement 
		
		//wlan_guestBW_enable
		ezplib_replace_attr("guest1_lan_rule", 0, "bwen", wlan_guestBW_enable); // Need to implement 
		
		//wlan_guest_priority
		ezplib_replace_attr("guest1_lan_rule", 0, "prio", wlan_guest_priority); // Need to implement 
		
		//wlan_guest_max_bw
		ezplib_replace_attr("guest1_lan_rule", 0, "bw", wlan_guest_max_bw); // Need to implement 
	}	
#endif // Tommy: Implement WLAN Guest function

	ezplib_get_attr_val("wl1_ssid_rule", 0, "ssid", TempSSID, 128, EZPLIB_USE_CLI);
	
	ssid = websGetVar(wp, T("ssid"), T("")); 
	mssid_1 = websGetVar(wp, T("mssid_1"), T("")); 
	mssid_2 = websGetVar(wp, T("mssid_2"), T("")); 
	mssid_3 = websGetVar(wp, T("mssid_3"), T(""));
	mssid_4 = websGetVar(wp, T("mssid_4"), T(""));
	mssid_5 = websGetVar(wp, T("mssid_5"), T("")); 
	mssid_6 = websGetVar(wp, T("mssid_6"), T("")); 
	mssid_7 = websGetVar(wp, T("mssid_7"), T("")); 	 

	bssid_num = websGetVar(wp, T("bssid_num"), T("1"));

	enablessid = websGetVar(wp, T("enablessid"), T(""));
	enablessid1 = websGetVar(wp, T("enablessid1"), T(""));
	enablessid2 = websGetVar(wp, T("enablessid2"), T(""));
	enablessid3 = websGetVar(wp, T("enablessid3"), T(""));

	hidessid = websGetVar(wp, T("hidessid"), T("")); 
	hidemssid_1 = websGetVar(wp, T("hidemssid_1"), T("")); 
	hidemssid_2 = websGetVar(wp, T("hidemssid_2"), T("")); 
	hidemssid_3 = websGetVar(wp, T("hidemssid_3"), T(""));
	hidemssid_4 = websGetVar(wp, T("hidemssid_4"), T("")); 
	hidemssid_5 = websGetVar(wp, T("hidemssid_5"), T("")); 
	hidemssid_6 = websGetVar(wp, T("hidemssid_6"), T("")); 
	hidemssid_7 = websGetVar(wp, T("hidemssid_7"), T("")); 	

// Intra-BSS
	intra_bss = websGetVar(wp, T("IntraBSS"), T("")); 
	intra_bss1 = websGetVar(wp, T("IntraBSS1"), T("")); 
	intra_bss2 = websGetVar(wp, T("IntraBSS2"), T("")); 
	intra_bss3 = websGetVar(wp, T("IntraBSS3"), T(""));
	intra_bss4 = websGetVar(wp, T("IntraBSS4"), T("")); 
	intra_bss5 = websGetVar(wp, T("IntraBSS5"), T(""));
	intra_bss6 = websGetVar(wp, T("IntraBSS6"), T("")); 
	intra_bss7 = websGetVar(wp, T("IntraBSS7"), T(""));
//bisolation
	main_intra_bss = websGetVar(wp, T("MainIntraBSS"), T(""));
	
//number of ssid	
	bssid_num1 = websGetVar(wp, T("bssid_num1"), T("")); 
	bssid_num2 = websGetVar(wp, T("bssid_num2"), T(""));	
	bssid_num3 = websGetVar(wp, T("bssid_num3"), T(""));	

	new_bssid_num = atoi(bssid_num);	

	//SSID, 
	if (0 == strlen(ssid)){
		nvram_commit();
		websError(wp, 403, T("'SSID' should not be empty!"));
		return;
	}
	
	if (0 != strlen(ssid)) {
	ezplib_replace_attr("wl1_ssid_rule", 0, "ssid", ssid);

	//enable ssid
		sprintf(enablessid_str, "%s", "1");

	// Hide SSID	
	if (!strncmp(hidessid, "1", 2)){
		sprintf(hidessid_str, "%s", "1");
	}else{
		sprintf(hidessid_str, "%s", "0");
	}			
	// Intra-BSS
	if (!strncmp(intra_bss, "0", 2)){
		sprintf(intra_bss_str, "%s", "0");		
	}else{
		sprintf(intra_bss_str, "%s", "1");
	}
	
	ezplib_replace_attr("wl1_basic_rule", 0, "enable", enablessid_str);	
	ezplib_replace_attr("wl1_basic_rule", 0, "hidden", hidessid_str);
	ezplib_replace_attr("wl1_basic_rule", 0, "isolation", intra_bss_str);

	}	

	ezplib_replace_attr("wl1_ssid_rule", 1, "ssid", mssid_1);

		//enable ssid1
		if (!strncmp(enablessid1, "1", 2)){
			sprintf(enablessid_str1, "%s", "1");
		}else{
			sprintf(enablessid_str1, "%s", "0");
		}
		// Hide SSID
		if (!strncmp(hidemssid_1, "1", 2)){
			sprintf(hidessid_str1, "%s", "1");
		}else{
			sprintf(hidessid_str1, "%s", "0");
		}
		// Intra-BSS
		if (!strncmp(intra_bss1, "0", 2)){
			sprintf(intra_bss_str1, "%s", "0");		
		}else{
			sprintf(intra_bss_str1, "%s", "1");
		}
	
	ezplib_replace_attr("wl1_basic_rule", 1, "enable", enablessid_str1);	
	ezplib_replace_attr("wl1_basic_rule", 1, "hidden", hidessid_str1);
	ezplib_replace_attr("wl1_basic_rule", 1, "isolation", intra_bss_str1);	

	
	ezplib_replace_attr("wl1_ssid_rule", 2, "ssid", mssid_2);
			
		//enable ssid2
		if (!strncmp(enablessid2, "1", 2)){
			sprintf(enablessid_str2, "%s", "1");
		}else{
			sprintf(enablessid_str2, "%s", "0");
		}
		// Hide SSID
		if (!strncmp(hidemssid_2, "1", 2)){
			sprintf(hidessid_str2, "%s", "1");
		}else{
			sprintf(hidessid_str2, "%s", "0");
		}
		// Intra-BSS
		if (!strncmp(intra_bss2, "0", 2)){
			sprintf(intra_bss_str2, "%s", "0");		
		}else{
			sprintf(intra_bss_str2, "%s", "1");
		}		

	ezplib_replace_attr("wl1_basic_rule", 2, "enable", enablessid_str2);
	ezplib_replace_attr("wl1_basic_rule", 2, "hidden", hidessid_str2);
	ezplib_replace_attr("wl1_basic_rule", 2, "isolation", intra_bss_str2);


	ezplib_replace_attr("wl1_ssid_rule", 3, "ssid", mssid_3);
				
		//enable ssid3
		if (!strncmp(enablessid3, "1", 2)){
			sprintf(enablessid_str3, "%s", "1");
		}else{
			sprintf(enablessid_str3, "%s", "0");
		}
		// hide ssid
		if (!strncmp(hidemssid_3, "1", 2)){
			sprintf(hidessid_str3, "%s", "1");
		}else{
			sprintf(hidessid_str3, "%s", "0");
		}
		// Intra-BSS
		if (!strncmp(intra_bss3, "0", 2)){
			sprintf(intra_bss_str3, "%s", "0");		
		}else{
			sprintf(intra_bss_str3, "%s", "1");
		}		

	ezplib_replace_attr("wl1_basic_rule", 3, "enable", enablessid_str3);
	ezplib_replace_attr("wl1_basic_rule", 3, "hidden", hidessid_str3);
	ezplib_replace_attr("wl1_basic_rule", 3, "isolation", intra_bss_str3);

		// Main Intra-BSS
		if (!strncmp(main_intra_bss, "0", 2)){
			sprintf(intra_bss_str_main, "%s", "0");		
		}else{
			sprintf(intra_bss_str_main, "%s", "1");
		}	
	ezplib_replace_attr("wl5g_basic_rule", 0, "bisolation", intra_bss_str_main);		

	nvram_set("wlv_rule_max", bssid_num);
							
	if (new_bssid_num < 1 || new_bssid_num > 4) {
		nvram_commit();
		websError(wp, 403, T("'bssid_num' %s is out of range!"), bssid_num);
		return;
	}

	if((!strcmp(bssid_num1, "0")) && (!strcmp(bssid_num2, "0")) && (!strcmp(bssid_num3, "1"))){

		replace_ssid_value(1 , 3);
				
	}
	if((!strcmp(bssid_num1, "0")) && (!strcmp(bssid_num2, "1")) && (!strcmp(bssid_num3, "0"))){

		replace_ssid_value(1 , 2);
		replace_blankssid_value(3);				
	}
	if((!strcmp(bssid_num1, "0")) && (!strcmp(bssid_num2, "1")) && (!strcmp(bssid_num3, "1"))){

		replace_ssid_value(1 , 2);
		replace_ssid_value(2 , 3);
				
	}		
	if((!strcmp(bssid_num2, "0")) && (!strcmp(bssid_num3, "1"))){

		replace_ssid_value(2 , 3);
				
	}
	if((!strcmp(bssid_num1, "0")) && (!strcmp(bssid_num2, "0")) && (!strcmp(bssid_num3, "0"))){

		replace_blankssid_value(1);
		replace_blankssid_value(2);
		replace_blankssid_value(3);
				
	}
	if((!strcmp(bssid_num1, "1")) && (!strcmp(bssid_num2, "0")) && (!strcmp(bssid_num3, "0"))){

		replace_blankssid_value(2);
		replace_blankssid_value(3);
				
	}
	if((!strcmp(bssid_num1, "1")) && (!strcmp(bssid_num2, "1")) && (!strcmp(bssid_num3, "0"))){

		replace_blankssid_value(3);
				
	}	
	sz11gChannel = websGetVar(wp, T("sz11gChannel"), T(""));		
	auto_channel = websGetVar(wp, T("Auto_Channel"), T(""));

        if (!strncmp(auto_channel, "1", 2)){
		sz11gChannel = "0";
	}
	
	nvram_set("AP1_Channel", sz11gChannel);
	ezplib_replace_attr("wl5g_basic_rule", 0, "channel", sz11gChannel);
		
	{
		if (strcmp(TempSSID, ssid)){
			ezplib_get_attr_val("wl1_wps_rule", 0, "configured", wordlist, sizeof(wordlist), EZPLIB_USE_CLI);
			if(wordlist){
				if (strcmp(wordlist, "0"))
					//doSystem("iwpriv ra0 set WscConfStatus=1");
					system("/sbin/ezp-wps-set 0 1 1 1");
				ezplib_replace_attr("wl1_wps_rule", 0, "configured" ,"1");
				g1_wsc_configured = 1;
			}
		}
	}

// AXIMCOM: It should execute, if extension channel have problem.
#if 0
	// Reset Extension Channel to 0
	ezplib_replace_attr("wl_advanced_rule", 0, "extcha", "0");

	system("/sbin/ezp-wl1-ctrl advance");	
#endif

	system("/sbin/ezp-wl1-ctrl basic");
	//doSystem("iwpriv ra0 set WscConfStatus=2");
	system("/sbin/ezp-wps-set 0 1 1 2");
// AXIMCOM/Holmas: WLAN Guest need to execute this script
	ezplib_get_attr_val("wl1_mode_rule", 0, "mode", TempBuf, 32, EZPLIB_USE_CLI);
	if (!strcmp(TempBuf, "normal")){ // Router mode
		system("/sbin/ezp-guest-lan");
	}
	
	nvram_commit();

	free(enablessid_str);
	free(enablessid_str1);
	free(enablessid_str2);	
	free(enablessid_str3);
	free(hidessid_str);
	free(hidessid_str1);
	free(hidessid_str2);	
	free(hidessid_str3);
	free(intra_bss_str);	
	free(intra_bss_str1);
	free(intra_bss_str2);
	free(intra_bss_str3);
	free(intra_bss_str_main);

	if ( (!strcmp(TempBuf, "normal")) && (!strcmp(wlan_guest, "1"))){ // Router mode
		//Steve add 2011/06/30
		if(GuestNoChange==1){
			setWebMessage(0, NULL);
			websRedirect(wp, "local/advance/wlan_5g.asp");
		}else{
        	system("sleep 3 && reboot &");
			websHeader(wp);

			websWrite(wp, T("<script>\n"));	
			websWrite(wp, T("function waitingpage(){\n"));
			websWrite(wp, T("top.location.href = '/local/advance/proceeding.asp';\n"));
			websWrite(wp, T("}\n"));
			websWrite(wp, T("waitingpage();\n"));
			websWrite(wp, T("</script>\n"));
			websFooter(wp);
			websDone(wp, 200);
		}
	}else{
	
		if((!strcmp(wlan_guest, "0")) && (!strcmp(OldBuf_en, "1"))){
			system("sleep 3 && reboot &");
			websHeader(wp);

			websWrite(wp, T("<script>\n"));	
			websWrite(wp, T("function waitingpage(){\n"));
			websWrite(wp, T("top.location.href = '/local/advance/proceeding.asp';\n"));
			websWrite(wp, T("}\n"));
			websWrite(wp, T("waitingpage();\n"));
			websWrite(wp, T("</script>\n"));
			websFooter(wp);
			websDone(wp, 200);
		}else{
			setWebMessage(0, NULL);
			websRedirect(wp, "local/advance/wlan_5g.asp");
		}
	}

#endif
}
/*kissth add for disable wps 
 * when user select WEP WPA-PSk security mode
 */
 void disable_wps(){
    char wsc_enable[6];
	
    ezplib_get_attr_val("wl_wps_rule", 0, "mode", wsc_enable, 6, EZPLIB_USE_CLI);
    if (!strcmp(wsc_enable,"7")){
	ezplib_replace_attr("wl_wps_rule", 0, "mode", "0");
	system("route delete 239.255.255.250 1>/dev/null 2>&1");
       system("kill -9 `cat /var/run/wscd.pid.rai0`");		
	//system("iwpriv rai0 set WscConfMode=0 1>/dev/null 2>&1");
	system("/sbin/ezp-wps-set 0 0 0 0");
    }
}
//void wifiSecurity(int nvram, webs_t wp, char_t *path, char_t *query)
void wifiSecurity( webs_t wp, char_t *path, char_t *query)
{

	char_t *ssid,*mssid_1, *mssid_2, *mssid_3, *mssid_4, *mssid_5, *mssid_6,*mssid_7,*bssid_num;
	char_t *hidessid, *hidemssid_1, *hidemssid_2, *hidemssid_3, *hidemssid_4, *hidemssid_5, *hidemssid_6,*hidemssid_7;
	char_t *sz11gChannel;
	char_t	*auto_channel;	
	//char_t *SSID;
	int mbssid, mbssid_num, i;
	int new_bssid_num, old_bssid_num = 1;
	char_t *security_mode;
	char 	hidessid_str[17];
	char wordlist[5];
	char TempSSID[128];

	ezplib_get_attr_val("wl0_ssid_rule", 0, "ssid", TempSSID, 128, EZPLIB_USE_CLI);
	
// Tommy,Merge WLAN General and Security web page,2009/1/18 09:26

	LFW(ssid, ssidIndex);
	if(!gstrlen(ssid))
		return;

	mbssid = atoi(ssid);

	LFW(security_mode, security_mode);


	//clear Radius settings
//	clearRadiusSetting(nvram, mbssid);
	clearRadiusSetting( mbssid);

#if 1
	if( !strcmp(security_mode, "Disable")){			// !------------------       Disable Mode --------------

//		STFs(nvram, mbssid, "AuthMode", "OPEN");
		ezplib_replace_attr("wl0_sec_rule", mbssid, "secmode", "disabled");
				

	}else if( !strcmp(security_mode, "SHARED")){	// !------------------       Shared Mode ----------------

		char *auth_method;	
//		wificonfWEP(nvram, wp, mbssid);
		wificonfWEP(wp, mbssid);						
		LFW(auth_method, auth_method);
		
		if (!strcmp(auth_method, "0")){                 // WEPAUTO
//		    STFs(nvram, mbssid, "AuthMode", "WEPAUTO");
			ezplib_replace_attr("wl0_sec_wep_rule", mbssid, "encmode", "auto");		
		} else if (!strcmp(auth_method, "1")){
			ezplib_replace_attr("wl0_sec_wep_rule", mbssid, "encmode", "open");		
		} else{
//		    STFs(nvram, mbssid, "AuthMode", security_mode);
			ezplib_replace_attr("wl0_sec_wep_rule", mbssid, "encmode", "shared");		
		} 
	
//		STFs(nvram, mbssid, "EncrypType", "WEP");
		ezplib_replace_attr("wl0_sec_rule", mbssid, "secmode", "wep");
             disable_wps();

	}else if( !strcmp(security_mode, "WEPAUTO")){ // !-------- WEP Auto Mode ----------------
//		wificonfWEP(nvram, wp, mbssid);
		wificonfWEP(wp, mbssid);
		
//		STFs(nvram, mbssid, "AuthMode", security_mode);
		ezplib_replace_attr("wl0_sec_wep_rule", mbssid, "encmode", "auto");
				
//		STFs(nvram, mbssid, "EncrypType", "WEP");
		ezplib_replace_attr("wl0_sec_rule", mbssid, "secmode", "wep");

	}else if(	!strcmp(security_mode, "WPA") ||  
			!strcmp(security_mode, "WPA1WPA2") ){// !--------- WPA Enterprise Mode ----------------
//		conf8021x(nvram, wp, mbssid);							// !------------------		WPA1WPA2 mixed mode
		conf8021x( wp, mbssid);	
		
		//wificonfWPAGeneral(nvram, wp, mbssid,);		
		if (!strcmp(security_mode, "WPA"))
//			wificonfWPAGeneral(nvram, wp, mbssid,0);
			wificonfWPAGeneral(wp, mbssid,0);			
	
		if (!strcmp(security_mode, "WPA1WPA2"))
//			wificonfWPAGeneral(nvram, wp, mbssid,2);
			wificonfWPAGeneral(wp, mbssid,2);

//		STFs(nvram, mbssid, "AuthMode", security_mode);
		if(!strcmp(security_mode , "WPA")){
			strcpy(security_mode,"wpa");
		}	
		ezplib_replace_attr("wl0_sec_rule", mbssid, "secmode", security_mode);
              ezplib_replace_attr("wl0_sec_wpa_rule", mbssid, "crypto", "mixed");

//		STFs(nvram, mbssid, "IEEE8021X", "0");
	}else if( !strcmp(security_mode, "WPAPSK")){ // !------------------       WPA Personal Mode ----------------
		char *pass_phrase_str;

		//wificonfWPAGeneral(nvram, wp, mbssid);
//		wificonfWPAGeneral(nvram, wp, mbssid,0);
		wificonfWPAGeneral(wp, mbssid,0);		

		LFW(pass_phrase_str, passphrase);

//		STFs(nvram, mbssid, "AuthMode", security_mode);
		if(!strcmp(security_mode , "WPAPSK")){
			strcpy(security_mode,"psk");
		}
		ezplib_replace_attr("wl0_sec_rule", mbssid, "secmode", security_mode);
//		STFs(nvram, mbssid, "IEEE8021X", "0");
              ezplib_replace_attr("wl0_sec_wpa_rule", mbssid, "crypto", "mixed");
		ezplib_replace_attr("wl0_sec_wpa_rule", mbssid, "key", pass_phrase_str);
             disable_wps();		
//		}
	}else if( !strcmp(security_mode, "WPA2")){		// !------------------  WPA2 Enterprise Mode ----------------

		char *pass_phrase_str;
		char *PMKCachePeriod;
		char *PreAuth;
		char *wpa_compatible;

//		conf8021x(nvram, wp, mbssid);
		conf8021x(wp, mbssid);
		//wificonfWPAGeneral(nvram, wp, mbssid,0);
//		wificonfWPAGeneral(nvram, wp, mbssid,1);
		wificonfWPAGeneral(wp, mbssid,1);

		LFW(pass_phrase_str, passphrase);
		LFW(PMKCachePeriod, PMKCachePeriod);
		LFW(PreAuth, PreAuthentication);
		LFW(wpa_compatible,wpa_compatible);
                                               	
		if (!strcmp(wpa_compatible, "1")){ // WPA1WPA2        
//			STFs(nvram, mbssid, "AuthMode", "WPA1WPA2");			
		ezplib_replace_attr("wl0_sec_wpa2_rule", mbssid, "wpacap", wpa_compatible);		
//			wificonfWPAGeneral(nvram, wp, mbssid,2);	
		wificonfWPAGeneral(wp, mbssid,2);				
		}else{
//			STFs(nvram, mbssid, "AuthMode", security_mode);
		ezplib_replace_attr("wl0_sec_wpa2_rule", mbssid, "wpacap", wpa_compatible);

		}
		
		if(!strcmp(security_mode , "WPA2")){
			strcpy(security_mode,"wpa2");
		}
		ezplib_replace_attr("wl0_sec_rule", mbssid, "secmode", security_mode);	
		ezplib_replace_attr("wl0_sec_wpa2_rule", mbssid, "key", pass_phrase_str);
		
//		STF(nvram, mbssid, PMKCachePeriod);              
		ezplib_replace_attr("wl0_sec_wpa2_rule", mbssid, "pmkperiod", PMKCachePeriod);
		
//		STF(nvram, mbssid, PreAuth);		
		ezplib_replace_attr("wl0_sec_wpa2_rule", mbssid, "preauth", PreAuth);
              ezplib_replace_attr("wl0_sec_wpa2_rule", mbssid, "crypto", "mixed");

	
	}else if(!strcmp(security_mode, "WPA2PSK") ||	// !------------------  WPA2 Personal Mode ----------------
				!strcmp(security_mode, "WPAPSKWPA2PSK") ){ 	//! -------------   WPA PSK WPA2 PSK mixed

		char *pass_phrase_str;
		char *wpapsk_compatible;

		//wificonfWPAGeneral(nvram, wp, mbssid);
		if (!strcmp(security_mode, "WPA2PSK"))
//			wificonfWPAGeneral(nvram, wp, mbssid,1);
			wificonfWPAGeneral(wp, mbssid,1);
			
		if (!strcmp(security_mode, "WPAPSKWPA2PSK"))
//			wificonfWPAGeneral(nvram, wp, mbssid,2);
			wificonfWPAGeneral(wp, mbssid,2);					
		
		LFW(pass_phrase_str, passphrase);
		LFW(wpapsk_compatible,wpapsk_compatible);		

		if (!strcmp(wpapsk_compatible, "1")){ // WPAPSKWPA2PSK
//			STFs(nvram, mbssid, "AuthMode", "WPAPSKWPA2PSK");
			ezplib_replace_attr("wl0_sec_wpa2_rule", mbssid, "wpacap", wpapsk_compatible);		
//			wificonfWPAGeneral(nvram, wp, mbssid,2);
			wificonfWPAGeneral(wp, mbssid,2);
		}else{
//			STFs(nvram, mbssid, "AuthMode", security_mode);
			ezplib_replace_attr("wl0_sec_wpa2_rule", mbssid, "wpacap", wpapsk_compatible);
		}
	
		if(!strcmp(security_mode , "WPA2PSK")){
			strcpy(security_mode,"psk2");
		}
		ezplib_replace_attr("wl0_sec_rule", mbssid, "secmode", security_mode);
		ezplib_replace_attr("wl0_sec_wpa2_rule", mbssid, "key", pass_phrase_str);
              ezplib_replace_attr("wl0_sec_wpa2_rule", mbssid, "crypto", "mixed");

	
	}

	else{
		return;
	}
	

//# WPS
	{
		//if(nvram == RT2860_NVRAM && mbssid == 0){		// only ra0 supports WPS now.
		if (strcmp(TempSSID, ssid)){
			//char *wordlist= nvram_bufget(RT2860_NVRAM, "WscModeOption");
			ezplib_get_attr_val("wl_wps_rule", 0, "configured", wordlist, sizeof(wordlist), EZPLIB_USE_CLI);
			if(wordlist){
				if (strcmp(wordlist, "0"))
					//doSystem("iwpriv rai0 set WscConfStatus=1");
					system("/sbin/ezp-wps-set 0 0 1 1");
				//nvram_bufset(RT2860_NVRAM, "WscConfigured", "1");
				ezplib_replace_attr("wl_wps_rule", 0, "configured" ,"1");
				g_wsc_configured = 1;
			}
		}
	}
//# WPS

//	nvram_commit(nvram);
	system("/sbin/ezp-wl0-ctrl basic");
	//doSystem("iwpriv rai0 set WscConfStatus=2");
	system("/sbin/ezp-wps-set 0 0 1 2");
	nvram_commit();

	initInternet();

#endif

#if 1//Arthur Chow 2009-03-06		
		setWebMessage(0, NULL);
#endif				
	websRedirect(wp, "local/advance/security.asp");

}
/*kissth add for disable wps 5g
 * when user select WEP WPA-PSk security mode
 */
 void disable_wps_5g(){
    char wsc_enable[6];
	
    ezplib_get_attr_val("wl1_wps_rule", 0, "mode", wsc_enable, 6, EZPLIB_USE_CLI);
    if (!strcmp(wsc_enable,"7")){
	ezplib_replace_attr("wl1_wps_rule", 0, "mode", "0");
	system("route delete 239.255.255.250 1>/dev/null 2>&1");
       system("kill -9 `cat /var/run/wscd.pid.ra0`");		
	//system("iwpriv ra0 set WscConfMode=0 1>/dev/null 2>&1");
	system("/sbin/ezp-wps-set 0 1 0 0");
    }
}
//void wifi5GSecurity(int nvram, webs_t wp, char_t *path, char_t *query)
void wifi5GSecurity( webs_t wp, char_t *path, char_t *query)
{

	char_t *ssid,*mssid_1, *mssid_2, *mssid_3, *mssid_4, *mssid_5, *mssid_6,*mssid_7,*bssid_num;
	char_t *hidessid, *hidemssid_1, *hidemssid_2, *hidemssid_3, *hidemssid_4, *hidemssid_5, *hidemssid_6,*hidemssid_7;
	char_t *sz11gChannel;
	char_t	*auto_channel;	
	int mbssid, mbssid_num, i;
	int new_bssid_num, old_bssid_num = 1;
	char_t *security_mode;
	char 	hidessid_str[17];
	char wordlist[5];
	char TempSSID[128];

	ezplib_get_attr_val("wl1_ssid_rule", 0, "ssid", TempSSID, 128, EZPLIB_USE_CLI);
	
	LFW(ssid, ssidIndex);
	if(!gstrlen(ssid))
		return;

	mbssid = atoi(ssid);

	LFW(security_mode, security_mode);

	clear5GRadiusSetting( mbssid);

#if 1
	if( !strcmp(security_mode, "Disable")){			// !------------------       Disable Mode --------------

		ezplib_replace_attr("wl1_sec_rule", mbssid, "secmode", "disabled");

	}else if( !strcmp(security_mode, "SHARED")){	// !------------------       Shared Mode ----------------

		char *auth_method;	
		wifi5gconfWEP(wp, mbssid);						
		LFW(auth_method, auth_method);
		
		if (!strcmp(auth_method, "0")){                 // WEPAUTO
			ezplib_replace_attr("wl1_sec_wep_rule", mbssid, "encmode", "auto");		
		}else{
			ezplib_replace_attr("wl1_sec_wep_rule", mbssid, "encmode", "shared");		
		} 
		ezplib_replace_attr("wl1_sec_rule", mbssid, "secmode", "wep");
		disable_wps_5g();
	}else if( !strcmp(security_mode, "WEPAUTO")){ // !-------- WEP Auto Mode ----------------
		wifi5gconfWEP(wp, mbssid);
		ezplib_replace_attr("wl1_sec_wep_rule", mbssid, "encmode", "auto");
		ezplib_replace_attr("wl1_sec_rule", mbssid, "secmode", "wep");

	}else if(	!strcmp(security_mode, "WPA") ||  
			!strcmp(security_mode, "WPA1WPA2") ){// !--------- WPA Enterprise Mode ----------------
		conf5g8021x( wp, mbssid);	
		if (!strcmp(security_mode, "WPA"))
			wificonf5GWPAGeneral(wp, mbssid,0);			
		if (!strcmp(security_mode, "WPA1WPA2"))
			wificonf5GWPAGeneral(wp, mbssid,2);
		if(!strcmp(security_mode , "WPA")){
			strcpy(security_mode,"wpa");
		}	
		ezplib_replace_attr("wl1_sec_rule", mbssid, "secmode", security_mode);
	}else if( !strcmp(security_mode, "WPAPSK")){ // !------------------       WPA Personal Mode ----------------
		char *pass_phrase_str;

		wificonf5GWPAGeneral(wp, mbssid,0);		

		LFW(pass_phrase_str, passphrase);
		if(!strcmp(security_mode , "WPAPSK")){
			strcpy(security_mode,"psk");
		}
		ezplib_replace_attr("wl1_sec_rule", mbssid, "secmode", security_mode);
		ezplib_replace_attr("wl1_sec_wpa_rule", mbssid, "key", pass_phrase_str);
		disable_wps_5g();		
	}else if( !strcmp(security_mode, "WPA2")){		// !------------------  WPA2 Enterprise Mode ----------------

		char *pass_phrase_str;
		char *PMKCachePeriod;
		char *PreAuth;
		char *wpa_compatible;

		conf5g8021x(wp, mbssid);
		wificonf5GWPAGeneral(wp, mbssid,1);

		LFW(pass_phrase_str, passphrase);
		LFW(PMKCachePeriod, PMKCachePeriod);
		LFW(PreAuth, PreAuthentication);
		LFW(wpa_compatible,wpa_compatible);
                                               	
		if (!strcmp(wpa_compatible, "1")){ // WPA1WPA2        
		ezplib_replace_attr("wl1_sec_wpa2_rule", mbssid, "wpacap", wpa_compatible);		
		wificonf5GWPAGeneral(wp, mbssid,2);				
		}else{
		ezplib_replace_attr("wl1_sec_wpa2_rule", mbssid, "wpacap", wpa_compatible);
		}
		
		if(!strcmp(security_mode , "WPA2")){
			strcpy(security_mode,"wpa2");
		}
		ezplib_replace_attr("wl1_sec_rule", mbssid, "secmode", security_mode);	
		ezplib_replace_attr("wl1_sec_wpa2_rule", mbssid, "key", pass_phrase_str);
		ezplib_replace_attr("wl1_sec_wpa2_rule", mbssid, "pmkperiod", PMKCachePeriod);
		ezplib_replace_attr("wl1_sec_wpa2_rule", mbssid, "preauth", PreAuth);
	}else if(!strcmp(security_mode, "WPA2PSK") ||	// !------------------  WPA2 Personal Mode ----------------
				!strcmp(security_mode, "WPAPSKWPA2PSK") ){ 	//! -------------   WPA PSK WPA2 PSK mixed

		char *pass_phrase_str;
		char *wpapsk_compatible;

		if (!strcmp(security_mode, "WPA2PSK"))
			wificonf5GWPAGeneral(wp, mbssid,1);
		if (!strcmp(security_mode, "WPAPSKWPA2PSK"))
			wificonf5GWPAGeneral(wp, mbssid,2);					
		LFW(pass_phrase_str, passphrase);
		LFW(wpapsk_compatible,wpapsk_compatible);		

		if (!strcmp(wpapsk_compatible, "1")){ // WPAPSKWPA2PSK
			ezplib_replace_attr("wl1_sec_wpa2_rule", mbssid, "wpacap", wpapsk_compatible);		
			wificonf5GWPAGeneral(wp, mbssid,2);
		}else{
			ezplib_replace_attr("wl1_sec_wpa2_rule", mbssid, "wpacap", wpapsk_compatible);
		}
		if(!strcmp(security_mode , "WPA2PSK")){
			strcpy(security_mode,"psk2");
		}
		ezplib_replace_attr("wl1_sec_rule", mbssid, "secmode", security_mode);
		ezplib_replace_attr("wl1_sec_wpa2_rule", mbssid, "key", pass_phrase_str);
	
	}
	else{
		return;
	}
//# WPS
	{
		if (strcmp(TempSSID, ssid)){
			ezplib_get_attr_val("wl1_wps_rule", 0, "configured", wordlist, sizeof(wordlist), EZPLIB_USE_CLI);
			if(wordlist){
				if (strcmp(wordlist, "0"))
					//doSystem("iwpriv ra0 set WscConfStatus=1");
					system("/sbin/ezp-wps-set 0 1 1 1");
				ezplib_replace_attr("wl1_wps_rule", 0, "configured" ,"1");
				g1_wsc_configured = 1;
			}
		}
	}
//# WPS
	system("/sbin/ezp-wl1-ctrl basic");
	//doSystem("iwpriv ra0 set WscConfStatus=2");
	system("/sbin/ezp-wps-set 0 1 1 2");
	nvram_commit();

	initInternet();
#endif

#if 1//Arthur Chow 2009-03-06		
		setWebMessage(0, NULL);
#endif				
	websRedirect(wp, "local/advance/security_5g.asp");
}

// Tommy, decrease code size , 2009/3/17 01:28
#if Include_Ralink_Original
void Security(int nvram, webs_t wp, char_t *path, char_t *query)
{
	char_t *SSID;
	int mbssid, mbssid_num, i;
	char_t *security_mode;

	LFW(SSID, ssidIndex);
	if(!gstrlen(SSID))
		return;

	mbssid = atoi(SSID);

	default_shown_mbssid[nvram] = mbssid;

	LFW(security_mode, security_mode);

#ifndef CONFIG_RALINK_RT2880
	// RT2880: GPIO13 is belong to WPS PBC indicator.

//	if (!strcmp(security_mode, "Disable") || !strcmp(security_mode, "OPEN"))
//		ledAlways(13, LED_OFF); //turn off security LED (gpio 13)
//	else
//		ledAlways(13, LED_ON); //turn on security LED (gpio 13)

#endif

	//clear Radius settings
	clearRadiusSetting(nvram, mbssid);

	if( !strcmp(security_mode, "Disable")){			// !------------------       Disable Mode --------------
		STFs(nvram, mbssid, "AuthMode", "OPEN");
		STFs(nvram, mbssid, "EncrypType", "NONE");
		STFs(nvram, mbssid, "IEEE8021X", "0");
	}else if( !strcmp(security_mode, "OPEN")){		// !------------------       Open Mode ----------------
		confWEP(nvram, wp, mbssid);
		STFs(nvram, mbssid, "AuthMode", security_mode);
		STFs(nvram, mbssid, "EncrypType", "WEP");
		STFs(nvram, mbssid, "IEEE8021X", "0");
	}else if( !strcmp(security_mode, "SHARED")){	// !------------------       Shared Mode ----------------
		char *security_shared_mode;
		confWEP(nvram, wp, mbssid);

		LFW(security_shared_mode, security_shared_mode);

		STFs(nvram, mbssid, "AuthMode", security_mode);
		if(!strcmp(security_shared_mode, "None"))
			STFs(nvram, mbssid, "EncrypType", "NONE");
		else
			STFs(nvram, mbssid, "EncrypType", "WEP");

		STFs(nvram, mbssid, "IEEE8021X", "0");
	}else if( !strcmp(security_mode, "WEPAUTO")){ // !------------------       WEP Auto Mode ----------------
		confWEP(nvram, wp, mbssid);
		STFs(nvram, mbssid, "AuthMode", security_mode);
		STFs(nvram, mbssid, "EncrypType", "WEP");
		STFs(nvram, mbssid, "IEEE8021X", "0");
	}else if(	!strcmp(security_mode, "WPA") ||  
				!strcmp(security_mode, "WPA1WPA2") ){	// !------------------		WPA Enterprise Mode ----------------
		conf8021x(nvram, wp, mbssid);							// !------------------		WPA1WPA2 mixed mode
		confWPAGeneral(nvram, wp, mbssid);

		STFs(nvram, mbssid, "AuthMode", security_mode);
		STFs(nvram, mbssid, "IEEE8021X", "0");
	}else if( !strcmp(security_mode, "WPAPSK")){ // !------------------       WPA Personal Mode ----------------
		char *pass_phrase_str;

		confWPAGeneral(nvram, wp, mbssid);

		LFW(pass_phrase_str, passphrase);

		STFs(nvram, mbssid, "AuthMode", security_mode);
		STFs(nvram, mbssid, "IEEE8021X", "0");

		if ((RT2860_NVRAM == nvram) || (RTINIC_NVRAM == nvram)){
			nvram_bufset(nvram, racat("WPAPSK", mbssid+1), pass_phrase_str);
		} else if (RT2561_NVRAM == nvram) {
			// nvram_bufset(nvram, "WPAPSK", pass_phrase_str);
			STFs(nvram, mbssid, "WPAPSK", pass_phrase_str);
		}
	}else if( !strcmp(security_mode, "WPA2")){		// !------------------  WPA2 Enterprise Mode ----------------
		char *pass_phrase_str;
		char *PMKCachePeriod;
		char *PreAuth;

		conf8021x(nvram, wp, mbssid);
		confWPAGeneral(nvram, wp, mbssid);

		LFW(pass_phrase_str, passphrase);
		LFW(PMKCachePeriod, PMKCachePeriod);
		LFW(PreAuth, PreAuthentication);

		STFs(nvram, mbssid, "AuthMode", security_mode);
		STFs(nvram, mbssid, "IEEE8021X", "0");
		if ((RT2860_NVRAM == nvram) || (RTINIC_NVRAM == nvram)){
			nvram_bufset(nvram, racat("WPAPSK", mbssid+1), pass_phrase_str);
		} else if (RT2561_NVRAM == nvram) {
			// nvram_bufset(nvram, "WPAPSK", pass_phrase_str);
			STFs(nvram, mbssid, "WPAPSK", pass_phrase_str);
		}
		STF(nvram, mbssid, PMKCachePeriod);
		STF(nvram, mbssid, PreAuth);
	}else if(	!strcmp(security_mode, "WPA2PSK") ||	// !------------------  WPA2 Personal Mode ----------------
				!strcmp(security_mode, "WPAPSKWPA2PSK") ){ 	//! -------------   WPA PSK WPA2 PSK mixed
		char *pass_phrase_str;

		confWPAGeneral(nvram, wp, mbssid);
		LFW(pass_phrase_str, passphrase);

		STFs(nvram, mbssid, "AuthMode", security_mode);
		STFs(nvram, mbssid, "IEEE8021X", "0");

		if ((RT2860_NVRAM == nvram) || (RTINIC_NVRAM == nvram)) {
			nvram_bufset(nvram, racat("WPAPSK", mbssid+1), pass_phrase_str);
		} else if (RT2561_NVRAM == nvram) {
			// nvram_bufset(nvram, "WPAPSK", pass_phrase_str);
			STFs(nvram, mbssid, "WPAPSK", pass_phrase_str);
		}
	}else if( !strcmp(security_mode, "IEEE8021X")){	// !------------------ 802.1X WEP Mode ----------------
		char *ieee8021x_wep;

		conf8021x(nvram, wp, mbssid);
		STFs(nvram, mbssid, "IEEE8021X", "1");
		STFs(nvram, mbssid, "AuthMode", "OPEN");

		LFW(ieee8021x_wep, ieee8021x_wep);
		if(ieee8021x_wep[0] == '0')
			STFs(nvram, mbssid, "EncrypType", "NONE");
		else
			STFs(nvram, mbssid, "EncrypType", "WEP");
	}else{
		return;
	}

//# Access Policy
	if(AccessPolicyHandle(nvram, wp, mbssid) == -1)
		trace(0, "** error in AccessPolicyHandle()\n");

//# WPS
	{
		if(nvram == RT2860_NVRAM && mbssid == 0){		// only ra0 supports WPS now.
			char *wordlist= nvram_bufget(RT2860_NVRAM, "WscModeOption");
			if(wordlist){
				if (strcmp(wordlist, "0"))
					//doSystem("iwpriv rai0 set WscConfStatus=1");
					system("/sbin/ezp-wps-set 0 0 1 1");
				nvram_bufset(RT2860_NVRAM, "WscConfigured", "1");
				g_wsc_configured = 1;
			}
		}
	}
//# WPS

	nvram_commit(nvram);

	/*
	 *  TODO: move to inic.c
	 */
	mbssid_num = atoi(nvram_bufget(nvram, "BssidNum"));
	if(nvram == RT2860_NVRAM){
		doSystem("ralink_init make_wireless_config rt2860");
		
//Ralink fixed bug: multi-ssid security problem		
#if 0
		for(i=0; i<mbssid_num; i++){
			doSystem("ifconfig ra%d down", i);
			doSystem("ifconfig ra%d up", i);
		}
#else		
		for(i=0; i<mbssid_num; i++){
			doSystem("ifconfig ra%d down", i);
	       	}
        	for(i=0; i<mbssid_num; i++){			
			doSystem("ifconfig ra%d up", i);
		}
#endif		
		WPSRestart();
	}else if(nvram == RTINIC_NVRAM){
		doSystem("ralink_init make_wireless_config inic");
//Ralink fixed bug: multi-ssid security problem		
#if 0
		for(i=0; i<mbssid_num; i++){
			doSystem("ifconfig rai%d down", i);
			doSystem("ifconfig rai%d up", i);
		}
#else		
		for(i=0; i<mbssid_num; i++){
			doSystem("ifconfig rai%d down", i);
	       	}
        	for(i=0; i<mbssid_num; i++){			
			doSystem("ifconfig rai%d up", i);
		}
#endif		
		// WPSRestart();
	} else if(nvram == RT2561_NVRAM) {
		doSystem("ralink_init make_wireless_config rt2561");
//Ralink fixed bug: multi-ssid security problem		
#if 0
		for(i=0; i<mbssid_num; i++){
			doSystem("ifconfig raL%d down", i);
			doSystem("ifconfig raL%d up", i);
		}
#else		
		for(i=0; i<mbssid_num; i++){
			doSystem("ifconfig raL%d down", i);
	       	}
        	for(i=0; i<mbssid_num; i++){			
			doSystem("ifconfig raL%d up", i);
		}
#endif		
		// WPSRestart();
	}else
		printf("*** Unknown interface.\n");

	restart8021XDaemon(nvram);

	//debug print
	websHeader(wp);
	websWrite(wp, T("<h2>MBSSID index: %d, Security Mode: %s Done</h2><br>\n"),
			mbssid, security_mode);
	websFooter(wp);
	websDone(wp, 200);	

}
#endif // #if Include_Ralink_Original

static void wifiAPGeneral(webs_t wp, char_t *path, char_t *query)
{
	
	APGeneral( wp, path, query);
}

static void wifiAPGeneral5G(webs_t wp, char_t *path, char_t *query)
{
	
	APGeneral5G( wp, path, query);
}

/*
static void wifiAPGeneral(webs_t wp, char_t *path, char_t *query)
{
//	APGeneral(RT2860_NVRAM, wp, path, query);
}
*/

// Tommy, decrease code size , 2009/3/17 01:28
#if Include_Ralink_Original
static void APSecurity(webs_t wp, char_t *path, char_t *query)
{
	Security(RT2860_NVRAM, wp, path, query);
}
#endif // #if Include_Ralink_Original

//void MFSecurity(int nvram, webs_t wp, char_t *path, char_t *query)
void MFSecurity(webs_t wp, char_t *path, char_t *query)

{
#if 1	
	char_t *SSID;
	int mbssid, mbssid_num, i;
	char_t *security_mode;

	LFW(SSID, ssidIndex);
	if(!gstrlen(SSID))
		return;

	mbssid = atoi(SSID);
	printf(">>>>>>>>>>>>>>>>>>>>>mbssid: %d\n", mbssid);
//	default_shown_mbssid[0] = mbssid;
// Tommy, Accelerate Wireless Apply, 2009/5/14 01:30
//#if 0 // Tommy, Accelerate Wireless Apply, 2009/5/14 01:30	
	
//#if 0
//	LFW(security_mode, security_mode);

//#ifndef CONFIG_RALINK_RT2880
	// RT2880: GPIO13 is belong to WPS PBC indicator.

//	if (!strcmp(security_mode, "Disable") || !strcmp(security_mode, "OPEN"))
//		ledAlways(13, LED_OFF); //turn off security LED (gpio 13)
//	else
//		ledAlways(13, LED_ON); //turn on security LED (gpio 13)

//#endif


//#endif

//#endif // Tommy, Accelerate Wireless Apply, 2009/5/14 01:30
// Tommy, Accelerate Wireless Apply, 2009/5/14 01:30

//# Access Policy
//	if(AccessPolicyHandle(nvram, wp, mbssid) == -1)
	if(AccessPolicyHandle( wp, mbssid) == -1)
		trace(0, "** error in AccessPolicyHandle()\n");

//	nvram_commit(nvram);
	system("/sbin/ezp-wl0-ctrl basic");
	
	nvram_commit();

	//initInternet();
//	reconfig_wireless(RT2860_NVRAM);	

	//restart8021XDaemon(RT2860_NVRAM);

#if 1//Arthur Chow 2009-03-06		
		setWebMessage(0, NULL);
#endif
	websRedirect(wp, "local/advance/mf.asp");

#endif	
}


//void MF5GSecurity(int nvram, webs_t wp, char_t *path, char_t *query)
void MF5GSecurity(webs_t wp, char_t *path, char_t *query)

{
#if 1	
	char_t *SSID;
	int mbssid, mbssid_num, i;
	char_t *security_mode;

	LFW(SSID, ssidIndex);
	if(!gstrlen(SSID))
		return;

	mbssid = atoi(SSID);
	if(AccessPolicy5GHandle( wp, mbssid) == -1)
		trace(0, "** error in AccessPolicyHandle()\n");
	system("/sbin/ezp-wl1-ctrl basic");
	
	nvram_commit();

#if 1//Arthur Chow 2009-03-06		
		setWebMessage(0, NULL);
#endif
	websRedirect(wp, "local/advance/mf_5g.asp");

#endif	
}

// goform/wifiAssocList
static void wifiAssocList(webs_t wp, char_t *path, char_t *query)
{
	websRedirect(wp, "local/advance/assoclist.asp");
}

// goform/EZwifiAPSecurity
static void EZwifiAPSecurity(webs_t wp, char_t *path, char_t *query)
{
	char_t *ssid = websGetVar(wp, T("ssid"), T(""));
	char_t *password = websGetVar(wp, T("password"), T(""));
	char_t *security = websGetVar(wp, T("sel_security"), T(""));
	char wordlist[5];
	char TempSSID[128];
	ezplib_get_attr_val("wl0_ssid_rule", 0, "ssid", TempSSID, 128, EZPLIB_USE_CLI);
	
	//set SSID
	ezplib_replace_attr("wl0_ssid_rule", 0, "ssid", ssid);

	if( !strcmp(security, "0")){
		ezplib_replace_attr("wl0_sec_rule", 0, "secmode", "disabled");
	}else if( !strcmp(security, "1")){ //WPA-PSK
		ezplib_replace_attr("wl0_sec_rule", 0, "secmode", "psk");
		ezplib_replace_attr("wl0_sec_wpa_rule", 0, "crypto", "tkip");
		ezplib_replace_attr("wl0_sec_wpa_rule", 0, "key", password);			
	}else if( !strcmp(security, "2")){ //WPA2-PSK
		ezplib_replace_attr("wl0_sec_rule", 0, "secmode", "psk2");
		ezplib_replace_attr("wl0_sec_wpa2_rule", 0, "crypto", "aes");
		ezplib_replace_attr("wl0_sec_wpa2_rule", 0, "key", password);
	}

// Tommy, EZ mode change setting, WPS need to change Configured, 2009/3/24 11:06
//# WPS
#if 0
	{
		//if(nvram == RT2860_NVRAM && mbssid == 0){		// only ra0 supports WPS now.
		if (strcmp(TempSSID, ssid)){
			//char *wordlist= nvram_bufget(RT2860_NVRAM, "WscModeOption");
			ezplib_get_attr_val("wl_wps_rule", 0, "configured", wordlist, sizeof(wordlist), EZPLIB_USE_CLI);
			if(wordlist){
				if (strcmp(wordlist, "0"))
					doSystem("iwpriv ra0 set WscConfStatus=1");
				//nvram_bufset(RT2860_NVRAM, "WscConfigured", "1");
				ezplib_replace_attr("wl_wps_rule", 0, "configured" ,"1");
				g_wsc_configured = 1;
			}
		}
	}
#endif
//# WPS

// Tommy, WR71210IRA set SSID and Security in EZ mode 
#if 1
//	doSystem("iwpriv rai0 set WscConfStatus=1");
	ezplib_replace_attr("wl_wps_rule", 0, "configured" ,"1");
	g_wsc_configured = 1;
#endif

	system("/sbin/ezp-wl0-ctrl basic");
	//doSystem("iwpriv rai0 set WscConfStatus=2");
	system("/sbin/ezp-wps-set 0 0 1 2");
	nvram_commit();	
	websRedirect(wp, "local/wireless_security.asp");
}
	
// goform/EZAPSecurity
static void EZAPSecurity(webs_t wp, char_t *path, char_t *query)
{
	char_t *ssid = websGetVar(wp, T("ssid"), T(""));
	char_t *password = websGetVar(wp, T("password"), T(""));
	char_t *security = websGetVar(wp, T("sel_security"), T(""));

	//set SSID
	ezplib_replace_attr("wl0_ssid_rule", 0, "ssid", ssid);

	if( !strcmp(security, "0")){
		ezplib_replace_attr("wl0_sec_rule", 0, "secmode", "disabled");
	}else if( !strcmp(security, "1")){ //WPA-PSK
		ezplib_replace_attr("wl0_sec_rule", 0, "secmode", "psk");
		ezplib_replace_attr("wl0_sec_wpa_rule", 0, "crypto", "tkip");
		ezplib_replace_attr("wl0_sec_wpa_rule", 0, "key", password);			
	}else if( !strcmp(security, "2")){ //WPA2-PSK
		ezplib_replace_attr("wl0_sec_rule", 0, "secmode", "psk2");
		ezplib_replace_attr("wl0_sec_wpa2_rule", 0, "crypto", "aes");
		ezplib_replace_attr("wl0_sec_wpa2_rule", 0, "key", password);
	}


// Tommy, WR71210IRA set SSID and Security in EZ mode 
#if 1
//	doSystem("iwpriv rai0 set WscConfStatus=1");
	ezplib_replace_attr("wl_wps_rule", 0, "configured" ,"1");
	g_wsc_configured = 1;
#endif
	
	printf("/sbin/ezp-wl0-ctrl basic\n");
	system("/sbin/ezp-wl0-ctrl basic");
	//doSystem("iwpriv rai0 set WscConfStatus=2");
	system("/sbin/ezp-wps-set 0 0 1 2");
// Tommy, EZ mode change setting, WPS need to change Configured, 2009/3/24 11:06
#if 0
//#WPS
	{
		char *wordlist= nvram_bufget(RT2860_NVRAM, "WscModeOption");
		if(wordlist){
			if (strcmp(wordlist, "0"))
				doSystem("iwpriv ra0 set WscConfStatus=1");
			nvram_bufset(RT2860_NVRAM, "WscConfigured", "1");
			g_wsc_configured = 1;
		}
	}

//#WPS
#endif
	nvram_commit();	
	websRedirect(wp, "local/genie5.html");
}

static void wifiAPSecurity(webs_t wp, char_t *path, char_t *query)
{
	wifiSecurity( wp, path, query);
}
static void wifi5GAPSecurity(webs_t wp, char_t *path, char_t *query)
{
	wifi5GSecurity( wp, path, query);
}

/*
static void wifiAPSecurity(webs_t wp, char_t *path, char_t *query)
{
	//wifiSecurity(RT2860_NVRAM, wp, path, query);
}
*/
static void MultiFilterSecurity(webs_t wp, char_t *path, char_t *query)
{
	//MFSecurity(RT2860_NVRAM, wp, path, query);
	MFSecurity(wp, path, query);
}

static void MultiFilter5GSecurity(webs_t wp, char_t *path, char_t *query)
{
	MF5GSecurity(wp, path, query);
}

//void DeleteAccessPolicyList(int nvram, webs_t wp, char_t *path, char_t *query)
void DeleteAccessPolicyList(webs_t wp, char_t *path, char_t *query)
{
        char *value, *index, *value1;
        char tmp[TMP_LEN], buf[TMP_LEN], tmp1[TMP_LEN];
        int i, j=0, left_num=0, rule_num0, rule_num1, rule_num2, rule_num3;

	int mbssid,aplist_num;
	char_t *SSID;

	sscanf(query, "%d,%d", &mbssid, &aplist_num);
                
        rule_num0 = atoi(nvram_safe_get("wl00_acl_num"));
        rule_num1 = atoi(nvram_safe_get("wl01_acl_num"));
        rule_num2 = atoi(nvram_safe_get("wl02_acl_num"));
        rule_num3 = atoi(nvram_safe_get("wl03_acl_num"));

	if(mbssid == 0){
                sprintf(tmp, "newap_%d_%d", mbssid,aplist_num);
	
		if(tmp)
		{
		    if( (left_num = ezplib_get_rule_num("wl00_acl_rule")) ==1)
		    {
		    	nvram_set("wl00_acl_rule", "");
				ezplib_replace_attr("wl00_acl_basic_rule", 0, "defpolicy", "0");		 
		    }
		    else
		        ezplib_delete_rule("wl00_acl_rule", aplist_num);
		    j++;

		}

		if(j>0){
           		rule_num0 = rule_num0 - j;
           		sprintf(tmp,"%d", rule_num0);       
           		nvram_set("wl00_acl_num", tmp); //update rule numbers
        		}
	} 
	else if(mbssid == 1){
                sprintf(tmp, "newap_%d_%d", mbssid,aplist_num);
		
		if(tmp)
		{
		    if( (left_num = ezplib_get_rule_num("wl01_acl_rule")) ==1)
		    {
		    	nvram_set("wl01_acl_rule", "");
				ezplib_replace_attr("wl01_acl_basic_rule", 0, "defpolicy", "0");		    	
		    }
		    else
		        ezplib_delete_rule("wl01_acl_rule", aplist_num);
		        
		    j++;
		}

		if(j>0){
           		rule_num1 = rule_num1 - j;
           		sprintf(tmp,"%d", rule_num1);       
           		nvram_set("wl01_acl_num", tmp); //update rule numbers
        		}
	}
	else if(mbssid == 2){
                sprintf(tmp, "newap_%d_%d", mbssid,aplist_num);
	
		if(tmp)
		{
		    if( (left_num = ezplib_get_rule_num("wl02_acl_rule")) ==1)
		    {
		    	nvram_set("wl02_acl_rule", "");
				ezplib_replace_attr("wl02_acl_basic_rule", 0, "defpolicy", "0");		    	
		    }
		    else
		        ezplib_delete_rule("wl02_acl_rule", aplist_num);
		       
		    j++;
		}
		if(j>0){
           		rule_num2 = rule_num2 - j;
           		sprintf(tmp,"%d", rule_num2);        
           		nvram_set("wl02_acl_num", tmp); //update rule numbers
        		}
	}
	else if(mbssid == 3){
                sprintf(tmp, "newap_%d_%d", mbssid,aplist_num);
		
		if(tmp)
		{
		    if( (left_num = ezplib_get_rule_num("wl03_acl_rule")) ==1)
		    {
		    	nvram_set("wl03_acl_rule", "");
				ezplib_replace_attr("wl03_acl_basic_rule", 0, "defpolicy", "0");		    	
		    }
		    else
		        ezplib_delete_rule("wl03_acl_rule", aplist_num);
		        
		    j++;

		}

		if(j>0){
           		rule_num3 = rule_num3 - j;
           		sprintf(tmp,"%d", rule_num3);        
           		nvram_set("wl03_acl_num", tmp); //update rule numbers
        		}
	} 

	nvram_commit();          
	
	websWrite(wp, T("HTTP/1.1 200 OK\nContent-type: text/plain\nPragma: no-cache\nCache-Control: no-cache\n\n"));
	websWrite(wp, T("ok done"));
	websDone(wp, 200);

}


//void Delete5GAccessPolicyList(int nvram, webs_t wp, char_t *path, char_t *query)
void Delete5GAccessPolicyList(webs_t wp, char_t *path, char_t *query)
{
        char *value, *index, *value1;
        char tmp[TMP_LEN], buf[TMP_LEN], tmp1[TMP_LEN];
        int i, j=0, left_num=0, rule_num0, rule_num1, rule_num2, rule_num3;

	int mbssid,aplist_num;
	char_t *SSID;

	sscanf(query, "%d,%d", &mbssid, &aplist_num);
                
        rule_num0 = atoi(nvram_safe_get("wl10_acl_num"));
        rule_num1 = atoi(nvram_safe_get("wl11_acl_num"));
        rule_num2 = atoi(nvram_safe_get("wl12_acl_num"));
        rule_num3 = atoi(nvram_safe_get("wl13_acl_num"));

	if(mbssid == 0){
                sprintf(tmp, "newap_%d_%d", mbssid,aplist_num);
	
		if(tmp)
		{
		    if( (left_num = ezplib_get_rule_num("wl10_acl_rule")) ==1)
		    {
		    	nvram_set("wl10_acl_rule", "");
				ezplib_replace_attr("wl10_acl_basic_rule", 0, "defpolicy", "0");		 
		    }
		    else
		        ezplib_delete_rule("wl10_acl_rule", aplist_num);
		    j++;

		}

		if(j>0){
           		rule_num0 = rule_num0 - j;
           		sprintf(tmp,"%d", rule_num0);       
           		nvram_set("wl10_acl_num", tmp); //update rule numbers
        		}
	} 
	else if(mbssid == 1){
                sprintf(tmp, "newap_%d_%d", mbssid,aplist_num);
		
		if(tmp)
		{
		    if( (left_num = ezplib_get_rule_num("wl11_acl_rule")) ==1)
		    {
		    	nvram_set("wl11_acl_rule", "");
				ezplib_replace_attr("wl11_acl_basic_rule", 0, "defpolicy", "0");		    	
		    }
		    else
		        ezplib_delete_rule("wl11_acl_rule", aplist_num);
		        
		    j++;
		}

		if(j>0){
           		rule_num1 = rule_num1 - j;
           		sprintf(tmp,"%d", rule_num1);       
           		nvram_set("wl11_acl_num", tmp); //update rule numbers
        		}
	}
	else if(mbssid == 2){
                sprintf(tmp, "newap_%d_%d", mbssid,aplist_num);
	
		if(tmp)
		{
		    if( (left_num = ezplib_get_rule_num("wl12_acl_rule")) ==1)
		    {
		    	nvram_set("wl12_acl_rule", "");
				ezplib_replace_attr("wl12_acl_basic_rule", 0, "defpolicy", "0");		    	
		    }
		    else
		        ezplib_delete_rule("wl12_acl_rule", aplist_num);
		       
		    j++;
		}
		if(j>0){
           		rule_num2 = rule_num2 - j;
           		sprintf(tmp,"%d", rule_num2);        
           		nvram_set("wl12_acl_num", tmp); //update rule numbers
        		}
	}
	else if(mbssid == 3){
                sprintf(tmp, "newap_%d_%d", mbssid,aplist_num);
		
		if(tmp)
		{
		    if( (left_num = ezplib_get_rule_num("wl13_acl_rule")) ==1)
		    {
		    	nvram_set("wl13_acl_rule", "");
				ezplib_replace_attr("wl13_acl_basic_rule", 0, "defpolicy", "0");		    	
		    }
		    else
		        ezplib_delete_rule("wl13_acl_rule", aplist_num);
		        
		    j++;

		}

		if(j>0){
           		rule_num3 = rule_num3 - j;
           		sprintf(tmp,"%d", rule_num3);        
           		nvram_set("wl13_acl_num", tmp); //update rule numbers
        		}
	} 

	nvram_commit();          
	
	websWrite(wp, T("HTTP/1.1 200 OK\nContent-type: text/plain\nPragma: no-cache\nCache-Control: no-cache\n\n"));
	websWrite(wp, T("ok done"));
	websDone(wp, 200);

}

static void APDeleteAccessPolicyList(webs_t wp, char_t *path, char_t *query)
{
	//DeleteAccessPolicyList(RT2860_NVRAM, wp, path, query);
	DeleteAccessPolicyList(wp, path, query);
}

static void APDelete5GAccessPolicyList(webs_t wp, char_t *path, char_t *query)
{
	//DeleteAccessPolicyList(RT2860_NVRAM, wp, path, query);
	Delete5GAccessPolicyList(wp, path, query);
}
static int isWPSConfiguredASP(int eid, webs_t wp, int argc, char_t **argv)
{
	if(g_wsc_configured){
		websWrite(wp, T("1"));
	}else
		websWrite(wp, T("0"));	
	return 0;
}

// Tommy, 2008/12/25 11:25上午
int length(char str[])
{
	int i=0;
	while (str[i] !='\0' && i<255) i++;
	if (i==255 && str[255]!='\0')
	   str[i--]='\0';
	return(i);
}

int get_channel(char channel[], char str[], char pattern[])
{
	int lnth, lnthdif,i,j,k;
	lnth=length(pattern);
	lnthdif=length(str)-lnth+1;
	if (lnth>0 && lnthdif>0){
		i=0;
		while (i<lnthdif){
			j=i; k=0;
			while (k<lnth && str[j++]==pattern[k]) k++;
			if (k==lnth){
			    if (str[i+lnth+1] == ' '){
				channel[0] = str[i+lnth];
				channel[1] = '\0';
				return 0;
			    }else if(str[i+lnth+2] == ' '){
				channel[0] = str[i+lnth];
				channel[1] = str[i+lnth+1];
				channel[2] = '\0';
				return 0;
			    }else{
				channel[0] = str[i+lnth];
				channel[1] = str[i+lnth+1];
				channel[2] = str[i+lnth+2];
				channel[3] = '\0';
				return 0;
			   }
			}else{
			    i++;
			}
		}
	}
	return -1;
}




void readline(char str[], FILE *fin)
{
	int n=0;
	char c;
	while ((c=fgetc(fin)) != '\n' && n<255)
		str[n++] = c;
	str[n] = '\0';
}

static int  getOpChannleASP(int eid, webs_t wp, int argc, char_t **argv)
{
	FILE *fin;
	char str[255],c;
	char channel[3];
	char ptn[]="Channel=";
	int channel_no, set_channel_no;

	
		//system("iwconfig rai0 > /tmp/opchannel");
		system("/sbin/ezp-wps-set 1 0 1 0");
		fin = fopen("/tmp/opchannel","r");
		
		while ((c=fgetc(fin)) != EOF){
			ungetc(c,fin);		
			readline(str,fin);
			if (get_channel(channel, str, ptn) == 0)
		   	break;
		}

		fclose(fin);

		system("rm -f /tmp/opchannel");	
		channel_no = atoi(&channel);

		if (channel_no == 1){
	    	    websWrite(wp, T("%s"), "Channel-01 2412MHz");
		}else if (channel_no == 2){
	    	    websWrite(wp, T("%s"), "Channel-02 2417MHz");
		}else if (channel_no == 3){	
	    	    websWrite(wp, T("%s"), "Channel-03 2422MHz");
		}else if (channel_no == 4){	
	    	    websWrite(wp, T("%s"), "Channel-04 2427MHz");
		}else if (channel_no == 5){	
	    	    websWrite(wp, T("%s"), "Channel-05 2432MHz");
		}else if (channel_no == 6){	
	    	    websWrite(wp, T("%s"), "Channel-06 2437MHz");
		}else if (channel_no == 7){	
	    	    websWrite(wp, T("%s"), "Channel-07 2442MHz");
		}else if (channel_no == 8){	
	    	    websWrite(wp, T("%s"), "Channel-08 2447MHz");
		}else if (channel_no == 9){	
	    	    websWrite(wp, T("%s"), "Channel-09 2452MHz");
		}else if (channel_no == 10){	
	    	    websWrite(wp, T("%s"), "Channel-10 2457MHz");
		}else if (channel_no == 11){	
	    	    websWrite(wp, T("%s"), "Channel-11 2462MHz");
		}else if (channel_no == 12){	
	    	    websWrite(wp, T("%s"), "Channel-12 2467MHz");
		}else if (channel_no == 13){	
	    	    websWrite(wp, T("%s"), "Channel-13 2472MHz");
		}else if (channel_no == 14){	
	    	    websWrite(wp, T("%s"), "Channel-14 2484MHz");
		}

	return 0;
}	
static int  getOpChannleASP5G(int eid, webs_t wp, int argc, char_t **argv)
{
	FILE *fin;
	char str[255],c;
	char channel[3];
	char ptn[]="Channel=";
	int channel_no, set_channel_no;

		//sleep(3);
		//system("iwconfig ra0 > /tmp/opchannel");
		system("/sbin/ezp-wps-set 1 1 1 0");

		fin = fopen("/tmp/opchannel","r");
		
		while ((c=fgetc(fin)) != EOF){
			ungetc(c,fin);		
			readline(str,fin);
			if (get_channel(channel, str, ptn) == 0)
		   	break;
		}

		fclose(fin);

		system("rm -f /tmp/opchannel");	
		channel_no = atoi(&channel);
             // system("iwconfig ra0 >> /tmp/bug");
	       //char buffer[512];
	       //sprintf(buffer, "echo channel_no=%s >>/tmp/bug",channel);
		//system(buffer);
		if(channel_no == 0){
	    	    websWrite(wp, T("%s"), "Channel-36 5180MHz");
		}else if (channel_no == 36){
	    	    websWrite(wp, T("%s"), "Channel-36 5180MHz");
		}else if (channel_no == 40){	
	    	    websWrite(wp, T("%s"), "Channel-40 5200MHz");
		}else if (channel_no == 44){	
	    	    websWrite(wp, T("%s"), "Channel-44 5220MHz");
		}else if (channel_no == 48){	
	    	    websWrite(wp, T("%s"), "Channel-48 5240MHz");
		}else if (channel_no == 52){	
	    	    websWrite(wp, T("%s"), "Channel-52 5260MHz");
		}else if (channel_no == 56){	
	    	    websWrite(wp, T("%s"), "Channel-56 5280MHz");
		}else if (channel_no == 60){	
	    	    websWrite(wp, T("%s"), "Channel-60 5300MHz");
		}else if (channel_no == 64){	
	    	    websWrite(wp, T("%s"), "Channel-64 5320MHz");
		}else if (channel_no == 100){	
	    	    websWrite(wp, T("%s"), "Channel-100 5500MHz");
		}else if (channel_no == 104){	
	    	    websWrite(wp, T("%s"), "Channel-104 5520MHz");
		}else if (channel_no == 108){	
	    	    websWrite(wp, T("%s"), "Channel-108 5540MHz");
		}else if (channel_no == 112){	
	    	    websWrite(wp, T("%s"), "Channel-112 5560MHz");
		}else if (channel_no == 116){
	    	    websWrite(wp, T("%s"), "Channel-116 5580MHz");
		}else if (channel_no == 132){	
	    	    websWrite(wp, T("%s"), "Channel-132 5660MHz");
		}else if (channel_no == 136){	
	    	    websWrite(wp, T("%s"), "Channel-136 5680MHz");
		}else if (channel_no == 140){	
	    	    websWrite(wp, T("%s"), "Channel-140 5700MHz");
		}else if (channel_no == 149){	
	    	    websWrite(wp, T("%s"), "Channel-149 5745MHz");
		}else if (channel_no == 153){	
	    	    websWrite(wp, T("%s"), "Channel-153 5765MHz");
		}else if (channel_no == 157){	
	    	    websWrite(wp, T("%s"), "Channel-157 5785MHz");
		}else if (channel_no == 161){	
	    	    websWrite(wp, T("%s"), "Channel-161 5805MHz");
		}else if (channel_no == 165){	
	    	    websWrite(wp, T("%s"), "Channel-165 5825MHz");
		}

	return 0;
}	

static int  getOpChannleNum(int eid, webs_t wp, int argc, char_t **argv)
{
	FILE *fin;
	char str[255],c;
	char channel[3];
	char ptn[]="Channel=";
	int channel_no, set_channel_no;
       char apcli_enable[32];
       ezplib_get_attr_val("wl0_apcli_rule", 0, "enable", apcli_enable,32,EZPLIB_USE_CLI);

       if(0==atoi(apcli_enable))
       {
        	websWrite(wp, T("%s"),"6");
	       return 0;
        }
    

	//system("iwconfig rai0 > /tmp/dat");
	system("/sbin/ezp-wps-set 1 0 2 0");
	
	fin = fopen("/tmp/dat","r");
		
	while ((c=fgetc(fin)) != EOF){
			ungetc(c,fin);		
			readline(str,fin);
			if (get_channel(channel, str, ptn) == 0)
		   	break;
	}
		
	fclose(fin);
	system("rm -f /tmp/dat");
	
	websWrite(wp, T("%s"),channel);
	return 0;	
}

static int  getOpChannleNum5G(int eid, webs_t wp, int argc, char_t **argv)
{
	FILE *fin;
	char str[255],c;
	char channel[3];
	char ptn[]="Channel=";
	int channel_no, set_channel_no;

	//system("iwconfig ra0 > /tmp/dat");
	system("/sbin/ezp-wps-set 1 1 2 0");
	fin = fopen("/tmp/dat","r");
		
	while ((c=fgetc(fin)) != EOF){
			ungetc(c,fin);		
			readline(str,fin);
			if (get_channel(channel, str, ptn) == 0)
		   	break;
	}
		
	fclose(fin);
	system("rm -f /tmp/dat");
	
	websWrite(wp, T("%s"),channel);
	return 0;	
}

#ifdef CONFIG_RT2860V2_AP_ANTENNA_DIVERSITY
void AntennaDiversityInit(void)
{
#if 0
	char *mode = nvram_bufget(RT2860_NVRAM, "AntennaDiversity");

	if(!gstrcmp(mode, "Disable")){						// Disable
		doSystem("echo 0 > /proc/AntDiv/AD_RUN");
	}else if(!gstrcmp(mode, "Enable_Algorithm1")){
		doSystem("echo 1 > /proc/AntDiv/AD_ALGORITHM"); // Algorithm1
		doSystem("echo 1 > /proc/AntDiv/AD_RUN");
	}else if(!gstrcmp(mode, "Enable_Algorithm2")){
		doSystem("echo 2 > /proc/AntDiv/AD_ALGORITHM"); // Algorithm2
		doSystem("echo 1 > /proc/AntDiv/AD_RUN");
	}else if(!gstrcmp(mode, "Antenna0")){				// fix Ant0
		doSystem("echo 0 > /proc/AntDiv/AD_RUN");
		doSystem("echo 0 > /proc/AntDiv/AD_FORCE_ANTENNA");
	}else if(!gstrcmp(mode, "Antenna2")){				// fix Ant2
		doSystem("echo 0 > /proc/AntDiv/AD_RUN");
		doSystem("echo 2 > /proc/AntDiv/AD_FORCE_ANTENNA");
	}else{
		doSystem("echo 0 > /proc/AntDiv/AD_RUN");
		return;
	}
	return;
#endif	
}

static void AntennaDiversity(webs_t wp, char_t *path, char_t *query)
{
#if 0	
	char_t	*mode;

	mode = websGetVar(wp, T("ADSelect"), T(""));
	if(!mode || !strlen(mode))
		return;

	nvram_bufset(RT2860_NVRAM, "AntennaDiversity", mode);
	nvram_commit(RT2860_NVRAM);

	// re-init
	AntennaDiversityInit();

	//debug print
	websHeader(wp);
	websWrite(wp, T("mode:%s"), mode);
	websFooter(wp);
	websDone(wp, 200);	
#endif	
}

static void getAntenna(webs_t wp, char_t *path, char_t *query)
{
	char buf[32];
	FILE *fp = fopen("/proc/AntDiv/AD_CHOSEN_ANTENNA", "r");
	if(!fp){
		strcmp(buf, "err");
	}else{
		fgets(buf, 32, fp);
		fclose(fp);
	}
	websWrite(wp, T("HTTP/1.1 200 OK\nContent-type: text/plain\nPragma: no-cache\nCache-Control: no-cache\n\n"));
	websWrite(wp, "%s", buf);
	websDone(wp, 200);
}

static int isAntennaDiversityBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
	websWrite(wp, T("1"));
	return 0;
}
#else
static int isAntennaDiversityBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
	websWrite(wp, T("0"));
	return 0;
}
#endif

/* goform/WiFiWMM */
static void WiFiWMM(webs_t wp, char_t *path, char_t *query)
{
#if 1	
	char_t *apsd_capable,*dls_capable;
        char_t *dscp_mode,*dscp_other;
        char tos_voice[320]={0},tos_video[320]={0},tos_data[320]={0};
        char *dscp_value[64]={"0x0","0x4","0x8","0xc","0x10","0x14","0x18","0x1c","0x20","0x24","0x28","0x2c","0x30","0x34","0x38","0x3c","0x40","0x44","0x48","0x4c","0x50","0x54","0x58","0x5c","0x60","0x64","0x68","0x6c","0x70","0x74","0x78","0x7c","0x80","0x84","0x88","0x8c","0x90","0x94","0x98","0x9c","0xA0","0xA4","0xA8","0xAc","0xB0","0xB4","0xB8","0xBc","0xC0","0xC4","0xC8","0xCc","0xD0","0xD4","0xD8","0xDc","0xE0","0xE4","0xE8","0xEc","0xF0","0xF4","0xF8","0xFc"};
	char_t *wmm_capable = websGetVar(wp, T("wmm_capable"), T(""));
        char wmm_capable_old[32];
	int i;
       int value;

        ezplib_get_attr_val("wl0_basic_rule", 0, "wme",wmm_capable_old, 32, EZPLIB_USE_CLI );
       printf("wmm_capable:%d wmm_capable_old:%d#######\n",atoi(wmm_capable),atoi(wmm_capable_old));

       if( atoi(wmm_capable)==1 && atoi(wmm_capable_old)==1)
       {
       }
       else if( atoi(wmm_capable)==1 && atoi(wmm_capable_old)==0)
       {
       }
       else if( atoi(wmm_capable)==0 && atoi(wmm_capable_old)==1)
       {
            char TempBuf[32];
            ezplib_get_attr_val("wl_mode_rule", 0, "mode", TempBuf, 32, EZPLIB_USE_CLI); 	
            if(!strcmp(TempBuf,"wisp"))
            {
               system("/etc/qos/tos_classify  delete");
            }
            
             system("/etc/qos/qos.sh stop");
           
         	ezplib_replace_attr("wl0_basic_rule", 0, "wme", atoi(wmm_capable)   ? "1" : "0");             
            	nvram_commit();
	
    		setWebMessage(0, NULL);		
    	       websRedirect(wp, "local/advance/qos.asp");
              return;

       }
       else if( atoi(wmm_capable)==0 && atoi(wmm_capable_old)==0)
       {
           setWebMessage(0, NULL);
           websRedirect(wp, "local/advance/qos.asp");
           return;
       }

        dscp_mode = websGetVar(wp, T("DSCP_mode_Select"), T(""));
        dscp_other=websGetVar(wp, T("DSCP_Priority_Select_9"), T(""));
        printf("dscp_mode:%s dscp_other:%s#######\n",dscp_mode,dscp_other);
        if(0==strcmp(dscp_mode,"0"))
        {
            strcpy(tos_voice,"0xE0,0xC0,0xB8");
            strcpy(tos_video,"0xA0,0x80");
            strcpy(tos_data,"none");
            ezplib_replace_attr("tos_classify",0,"other","0");
        }
        else
        {
        char *Priority_DSCP =websGetVar(wp, T("DSCP_Priority_Select_1"), T(""));
        char_t *Value_DSCP =websGetVar(wp, T("DSCP_value_1"), T(""));
        value = atoi(Value_DSCP);
        if(0!=strcmp(Value_DSCP,""))
        {
          value = atoi(Value_DSCP);
          dscp_value[value]="";
        }
        if(0==strcmp(Priority_DSCP,"02")&& (0!=strcmp(Value_DSCP,"")))
        {
            sprintf(tos_voice,"0x%x",(value<<2));
        }
        else if(0==strcmp(Priority_DSCP,"01")&&(0!=strcmp(Value_DSCP,"")))
        {
            sprintf(tos_video,"0x%x",(value<<2));
        }
        else if(0==strcmp(Priority_DSCP,"00")&&(0!=strcmp(Value_DSCP,"")))
        {
            sprintf(tos_data,"0x%x",(value<<2));
        }


        Priority_DSCP =websGetVar(wp, T("DSCP_Priority_Select_2"), T(""));
        Value_DSCP =websGetVar(wp, T("DSCP_value_2"), T(""));
        
        if(0!=strcmp(Value_DSCP,""))
        {
          value = atoi(Value_DSCP);
          dscp_value[value]="";
        }
        if(0==strcmp(Priority_DSCP,"02")&&(0!=strcmp(Value_DSCP,"")))
        {
            if(0 != tos_voice[1])
            {
                sprintf(tos_voice,"%s,0x%x",tos_voice,(value<<2));
            }
            else
            {
                 sprintf(tos_voice,"0x%x",(value<<2));
            }
           
        }
        else if(0==strcmp(Priority_DSCP,"01")&&(0!=strcmp(Value_DSCP,"")))
        {
             if(0 != tos_video[1])
            {
                sprintf(tos_video,"%s,0x%x",tos_video,(value<<2));
            }
            else
            {
                 sprintf(tos_video,"0x%x",(value<<2));
            }
        }
        else if(0==strcmp(Priority_DSCP,"00")&&(0!=strcmp(Value_DSCP,"")))
        {
             if(0 != tos_data[1])
            {
                sprintf(tos_data,"%s,0x%x",tos_data,(value<<2));
            }
            else
            {
                 sprintf(tos_data,"0x%x",(value<<2));
            }
        }

        Priority_DSCP =websGetVar(wp, T("DSCP_Priority_Select_3"), T(""));
        Value_DSCP =websGetVar(wp, T("DSCP_value_3"), T(""));
        
        if(0!=strcmp(Value_DSCP,""))
        {
          value = atoi(Value_DSCP);
          dscp_value[value]="";
        }
        if(0==strcmp(Priority_DSCP,"02")&&(0!=strcmp(Value_DSCP,"")))
        {
            if(0 != tos_voice[1])
            {
                sprintf(tos_voice,"%s,0x%x",tos_voice,(value<<2));
            }
            else
            {
                 sprintf(tos_voice,"0x%x",(value<<2));
            }

        }
        else if(0==strcmp(Priority_DSCP,"01")&&(0!=strcmp(Value_DSCP,"")))
        {
             if(0 != tos_video[1])
            {
                sprintf(tos_video,"%s,0x%x",tos_video,(value<<2));
            }
            else
            {
                 sprintf(tos_video,"0x%x",(value<<2));
            }
        }
        else if(0==strcmp(Priority_DSCP,"00")&&(0!=strcmp(Value_DSCP,"")))
        {
             if(0 != tos_data[1])
            {
                sprintf(tos_data,"%s,0x%x",tos_data,(value<<2));
            }
            else
            {
                 sprintf(tos_data,"0x%x",(value<<2));
            }
        }


        Priority_DSCP =websGetVar(wp, T("DSCP_Priority_Select_4"), T(""));
        Value_DSCP =websGetVar(wp, T("DSCP_value_4"), T(""));
       
        if(0!=strcmp(Value_DSCP,""))
        {
          value = atoi(Value_DSCP);
          dscp_value[value]="";
        }
        if(0==strcmp(Priority_DSCP,"02")&&(0!=strcmp(Value_DSCP,"")))
        {
            if(0 != tos_voice[1])
            {
                sprintf(tos_voice,"%s,0x%x",tos_voice,(value<<2));
            }
            else
            {
                 sprintf(tos_voice,"0x%x",(value<<2));
            }

        }
        else if(0==strcmp(Priority_DSCP,"01")&&(0!=strcmp(Value_DSCP,"")))
        {
             if(0 != tos_video[1])
            {
                sprintf(tos_video,"%s,0x%x",tos_video,(value<<2));
            }
            else
            {
                 sprintf(tos_video,"0x%x",(value<<2));
            }
        }
        else if(0==strcmp(Priority_DSCP,"00")&&(0!=strcmp(Value_DSCP,"")))
        {
             if(0 != tos_data[1])
            {
                sprintf(tos_data,"%s,0x%x",tos_data,(value<<2));
            }
            else
            {
                 sprintf(tos_data,"0x%x",(value<<2));
            }
        }


        Priority_DSCP =websGetVar(wp, T("DSCP_Priority_Select_5"), T(""));
        Value_DSCP =websGetVar(wp, T("DSCP_value_5"), T(""));
        
        if(0!=strcmp(Value_DSCP,""))
        {
          value = atoi(Value_DSCP);
          dscp_value[value]="";
        }
        if(0==strcmp(Priority_DSCP,"02")&&(0!=strcmp(Value_DSCP,"")))
        {
            if(0 != tos_voice[1])
            {
                sprintf(tos_voice,"%s,0x%x",tos_voice,(value<<2));
            }
            else
            {
                 sprintf(tos_voice,"0x%x",(value<<2));
            }

        }
        else if(0==strcmp(Priority_DSCP,"01")&&(0!=strcmp(Value_DSCP,"")))
        {
             if(0 != tos_video[1])
            {
                sprintf(tos_video,"%s,0x%x",tos_video,(value<<2));
            }
            else
            {
                 sprintf(tos_video,"0x%x",(value<<2));
            }
        }
        else if(0==strcmp(Priority_DSCP,"00")&&(0!=strcmp(Value_DSCP,"")))
        {
             if(0 != tos_data[1])
            {
                sprintf(tos_data,"%s,0x%x",tos_data,(value<<2));
            }
            else
            {
                 sprintf(tos_data,"0x%x",(value<<2));
            }
        }

        Priority_DSCP =websGetVar(wp, T("DSCP_Priority_Select_6"), T(""));
        Value_DSCP =websGetVar(wp, T("DSCP_value_6"), T(""));
        
        if(0!=strcmp(Value_DSCP,""))
        {
          value = atoi(Value_DSCP);
          dscp_value[value]="";
        }
        if(0==strcmp(Priority_DSCP,"02")&&(0!=strcmp(Value_DSCP,"")))
        {
            if(0 != tos_voice[1])
            {
                sprintf(tos_voice,"%s,0x%x",tos_voice,(value<<2));
            }
            else
            {
                 sprintf(tos_voice,"0x%x",(value<<2));
            }

        }
        else if(0==strcmp(Priority_DSCP,"01")&&(0!=strcmp(Value_DSCP,"")))
        {
             if(0 != tos_video[1])
            {
                sprintf(tos_video,"%s,0x%x",tos_video,(value<<2));
            }
            else
            {
                 sprintf(tos_video,"0x%x",(value<<2));
            }
        }
        else if(0==strcmp(Priority_DSCP,"00")&&(0!=strcmp(Value_DSCP,"")))
        {
             if(0 != tos_data[1])
            {
                sprintf(tos_data,"%s,0x%x",tos_data,(value<<2));
            }
            else
            {
                 sprintf(tos_data,"0x%x",(value<<2));
            }
        }



        Priority_DSCP =websGetVar(wp, T("DSCP_Priority_Select_7"), T(""));
        Value_DSCP =websGetVar(wp, T("DSCP_value_7"), T(""));
        
        if(0!=strcmp(Value_DSCP,""))
        {
          value = atoi(Value_DSCP);
          dscp_value[value]="";
        }
        if(0==strcmp(Priority_DSCP,"02")&&(0!=strcmp(Value_DSCP,"")))
        {
            if(0 != tos_voice[1])
            {
                sprintf(tos_voice,"%s,0x%x",tos_voice,(value<<2));
            }
            else
            {
                 sprintf(tos_voice,"0x%x",(value<<2));
            }

        }
        else if(0==strcmp(Priority_DSCP,"01")&&(0!=strcmp(Value_DSCP,"")))
        {
             if(0 != tos_video[1])
            {
                sprintf(tos_video,"%s,0x%x",tos_video,(value<<2));
            }
            else
            {
                 sprintf(tos_video,"0x%x",(value<<2));
            }
        }
        else if(0==strcmp(Priority_DSCP,"00")&&(0!=strcmp(Value_DSCP,"")))
        {
             if(0 != tos_data[1])
            {
                sprintf(tos_data,"%s,0x%x",tos_data,(value<<2));
            }
            else
            {
                 sprintf(tos_data,"0x%x",(value<<2));
            }
        }
        
        Priority_DSCP =websGetVar(wp, T("DSCP_Priority_Select_8"), T(""));
        Value_DSCP =websGetVar(wp, T("DSCP_value_8"), T(""));
       
        if(0 !=strcmp(Value_DSCP,""))
        {
          value = atoi(Value_DSCP);
          dscp_value[value]="";
        }
        if(0==strcmp(Priority_DSCP,"02")&&(0!=strcmp(Value_DSCP,"")))
        {
            if(0 != tos_voice[1])
            {
                sprintf(tos_voice,"%s,0x%x",tos_voice,(value<<2));
            }
            else
            {
                 sprintf(tos_voice,"0x%x",(value<<2));
            }

        }
        else if(0==strcmp(Priority_DSCP,"01")&&(0!=strcmp(Value_DSCP,"")))
        {
             if(0 != tos_video[1])
            {
                sprintf(tos_video,"%s,0x%x",tos_video,(value<<2));
            }
            else
            {
                 sprintf(tos_video,"0x%x",(value<<2));
            }
        }
        else if(0==strcmp(Priority_DSCP,"00")&&(0!=strcmp(Value_DSCP,"")))
        {
             if(0 != tos_data[1])
            {
                sprintf(tos_data,"%s,0x%x",tos_data,(value<<2));
            }
            else
            {
                 sprintf(tos_data,"0x%x",(value<<2));
            }
        }
        
        Priority_DSCP =websGetVar(wp, T("DSCP_Priority_Select_9"), T(""));
        if(0==strcmp(Priority_DSCP,"02"))
        {
             for(i=0;i<64;i++)
             {
                 for(i=0;i<64;i++)
                 {
                  if(dscp_value[i]!="")
                  {
                    if(i==0)
                    {
                        sprintf(tos_voice,"%s,",dscp_value[i]);
                    }
                    else if(i==63)
                    {
                        sprintf(tos_voice,"%s%s",tos_voice,dscp_value[i]);
                    }
                    else
                    {
                        sprintf(tos_voice,"%s%s,",tos_voice,dscp_value[i]);
                    }
                  }
                 }
             }
          ezplib_replace_attr("tos_classify",0,"other","2");

        }
        else if(0==strcmp(Priority_DSCP,"01"))
        {
             for(i=0;i<64;i++)
             {
               if(dscp_value[i]!="")
               {
                 if(i==0)
                 {
                    sprintf(tos_video,"%s,",dscp_value[i]);
                 }
                 else if(i==63)
                 {
                    sprintf(tos_video,"%s%s",tos_video,dscp_value[i]);
                 }
                 else
                 {
                    sprintf(tos_video,"%s%s,",tos_video,dscp_value[i]);
                 }
               }
             }
         ezplib_replace_attr("tos_classify",0,"other","1");
        }
        else if(0==strcmp(Priority_DSCP,"00"))
        {
             for(i=0;i<64;i++)
             {
               if(dscp_value[i]!="")
               {
                 if(i==0)
                 {
                    sprintf(tos_data,"%s,",dscp_value[i]);
                 }
                 else if(i==63)
                 {
                    sprintf(tos_data,"%s%s",tos_data,dscp_value[i]);
                 }
                 else
                 {
                    sprintf(tos_data,"%s%s,",tos_data,dscp_value[i]);
                 }
              }
             }
         memset(tos_data,0,320);
         strcpy(tos_data,"none");    
         ezplib_replace_attr("tos_classify",0,"other","0");
        }

        }
        
        if(0==strcmp(tos_voice,""))
        {
          strcpy(tos_voice,"none");
        }
        if(0==strcmp(tos_video,""))
        {
          strcpy(tos_video,"none");
        }
        if(0==strcmp(tos_data,""))
        {
          strcpy(tos_data,"none");
        }
        printf("tos_voice:%s\n",tos_voice);
        printf("tos_video:%s\n",tos_video);
        printf("tos_data:%s\n",tos_data);


//	nvram_bufset(RT2860_NVRAM, "WmmCapable",   atoi(wmm_capable)   ? "1" : "0" );

	 //delete old rule
        char TempBuf[32];
        ezplib_get_attr_val("wl_mode_rule", 0, "mode", TempBuf, 32, EZPLIB_USE_CLI); 	
        if(!strcmp(TempBuf,"wisp"))
        {
           system("/etc/qos/tos_classify  delete");
        }
	
	ezplib_replace_attr("wl0_basic_rule", 0, "wme", atoi(wmm_capable)   ? "1" : "0");

        ezplib_replace_attr("tos_classify",0,"voice",tos_voice);
        ezplib_replace_attr("tos_classify",0,"video",tos_video);
        ezplib_replace_attr("tos_classify",0,"data",tos_data);
        ezplib_replace_attr("tos_classify",0,"mode",dscp_mode);
        
    
        system("/etc/qos/qos.sh stop");
        system("/etc/qos/qos.sh restart");
/*        
        if(!strcmp(TempBuf,"wisp"))
        {
           system("ebtables -t nat -F");
           system("ebtables -t nat -X");
        } 
*/
	
//	nvram_commit(RT2860_NVRAM);
	nvram_commit();
	//initInternet();
//	reconfig_wireless(RT2860_NVRAM);	
//	WPSRestart();
	
#if 1//Arthur Chow 2009-03-06		
		setWebMessage(0, NULL);
#endif			
	websRedirect(wp, "local/advance/qos.asp");
#endif	
}	


/* goform/WiFi5GWMM */
static void WiFi5GWMM(webs_t wp, char_t *path, char_t *query)
{
#if 1	
	char_t *apsd_capable,*dls_capable;
        char_t *dscp_mode,*dscp_other;
        char tos_voice[320]={0},tos_video[320]={0},tos_data[320]={0};
        char *dscp_value[64]={"0x0","0x4","0x8","0xc","0x10","0x14","0x18","0x1c","0x20","0x24","0x28","0x2c","0x30","0x34","0x38","0x3c","0x40","0x44","0x48","0x4c","0x50","0x54","0x58","0x5c","0x60","0x64","0x68","0x6c","0x70","0x74","0x78","0x7c","0x80","0x84","0x88","0x8c","0x90","0x94","0x98","0x9c","0xA0","0xA4","0xA8","0xAc","0xB0","0xB4","0xB8","0xBc","0xC0","0xC4","0xC8","0xCc","0xD0","0xD4","0xD8","0xDc","0xE0","0xE4","0xE8","0xEc","0xF0","0xF4","0xF8","0xFc"};
	char_t *wmm_capable = websGetVar(wp, T("wmm_capable"), T(""));	
       char_t wmm_capable_old[32];  
	int i;
       int value;
      



       

	 dscp_mode = websGetVar(wp, T("DSCP_mode_Select"), T(""));
        dscp_other=websGetVar(wp, T("DSCP_Priority_Select_9"), T(""));
        printf("dscp_mode:%s dscp_other:%s#######\n",dscp_mode,dscp_other);
        if(0==strcmp(dscp_mode,"0"))
        {
            strcpy(tos_voice,"0xE0,0xC0,0xB8");
            strcpy(tos_video,"0xA0,0x80");
            strcpy(tos_data,"none");
            ezplib_replace_attr("tos_classify",0,"other","0");
        }
        else
        {
        char *Priority_DSCP =websGetVar(wp, T("DSCP_Priority_Select_1"), T(""));
        char_t *Value_DSCP =websGetVar(wp, T("DSCP_value_1"), T(""));
        value = atoi(Value_DSCP);
        if(0!=strcmp(Value_DSCP,""))
        {
          value = atoi(Value_DSCP);
          dscp_value[value]="";
        }
        if(0==strcmp(Priority_DSCP,"02")&& (0!=strcmp(Value_DSCP,"")))
        {
            sprintf(tos_voice,"0x%x",(value<<2));
        }
        else if(0==strcmp(Priority_DSCP,"01")&&(0!=strcmp(Value_DSCP,"")))
        {
            sprintf(tos_video,"0x%x",(value<<2));
        }
        else if(0==strcmp(Priority_DSCP,"00")&&(0!=strcmp(Value_DSCP,"")))
        {
            sprintf(tos_data,"0x%x",(value<<2));
        }


        Priority_DSCP =websGetVar(wp, T("DSCP_Priority_Select_2"), T(""));
        Value_DSCP =websGetVar(wp, T("DSCP_value_2"), T(""));

        if(0!=strcmp(Value_DSCP,""))
        {
          value = atoi(Value_DSCP);
          dscp_value[value]="";
        }
        if(0==strcmp(Priority_DSCP,"02")&&(0!=strcmp(Value_DSCP,"")))
        {
            if(0 != tos_voice[1])
            {
                sprintf(tos_voice,"%s,0x%x",tos_voice,(value<<2));
            }
            else
            {
                 sprintf(tos_voice,"0x%x",(value<<2));
            }

        }
        else if(0==strcmp(Priority_DSCP,"01")&&(0!=strcmp(Value_DSCP,"")))
        {
             if(0 != tos_video[1])
            {
                sprintf(tos_video,"%s,0x%x",tos_video,(value<<2));
            }
            else
            {
                 sprintf(tos_video,"0x%x",(value<<2));
            }
        }
        else if(0==strcmp(Priority_DSCP,"00")&&(0!=strcmp(Value_DSCP,"")))
        {
             if(0 != tos_data[1])
            {
                sprintf(tos_data,"%s,0x%x",tos_data,(value<<2));
            }
            else
            {
                 sprintf(tos_data,"0x%x",(value<<2));
            }
        }

        Priority_DSCP =websGetVar(wp, T("DSCP_Priority_Select_3"), T(""));
        Value_DSCP =websGetVar(wp, T("DSCP_value_3"), T(""));

        if(0!=strcmp(Value_DSCP,""))
        {
          value = atoi(Value_DSCP);
          dscp_value[value]="";
        }
        if(0==strcmp(Priority_DSCP,"02")&&(0!=strcmp(Value_DSCP,"")))
        {
            if(0 != tos_voice[1])
            {
                sprintf(tos_voice,"%s,0x%x",tos_voice,(value<<2));
            }
            else
            {
                 sprintf(tos_voice,"0x%x",(value<<2));
            }

        }
        else if(0==strcmp(Priority_DSCP,"01")&&(0!=strcmp(Value_DSCP,"")))
        {
             if(0 != tos_video[1])
            {
                sprintf(tos_video,"%s,0x%x",tos_video,(value<<2));
            }
            else
            {
                 sprintf(tos_video,"0x%x",(value<<2));
            }
        }
        else if(0==strcmp(Priority_DSCP,"00")&&(0!=strcmp(Value_DSCP,"")))
        {
             if(0 != tos_data[1])
            {
                sprintf(tos_data,"%s,0x%x",tos_data,(value<<2));
            }
            else
            {
                 sprintf(tos_data,"0x%x",(value<<2));
            }
        }


        Priority_DSCP =websGetVar(wp, T("DSCP_Priority_Select_4"), T(""));
        Value_DSCP =websGetVar(wp, T("DSCP_value_4"), T(""));

        if(0!=strcmp(Value_DSCP,""))
        {
          value = atoi(Value_DSCP);
          dscp_value[value]="";
        }
        if(0==strcmp(Priority_DSCP,"02")&&(0!=strcmp(Value_DSCP,"")))
        {
            if(0 != tos_voice[1])
            {
                sprintf(tos_voice,"%s,0x%x",tos_voice,(value<<2));
            }
            else
            {
                 sprintf(tos_voice,"0x%x",(value<<2));
            }

        }
        else if(0==strcmp(Priority_DSCP,"01")&&(0!=strcmp(Value_DSCP,"")))
        {
             if(0 != tos_video[1])
            {
                sprintf(tos_video,"%s,0x%x",tos_video,(value<<2));
            }
            else
            {
                 sprintf(tos_video,"0x%x",(value<<2));
            }
        }
        else if(0==strcmp(Priority_DSCP,"00")&&(0!=strcmp(Value_DSCP,"")))
        {
             if(0 != tos_data[1])
            {
                sprintf(tos_data,"%s,0x%x",tos_data,(value<<2));
            }
            else
            {
                 sprintf(tos_data,"0x%x",(value<<2));
            }
        }


        Priority_DSCP =websGetVar(wp, T("DSCP_Priority_Select_5"), T(""));
        Value_DSCP =websGetVar(wp, T("DSCP_value_5"), T(""));

        if(0!=strcmp(Value_DSCP,""))
        {
          value = atoi(Value_DSCP);
          dscp_value[value]="";
        }
        if(0==strcmp(Priority_DSCP,"02")&&(0!=strcmp(Value_DSCP,"")))
        {
            if(0 != tos_voice[1])
            {
                sprintf(tos_voice,"%s,0x%x",tos_voice,(value<<2));
            }
            else
            {
                 sprintf(tos_voice,"0x%x",(value<<2));
            }

        }
        else if(0==strcmp(Priority_DSCP,"01")&&(0!=strcmp(Value_DSCP,"")))
        {
             if(0 != tos_video[1])
            {
                sprintf(tos_video,"%s,0x%x",tos_video,(value<<2));
            }
            else
            {
                 sprintf(tos_video,"0x%x",(value<<2));
            }
        }
        else if(0==strcmp(Priority_DSCP,"00")&&(0!=strcmp(Value_DSCP,"")))
        {
             if(0 != tos_data[1])
            {
                sprintf(tos_data,"%s,0x%x",tos_data,(value<<2));
            }
            else
            {
                 sprintf(tos_data,"0x%x",(value<<2));
            }
        }

        Priority_DSCP =websGetVar(wp, T("DSCP_Priority_Select_6"), T(""));
        Value_DSCP =websGetVar(wp, T("DSCP_value_6"), T(""));

        if(0!=strcmp(Value_DSCP,""))
        {
          value = atoi(Value_DSCP);
          dscp_value[value]="";
        }
        if(0==strcmp(Priority_DSCP,"02")&&(0!=strcmp(Value_DSCP,"")))
        {
            if(0 != tos_voice[1])
            {
                sprintf(tos_voice,"%s,0x%x",tos_voice,(value<<2));
            }
            else
            {
                 sprintf(tos_voice,"0x%x",(value<<2));
            }

        }
        else if(0==strcmp(Priority_DSCP,"01")&&(0!=strcmp(Value_DSCP,"")))
        {
             if(0 != tos_video[1])
            {
                sprintf(tos_video,"%s,0x%x",tos_video,(value<<2));
            }
            else
            {
                 sprintf(tos_video,"0x%x",(value<<2));
            }
        }
        else if(0==strcmp(Priority_DSCP,"00")&&(0!=strcmp(Value_DSCP,"")))
        {
             if(0 != tos_data[1])
            {
                sprintf(tos_data,"%s,0x%x",tos_data,(value<<2));
            }
            else
            {
                 sprintf(tos_data,"0x%x",(value<<2));
            }
        }



        Priority_DSCP =websGetVar(wp, T("DSCP_Priority_Select_7"), T(""));
        Value_DSCP =websGetVar(wp, T("DSCP_value_7"), T(""));

        if(0!=strcmp(Value_DSCP,""))
        {
          value = atoi(Value_DSCP);
          dscp_value[value]="";
        }
        if(0==strcmp(Priority_DSCP,"02")&&(0!=strcmp(Value_DSCP,"")))
        {
            if(0 != tos_voice[1])
            {
                sprintf(tos_voice,"%s,0x%x",tos_voice,(value<<2));
            }
            else
            {
                 sprintf(tos_voice,"0x%x",(value<<2));
            }

        }
        else if(0==strcmp(Priority_DSCP,"01")&&(0!=strcmp(Value_DSCP,"")))
        {
             if(0 != tos_video[1])
            {
                sprintf(tos_video,"%s,0x%x",tos_video,(value<<2));
            }
            else
            {
                 sprintf(tos_video,"0x%x",(value<<2));
            }
        }
        else if(0==strcmp(Priority_DSCP,"00")&&(0!=strcmp(Value_DSCP,"")))
        {
             if(0 != tos_data[1])
            {
                sprintf(tos_data,"%s,0x%x",tos_data,(value<<2));
            }
            else
            {
                 sprintf(tos_data,"0x%x",(value<<2));
            }
        }

        Priority_DSCP =websGetVar(wp, T("DSCP_Priority_Select_8"), T(""));
        Value_DSCP =websGetVar(wp, T("DSCP_value_8"), T(""));
       
        if(0 !=strcmp(Value_DSCP,""))
        {
          value = atoi(Value_DSCP);
          dscp_value[value]="";
        }
        if(0==strcmp(Priority_DSCP,"02")&&(0!=strcmp(Value_DSCP,"")))
        {
            if(0 != tos_voice[1])
            {
                sprintf(tos_voice,"%s,0x%x",tos_voice,(value<<2));
            }
            else
            {
                 sprintf(tos_voice,"0x%x",(value<<2));
            }

        }
        else if(0==strcmp(Priority_DSCP,"01")&&(0!=strcmp(Value_DSCP,"")))
        {
             if(0 != tos_video[1])
            {
                sprintf(tos_video,"%s,0x%x",tos_video,(value<<2));
            }
            else
            {
                 sprintf(tos_video,"0x%x",(value<<2));
            }
        }
        else if(0==strcmp(Priority_DSCP,"00")&&(0!=strcmp(Value_DSCP,"")))
        {
             if(0 != tos_data[1])
            {
                sprintf(tos_data,"%s,0x%x",tos_data,(value<<2));
            }
            else
            {
                 sprintf(tos_data,"0x%x",(value<<2));
            }
        }

        Priority_DSCP =websGetVar(wp, T("DSCP_Priority_Select_9"), T(""));
        if(0==strcmp(Priority_DSCP,"02"))
        {
             for(i=0;i<64;i++)
             {
                 for(i=0;i<64;i++)
                 {
                  if(dscp_value[i]!="")
                  {
                    if(i==0)
                    {
                        sprintf(tos_voice,"%s,",dscp_value[i]);
                    }
                    else if(i==63)
                    {
                        sprintf(tos_voice,"%s%s",tos_voice,dscp_value[i]);
                    }
                    else
                    {
                        sprintf(tos_voice,"%s%s,",tos_voice,dscp_value[i]);
                    }
                  }
                 }
             }
          ezplib_replace_attr("tos_classify",0,"other","2");

        }
        else if(0==strcmp(Priority_DSCP,"01"))
        {
             for(i=0;i<64;i++)
             {
               if(dscp_value[i]!="")
               {
                 if(i==0)
                 {
                    sprintf(tos_video,"%s,",dscp_value[i]);
                 }
                 else if(i==63)
                 {
                    sprintf(tos_video,"%s%s",tos_video,dscp_value[i]);
                 }
                 else
                 {
                    sprintf(tos_video,"%s%s,",tos_video,dscp_value[i]);
                 }
               }
             }
         ezplib_replace_attr("tos_classify",0,"other","1");
        }
        else if(0==strcmp(Priority_DSCP,"00"))
        {
             for(i=0;i<64;i++)
             {
               if(dscp_value[i]!="")
               {
                 if(i==0)
                 {
                    sprintf(tos_data,"%s,",dscp_value[i]);
                 }
                 else if(i==63)
                 {
                    sprintf(tos_data,"%s%s",tos_data,dscp_value[i]);
                 }
                 else
                 {
                    sprintf(tos_data,"%s%s,",tos_data,dscp_value[i]);
                 }
              }
             }
         memset(tos_data,0,320);
         strcpy(tos_data,"none");    

         ezplib_replace_attr("tos_classify",0,"other","0");
        }

        }
        if(0==strcmp(tos_voice,""))
        {
          strcpy(tos_voice,"none");
        }
        if(0==strcmp(tos_video,""))
        {
          strcpy(tos_video,"none");
        }
        if(0==strcmp(tos_data,""))
        {
          strcpy(tos_data,"none");
        }

        printf("tos_voice:%s\n",tos_voice);
        printf("tos_video:%s\n",tos_video);
        printf("tos_data:%s\n",tos_data);
//      nvram_bufset(RT2860_NVRAM, "WmmCapable",   atoi(wmm_capable)   ? "1" : "0" );
        //delete old rule
        char TempBuf[32];
        ezplib_get_attr_val("wl_mode_rule", 0, "mode", TempBuf, 32, EZPLIB_USE_CLI);
        if(!strcmp(TempBuf,"wisp"))
        {
           system("/etc/qos/tos_classify  delete");
        }
        ezplib_replace_attr("wl1_basic_rule", 0, "wme", atoi(wmm_capable)   ? "1" : "0");
        ezplib_replace_attr("tos_classify",0,"voice",tos_voice);
        ezplib_replace_attr("tos_classify",0,"video",tos_video);
        ezplib_replace_attr("tos_classify",0,"data",tos_data);
        ezplib_replace_attr("tos_classify",0,"mode",dscp_mode);
        
        system("/etc/qos/tos_classify  delete");
        system("/etc/qos/qos.sh stop");
        system("/etc/qos/qos.sh restart");
/*        
        if(!strcmp(TempBuf,"wisp"))
        {
           system("ebtables -t nat -F");
           system("ebtables -t nat -X");
        } 
*/

	nvram_commit();
	
#if 1//Arthur Chow 2009-03-06		
		setWebMessage(0, NULL);
#endif			
	websRedirect(wp, "local/advance/qos_5g.asp");
#endif	
}	

/* goform/wifischeduler */
static void wifischeduler(webs_t wp, char_t *path, char_t *query)
{
	char *scheduling_action_0,*scheduling_start_hour_0,*scheduling_start_min_0,*scheduling_end_hour_0,*scheduling_end_min_0,
	     *scheduling_action_1,*scheduling_start_hour_1,*scheduling_start_min_1,*scheduling_end_hour_1,*scheduling_end_min_1,
	     *scheduling_action_2,*scheduling_start_hour_2,*scheduling_start_min_2,*scheduling_end_hour_2,*scheduling_end_min_2,
	     *scheduling_action_3,*scheduling_start_hour_3,*scheduling_start_min_3,*scheduling_end_hour_3,*scheduling_end_min_3,
	     *scheduling_action_4,*scheduling_start_hour_4,*scheduling_start_min_4,*scheduling_end_hour_4,*scheduling_end_min_4,
	     *scheduling_action_5,*scheduling_start_hour_5,*scheduling_start_min_5,*scheduling_end_hour_5,*scheduling_end_min_5,
	     *scheduling_action_6,*scheduling_start_hour_6,*scheduling_start_min_6,*scheduling_end_hour_6,*scheduling_end_min_6,
	     *scheduling_action_7,*scheduling_start_hour_7,*scheduling_start_min_7,*scheduling_end_hour_7,*scheduling_end_min_7;
	     
	char *ScheduleEnable = websGetVar(wp, T("WLAN_Schedule_Enable"), T("0"));
	char *Scheduling_day_Active_0 = websGetVar(wp, T("WLAN_Scheduling_day_Active_0"), T("0"));
	char *Scheduling_day_Active_1 = websGetVar(wp, T("WLAN_Scheduling_day_Active_1"), T("0"));
	char *Scheduling_day_Active_2 = websGetVar(wp, T("WLAN_Scheduling_day_Active_2"), T("0"));
	char *Scheduling_day_Active_3 = websGetVar(wp, T("WLAN_Scheduling_day_Active_3"), T("0"));
	char *Scheduling_day_Active_4 = websGetVar(wp, T("WLAN_Scheduling_day_Active_4"), T("0"));
	char *Scheduling_day_Active_5 = websGetVar(wp, T("WLAN_Scheduling_day_Active_5"), T("0"));
	char *Scheduling_day_Active_6 = websGetVar(wp, T("WLAN_Scheduling_day_Active_6"), T("0"));
	char *Scheduling_day_Active_7 = websGetVar(wp, T("WLAN_Scheduling_day_Active_7"), T("0"));

	//system("/etc/rc.common /etc/init.d/sch stop");
	
	// someone use malform page.....
	if(!ScheduleEnable || !Scheduling_day_Active_0 || !Scheduling_day_Active_1 || !Scheduling_day_Active_2 ||
	   !Scheduling_day_Active_3 || !Scheduling_day_Active_4 || !Scheduling_day_Active_5 || !Scheduling_day_Active_6 || 
	   !Scheduling_day_Active_7)
		return;

	// WiFi Scheduler enable/disable
	nvram_set("sched_enable", ScheduleEnable);
	nvram_set("sched_enable_bak", ScheduleEnable);

	// action 0
	scheduling_action_0 = websGetVar(wp, T("WLAN_scheduling_action_0"), T(""));
	ezplib_replace_attr("sched_rule", 0, "wlanstatus", scheduling_action_0);
	
	//if (!strcmp(Scheduling_day_Active_0, "1")){
	//	ezplib_replace_attr("sched_rule", 0, "wkday", "eve");
	//}else{
	//	ezplib_replace_attr("sched_rule", 0, "wkday", "");
	//}	
	ezplib_replace_attr("sched_rule", 0, "enable", Scheduling_day_Active_0);

	scheduling_start_hour_0 = websGetVar(wp, T("WLAN_scheduling_start_hour_0"), T(""));
	ezplib_replace_attr("sched_rule", 0, "beghr", scheduling_start_hour_0);
		
	scheduling_start_min_0 = websGetVar(wp, T("WLAN_scheduling_start_min_0"), T(""));
	if (!strcmp(scheduling_start_min_0, "1")){
		ezplib_replace_attr("sched_rule", 0, "begmin", "30");
	}else{
		ezplib_replace_attr("sched_rule", 0, "begmin", "00");	
	}
		
	scheduling_end_hour_0 = websGetVar(wp, T("WLAN_scheduling_end_hour_0"), T(""));
	ezplib_replace_attr("sched_rule", 0, "endhr", scheduling_end_hour_0);	
			
	scheduling_end_min_0 = websGetVar(wp, T("WLAN_scheduling_end_min_0"), T(""));
	if (!strcmp(scheduling_end_min_0, "1")){
		ezplib_replace_attr("sched_rule", 0, "endmin", "30");
	}else{
		ezplib_replace_attr("sched_rule", 0, "endmin", "00");	
	}

	// action 7	
	scheduling_action_7 = websGetVar(wp, T("WLAN_scheduling_action_7"), T(""));
	ezplib_replace_attr("sched_rule", 1, "wlanstatus", scheduling_action_7);
	
	//if (!strcmp(Scheduling_day_Active_7, "1")){
	//	ezplib_replace_attr("sched_rule", 1, "wkday", "sun");
	//}else{
	//	ezplib_replace_attr("sched_rule", 1, "wkday", "");
	//}
	ezplib_replace_attr("sched_rule", 1, "enable", Scheduling_day_Active_7);	

	scheduling_start_hour_7 = websGetVar(wp, T("WLAN_scheduling_start_hour_7"), T(""));
	ezplib_replace_attr("sched_rule", 1, "beghr", scheduling_start_hour_7);
		
	scheduling_start_min_7 = websGetVar(wp, T("WLAN_scheduling_start_min_7"), T(""));
	if (!strcmp(scheduling_start_min_7, "1")){
		ezplib_replace_attr("sched_rule", 1, "begmin", "30");
	}else{
		ezplib_replace_attr("sched_rule", 1, "begmin", "00");	
	}
		
	scheduling_end_hour_7 = websGetVar(wp, T("WLAN_scheduling_end_hour_7"), T(""));
	ezplib_replace_attr("sched_rule", 1, "endhr", scheduling_end_hour_7);	
			
	scheduling_end_min_7 = websGetVar(wp, T("WLAN_scheduling_end_min_7"), T(""));
	if (!strcmp(scheduling_end_min_7, "1")){
		ezplib_replace_attr("sched_rule", 1, "endmin", "30");
	}else{
		ezplib_replace_attr("sched_rule", 1, "endmin", "00");	
	}


	// action 1
	scheduling_action_1 = websGetVar(wp, T("WLAN_scheduling_action_1"), T(""));
	ezplib_replace_attr("sched_rule", 2, "wlanstatus", scheduling_action_1);
	
	//if (!strcmp(Scheduling_day_Active_1, "1")){
	//	ezplib_replace_attr("sched_rule", 2, "wkday", "mon");
	//}else{
	//	ezplib_replace_attr("sched_rule", 2, "wkday", "");
	//}
	ezplib_replace_attr("sched_rule", 2, "enable", Scheduling_day_Active_1);

	scheduling_start_hour_1 = websGetVar(wp, T("WLAN_scheduling_start_hour_1"), T(""));
	ezplib_replace_attr("sched_rule", 2, "beghr", scheduling_start_hour_1);
		
	scheduling_start_min_1 = websGetVar(wp, T("WLAN_scheduling_start_min_1"), T(""));
	if (!strcmp(scheduling_start_min_1, "1")){
		ezplib_replace_attr("sched_rule", 2, "begmin", "30");
	}else{
		ezplib_replace_attr("sched_rule", 2, "begmin", "00");	
	}
		
	scheduling_end_hour_1 = websGetVar(wp, T("WLAN_scheduling_end_hour_1"), T(""));
	ezplib_replace_attr("sched_rule", 2, "endhr", scheduling_end_hour_1);	
			
	scheduling_end_min_1 = websGetVar(wp, T("WLAN_scheduling_end_min_1"), T(""));
	if (!strcmp(scheduling_end_min_1, "1")){
		ezplib_replace_attr("sched_rule", 2, "endmin", "30");
	}else{
		ezplib_replace_attr("sched_rule", 2, "endmin", "00");	
	}

	// action 2		
	scheduling_action_2 = websGetVar(wp, T("WLAN_scheduling_action_2"), T(""));
	ezplib_replace_attr("sched_rule", 3, "wlanstatus", scheduling_action_2);
	
	//if (!strcmp(Scheduling_day_Active_2, "1")){
	//	ezplib_replace_attr("sched_rule", 3, "wkday", "tue");
	//}else{
	//	ezplib_replace_attr("sched_rule", 3, "wkday", "");
	//}	
	ezplib_replace_attr("sched_rule", 3, "enable", Scheduling_day_Active_2);

	scheduling_start_hour_2 = websGetVar(wp, T("WLAN_scheduling_start_hour_2"), T(""));
	ezplib_replace_attr("sched_rule", 3, "beghr", scheduling_start_hour_2);
		
	scheduling_start_min_2 = websGetVar(wp, T("WLAN_scheduling_start_min_2"), T(""));
	if (!strcmp(scheduling_start_min_2, "1")){
		ezplib_replace_attr("sched_rule", 3, "begmin", "30");
	}else{
		ezplib_replace_attr("sched_rule", 3, "begmin", "00");	
	}
		
	scheduling_end_hour_2 = websGetVar(wp, T("WLAN_scheduling_end_hour_2"), T(""));
	ezplib_replace_attr("sched_rule", 3, "endhr", scheduling_end_hour_2);	
			
	scheduling_end_min_2 = websGetVar(wp, T("WLAN_scheduling_end_min_2"), T(""));
	if (!strcmp(scheduling_end_min_2, "1")){
		ezplib_replace_attr("sched_rule", 3, "endmin", "30");
	}else{
		ezplib_replace_attr("sched_rule", 3, "endmin", "00");	
	}

	// action 3		
	scheduling_action_3 = websGetVar(wp, T("WLAN_scheduling_action_3"), T(""));
	ezplib_replace_attr("sched_rule", 4, "wlanstatus", scheduling_action_3);
	
	//if (!strcmp(Scheduling_day_Active_3, "1")){
	//	ezplib_replace_attr("sched_rule", 4, "wkday", "wed");
	//}else{
	//	ezplib_replace_attr("sched_rule", 4, "wkday", "");
	//}	
	ezplib_replace_attr("sched_rule", 4, "enable", Scheduling_day_Active_3);

	scheduling_start_hour_3 = websGetVar(wp, T("WLAN_scheduling_start_hour_3"), T(""));
	ezplib_replace_attr("sched_rule", 4, "beghr", scheduling_start_hour_3);
		
	scheduling_start_min_3 = websGetVar(wp, T("WLAN_scheduling_start_min_3"), T(""));
	if (!strcmp(scheduling_start_min_3, "1")){
		ezplib_replace_attr("sched_rule", 4, "begmin", "30");
	}else{
		ezplib_replace_attr("sched_rule", 4, "begmin", "00");	
	}
		
	scheduling_end_hour_3 = websGetVar(wp, T("WLAN_scheduling_end_hour_3"), T(""));
	ezplib_replace_attr("sched_rule", 4, "endhr", scheduling_end_hour_3);	
			
	scheduling_end_min_3 = websGetVar(wp, T("WLAN_scheduling_end_min_3"), T(""));
	if (!strcmp(scheduling_end_min_3, "1")){
		ezplib_replace_attr("sched_rule", 4, "endmin", "30");
	}else{
		ezplib_replace_attr("sched_rule", 4, "endmin", "00");	
	}

	// action 4		
	scheduling_action_4 = websGetVar(wp, T("WLAN_scheduling_action_4"), T(""));
	ezplib_replace_attr("sched_rule", 5, "wlanstatus", scheduling_action_4);
	
	//if (!strcmp(Scheduling_day_Active_4, "1")){
	//	ezplib_replace_attr("sched_rule", 5, "wkday", "thu");
	//}else{
	//	ezplib_replace_attr("sched_rule", 5, "wkday", "");
	//}
	ezplib_replace_attr("sched_rule", 5, "enable", Scheduling_day_Active_4);	

	scheduling_start_hour_4 = websGetVar(wp, T("WLAN_scheduling_start_hour_4"), T(""));
	ezplib_replace_attr("sched_rule", 5, "beghr", scheduling_start_hour_4);
		
	scheduling_start_min_4 = websGetVar(wp, T("WLAN_scheduling_start_min_4"), T(""));
	if (!strcmp(scheduling_start_min_4, "1")){
		ezplib_replace_attr("sched_rule", 5, "begmin", "30");
	}else{
		ezplib_replace_attr("sched_rule", 5, "begmin", "00");	
	}
		
	scheduling_end_hour_4 = websGetVar(wp, T("WLAN_scheduling_end_hour_4"), T(""));
	ezplib_replace_attr("sched_rule", 5, "endhr", scheduling_end_hour_4);	
			
	scheduling_end_min_4 = websGetVar(wp, T("WLAN_scheduling_end_min_4"), T(""));
	if (!strcmp(scheduling_end_min_4, "1")){
		ezplib_replace_attr("sched_rule", 5, "endmin", "30");
	}else{
		ezplib_replace_attr("sched_rule", 5, "endmin", "00");	
	}


	// action 5		
	scheduling_action_5 = websGetVar(wp, T("WLAN_scheduling_action_5"), T(""));
	ezplib_replace_attr("sched_rule", 6, "wlanstatus", scheduling_action_5);
	
	//if (!strcmp(Scheduling_day_Active_5, "1")){
	//	ezplib_replace_attr("sched_rule", 6, "wkday", "fri");
	//}else{
	//	ezplib_replace_attr("sched_rule", 6, "wkday", "");
	//}	
	ezplib_replace_attr("sched_rule", 6, "enable", Scheduling_day_Active_5);

	scheduling_start_hour_5 = websGetVar(wp, T("WLAN_scheduling_start_hour_5"), T(""));
	ezplib_replace_attr("sched_rule", 6, "beghr", scheduling_start_hour_5);
		
	scheduling_start_min_5 = websGetVar(wp, T("WLAN_scheduling_start_min_5"), T(""));
	if (!strcmp(scheduling_start_min_5, "1")){
		ezplib_replace_attr("sched_rule", 6, "begmin", "30");
	}else{
		ezplib_replace_attr("sched_rule", 6, "begmin", "00");	
	}
		
	scheduling_end_hour_5 = websGetVar(wp, T("WLAN_scheduling_end_hour_5"), T(""));
	ezplib_replace_attr("sched_rule", 6, "endhr", scheduling_end_hour_5);	
			
	scheduling_end_min_5 = websGetVar(wp, T("WLAN_scheduling_end_min_5"), T(""));
	if (!strcmp(scheduling_end_min_5, "1")){
		ezplib_replace_attr("sched_rule", 6, "endmin", "30");
	}else{
		ezplib_replace_attr("sched_rule", 6, "endmin", "00");	
	}

	// action 6		
	scheduling_action_6 = websGetVar(wp, T("WLAN_scheduling_action_6"), T(""));
	ezplib_replace_attr("sched_rule", 7, "wlanstatus", scheduling_action_6);
	
	//if (!strcmp(Scheduling_day_Active_6, "1")){
	//	ezplib_replace_attr("sched_rule", 7, "wkday", "sat");
	//}else{
	//	ezplib_replace_attr("sched_rule", 7, "wkday", "");
	//}
	ezplib_replace_attr("sched_rule", 7, "enable", Scheduling_day_Active_6);

	scheduling_start_hour_6 = websGetVar(wp, T("WLAN_scheduling_start_hour_6"), T(""));
	ezplib_replace_attr("sched_rule", 7, "beghr", scheduling_start_hour_6);
		
	scheduling_start_min_6 = websGetVar(wp, T("WLAN_scheduling_start_min_6"), T(""));
	if (!strcmp(scheduling_start_min_6, "1")){
		ezplib_replace_attr("sched_rule", 7, "begmin", "30");
	}else{
		ezplib_replace_attr("sched_rule", 7, "begmin", "00");	
	}
		
	scheduling_end_hour_6 = websGetVar(wp, T("WLAN_scheduling_end_hour_6"), T(""));
	ezplib_replace_attr("sched_rule", 7, "endhr", scheduling_end_hour_6);	
			
	scheduling_end_min_6 = websGetVar(wp, T("WLAN_scheduling_end_min_6"), T(""));
	if (!strcmp(scheduling_end_min_6, "1")){
		ezplib_replace_attr("sched_rule", 7, "endmin", "30");
	}else{
		ezplib_replace_attr("sched_rule", 7, "endmin", "00");	
	}

	//system("/etc/rc.common /etc/init.d/sch start");
	system("/sbin/ezp-scheduler");
	nvram_commit();
	
// Tommy, set daemon scheduling, 2009/10/21 04:05
	//openlog("scheduling", LOG_PID, LOG_DAEMON);

// Tommy, restore the original daemon, 2009/10/21 04:05
	//openlog("webmanagement", LOG_PID, LOG_DAEMON);
        
#if 1//Arthur Chow 2009-03-06		
		setWebMessage(0, NULL);
#endif		
	websRedirect(wp, "local/advance/scheduling.asp");
}	  


/* goform/wifi5gscheduler */
static void wifi5gscheduler(webs_t wp, char_t *path, char_t *query)
{
	char *scheduling_action_0,*scheduling_start_hour_0,*scheduling_start_min_0,*scheduling_end_hour_0,*scheduling_end_min_0,
	     *scheduling_action_1,*scheduling_start_hour_1,*scheduling_start_min_1,*scheduling_end_hour_1,*scheduling_end_min_1,
	     *scheduling_action_2,*scheduling_start_hour_2,*scheduling_start_min_2,*scheduling_end_hour_2,*scheduling_end_min_2,
	     *scheduling_action_3,*scheduling_start_hour_3,*scheduling_start_min_3,*scheduling_end_hour_3,*scheduling_end_min_3,
	     *scheduling_action_4,*scheduling_start_hour_4,*scheduling_start_min_4,*scheduling_end_hour_4,*scheduling_end_min_4,
	     *scheduling_action_5,*scheduling_start_hour_5,*scheduling_start_min_5,*scheduling_end_hour_5,*scheduling_end_min_5,
	     *scheduling_action_6,*scheduling_start_hour_6,*scheduling_start_min_6,*scheduling_end_hour_6,*scheduling_end_min_6,
	     *scheduling_action_7,*scheduling_start_hour_7,*scheduling_start_min_7,*scheduling_end_hour_7,*scheduling_end_min_7;
	     
	char *ScheduleEnable = websGetVar(wp, T("WLAN_Schedule_Enable"), T("0"));
	char *Scheduling_day_Active_0 = websGetVar(wp, T("WLAN_Scheduling_day_Active_0"), T("0"));
	char *Scheduling_day_Active_1 = websGetVar(wp, T("WLAN_Scheduling_day_Active_1"), T("0"));
	char *Scheduling_day_Active_2 = websGetVar(wp, T("WLAN_Scheduling_day_Active_2"), T("0"));
	char *Scheduling_day_Active_3 = websGetVar(wp, T("WLAN_Scheduling_day_Active_3"), T("0"));
	char *Scheduling_day_Active_4 = websGetVar(wp, T("WLAN_Scheduling_day_Active_4"), T("0"));
	char *Scheduling_day_Active_5 = websGetVar(wp, T("WLAN_Scheduling_day_Active_5"), T("0"));
	char *Scheduling_day_Active_6 = websGetVar(wp, T("WLAN_Scheduling_day_Active_6"), T("0"));
	char *Scheduling_day_Active_7 = websGetVar(wp, T("WLAN_Scheduling_day_Active_7"), T("0"));

	if(!ScheduleEnable || !Scheduling_day_Active_0 || !Scheduling_day_Active_1 || !Scheduling_day_Active_2 ||
	   !Scheduling_day_Active_3 || !Scheduling_day_Active_4 || !Scheduling_day_Active_5 || !Scheduling_day_Active_6 || 
	   !Scheduling_day_Active_7)
		return;

	// WiFi Scheduler enable/disable
	nvram_set("sched1_enable", ScheduleEnable);
	nvram_set("sched1_enable_bak", ScheduleEnable);

	// action 0
	scheduling_action_0 = websGetVar(wp, T("WLAN_scheduling_action_0"), T(""));
	ezplib_replace_attr("sched1_rule", 0, "wlanstatus", scheduling_action_0);
	
	ezplib_replace_attr("sched1_rule", 0, "enable", Scheduling_day_Active_0);

	scheduling_start_hour_0 = websGetVar(wp, T("WLAN_scheduling_start_hour_0"), T(""));
	ezplib_replace_attr("sched1_rule", 0, "beghr", scheduling_start_hour_0);
		
	scheduling_start_min_0 = websGetVar(wp, T("WLAN_scheduling_start_min_0"), T(""));
	if (!strcmp(scheduling_start_min_0, "1")){
		ezplib_replace_attr("sched1_rule", 0, "begmin", "30");
	}else{
		ezplib_replace_attr("sched1_rule", 0, "begmin", "00");	
	}
		
	scheduling_end_hour_0 = websGetVar(wp, T("WLAN_scheduling_end_hour_0"), T(""));
	ezplib_replace_attr("sched1_rule", 0, "endhr", scheduling_end_hour_0);	
			
	scheduling_end_min_0 = websGetVar(wp, T("WLAN_scheduling_end_min_0"), T(""));
	if (!strcmp(scheduling_end_min_0, "1")){
		ezplib_replace_attr("sched1_rule", 0, "endmin", "30");
	}else{
		ezplib_replace_attr("sched1_rule", 0, "endmin", "00");	
	}

	// action 7	
	scheduling_action_7 = websGetVar(wp, T("WLAN_scheduling_action_7"), T(""));
	ezplib_replace_attr("sched1_rule", 1, "wlanstatus", scheduling_action_7);
	
	ezplib_replace_attr("sched1_rule", 1, "enable", Scheduling_day_Active_7);	

	scheduling_start_hour_7 = websGetVar(wp, T("WLAN_scheduling_start_hour_7"), T(""));
	ezplib_replace_attr("sched1_rule", 1, "beghr", scheduling_start_hour_7);
		
	scheduling_start_min_7 = websGetVar(wp, T("WLAN_scheduling_start_min_7"), T(""));
	if (!strcmp(scheduling_start_min_7, "1")){
		ezplib_replace_attr("sched1_rule", 1, "begmin", "30");
	}else{
		ezplib_replace_attr("sched1_rule", 1, "begmin", "00");	
	}
		
	scheduling_end_hour_7 = websGetVar(wp, T("WLAN_scheduling_end_hour_7"), T(""));
	ezplib_replace_attr("sched1_rule", 1, "endhr", scheduling_end_hour_7);	
			
	scheduling_end_min_7 = websGetVar(wp, T("WLAN_scheduling_end_min_7"), T(""));
	if (!strcmp(scheduling_end_min_7, "1")){
		ezplib_replace_attr("sched1_rule", 1, "endmin", "30");
	}else{
		ezplib_replace_attr("sched1_rule", 1, "endmin", "00");	
	}


	// action 1
	scheduling_action_1 = websGetVar(wp, T("WLAN_scheduling_action_1"), T(""));
	ezplib_replace_attr("sched1_rule", 2, "wlanstatus", scheduling_action_1);
	
	ezplib_replace_attr("sched1_rule", 2, "enable", Scheduling_day_Active_1);

	scheduling_start_hour_1 = websGetVar(wp, T("WLAN_scheduling_start_hour_1"), T(""));
	ezplib_replace_attr("sched1_rule", 2, "beghr", scheduling_start_hour_1);
		
	scheduling_start_min_1 = websGetVar(wp, T("WLAN_scheduling_start_min_1"), T(""));
	if (!strcmp(scheduling_start_min_1, "1")){
		ezplib_replace_attr("sched1_rule", 2, "begmin", "30");
	}else{
		ezplib_replace_attr("sched1_rule", 2, "begmin", "00");	
	}
		
	scheduling_end_hour_1 = websGetVar(wp, T("WLAN_scheduling_end_hour_1"), T(""));
	ezplib_replace_attr("sched1_rule", 2, "endhr", scheduling_end_hour_1);	
			
	scheduling_end_min_1 = websGetVar(wp, T("WLAN_scheduling_end_min_1"), T(""));
	if (!strcmp(scheduling_end_min_1, "1")){
		ezplib_replace_attr("sched1_rule", 2, "endmin", "30");
	}else{
		ezplib_replace_attr("sched1_rule", 2, "endmin", "00");	
	}

	// action 2		
	scheduling_action_2 = websGetVar(wp, T("WLAN_scheduling_action_2"), T(""));
	ezplib_replace_attr("sched1_rule", 3, "wlanstatus", scheduling_action_2);
	
	ezplib_replace_attr("sched1_rule", 3, "enable", Scheduling_day_Active_2);

	scheduling_start_hour_2 = websGetVar(wp, T("WLAN_scheduling_start_hour_2"), T(""));
	ezplib_replace_attr("sched1_rule", 3, "beghr", scheduling_start_hour_2);
		
	scheduling_start_min_2 = websGetVar(wp, T("WLAN_scheduling_start_min_2"), T(""));
	if (!strcmp(scheduling_start_min_2, "1")){
		ezplib_replace_attr("sched1_rule", 3, "begmin", "30");
	}else{
		ezplib_replace_attr("sched1_rule", 3, "begmin", "00");	
	}
		
	scheduling_end_hour_2 = websGetVar(wp, T("WLAN_scheduling_end_hour_2"), T(""));
	ezplib_replace_attr("sched1_rule", 3, "endhr", scheduling_end_hour_2);	
			
	scheduling_end_min_2 = websGetVar(wp, T("WLAN_scheduling_end_min_2"), T(""));
	if (!strcmp(scheduling_end_min_2, "1")){
		ezplib_replace_attr("sched1_rule", 3, "endmin", "30");
	}else{
		ezplib_replace_attr("sched1_rule", 3, "endmin", "00");	
	}

	// action 3		
	scheduling_action_3 = websGetVar(wp, T("WLAN_scheduling_action_3"), T(""));
	ezplib_replace_attr("sched1_rule", 4, "wlanstatus", scheduling_action_3);
	
	ezplib_replace_attr("sched1_rule", 4, "enable", Scheduling_day_Active_3);

	scheduling_start_hour_3 = websGetVar(wp, T("WLAN_scheduling_start_hour_3"), T(""));
	ezplib_replace_attr("sched1_rule", 4, "beghr", scheduling_start_hour_3);
		
	scheduling_start_min_3 = websGetVar(wp, T("WLAN_scheduling_start_min_3"), T(""));
	if (!strcmp(scheduling_start_min_3, "1")){
		ezplib_replace_attr("sched1_rule", 4, "begmin", "30");
	}else{
		ezplib_replace_attr("sched1_rule", 4, "begmin", "00");	
	}
		
	scheduling_end_hour_3 = websGetVar(wp, T("WLAN_scheduling_end_hour_3"), T(""));
	ezplib_replace_attr("sched1_rule", 4, "endhr", scheduling_end_hour_3);	
			
	scheduling_end_min_3 = websGetVar(wp, T("WLAN_scheduling_end_min_3"), T(""));
	if (!strcmp(scheduling_end_min_3, "1")){
		ezplib_replace_attr("sched1_rule", 4, "endmin", "30");
	}else{
		ezplib_replace_attr("sched1_rule", 4, "endmin", "00");	
	}

	// action 4		
	scheduling_action_4 = websGetVar(wp, T("WLAN_scheduling_action_4"), T(""));
	ezplib_replace_attr("sched1_rule", 5, "wlanstatus", scheduling_action_4);
	
	ezplib_replace_attr("sched1_rule", 5, "enable", Scheduling_day_Active_4);	

	scheduling_start_hour_4 = websGetVar(wp, T("WLAN_scheduling_start_hour_4"), T(""));
	ezplib_replace_attr("sched1_rule", 5, "beghr", scheduling_start_hour_4);
		
	scheduling_start_min_4 = websGetVar(wp, T("WLAN_scheduling_start_min_4"), T(""));
	if (!strcmp(scheduling_start_min_4, "1")){
		ezplib_replace_attr("sched1_rule", 5, "begmin", "30");
	}else{
		ezplib_replace_attr("sched1_rule", 5, "begmin", "00");	
	}
		
	scheduling_end_hour_4 = websGetVar(wp, T("WLAN_scheduling_end_hour_4"), T(""));
	ezplib_replace_attr("sched1_rule", 5, "endhr", scheduling_end_hour_4);	
			
	scheduling_end_min_4 = websGetVar(wp, T("WLAN_scheduling_end_min_4"), T(""));
	if (!strcmp(scheduling_end_min_4, "1")){
		ezplib_replace_attr("sched1_rule", 5, "endmin", "30");
	}else{
		ezplib_replace_attr("sched1_rule", 5, "endmin", "00");	
	}


	// action 5		
	scheduling_action_5 = websGetVar(wp, T("WLAN_scheduling_action_5"), T(""));
	ezplib_replace_attr("sched1_rule", 6, "wlanstatus", scheduling_action_5);
	
	ezplib_replace_attr("sched1_rule", 6, "enable", Scheduling_day_Active_5);

	scheduling_start_hour_5 = websGetVar(wp, T("WLAN_scheduling_start_hour_5"), T(""));
	ezplib_replace_attr("sched1_rule", 6, "beghr", scheduling_start_hour_5);
		
	scheduling_start_min_5 = websGetVar(wp, T("WLAN_scheduling_start_min_5"), T(""));
	if (!strcmp(scheduling_start_min_5, "1")){
		ezplib_replace_attr("sched1_rule", 6, "begmin", "30");
	}else{
		ezplib_replace_attr("sched1_rule", 6, "begmin", "00");	
	}
		
	scheduling_end_hour_5 = websGetVar(wp, T("WLAN_scheduling_end_hour_5"), T(""));
	ezplib_replace_attr("sched1_rule", 6, "endhr", scheduling_end_hour_5);	
			
	scheduling_end_min_5 = websGetVar(wp, T("WLAN_scheduling_end_min_5"), T(""));
	if (!strcmp(scheduling_end_min_5, "1")){
		ezplib_replace_attr("sched1_rule", 6, "endmin", "30");
	}else{
		ezplib_replace_attr("sched1_rule", 6, "endmin", "00");	
	}

	// action 6		
	scheduling_action_6 = websGetVar(wp, T("WLAN_scheduling_action_6"), T(""));
	ezplib_replace_attr("sched1_rule", 7, "wlanstatus", scheduling_action_6);
	
	ezplib_replace_attr("sched1_rule", 7, "enable", Scheduling_day_Active_6);

	scheduling_start_hour_6 = websGetVar(wp, T("WLAN_scheduling_start_hour_6"), T(""));
	ezplib_replace_attr("sched1_rule", 7, "beghr", scheduling_start_hour_6);
		
	scheduling_start_min_6 = websGetVar(wp, T("WLAN_scheduling_start_min_6"), T(""));
	if (!strcmp(scheduling_start_min_6, "1")){
		ezplib_replace_attr("sched1_rule", 7, "begmin", "30");
	}else{
		ezplib_replace_attr("sched1_rule", 7, "begmin", "00");	
	}
		
	scheduling_end_hour_6 = websGetVar(wp, T("WLAN_scheduling_end_hour_6"), T(""));
	ezplib_replace_attr("sched1_rule", 7, "endhr", scheduling_end_hour_6);	
			
	scheduling_end_min_6 = websGetVar(wp, T("WLAN_scheduling_end_min_6"), T(""));
	if (!strcmp(scheduling_end_min_6, "1")){
		ezplib_replace_attr("sched1_rule", 7, "endmin", "30");
	}else{
		ezplib_replace_attr("sched1_rule", 7, "endmin", "00");	
	}
       //TODO add /sbin/ezp-scheduler-5g
	system("/sbin/ezp1-scheduler");
	nvram_commit();
	
#if 1//Arthur Chow 2009-03-06		
		setWebMessage(0, NULL);
#endif		
	websRedirect(wp, "local/advance/scheduling_5g.asp");
}	  

/* goform/EZwifischeduler */
static void EZwifischeduler(webs_t wp, char_t *path, char_t *query)
{
	char *scheduling_action_0,*scheduling_start_hour_0,*scheduling_start_min_0,*scheduling_end_hour_0,*scheduling_end_min_0,
	     *scheduling_action_1,*scheduling_start_hour_1,*scheduling_start_min_1,*scheduling_end_hour_1,*scheduling_end_min_1,
	     *scheduling_action_2,*scheduling_start_hour_2,*scheduling_start_min_2,*scheduling_end_hour_2,*scheduling_end_min_2,
	     *scheduling_action_3,*scheduling_start_hour_3,*scheduling_start_min_3,*scheduling_end_hour_3,*scheduling_end_min_3,
	     *scheduling_action_4,*scheduling_start_hour_4,*scheduling_start_min_4,*scheduling_end_hour_4,*scheduling_end_min_4,
	     *scheduling_action_5,*scheduling_start_hour_5,*scheduling_start_min_5,*scheduling_end_hour_5,*scheduling_end_min_5,
	     *scheduling_action_6,*scheduling_start_hour_6,*scheduling_start_min_6,*scheduling_end_hour_6,*scheduling_end_min_6,
	     *scheduling_action_7,*scheduling_start_hour_7,*scheduling_start_min_7,*scheduling_end_hour_7,*scheduling_end_min_7;
	     
	char *Scheduling_day_Active_0 = websGetVar(wp, T("WLAN_Scheduling_day_Active_0"), T("0"));
	char *Scheduling_day_Active_1 = websGetVar(wp, T("WLAN_Scheduling_day_Active_1"), T("0"));
	char *Scheduling_day_Active_2 = websGetVar(wp, T("WLAN_Scheduling_day_Active_2"), T("0"));
	char *Scheduling_day_Active_3 = websGetVar(wp, T("WLAN_Scheduling_day_Active_3"), T("0"));
	char *Scheduling_day_Active_4 = websGetVar(wp, T("WLAN_Scheduling_day_Active_4"), T("0"));
	char *Scheduling_day_Active_5 = websGetVar(wp, T("WLAN_Scheduling_day_Active_5"), T("0"));
	char *Scheduling_day_Active_6 = websGetVar(wp, T("WLAN_Scheduling_day_Active_6"), T("0"));
	char *Scheduling_day_Active_7 = websGetVar(wp, T("WLAN_Scheduling_day_Active_7"), T("0"));
	
	// someone use malform page.....
	if(!Scheduling_day_Active_0 || !Scheduling_day_Active_1 || !Scheduling_day_Active_2 ||
	   !Scheduling_day_Active_3 || !Scheduling_day_Active_4 || !Scheduling_day_Active_5 || !Scheduling_day_Active_6 || 
	   !Scheduling_day_Active_7)
		return;
       char *wlan_mode = websGetVar(wp, T("sel_mode"), T("0"));
           	ezplib_replace_attr("wl_easy_mode_rule", 0, "mode", wlan_mode);
	if (!strcmp(wlan_mode, "0")){		
           	scheduling_action_0 = websGetVar(wp, T("WLAN_scheduling_action_0"), T(""));
           	ezplib_replace_attr("sched_rule", 0, "wlanstatus", scheduling_action_0);
           	
           	ezplib_replace_attr("sched_rule", 0, "enable", Scheduling_day_Active_0);
           
           	scheduling_start_hour_0 = websGetVar(wp, T("WLAN_scheduling_start_hour_0"), T(""));
           	ezplib_replace_attr("sched_rule", 0, "beghr", scheduling_start_hour_0);
           		
           	scheduling_start_min_0 = websGetVar(wp, T("WLAN_scheduling_start_min_0"), T(""));
           	if (!strcmp(scheduling_start_min_0, "1")){
           		ezplib_replace_attr("sched_rule", 0, "begmin", "30");
           	}else{
           		ezplib_replace_attr("sched_rule", 0, "begmin", "00");	
           	}
           		
           	scheduling_end_hour_0 = websGetVar(wp, T("WLAN_scheduling_end_hour_0"), T(""));
           	ezplib_replace_attr("sched_rule", 0, "endhr", scheduling_end_hour_0);	
           			
           	scheduling_end_min_0 = websGetVar(wp, T("WLAN_scheduling_end_min_0"), T(""));
           	if (!strcmp(scheduling_end_min_0, "1")){
           		ezplib_replace_attr("sched_rule", 0, "endmin", "30");
           	}else{
           		ezplib_replace_attr("sched_rule", 0, "endmin", "00");	
           	}
           
           	// action 7	
           	scheduling_action_7 = websGetVar(wp, T("WLAN_scheduling_action_7"), T(""));
           	ezplib_replace_attr("sched_rule", 1, "wlanstatus", scheduling_action_7);
           	
           	ezplib_replace_attr("sched_rule", 1, "enable", Scheduling_day_Active_7);	
           
           	scheduling_start_hour_7 = websGetVar(wp, T("WLAN_scheduling_start_hour_7"), T(""));
           	ezplib_replace_attr("sched_rule", 1, "beghr", scheduling_start_hour_7);
           		
           	scheduling_start_min_7 = websGetVar(wp, T("WLAN_scheduling_start_min_7"), T(""));
           	if (!strcmp(scheduling_start_min_7, "1")){
           		ezplib_replace_attr("sched_rule", 1, "begmin", "30");
           	}else{
           		ezplib_replace_attr("sched_rule", 1, "begmin", "00");	
           	}
           		
           	scheduling_end_hour_7 = websGetVar(wp, T("WLAN_scheduling_end_hour_7"), T(""));
           	ezplib_replace_attr("sched_rule", 1, "endhr", scheduling_end_hour_7);	
           			
           	scheduling_end_min_7 = websGetVar(wp, T("WLAN_scheduling_end_min_7"), T(""));
           	if (!strcmp(scheduling_end_min_7, "1")){
           		ezplib_replace_attr("sched_rule", 1, "endmin", "30");
           	}else{
           		ezplib_replace_attr("sched_rule", 1, "endmin", "00");	
           	}
           
           
           	// action 1
           	scheduling_action_1 = websGetVar(wp, T("WLAN_scheduling_action_1"), T(""));
           	ezplib_replace_attr("sched_rule", 2, "wlanstatus", scheduling_action_1);
           	
           	ezplib_replace_attr("sched_rule", 2, "enable", Scheduling_day_Active_1);
           
           	scheduling_start_hour_1 = websGetVar(wp, T("WLAN_scheduling_start_hour_1"), T(""));
           	ezplib_replace_attr("sched_rule", 2, "beghr", scheduling_start_hour_1);
           		
           	scheduling_start_min_1 = websGetVar(wp, T("WLAN_scheduling_start_min_1"), T(""));
           	if (!strcmp(scheduling_start_min_1, "1")){
           		ezplib_replace_attr("sched_rule", 2, "begmin", "30");
           	}else{
           		ezplib_replace_attr("sched_rule", 2, "begmin", "00");	
           	}
           		
           	scheduling_end_hour_1 = websGetVar(wp, T("WLAN_scheduling_end_hour_1"), T(""));
           	ezplib_replace_attr("sched_rule", 2, "endhr", scheduling_end_hour_1);	
           			
           	scheduling_end_min_1 = websGetVar(wp, T("WLAN_scheduling_end_min_1"), T(""));
           	if (!strcmp(scheduling_end_min_1, "1")){
           		ezplib_replace_attr("sched_rule", 2, "endmin", "30");
           	}else{
           		ezplib_replace_attr("sched_rule", 2, "endmin", "00");	
           	}
           
           	// action 2		
           	scheduling_action_2 = websGetVar(wp, T("WLAN_scheduling_action_2"), T(""));
           	ezplib_replace_attr("sched_rule", 3, "wlanstatus", scheduling_action_2);
           	
           	ezplib_replace_attr("sched_rule", 3, "enable", Scheduling_day_Active_2);
           
           	scheduling_start_hour_2 = websGetVar(wp, T("WLAN_scheduling_start_hour_2"), T(""));
           	ezplib_replace_attr("sched_rule", 3, "beghr", scheduling_start_hour_2);
           		
           	scheduling_start_min_2 = websGetVar(wp, T("WLAN_scheduling_start_min_2"), T(""));
           	if (!strcmp(scheduling_start_min_2, "1")){
           		ezplib_replace_attr("sched_rule", 3, "begmin", "30");
           	}else{
           		ezplib_replace_attr("sched_rule", 3, "begmin", "00");	
           	}
           		
           	scheduling_end_hour_2 = websGetVar(wp, T("WLAN_scheduling_end_hour_2"), T(""));
           	ezplib_replace_attr("sched_rule", 3, "endhr", scheduling_end_hour_2);	
           			
           	scheduling_end_min_2 = websGetVar(wp, T("WLAN_scheduling_end_min_2"), T(""));
           	if (!strcmp(scheduling_end_min_2, "1")){
           		ezplib_replace_attr("sched_rule", 3, "endmin", "30");
           	}else{
           		ezplib_replace_attr("sched_rule", 3, "endmin", "00");	
           	}
           
           	// action 3		
           	scheduling_action_3 = websGetVar(wp, T("WLAN_scheduling_action_3"), T(""));
           	ezplib_replace_attr("sched_rule", 4, "wlanstatus", scheduling_action_3);
           	
           	ezplib_replace_attr("sched_rule", 4, "enable", Scheduling_day_Active_3);
           
           	scheduling_start_hour_3 = websGetVar(wp, T("WLAN_scheduling_start_hour_3"), T(""));
           	ezplib_replace_attr("sched_rule", 4, "beghr", scheduling_start_hour_3);
           		
           	scheduling_start_min_3 = websGetVar(wp, T("WLAN_scheduling_start_min_3"), T(""));
           	if (!strcmp(scheduling_start_min_3, "1")){
           		ezplib_replace_attr("sched_rule", 4, "begmin", "30");
           	}else{
           		ezplib_replace_attr("sched_rule", 4, "begmin", "00");	
           	}
           		
           	scheduling_end_hour_3 = websGetVar(wp, T("WLAN_scheduling_end_hour_3"), T(""));
           	ezplib_replace_attr("sched_rule", 4, "endhr", scheduling_end_hour_3);	
           			
           	scheduling_end_min_3 = websGetVar(wp, T("WLAN_scheduling_end_min_3"), T(""));
           	if (!strcmp(scheduling_end_min_3, "1")){
           		ezplib_replace_attr("sched_rule", 4, "endmin", "30");
           	}else{
           		ezplib_replace_attr("sched_rule", 4, "endmin", "00");	
           	}
           
           	// action 4		
           	scheduling_action_4 = websGetVar(wp, T("WLAN_scheduling_action_4"), T(""));
           	ezplib_replace_attr("sched_rule", 5, "wlanstatus", scheduling_action_4);
           	
           	ezplib_replace_attr("sched_rule", 5, "enable", Scheduling_day_Active_4);	
           
           	scheduling_start_hour_4 = websGetVar(wp, T("WLAN_scheduling_start_hour_4"), T(""));
           	ezplib_replace_attr("sched_rule", 5, "beghr", scheduling_start_hour_4);
           		
           	scheduling_start_min_4 = websGetVar(wp, T("WLAN_scheduling_start_min_4"), T(""));
           	if (!strcmp(scheduling_start_min_4, "1")){
           		ezplib_replace_attr("sched_rule", 5, "begmin", "30");
           	}else{
           		ezplib_replace_attr("sched_rule", 5, "begmin", "00");	
           	}
           		
           	scheduling_end_hour_4 = websGetVar(wp, T("WLAN_scheduling_end_hour_4"), T(""));
           	ezplib_replace_attr("sched_rule", 5, "endhr", scheduling_end_hour_4);	
           			
           	scheduling_end_min_4 = websGetVar(wp, T("WLAN_scheduling_end_min_4"), T(""));
           	if (!strcmp(scheduling_end_min_4, "1")){
           		ezplib_replace_attr("sched_rule", 5, "endmin", "30");
           	}else{
           		ezplib_replace_attr("sched_rule", 5, "endmin", "00");	
           	}
           
           
           	// action 5		
           	scheduling_action_5 = websGetVar(wp, T("WLAN_scheduling_action_5"), T(""));
           	ezplib_replace_attr("sched_rule", 6, "wlanstatus", scheduling_action_5);
           	
           	ezplib_replace_attr("sched_rule", 6, "enable", Scheduling_day_Active_5);
           
           	scheduling_start_hour_5 = websGetVar(wp, T("WLAN_scheduling_start_hour_5"), T(""));
           	ezplib_replace_attr("sched_rule", 6, "beghr", scheduling_start_hour_5);
           		
           	scheduling_start_min_5 = websGetVar(wp, T("WLAN_scheduling_start_min_5"), T(""));
           	if (!strcmp(scheduling_start_min_5, "1")){
           		ezplib_replace_attr("sched_rule", 6, "begmin", "30");
           	}else{
           		ezplib_replace_attr("sched_rule", 6, "begmin", "00");	
           	}
           		
           	scheduling_end_hour_5 = websGetVar(wp, T("WLAN_scheduling_end_hour_5"), T(""));
           	ezplib_replace_attr("sched_rule", 6, "endhr", scheduling_end_hour_5);	
           			
           	scheduling_end_min_5 = websGetVar(wp, T("WLAN_scheduling_end_min_5"), T(""));
           	if (!strcmp(scheduling_end_min_5, "1")){
           		ezplib_replace_attr("sched_rule", 6, "endmin", "30");
           	}else{
           		ezplib_replace_attr("sched_rule", 6, "endmin", "00");	
           	}
           
           	// action 6		
           	scheduling_action_6 = websGetVar(wp, T("WLAN_scheduling_action_6"), T(""));
           	ezplib_replace_attr("sched_rule", 7, "wlanstatus", scheduling_action_6);
           	
           	ezplib_replace_attr("sched_rule", 7, "enable", Scheduling_day_Active_6);
           
           	scheduling_start_hour_6 = websGetVar(wp, T("WLAN_scheduling_start_hour_6"), T(""));
           	ezplib_replace_attr("sched_rule", 7, "beghr", scheduling_start_hour_6);
           		
           	scheduling_start_min_6 = websGetVar(wp, T("WLAN_scheduling_start_min_6"), T(""));
           	if (!strcmp(scheduling_start_min_6, "1")){
           		ezplib_replace_attr("sched_rule", 7, "begmin", "30");
           	}else{
           		ezplib_replace_attr("sched_rule", 7, "begmin", "00");	
           	}
           		
           	scheduling_end_hour_6 = websGetVar(wp, T("WLAN_scheduling_end_hour_6"), T(""));
           	ezplib_replace_attr("sched_rule", 7, "endhr", scheduling_end_hour_6);	
           			
           	scheduling_end_min_6 = websGetVar(wp, T("WLAN_scheduling_end_min_6"), T(""));
           	if (!strcmp(scheduling_end_min_6, "1")){
           		ezplib_replace_attr("sched_rule", 7, "endmin", "30");
           	}else{
           		ezplib_replace_attr("sched_rule", 7, "endmin", "00");	
           	}
	}else{
           	scheduling_action_0 = websGetVar(wp, T("WLAN_scheduling_action_0"), T(""));
           	ezplib_replace_attr("sched1_rule", 0, "wlanstatus", scheduling_action_0);
           	
           	ezplib_replace_attr("sched1_rule", 0, "enable", Scheduling_day_Active_0);
           
           	scheduling_start_hour_0 = websGetVar(wp, T("WLAN_scheduling_start_hour_0"), T(""));
           	ezplib_replace_attr("sched1_rule", 0, "beghr", scheduling_start_hour_0);
           		
           	scheduling_start_min_0 = websGetVar(wp, T("WLAN_scheduling_start_min_0"), T(""));
           	if (!strcmp(scheduling_start_min_0, "1")){
           		ezplib_replace_attr("sched1_rule", 0, "begmin", "30");
           	}else{
           		ezplib_replace_attr("sched1_rule", 0, "begmin", "00");	
           	}
           		
           	scheduling_end_hour_0 = websGetVar(wp, T("WLAN_scheduling_end_hour_0"), T(""));
           	ezplib_replace_attr("sched1_rule", 0, "endhr", scheduling_end_hour_0);	
           			
           	scheduling_end_min_0 = websGetVar(wp, T("WLAN_scheduling_end_min_0"), T(""));
           	if (!strcmp(scheduling_end_min_0, "1")){
           		ezplib_replace_attr("sched1_rule", 0, "endmin", "30");
           	}else{
           		ezplib_replace_attr("sched1_rule", 0, "endmin", "00");	
           	}
           
           	// action 7	
           	scheduling_action_7 = websGetVar(wp, T("WLAN_scheduling_action_7"), T(""));
           	ezplib_replace_attr("sched1_rule", 1, "wlanstatus", scheduling_action_7);
           	
           	ezplib_replace_attr("sched1_rule", 1, "enable", Scheduling_day_Active_7);	
           
           	scheduling_start_hour_7 = websGetVar(wp, T("WLAN_scheduling_start_hour_7"), T(""));
           	ezplib_replace_attr("sched1_rule", 1, "beghr", scheduling_start_hour_7);
           		
           	scheduling_start_min_7 = websGetVar(wp, T("WLAN_scheduling_start_min_7"), T(""));
           	if (!strcmp(scheduling_start_min_7, "1")){
           		ezplib_replace_attr("sched1_rule", 1, "begmin", "30");
           	}else{
           		ezplib_replace_attr("sched1_rule", 1, "begmin", "00");	
           	}
           		
           	scheduling_end_hour_7 = websGetVar(wp, T("WLAN_scheduling_end_hour_7"), T(""));
           	ezplib_replace_attr("sched1_rule", 1, "endhr", scheduling_end_hour_7);	
           			
           	scheduling_end_min_7 = websGetVar(wp, T("WLAN_scheduling_end_min_7"), T(""));
           	if (!strcmp(scheduling_end_min_7, "1")){
           		ezplib_replace_attr("sched1_rule", 1, "endmin", "30");
           	}else{
           		ezplib_replace_attr("sched1_rule", 1, "endmin", "00");	
           	}
           
           
           	// action 1
           	scheduling_action_1 = websGetVar(wp, T("WLAN_scheduling_action_1"), T(""));
           	ezplib_replace_attr("sched1_rule", 2, "wlanstatus", scheduling_action_1);
           	
           	ezplib_replace_attr("sched1_rule", 2, "enable", Scheduling_day_Active_1);
           
           	scheduling_start_hour_1 = websGetVar(wp, T("WLAN_scheduling_start_hour_1"), T(""));
           	ezplib_replace_attr("sched1_rule", 2, "beghr", scheduling_start_hour_1);
           		
           	scheduling_start_min_1 = websGetVar(wp, T("WLAN_scheduling_start_min_1"), T(""));
           	if (!strcmp(scheduling_start_min_1, "1")){
           		ezplib_replace_attr("sched1_rule", 2, "begmin", "30");
           	}else{
           		ezplib_replace_attr("sched1_rule", 2, "begmin", "00");	
           	}
           		
           	scheduling_end_hour_1 = websGetVar(wp, T("WLAN_scheduling_end_hour_1"), T(""));
           	ezplib_replace_attr("sched1_rule", 2, "endhr", scheduling_end_hour_1);	
           			
           	scheduling_end_min_1 = websGetVar(wp, T("WLAN_scheduling_end_min_1"), T(""));
           	if (!strcmp(scheduling_end_min_1, "1")){
           		ezplib_replace_attr("sched1_rule", 2, "endmin", "30");
           	}else{
           		ezplib_replace_attr("sched1_rule", 2, "endmin", "00");	
           	}
           
           	// action 2		
           	scheduling_action_2 = websGetVar(wp, T("WLAN_scheduling_action_2"), T(""));
           	ezplib_replace_attr("sched1_rule", 3, "wlanstatus", scheduling_action_2);
           	
           	ezplib_replace_attr("sched1_rule", 3, "enable", Scheduling_day_Active_2);
           
           	scheduling_start_hour_2 = websGetVar(wp, T("WLAN_scheduling_start_hour_2"), T(""));
           	ezplib_replace_attr("sched1_rule", 3, "beghr", scheduling_start_hour_2);
           		
           	scheduling_start_min_2 = websGetVar(wp, T("WLAN_scheduling_start_min_2"), T(""));
           	if (!strcmp(scheduling_start_min_2, "1")){
           		ezplib_replace_attr("sched1_rule", 3, "begmin", "30");
           	}else{
           		ezplib_replace_attr("sched1_rule", 3, "begmin", "00");	
           	}
           		
           	scheduling_end_hour_2 = websGetVar(wp, T("WLAN_scheduling_end_hour_2"), T(""));
           	ezplib_replace_attr("sched1_rule", 3, "endhr", scheduling_end_hour_2);	
           			
           	scheduling_end_min_2 = websGetVar(wp, T("WLAN_scheduling_end_min_2"), T(""));
           	if (!strcmp(scheduling_end_min_2, "1")){
           		ezplib_replace_attr("sched1_rule", 3, "endmin", "30");
           	}else{
           		ezplib_replace_attr("sched1_rule", 3, "endmin", "00");	
           	}
           
           	// action 3		
           	scheduling_action_3 = websGetVar(wp, T("WLAN_scheduling_action_3"), T(""));
           	ezplib_replace_attr("sched1_rule", 4, "wlanstatus", scheduling_action_3);
           	
           	ezplib_replace_attr("sched1_rule", 4, "enable", Scheduling_day_Active_3);
           
           	scheduling_start_hour_3 = websGetVar(wp, T("WLAN_scheduling_start_hour_3"), T(""));
           	ezplib_replace_attr("sched1_rule", 4, "beghr", scheduling_start_hour_3);
           		
           	scheduling_start_min_3 = websGetVar(wp, T("WLAN_scheduling_start_min_3"), T(""));
           	if (!strcmp(scheduling_start_min_3, "1")){
           		ezplib_replace_attr("sched1_rule", 4, "begmin", "30");
           	}else{
           		ezplib_replace_attr("sched1_rule", 4, "begmin", "00");	
           	}
           		
           	scheduling_end_hour_3 = websGetVar(wp, T("WLAN_scheduling_end_hour_3"), T(""));
           	ezplib_replace_attr("sched1_rule", 4, "endhr", scheduling_end_hour_3);	
           			
           	scheduling_end_min_3 = websGetVar(wp, T("WLAN_scheduling_end_min_3"), T(""));
           	if (!strcmp(scheduling_end_min_3, "1")){
           		ezplib_replace_attr("sched1_rule", 4, "endmin", "30");
           	}else{
           		ezplib_replace_attr("sched1_rule", 4, "endmin", "00");	
           	}
           
           	// action 4		
           	scheduling_action_4 = websGetVar(wp, T("WLAN_scheduling_action_4"), T(""));
           	ezplib_replace_attr("sched1_rule", 5, "wlanstatus", scheduling_action_4);
           	
           	ezplib_replace_attr("sched1_rule", 5, "enable", Scheduling_day_Active_4);	
           
           	scheduling_start_hour_4 = websGetVar(wp, T("WLAN_scheduling_start_hour_4"), T(""));
           	ezplib_replace_attr("sched1_rule", 5, "beghr", scheduling_start_hour_4);
           		
           	scheduling_start_min_4 = websGetVar(wp, T("WLAN_scheduling_start_min_4"), T(""));
           	if (!strcmp(scheduling_start_min_4, "1")){
           		ezplib_replace_attr("sched1_rule", 5, "begmin", "30");
           	}else{
           		ezplib_replace_attr("sched1_rule", 5, "begmin", "00");	
           	}
           		
           	scheduling_end_hour_4 = websGetVar(wp, T("WLAN_scheduling_end_hour_4"), T(""));
           	ezplib_replace_attr("sched1_rule", 5, "endhr", scheduling_end_hour_4);	
           			
           	scheduling_end_min_4 = websGetVar(wp, T("WLAN_scheduling_end_min_4"), T(""));
           	if (!strcmp(scheduling_end_min_4, "1")){
           		ezplib_replace_attr("sched1_rule", 5, "endmin", "30");
           	}else{
           		ezplib_replace_attr("sched1_rule", 5, "endmin", "00");	
           	}
           
           
           	// action 5		
           	scheduling_action_5 = websGetVar(wp, T("WLAN_scheduling_action_5"), T(""));
           	ezplib_replace_attr("sched1_rule", 6, "wlanstatus", scheduling_action_5);
           	
           	ezplib_replace_attr("sched1_rule", 6, "enable", Scheduling_day_Active_5);
           
           	scheduling_start_hour_5 = websGetVar(wp, T("WLAN_scheduling_start_hour_5"), T(""));
           	ezplib_replace_attr("sched1_rule", 6, "beghr", scheduling_start_hour_5);
           		
           	scheduling_start_min_5 = websGetVar(wp, T("WLAN_scheduling_start_min_5"), T(""));
           	if (!strcmp(scheduling_start_min_5, "1")){
           		ezplib_replace_attr("sched1_rule", 6, "begmin", "30");
           	}else{
           		ezplib_replace_attr("sched1_rule", 6, "begmin", "00");	
           	}
           		
           	scheduling_end_hour_5 = websGetVar(wp, T("WLAN_scheduling_end_hour_5"), T(""));
           	ezplib_replace_attr("sched1_rule", 6, "endhr", scheduling_end_hour_5);	
           			
           	scheduling_end_min_5 = websGetVar(wp, T("WLAN_scheduling_end_min_5"), T(""));
           	if (!strcmp(scheduling_end_min_5, "1")){
           		ezplib_replace_attr("sched1_rule", 6, "endmin", "30");
           	}else{
           		ezplib_replace_attr("sched1_rule", 6, "endmin", "00");	
           	}
           
           	// action 6		
           	scheduling_action_6 = websGetVar(wp, T("WLAN_scheduling_action_6"), T(""));
           	ezplib_replace_attr("sched1_rule", 7, "wlanstatus", scheduling_action_6);
           	
           	ezplib_replace_attr("sched1_rule", 7, "enable", Scheduling_day_Active_6);
           
           	scheduling_start_hour_6 = websGetVar(wp, T("WLAN_scheduling_start_hour_6"), T(""));
           	ezplib_replace_attr("sched1_rule", 7, "beghr", scheduling_start_hour_6);
           		
           	scheduling_start_min_6 = websGetVar(wp, T("WLAN_scheduling_start_min_6"), T(""));
           	if (!strcmp(scheduling_start_min_6, "1")){
           		ezplib_replace_attr("sched1_rule", 7, "begmin", "30");
           	}else{
           		ezplib_replace_attr("sched1_rule", 7, "begmin", "00");	
           	}
           		
           	scheduling_end_hour_6 = websGetVar(wp, T("WLAN_scheduling_end_hour_6"), T(""));
           	ezplib_replace_attr("sched1_rule", 7, "endhr", scheduling_end_hour_6);	
           			
           	scheduling_end_min_6 = websGetVar(wp, T("WLAN_scheduling_end_min_6"), T(""));
           	if (!strcmp(scheduling_end_min_6, "1")){
           		ezplib_replace_attr("sched1_rule", 7, "endmin", "30");
           	}else{
           		ezplib_replace_attr("sched1_rule", 7, "endmin", "00");	
           	}
	}
	// action 0

	nvram_commit();

	websRedirect(wp, "local/power_saving.asp");
}	  


// INPUT  Setting : 0 - Disable ; 1 - Enable
//	return 0: fail ; 1 : success
//	
int WLAN_Scheduler(int setting)
{
      char *value0,*value1;
	  
	if (setting == 1){ // Enable
		// WiFi Scheduler enable
		//system("/etc/rc.common /etc/init.d/sch stop");
		value0 = nvram_safe_get("sched_enable_bak");
		value1 = nvram_safe_get("sched1_enable_bak");
		if(!strcmp(value0,"0") && !strcmp(value1,"0")){
		     nvram_set("sched_enable", "1");
		     nvram_set("sched1_enable", "1");
		     nvram_set("sched_enable_bak", "1");
		     nvram_set("sched1_enable_bak", "1");
		}else{
		     nvram_set("sched_enable", value0);
		     nvram_set("sched1_enable", value1);
		}
		//system("/etc/rc.common /etc/init.d/sch start");
		nvram_commit();
		system("/sbin/ezp-scheduler");
		system("/sbin/ezp1-scheduler");
		return 1;		
	}else if (setting == 0){ // Disable
		// WiFi Scheduler disable
		//system("/etc/rc.common /etc/init.d/sch start");
		nvram_set("sched_enable", "0");
		nvram_set("sched1_enable", "0");
		//system("/etc/rc.common /etc/init.d/sch stop");
		nvram_commit();
		system("/sbin/ezp-scheduler");		
		system("/sbin/ezp1-scheduler");		
		return 1;	
	}else{
		printf("Error: (setting) parameter is not correct !!! ");
		return 0;
	}
}
	

// INPUT  Setting : 0 - Disable ; 1 - Enable
//	return 0: fail ; 1 : success
//		
int WLAN_EZSecurity(int setting)
{
#if 0	
	if (setting == 1){ // Enable
		nvram_bufset(RT2860_NVRAM, "easy_wireless_security","1");
		
		// Use Original Wireless Security
		nvram_commit(RT2860_NVRAM);
		return 1;		
	}else if (setting == 0){ // Disable
		nvram_bufset(RT2860_NVRAM, "easy_wireless_security","0");
		
		// Set No Security
		int mbssid = 0; // always use the first one ssid
		STFs(RT2860_NVRAM, mbssid, "AuthMode", "OPEN");
		STFs(RT2860_NVRAM, mbssid, "EncrypType", "NONE");
		STFs(RT2860_NVRAM, mbssid, "IEEE8021X", "0");			

		nvram_commit(RT2860_NVRAM);	
		//initInternet();
		reconfig_wireless(RT2860_NVRAM);
		return 1;	
	}else{
		printf("Error: (setting) parameter is not correct !!! ");
		return 0;
	}
#endif
	return 0;
}

#endif //Steve10

void formDefineWireless(void) {
	websFormDefine(T("wifiAPGeneral"), wifiAPGeneral);
	websFormDefine(T("wifiAPGeneral5G"), wifiAPGeneral5G);
//
	websAspDefine(T("Channellist"), Channellist);
#if 1
	websAspDefine(T("getWlan11aChannels"), getWlan11aChannels);
	websAspDefine(T("getWlan11bChannels"), getWlan11bChannels);
	websAspDefine(T("getWlan11gChannels"), getWlan11gChannels);
// Tommy, 2008/12/26 03:14下午
	websAspDefine(T("getWlan11gChannelsFreq"), getWlan11gChannelsFreq);
	websAspDefine(T("getWlan11aChannelsFreq"), getWlan11aChannelsFreq);
	
	websAspDefine(T("getGeneralChannelsFreq"), getGeneralChannelsFreq);
	websAspDefine(T("get5gGeneralChannelsFreq"), get5gGeneralChannelsFreq);
	websAspDefine(T("getWlan11gExtChannelsFreq"), getWlan11gExtChannelsFreq);	
	websAspDefine(T("getWlan11aExtChannelsFreq"), getWlan11aExtChannelsFreq);	
	websAspDefine(T("getRadioStatusASP"), getRadioStatusASP);
       websAspDefine(T("getRadioStatusASP5G"), getRadioStatusASP5G);
	
	websAspDefine(T("getAPClientConnectionESSID"), getAPClientConnectionESSID);	
	websAspDefine(T("getAPClientConnectionStatus"), getAPClientConnectionStatus);
	websAspDefine(T("getAPClientConnectionESSIDMAC"), getAPClientConnectionESSIDMAC);
	
	websAspDefine(T("getAPClient5GConnectionESSID"), getAPClient5GConnectionESSID);	
	websAspDefine(T("getAPClient5GConnectionStatus"), getAPClient5GConnectionStatus);
	websAspDefine(T("getAPClient5GConnectionESSIDMAC"), getAPClient5GConnectionESSIDMAC);	

       websAspDefine(T("getConnectedAPMode"), getConnectedAPMode);
		
	websAspDefine(T("getWlanApcliBuilt"), getWlanApcliBuilt);
	websAspDefine(T("getWlanChannel"), getWlanChannel);
	websAspDefine(T("getWlanCurrentMac"), getWlanCurrentMac);
	websAspDefine(T("getWlanStaInfo"), getWlanStaInfo);

        websAspDefine(T("getmacrepeaterStaInfo"), getmacrepeaterStaInfo);
// Tommy, 2009/1/21 05:42
	websAspDefine(T("getwifiStaInfo"), getwifiStaInfo);	
	websAspDefine(T("getwifi5GStaInfo"), getwifi5GStaInfo);	
	websAspDefine(T("getwifi5GRadarStatus"), getwifi5GRadarStatus);	
	websAspDefine(T("getDLSBuilt"), getDLSBuilt);
	websAspDefine(T("getWlanM2UBuilt"), getWlanM2UBuilt);
// Tommy, Patch WDS for WEP, 2009/3/26 02:38
#if 0	
	websAspDefine(T("getWlanWdsEncType"), getWlanWdsEncType);
#endif	
	websAspDefine(T("isWPSConfiguredASP"), isWPSConfiguredASP);
	websAspDefine(T("isAntennaDiversityBuilt"), isAntennaDiversityBuilt);
// Tommy, 2008/12/25 11:22上午	
	websAspDefine(T("getOpChannleASP"), getOpChannleASP);	
       websAspDefine(T("getOpChannleASP5G"), getOpChannleASP5G);
	websAspDefine(T("getOpChannleNum"), getOpChannleNum);
	websAspDefine(T("getOpChannleNum5G"), getOpChannleNum5G);
		
#ifdef CONFIG_RT2860V2_AP_ANTENNA_DIVERSITY
	websFormDefine(T("AntennaDiversity"), AntennaDiversity);
	websFormDefine(T("getAntenna"), getAntenna);
#endif
// Tommy, decrease code size , 2009/3/17 01:28
#if Include_Ralink_Original
	websFormDefine(T("wirelessBasic"), wirelessBasic);
	websFormDefine(T("wirelessAdvanced"), wirelessAdvanced);
// Tommy, Patch WDS for WEP, 2009/3/26 02:38	
	websFormDefine(T("wirelessWds"), wirelessWds);	
#endif // #if Include_Ralink_Original
// Passphrase to WEP
#if 1
	websFormDefine(T("wifiget128wepkey"), wifiget128wepkey);
	websFormDefine(T("wifiget40wepkey"), wifiget40wepkey);	
#endif
// Tommy, 2008/12/11 02:03下午	
	websFormDefine(T("wifiAdvanced"), wifiAdvanced);
	websFormDefine(T("wifi5GAdvanced"), wifi5GAdvanced);
	websFormDefine(T("wifiGeneral"), wifiGeneral);
	websFormDefine(T("wifiWDS"), wifiWDS);
	websFormDefine(T("wifi5GWDS"), wifi5GWDS);
	websFormDefine(T("wifiApcli"), wifiApcli);
	websAspDefine(T("getwifiRSSI"), getwifiRSSI);	
	websAspDefine(T("getwifiBSSIDList"), getwifiBSSIDList);
	websAspDefine(T("getwifi5GBSSIDList"), getwifi5GBSSIDList);	
	websFormDefine(T("wispwifiApcli"), wispwifiApcli);	
	websFormDefine(T("wispwifi5GApcli"), wispwifi5GApcli);	
	websFormDefine(T("urwifiApcli"), urwifiApcli);	
	websFormDefine(T("urwifi5GApcli"), urwifi5GApcli);	
		
	websFormDefine(T("wirelessApcli"), wirelessApcli);
	websFormDefine(T("wirelessWmm"), wirelessWmm);
	websFormDefine(T("wirelessGetSecurity"), wirelessGetSecurity);	
	websFormDefine(T("wireless5gGetSecurity"), wireless5gGetSecurity);	
	websFormDefine(T("GetWDSSecurity"), GetWDSSecurity);	
	websFormDefine(T("Get5GWDSSecurity"), Get5GWDSSecurity);	
// Tommy, decrease code size , 2009/3/17 01:28
#if Include_Ralink_Original	
	websFormDefine(T("APSecurity"), APSecurity);
#endif // #if Include_Ralink_Original
	
// Tommy , 2009/1/21 05:48
	websFormDefine(T("wifiAssocList"), wifiAssocList);
	websFormDefine(T("EZwifiAPSecurity"), EZwifiAPSecurity);	
	websFormDefine(T("EZAPSecurity"), EZAPSecurity);		
		
	websFormDefine(T("wifiAPSecurity"), wifiAPSecurity);
	websFormDefine(T("wifi5GAPSecurity"), wifi5GAPSecurity);
	websFormDefine(T("MultiFilterSecurity"), MultiFilterSecurity);
	websFormDefine(T("MultiFilter5GSecurity"), MultiFilter5GSecurity);
	websFormDefine(T("APDeleteAccessPolicyList"), APDeleteAccessPolicyList);
	websFormDefine(T("APDelete5GAccessPolicyList"), APDelete5GAccessPolicyList);
// Tommy, WLAN Scheduling , 2008/12/9 05:03下午
	websFormDefine(T("wifischeduler"), wifischeduler);
	websFormDefine(T("wifi5gscheduler"), wifi5gscheduler);
	websFormDefine(T("EZwifischeduler"), EZwifischeduler);	
	websFormDefine(T("WiFiWMM"), WiFiWMM);	
	websFormDefine(T("WiFi5GWMM"), WiFi5GWMM);	
#endif
}
