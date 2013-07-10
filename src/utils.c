/* vi: set sw=4 ts=4 sts=4: */
/*
 *	utils.c -- System Utilities
 *
 *	Copyright (c) Ralink Technology Corporation All Rights Reserved.
 *
 *	$Id: utils.c,v 1.92.4.1 2008-07-29 13:02:54 steven Exp $
 */
#include	<time.h>
#include	<signal.h>
#include	<sys/ioctl.h>
#include	<sys/time.h>


#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>
#include  <sys/sysinfo.h>

#include	"webs.h"
#include	"utils.h"
#include	"wireless.h"
#include 	"nvram.h"
#include 	"nvram_rule.h"
//#include	"wireless.h"
//#include	"internet.h"


//#include	"sdk_version.h"
//#include	"nvram.h"
//#include	"ralink_gpio.h"
#include       "led_gpio.h"
//#include	"linux/autoconf.h"  //kernel config
//#include	"config/autoconf.h" //user config

#define TMP_LEN 256 //aron patch for giga, from httpd common.h

#if 1 //Steve2023
//#if 1//Arthur Chow 2009-03-30
//#include	"sdk_version.h"
//#endif
static	char reValue[256];
#define SHORT_BUF_LEN   32

static int  getCfgGeneral(int eid, webs_t wp, int argc, char_t **argv);
static int  getCfgNthGeneral(int eid, webs_t wp, int argc, char_t **argv);
static int  getCfgZero(int eid, webs_t wp, int argc, char_t **argv);
static int  getCfgNthZero(int eid, webs_t wp, int argc, char_t **argv);
static int  getCfg2General(int eid, webs_t wp, int argc, char_t **argv);
static int  getCfg2NthGeneral(int eid, webs_t wp, int argc, char_t **argv);
static int  getCfg2Zero(int eid, webs_t wp, int argc, char_t **argv);
static int  getCfg2NthZero(int eid, webs_t wp, int argc, char_t **argv);
static int  getCfg3General(int eid, webs_t wp, int argc, char_t **argv);
static int  getCfg3Zero(int eid, webs_t wp, int argc, char_t **argv);
static int  getDpbSta(int eid, webs_t wp, int argc, char_t **argv);
static int  getLangBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int  getMiiInicBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int  getPlatform(int eid, webs_t wp, int argc, char_t **argv);
static int  getStationBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int  getSysBuildTime(int eid, webs_t wp, int argc, char_t **argv);
static int  getSdkVersion(int eid, webs_t wp, int argc, char_t **argv);
static int  getSysUptime(int eid, webs_t wp, int argc, char_t **argv);
#if 1//Arthur Chow 2009-01-03
static int  getSysResource(int eid, webs_t wp, int argc, char_t **argv);
#endif
static int  getPortStatus(int eid, webs_t wp, int argc, char_t **argv);
static int  isOnePortOnly(int eid, webs_t wp, int argc, char_t **argv);
static void forceMemUpgrade(webs_t wp, char_t *path, char_t *query);
static void setOpMode(webs_t wp, char_t *path, char_t *query);
//Tommy, 2008/12/16 08:14下午
static void SetOperationMode(webs_t wp, char_t *path, char_t *query);
// Steve, 2009/01/16 02:45下午
static int  getCfgLLTD(int eid, webs_t wp, int argc, char_t **argv);
#if 1//Arthur Chow 2009-02-15: For easy mode page
static int  startLLTDcheck(int eid, webs_t wp, int argc, char_t **argv);
static int  startInternetcheck(int eid, webs_t wp, int argc, char_t **argv);
static int  statusRoutercheck(int eid, webs_t wp, int argc, char_t **argv);
static int  statusInternetcheck(int eid, webs_t wp, int argc, char_t **argv);
static int  javascriptLLTD(int eid, webs_t wp, int argc, char_t **argv);
static void easy_setting(webs_t wp, char_t *path, char_t *query);
static int  clearLLTDInfo(int eid, webs_t wp, int argc, char_t **argv);
static int  existLLTDinfo(int eid, webs_t wp, int argc, char_t **argv);
#endif
#if 1//Arthur Chow 2009-03-30
static int  getFirmwareVersion(int eid, webs_t wp, int argc, char_t **argv);
static int  getFirmwareDate(int eid, webs_t wp, int argc, char_t **argv);
#endif
//Steve 2010-08-27
static int  getGetUploadFlag(int eid, webs_t wp, int argc, char_t **argv);

/*********************************************************************
 * System Utilities
 */

#if 1
int
wfputs(char *buf, FILE * fp)
{
#ifdef HTTPS_SUPPORT
    if (do_ssl)
        return BIO_puts((BIO *) fp, buf);
    else
#endif
        printf("fputs started !!!!");
		return fputs(buf, fp);
}

int
wfprintf(FILE * fp, char *fmt, ...)
{
    va_list args;
    /* FIXME: 4096 might be insufficient */
    char buf[4096];
    int ret;

    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
#ifdef HTTPS_SUPPORT
    if (do_ssl)
        ret = BIO_printf((BIO *) fp, "%s", buf);
    else
#endif
        ret = fprintf(fp, "%s", buf);
    va_end(args);
    return ret;
}

int
wfflush(FILE * fp)
{
#ifdef HTTPS_SUPPORT
    if (do_ssl) {
        BIO_flush((BIO *) fp);
        return 1;
    } else
#endif
    return fflush(fp);
}
int
is_digit_str(char *str)
{
    int i;

    for (i = 0; *(str + i); i++) {
        if (!isdigit(*(str + i))) {
            printf("not digit str\n");
			return FALSE;
        }
    }
	printf("is digit str\n");
    return TRUE;
}

int
is_ascii_str(char *str)
{
	int i;

	for (i = 0; *(str + i); i++) {
		if (!isascii(*(str + i))) {
			return FALSE;
		}
	}
	return TRUE;
}


int
valid_name(webs_t wp, char *value, struct variable *v)
{
	int max;
	if (!*value) {
		if (v->nullok) {
			return TRUE;
		} else {
			websDebugWrite(wp, "Invalid <b>%s</b>: empty value<br>",
					v->longname);
			return FALSE;
		}
	}

	if (valid_word(wp, value, v) == FALSE) {
		return FALSE;
	}

	max = atoi(v->argv[0]);

	if (!is_ascii_str(value)) {
		websDebugWrite(wp,
				"Invalid <b>%s</b> %s: NULL or have illegal "
				"characters<br>",
				v->longname,
				value);
		return FALSE;
	}
	if (strlen(value) > max) {
		websDebugWrite(wp,
				"Invalid <b>%s</b> %s: out of range 1-%d characters<br>",
				v->longname, value, max);
		return FALSE;
	}

	return TRUE;
}


/* 
 * XXX: Don't understand what this code does :(
 */

int
is_legal_netmask(char *value)
{
	struct in_addr netmask;
	unsigned long mask;
	int i, maskbits;

	if (!inet_aton(value,&netmask))  {
		return FALSE;
	}

	mask = ntohl(netmask.s_addr);
	for ( maskbits=32 ; (mask & (1L<<(32-maskbits))) == 0 ; maskbits-- )
		;
	/* Re-create the netmask and compare to the origianl
	 * to make sure it is a valid netmask.
	 */
	mask = 0;
	for ( i=0 ; i<maskbits ; i++ )
		mask |= 1<<(31-i);
	if ( mask != ntohl(netmask.s_addr) ) {
		return FALSE;
	}
	return TRUE;
}


/* 
 * Example:
 * legal_ipaddr("192.168.1.1"); return true;
 * legal_ipaddr("192.168.1.1111"); return false;
 */
int
is_legal_ipaddr(char *value)
{
    struct in_addr ipaddr;

    /* Use inet_aton() to verify the IP address format. */
    if (!inet_aton(value, &ipaddr)) {
        return FALSE;
    } else {
        return TRUE;
    }
}

/* Valid family. When returning w/ FALSE, websDebugWrite() must be invoked
 * before exit. Thus, error_value could be set to 1.
 * For NVRAM, ^ and | are ommitted. */
int
valid_word(webs_t wp, char *value, struct variable *v)
{
    char *longname;

    if (!value || !(*value))
        return TRUE;
    if (!v || !(v->longname))
        longname = "Input String";
    else
        longname = v->longname;
    if (strchr(value, '^') || strchr(value, '|') || 
        strchr(value, '\'') || strchr(value, '"') ||
        strchr(value, ';') || strchr(value, '$') ||
        strchr(value, '(') || strchr(value, ')') ||
        strchr(value, '`') || strchr(value, '&') ||
        strchr(value, '>') || strchr(value, '<')) {
        return FALSE;
    }

    return TRUE;
}
#define IS_NUM(NC) ((NC >= '0') && (NC <= '9'))
#define IS_ASCII(NC) (((NC >= 'a') && (NC <= 'z')) || ((NC >= 'A') && (NC <= 'Z')))
#define IS_ALLOWED_CHAR(NC) ((NC == '-') || (NC == '_') || (NC == '.'))
#define VALID_DOMAIN_NAME_CHAR(NC) (IS_NUM(NC) || IS_ASCII(NC) || IS_ALLOWED_CHAR(NC))
int
valid_domain(webs_t wp, char *value, struct variable *v)
{
    int slen;
    char *p;

    if (!*value) {
        if (v->nullok) {
            return TRUE;
        } else {
            return FALSE;
        }
    }

    if (valid_word(wp, value, v) == FALSE)
        return FALSE;
    if ( v->argv && *(v->argv) && ((slen = atoi(*(v->argv))) != 0) && (strlen(value) > slen)){
        /* value length too long */
        return FALSE;
    }
    for (p = value; *p; p++){
        if(!VALID_DOMAIN_NAME_CHAR(*p))
            return FALSE;
    }
    return TRUE;
}

int
valid_ipaddr(webs_t wp, char *value, struct variable *v)
{ 
    if (!*value) {
        if (v->nullok) {
            return TRUE;
        } else {
            return FALSE;
        }
    }

    if (valid_word(wp, value, v) == FALSE)
        return FALSE;

    if (!is_legal_ipaddr(value)) {
        return FALSE;
    }

    if ((*(value+strlen(value)-2)=='.' && *(value+strlen(value)-1)=='0') || 
            (*(value+strlen(value)-4)=='.' && *(value+strlen(value)-3)=='2' &&
             *(value+strlen(value)-2)=='5' && *(value+strlen(value)-1)=='5')) {
        return FALSE;
    }
    return TRUE;
}
int
valid_number(webs_t wp, char *value, struct variable *v)
{
    int i;

    if (!*value) {
        if (v->nullok) {
            return TRUE;
        } else {
            return FALSE;
        }
    }

    if (valid_word(wp, value, v) == FALSE) {
        return FALSE;
    }

    for (i = 0; value[i] != '\0'; i++) {
        if (value[i] > '9' || value[i] < '0') {
            return FALSE;
        }
    }

    return TRUE;
}

int
valid_range(webs_t wp, char *value, struct variable *v)
{
    int n, start, end;

    if (!*value) {
        if (v->nullok) {
            return TRUE;
        } else {
            return FALSE;
        }
    }

    if (valid_number(wp, value, v) == FALSE) {
        return FALSE;
    }

    n = atoi(value);
    start = atoi(v->argv[0]);
    end = atoi(v->argv[1]);
	printf("valid range value=%d,start value=%d,end value=%d\n",n,start,end);

    if (!is_digit_str(value) || n < start || n > end) {
		printf("value is not in the range!!!!");
        return FALSE;
    }

    return TRUE;
}

int
valid_choice(webs_t wp, char *value, struct variable *v)
{
    char **choice;

    if (!*value) {
        if (v->nullok) {
            return TRUE;
        } else {
            return FALSE;
        }
    }

    if (valid_word(wp, value, v) == FALSE)
        return FALSE;

    for (choice = v->argv; *choice; choice++) {
        if (!strcmp(value, *choice))
            return TRUE;
    }

    for (choice = v->argv; *choice; choice++)
    return FALSE;
}
int
valid_strict_name(webs_t wp, char *value, struct variable *v)
{
	int max; 

	if (!*value) {
		if (v->nullok) {
			return TRUE;
		} else {
			websDebugWrite(wp, "Invalid <b>%s</b>: empty value<br>",
					v->longname);
			return FALSE;
		}    
	}    

	if (valid_word(wp, value, v) == FALSE) {
		return FALSE;
	}    

	max = atoi(v->argv[0]);
	if (strchr(value, ' ') || strchr(value, '*')) {
		websDebugWrite(wp,
				"Invalid <b>%s</b> %s: * and space are not "
				"allowed<br>", 
				v->longname,
				value);
		return FALSE;
	}    

	if (!is_ascii_str(value)) {
		websDebugWrite(wp,
				"Invalid <b>%s</b> %s: NULL or have illegal "
				"characters<br>",
				v->longname,
				value);
		return FALSE;
	}    

	if (is_digit_str(value)) {
		websDebugWrite(wp,
				"Invalid <b>%s</b> %s: can not use only number<br>",
				v->longname, value);
		return FALSE;
	}

	if (strlen(value) > max) {
		websDebugWrite(wp,
				"Invalid <b>%s</b> %s: out of range 1-%d characters<br>",
				v->longname, value, max);
		return FALSE;
	}

	return TRUE;
}

int
valid_if(webs_t wp, char *value, struct variable *v)
{
	char *rule_num;
	char tmp[TMP_LEN];
	int if_str_len, diff, i = 0;
	int res = FALSE;

	if (!*value) {
		if (v->nullok) {
			return TRUE;
		} else {
			websDebugWrite(wp, "Invalid <b>%s</b>: empty value<br>",
					v->longname);
			return FALSE;
		}
	}

	if (valid_word(wp, value, v) == FALSE)
		return FALSE;
	/**
	 * The input 'value' would be compose as prefix + num. Such as lan1,
	 * wan2.
	 */

	snprintf(tmp, TMP_LEN, "%s_num", (char *)v->argv[0]);
	rule_num = nvram_safe_get(tmp);
	while (v->argv[i]) {
		/* Validate the if-prefix: wan, lan, pppoe... */
		if_str_len = strlen(v->argv[i]);
		if (!strncmp(value, v->argv[i], if_str_len)) {
			/* Validate the if-num. */
			value += if_str_len;
			diff = (int)(*value - '0');

			/* For UI, the index is
			 * 1-based. */
			if (diff < 0 || diff >= atoi(rule_num)) {
				websDebugWrite(wp,"Invalid <b>%s</b> %s: %" 
						"out of interface range 0-%d<br>",
						v->longname, value, atoi(rule_num));
				res = FALSE;
			}
			else {
				res = TRUE;
			}
			break;
		}
		i++;
	}
	if (!v->argv[i]) {
		websDebugWrite(wp, "Invalid <b>%s</b> %s: not an interface name<br>",
				v->longname, value);
	}
	return res;
}


int
valid_netmask(webs_t wp, char *value, struct variable *v)
{
	if (!*value) {
		if (v->nullok) {
			return TRUE;
		} else {
			websDebugWrite(wp, "Invalid <b>%s</b>: empty value<br>",
					v->longname);
			return FALSE;
		}
	}

	if (valid_word(wp, value, v) == FALSE)
		return FALSE;

	if (!is_legal_netmask(value)) {
		websDebugWrite(wp, "Invalid <b>%s</b> %s: not a legal netmask<br>",
				v->longname, value);
		return FALSE;
	}

	/* TODO: 255.255.255.0, 255.255.255.127, 255.255.255.63, 255.255.255.X */

	return TRUE;
}


int
valid_length_range(webs_t wp, char *value, struct variable *v)
{
	int length, min, max;

	min = atoi(v->argv[0]);
	max = atoi(v->argv[1]);
	length = strlen(value);

	if (!*value) {
		if (v->nullok) {
			return TRUE;
		} else {
			websDebugWrite(wp, "Invalid <b>%s</b>: empty value<br>",
					v->longname);
			return FALSE;
		}
	}
	if (valid_word(wp, value, v) == FALSE) {
		return FALSE;
	}

	if (length < min || length > max) {
		websDebugWrite(wp, "Invalid <b>%s</b> %d: "
				"length should between (%d - %d)<br>",
				v->longname, length, min,
				max);
		return FALSE;
	}

	return TRUE;
}

#endif
//aron patch for giga
char* prefix2mask(int idx)
{ 
    char *mask;
    switch(idx) //Translate AXIM's netmask to AboCom's UI format
    {
        case 0:
	        mask="0.0.0.0";
	        break;
	case 1:
	        mask="128.0.0.0";
	        break;
	case 2:
	        mask="192.0.0.0";
	        break;
	case 3:
	        mask="224.0.0.0";
	        break;
	case 4:
	        mask="240.0.0.0";
	        break;
	case 5:
	        mask="248.0.0.0";
	        break;
	case 6:
	        mask="252.0.0.0";
	        break;
	case 7:
	        mask="254.0.0.0";
	        break;    
        case 8:
	        mask="255.0.0.0";
	        break;
	case 9:
	        mask="255.128.0.0";
	        break;
	case 10:
	        mask="255.192.0.0";
	        break;
	case 11:
	        mask="255.224.0.0";
	        break;
	case 12:
	        mask="255.240.0.0";
	        break;
	case 13:
	        mask="255.248.0.0";
	        break;
	case 14:
	        mask="255.252.0.0";
	        break;
	case 15:
	        mask="255.254.0.0";
	        break;
        case 16:
	        mask="255.255.0.0";
	        break;
	case 17:
	        mask="255.255.128.0";
	        break;
	case 18:
	        mask="255.255.192.0";
	        break;
	case 19:
	        mask="255.255.224.0";
	        break;
	case 20:
	        mask="255.255.240.0";
	        break;
	case 21:
	        mask="255.255.248.0";
	        break;
	case 22:
	        mask="255.255.252.0";
	        break;
	case 23:
	        mask="255.255.254.0";
	        break;
	case 24:
	        mask="255.255.255.0";
	        break;
	case 25:
	        mask="255.255.255.128";
	        break;
	case 26:
	        mask="255.255.255.192";
	        break;
	case 27:
	        mask="255.255.255.224";
	        break;
	case 28:
	        mask="255.255.255.240";
	        break;
	case 29:
	        mask="255.255.255.248";
	        break;
	case 30:
	        mask="255.255.255.252";
	        break;
	case 31:
	        mask="255.255.255.254";
	        break;
	case 32:
	        mask="255.255.255.255";
	        break;
	default:
	        mask="";
    }
          
    return mask;
}

void arplookup(char *ip, char *arp)
{
    char buf[256];
    FILE *fp = fopen("/proc/net/arp", "r");
    if(!fp){
        trace(0, T("no proc fs mounted!\n"));
        return;
    }
    strcpy(arp, "00:00:00:00:00:00");
    while(fgets(buf, 256, fp)){
        char ip_entry[32], hw_type[8],flags[8], hw_address[32];
        sscanf(buf, "%s %s %s %s", ip_entry, hw_type, flags, hw_address);
        if(!strcmp(ip, ip_entry)){
            strcpy(arp, hw_address);
            break;
        }
    }

    fclose(fp);
    return;
}


/*
 * description: kill process whose pid was recorded in file
 *              (va is supported)
 */
int doKillPid(char_t *fmt, ...)
{
	va_list		vargs;
	char_t		*pid_fname = NULL;
	struct stat	st;

	va_start(vargs, fmt);
	if (fmtValloc(&pid_fname, WEBS_BUFSIZE, fmt, vargs) >= WEBS_BUFSIZE) {
		trace(0, T("doKillPid: lost data, buffer overflow\n"));
	}
	va_end(vargs);

	if (pid_fname) {
		if (-1 == stat(pid_fname, &st)) //check the file existence
			return 0;
		doSystem("kill `cat %s`", pid_fname);
		doSystem("rm -f %s", pid_fname);
		bfree(B_L, pid_fname);
	}
	return 0;
}

/*
 * description: parse va and do system
 */
int doSystem(char_t *fmt, ...)
{
	va_list		vargs;
	char_t		*cmd = NULL;
	int			rc = 0;

	va_start(vargs, fmt);
	if (fmtValloc(&cmd, WEBS_BUFSIZE, fmt, vargs) >= WEBS_BUFSIZE) {
		trace(0, T("doSystem: lost data, buffer overflow\n"));
	}
	va_end(vargs);

	if (cmd) {
		trace(0, T("%s\n"), cmd);
		rc = system(cmd);
		bfree(B_L, cmd);
	}
	return rc;
}

/*
 * arguments: modname - module name
 * description: test the insertion of module through /proc/modules
 * return: -1 = fopen error, 1 = module inserted, 0 = not inserted yet
 */
int getModIns(char *modname)
{
	FILE *fp;
	char buf[128];
	int i;

	if (NULL == (fp = fopen("/proc/modules", "r"))) {
		error(E_L, E_LOG, T("getModIns: open /proc/modules error"));
		return -1;
	}

	while (NULL != fgets(buf, 128, fp)) {
		i = 0;
		while (!isspace(buf[i++]))
			;
		buf[i - 1] = '\0';
		if (!strcmp(buf, modname)) {
			fclose(fp);
			return 1;
		}
	}
	fclose(fp);
	//error(E_L, E_LOG, T("getModIns: module %s not inserted"), modname);
	return 0;
}

/*
 * arguments: index - index of the Nth value
 *            values - un-parsed values
 * description: parse values delimited by semicolon, and return the value
 *              according to given index (starts from 0)
 * WARNING: the original values will be destroyed by strtok
 */
char *getNthValue(int index, char *values)
{
	int i;
	static char *tok;

	if (NULL == values)
		return NULL;
	for (i = 0, tok = strtok(values, ";");
			(i < index) && tok;
			i++, tok = strtok(NULL, ";")) {
		;
	}
	if (NULL == tok)
		return "";
	return tok;
}

/*
 * arguments: index - index of the Nth value (starts from 0)
 *            old_values - un-parsed values
 *            new_value - new value to be replaced
 * description: parse values delimited by semicolon,
 *              replace the Nth value with new_value,
 *              and return the result
 * WARNING: return the internal static string -> use it carefully
 */
char *setNthValue(int index, char *old_values, char *new_value)
{
	int i;
	char *p, *q;
	static char ret[2048];
	char buf[8][256];

	memset(ret, 0, 2048);
	for (i = 0; i < 8; i++)
		memset(buf[i], 0, 256);

	//copy original values
	for ( i = 0, p = old_values, q = strchr(p, ';')  ;
	      i < 8 && q != NULL                         ;
	      i++, p = q + 1, q = strchr(p, ';')         )
	{
		strncpy(buf[i], p, q - p);
	}
	strcpy(buf[i], p); //the last one

	//replace buf[index] with new_value
	strncpy(buf[index], new_value, 256);

	//calculate maximum index
	index = (i > index)? i : index;

	//concatenate into a single string delimited by semicolons
	strcat(ret, buf[0]);
	for (i = 1; i <= index; i++) {
		strncat(ret, ";", 2);
		strncat(ret, buf[i], 256);
	}

	p = ret;
	return p;
}

/*
 * arguments: values - values delimited by semicolon
 * description: parse values delimited by semicolon, and return the number of
 *              values
 */
int getValueCount(char *values)
{
	int cnt = 0;

	if (NULL == values)
		return 0;
	while (*values++ != '\0') {
		if (*values == ';')
			++cnt;
	}
	return (cnt + 1);
}

/*
 * check the existence of semicolon in str
 */
int checkSemicolon(char *str)
{
	char *c = strchr(str, ';');
	if (c)
		return 1;
	return 0;
}

/*
 * argument: ip address
 * return: 1 = the given ip address is within LAN's scope
 *         0 = otherwise
 */
#if 0
int isInLan(char *radius_ip_addr)
{
    char lan_if_addr[16];
    char lan_if_netmask[16];

    struct in_addr lan_ip;
    struct in_addr lan_netmask;
    struct in_addr radius_ip;

    if ((getIfIp(getLanIfName(), lan_if_addr)) == -1) {
        printf("getLanIP error\n");
        return 0;
    }
    if ((getIfNetmask(getLanIfName(), lan_if_netmask)) == -1) {
        printf("getLanNetmask error\n");
        return 0;
    }

    inet_aton(lan_if_addr, &lan_ip);
    inet_aton(lan_if_netmask, &lan_netmask);
    inet_aton(radius_ip_addr, &radius_ip);

    printf("lan_ip:%08x\n", lan_ip.s_addr);
    printf("lan_netmask:%08x\n", lan_netmask.s_addr);
    printf("radius_ip:%08x\n", radius_ip.s_addr);

    if ((lan_ip.s_addr & lan_netmask.s_addr) == (radius_ip.s_addr & lan_netmask.s_addr) ){
        printf("in Lan\n");
        return 1;
    } else {
        printf("not in lan\n");
        return 0;
    }
}
#endif

/*
 * substitution of getNthValue which dosen't destroy the original value
 */
int getNthValueSafe(int index, char *value, char delimit, char *result, int len)
{
    int i=0, result_len=0;
    char *begin, *end;

    if(!value || !result || !len)
        return -1;

    begin = value;
    end = strchr(begin, delimit);

    while(i<index && end){
        begin = end+1;
        end = strchr(begin, delimit);
        i++;
    }

    //no delimit
    if(!end){
		if(i == index){
			end = begin + strlen(begin);
			result_len = (len-1) < (end-begin) ? (len-1) : (end-begin);
		}else
			return -1;
	}else
		result_len = (len-1) < (end-begin)? (len-1) : (end-begin);

	memcpy(result, begin, result_len );
	*(result+ result_len ) = '\0';

	return 0;
}

/*
 *  argument:  [IN]     index -- the index array of deleted items(begin from 0)
 *             [IN]     count -- deleted itmes count.
 *             [IN/OUT] value -- original string/return string
 *             [IN]     delimit -- delimitor
 */
int deleteNthValueMulti(int index[], int count, char *value, char delimit)
{
	char *begin, *end;
	int i=0,j=0;
	int need_check_flag=0;
	char *buf = strdup(value);

	begin = buf;

	end = strchr(begin, delimit);
	while(end){
		if(i == index[j]){
			memset(begin, 0, end - begin );
			if(index[j] == 0)
				need_check_flag = 1;
			j++;
			if(j >=count)
				break;
		}
		begin = end;

		end = strchr(begin+1, delimit);
		i++;
	}

	if(!end && index[j] == i)
		memset(begin, 0, strlen(begin));

	if(need_check_flag){
		for(i=0; i<strlen(value); i++){
			if(buf[i] == '\0')
				continue;
			if(buf[i] == ';')
				buf[i] = '\0';
			break;
		}
	}

	for(i=0, j=0; i<strlen(value); i++){
		if(buf[i] != '\0'){
			value[j++] = buf[i];
		}
	}
	value[j] = '\0';

	free(buf);
	return 0;
}



/*
 * nanosleep(2) don't depend on signal SIGALRM and could cooperate with
 * other SIGALRM-related functions(ex. setitimer(2))
 */
unsigned int Sleep(unsigned int secs)
{
	int rc;
	struct timespec ts, remain;
	ts.tv_sec  = secs;
	ts.tv_nsec = 0;

sleep_again:
	rc = nanosleep(&ts, &remain);
	if(rc == -1 && errno == EINTR){
		ts.tv_sec = remain.tv_sec;
		ts.tv_nsec = remain.tv_nsec;
		goto sleep_again;
	}
	return 0;
}

/*
 * The setitimer() is Linux-specified.
 */
int setTimer(int microsec, void ((*sigroutine)(int)))
{
	struct itimerval value, ovalue;

	signal(SIGALRM, sigroutine);
	value.it_value.tv_sec = 0;
	value.it_value.tv_usec = microsec;
	value.it_interval.tv_sec = 0;
	value.it_interval.tv_usec = microsec;
	return setitimer(ITIMER_REAL, &value, &ovalue);
}

void stopTimer(void)
{
	struct itimerval value, ovalue;

	value.it_value.tv_sec = 0;
	value.it_value.tv_usec = 0;
	value.it_interval.tv_sec = 0;
	value.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL, &value, &ovalue);
}

/*
 * configure LED blinking with proper frequency (privatly use only)
 *   on: number of ticks that LED is on
 *   off: number of ticks that LED is off
 *   blinks: number of on/off cycles that LED blinks
 *   rests: number of on/off cycles that LED resting
 *   times: stop blinking after <times> times of blinking
 * where 1 tick == 100 ms
 */

#if 1 // Tommy debug +++++++++++++++++++++++ 
 
static int gpioLedSet(int gpio, unsigned int on, unsigned int off,
		unsigned int blinks, unsigned int rests, unsigned int times)
{
	int fd;
	ralink_gpio_led_info led;

	//parameters range check
	if (gpio < 0 || gpio >= RALINK_GPIO_NUMBER ||
			on > RALINK_GPIO_LED_INFINITY ||
			off > RALINK_GPIO_LED_INFINITY ||
			blinks > RALINK_GPIO_LED_INFINITY ||
			rests > RALINK_GPIO_LED_INFINITY ||
			times > RALINK_GPIO_LED_INFINITY) {
		return -1;
	}
	led.gpio = gpio;
	led.on = on;
	led.off = off;
	led.blinks = blinks;
	led.rests = rests;
	led.times = times;

	fd = open("/dev/gpio", O_RDONLY);
	if (fd < 0) {
		perror("/dev/gpio");
		return -1;
	}
	if (ioctl(fd, RALINK_GPIO_LED_SET, &led) < 0) {
		perror("ioctl");
		close(fd);
		return -1;
	}
	close(fd);
	return 0;
}

int ledAlways(int gpio, int on)
{
	if (on)
		return gpioLedSet(gpio, RALINK_GPIO_LED_INFINITY, 0, 1, 1, RALINK_GPIO_LED_INFINITY);
	else
		return gpioLedSet(gpio, 0, RALINK_GPIO_LED_INFINITY, 1, 1, RALINK_GPIO_LED_INFINITY);
}

int ledWps(int gpio, int mode)
{
	switch (mode) {
		case WPS_LED_RESET:
			return gpioLedSet(gpio, 0, RALINK_GPIO_LED_INFINITY, 1, 1, RALINK_GPIO_LED_INFINITY);
			break;
		case WPS_LED_PROGRESS:
			return gpioLedSet(gpio, 2, 1, RALINK_GPIO_LED_INFINITY, 1, RALINK_GPIO_LED_INFINITY);
			break;
		case WPS_LED_ERROR:
			return gpioLedSet(gpio, 1, 1, RALINK_GPIO_LED_INFINITY, 1, RALINK_GPIO_LED_INFINITY);
			break;
		case WPS_LED_SESSION_OVERLAP:
			return gpioLedSet(gpio, 1, 1, 10, 5, RALINK_GPIO_LED_INFINITY);
			break;
		case WPS_LED_SUCCESS:
			gpioLedSet(gpio, 3000, 1, 1, 1, 1);
			break;
	}
	return 0;
}
#endif // Tommy debug -----------------------------

/*
 * concatenate a string with an integer
 * ex: racat("SSID", 1) will return "SSID1"
 */
char *racat(char *s, int i)
{
	static char str[32];
	snprintf(str, 32, "%s%1d", s, i);
	return str;
}

void websLongWrite(webs_t wp, char *longstr)
{
    char tmp[513] = {0};
    int len = strlen(longstr);
    char *end = longstr + len;

    while(longstr < end){
        strncpy(tmp, longstr, 512);
        websWrite(wp, T("%s"), tmp);
        longstr += 512;
    }
    return;
}
static int
ej_lang(int eid, webs_t wp, int argc, char_t ** argv)
{
    char *str;
    char *trans;

    if (ejArgs(argc, argv, "%s", &str) < 1) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    //trans = lang_translate(str);
    return websWrite(wp, "%s", str);
}
static int
ej_increase(int eid, webs_t wp, int argc, char_t ** argv)
{
    int val, inc;

    if (ejArgs(argc, argv, "%d %d", &val, &inc) < 2) {
        //websDebugWrite(wp, "Insufficient args\n");
        return EZPLIB_INVALID;
    }

    return websWrite(wp, "%d", val+inc);
}
/*
 * ej family. Return 0 if everything is fine; return 1 if something wrong.
 */

static int
ej_nv_get(int eid, webs_t wp, int argc, char_t **argv)
{
    char *name, *c;
    int ret = 0;

    if (ejArgs(argc, argv, "%s", &name) < 1) {
        //websDebugWrite(wp, "Insufficient args\n");
        return -1;
    }

    for (c = nvram_safe_get(name); *c; c++) {
        if (isprint((int) *c) &&
            *c != '"' && *c != '&' && *c != '<' && *c != '>' && *c != '\'')
            ret += websWrite(wp, "%c", *c);
        else
            ret += websWrite(wp, "&#%d;", *c);
    }
    return ret;
}

static int
ej_nv_attr_get(int eid, webs_t wp, int argc, char_t **argv)
{
    char *name, *type;
    int nth;
    char buf[TMP_LEN];
    int ret;

    if (ejArgs(argc, argv, "%s %d %s", &name, &nth, &type) < 3) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    ret = ezplib_get_attr_val(name, nth, type, buf, TMP_LEN, EZPLIB_USE_CLI); 
    if (ret > 0) {
       websWrite(wp, "%s", buf);
    }

    return ret; 
}
/* It is abstracted from ej_nvg_attr_match() because this function is used by
 * bw.c as well. NOTE: if it doesn't match, NULL returns.
 */
char *
nvg_attr_match(char *tag, char *rule_set, int rule_num, char *attr, char *match,
        char *output)
{
    char *val;
    char buf[TMP_LEN];
    int ret;

    //val = GOZILA_GET(tag);
    //if (!val) {
        ret = 
            ezplib_get_attr_val(rule_set, rule_num, attr, buf, sizeof(buf), 
                    EZPLIB_USE_CLI);
        if (ret < 0) {
            return NULL;
        }
        val = buf;
    //}

    if (val && strcmp(val, match) == 0) {
        return output;
    } 
    return NULL;
}

int
ej_nvg_attr_get(int eid, webs_t wp, int argc, char_t **argv)
{
    int nth, ret;
    char *tag, *rule_set, *attr, *val;
    char buf[TMP_LEN];

    if (ejArgs(argc, argv, "%s %s %d %s", &tag, &rule_set, &nth, &attr) < 4) {
        return EZPLIB_INVALID;
    }

    //val = GOZILA_GET(tag);
   // if (!val) {
        ret = 
            ezplib_get_attr_val(rule_set, nth, attr, buf, sizeof(buf), 
                    EZPLIB_USE_CLI);
        if (ret < 0) {
            return ret;
        }
        val = buf;
  //  }
    return websWrite(wp, val);
}

int
ej_nvg_attr_match(int eid, webs_t wp, int argc, char_t **argv)
{
    char *tag, *rule_set, *attr, *match, *output, *selected;
    int nth;

    if (ejArgs(argc, argv, "%s %s %d %s %s %s", 
                &tag, &rule_set, &nth, &attr, &match, &output) < 6) {

        return EZPLIB_INVALID;
    }

    selected = nvg_attr_match(tag, rule_set, nth, attr, match, output);
    if (selected) {
        websWrite(wp, selected);
    }
    return 0;
}
/*
 * Example: 
 * wan_proto=dhcp
 * <% nvram_match("wan_proto", "dhcp", "selected"); %> produces "selected"
 * <% nv_match("wan_proto", "static", "selected"); %> does not produce
 */
static int
ej_nv_match(int eid, webs_t wp, int argc, char_t ** argv)
{
    char *name, *match, *output;

    if (ejArgs(argc, argv, "%s %s %s", &name, &match, &output) < 3) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    if (nvram_match(name, match))
        return websWrite(wp, output);

    return 0;
}

/*
 * Example: 
 * wan_proto=dhcp
 * <% nv_invmatch("wan_proto", "dhcp", "disabled"); %> does not produce
 * <% nv_invmatch("wan_proto", "static", "disabled"); %> produces "disabled"
 */
static int
ej_nv_invmatch(int eid, webs_t wp, int argc, char_t ** argv)
{
    char *name, *invmatch, *output;

    if (ejArgs(argc, argv, "%s %s %s", &name, &invmatch, &output) < 3) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    if (nvram_invmatch(name, invmatch))
        return websWrite(wp, output);

    return 0;
}

int
ej_tbl_get(int eid, webs_t wp, int argc, char **argv)
{
	char rule[LONG_BUF_LEN], tmp[LONG_BUF_LEN];
	char *rule_set;
	int num;
	/* 1-based. Attribute num is equal to the number of '^' plus 1. */
	int nattr = 1;
	char *cur;
	int i, j, rlen;

	if (ejArgs(argc, argv, "%s", &rule_set) < 1) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}    

	num = ezplib_get_rule_num(rule_set);

	websWrite(wp, "<script language='javascript'>\n");
	websWrite(wp, "var %s = ", rule_set);

	cur = tmp; 
	*cur++ = '['; 
	for (i = 0; i < num; i++) {
		rlen = ezplib_get_rule(rule_set, i, rule, LONG_BUF_LEN);
		/* The beginning of a rule to replace */
		cur += 
			snprintf(cur, tmp + LONG_BUF_LEN - cur, "%s['", (cur == tmp + 1) ? "" : ",");

		for (j = 0; j < rlen; j++) {
			if (rule[j] == '^') {
				cur += snprintf(cur, tmp + LONG_BUF_LEN - cur, "','");
				nattr++;
			}
			else {
				*cur++ = rule[j];
			}
		}
		cur += snprintf(cur, tmp + LONG_BUF_LEN - cur, "']");

		/* Dump for every rule in case 'tmp' is overflowed. */
		websWrite(wp, "%s", tmp);
		cur = tmp;
	}
	cur += snprintf(cur, tmp + LONG_BUF_LEN - cur, "];\n");
	websWrite(wp, "%s", tmp);

	websWrite(wp, "</script>");

	return 0;
}

int
ej_rule_get(int eid, webs_t wp, int argc, char **argv)
{
	char rule[LONG_BUF_LEN], tmp[LONG_BUF_LEN];
	char *rule_set;
	int num;
	/* 1-based. Attribute num is equal to the number of '^' plus 1. */
	int nattr = 1;
	char *cur;
	int cur_num;
	int i, j, rlen;

	if (ejArgs(argc, argv, "%s %d", &rule_set, &cur_num) < 2) {
		//websError(wp, 400, "Insufficient args\n");
		websWrite(wp, "Insufficient args\n");
		return -1;
	}    

	num = ezplib_get_rule_num(rule_set);

	websWrite(wp, "<script language='javascript'>\n");
	websWrite(wp, "var %s = %d;\n", "num", cur_num);
	websWrite(wp, "var %s = ", rule_set);
	
	cur = tmp;

	rlen = ezplib_get_rule(rule_set, cur_num, rule, LONG_BUF_LEN);
	/* The beginning of a rule to replace */
	cur += snprintf(cur, tmp + LONG_BUF_LEN - cur, "['");

	for (j = 0; j < rlen; j++) {
		if (rule[j] == '^') {
			cur += snprintf(cur, tmp + LONG_BUF_LEN - cur, "','");
			nattr++;
		}
		else {
			*cur++ = rule[j];
		}
	}
	cur += snprintf(cur, tmp + LONG_BUF_LEN - cur, "']");
	websWrite(wp, "%s", tmp);

	websWrite(wp, "</script>");

	return 0;
}

int
ej_tbl_create_table(int eid, webs_t wp, int argc, char **argv) 
{
	char *rname, *tname; 
	int mv;
	int max, nusr;
    char buf1[SHORT_BUF_LEN], buf2[SHORT_BUF_LEN];

    if (ejArgs(argc, argv, "%s %s %d", &tname, &rname, &mv) < 3) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }

    snprintf(buf1, SHORT_BUF_LEN, "%s_max", rname);
    snprintf(buf2, SHORT_BUF_LEN, "%s_num", rname);
    max = atoi(nvram_safe_get(buf1));
    nusr = atoi(nvram_safe_get(buf2));

    websWrite(wp, "<div id='%s'></div>", tname);
    websWrite(wp, "<input type='hidden' id='%s_max_rule_num' value='%d' />", tname, max);
    websWrite(wp, "<input type='hidden' id='%s_cur_rule_num' value='%d' />", tname, nusr);
    websWrite(wp, "<input type='hidden' id='%s_rule_flag' value='1' />", tname);
    websWrite(wp, "<input type='hidden' id='%s_rule_no' value='1' />", tname);
#if 0
    websWrite(wp, "<input type='button' id='%s_add' ", tname);
    websWrite(wp, "onClick='tbl_show_input(\"%s\", 1, %s);' value='%s' />",
                  tname, rname, "Add");
    websWrite(wp, "<input disabled type='button' id='%s_delete' ", tname);
    websWrite(wp, "onClick='tbl_del_tr(\"%s\", %s);' value='%s' />",
                   tname, rname, "Delete");
    websWrite(wp, "<input disabled type='button' id='%s_modify' ", tname);
    websWrite(wp, "onclick='tbl_show_input(\"%s\", 0, %s);' value='%s' />",
                   tname, rname, "Modify");

    if (mv == 1) {
        websWrite(wp, "<input disabled type='button' id='%s_up' ", tname);
        websWrite(wp, "onClick='tbl_moveup_tr(\"%s\", %s);' value='%s' />", 
                      tname, rname, "Up");
        websWrite(wp, "<input disabled type='button' id='%s_down' ", tname);
        websWrite(wp, "onclick='tbl_movedown_tr(\"%s\", %s);' value='%s' />",
                      tname, rname, "Down");
        websWrite(wp, "<input type='hidden' id='%s_enable_up_down' "
                            "value='true' />", tname);
    } else {
        websWrite(wp, "<input type='hidden' id='%s_enable_up_down' "
                            "value='false' />", tname);
    }
#endif
    return 1;
}
#if 0
/* william, for ipsec vpn --> ipsec vpn Liteon */
int
showIpsecVpnRulesASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char tmp[SHORT_BUF_LEN];
	char tmp_active[SHORT_BUF_LEN];
	char tmp_name[SHORT_BUF_LEN];
	char tmp_ip[SHORT_BUF_LEN];
	int i;
	int rule_num=0;

    rule_num=atoi(nvram_safe_get("ipsec_rule_num"));
    //printf("\n\n   rule_num=%d -> 111\n", rule_num);        
	
	for(i=0;i<rule_num;i++){

		websWrite(wp, T("<tr>\n"));
		// output No.
		websWrite(wp, T("<td valign=\"top\"><center><span class=\"table_left\">%d</span></center></td>"), i+1);

         // Name
		ezplib_get_attr_val("ipsec_rule", i, "name", tmp_name, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("name=%s\t",tmp_name);
		if(strlen(tmp_name))
			websWrite(wp, T("<td width=\"10%\"   align=center><center><span class=\"table_font\">%s</span></center></td>"), tmp_name);
		else
			websWrite(wp, T("<td width=\"10%\"   align=center><center><span class=\"table_font\">- </span></center></td>"));
		
		//Active     
        ezplib_get_attr_val("ipsec_rule", i, "enable", tmp_active, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("enable=%s\t",tmp_active);
		if( !strcmp(tmp_active, "1"))
			websWrite(wp, T("<td width=\"10%\"   align=center><center><span class=\"table_font\"><img src=\"images/i_active_on.gif\" width=\"18\" height=\"18\"  title=\"On\"> </span></center></td>"));
		else
			websWrite(wp, T("<td width=\"10%\"   align=center><center><span class=\"table_font\"><img src=\"images/i_active_off.gif\" width=\"18\" height=\"18\"  title=\"Off\"> </span></center></td>"));
		
        //Extern interface 
        ezplib_get_attr_val("ipsec_rule", i, "local_extif", tmp, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("extif=%s\t",tmp);
		websWrite(wp, T("<td width=\"10%\"   align=center><center><span class=\"table_font\">%s</span></center>"), tmp);
		
        //Remote gateway 
        ezplib_get_attr_val("ipsec_rule", i, "remote_gateway", tmp, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("proto=%s\t",tmp);
		websWrite(wp, T("<td width=\"10%\"   align=center><center><span class=\"table_font\">%s</span></center>"), tmp);

        //Remote iner ip address 
        ezplib_get_attr_val("ipsec_rule", i, "remote_inipaddr", tmp_ip, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("extport_start=%s\t",tmp_port);

		//Remote iner ip netmask 
        ezplib_get_attr_val("ipsec_rule", i, "remote_netmask", tmp, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("extport_end=%s\t",tmp);
		websWrite(wp, T("<td width=\"20%\"   align=center><center><span class=\"table_font\">%s / %s</span></center>"), tmp_ip, tmp);
        
        ezplib_get_attr_val("ipsec_status_rule", i, "phase1", tmp, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("inport_start=%s\t",tmp);
        if( !strcmp(tmp, "None"))
			websWrite(wp, T("<td width=\"5%\"   align=center><center><span class=\"table_font\"><img src=\"images/i_active_off.gif\" width=\"18\" height=\"18\"  title=\"Off\"> </span></center></td>"));
		else
			websWrite(wp, T("<td width=\"5%\"   align=center><center><span class=\"table_font\"><img src=\"images/i_active_on.gif\" width=\"18\" height=\"18\"  title=\"On\"> </span></center></td>"));
		
        ezplib_get_attr_val("ipsec_status_rule", i, "phase2", tmp, 
                SHORT_BUF_LEN, EZPLIB_USE_CLI);
        //printf("inport_end=%s\n",tmp);
         if( !strcmp(tmp, "None"))
			websWrite(wp, T("<td width=\"5%\"   align=center><center><span class=\"table_font\"><img src=\"images/i_active_off.gif\" width=\"18\" height=\"18\"  title=\"Off\"> </span></center></td>"));
		else
			websWrite(wp, T("<td width=\"5%\"   align=center><center><span class=\"table_font\"><img src=\"images/i_active_on.gif\" width=\"18\" height=\"18\"  title=\"On\"> </span></center></td>"));
		
		//Modify
		websWrite(wp, T("<td><center><span class=\"table_right\">"));
		websWrite(wp, T("<a href=\"javascript: doEditRule(%d);\" onmouseout=\"MM_swapImgRestore()\" onmouseover=\"MM_swapImage('fnatEImage%d','','images/i_edit_on.gif',1)\">"),i+1,i+1);
		websWrite(wp, T("<img src=\"images/i_edit.gif\" name=\"fnatEImage\" width=\"18\" height=\"18\" border=\"0\" id=\"fnatEImage%d\"  title=\"Edit\"/></a>"),i+1);
		websWrite(wp, T("&nbsp;&nbsp;<a href=\"javascript: doDeleteRule(%d);\" onmouseout=\"MM_swapImgRestore()\" onmouseover=\"MM_swapImage('fnatDImage%d','','images/i_delete_on.gif',1)\">"),i+1,i+1);
		websWrite(wp, T("<img src=\"images/i_delete.gif\" name=\"fnatDImage\" width=\"18\" height=\"18\" border=\"0\" id=\"fnatDImage%d\"  title=\"Delete\"/>"),i+1);
		websWrite(wp, T("</a></span></center></td>"));
		websWrite(wp, T("</tr>\n"));  
    }

	return 0;	
}


int
ej_get_ipsec_status(int eid, webs_t wp, int argc, char_t ** argv)
{ 
	    system("/usr/bin/vpnstatus");
		return 1;
}
#endif
/*
 * Example: 
 * wan_proto = dhcp; gozilla = 0;
 * <% nvg_match("wan_proto", "dhcp", "selected"); %> produces "selected"
 *
 * wan_proto = dhcp; gozilla = 1; websGetVar(wp, "wan_proto", NULL) = static;
 * <% nvg_match("wan_proto", "static", "selected"); %> produces "selected"
 */
int
ej_nvg_match(int eid, webs_t wp, int argc, char_t ** argv)
{
	char *name, *match, *output;
	//char *type;

	if (ejArgs(argc, argv, "%s %s %s", &name, &match, &output) < 3) { 
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}    

	if (nvram_match(name, match)) {
		return websWrite(wp, output);
	}    

	return 0;
}


int
ej_do_pagehead1(int eid, webs_t wp, int argc, char_t ** argv)
{
	websWrite(wp,
			"<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Strict//EN' '"
			"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'>\n");
	return 0;
}

int
ej_do_pagehead2(int eid, webs_t wp, int argc, char_t ** argv)
{
    char *no_table;

    /* It's allowed not to feed a parameter. */
    ejArgs(argc, argv, "%s", &no_table);

    websWrite(wp,
              "<meta http-equiv='Content-Type' content='"
              "application/xhtml+xml' />\n");
    websWrite(wp,
              "<link type='text/css' rel='stylesheet' href='"
              "style/style.css' />\n");
    if (!no_table || strcmp(no_table, "no_table")) {
        websWrite(wp,
                  "<link type='text/css' rel='stylesheet' href='"
                  "style/table.css' />\n");
    }

    websWrite(wp,
              "<!--[if lte IE 6]>\n"
              "<style type='text/css'>\n"
              "     #logo { behavior: url(iepngfix.htc); } \n"
              "</style>\n"
              "<link type='text/css' rel='stylesheet' href='"
              "style/ie6.css' />\n"
              "<![endif]-->\n");

    websWrite(wp,
              "<script type='text/javascript' src='images/common.js'></script>\n");
    websWrite(wp,
              "<script type='text/javascript' src='images/code.js'></script>\n");
    websWrite(wp,
              "<script type='text/javascript' src='images/svg-common.js'>"
              "</script>\n");
    return 0;
}
static void
strtoupper(char *upper, char *orig){
    if(!orig || !upper)
        return;
    while(orig && *orig){
        *upper = toupper(*orig);
        upper++;
        orig++;
    }
    *upper = 0;
}

#if 0
int
ej_ad_tools_showif(int eid, webs_t wp, int argc, char_t **argv){
    char *iface, *act, *rule_set = "adtool_rule", 
         buf[TMP_LEN], buf_sel[SHORT_BUF_LEN],
         upperbuf[SHORT_BUF_LEN];
    int num, i;

    if (ejArgs(argc, argv, "%s %s", &iface, &act) < 1) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }
    sprintf(buf, "%siface", act);
    ezplib_get_attr_val(rule_set, 0, buf, buf_sel, SHORT_BUF_LEN, EZPLIB_USE_CLI);
    sprintf(buf, "%s_num", iface);
    num = atoi(nvram_safe_get(buf));
    for (i = 0; i < num; i++){
#ifdef EZP_PROD_BRAND_ZYUS
        if (!strcmp(iface, "wan")) {
            switch (i) {
                case 0:
                    strcpy(upperbuf, "Ethernet WAN");
                    break;
                case 1 :
                    strcpy(upperbuf, "Mobile WAN");
                    break;
            }
        } else {
            strcpy(upperbuf, "LAN");
        }
#endif
        if(!strcmp(buf_sel, buf))
        {
            /* Selected WAN iface */
#ifdef EZP_PROD_BRAND_ZYUS
            sprintf(buf, "<option value=\"%s%d\" selected> %s</option>",
                                        iface, i, upperbuf);
#else
            strtoupper(upperbuf, iface);
            sprintf(buf, "<option value=\"%s%d\" selected> %s%d</option>", 
                    iface, i, upperbuf, i+1);
#endif
        }
        else
        {
            /* Unselected WAN iface */
#ifdef EZP_PROD_BRAND_ZYUS
            sprintf(buf, "<option value=\"%s%d\"> %s</option>",
                    iface, i, upperbuf);
#else
            strtoupper(upperbuf, iface);
            sprintf(buf, "<option value=\"%s%d\"> %s%d</option>", 
                    iface, i, upperbuf, i+1);
#endif
        }
        websWrite(wp, buf);
    }
    return 0;
}
#else
int
ej_ad_tools_showif(int eid, webs_t wp, int argc, char_t **argv){
    char *iface, *act, *rule_set = "adtool_rule", 
         buf[TMP_LEN], buf_sel[SHORT_BUF_LEN],
         upperbuf[SHORT_BUF_LEN];
    int num, i;

    if (ejArgs(argc, argv, "%s %s", &iface, &act) < 1) {
        websError(wp, 400, "Insufficient args\n");
        return -1;
    }
    sprintf(buf, "%siface", act);
    ezplib_get_attr_val(rule_set, 0, buf, buf_sel, SHORT_BUF_LEN, EZPLIB_USE_CLI);
    sprintf(buf, "%s_num", iface);
    
#ifdef EZP_PROD_BRAND_ZYUS
        if (!strcmp(iface, "wan")) {
			strcpy(upperbuf, "Ethernet WAN");
        } else {
            strcpy(upperbuf, "LAN");
        }
#endif
        if(!strcmp(buf_sel, buf))
        {
            /* Selected WAN iface */
#ifdef EZP_PROD_BRAND_ZYUS
            sprintf(buf, "<option value=\"%s\" selected> %s</option>",
                                        iface, upperbuf);
#else
            strtoupper(upperbuf, iface);
            sprintf(buf, "<option value=\"%s\" selected> %s</option>", 
                    iface, upperbuf);
#endif
        }
        else
        {
            /* Unselected WAN iface */
#ifdef EZP_PROD_BRAND_ZYUS
            sprintf(buf, "<option value=\"%s\"> %s</option>",
                    iface, upperbuf);
#else
            strtoupper(upperbuf, iface);
            sprintf(buf, "<option value=\"%s\"> %s</option>", 
                    iface, upperbuf);
#endif
        }
        websWrite(wp, buf);

    return 0;
}
#endif

int
wfputc(char c, FILE * fp)
{
#ifdef HTTPS_SUPPORT
    if (do_ssl)
        return BIO_printf((BIO *) fp, "%c", c);
    else
#endif
        return fputc(c, fp);
}

void
do_file(char *path, webs_t stream, ...)
{
    FILE *fp;
    char c;
	char ch;
    int count=0,i=0;

    if(!(fp=fopen(path,"r"))){
    	printf("cant open file!\n");
		return;
	}
    ch=fgetc(fp);
    while(ch!= EOF)
    {
        ch=fgetc(fp);
        count++;
    }

    char *buffer = (char*)malloc(count*sizeof(char));
	if(buffer != NULL)
	{
		memset(buffer,0,count*sizeof(char));
	}
	if (!(fp = fopen(path, "r")))
        return;
        printf("get value=\n");
    while ((c = getc(fp)) != EOF) {
        printf("%c",c);
        buffer[i] = c;
		i++;
    }
    fclose(fp);
	buffer[i] = '\0';
    websWrite(stream, buffer);
	printf("\n\nbuffer value=%s\n\n",buffer);
	free(buffer);
}

int
ej_st_show_traceroute(int eid, webs_t wp, int argc, char_t ** argv)
{
    system("grep 255.255.255.255 /tmp/debug > /tmp/debug1");
    char buf[100]={0};
    FILE *fp=NULL;

    if(NULL==(fp=fopen("/tmp/debug1","r")))
    {
         return 0;
     }
    fgets(buf,sizeof(buf),fp); 
    
    if(0==strncmp(buf,"cmd_line=traceroute",19))
    {
       printf("traceroute 255.255.255.255\n");
       system("echo \"traceroute: can not find interface\" >> /tmp/traceroute.tmp");
    }
 
    do_file("/tmp/traceroute.tmp", wp, NULL);
    system("rm /tmp/traceroute.tmp");
    return 1;
}

/*********************************************************************
 * Web Related Utilities
 */

void formDefineUtilities(void)
{
	
	websAspDefine(T("getCfgGeneral"), getCfgGeneral);
	websAspDefine(T("getCfgNthGeneral"), getCfgNthGeneral);
	websAspDefine(T("getCfgZero"), getCfgZero);
	websAspDefine(T("getCfgNthZero"), getCfgNthZero);
	websAspDefine(T("getCfg2General"), getCfg2General);
	websAspDefine(T("getCfg2NthGeneral"), getCfg2NthGeneral);
	websAspDefine(T("getCfg2Zero"), getCfg2Zero);
	websAspDefine(T("getCfg2NthZero"), getCfg2NthZero);
	websAspDefine(T("getCfg3General"), getCfg3General);
	websAspDefine(T("getCfg3Zero"), getCfg3Zero);
	websAspDefine(T("getDpbSta"), getDpbSta);
	websAspDefine(T("getLangBuilt"), getLangBuilt);
	websAspDefine(T("getMiiInicBuilt"), getMiiInicBuilt);
	websAspDefine(T("getPlatform"), getPlatform);
	websAspDefine(T("getStationBuilt"), getStationBuilt);
	websAspDefine(T("getSysBuildTime"), getSysBuildTime);
//	websAspDefine(T("getSdkVersion"), getSdkVersion);
	websAspDefine(T("getSysUptime"), getSysUptime);
#if 1//Arthur Chow 2009-01-03
	websAspDefine(T("getSysResource"), getSysResource);
#endif
	websAspDefine(T("getPortStatus"), getPortStatus);
	websAspDefine(T("isOnePortOnly"), isOnePortOnly);
//	websFormDefine(T("forceMemUpgrade"), forceMemUpgrade);
//	websFormDefine(T("setOpMode"), setOpMode);
// Tommy, 2008/12/16 08:14下午
	websFormDefine(T("SetOperationMode"), SetOperationMode);
// Steve, 2009/01/16 02:45下午
	websAspDefine(T("getCfgLLTD"), getCfgLLTD);
#if 1//Arthur Chow 2009-02-15: For easy mode page
	websAspDefine(T("startLLTDcheck"), startLLTDcheck);
	websAspDefine(T("startInternetcheck"), startInternetcheck);
	websAspDefine(T("statusRoutercheck"), statusRoutercheck);
	websAspDefine(T("statusInternetcheck"), statusInternetcheck);
	websAspDefine(T("javascriptLLTD"), javascriptLLTD);
	websFormDefine(T("easy_setting"), easy_setting);
	websAspDefine(T("clearLLTDInfo"), clearLLTDInfo);
	websAspDefine(T("existLLTDinfo"), existLLTDinfo);
#endif
#if 1//Arthur Chow 2009-03-30
	websAspDefine(T("getFirmwareVersion"), getFirmwareVersion);
	websAspDefine(T("getFirmwareDate"), getFirmwareDate);
#endif
//Steve add 2010-08-27
	websAspDefine(T("getGetUploadFlag"), getGetUploadFlag);
//kissth add sys_tools
	websAspDefine(T("ad_tools_showif"), ej_ad_tools_showif);
	websAspDefine(T("st_show_traceroute"), ej_st_show_traceroute);
	websAspDefine(T("lang"), ej_lang);
	websAspDefine(T("increase"), ej_increase);
	websAspDefine(T("nv_get"), ej_nv_get);
	websAspDefine(T("nv_attr_get"), ej_nv_attr_get);
	websAspDefine(T("nvg_attr_get"), ej_nvg_attr_get);
	websAspDefine(T("nvg_attr_match"), ej_nvg_attr_match);
	websAspDefine(T("nv_match"), ej_nv_match);
	websAspDefine(T("nv_invmatch"), ej_nv_invmatch);
//william add for ipsec start
	websAspDefine(T("tbl_get"), ej_tbl_get);

	websAspDefine(T("rule_get"), ej_rule_get);

	websAspDefine(T("tbl_create_table"), ej_tbl_create_table);
	websAspDefine(T("nvg_match"), ej_nvg_match);
//	websAspDefine(T("get_ipsec_status"), ej_get_ipsec_status);
//	websAspDefine(T("showIpsecVpnRulesASP"), showIpsecVpnRulesASP);
//end
	websAspDefine(T("do_pagehead2"), ej_do_pagehead2);
	websAspDefine(T("do_pagehead1"), ej_do_pagehead1);
}


/*
 * arguments: type - 0 = return the configuration of 'field' (default)
 *                   1 = write the configuration of 'field'
 *            field - parameter name in nvram
 * description: read general configurations from nvram
 *              if value is NULL -> ""
 */
static int getCfgGeneral(int eid, webs_t wp, int argc, char_t **argv)
{
	int type;
	char_t *field;
	char *value=NULL;
	char TempBuf[32];
        char buf[TMP_LEN], tmp_SSID[TMP_LEN], *p_ssid, *p_SSID; //aron add
	int ret; //aron add
	int idx; //aron add

	int i, j;//Steve
	char buf2[TMP_LEN]; //Steve add
	char *pTempBuf;	

	if (ejArgs(argc, argv, T("%d %s"), &type, &field) < 2) {
		return websWrite(wp, T("Insufficient args\n"));
	}
	
	//Steve
	printf("\n ---> getCfgGeneral(): type = %d & field = %s",type,field);
	if (!strcmp(field, "Language")){
		/* Get the setting of value from AXIMCom's nvram structure into reValue */
		value = nvram_safe_get("lang");
	}else if (!strcmp(field, "WeatherDegree")){ 
		ezplib_get_attr_val("weather_rule", 0, "degree", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf); 
		value = reValue;	
	}else if (!strcmp(field, "WeatherCity")){ 
		ezplib_get_attr_val("weather_rule", 0, "city", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf); 
		value = reValue;	
	}else if (!strcmp(field, "SystemName")){
		value = nvram_safe_get("hostname");
	}else if (!strcmp(field, "SSID")){
		ezplib_get_attr_val("wl0_ssid_rule", 0, "ssid", buf, 33, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
		char *str;
		if(strstr(reValue,"<")){
		    str = sub_replace(reValue,"<","&#60;");
		    printf("-----------ssid value=%s\n",str);
		    strcpy(reValue,str);
		    free(str);
		}
		value = reValue;
	}
	else if (!strcmp(field, "SSID1")){
		ezplib_get_attr_val("wl0_ssid_rule", 1, "ssid", buf, 33, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;	
	}
	else if (!strcmp(field, "SSID2")){
		ezplib_get_attr_val("wl0_ssid_rule", 2, "ssid", buf, 33, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;	
	}
	else if (!strcmp(field, "SSID3")){
		ezplib_get_attr_val("wl0_ssid_rule", 3, "ssid", buf, 33, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;	
	}
	else if (!strcmp(field, "SSID4")){
		ezplib_get_attr_val("wl0_ssid_rule", 4, "ssid", buf, 33, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;	
	}
	else if (!strcmp(field, "SSID5")){
		ezplib_get_attr_val("wl0_ssid_rule", 5, "ssid", buf, 33, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;	
	}
	else if (!strcmp(field, "SSID6")){
		ezplib_get_attr_val("wl0_ssid_rule", 6, "ssid", buf, 33, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;	
	}
	else if (!strcmp(field, "SSID7")){
		ezplib_get_attr_val("wl0_ssid_rule", 7, "ssid", buf, 33, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;	
	}
	else if (!strcmp(field, "SSID5G1")){
		ezplib_get_attr_val("wl1_ssid_rule", 0, "ssid", buf, 33, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		char *str;
		if(strstr(reValue,"<")){
		    str = sub_replace(reValue,"<","&#60;");
		    printf("-----------ssid value=%s\n",str);
		    strcpy(reValue,str);
		    free(str);
		}
		value = reValue;
	}else if (!strcmp(field, "SSID5G2")){
		ezplib_get_attr_val("wl1_ssid_rule", 1, "ssid", buf, 33, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;	
	}else if (!strcmp(field, "SSID5G3")){
		ezplib_get_attr_val("wl1_ssid_rule", 2, "ssid", buf, 33, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;	
	}else if (!strcmp(field, "SSID5G4")){
		ezplib_get_attr_val("wl1_ssid_rule", 3, "ssid", buf, 33, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;	
	}else if (!strcmp(field, "lan_ipaddr")){ 
		value = nvram_safe_get("lan0_ipaddr");
	}else if (!strcmp(field, "lan_pcipaddr")){ 
	      value = websGetRequestIpaddr(wp);	
	}	
	else if (!strcmp(field, "lan_ipaddr2")){ 
		ezplib_get_attr_val("lan_static_rule", 0, "ipaddr", buf, 33, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;
	}else if (!strcmp(field, "lan_netmask")){
		value = nvram_safe_get("lan0_mask");
		idx = atoi(value);
		value = prefix2mask(idx); //Translate AXIM's netmask to AboCom's UI format
	}
    else if (!strcmp(field, "fr_rule_num")){ 
		value = nvram_safe_get("fr_rule_num");
	}
	else if(!strcmp(field,"nat_port")){
		char *nv_rule_num = "fr_rule_num";
		int port_num;
		char strbuf[512];
		int j=0;
		port_num = atoi(nvram_safe_get(nv_rule_num));
		for(i=0; i< port_num;i++){
			ezplib_get_attr_val("fr_rule", i, "extport_start", buf, TMP_LEN, EZPLIB_USE_CLI);
			if(i!=0){
				sprintf(buf2,";%s",buf);
				strncpy(strbuf+j,buf2,strlen(buf2));
				j=j+strlen(buf2);
			}else{
				//printf("buf=%s strlen(buf)=%d\n", buf, strlen(buf));
				strncpy(strbuf+j,buf,strlen(buf));
				j=strlen(buf);
			}
		}
		printf("strbuf=%s\n", strbuf);
		value = strbuf;
	}
	else if (!strcmp(field, "rt_rule_num")){ 
		value = nvram_safe_get("rt_rule_num");
	}
	else if(!strcmp(field,"route_name")){
		char *rt_rule_num = "rt_rule_num";
		int name_num;
		char strbuf[512];
		int j=0;
		name_num = atoi(nvram_safe_get(rt_rule_num));
		for(i=0; i< name_num;i++){
			ezplib_get_attr_val("rt_rule", i, "name", buf, TMP_LEN, EZPLIB_USE_CLI);
			if(i!=0){			
			     sprintf(buf2,";%s",buf);
         		     strncpy(strbuf+j,buf2,strlen(buf2));
         		     j=j+strlen(buf2);
        	      }else{
        		//printf("buf=%s strlen(buf)=%d\n", buf, strlen(buf));
        		    strncpy(strbuf+j,buf,strlen(buf));
        		    j=strlen(buf);
        	     }
		}
		printf("route name strbuf=%s\n", strbuf);
		value = strbuf;
	}
	else if (!strcmp(field, "RemoteManagementPort")){
		//Because debug,temp
		#if 1
		ezplib_get_attr_val("http_rule", 0, "port", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf); 
		value = reValue;	
	    #else
	    	strcpy(reValue, "80");
	    	value = reValue;
	    #endif	
	}
	else if (!strcmp(field, "RemoteManagementSecuredIP")){
		ezplib_get_attr_val("http_rule", 0, "secipaddr", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);	
		if(!strlen(TempBuf))
		strcpy(reValue, "0.0.0.0"); 
		else
			strcpy(reValue, TempBuf); 	
		value = reValue;
	}		
	else if (!strcmp(field, "websHostFilters")){
		char *nv_rule_num = "wf_rule_num";
		int r_num;
		char strbuf[2048];
		int j=0;
		r_num = atoi(nvram_safe_get(nv_rule_num));
		printf("\n");
		for(i=0; i< r_num;i++){
			ezplib_get_attr_val("wf_rule", i, "keyword", buf, TMP_LEN, EZPLIB_USE_CLI);
			if(i!=0){			
				sprintf(buf2,";%s",buf);
        		strncpy(strbuf+j,buf2,strlen(buf2));
        		j=j+strlen(buf2);
        	}else{
        		//printf("buf=%s strlen(buf)=%d\n", buf, strlen(buf));
        		strncpy(strbuf+j,buf,strlen(buf));
        		j=strlen(buf);
        	}
		}
		printf("strbuf=%s\n", strbuf);
		value = strbuf;
	}
    //---Steve Start 
    else if (!strcmp(field, "DomainName")){
		ezplib_get_attr_val("lan_dhcps_rule", 0, "domain", buf, TMP_LEN, EZPLIB_USE_CLI);	
		strcpy(reValue, buf); 	
		value = reValue;	
	}		
	else if (!strcmp(field, "AdminInactivityTimer")){
		ezplib_get_attr_val("http_rule", 0, "adm_timeout", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);	
		strcpy(reValue, TempBuf); 	
		value = reValue;	
	}
	else if (!strcmp(field, "AdminCurUser")){
		ezplib_get_attr_val("http_rule", 0, "curusername", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);	
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "igmpEnabled")){
		pTempBuf = nvram_safe_get("igmp_proxy_rule");
		strcpy(reValue, pTempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WANPingFilter")){
		ezplib_get_attr_val("fw_rule", 0, "icmp_ping_enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf); 
		value = reValue;
	}
	else if (!strcmp(field, "fl_rule_num")){ 
		value = nvram_safe_get("fl_rule_num");
	}
    //---Steve End
        //--------------------------------------------- aron patch for giga
	else if (!strcmp(field, "wanConnectionMode")) //wan connection mode
	{ 
		value = nvram_get("wan0_proto");
		if (!strcmp(value, "dhcp"))
		    value =  "DHCP";
		else if(!strcmp(value, "static"))
		    value =  "STATIC";
		else if(!strcmp(value, "pppoe"))
		    value =  "PPPOE";
		else if(!strcmp(value, "pptp"))
		    value =  "PPTP";
		else if(!strcmp(value, "l2tp"))
		    value =  "L2TP";
		else if(!strcmp(value, "wwan"))
		    value =  "WWAN";
	}
	else if (!strcmp(field, "wan_pptp_user")) //wan0 pptp username
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "username", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_pptp_pass")) //wan0 pptp password
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "passwd", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_pptp_optime")) //wan0 pptp idletime
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "timeout", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_pptp_server")) //wan0 pptp server
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "serverip", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_pptp_ip")) //wan0 pptp physical IP Addr
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_pptp_netmask")) //wan0 pptp physical IP netmask
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "netmask", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        idx = atoi(reValue);
	        value = prefix2mask(idx); //Translate AXIM's netmask to AboCom's UI format
	}
	else if (!strcmp(field, "wan_pptp_gateway")) //wan0 pptp physical IP gateway
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "gateway", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_pptp_wanip_addr")) //wan0 pptp PPP IP Addr
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "custom_ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_l2tp_user")) //wan0 l2tp username
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "username", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_l2tp_pass")) //wan0 l2tp password
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "passwd", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_l2tp_optime")) //wan0 l2tp idletime
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "timeout", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_l2tp_server")) //wan0 l2tp server
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "serverip", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_l2tp_ip")) //wan0 l2tp physical IP Addr
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_l2tp_netmask")) //wan0 l2tp physical IP netmask
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "netmask", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        idx = atoi(reValue);
	        value = prefix2mask(idx); //Translate AXIM's netmask to AboCom's UI format
	}
	else if (!strcmp(field, "wan_l2tp_gateway")) //wan0 l2tp physical IP gateway
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "gateway", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_l2tp_wanip_addr")) //wan0 l2tp PPP IP Addr
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "custom_ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_ipaddr")) //wan0 static ip address
	{
		ezplib_get_attr_val("wan_static_rule", 0, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_netmask")) //wan0 static netmask
	{
		    ezplib_get_attr_val("wan_static_rule", 0, "mask", buf, TMP_LEN, EZPLIB_USE_CLI); 
	            strcpy(reValue, buf);
	            idx = atoi(reValue);
	            value = prefix2mask(idx); //Translate AXIM's netmask to AboCom's UI format
	}
	else if (!strcmp(field, "wan_gateway")) //wan0 static gateway
	{
		ezplib_get_attr_val("wan_static_rule", 0, "gateway", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	        
	}
	else if (!strcmp(field, "wan_primary_dns")) //wan0 (static) primary dns
	{
		ezplib_get_attr_val("wan_static_rule", 0, "dns1", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
		value = reValue;
	}
        else if (!strcmp(field, "wan_secondary_dns")) //wan0 (static) secondary dns
	{
		ret = ezplib_get_attr_val("wan_static_rule", 0, "dns2", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;   
	}
	else if (!strcmp(field, "wan_pppoe_user")) //wan0 pppoe username
	{
		ezplib_get_attr_val("wan_pppoe_rule", 0, "username", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	        
	}
	else if (!strcmp(field, "wan_pppoe_pass")) //wan0 pppoe password
	{
		ezplib_get_attr_val("wan_pppoe_rule", 0, "passwd", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	        
	}
	else if (!strcmp(field, "wan_pppoe_mtu")) //wan0 pppoe mtu size
	{
		ezplib_get_attr_val("wan_pppoe_rule", 0, "mtu", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	        
	}
	else if (!strcmp(field, "wan_pppoe_srvname")) //wan0 pppoe service name
	{
		ezplib_get_attr_val("wan_pppoe_rule", 0, "servname", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	        
	}
	else if (!strcmp(field, "wan_pppoe_optime")) //wan0 pppoe optime
	{
		ezplib_get_attr_val("wan_pppoe_rule", 0, "idletime", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_pppoe_wanip_addr")) //wan0 pppoe PPP IP Addr
	{
		ezplib_get_attr_val("wan_pppoe_rule", 0, "custom_ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_dns1_type")) //wan0 primary dns type
	{
		ezplib_get_attr_val("wan_dns_rule", 0, "dnstype1", buf, TMP_LEN, EZPLIB_USE_CLI); 
		if (!strcmp(buf, "ispdns"))
		     value =  "ISP";
		else if (!strcmp(buf, "custom"))
		     value =  "USER";
		else if (!strcmp(buf, "none"))
		     value =  "NONE";
	}
	else if (!strcmp(field, "wan_dns2_type")) //wan0 primary dns type
	{
		ezplib_get_attr_val("wan_dns_rule", 0, "dnstype2", buf, TMP_LEN, EZPLIB_USE_CLI); 
		if (!strcmp(buf, "ispdns"))
		     value =  "ISP";
		else if (!strcmp(buf, "custom"))
		     value =  "USER";
		else if (!strcmp(buf, "none"))
		     value =  "NONE";
	}			
	else if (!strcmp(field, "macCloneMac")) //wan0 clone mac address
	{
		ezplib_get_attr_val("wan_hwaddr_clone_rule", 0, "hwaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	        
	}
	else if (!strcmp(field, "spoofIpCloneMac")) //wan0 clone ip address
	{
		ezplib_get_attr_val("wan_hwaddr_clone_rule", 0, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	        
	}
	else if (!strcmp(field, "apn_3g")) //wan0 3g apn
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "apn", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	        
	}
	else if (!strcmp(field, "pin_3g")) //wan0 3g pin
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "pin", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "username_3g")) //wan0 3g username
	{
		ret = ezplib_get_attr_val("wan_wwan_rule", 0, "username", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "password_3g")) //wan0 3g password
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "passwd", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "dial_3g")) //wan0 3g dial number
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "dialstr", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "max_idle_time")) //wan0 3g ppp idle time
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "idletime", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "ppp_echo_interval")) //wan0 3g ppp echo interval
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "redialperiod", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "ppp_retry_threshold")) //wan0 3g ppp retry threshold
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "echo_failure", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "mtu_3g")) //wan0 3g mtu
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "mtu", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "dhcpStart")) //lan0 dhcp start ip
	{
		ezplib_get_attr_val("lan_dhcps_rule", 0, "start", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "lan_dhcp_pool_size")) //lan0 dhcp pool size
	{
		ezplib_get_attr_val("lan_dhcps_rule", 0, "num", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "dhcp_dns1_type")) //lan0 dhcp first dns type
	{
		ezplib_get_attr_val("lan_dhcps_rule", 0, "dnstype", buf, TMP_LEN, EZPLIB_USE_CLI); 
		if (!strcmp(buf, "dnsrelay"))
		     value =  "RELAY";
		else if (!strcmp(buf, "ispdns"))
		     value =  "ISP";
		else if (!strcmp(buf, "custom"))
		     value =  "USER";
		else if (!strcmp(buf, "none"))
		     value =  "NONE";
		else if (!strcmp(buf, "opendns"))
		     value =  "OPENDNS";
		else if (!strcmp(buf, "googledns"))
		     value =  "GOOGLEDNS";
	}
	else if (!strcmp(field, "dhcpPriDns")) //lan0 dhcp primary dns
	{
		ezplib_get_attr_val("lan_dhcps_rule", 0, "dnsaddr1", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "dhcp_dns2_type")) //lan0 dhcp secondary dns type
	{
		ezplib_get_attr_val("lan_dhcps_rule", 0, "dnstype2", buf, TMP_LEN, EZPLIB_USE_CLI); 
		if (!strcmp(buf, "dnsrelay"))
		     value =  "RELAY";
		else if (!strcmp(buf, "ispdns"))
		     value =  "ISP";
		else if (!strcmp(buf, "custom"))
		     value =  "USER";
		else if (!strcmp(buf, "none"))
		     value =  "NONE";
		else if (!strcmp(buf, "opendns"))
		     value =  "OPENDNS";
		else if (!strcmp(buf, "googledns"))
		     value =  "GOOGLEDNS";
	}
	else if (!strcmp(field, "dhcpSecDns")) //lan0 dhcp secondary dns
	{
		ezplib_get_attr_val("lan_dhcps_rule", 0, "dnsaddr2", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	
	else if (!strcmp(field, "DDNSProvider")) //wan0 ddns provider
	{
		ezplib_get_attr_val("wan_ddns_rule", 0, "type", buf, TMP_LEN, EZPLIB_USE_CLI); 
		if (!strcmp(buf, "dyndns"))
		     value =  "dyndns.org";
		else if (!strcmp(buf, "noip"))
		     value =  "no-ip.com";
		else if (!strcmp(buf, "eurodns"))
		     value =  "eurodyndns";
		else if (!strcmp(buf, "regfish"))
		     value =  "regfish";
	}
	else if (!strcmp(field, "DDNS")) //wan0 ddns Hostname
	{
		ezplib_get_attr_val("wan_ddns_rule", 0, "hostname", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "DDNSAccount")) //wan0 ddns user name
	{
		ezplib_get_attr_val("wan_ddns_rule", 0, "username", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "DDNSPassword")) //wan0 ddns password
	{
		ezplib_get_attr_val("wan_ddns_rule", 0, "passwd", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "DDNSCustomServer")) //wan0 ddns server
	{
		ezplib_get_attr_val("wan_ddns_rule", 0, "server", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "TIME_servtype")) //ntp time setting type
	{
		ezplib_get_attr_val("ntp_rule", 0, "custom_time", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        //value = reValue;
	        idx = atoi(reValue);
	        if(idx==0)      
	            value="1";
	        else if(idx==1) //AXIM treat "manual time" as "1"
	            value="0";
	}
	else if (!strcmp(field, "current_hour")) //ntp manual hour
	{
		ezplib_get_attr_val("ntp_rule", 0, "hour", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "current_Min")) //ntp manual minute
	{
		ezplib_get_attr_val("ntp_rule", 0, "min", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "current_Sec")) //ntp manual second
	{
		ezplib_get_attr_val("ntp_rule", 0, "sec", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "current_Year")) //ntp manual year
	{
		ezplib_get_attr_val("ntp_rule", 0, "year", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "current_Mon")) //ntp manual month
	{
		ezplib_get_attr_val("ntp_rule", 0, "mon", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "current_Day")) //ntp manual day
	{
		ezplib_get_attr_val("ntp_rule", 0, "date", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "enabledaylight")) //ntp daylight saving enable
	{
		ezplib_get_attr_val("ntp_rule", 0, "daylight_saving", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "startMon")) //ntp daylight saving start month
	{
		ezplib_get_attr_val("ntp_rule", 0, "ds_start_mon", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "startDay")) //ntp daylight saving start day
	{
		ezplib_get_attr_val("ntp_rule", 0, "ds_start_day", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "startclock")) //ntp daylight saving start clock
	{
		ezplib_get_attr_val("ntp_rule", 0, "ds_start_hour", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "endMon")) //ntp daylight saving end month
	{
		ezplib_get_attr_val("ntp_rule", 0, "ds_end_mon", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "endDay")) //ntp daylight saving end day
	{
		ezplib_get_attr_val("ntp_rule", 0, "ds_end_day", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "endclock")) //ntp daylight saving end clock
	{
		ezplib_get_attr_val("ntp_rule", 0, "ds_end_hour", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "NTP_servtype")) //ntp server type
	{
		/*
		ezplib_get_attr_val("ntp_rule", 0, "type", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        if (!strcmp(buf, "pool"))
		     value =  "0";
		else if (!strcmp(buf, "ipaddr"))
		     value =  "1";
		*/
		ezplib_get_attr_val("ntp_rule", 0, "custom_server", buf, TMP_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "NTPServerIP")) //ntp server ip
	{
		//ezplib_get_attr_val("ntp_rule", 0, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI);
		ezplib_get_attr_val("ntp_rule", 0, "serv_ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;        
	}
	else if (!strcmp(field, "TZ")) //ntp time zone
	{
		ezplib_get_attr_val("ntp_rule", 0, "zone", buf, TMP_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "dhcpStaticMac1")) //dhcp static lease MAC 1
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 0, "hwaddr", buf, TMP_LEN, EZPLIB_USE_CLI);
	        if(!strcmp(buf,""))
	        {
	            value = "00:00:00:00:00:00";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "dhcpStaticIp1")) //dhcp static lease IP 1
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 0, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI);
	        if(!strcmp(buf,""))
	        {
	            value = "0.0.0.0";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "dhcpStaticMac2")) //dhcp static lease MAC 2
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 1, "hwaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        if(!strcmp(buf,""))
	        {
	            value = "00:00:00:00:00:00";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "dhcpStaticIp2")) //dhcp static lease IP 2
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 1, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        if(!strcmp(buf,""))
	        {
	            value = "0.0.0.0";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "dhcpStaticMac3")) //dhcp static lease MAC 3
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 2, "hwaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        if(!strcmp(buf,""))
	        {
	            value = "00:00:00:00:00:00";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "dhcpStaticIp3")) //dhcp static lease IP 3
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 2, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        if(!strcmp(buf,""))
	        {
	            value = "0.0.0.0";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "dhcpStaticMac4")) //dhcp static lease MAC 4
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 3, "hwaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        if(!strcmp(buf,""))
	        {
	            value = "00:00:00:00:00:00";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "dhcpStaticIp4")) //dhcp static lease IP 4
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 3, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        if(!strcmp(buf,""))
	        {
	            value = "0.0.0.0";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "dhcpStaticMac5")) //dhcp static lease MAC 5
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 4, "hwaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        if(!strcmp(buf,""))
	        {
	            value = "00:00:00:00:00:00";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "dhcpStaticIp5")) //dhcp static lease IP 5
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 4, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        if(!strcmp(buf,""))
	        {
	            value = "0.0.0.0";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "dhcpStaticMac6")) //dhcp static lease MAC 6
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 5, "hwaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        if(!strcmp(buf,""))
	        {
	            value = "00:00:00:00:00:00";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "dhcpStaticIp6")) //dhcp static lease IP 6
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 5, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        if(!strcmp(buf,""))
	        {
	            value = "0.0.0.0";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "dhcpStaticMac7")) //dhcp static lease MAC 7
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 6, "hwaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        if(!strcmp(buf,""))
	        {
	            value = "00:00:00:00:00:00";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "dhcpStaticIp7")) //dhcp static lease IP 7
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 6, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        if(!strcmp(buf,""))
	        {
	            value = "0.0.0.0";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "dhcpStaticMac8")) //dhcp static lease MAC 8
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 7, "hwaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        if(!strcmp(buf,""))
	        {
	            value = "00:00:00:00:00:00";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "dhcpStaticIp8")) //dhcp static lease IP 8
	{
		ezplib_get_attr_val("fl_hwaddr_rule", 7, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        if(!strcmp(buf,""))
	        {
	            value = "0.0.0.0";
	        }
	        else
	        {
	            strcpy(reValue, buf);
	            value = reValue;
	        }
	}
	else if (!strcmp(field, "RIPversion")) //RIP
	{
		ezplib_get_attr_val("rt_rip_rule", 0, "rip", buf, TMP_LEN, EZPLIB_USE_CLI); 
		if (!strcmp(buf, "none"))
		     value =  "0";
		else if (!strcmp(buf, "v1"))
		     value =  "1";
		else if (!strcmp(buf, "v2"))
		     value =  "2";
	}
	else if (!strcmp(field, "igmpEnabled")) //igmp proxy enable
	{
		ezplib_get_attr_val("igmp_proxy_rule", 0, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "lan2_ipaddr")) //lan ip alias IP Addr
	{
		ezplib_get_attr_val("lan_static_rule", 0, "alias_ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "lan2_netmask")) //lan ip alias netmask
	{
		ezplib_get_attr_val("lan_static_rule", 0, "alias_netmask", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
		idx = atoi(reValue);
		value = prefix2mask(idx); //Translate AXIM's netmask to AboCom's UI format
	}
	else if (!strcmp(field, "bigPond_username")) //wan bigpond username
	{
		ezplib_get_attr_val("wan_bigpond_rule", 0, "username", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "bigPond_pass")) //wan bigpond password
	{
		ezplib_get_attr_val("wan_bigpond_rule", 0, "passwd", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	//-------------------------- Aron patch end
// WDS Start +++
	else if (!strcmp(field, "WdsList")){
		memset(reValue,0,sizeof(reValue));
		for (i = 0; i < 4; i++){		
			ezplib_get_attr_val("wl0_wds_basic_rule", i, "hwaddr", TempBuf, 32, EZPLIB_USE_CLI);
			if (!strcmp(TempBuf, "")){
				break;
			}else{
				strcat(reValue, TempBuf);
				strcat(reValue,";");
			}	
    	}
		value = reValue;
	}
	else if (!strcmp(field, "WdsEncrypType")){
		ezplib_get_attr_val("wl0_wds_basic_rule", 0, "secmode", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "disabled")){
			strcpy(reValue, "NONE");
		}else if (!strcmp(TempBuf, "wep")){
			strcpy(reValue, "WEP");
		}else if (!strcmp(TempBuf, "psk")){
			ezplib_get_attr_val("wl0_wds_sec_wpa_rule", 0, "crypto", TempBuf, 32, EZPLIB_USE_CLI);
			if (!strcmp(TempBuf, "tkip")){
				strcpy(reValue, "TKIP");
			}else if (!strcmp(TempBuf, "aes")){
				strcpy(reValue, "AES");
			}
		}
		value = reValue;
	}
	else if (!strcmp(field, "Wds0Key")){
			ezplib_get_attr_val("wl0_wds_sec_wep_rule", 0, "key", buf, 65, EZPLIB_USE_CLI);
			strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "Wds5GList")){
	       memset(reValue,0,sizeof(reValue));
		for (i = 0; i < 4; i++){		
			ezplib_get_attr_val("wl1_wds_basic_rule", i, "hwaddr", TempBuf, 32, EZPLIB_USE_CLI);
			if (!strcmp(TempBuf, "")){
				break;
			}else{
				strcat(reValue, TempBuf);
				strcat(reValue,";");
			}	
    	}
		value = reValue;
	}
	else if (!strcmp(field, "Wds5GEncrypType")){
		ezplib_get_attr_val("wl1_wds_basic_rule", 0, "secmode", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "disabled")){
			strcpy(reValue, "NONE");
		}else if (!strcmp(TempBuf, "wep")){
			strcpy(reValue, "WEP");
		}else if (!strcmp(TempBuf, "psk")){
			ezplib_get_attr_val("wl1_wds_sec_wpa_rule", 0, "crypto", TempBuf, 32, EZPLIB_USE_CLI);
			if (!strcmp(TempBuf, "tkip")){
				strcpy(reValue, "TKIP");
			}else if (!strcmp(TempBuf, "aes")){
				strcpy(reValue, "AES");
			}
		}
		value = reValue;
	}
	else if (!strcmp(field, "Wds0Key")){
			ezplib_get_attr_val("wl1_wds_sec_wep_rule", 0, "key", buf, 65, EZPLIB_USE_CLI);
			strcpy(reValue, TempBuf);
		value = reValue;
	}

// WDS End ---	
// LogSetting Start +++
	else if (!strcmp(field, "log_SystemMaintenance")){
			ezplib_get_attr_val("log_rule", 0, "web_mgmt", TempBuf, 32, EZPLIB_USE_CLI);
			strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "log_dns")){
			ezplib_get_attr_val("log_rule", 0, "dns", TempBuf, 32, EZPLIB_USE_CLI);
			strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "log_PPP")){
			ezplib_get_attr_val("log_rule", 0, "ppp", TempBuf, 32, EZPLIB_USE_CLI);
			strcpy(reValue, TempBuf);
		value = reValue;
	}	
	else if (!strcmp(field, "log_upnp")){
			ezplib_get_attr_val("log_rule", 0, "upnp", TempBuf, 32, EZPLIB_USE_CLI);
			strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "log_wlan")){
			ezplib_get_attr_val("log_rule", 0, "wireless", TempBuf, 32, EZPLIB_USE_CLI);
			strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "log_ntpclient")){
			ezplib_get_attr_val("log_rule", 0, "ntp", TempBuf, 32, EZPLIB_USE_CLI);
			strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "log_syswarning")){
			ezplib_get_attr_val("log_rule", 0, "sys_warning", TempBuf, 32, EZPLIB_USE_CLI);
			strcpy(reValue, TempBuf);
		value = reValue;
	}		
	else if (!strcmp(field, "log_dhcpServer")){
			ezplib_get_attr_val("log_rule", 0, "dhcp_serv", TempBuf, 32, EZPLIB_USE_CLI);
			strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "log_dhcpClient")){
			ezplib_get_attr_val("log_rule", 0, "dhcp_cli", TempBuf, 32, EZPLIB_USE_CLI);
			strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "log_ddns")){
			ezplib_get_attr_val("log_rule", 0, "ddns", TempBuf, 32, EZPLIB_USE_CLI);
			strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "log_Firewall")){
			ezplib_get_attr_val("log_rule", 0, "firewall", TempBuf, 32, EZPLIB_USE_CLI);
			strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "log_index")){
			pTempBuf = nvram_safe_get("log_index");
			strcpy(reValue, pTempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "log_selected_num")){
			pTempBuf = nvram_safe_get("log_selected_num");
			strcpy(reValue, pTempBuf);
		value = reValue;
	}
// LogSetting End ---
// Set Operation Mode Start +++
	else if (!strcmp(field, "lan_ipaddr_router")){
		ezplib_get_attr_val("lan_static_rule", 0, "ipaddr_normal", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "lan_ipaddr_ap")){
		ezplib_get_attr_val("lan_static_rule", 0, "ipaddr_ap", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
// Set Operation Mode End ---
// APCli Start +++
      else if (!strcmp(field, "ApCliMacRepeater")){
            ezplib_get_attr_val("wl0_apcli_rule", 0, "macrepeater", TempBuf, 32, EZPLIB_USE_CLI);
            strcpy(reValue, TempBuf);
            value = reValue;
        }

      	else if (!strcmp(field, "ApCliAddrMode")){
		ezplib_get_attr_val("wl0_apcli_rule", 0, "addrmode", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCliSsid")){
		ezplib_get_attr_val("wl0_apcli_rule", 0, "ssid", buf, 33, EZPLIB_USE_CLI);

		/* Added by Bruce Liu, 2013-4-27 for special character */
		memset(tmp_SSID, 0, 128);
		p_ssid = tmp_SSID;
		p_SSID = buf;
		j = 0;
		while (  ( j < 32) && (p_ssid != NULL) && ( (p_ssid + 5) != NULL)  )  {
				if (buf[j] == 92) {
						memcpy(p_ssid, "\\\\", 4 );
						p_ssid += 2;
				} else if (buf[j] == 34) {
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
		
		strcpy(reValue, tmp_SSID);
		value = reValue;
	}
	else if (!strcmp(field, "ApCliBssid")){
		ezplib_get_attr_val("wl0_apcli_rule", 0, "bssid", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCliLockMac")){
		ezplib_get_attr_val("wl0_apcli_rule", 0, "lockmac", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCliAuthMode")){
		ezplib_get_attr_val("wl0_apcli_rule", 0, "secmode", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "disabled")){
			strcpy(reValue, "OPEN");
		}else if (!strcmp(TempBuf, "wep")){
			ezplib_get_attr_val("wl0_apcli_sec_wep_rule", 0, "encmode", TempBuf, 32, EZPLIB_USE_CLI);
			if (!strcmp(TempBuf, "open")){
				strcpy(reValue, "OPEN");
			}else if (!strcmp(TempBuf, "shared")){
				strcpy(reValue, "SHARED");
			}	
		}else if (!strcmp(TempBuf, "psk")){
			strcpy(reValue, "WPAPSK");
		}else if (!strcmp(TempBuf, "psk2")){
			strcpy(reValue, "WPA2PSK");
		}else if (!strcmp(TempBuf, "wpa")){
			strcpy(reValue, "WPA");
		}else if (!strcmp(TempBuf, "wpa2")){
			strcpy(reValue, "WPA2");
		}
		value = reValue;
	}
	else if (!strcmp(field, "ApCliEncrypType")){
		ezplib_get_attr_val("wl0_apcli_rule", 0, "secmode", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "disabled")){
			strcpy(reValue, "NONE");
		}else if (!strcmp(TempBuf, "wep")){
			strcpy(reValue, "WEP");
		}else if (!strcmp(TempBuf, "psk")){
			ezplib_get_attr_val("wl0_apcli_sec_wpa_rule", 0, "crypto", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
			if (!strcmp(TempBuf, "tkip")){
				strcpy(reValue, "TKIP");
			}else if (!strcmp(TempBuf, "aes")){
				strcpy(reValue, "AES");
			}else{
				strcpy(reValue, TempBuf);
			}
		}else if (!strcmp(TempBuf, "psk2")){
			ezplib_get_attr_val("wl0_apcli_sec_wpa2_rule", 0, "crypto", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
			if (!strcmp(TempBuf, "tkip")){
				strcpy(reValue, "TKIP");
			}else if (!strcmp(TempBuf, "aes")){
				strcpy(reValue, "AES");
			}else{
				strcpy(reValue, TempBuf);
			}
            	}else if (!strcmp(TempBuf, "wpa")){
			ezplib_get_attr_val("wl0_apcli_sec_wpa_rule", 0, "crypto", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
			if (!strcmp(TempBuf, "tkip")){
				strcpy(reValue, "TKIP");
			}else if (!strcmp(TempBuf, "aes")){
				strcpy(reValue, "AES");
			}else{
				strcpy(reValue, TempBuf);
			}
		}else if (!strcmp(TempBuf, "wpa2")){
			ezplib_get_attr_val("wl0_apcli_sec_wpa_rule", 0, "crypto", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
			if (!strcmp(TempBuf, "tkip")){
				strcpy(reValue, "TKIP");
			}else if (!strcmp(TempBuf, "aes")){
				strcpy(reValue, "AES");
			}else{
				strcpy(reValue, TempBuf);
			}
		}
		value = reValue;
	}
	else if (!strcmp(field, "ApCliWPAPSK")){
		ezplib_get_attr_val("wl0_apcli_rule", 0, "secmode", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "psk")){
			ezplib_get_attr_val("wl0_apcli_sec_wpa_rule", 0, "key", buf, 65, EZPLIB_USE_CLI);
		}else if (!strcmp(TempBuf, "psk2")){
			ezplib_get_attr_val("wl0_apcli_sec_wpa2_rule", 0, "key", buf, 65, EZPLIB_USE_CLI);
		}else{
			strcpy(buf, "");
		}		
		strcpy(reValue, buf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCliDefaultKeyId")){
		ezplib_get_attr_val("wl0_apcli_sec_wep_rule", 0, "key_index", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCliKey1Type")){
		ezplib_get_attr_val("wl0_apcli_sec_wep_rule", 0, "keytype", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCliKey2Type")){
		ezplib_get_attr_val("wl0_apcli_sec_wep_rule", 0, "keytype", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCliKey3Type")){
		ezplib_get_attr_val("wl0_apcli_sec_wep_rule", 0, "keytype", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCliKey4Type")){
		ezplib_get_attr_val("wl0_apcli_sec_wep_rule", 0, "keytype", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCliKey1Str")){
		ezplib_get_attr_val("wl0_apcli_sec_wep_rule", 0, "key1", buf, 65, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCliKey2Str")){
		ezplib_get_attr_val("wl0_apcli_sec_wep_rule", 0, "key2", buf, 65, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCliKey3Str")){
		ezplib_get_attr_val("wl0_apcli_sec_wep_rule", 0, "key3", buf, 65, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCliKey4Str")){
		ezplib_get_attr_val("wl0_apcli_sec_wep_rule", 0, "key4", buf, 65, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
		value = reValue;
	}
       else if (!strcmp(field, "ApCli5GAddrMode")){
		ezplib_get_attr_val("wl1_apcli_rule", 0, "addrmode", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCli5GSsid")){
		ezplib_get_attr_val("wl1_apcli_rule", 0, "ssid", buf, 33, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCli5GBssid")){
		ezplib_get_attr_val("wl1_apcli_rule", 0, "bssid", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCli5GLockMac")){
		ezplib_get_attr_val("wl1_apcli_rule", 0, "lockmac", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCli5GAuthMode")){
		ezplib_get_attr_val("wl1_apcli_rule", 0, "secmode", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "disabled")){
			strcpy(reValue, "OPEN");
		}else if (!strcmp(TempBuf, "wep")){
			ezplib_get_attr_val("wl1_apcli_sec_wep_rule", 0, "encmode", TempBuf, 32, EZPLIB_USE_CLI);
			if (!strcmp(TempBuf, "open")){
				strcpy(reValue, "OPEN");
			}else if (!strcmp(TempBuf, "shared")){
				strcpy(reValue, "SHARED");
			}	
		}else if (!strcmp(TempBuf, "psk")){
			strcpy(reValue, "WPAPSK");
		}else if (!strcmp(TempBuf, "psk2")){
			strcpy(reValue, "WPA2PSK");
		}
		value = reValue;
	}
	else if (!strcmp(field, "ApCli5GEncrypType")){
		ezplib_get_attr_val("wl1_apcli_rule", 0, "secmode", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "disabled")){
			strcpy(reValue, "NONE");
		}else if (!strcmp(TempBuf, "wep")){
			strcpy(reValue, "WEP");
		}else if (!strcmp(TempBuf, "psk")){
			ezplib_get_attr_val("wl1_apcli_sec_wpa_rule", 0, "crypto", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
			if (!strcmp(TempBuf, "tkip")){
				strcpy(reValue, "TKIP");
			}else if (!strcmp(TempBuf, "aes")){
				strcpy(reValue, "AES");
			}else{
				strcpy(reValue, TempBuf);
			}
		}else if (!strcmp(TempBuf, "psk2")){
			ezplib_get_attr_val("wl1_apcli_sec_wpa2_rule", 0, "crypto", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
			if (!strcmp(TempBuf, "tkip")){
				strcpy(reValue, "TKIP");
			}else if (!strcmp(TempBuf, "aes")){
				strcpy(reValue, "AES");
			}else{
				strcpy(reValue, TempBuf);
			}
		}
		value = reValue;
	}
	else if (!strcmp(field, "ApCli5GWPAPSK")){
		ezplib_get_attr_val("wl1_apcli_rule", 0, "secmode", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "psk")){
			ezplib_get_attr_val("wl1_apcli_sec_wpa_rule", 0, "key", buf, 65, EZPLIB_USE_CLI);
		}else if (!strcmp(TempBuf, "psk2")){
			ezplib_get_attr_val("wl1_apcli_sec_wpa2_rule", 0, "key", buf, 65, EZPLIB_USE_CLI);
		}else{
			strcpy(buf, "");
		}		
		strcpy(reValue, buf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCli5GDefaultKeyId")){
		ezplib_get_attr_val("wl1_apcli_sec_wep_rule", 0, "key_index", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCli5GKey1Type")){
		ezplib_get_attr_val("wl1_apcli_sec_wep_rule", 0, "keytype", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCli5GKey2Type")){
		ezplib_get_attr_val("wl1_apcli_sec_wep_rule", 0, "keytype", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCli5GKey3Type")){
		ezplib_get_attr_val("wl1_apcli_sec_wep_rule", 0, "keytype", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCli5GKey4Type")){
		ezplib_get_attr_val("wl1_apcli_sec_wep_rule", 0, "keytype", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCli5GKey1Str")){
		ezplib_get_attr_val("wl1_apcli_sec_wep_rule", 0, "key1", buf, 65, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCli5GKey2Str")){
		ezplib_get_attr_val("wl1_apcli_sec_wep_rule", 0, "key2", buf, 65, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCli5GKey3Str")){
		ezplib_get_attr_val("wl1_apcli_sec_wep_rule", 0, "key3", buf, 65, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCli5GKey4Str")){
		ezplib_get_attr_val("wl1_apcli_sec_wep_rule", 0, "key4", buf, 65, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
		value = reValue;
	}

// APCli End ---
	//------------------------------------------------------------
	//----------------------------------------------------------IPv6
	else if(!strcmp(field, "lan_ipv6addr_static")) //IPv6 lan address for static
	{
		//ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "static_prefix", buf, TMP_LEN, EZPLIB_USE_CLI); //Steve change 2011/03/09
		//ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "dhcp_prefix", buf, TMP_LEN, EZPLIB_USE_CLI); //aron change to "dhcp_prefix" 2010.11.19
		ezplib_get_attr_val("lan_staticv6_rule", 0, "static_prefix", buf, TMP_LEN, EZPLIB_USE_CLI); //Steve change 2011/03/10
		strcpy(reValue, buf);
	        value = reValue;
printf("lan static value=====%s\n",value);
	}
	else if(!strcmp(field, "lan_ipv6addr_pppoe")) //IPv6 lan address for pppoe
	{
		ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "pppoe_prefix", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
printf("lan pppoe value=====%s\n",value);
	}
	else if(!strcmp(field, "lan_ipv6addr_dhcp")) //IPv6 lan address  for dhcp
	{
		//ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "dhcp_prefix", buf, TMP_LEN, EZPLIB_USE_CLI);
		ezplib_get_attr_val("lan_staticv6_rule", 0, "dhcp_prefix", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
printf("lan dhcp value=====%s\n",value);
	}
/*
	else if(!strcmp(field, "lan_ipv6netsize")) //IPv6 lan net size
	{
		ezplib_get_attr_val("lan_staticv6_rule", 0, "mask", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
*/	
	else if(!strcmp(field, "lan_ipv6gateway")) //IPv6 lan gateway
	{
		ezplib_get_attr_val("lan_staticv6_rule", 0, "gateway", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if(!strcmp(field, "lan_ipv6dns")) //IPv6 lan dns
	{
		ezplib_get_attr_val("lan_staticv6_rule", 0, "dns", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if(!strcmp(field, "wan_ipv6addr")) //IPv6 Wan address
	{
		ezplib_get_attr_val("wan_staticv6_rule", 0, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if(!strcmp(field, "wan_ipv6netsize")) //IPv6 Wan size
	{
		ezplib_get_attr_val("wan_staticv6_rule", 0, "length", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if(!strcmp(field, "wan_ipv6gateway")) //IPv6 Wan gateway
	{
		ezplib_get_attr_val("wan_staticv6_rule", 0, "gateway", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if(!strcmp(field, "wan_ipv6dns1")) //IPv6 Wan primary dns
	{
		ezplib_get_attr_val("wan_staticv6_rule", 0, "dns1", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if(!strcmp(field, "wan_ipv6dns2")) //IPv6 Wan secondary dns
	{
		ezplib_get_attr_val("wan_staticv6_rule", 0, "dns2", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if(!strcmp(field, "Ipv6_dhcp_lifetime")) //IPv6 DHCP lifetime
	{
		ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "lifetime", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if(!strcmp(field, "dhcp_ipv6dns1")) //IPv6 Dhcp primary dns
	{
		ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "dnstype", buf, TMP_LEN, EZPLIB_USE_CLI);
		if(!strncmp(buf, "auto", 4)) {
			ezplib_get_attr_val("wan_dnsv6_rule", 0, "dns1", buf, TMP_LEN, EZPLIB_USE_CLI); 
		} else {
			ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "dnsaddr1", buf, TMP_LEN, EZPLIB_USE_CLI); 
		}
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if(!strcmp(field, "dhcp_ipv6dns2")) //IPv6 Dhcp secondary dns
	{
		ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "dnstype", buf, TMP_LEN, EZPLIB_USE_CLI);
		if(!strncmp(buf, "auto", 4)) {
			ezplib_get_attr_val("wan_dnsv6_rule", 0, "dns2", buf, TMP_LEN, EZPLIB_USE_CLI); 
		} else {
			ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "dnsaddr2", buf, TMP_LEN, EZPLIB_USE_CLI); 
		}
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if(!strcmp(field, "ppoe_ipv6user")) //IPv6 PPPoE user name
	{
		ezplib_get_attr_val("wan_pppoev6_rule", 0, "username", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if(!strcmp(field, "pppoe_ipv6service")) //IPv6 PPPoE service name
	{
		ezplib_get_attr_val("wan_pppoev6_rule", 0, "servicename", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}	
	else if(!strcmp(field, "pppoe_ipv6idle")) //IPv6 PPPoE idle time
	{
		ezplib_get_attr_val("wan_pppoev6_rule", 0, "idletime", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if(!strcmp(field, "pppoe_ipv6mtu")) //IPv6 PPPoE mtu
	{
		ezplib_get_attr_val("wan_pppoev6_rule", 0, "mtu", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if(!strcmp(field, "pppoe_ipv6addr")) //IPv6 PPPoE custom address
	{
		ezplib_get_attr_val("wan_pppoev6_rule", 0, "custom_ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "pppoe_ipv6pass")) //IPv6 pppoe password
	{
		ezplib_get_attr_val("wan_pppoev6_rule", 0, "passwd", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	        
	}
	else if (!strcmp(field, "ipv6_dns1_type")) //pppoe primary dns type
	{
		ezplib_get_attr_val("wan_dnsv6_rule", 0, "dnstype1", buf, TMP_LEN, EZPLIB_USE_CLI); 
		if (!strcmp(buf, "ispdns"))
		     value =  "ISP";
		else if (!strcmp(buf, "custom"))
		     value =  "USER";
		else if (!strcmp(buf, "none"))
		     value =  "NONE";
	}
	else if (!strcmp(field, "ipv6_dns2_type")) //ppp0e secondary dns type
	{
		ezplib_get_attr_val("wan_dnsv6_rule", 0, "dnstype2", buf, TMP_LEN, EZPLIB_USE_CLI); 
		if (!strcmp(buf, "ispdns"))
		     value =  "ISP";
		else if (!strcmp(buf, "custom"))
		     value =  "USER";
		else if (!strcmp(buf, "none"))
		     value =  "NONE";
	}
	else if (!strcmp(field, "IPv6State")) //IPv6 dhcp state
	{
		ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "type", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	        
	}

	else if (!strcmp(field, "Ipv6_dhcp_addrprefix0")) //IPv6 dhcp address prefix
	{
		ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "static_prefix", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	        
printf("lan static prefix value=====%s\n",value);
	}
	else if (!strcmp(field, "Ipv6_dhcp_addrprefix1")) //IPv6 dhcp address prefix
	{
		ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "pppoe_prefix", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
printf("lan pppoe prefix value=====%s\n",value);	       	        
	}
	else if (!strcmp(field, "Ipv6_dhcp_addrprefix2")) //IPv6 dhcp address prefix
	{
		ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "dhcp_prefix", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
printf("lan dhcp prefix value=====%s\n",value);	        	        
	}
	else if (!strcmp(field, "Ipv6_dhcp_addrstart")) //IPv6 dhcp address start
	{
		ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "start", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
printf("lan end value=====%s\n",value);	        	        
	}	
	else if (!strcmp(field, "Ipv6_dhcp_addrend")) //IPv6 dhcp address end
	{
		ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "end", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
printf("lan  start value=====%s\n",value);	        	        
	}
//Steve new add 2011 	
	else if (!strcmp(field, "Ipv6_wan_dhcp_type")) //IPv6 wan dhcp type
	{
		ezplib_get_attr_val("wan_dhcpv6_rule", 0, "type", buf, TMP_LEN, EZPLIB_USE_CLI); 
		if (!strcmp(buf, "stateful"))
		     value =  "1";
		else if (!strcmp(buf, "stateless"))
		     value =  "0";
		else 
		     value =  "0";
printf("Ipv6 wan dhcp type=====%s\n",value);	        	        
	}
	else if(!strcmp(field, "lan_ipv6addr_dhcp_suf")) //IPv6 lan address suffix for dhcp
	{
		ezplib_get_attr_val("lan_staticv6_rule", 0, "dhcp_suffix", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
printf("lan dhcp value=====%s\n",value);
	}
	else if(!strcmp(field, "lan_ipv6addr_static_suf")) //IPv6 lan address suffix for static
	{
		ezplib_get_attr_val("lan_staticv6_rule", 0, "static_suffix", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
printf("lan dhcp value=====%s\n",value);
	}
//+++ WLAN Guest function (Begin)
	else if (!strcmp(field, "WlanGuestIP"))
	{
		ezplib_get_attr_val("guest_lan_rule", 0, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "WlanGuestMASK"))
	{
		ezplib_get_attr_val("guest_lan_rule", 0, "netmask", buf, TMP_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
		idx = atoi(reValue);
		value = prefix2mask(idx); //Translate AXIM's netmask to AboCom's UI format
	}
	else if (!strcmp(field, "WlanGuestBWMax"))
	{
		ezplib_get_attr_val("guest_lan_rule", 0, "bw", buf, TMP_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "WlanGuestIP5G"))
	{
		ezplib_get_attr_val("guest1_lan_rule", 0, "ipaddr", buf, TMP_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "WlanGuestMASK5G"))
	{
		ezplib_get_attr_val("guest1_lan_rule", 0, "netmask", buf, TMP_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
		idx = atoi(reValue);
		value = prefix2mask(idx); //Translate AXIM's netmask to AboCom's UI format
	}
	else if (!strcmp(field, "WlanGuestBWMax5G"))
	{
		ezplib_get_attr_val("guest1_lan_rule", 0, "bw", buf, TMP_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
	        value = reValue;
	}
//--- WLAN Guest function (End)
	else if (!strcmp(field, "SNMP_enable")){ 
		ezplib_get_attr_val("snmpd_user_rule", 0, "enable", buf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;
	}
	else if (!strcmp(field, "SNMP_version_v1")){ 
		ezplib_get_attr_val("snmpd_user_rule", 0, "v1_enable", buf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;
	}
	else if (!strcmp(field, "SNMP_version_v2c")){ 
		ezplib_get_attr_val("snmpd_user_rule", 0, "v2c_enable", buf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;
	}
	else if (!strcmp(field, "SNMP_version_usm")){ 
		ezplib_get_attr_val("snmpd_user_rule", 0, "usm_enable", buf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;
	}
    else if (!strcmp(field, "SNMP_contact")){ 
		value = nvram_safe_get("psyscontact");
	}else if (!strcmp(field, "SNMP_comm_ro")){ 
		value = nvram_safe_get("snmpd_ro_community");
    }
	else if (!strcmp(field, "SNMP_comm_rw")){ 
		value = nvram_safe_get("snmpd_rw_community");
    }
	else if (!strcmp(field, "SNMP_trap_ip")){ 
		value = nvram_safe_get("snmpd_trap_ipadress");
    }
    else if (!strcmp(field, "SNMP_user_ro")){ 
		value = nvram_safe_get("snmpd_ro_username");
    }
	else if (!strcmp(field, "ssh_enable")){ 
		ezplib_get_attr_val("ssh_rule",0,"enable",buf,32,EZPLIB_USE_CLI);
		strcpy(reValue,buf);
		value = reValue;
    }
	else if (!strcmp(field, "ssh_port")){ 
		ezplib_get_attr_val("ssh_rule",0,"port",buf,32,EZPLIB_USE_CLI);
		strcpy(reValue,buf);
		value = reValue;
    }
	else if (!strcmp(field, "telnet_enable")){ 
		value = nvram_safe_get("telnet_enable");
    }
	else if (!strcmp(field, "telnet_port")){ 
		value = nvram_safe_get("telnet_port");
    }
	else if (!strcmp(field, "https_enable")){ 
		value = nvram_safe_get("https_enable");
    }
	else if (!strcmp(field, "https_port")){ 
		value = nvram_safe_get("https_port");
    }
	else if (!strcmp(field, "SNMP_user_ro_passwd")){ 
		value = nvram_safe_get("snmpd_ro_passwd");
    }
    else if (!strcmp(field, "SNMP_user_rw")){ 
		value = nvram_safe_get("snmpd_rw_username");
    }
	else if (!strcmp(field, "SNMP_user_rw_passwd")){ 
		value = nvram_safe_get("snmpd_rw_passwd");
    }
	else if (!strcmp(field, "RIP_enable")){ 
		ezplib_get_attr_val("rip_conf_rule", 0, "RIP_enable", buf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;
    }
	else if (!strcmp(field, "RIPng_enable")){ 
		ezplib_get_attr_val("rip_conf_rule", 0, "RIPng_enable", buf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;
    }
	else if (!strcmp(field, "RIP_version_v1")){ 
		ezplib_get_attr_val("rip_conf_rule", 0, "RIPv1_enable", buf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;
    }
	else if (!strcmp(field, "RIP_version_v2")){ 
		ezplib_get_attr_val("rip_conf_rule", 0, "RIPv2_enable", buf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;
	}        
	else if (!strcmp(field, "RIP_lan")){ 
		ezplib_get_attr_val("rip_conf_rule", 0, "RIPlan_enable", buf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;
	} 
	else if (!strcmp(field, "RIP_wan")){ 
		ezplib_get_attr_val("rip_conf_rule", 0, "RIPwan_enable", buf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;
	}
	else if (!strcmp(field, "RIPng_lan")){ 
		ezplib_get_attr_val("rip_conf_rule", 0, "RIPnglan_enable", buf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;
	}
	else if (!strcmp(field, "RIPng_wan")){ 
		ezplib_get_attr_val("rip_conf_rule", 0, "RIPngwan_enable", buf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;
	}else if (!strcmp(field, "opendns_service")){ 
		ezplib_get_attr_val("wan_opendns_rule", 0, "enable", buf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;
	}else if (!strcmp(field, "opendns_redircet")){ 
		ezplib_get_attr_val("wan_opendns_rule", 0, "redirect", buf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;
	}else if (!strcmp(field, "opendns_username")){ 
		ezplib_get_attr_val("wan_opendns_rule", 0, "username", buf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;
	}else if (!strcmp(field, "opendns_password")){ 
		ezplib_get_attr_val("wan_opendns_rule", 0, "passwd", buf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;
	}else if (!strcmp(field, "opendns_lable")){ 
		ezplib_get_attr_val("wan_opendns_rule", 0, "label", buf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, buf); 
		value = reValue;
	}else if (!strcmp(field, "tos_voice"))
        {
                ezplib_get_attr_val("tos_classify", 0, "voice", buf, 36, EZPLIB_USE_CLI);
                strcpy(reValue, buf);
                //printf("%s############\n",buf);
                value = reValue;
        }
        else if (!strcmp(field, "tos_video"))
        {
                ezplib_get_attr_val("tos_classify", 0, "video", buf, 36, EZPLIB_USE_CLI);
                strcpy(reValue, buf);
                //printf("%s############\n",buf);
                value = reValue;
        }
        else if (!strcmp(field, "tos_data"))
        {
                ezplib_get_attr_val("tos_classify", 0, "data", buf, 36, EZPLIB_USE_CLI);
                strcpy(reValue, buf);
                //printf("%s############\n",buf);
                value = reValue;
        }
        else if (!strcmp(field, "dscp_mode"))
        {
                ezplib_get_attr_val("tos_classify", 0, "mode", buf, 36, EZPLIB_USE_CLI);
                strcpy(reValue, buf);
                //printf("%s############\n",buf);
                value = reValue;
        }
        else if (!strcmp(field, "dscp_other"))
        {
                ezplib_get_attr_val("tos_classify", 0, "other", buf, 36, EZPLIB_USE_CLI);
                strcpy(reValue, buf);
                //printf("%s############\n",buf);
                value = reValue;
        }else{
		printf(" No get Cfg action !!!! \n");	
	}
	
	if (1 == type) {
		if (NULL == value)
			return websWrite(wp, T(""));
		return websWrite(wp, T("%s"), value);
	}
	if (NULL == value)
		ejSetResult(eid, "");
	ejSetResult(eid, value);
	return 0;
}

/*
 * arguments: type - 0 = return the configuration of 'field' (default)
 *                   1 = write the configuration of 'field'
 *            field - parameter name in nvram
 *            idx - index of nth
 * description: read general configurations from nvram (if value is NULL -> "")
 *              parse it and return the Nth value delimited by semicolon
 */
static int getCfgNthGeneral(int eid, webs_t wp, int argc, char_t **argv)
{
	int type, idx;
	char_t *field;
	char *value;
	char *nth;

	if (ejArgs(argc, argv, T("%d %s %d"), &type, &field, &idx) < 3) {
		return websWrite(wp, T("Insufficient args\n"));
	}
	//value = nvram_bufget(RT2860_NVRAM, field);
	if (1 == type) {
		if (NULL == value)
			return websWrite(wp, T(""));
		nth = getNthValue(idx, value);
		if (NULL == nth)
			return websWrite(wp, T(""));
		return websWrite(wp, T("%s"), nth);
	}
	if (NULL == value)
		ejSetResult(eid, "");
	nth = getNthValue(idx, value);
	if (NULL == nth)
		ejSetResult(eid, "");
	ejSetResult(eid, value);
	return 0;
}

/*
 * arguments: type - 0 = return the configuration of 'field' (default)
 *                   1 = write the configuration of 'field'
 *            field - parameter name in nvram
 * description: read general configurations from nvram
 *              if value is NULL -> "0"
 */
static int getCfgZero(int eid, webs_t wp, int argc, char_t **argv)
{
	int type;
	char_t *field;
	char *value;
	char TempBuf[32];
	char buf[TMP_LEN]; //aron add
	int ret; //aron add
	int idx,i; //aron add
	int tmp_val;
	char device[16];
	char *if_ptr;

	char ModeTmpBuf[32];
	ezplib_get_attr_val("wl_mode_rule", 0, "mode", ModeTmpBuf, 32, EZPLIB_USE_CLI);

	if (ejArgs(argc, argv, T("%d %s"), &type, &field) < 2) {
		return websWrite(wp, T("Insufficient args\n"));
	}

    // Tommy for Debug	
    printf("\n ---> getCfgZero(): type = %d & field = %s",type,field);
    if(!strcmp(field,"wisp_mode")){
        if (snprintf(device, 16, "wan0_device") >= 16)
            return -1;
        if_ptr = nvram_safe_get(device);
        if (!strcmp(if_ptr, "apclii0")){
			  strcpy(reValue, "0");
		}else if (!strcmp(if_ptr, "apcli0")){
			strcpy(reValue, "1");
		}else{
            printf("Error: Cannot get the mode value !!!\n");
			strcpy(reValue, "0"); // default 2.4g mode
		}
		value = reValue;
	}
	else if(!strcmp(field, "brg_mode")){
		ezplib_get_attr_val("bridge_rule", 0, "brg_mode", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
	}
	else if(!strcmp(field, "auth_type")){
		ezplib_get_attr_val("wl0_wpa_auth_rule", 0, "wpa_auth", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
	}
	else if(!strcmp(field, "auth_user")){
		ezplib_get_attr_val("wl0_wpa_auth_rule", 0, "wpa_user", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
	}
	else if(!strcmp(field, "auth_passwrd")){
		ezplib_get_attr_val("wl0_wpa_auth_rule", 0, "wpa_passwd", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
	}
       else if(!strcmp(field, "auth_crypto")){
		ezplib_get_attr_val("wl0_wpa_auth_rule", 0, "wpa_crypto", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
	}
	else if(!strcmp(field, "auth_5g_type")){
		ezplib_get_attr_val("wl1_wpa_auth_rule", 0, "wpa_auth", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
	}
	else if(!strcmp(field, "auth_5g_user")){
		ezplib_get_attr_val("wl1_wpa_auth_rule", 0, "wpa_user", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
	}
	else if(!strcmp(field, "auth_5g_passwrd")){
		ezplib_get_attr_val("wl1_wpa_auth_rule", 0, "wpa_passwd", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
	}
	else if(!strcmp(field, "board_model")){
		ezplib_get_attr_val("board_model_rule", 0, "bd_model", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "OP_Mode")){
		/* Get the setting of value from AXIMCom's nvram structure into reValue */
		ezplib_get_attr_val("wl_mode_rule", 0, "mode", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "normal")){
			strcpy(reValue, "0");
		}else if (!strcmp(TempBuf, "ap")){
			strcpy(reValue, "1");
		}else if (!strcmp(TempBuf, "ur")){
			strcpy(reValue, "5");
		}else if (!strcmp(TempBuf, "wisp")){
			strcpy(reValue, "6");
		}else if (!strcmp(TempBuf, "wisp_ur")){
			strcpy(reValue, "7");
		}else{
			printf("Error: Cannot get the mode value !!!\n");
			strcpy(reValue, "0"); // default normal mode
		}
		value = reValue;
	}else if (!strcmp(field, "AAA")){ 
	}
	else if (!strcmp(field, "Channel")){
		if (!strcmp(ModeTmpBuf, "ap")){
			ezplib_get_attr_val("wl_ap_basic_rule", 0, "channel", TempBuf, 32, EZPLIB_USE_CLI);
		}else {
			ezplib_get_attr_val("wl_basic_rule", 0, "channel", TempBuf, 32, EZPLIB_USE_CLI);
		}	
		strcpy(reValue, TempBuf); 
		value = reValue;	
	}else if (!strcmp(field, "Channel_5G")){
		ezplib_get_attr_val("wl5g_basic_rule", 0, "channel", TempBuf, 32, EZPLIB_USE_CLI);		
		strcpy(reValue, TempBuf); 
		value = reValue;		
	}else if (!strcmp(field, "wisp2.4g_mode")){
		ezplib_get_attr_val("wl_wisp_mode_rule", 0, "mode2.4g", TempBuf, 32, EZPLIB_USE_CLI);		
		strcpy(reValue, TempBuf); 
		value = reValue;		
	}else if (!strcmp(field, "wisp5g_mode")){
		ezplib_get_attr_val("wl_wisp_mode_rule", 0, "mode5g", TempBuf, 32, EZPLIB_USE_CLI);		
		strcpy(reValue, TempBuf); 
		value = reValue;		
	}
	else if (!strcmp(field, "WirelessMode")){
		if (!strcmp(ModeTmpBuf, "ap")){
			ezplib_get_attr_val("wl_ap_basic_rule", 0, "net_mode", TempBuf, 32, EZPLIB_USE_CLI);
		}else {
			ezplib_get_attr_val("wl_basic_rule", 0, "net_mode", TempBuf, 32, EZPLIB_USE_CLI);
		}	
		strcpy(reValue, TempBuf); 
		value = reValue;
	}else if (!strcmp(field, "Wireless5GMode")){
		ezplib_get_attr_val("wl5g_basic_rule", 0, "net_mode", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf); 
		value = reValue;
	}else if (!strcmp(field, "natEnabled")){ 
		value = nvram_safe_get("nat_rule");
	}else if (!strcmp(field, "FIREWALLEnabled")){ 
		ezplib_get_attr_val("fw_rule", 0, "state_enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf); 
		value = reValue;
	}else if (!strcmp(field, "RL_QoSEnable")){ 
		ezplib_get_attr_val("bw_rule", 0, "enable", TempBuf, 32, EZPLIB_USE_CLI);	    
		strcpy(reValue, TempBuf);
		value = reValue;
	}else if (!strcmp(field, "RemoteManagementSecuredSel")){ 
		ezplib_get_attr_val("http_rule", 0, "secipaddr", TempBuf, 32, EZPLIB_USE_CLI);
		if(!strlen(TempBuf))
			strcpy(reValue, "0");
		else
			strcpy(reValue, "1");
		value = reValue;
	}
	//-----Steve Start
	else if (!strcmp(field, "websFilterActivex")){
		ezplib_get_attr_val("wf_content_rule", 0, "activex_enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf); 
		value = reValue;		
	}
	else if (!strcmp(field, "websFilterJava")){
		ezplib_get_attr_val("wf_content_rule", 0, "java_enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf); 
		value = reValue;		
	}
	else if (!strcmp(field, "websFilterProxy")){
		ezplib_get_attr_val("wf_content_rule", 0, "proxy_enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf); 
		value = reValue;		
	}
	else if (!strcmp(field, "websFilterCookies")){
		ezplib_get_attr_val("wf_content_rule", 0, "cookie_enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf); 
		value = reValue;
	}
	else if (!strcmp(field, "websHostFiltersEnabled")){
		ezplib_get_attr_val("wf_content_rule", 0, "url_enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf); 
		value = reValue;
	}
	else if (!strcmp(field, "RemoteManagementSel")){
		ezplib_get_attr_val("http_rule", 0, "rmgt_enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf); 
		value = reValue;
	}
	else if (!strcmp(field, "AutoBridge")){
		ezplib_get_attr_val("wl_mode_rule", 0, "auto_bridge", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf); 
		value = reValue;
	}
	else if (!strcmp(field, "IPPortFilterEnable")){
		value = nvram_safe_get("fl_enable");
	}
	//-----Steve End
	//-----Wireless Start
	else if (!strcmp(field, "easy_wireless_security")){
		strcpy(reValue, "0");
		value = reValue;
	}
	else if (!strcmp(field, "staProfile")){
		strcpy(reValue, "0");
		value = reValue;
	}
	else if (!strcmp(field, "staAuth")){
		strcpy(reValue, "0");
		value = reValue;
	}
	else if (!strcmp(field, "staKey1")){
		strcpy(reValue, "0");
		value = reValue;
	}
	else if (!strcmp(field, "staKey2")){
		strcpy(reValue, "0");
		value = reValue;
	}
	else if (!strcmp(field, "staKey3")){
		strcpy(reValue, "0");
		value = reValue;
	}
	else if (!strcmp(field, "staKey4")){
		strcpy(reValue, "0");
		value = reValue;
	}
	else if (!strcmp(field, "IEEE8021X")){
		strcpy(reValue, "0");
		value = reValue;
	}
// EZ mode Wireless Security Start +++	
	else if (!strcmp(field, "SecMode")){
	    strcpy(reValue, "0");
	    int i;
	    int ssid_num = atoi(nvram_safe_get("wlv_rule_max"));
	    for(i=0;i<ssid_num;i++){
		  ezplib_get_attr_val("wl0_sec_rule", i, "secmode", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
               if(!strcmp(TempBuf,"wep") || !strcmp(TempBuf,"psk") ||!strcmp(TempBuf,"wpa")){
                    strcpy(reValue, "1");
			break;
	         }else{
                    continue;
	         }
	    }
	   value = reValue;
	}       
	else if (!strcmp(field, "Sec5GMode")){
	    strcpy(reValue, "0");
	    int i;
	    int ssid_num = atoi(nvram_safe_get("wlv_rule_max"));
	    for(i=0;i<ssid_num;i++){
		  ezplib_get_attr_val("wl1_sec_rule", i, "secmode", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
               if(!strcmp(TempBuf,"wep") || !strcmp(TempBuf,"psk") ||!strcmp(TempBuf,"wpa")){
                    strcpy(reValue, "1");
			break;
	         }else{
                    continue;
	         }
	    }
	   value = reValue;
	}       
	else if (!strcmp(field, "AuthMode")){
		if (!strcmp(ModeTmpBuf, "ap")){
			ezplib_get_attr_val("wl0_sec_rule", 0, "secmode", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
		}else {
			ezplib_get_attr_val("wl0_apcli_rule", 0, "secmode", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
		}	
			if (!strcmp(TempBuf, "disabled")){
		strcpy(reValue, "OPEN");
			}else if (!strcmp(TempBuf, "wep")){
				strcpy(reValue, "SHARED");
			}else if (!strcmp(TempBuf, "psk")){
				strcpy(reValue, "WPAPSK");
			}else if (!strcmp(TempBuf, "psk2")){
				strcpy(reValue, "WPA2PSK");
			}else if (!strcmp(TempBuf, "wpa")){
				strcpy(reValue, "WPA");
			}else if (!strcmp(TempBuf, "wpa2")){
				strcpy(reValue, "WPA2");
			}
		value = reValue;
	}
	else if (!strcmp(field, "Auth5GMode")){
			ezplib_get_attr_val("wl1_sec_rule", 0, "secmode", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
			if (!strcmp(TempBuf, "disabled")){
		strcpy(reValue, "OPEN");
			}else if (!strcmp(TempBuf, "wep")){
				strcpy(reValue, "SHARED");
			}else if (!strcmp(TempBuf, "psk")){
				strcpy(reValue, "WPAPSK");
			}else if (!strcmp(TempBuf, "psk2")){
				strcpy(reValue, "WPA2PSK");
			}else if (!strcmp(TempBuf, "wpa")){
				strcpy(reValue, "WPA");
			}else if (!strcmp(TempBuf, "wpa2")){
				strcpy(reValue, "WPA2");
			}
		value = reValue;
	}
	else if (!strcmp(field, "EncrypType")){
			ezplib_get_attr_val("wl0_sec_rule", 0, "secmode", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
			if (!strcmp(TempBuf, "disabled")){
		strcpy(reValue, "NONE");
			}else if (!strcmp(TempBuf, "wep")){
				strcpy(reValue, "WEP");
			}else if (!strcmp(TempBuf, "psk")){
				ezplib_get_attr_val("wl0_sec_wpa_rule", 0, "crypto", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
				if (!strcmp(TempBuf, "tkip")){
					strcpy(reValue, "TKIP");
				}else if (!strcmp(TempBuf, "aes")){
					strcpy(reValue, "AES");
				}else{
					strcpy(reValue, TempBuf);
				}
			}else if (!strcmp(TempBuf, "psk2")){
				ezplib_get_attr_val("wl0_sec_wpa2_rule", 0, "crypto", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
				if (!strcmp(TempBuf, "tkip")){
					strcpy(reValue, "TKIP");
				}else if (!strcmp(TempBuf, "aes")){
					strcpy(reValue, "AES");
				}else{
					strcpy(reValue, TempBuf);
				}
			}else if (!strcmp(TempBuf, "wpa")){
				ezplib_get_attr_val("wl0_sec_wpa_rule", 0, "crypto", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
				if (!strcmp(TempBuf, "tkip")){
					strcpy(reValue, "TKIP");
				}else if (!strcmp(TempBuf, "aes")){
					strcpy(reValue, "AES");
				}else{
					strcpy(reValue, TempBuf);
				}
			}else if (!strcmp(TempBuf, "wpa2")){
				ezplib_get_attr_val("wl0_sec_wpa2_rule", 0, "crypto", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
				if (!strcmp(TempBuf, "tkip")){
					strcpy(reValue, "TKIP");
				}else if (!strcmp(TempBuf, "aes")){
					strcpy(reValue, "AES");
				}else{
					strcpy(reValue, TempBuf);
				}
			}
			value = reValue;
	}
	else if (!strcmp(field, "Encryp5GType")){
			ezplib_get_attr_val("wl1_sec_rule", 0, "secmode", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
			if (!strcmp(TempBuf, "disabled")){
		strcpy(reValue, "NONE");
			}else if (!strcmp(TempBuf, "wep")){
				strcpy(reValue, "WEP");
			}else if (!strcmp(TempBuf, "psk")){
				ezplib_get_attr_val("wl1_sec_wpa_rule", 0, "crypto", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
				if (!strcmp(TempBuf, "tkip")){
					strcpy(reValue, "TKIP");
				}else if (!strcmp(TempBuf, "aes")){
					strcpy(reValue, "AES");
				}else{
					strcpy(reValue, TempBuf);
				}
			}else if (!strcmp(TempBuf, "psk2")){
				ezplib_get_attr_val("wl1_sec_wpa2_rule", 0, "crypto", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
				if (!strcmp(TempBuf, "tkip")){
					strcpy(reValue, "TKIP");
				}else if (!strcmp(TempBuf, "aes")){
					strcpy(reValue, "AES");
				}else{
					strcpy(reValue, TempBuf);
				}
			}else if (!strcmp(TempBuf, "wpa")){
				ezplib_get_attr_val("wl1_sec_wpa_rule", 0, "crypto", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
				if (!strcmp(TempBuf, "tkip")){
					strcpy(reValue, "TKIP");
				}else if (!strcmp(TempBuf, "aes")){
					strcpy(reValue, "AES");
				}else{
					strcpy(reValue, TempBuf);
				}
			}else if (!strcmp(TempBuf, "wpa2")){
				ezplib_get_attr_val("wl1_sec_wpa2_rule", 0, "crypto", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
				if (!strcmp(TempBuf, "tkip")){
					strcpy(reValue, "TKIP");
				}else if (!strcmp(TempBuf, "aes")){
					strcpy(reValue, "AES");
				}else{
					strcpy(reValue, TempBuf);
				}
			}
			value = reValue;
	}

	else if (!strcmp(field, "WPAPSK1")){
			ezplib_get_attr_val("wl0_sec_rule", 0, "secmode", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
			if (!strcmp(TempBuf, "disabled")){
				strcpy(reValue, "");
			}else if (!strcmp(TempBuf, "wep")){
				ezplib_get_attr_val("wl0_sec_wep_rule", 0, "key_index", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
				if (!strcmp(TempBuf, "1")){
					ezplib_get_attr_val("wl0_sec_wep_rule", 0, "key1", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
				}else if (!strcmp(TempBuf, "2")){
					ezplib_get_attr_val("wl0_sec_wep_rule", 0, "key2", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
				}else if (!strcmp(TempBuf, "3")){
					ezplib_get_attr_val("wl0_sec_wep_rule", 0, "key3", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
				}else if (!strcmp(TempBuf, "4")){
					ezplib_get_attr_val("wl0_sec_wep_rule", 0, "key4", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
				}
				strcpy(reValue, TempBuf);
			}else if (!strcmp(TempBuf, "psk")){
				ezplib_get_attr_val("wl0_sec_wpa_rule", 0, "key", buf, 65, EZPLIB_USE_CLI);
					strcpy(reValue, buf);
			}else if (!strcmp(TempBuf, "psk2")){
				ezplib_get_attr_val("wl0_sec_wpa2_rule", 0, "key", buf, 65, EZPLIB_USE_CLI);
					strcpy(reValue, buf);
			}else if (!strcmp(TempBuf, "wpa")){
					ezplib_get_attr_val("wl0_sec_wpa_rule", 0, "key", buf, 65, EZPLIB_USE_CLI);
					strcpy(reValue, buf);
			}else if (!strcmp(TempBuf, "wpa2")){
				ezplib_get_attr_val("wl0_sec_wpa2_rule", 0, "key", buf, 65, EZPLIB_USE_CLI);
					strcpy(reValue, buf);
			}
		value = reValue;
	}
// EZ mode Wireless Security End ---
// WDS Start ++++++
	else if (!strcmp(field, "WdsEnable")){
    	ezplib_get_attr_val("wl_wds_rule", 0, "mode", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
        if (!strcmp(TempBuf, "disabled")){
			strcpy(reValue, "0");
    	}else if (!strcmp(TempBuf, "repeater")){
			strcpy(reValue, "1");
    	}else if (!strcmp(TempBuf, "bridge")){
			strcpy(reValue, "2");
    	}else{
			printf(" Error : No arg !!!! \n");	
    	}
		value = reValue;
	}
	else if (!strcmp(field, "WdsPhyMode")){
		if (!strcmp(ModeTmpBuf, "ap")){
			ezplib_get_attr_val("wl_ap_advanced_rule", 0, "wdstxmode", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
		}else {
			ezplib_get_attr_val("wl_advanced_rule", 0, "wdstxmode", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
		}	
#if 1
		if (!strcmp(TempBuf, "")){
			strcpy(reValue, "CCK");
		}else{	
			strcpy(reValue, TempBuf);
		}
		value = reValue;
#else
		if (!strcmp(TempBuf, "CCK")){
			strcpy(reValue, "CCK;CCK;CCK;CCK");
    	}else if (!strcmp(TempBuf, "OFDM")){
			strcpy(reValue, "OFDM;OFDM;OFDM;OFDM");
    	}else if (!strcmp(TempBuf, "HTMIX")){
			strcpy(reValue, "HTMIX;HTMIX;HTMIX;HTMIX");
    	}else if (!strcmp(TempBuf, "GREENFIELD")){
			strcpy(reValue, "GREENFIELD;GREENFIELD;GREENFIELD;GREENFIELD");
    	}
		value = reValue;
#endif		
	}
      else if (!strcmp(field, "Wds5GEnable")){
    	ezplib_get_attr_val("wl1_wds_rule", 0, "mode", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);
        if (!strcmp(TempBuf, "disabled")){
			strcpy(reValue, "0");
    	}else if (!strcmp(TempBuf, "repeater")){
			strcpy(reValue, "1");
    	}else if (!strcmp(TempBuf, "bridge")){
			strcpy(reValue, "2");
    	}else{
			printf(" Error : No arg !!!! \n");	
    	}
		value = reValue;
	}
	else if (!strcmp(field, "Wds5GPhyMode")){
		ezplib_get_attr_val("wl1_advanced_rule", 0, "wdstxmode", TempBuf, SHORT_BUF_LEN, EZPLIB_USE_CLI);

		if (!strcmp(TempBuf, "")){
			strcpy(reValue, "OFDM");
		}else{	
			strcpy(reValue, TempBuf);
		}
		value = reValue;	
	}

// WDS End   ------
// Scheduling & Power Saving Start +++
	else if (!strcmp(field, "WiFiScheduleEnable")){
		value = nvram_safe_get("sched_enable");
	}
	else if (!strcmp(field, "WiFi5GScheduleEnable")){
		value = nvram_safe_get("sched1_enable");
	}
	/* Everyday */
	else if (!strcmp(field, "WiFischeduling_action_0")){
		ezplib_get_attr_val("sched_rule", 0, "wlanstatus", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFiScheduling_day_Active_0")){
		ezplib_get_attr_val("sched_rule", 0, "enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_hour_0")){
		ezplib_get_attr_val("sched_rule", 0, "beghr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_min_0")){
		ezplib_get_attr_val("sched_rule", 0, "begmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_hour_0")){
		ezplib_get_attr_val("sched_rule", 0, "endhr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_min_0")){
		ezplib_get_attr_val("sched_rule", 0, "endmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	/* Sunday */
	else if (!strcmp(field, "WiFischeduling_action_7")){
		ezplib_get_attr_val("sched_rule", 1, "wlanstatus", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFiScheduling_day_Active_7")){
		ezplib_get_attr_val("sched_rule", 1, "enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_hour_7")){
		ezplib_get_attr_val("sched_rule", 1, "beghr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_min_7")){
		ezplib_get_attr_val("sched_rule", 1, "begmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_hour_7")){
		ezplib_get_attr_val("sched_rule", 1, "endhr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_min_7")){
		ezplib_get_attr_val("sched_rule", 1, "endmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	/* Monday */
	else if (!strcmp(field, "WiFischeduling_action_1")){
		ezplib_get_attr_val("sched_rule", 2, "wlanstatus", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFiScheduling_day_Active_1")){
		ezplib_get_attr_val("sched_rule", 2, "enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_hour_1")){
		ezplib_get_attr_val("sched_rule", 2, "beghr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_min_1")){
		ezplib_get_attr_val("sched_rule", 2, "begmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_hour_1")){
		ezplib_get_attr_val("sched_rule", 2, "endhr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_min_1")){
		ezplib_get_attr_val("sched_rule", 2, "endmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	/* Tuesday */
	else if (!strcmp(field, "WiFischeduling_action_2")){
		ezplib_get_attr_val("sched_rule", 3, "wlanstatus", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFiScheduling_day_Active_2")){
		ezplib_get_attr_val("sched_rule", 3, "enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_hour_2")){
		ezplib_get_attr_val("sched_rule", 3, "beghr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_min_2")){
		ezplib_get_attr_val("sched_rule", 3, "begmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_hour_2")){
		ezplib_get_attr_val("sched_rule", 3, "endhr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_min_2")){
		ezplib_get_attr_val("sched_rule", 3, "endmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	/* Wednesday */
	else if (!strcmp(field, "WiFischeduling_action_3")){
		ezplib_get_attr_val("sched_rule", 4, "wlanstatus", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFiScheduling_day_Active_3")){
		ezplib_get_attr_val("sched_rule", 4, "enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_hour_3")){
		ezplib_get_attr_val("sched_rule", 4, "beghr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_min_3")){
		ezplib_get_attr_val("sched_rule", 4, "begmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_hour_3")){
		ezplib_get_attr_val("sched_rule", 4, "endhr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_min_3")){
		ezplib_get_attr_val("sched_rule", 4, "endmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	/* Thursday */
	else if (!strcmp(field, "WiFischeduling_action_4")){
		ezplib_get_attr_val("sched_rule", 5, "wlanstatus", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFiScheduling_day_Active_4")){
		ezplib_get_attr_val("sched_rule", 5, "enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_hour_4")){
		ezplib_get_attr_val("sched_rule", 5, "beghr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_min_4")){
		ezplib_get_attr_val("sched_rule", 5, "begmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_hour_4")){
		ezplib_get_attr_val("sched_rule", 5, "endhr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_min_4")){
		ezplib_get_attr_val("sched_rule", 5, "endmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}	
	/* Friday */
	else if (!strcmp(field, "WiFischeduling_action_5")){
		ezplib_get_attr_val("sched_rule", 6, "wlanstatus", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFiScheduling_day_Active_5")){
		ezplib_get_attr_val("sched_rule", 6, "enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_hour_5")){
		ezplib_get_attr_val("sched_rule", 6, "beghr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_min_5")){
		ezplib_get_attr_val("sched_rule", 6, "begmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_hour_5")){
		ezplib_get_attr_val("sched_rule", 6, "endhr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_min_5")){
		ezplib_get_attr_val("sched_rule", 6, "endmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	/* Saturday */
	else if (!strcmp(field, "WiFischeduling_action_6")){
		ezplib_get_attr_val("sched_rule", 7, "wlanstatus", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFiScheduling_day_Active_6")){
		ezplib_get_attr_val("sched_rule", 7, "enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_hour_6")){
		ezplib_get_attr_val("sched_rule", 7, "beghr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_start_min_6")){
		ezplib_get_attr_val("sched_rule", 7, "begmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_hour_6")){
		ezplib_get_attr_val("sched_rule", 7, "endhr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFischeduling_end_min_6")){
		ezplib_get_attr_val("sched_rule", 7, "endmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5GScheduleEnable")){
		value = nvram_safe_get("sched1_enable");
	}
	/* Everyday */
	else if (!strcmp(field, "WiFi5Gscheduling_action_0")){
		ezplib_get_attr_val("sched1_rule", 0, "wlanstatus", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5GScheduling_day_Active_0")){
		ezplib_get_attr_val("sched1_rule", 0, "enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_start_hour_0")){
		ezplib_get_attr_val("sched1_rule", 0, "beghr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_start_min_0")){
		ezplib_get_attr_val("sched1_rule", 0, "begmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_end_hour_0")){
		ezplib_get_attr_val("sched1_rule", 0, "endhr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_end_min_0")){
		ezplib_get_attr_val("sched1_rule", 0, "endmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	/* Sunday */
	else if (!strcmp(field, "WiFi5Gscheduling_action_7")){
		ezplib_get_attr_val("sched1_rule", 1, "wlanstatus", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5GScheduling_day_Active_7")){
		ezplib_get_attr_val("sched1_rule", 1, "enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_start_hour_7")){
		ezplib_get_attr_val("sched1_rule", 1, "beghr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_start_min_7")){
		ezplib_get_attr_val("sched1_rule", 1, "begmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_end_hour_7")){
		ezplib_get_attr_val("sched_rule", 1, "endhr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_end_min_7")){
		ezplib_get_attr_val("sched1_rule", 1, "endmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	/* Monday */
	else if (!strcmp(field, "WiFi5Gscheduling_action_1")){
		ezplib_get_attr_val("sched1_rule", 2, "wlanstatus", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5GScheduling_day_Active_1")){
		ezplib_get_attr_val("sched1_rule", 2, "enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_start_hour_1")){
		ezplib_get_attr_val("sched1_rule", 2, "beghr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_start_min_1")){
		ezplib_get_attr_val("sched1_rule", 2, "begmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_end_hour_1")){
		ezplib_get_attr_val("sched1_rule", 2, "endhr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_end_min_1")){
		ezplib_get_attr_val("sched1_rule", 2, "endmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	/* Tuesday */
	else if (!strcmp(field, "WiFi5Gscheduling_action_2")){
		ezplib_get_attr_val("sched1_rule", 3, "wlanstatus", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5GScheduling_day_Active_2")){
		ezplib_get_attr_val("sched1_rule", 3, "enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_start_hour_2")){
		ezplib_get_attr_val("sched1_rule", 3, "beghr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_start_min_2")){
		ezplib_get_attr_val("sched1_rule", 3, "begmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_end_hour_2")){
		ezplib_get_attr_val("sched1_rule", 3, "endhr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_end_min_2")){
		ezplib_get_attr_val("sched1_rule", 3, "endmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	/* Wednesday */
	else if (!strcmp(field, "WiFi5Gscheduling_action_3")){
		ezplib_get_attr_val("sched1_rule", 4, "wlanstatus", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5GScheduling_day_Active_3")){
		ezplib_get_attr_val("sched1_rule", 4, "enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_start_hour_3")){
		ezplib_get_attr_val("sched1_rule", 4, "beghr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_start_min_3")){
		ezplib_get_attr_val("sched1_rule", 4, "begmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_end_hour_3")){
		ezplib_get_attr_val("sched1_rule", 4, "endhr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_end_min_3")){
		ezplib_get_attr_val("sched1_rule", 4, "endmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	/* Thursday */
	else if (!strcmp(field, "WiFi5Gscheduling_action_4")){
		ezplib_get_attr_val("sched1_rule", 5, "wlanstatus", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5GScheduling_day_Active_4")){
		ezplib_get_attr_val("sched1_rule", 5, "enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_start_hour_4")){
		ezplib_get_attr_val("sched1_rule", 5, "beghr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_start_min_4")){
		ezplib_get_attr_val("sched1_rule", 5, "begmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_end_hour_4")){
		ezplib_get_attr_val("sched1_rule", 5, "endhr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_end_min_4")){
		ezplib_get_attr_val("sched1_rule", 5, "endmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}	
	/* Friday */
	else if (!strcmp(field, "WiFi5Gscheduling_action_5")){
		ezplib_get_attr_val("sched1_rule", 6, "wlanstatus", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5GScheduling_day_Active_5")){
		ezplib_get_attr_val("sched1_rule", 6, "enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_start_hour_5")){
		ezplib_get_attr_val("sched1_rule", 6, "beghr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_start_min_5")){
		ezplib_get_attr_val("sched1_rule", 6, "begmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_end_hour_5")){
		ezplib_get_attr_val("sched1_rule", 6, "endhr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_end_min_5")){
		ezplib_get_attr_val("sched1_rule", 6, "endmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	/* Saturday */
	else if (!strcmp(field, "WiFi5Gscheduling_action_6")){
		ezplib_get_attr_val("sched1_rule", 7, "wlanstatus", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5GScheduling_day_Active_6")){
		ezplib_get_attr_val("sched1_rule", 7, "enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_start_hour_6")){
		ezplib_get_attr_val("sched1_rule", 7, "beghr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_start_min_6")){
		ezplib_get_attr_val("sched1_rule", 7, "begmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_end_hour_6")){
		ezplib_get_attr_val("sched1_rule", 7, "endhr", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "WiFi5Gscheduling_end_min_6")){
		ezplib_get_attr_val("sched1_rule", 7, "endmin", TempBuf, 32, EZPLIB_USE_CLI);
		if (!strcmp(TempBuf, "30")){
			strcpy(reValue, "1");
		}else{
			strcpy(reValue, "0");
		}
		value = reValue;
	}		
// Scheduling & Power Saving End ---
	//-----Wireless End
        //------------------ aron patch for giga
        else if (!strcmp(field, "wan_ip_auto")){ //Ethernet is DHCP or Static IP
		value = nvram_get("wan0_proto");
		//Translate AXIM's wan proto to AboCom's UI format 
		if (!strcmp(value, "dhcp"))
		    value =  "0";
		else if(!strcmp(value, "static"))
		    value =  "1";
	}
	else if (!strcmp(field, "wan_pppoe_nailup")) //wan0 pppoe nailup
	{
                   ezplib_get_attr_val("wan_pppoe_rule", 0, "demand", buf, TMP_LEN, EZPLIB_USE_CLI); 
	           strcpy(reValue, buf);
	           idx = atoi(reValue);
	           if(idx==0)      //AXIM treat nailup "checked" as "0"
	               value="1";
	           else if(idx==1) //on-demand
	               value="0";
	}
	else if (!strcmp(field, "wan_pppoe_wanip_mode")) //wan0 pppoe PPP IP type
	{
		ezplib_get_attr_val("wan_pppoe_rule", 0, "pppiptype", buf, TMP_LEN, EZPLIB_USE_CLI);
		if (!strcmp(buf, "isp"))
		     value =  "1";
		else if (!strcmp(buf, "custom"))
		     value =  "0";
	}
	else if (!strcmp(field, "wan_MacClone_mode")) //wan0 clone mac type
	{
		   ezplib_get_attr_val("wan_hwaddr_clone_rule", 0, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
	           strcpy(reValue, buf);
	           idx = atoi(reValue);
	           if(idx==0)      //AXIM treat factory default as "0"
	               value="0";
	           else if(idx==1)
	           {
	               ezplib_get_attr_val("wan_hwaddr_clone_rule", 0, "addrtype", buf, TMP_LEN, EZPLIB_USE_CLI); 
	               if (!strcmp(buf, "ipaddr")) //clone mac by IP
		            value =  "1";
		       else if (!strcmp(buf, "hwaddr")) //clone mac by MAC
		            value =  "2";
	           }
	}
	else if (!strcmp(field, "brand_3g")) //wan0 3g brand
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "brand", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "model_3g")) //wan0 3g model
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "model", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "apnType_3g")) //wan0 3g apn type
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "apn_type", buf, TMP_LEN, EZPLIB_USE_CLI); 
	        strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "location_3g")) //wan0 3g location
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "location", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "srvPro_3g")) //wan0 3g location
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "isp", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "auth_3g")) //wan0 3g authentication
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "auth", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "connMode_3g")) //wan0 3g connection mode
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "mode", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "ppp_conn_type")) //wan0 3g ppp connection type
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "demand", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "turbo_3g")) //wan0 3g turbo link
	{
		ezplib_get_attr_val("wan_wwan_rule", 0, "turbolink", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "wan_3g_nailup")) //wan0 3g nailup
	{
                   ezplib_get_attr_val("wan_wwan_rule", 0, "demand", buf, TMP_LEN, EZPLIB_USE_CLI); 
	           strcpy(reValue, buf);
	           idx = atoi(reValue);
	           if(idx==0)      //AXIM treat nailup "checked" as "0"
	               value="1";
	           else if(idx==1) //on-demand
	               value="0";
	}
	else if (!strcmp(field, "dhcpEnabled")) //lan0 dhcp enable
	{
		ezplib_get_attr_val("lan_dhcps_rule", 0, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "DDNSEnabled")) //wan0 ddns enable
	{
		ezplib_get_attr_val("wan_ddns_rule", 0, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "upnpEnabled")) //upnp enable
	{
		ezplib_get_attr_val("upnp_rule", 0, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "snoopingEnabled")) //igmp snooping enable
	{
		ezplib_get_attr_val("igmp_snoop_rule", 0, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "snoopingLAN1")) //igmp snooping port1
	{
		ezplib_get_attr_val("igmp_snoop_rule", 0, "p1", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "snoopingLAN2")) //igmp snooping port2
	{
		ezplib_get_attr_val("igmp_snoop_rule", 0, "p2", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "snoopingLAN3")) //igmp snooping port3
	{
		ezplib_get_attr_val("igmp_snoop_rule", 0, "p3", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "snoopingLAN4")) //igmp snooping port4
	{
		ezplib_get_attr_val("igmp_snoop_rule", 0, "p4", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "OperationMode")) //OperationMode
	{
		ezplib_get_attr_val("wl_mode_rule", 0, "mode", buf, TMP_LEN, EZPLIB_USE_CLI); 
		if (!strcmp(buf, "normal"))
		     value =  "0";
		else if (!strcmp(buf, "ap"))
		     value =  "1";
		else if (!strcmp(buf, "ur"))
		     value =  "5";
		else if (!strcmp(buf, "wisp"))     
		     value =  "6";
		else if (!strcmp(buf, "wisp_ur"))     
		     value =  "7";
	}
	else if (!strcmp(field, "wan_pptp_nailup")) //wan0 pptp nailup
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "nailup", buf, TMP_LEN, EZPLIB_USE_CLI);
		if (!strcmp(buf, "0"))
		     value =  "0";
		else if (!strcmp(buf, "1"))
		     value =  "1";
	}
	else if (!strcmp(field, "wan_pptp_mode")) //wan0 pptp physical IP type
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "iptype", buf, TMP_LEN, EZPLIB_USE_CLI);
		if (!strcmp(buf, "dhcp"))
		     value =  "1";
		else if (!strcmp(buf, "static"))
		     value =  "0";
	}
	else if (!strcmp(field, "wan_pptp_wanip_mode")) //wan0 pptp PPP IP type
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "pppiptype", buf, TMP_LEN, EZPLIB_USE_CLI);
		if (!strcmp(buf, "isp"))
		     value =  "1";
		else if (!strcmp(buf, "custom"))
		     value =  "0";
	}
	else if (!strcmp(field, "wan_l2tp_nailup")) //wan0 pptp nailup
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "nailup", buf, TMP_LEN, EZPLIB_USE_CLI);
		if (!strcmp(buf, "0"))
		     value =  "0";
		else if (!strcmp(buf, "1"))
		     value =  "1";
	}
	else if (!strcmp(field, "wan_l2tp_mode")) //wan0 l2tp physical IP type
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "iptype", buf, TMP_LEN, EZPLIB_USE_CLI);
		if (!strcmp(buf, "dhcp"))
		     value =  "1";
		else if (!strcmp(buf, "static"))
		     value =  "0";
	}
	else if (!strcmp(field, "wan_l2tp_wanip_mode")) //wan0 l2tp PPP IP type
	{
		ezplib_get_attr_val("wan_pptp_l2tp_rule", 0, "pppiptype", buf, TMP_LEN, EZPLIB_USE_CLI);
		if (!strcmp(buf, "isp"))
		     value =  "1";
		else if (!strcmp(buf, "custom"))
		     value =  "0";
	}
	else if (!strcmp(field, "Lan2Enabled")) //lan ip alias enable
	{
		ezplib_get_attr_val("lan_static_rule", 0, "alias_enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "lanIp_mode")) //lan0 ip mode
	{
		value = nvram_get("lan0_proto");
		if (!strcmp(value, "dhcp"))
		    value =  "0";
		else if(!strcmp(value, "static"))
		    value =  "1";
	}
        else if (!strcmp(field, "bigPond_srv")) //wan0 bigpond server
	{
		ezplib_get_attr_val("wan_bigpond_rule", 0, "server", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "bigPond_enb")) //wan0 bigpond enable
	{
		ezplib_get_attr_val("wan_bigpond_rule", 0, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	//------------------------ Aron patch end
	else if(!strcmp(field, "EnableSSID")) //enable ssid
	{
		ezplib_get_attr_val("wl0_basic_rule", 0, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	

	}
	else if(!strcmp(field, "EnableSSID1")) //enable ssid1
	{
		ezplib_get_attr_val("wl0_basic_rule", 1, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	
		
	}
	else if(!strcmp(field, "EnableSSID2")) //enable ssid2
	{
		ezplib_get_attr_val("wl0_basic_rule", 2, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	
		
	}
	else if(!strcmp(field, "EnableSSID3")) //enable ssid3
	{
		ezplib_get_attr_val("wl0_basic_rule", 3, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	
		
	}
	else if(!strcmp(field, "EnableSSID4")) //enable ssid4
	{
		ezplib_get_attr_val("wl0_basic_rule", 4, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	
		
	}
	else if(!strcmp(field, "EnableSSID5")) //enable ssid5
	{
		ezplib_get_attr_val("wl0_basic_rule", 5, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	
		
	}
	else if(!strcmp(field, "EnableSSID6")) //enable ssid6
	{
		ezplib_get_attr_val("wl0_basic_rule", 6, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	
		
	}
	else if(!strcmp(field, "EnableSSID7")) //enable ssid7
	{
		ezplib_get_attr_val("wl0_basic_rule", 7, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	
		
	}
	else if(!strcmp(field, "HideSSID")) //hidden ssid1
	{
		ezplib_get_attr_val("wl0_basic_rule", 0, "hidden", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	

	}
	else if(!strcmp(field, "HideSSID1")) //hidden ssid1
	{
		ezplib_get_attr_val("wl0_basic_rule", 1, "hidden", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	
		
	}
	else if(!strcmp(field, "HideSSID2")) //hidden ssid2
	{
		ezplib_get_attr_val("wl0_basic_rule", 2, "hidden", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	
		
	}
	else if(!strcmp(field, "HideSSID3")) //hidden ssid3
	{
		ezplib_get_attr_val("wl0_basic_rule", 3, "hidden", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	
		
	}
	else if(!strcmp(field, "HideSSID4")) //hidden ssid4
	{
		ezplib_get_attr_val("wl0_basic_rule", 4, "hidden", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	
		
	}
	else if(!strcmp(field, "HideSSID5")) //hidden ssid5
	{
		ezplib_get_attr_val("wl0_basic_rule", 5, "hidden", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	
		
	}
	else if(!strcmp(field, "HideSSID6")) //hidden ssid6
	{
		ezplib_get_attr_val("wl0_basic_rule", 6, "hidden", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	
		
	}
	else if(!strcmp(field, "HideSSID7")) //hidden ssid7
	{
		ezplib_get_attr_val("wl0_basic_rule", 7, "hidden", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	
		
	}
	//------------------------------------------------
	else if(!strcmp(field, "NoForwarding")) //Intra_BSS1
	{
		ezplib_get_attr_val("wl0_basic_rule", 0, "isolation", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	
	}
	else if(!strcmp(field, "NoForwarding1")) //Intra_BSS1
	{
		ezplib_get_attr_val("wl0_basic_rule", 1, "isolation", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	
	
	}
	else if(!strcmp(field, "NoForwarding2")) //Intra_BSS2
	{
		ezplib_get_attr_val("wl0_basic_rule", 2, "isolation", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	

	}
	else if(!strcmp(field, "NoForwarding3")) //Intra_BSS3
	{
		ezplib_get_attr_val("wl0_basic_rule", 3, "isolation", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
		
	}
	else if(!strcmp(field, "NoForwarding4")) //Intra_BSS4
	{
		ezplib_get_attr_val("wl0_basic_rule", 4, "isolation", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
		
	}
	else if(!strcmp(field, "NoForwarding5")) //Intra_BSS5
	{
		ezplib_get_attr_val("wl0_basic_rule", 5, "isolation", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
		
	}
	else if(!strcmp(field, "NoForwarding6")) //Intra_BSS6
	{
		ezplib_get_attr_val("wl0_basic_rule", 6, "isolation", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
		
	}
	else if(!strcmp(field, "NoForwarding7")) //Intra_BSS7
	{
		ezplib_get_attr_val("wl0_basic_rule", 7, "isolation", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
		
	}
	else if(!strcmp(field, "MainIntra_BSS")) //Main Intra_BSS
	{
		if (!strcmp(ModeTmpBuf, "ap")){
			ezplib_get_attr_val("wl_ap_basic_rule", 0, "bisolation", buf, TMP_LEN, EZPLIB_USE_CLI);
		}else {
			ezplib_get_attr_val("wl_basic_rule", 0, "bisolation", buf, TMP_LEN, EZPLIB_USE_CLI); 
		}
		strcpy(reValue, buf);
		value = reValue;
	}else if(!strcmp(field, "EnableSSID1_5G")) //enable ssid1
	{
		ezplib_get_attr_val("wl1_basic_rule", 1, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	
		
	}else if(!strcmp(field, "EnableSSID2_5G")) //enable ssid2
	{
		ezplib_get_attr_val("wl1_basic_rule", 2, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	
		
	}else if(!strcmp(field, "EnableSSID3_5G")) //enable ssid3
	{
		ezplib_get_attr_val("wl1_basic_rule", 3, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	
		
	}
	else if(!strcmp(field, "HideSSID_5G")) //hidden ssid1
	{
		ezplib_get_attr_val("wl1_basic_rule", 0, "hidden", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	

	}else if(!strcmp(field, "HideSSID1_5G")) //hidden ssid2
	{
		ezplib_get_attr_val("wl1_basic_rule", 1, "hidden", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	
		
	}else if(!strcmp(field, "HideSSID2_5G")) //hidden ssid3
	{
		ezplib_get_attr_val("wl1_basic_rule", 2, "hidden", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	
		
	}else if(!strcmp(field, "HideSSID3_5G")) //hidden ssid4
	{
		ezplib_get_attr_val("wl1_basic_rule", 3, "hidden", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	
		
	}
	//------------------------------------------------
	else if(!strcmp(field, "NoForwarding_5G")) //Intra_BSS1
	{
		ezplib_get_attr_val("wl1_basic_rule", 0, "isolation", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	
	}else if(!strcmp(field, "NoForwarding1_5G")) //Intra_BSS2
	{
		ezplib_get_attr_val("wl1_basic_rule", 1, "isolation", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	
	
	}else if(!strcmp(field, "NoForwarding2_5G")) //Intra_BSS3
	{
		ezplib_get_attr_val("wl1_basic_rule", 2, "isolation", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;	

	}else if(!strcmp(field, "NoForwarding3_5G")) //Intra_BSS4
	{
		ezplib_get_attr_val("wl1_basic_rule", 3, "isolation", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
		
	}
	else if(!strcmp(field, "MainIntra_BSS_5G")) //Main Intra_BSS
	{
		ezplib_get_attr_val("wl5g_basic_rule", 0, "bisolation", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;      

	}

// APCli Start +++
	else if (!strcmp(field, "ApCliEnable")){
		ezplib_get_attr_val("wl0_apcli_rule", 0, "enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
      	else if (!strcmp(field, "ApCliAddrMode")){
		ezplib_get_attr_val("wl0_apcli_rule", 0, "addrmode", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
	else if (!strcmp(field, "ApCli5GEnable")){
		ezplib_get_attr_val("wl1_apcli_rule", 0, "enable", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
      	else if (!strcmp(field, "ApCli5GAddrMode")){
		ezplib_get_attr_val("wl1_apcli_rule", 0, "addrmode", TempBuf, 32, EZPLIB_USE_CLI);
		strcpy(reValue, TempBuf);
		value = reValue;
	}
// APCli Start ---
//------------------------------------------------------------Wireless Security
	else if(!strcmp(field, "DefaultWEPKey")) //DefaultWEPKey
	{
		ezplib_get_attr_val("wl0_sec_wep_rule", 0, "key_index", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
			
	}//-------------------------------------------------------------Wireless Advanced
	else if(!strcmp(field, "DefaultWEPKey5G")) //DefaultWEPKey
	{
		ezplib_get_attr_val("wl1_sec_wep_rule", 0, "key_index", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;			
	}//-------------------------------------------------------------Wireless Advanced
	else if(!strcmp(field, "RTSThreshold")) //RTSThreshold
	{
		if (!strcmp(ModeTmpBuf, "ap")){
			ezplib_get_attr_val("wl_ap_advanced_rule", 0, "rts", buf, TMP_LEN, EZPLIB_USE_CLI); 
		}else {
			ezplib_get_attr_val("wl_advanced_rule", 0, "rts", buf, TMP_LEN, EZPLIB_USE_CLI); 
		}
		strcpy(reValue, buf);
		value = reValue;			
	}else if(!strcmp(field, "FragThreshold")) //Fragmentation
	{
		if (!strcmp(ModeTmpBuf, "ap")){
			ezplib_get_attr_val("wl_ap_advanced_rule", 0, "frag", buf, TMP_LEN, EZPLIB_USE_CLI); 
		}else {
			ezplib_get_attr_val("wl_advanced_rule", 0, "frag", buf, TMP_LEN, EZPLIB_USE_CLI); 
		}	
		strcpy(reValue, buf);
		value = reValue;			
	}else if(!strcmp(field, "RTS5GThreshold")) //RTSThreshold
	{
		ezplib_get_attr_val("wl1_advanced_rule", 0, "rts", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;			
	}else if(!strcmp(field, "Frag5GThreshold")) //Fragmentation
	{
		ezplib_get_attr_val("wl1_advanced_rule", 0, "frag", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;			
	}
	else if(!strcmp(field, "TxPower")) //Output power
	{
		if (!strcmp(ModeTmpBuf, "ap")){
			ezplib_get_attr_val("wl_ap_basic_rule", 0, "txpower", buf, TMP_LEN, EZPLIB_USE_CLI);
		}else {
			ezplib_get_attr_val("wl_basic_rule", 0, "txpower", buf, TMP_LEN, EZPLIB_USE_CLI);  
		}
		strcpy(reValue, buf);
		value = reValue;			
	}else if(!strcmp(field, "Tx5GPower")) //Output power
	{
		ezplib_get_attr_val("wl5g_basic_rule", 0, "txpower", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;			
	}
	else if(!strcmp(field, "NoForwardingAdv")) //Intra-BSS enable
	{
		if (!strcmp(ModeTmpBuf, "ap")){
			ezplib_get_attr_val("wl_ap_basic_rule", 0, "bisolation", buf, TMP_LEN, EZPLIB_USE_CLI);
		}else {
			ezplib_get_attr_val("wl_basic_rule", 0, "bisolation", buf, TMP_LEN, EZPLIB_USE_CLI); 
		}
		strcpy(reValue, buf);
		value = reValue;      			
	}else if(!strcmp(field, "NoForwarding5GAdv")) //Intra-BSS enable
	{
		ezplib_get_attr_val("wl5g_basic_rule", 0, "bisolation", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	        			
	}
	else if(!strcmp(field, "HT_OpMode")) //Operating Mode
	{
		if (!strcmp(ModeTmpBuf, "ap")){
			ezplib_get_attr_val("wl_ap_advanced_rule", 0, "opmode", buf, TMP_LEN, EZPLIB_USE_CLI); 
		}else {
			ezplib_get_attr_val("wl_advanced_rule", 0, "opmode", buf, TMP_LEN, EZPLIB_USE_CLI); 
		}
		strcpy(reValue, buf);
		value = reValue;			
	}else if(!strcmp(field, "HT_BW")) //Channel Bandwidth
	{
		if (!strcmp(ModeTmpBuf, "ap")){
			ezplib_get_attr_val("wl_ap_advanced_rule", 0, "htbw", buf, TMP_LEN, EZPLIB_USE_CLI); 
		}else {
			ezplib_get_attr_val("wl_advanced_rule", 0, "htbw", buf, TMP_LEN, EZPLIB_USE_CLI); 
		}
		strcpy(reValue, buf);
		value = reValue;			
	}else if(!strcmp(field, "HT_GI")) //Guard Interval
	{
		if (!strcmp(ModeTmpBuf, "ap")){
			ezplib_get_attr_val("wl_ap_advanced_rule", 0, "gi", buf, TMP_LEN, EZPLIB_USE_CLI); 
		}else {
			ezplib_get_attr_val("wl_advanced_rule", 0, "gi", buf, TMP_LEN, EZPLIB_USE_CLI); 
		}
		strcpy(reValue, buf);
		value = reValue;			
	}else if(!strcmp(field, "DTIM")) //Guard Interval
	{
		if (!strcmp(ModeTmpBuf, "ap")){
			ezplib_get_attr_val("wl_ap_advanced_rule", 0, "dtim", buf, TMP_LEN, EZPLIB_USE_CLI); 
		}else {
			ezplib_get_attr_val("wl_advanced_rule", 0, "dtim", buf, TMP_LEN, EZPLIB_USE_CLI); 
		}
		strcpy(reValue, buf);
		value = reValue;			
	}/* Added by Bruce Liu, 2012-1-9 */
    else if(!strcmp(field, "distance")) // Between distance AP and client.
    {
        if (!strcmp(ModeTmpBuf, "ap")){
			ezplib_get_attr_val("wl_ap_advanced_rule", 0, "distance", buf, TMP_LEN, EZPLIB_USE_CLI); 
		}else {
			ezplib_get_attr_val("wl_advanced_rule", 0, "distance", buf, TMP_LEN, EZPLIB_USE_CLI); 
		}
        strcpy(reValue, buf);
		tmp_val = atoi(reValue);
		tmp_val = tmp_val / 1000; /* convert from m to km */
		sprintf(reValue, "%d", tmp_val);
		value = reValue;
    }/* Ended By Bruce Liu */
	else if(!strcmp(field, "beacon")) //Beacon Interval
	{
		if (!strcmp(ModeTmpBuf, "ap")){
			ezplib_get_attr_val("wl_ap_advanced_rule", 0, "bcn", buf, TMP_LEN, EZPLIB_USE_CLI); 
		}else {
			ezplib_get_attr_val("wl_advanced_rule", 0, "bcn", buf, TMP_LEN, EZPLIB_USE_CLI); 
		}
		strcpy(reValue, buf);
		value = reValue;			
	}
    else if(!strcmp(field, "TxBurst")) //Guard Interval
	{
		if (!strcmp(ModeTmpBuf, "ap")){
			ezplib_get_attr_val("wl_ap_advanced_rule", 0, "txburst", buf, TMP_LEN, EZPLIB_USE_CLI); 
		}else {
			ezplib_get_attr_val("wl_advanced_rule", 0, "txburst", buf, TMP_LEN, EZPLIB_USE_CLI); 
		}
		strcpy(reValue, buf);
		value = reValue;			
	}else if(!strcmp(field, "AntennaMode")) //Antenna mode
	{
		if (!strcmp(ModeTmpBuf, "ap")){
			ezplib_get_attr_val("wl_ap_advanced_rule", 0, "antennamode", buf, TMP_LEN, EZPLIB_USE_CLI); 
		}else {
			ezplib_get_attr_val("wl_advanced_rule", 0, "antennamode", buf, TMP_LEN, EZPLIB_USE_CLI); 
		}
		strcpy(reValue, buf);
		value = reValue;			
	}//---------------------------------------------------------Wireless QoS
	else if(!strcmp(field, "HT_5G_OpMode")) //Operating Mode
	{
		ezplib_get_attr_val("wl1_advanced_rule", 0, "opmode", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;			
	}else if(!strcmp(field, "HT_5G_BW")) //Channel Bandwidth
	{
		ezplib_get_attr_val("wl1_advanced_rule", 0, "htbw", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;			
	}else if(!strcmp(field, "HT_5G_GI")) //Guard Interval
	{
		ezplib_get_attr_val("wl1_advanced_rule", 0, "gi", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;			
	}else if(!strcmp(field, "DTIM_5G")) //Guard Interval
	{
		ezplib_get_attr_val("wl1_advanced_rule", 0, "dtim", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;			
	}else if(!strcmp(field, "AntennaMode_5G")) //5g antenna mode
	{
		ezplib_get_attr_val("wl1_advanced_rule", 0, "antennamode", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;			
	}//---------------------------------------------------------Wireless QoS
	else if(!strcmp(field, "WmmCapable")) //Guard Interval
	{
		ezplib_get_attr_val("wl0_basic_rule", 0, "wme", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;			
	}//----------------------------------------------------------IPV6
	else if(!strcmp(field, "WmmCapable5G")) //Guard Interval
	{
		ezplib_get_attr_val("wl1_basic_rule", 0, "wme", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;			
	}//----------------------------------------------------------IPV6
	else if(!strcmp(field, "Ipv6DhcpCapable")) //enable Ipv6 dhcp 
	{
		ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;			
printf("enable dhcp *********%s\n",value);
	}
	else if(!strcmp(field, "dnsType")) //Ipv6 dhcp dns type
	{
		ezplib_get_attr_val("lan_dhcpsv6_rule", 0, "dnstype", buf, TMP_LEN, EZPLIB_USE_CLI); 
		if (!strcmp(buf, "auto"))
		     value =  "0";
		else if (!strcmp(buf, "custom"))
		     value =  "1";		     			
	}
	else if(!strcmp(field, "PppoeIPv6_iptype")) //Ipv6 dhcp pppoe address type
	{
		ezplib_get_attr_val("wan_pppoev6_rule", 0, "pppiptype", buf, TMP_LEN, EZPLIB_USE_CLI); 
		if (!strcmp(buf, "isp"))
		     value =  "0";
		else if (!strcmp(buf, "custom"))
		     value =  "1";		     			
	}
        else if (!strcmp(field, "wan_ipv6_proto")){ //IPV6 wan proto
		value = nvram_get("wan0_protov6");
		if (!strcmp(value, "static"))
		    value =  "0";
		else if(!strcmp(value, "dhcp"))
		    value =  "1";
		else if(!strcmp(value, "pppoe"))
		    value =  "2";
		else if(!strcmp(value, "link-local")) //aron add  2010.12.01
		    value =  "3";
	}
//+++ WLAN Guest function (Begin)
	else if (!strcmp(field, "WlanGuestEnable"))
	{
		ezplib_get_attr_val("guest_lan_rule", 0, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "WlanGuestBWEnable"))
	{
		ezplib_get_attr_val("guest_lan_rule", 0, "bwen", buf, TMP_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "WlanGuestBWPriority"))
	{
		ezplib_get_attr_val("guest_lan_rule", 0, "prio", buf, TMP_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "WlanGuestEnable5G"))
	{
		ezplib_get_attr_val("guest1_lan_rule", 0, "enable", buf, TMP_LEN, EZPLIB_USE_CLI); 
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "WlanGuestBWEnable5G"))
	{
		ezplib_get_attr_val("guest1_lan_rule", 0, "bwen", buf, TMP_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "WlanGuestBWPriority5G"))
	{
		ezplib_get_attr_val("guest1_lan_rule", 0, "prio", buf, TMP_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
	        value = reValue;
	}
//--- WLAN Guest function (End)
// easy mode WLAN power saving set
	else if (!strcmp(field, "wlan_mode"))
	{
		ezplib_get_attr_val("wl_easy_mode_rule", 0, "mode", buf, TMP_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
	        value = reValue;
	}
	else if (!strcmp(field, "stp_set"))
	{
		ezplib_get_attr_val("lan_main_rule", 0, "stp", buf, TMP_LEN, EZPLIB_USE_CLI);
		strcpy(reValue, buf);
	        value = reValue;
	}else if (!strcmp(field, "snooping_set"))
    {
        ezplib_get_attr_val("lan_main_rule", 0, "snooping", buf, TMP_LEN, EZPLIB_USE_CLI);
        strcpy(reValue, buf);
        value = reValue;
    }
	else{
		printf(" No get Cfg action !!!! \n");	
	}
	
	if (1 == type) {
		if (!strcmp(value, "")){
			return websWrite(wp, T("0"));
		}
		return websWrite(wp, T("%s"), value);
	}
	if (!strcmp(value, ""))
		ejSetResult(eid, "0");
	ejSetResult(eid, value);
	return 0;
}


/*
 * arguments: type - 0 = return the configuration of 'field' (default)
 *                   1 = write the configuration of 'field'
 *            field - parameter name in nvram
 *            idx - index of nth
 * description: read general configurations from nvram (if value is NULL -> "0")
 *              parse it and return the Nth value delimited by semicolon
 */
static int getCfgNthZero(int eid, webs_t wp, int argc, char_t **argv)
{
	int type, idx;
	char_t *field;
	char *value;
	char *nth;

	if (ejArgs(argc, argv, T("%d %s %d"), &type, &field, &idx) < 3) {
		return websWrite(wp, T("Insufficient args\n"));
	}
	//value = nvram_bufget(RT2860_NVRAM, field);
	if (1 == type) {
		if (!strcmp(value, ""))
			return websWrite(wp, T("0"));
		nth = getNthValue(idx, value);
		if (NULL == nth)
			return websWrite(wp, T("0"));
		return websWrite(wp, T("%s"), nth);
	}
	if (!strcmp(value, ""))
		ejSetResult(eid, "0");
	nth = getNthValue(idx, value);
	if (NULL == nth)
		ejSetResult(eid, "0");
	ejSetResult(eid, value);
	return 0;
}

/*
 * arguments: type - 0 = return the configuration of 'field' (default)
 *                   1 = write the configuration of 'field'
 *            field - parameter name in nvram
 * description: read general configurations from nvram
 *              if value is NULL -> ""
 */
static int getCfg2General(int eid, webs_t wp, int argc, char_t **argv)
{
	int type;
	char_t *field;
	char *value;

	if (ejArgs(argc, argv, T("%d %s"), &type, &field) < 2) {
		return websWrite(wp, T("Insufficient args\n"));
	}
	//value = nvram_bufget(RTINIC_NVRAM, field);
	if (1 == type) {
		if (NULL == value)
			return websWrite(wp, T(""));
		return websWrite(wp, T("%s"), value);
	}
	if (NULL == value)
		ejSetResult(eid, "");
	ejSetResult(eid, value);
	return 0;
}

/*
 * arguments: type - 0 = return the configuration of 'field' (default)
 *                   1 = write the configuration of 'field'
 *            field - parameter name in nvram
 *            idx - index of nth
 * description: read general configurations from nvram (if value is NULL -> "")
 *              parse it and return the Nth value delimited by semicolon
 */
static int getCfg2NthGeneral(int eid, webs_t wp, int argc, char_t **argv)
{
	int type, idx;
	char_t *field;
	char *value;
	char *nth;

	if (ejArgs(argc, argv, T("%d %s %d"), &type, &field, &idx) < 3) {
		return websWrite(wp, T("Insufficient args\n"));
	}
	//value = nvram_bufget(RTINIC_NVRAM, field);
	if (1 == type) {
		if (NULL == value)
			return websWrite(wp, T(""));
		nth = getNthValue(idx, value);
		if (NULL == nth)
			return websWrite(wp, T(""));
		return websWrite(wp, T("%s"), nth);
	}
	if (NULL == value)
		ejSetResult(eid, "");
	nth = getNthValue(idx, value);
	if (NULL == nth)
		ejSetResult(eid, "");
	ejSetResult(eid, value);
	return 0;
}

/*
 * arguments: type - 0 = return the configuration of 'field' (default)
 *                   1 = write the configuration of 'field'
 *            field - parameter name in nvram
 * description: read general configurations from nvram
 *              if value is NULL -> "0"
 */
static int getCfg2Zero(int eid, webs_t wp, int argc, char_t **argv)
{
	int type;
	char_t *field;
	char *value;

	if (ejArgs(argc, argv, T("%d %s"), &type, &field) < 2) {
		return websWrite(wp, T("Insufficient args\n"));
	}
	//value = nvram_bufget(RTINIC_NVRAM, field);
	if (1 == type) {
		if (!strcmp(value, ""))
			return websWrite(wp, T("0"));
		return websWrite(wp, T("%s"), value);
	}
	if (!strcmp(value, ""))
		ejSetResult(eid, "0");
	ejSetResult(eid, value);
	return 0;
}

/*
 * arguments: type - 0 = return the configuration of 'field' (default)
 *                   1 = write the configuration of 'field'
 *            field - parameter name in nvram
 *            idx - index of nth
 * description: read general configurations from nvram (if value is NULL -> "0")
 *              parse it and return the Nth value delimited by semicolon
 */
static int getCfg2NthZero(int eid, webs_t wp, int argc, char_t **argv)
{
	int type, idx;
	char_t *field;
	char *value;
	char *nth;

	if (ejArgs(argc, argv, T("%d %s %d"), &type, &field, &idx) < 3) {
		return websWrite(wp, T("Insufficient args\n"));
	}
	//value = nvram_bufget(RTINIC_NVRAM, field);
	if (1 == type) {
		if (!strcmp(value, ""))
			return websWrite(wp, T("0"));
		nth = getNthValue(idx, value);
		if (NULL == nth)
			return websWrite(wp, T("0"));
		return websWrite(wp, T("%s"), nth);
	}
	if (!strcmp(value, ""))
		ejSetResult(eid, "0");
	nth = getNthValue(idx, value);
	if (NULL == nth)
		ejSetResult(eid, "0");
	ejSetResult(eid, value);
	return 0;
}

/*
 * arguments: type - 0 = return the configuration of 'field' (default)
 *                   1 = write the configuration of 'field'
 *            field - parameter name in nvram
 * description: read general configurations from nvram
 *              if value is NULL -> ""
 */
static int getCfg3General(int eid, webs_t wp, int argc, char_t **argv)
{
	int type;
	char_t *field;
	char *value;

	if (ejArgs(argc, argv, T("%d %s"), &type, &field) < 2) {
		return websWrite(wp, T("Insufficient args\n"));
	}
	//value = nvram_bufget(RT2561_NVRAM, field);
	if (1 == type) {
		if (NULL == value)
			return websWrite(wp, T(""));
		return websWrite(wp, T("%s"), value);
	}
	if (NULL == value)
		ejSetResult(eid, "");
	ejSetResult(eid, value);
	return 0;
}

/*
 * arguments: type - 0 = return the configuration of 'field' (default)
 *                   1 = write the configuration of 'field'
 *            field - parameter name in nvram
 * description: read general configurations from nvram
 *              if value is NULL -> "0"
 */
static int getCfg3Zero(int eid, webs_t wp, int argc, char_t **argv)
{
	int type;
	char_t *field;
	char *value;

	if (ejArgs(argc, argv, T("%d %s"), &type, &field) < 2) {
		return websWrite(wp, T("Insufficient args"));
	}
	//value = nvram_bufget(RT2561_NVRAM, field);
	if (1 == type) {
		if (!strcmp(value, ""))
			return websWrite(wp, T("0"));
		return websWrite(wp, T("%s"), value);
	}
	if (!strcmp(value, ""))
		ejSetResult(eid, "0");
	ejSetResult(eid, value);
	return 0;
}

static int getDpbSta(int eid, webs_t wp, int argc, char_t **argv)
{
#ifdef CONFIG_RT2860V2_STA_DPB
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

static int getLangBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t *lang;

	if (ejArgs(argc, argv, T("%s"), &lang) < 1) {
		return websWrite(wp, T("0"));
	}
	if (!strncmp(lang, "en", 3))
#if 1 //def CONFIG_USER_GOAHEAD_LANG_EN
		return websWrite(wp, T("1"));
#else
		return websWrite(wp, T("0"));
#endif
	else if (!strncmp(lang, "zhtw", 5))
#if 1 //def CONFIG_USER_GOAHEAD_LANG_ZHTW
		return websWrite(wp, T("1"));
#else
		return websWrite(wp, T("0"));
#endif
	else if (!strncmp(lang, "zhcn", 5))
#ifdef CONFIG_USER_GOAHEAD_LANG_ZHCN
		return websWrite(wp, T("1"));
#else
		return websWrite(wp, T("1"));
#endif
#if 1//Arthur Chow 2009-03-21: For Multi-Language
	if (!strncmp(lang, "de", 3))
		return websWrite(wp, T("0"));
	if (!strncmp(lang, "fr", 3))
		return websWrite(wp, T("0"));
	if (!strncmp(lang, "es", 3))
		return websWrite(wp, T("0"));
	if (!strncmp(lang, "it", 3))
		return websWrite(wp, T("0"));
	if (!strncmp(lang, "turk", 5))
		return websWrite(wp, T("0"));
#endif

	return websWrite(wp, T("0"));
}

static int getMiiInicBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if defined (CONFIG_RT2880v2_INIC_MII) || defined (CONFIG_RT2880v2_INIC_MII_MODULE)
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

/*
 * description: write the current system platform accordingly
 */
static int getPlatform(int eid, webs_t wp, int argc, char_t **argv)
{
#ifdef CONFIG_RAETH_ROUTER
	return websWrite(wp, T("RT2880 with IC+ MACPHY"));
#endif
#ifdef CONFIG_ICPLUS_PHY
    return websWrite(wp, T("RT2880 with IC+ PHY"));
#endif
#ifdef CONFIG_RT_MARVELL
	return websWrite(wp, T("RT2880 with MARVELL"));
#endif
#ifdef CONFIG_MAC_TO_MAC_MODE
	return websWrite(wp, T("RT2880 with Vitesse"));
#endif
#ifdef CONFIG_RT_3052_ESW
	return websWrite(wp, T("RT3052 embedded switch"));
#endif

	return 0;
}

static int getStationBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if defined CONFIG_RT2860V2_STA || defined CONFIG_RT2860V2_STA_MODULE
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

/*
 * description: write System build time
 */
static int getSysBuildTime(int eid, webs_t wp, int argc, char_t **argv)
{
	return websWrite(wp, T("%s"), __DATE__);
}

/*
 * description: write RT288x SDK version
 */
#if 0 // Tommy debug
static int getSdkVersion(int eid, webs_t wp, int argc, char_t **argv)
{
	return websWrite(wp, T("%s"), RT288X_SDK_VERSION);
}
#endif // Tommy debug

//Steve 2010/12/01
int getMem()
{
	int gmem=0;
	FILE *fp;
	char cmdBuf[200]={0};

	memset(cmdBuf, 0, sizeof(cmdBuf)); 

	//sprintf(cmdBuf, "cat /proc/meminfo | grep MemTotal | cut -d' ' -f 9 > /tmp/sysmem");
	sprintf(cmdBuf, "cat /proc/meminfo | grep MemTotal | awk '{print $2}' > /tmp/sysmem");	
	system(cmdBuf);
	//printf("	%s \n",cmdBuf);
		
	fp = fopen("/tmp/sysmem", "r");
	if (!fp){
		printf("indicate error: Cannot open /tmp/sysmem !!!\n");
		return 16000;
	}else{
		fscanf(fp,"%d",&gmem);
		fclose(fp);
	}
	if(gmem > 0)
		return gmem;
	else
		return 16000;
}

#if 1//Arthur Chow 2009-01-03
/*
 * description: write System Resource
 */
static int getSysResource(int eid, webs_t wp, int argc, char_t **argv)
{
		FILE *fp = NULL;
		int len=0;
		char_t top_info[80];
		char_t mem_info[10];
		char_t cpu_info[10];
		int mem_num=0;


		//system("top -n 1 > /etc_ro/web/sys_resource");
    	system("top -n 1 > /tmp/sys_resource");

		//if (NULL == (fp = fopen("/etc_ro/web/sys_resource", "r")))
		if (NULL == (fp = fopen("/tmp/sys_resource", "r")))
		{
			printf("getSysResource: open /tmp/sys_resource error\n");
			error(E_L, E_LOG, T("getSysResource: open /tmp/sys_resource error"));
			strcpy(mem_info, "0");
			strcpy(cpu_info, "0");
		}
		else
		{
			len = fscanf(fp, "%s", top_info);
//			printf("0000[%d][%s]\n", len, top_info);
			len = fscanf(fp, "%s", top_info);
//			printf("1111[%d][%s]\n", len, top_info);
			if (len)
			{
				int str_len=0;
				strcpy(mem_info, top_info);
				str_len=strlen(mem_info);
				mem_info[str_len-1]=0;
			}
			else
				strcpy(mem_info, "0");
			/*len = fscanf(fp, "%s", top_info);
//			printf("0001[%d][%s]\n", len, top_info);
			len = fscanf(fp, "%s", top_info);
//			printf("0002[%d][%s]\n", len, top_info);
			len = fscanf(fp, "%s", top_info);
//			printf("0003[%d][%s]\n", len, top_info);
			len = fscanf(fp, "%s", top_info);
//			printf("0004[%d][%s]\n", len, top_info);
			len = fscanf(fp, "%s", top_info);
//			printf("0005[%d][%s]\n", len, top_info);
			len = fscanf(fp, "%s", top_info);
//			printf("0006[%d][%s]\n", len, top_info);
			len = fscanf(fp, "%s", top_info);
//			printf("0007[%d][%s]\n", len, top_info);
			len = fscanf(fp, "%s", top_info);
//			printf("0008[%d][%s]\n", len, top_info);
			len = fscanf(fp, "%s", top_info);
//			printf("0009[%d][%s]\n", len, top_info);
			len = fscanf(fp, "%s", top_info);
//			printf("0010[%d][%s]\n", len, top_info);
			len = fscanf(fp, "%s", top_info);
//			printf("2222[%d][%s]\n", len, top_info);
			if (len)
				strcpy(cpu_info, top_info);
			else
				strcpy(cpu_info, "0");*/
			fclose(fp);
		}
        //system("top -n 1|grep CPU:|awk -F ' ' '{print $2}' >/tmp/sys_resource");
		system("top -n 1 | grep 'CPU:' | awk -F ' ' '{print $8}' | awk -F '\%' '{print $1}' | xargs echo | awk '{print $1}' >/tmp/sys_resource");//Get CPU Idle
        if (NULL == (fp = fopen("/tmp/sys_resource", "r")))
        {

            strcpy(cpu_info, "0");
        }else{
            len = fscanf(fp, "%s", top_info);
            if(len == 0){
                strcpy(cpu_info, "0");
            }else{
            	int cpu_occupy = 100 - atoi(top_info);
				sprintf(cpu_info,"%d%%",cpu_occupy);
                //strcpy(cpu_info, top_info);
            }
        }
        fclose(fp);
		websWrite(wp, T("<tr >\n"));
		websWrite(wp, T("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoSystatCpu'>CPU Usage</font>:</span></td>\n"));
		websWrite(wp, T("<td valign='middle' ><span class='table_font2'>\n"));
		websWrite(wp, T("<table width='100%%' height='18' border='0' cellpadding='0' cellspacing='0' bordercolor='#7895c8'><tr><td width='60%%'>\n"));
		websWrite(wp, T("<table width='100%%' height='18' border='1' cellpadding='0' cellspacing='0' bordercolor='#7895c8'>\n"));
		websWrite(wp, T("</tr>\n"));
		websWrite(wp, T("<tr>\n"));
		websWrite(wp, T("<td background='images/i_percentage_center.gif'>\n"));
		websWrite(wp, T("<table width='%s' border='0' cellspacing='0' cellpadding='0'>\n"), cpu_info);
		websWrite(wp, T("<tr>\n"));
		websWrite(wp, T("<td height='18' background='images/i_percentage_blue.gif'><img src='images/air.gif' width='1' height='1' /></td>\n"));
		websWrite(wp, T("</tr>\n"));
		websWrite(wp, T("</table></td>\n"));
		websWrite(wp, T("</tr>\n"));
		websWrite(wp, T("</table>\n"));
		websWrite(wp, T("</td><td>&nbsp;%s</td></tr></table>\n"), cpu_info);
		websWrite(wp, T("</span></td>\n"));
		websWrite(wp, T("</tr>\n"));
		websWrite(wp, T("<tr >\n"));
		websWrite(wp, T("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- <font id='dash_infoSystatMem'>Memory Usage</font>:</span></td>\n"));
		websWrite(wp, T("<td valign='middle' ><span class='table_font2'>\n"));
		websWrite(wp, T("<table width='100%%' height='18' border='0' cellpadding='0' cellspacing='0' bordercolor='#7895c8'><tr><td width='60%%'>\n"));
		websWrite(wp, T("<table width='100%%' height='18' border='1' cellpadding='0' cellspacing='0' bordercolor='#7895c8'>\n"));
		websWrite(wp, T("</tr>\n"));
		websWrite(wp, T("<tr>\n"));
		websWrite(wp, T("<td background='images/i_percentage_center.gif'>\n"));
#if 1 //def CONFIG_RT2880_DRAM_32M
		mem_num = getMem();
		websWrite(wp, T("<table width='%d%%' border='0' cellspacing='0' cellpadding='0'>\n"), (atoi(mem_info)*100)/mem_num);
		//websWrite(wp, T("<table width='%d%%' border='0' cellspacing='0' cellpadding='0'>\n"), (atoi(mem_info)*100)/29068);
#else
		websWrite(wp, T("<table width='%d%%' border='0' cellspacing='0' cellpadding='0'>\n"), (atoi(mem_info)*100)/62012);
#endif		
		websWrite(wp, T("<tr>\n"));
		websWrite(wp, T("<td height='18' background='images/i_percentage_blue.gif'><img src='images/air.gif' width='1' height='1' /></td>\n"));
		websWrite(wp, T("</tr>\n"));
		websWrite(wp, T("</table></td>\n"));
		websWrite(wp, T("</tr>\n"));
		websWrite(wp, T("</table>\n"));
		
#if 1 //def CONFIG_RT2880_DRAM_32M
        websWrite(wp, T("</td><td>&nbsp;%d%%</td></tr></table>\n"), (atoi(mem_info)*100)/mem_num);
		//websWrite(wp, T("</td><td>&nbsp;%d%%</td></tr></table>\n"), (atoi(mem_info)*100)/29068);
#else
		websWrite(wp, T("</td><td>&nbsp;%d%%</td></tr></table>\n"), (atoi(mem_info)*100)/62012);
#endif		
		websWrite(wp, T("</span></td>\n"));
//		websWrite(wp, T("<tr >\n"));
//		websWrite(wp, T("<td valign='top'><span class='table_font'>&nbsp;&nbsp;- Memory Usage:</span></td>\n"));
//		websWrite(wp, T("<td ><span class='table_font2'>%sB</span></td>\n"), mem_info);
		return websWrite(wp, T("</tr>\n"));
}
#endif

/*marked by Gordon 2009/1/5
 * description: write System Uptime
 */
/*static int getSysUptime(int eid, webs_t wp, int argc, char_t **argv)
{
	struct tm *utime;
	time_t usecs;

	time(&usecs);
	utime = localtime(&usecs);

	if (utime->tm_hour > 0)
		return websWrite(wp, T("%d hour%s, %d min%s, %d sec%s"),
				utime->tm_hour, (utime->tm_hour == 1)? "" : "s",
				utime->tm_min, (utime->tm_min == 1)? "" : "s",
				utime->tm_sec, (utime->tm_sec == 1)? "" : "s");
	else if (utime->tm_min > 0)
		return websWrite(wp, T("%d min%s, %d sec%s"),
				utime->tm_min, (utime->tm_min == 1)? "" : "s",
				utime->tm_sec, (utime->tm_sec == 1)? "" : "s");
	else
		return websWrite(wp, T("%d sec%s"),
				utime->tm_sec, (utime->tm_sec == 1)? "" : "s");
	return 0;
}*/

static int getSysUptime(int eid, webs_t wp, int argc, char_t **argv)//modified by Gordon 2009/1/5
{
	int updays, uphours, upminutes, upseconds;
	struct sysinfo info;
	struct tm *current_time;
	time_t current_secs;

  time(&current_secs);
	current_time = localtime(&current_secs);
	sysinfo(&info);


	updays = (int) info.uptime / (60*60*24);
	upminutes = (int) info.uptime / 60;
	uphours = (upminutes / 60) % 24;
	upminutes %= 60;
	upseconds = ((int) info.uptime)%60;
	if (updays)
		return websWrite(wp, T("%d day%s, %d hour%s, %d min%s, %d sec%s") ,
		updays, (updays != 1) ? "s" : "",
		uphours, (uphours != 1) ? "s" : "",
		upminutes, (upminutes != 1) ? "s" : "",
		upseconds, (upseconds != 1) ? "s" : "");
	if (uphours)
		return websWrite(wp, T("%d hour%s, %d min%s, %d sec%s") ,
		uphours, (uphours != 1) ? "s" : "",
		upminutes, (upminutes != 1) ? "s" : "",
		upseconds, (upseconds != 1) ? "s" : "");
	if (upminutes)
	  return websWrite(wp, T("%d min%s, %d sec%s") ,
		upminutes, (upminutes != 1) ? "s" : "",
		upseconds, (upseconds != 1) ? "s" : "");
	else
		return websWrite(wp, T("%d sec%s") ,
		upseconds, (upseconds != 1) ? "s" : "");

	  return 0;
}

static int getPortStatus(int eid, webs_t wp, int argc, char_t **argv)
{

printf("\n ==> getPortStatus\n");
//#if (defined (CONFIG_RAETH_ROUTER) || defined CONFIG_RT_3052_ESW) && defined (CONFIG_USER_ETHTOOL)
#if 1
	
	char wanlink[10]={0};
	char wanspeed[10]={0};
		char link = '0';
		int speed = 100;
		char duplex = 'F';

	FILE *fp = popen("/sbin/ezp-portstate 5 link", "r");
	if(!fp){
		websWrite(wp, T("none"));
			return 0;
		}
	fgets(wanlink, 10, fp);
		pclose(fp);
		
  	FILE *fp_1 = popen("/sbin/ezp-portstate 5 speed", "r");
	if(!fp_1){
		websWrite(wp, T("none"));
			return 0;
	}
	fgets(wanspeed, 10, fp_1);
	pclose(fp_1);
	
	if (!strncmp(wanlink, "up", 2)){
					link = '1';
	}else if (!strncmp(wanlink, "down",4)){
		link='0';
	}else{
		link='0';
			}
	
	if (!strncmp(wanspeed, "1000", 4)){
					speed = 1000;
	}else if (!strncmp(wanlink, "100", 3)){
		speed=100;
	}else if (!strncmp(wanlink, "10", 2)){
		speed=10;
	}else{
		speed=10;
			}
	websWrite(wp, T("%c"), link);

	return 0;
#else
printf("\n ==> getPortStatus ->0\n");
	websWrite(wp, T("-1"));
	return 0;
#endif

}

inline int getOnePortOnly(void)
{
#if defined CONFIG_RAETH_ROUTER || defined CONFIG_MAC_TO_MAC_MODE || defined CONFIG_RT_3052_ESW
	return 0;
#elif defined CONFIG_ICPLUS_PHY
	return 1;
#else
	return 0;
#endif
	return 0;
}

static int isOnePortOnly(int eid, webs_t wp, int argc, char_t **argv)
{
	if( getOnePortOnly() == 1)
		websWrite(wp, T("true"));
	else
		websWrite(wp, T("false"));
	return 0;
}

void redirect_wholepage(webs_t wp, const char *url)
{
	websWrite(wp, T("HTTP/1.1 200 OK\nContent-type: text/html\nPragma: no-cache\nCache-Control: no-cache\n\n"));
	websWrite(wp, T("<html><head><script language=\"JavaScript\">"));
	websWrite(wp, T("parent.location.replace(\"%s\");"), url);
	websWrite(wp, T("</script></head></html>"));
}

int netmask_aton(const char *ip)
{
	int i, a[4], result = 0;
	sscanf(ip, "%d.%d.%d.%d", &a[0], &a[1], &a[2], &a[3]);
	for(i=0; i<4; i++){	//this is dirty
		if(a[i] == 255){
			result += 8;
			continue;
		}
		if(a[i] == 254)
			result += 7;
		if(a[i] == 252)
			result += 6;
		if(a[i] == 248)
			result += 5;
		if(a[i] == 240)
			result += 4;
		if(a[i] == 224)
			result += 3;
		if(a[i] == 192)
			result += 2;
		if(a[i] == 128)
			result += 1;
		//if(a[i] == 0)
		//	result += 0;
		break;
	}
	return result;
}

#if 0 // Tommy debug
static void forceMemUpgrade(webs_t wp, char_t *path, char_t *query)
{
	char_t *mode  = websGetVar(wp, T("ForceMemUpgradeSelect"), T("0"));
	if(!mode)
		return;
	if(!strcmp(mode, "1"))
		nvram_bufset(RT2860_NVRAM, "Force_mem_upgrade", "1");
	else
		nvram_bufset(RT2860_NVRAM, "Force_mem_upgrade", "0");
	nvram_commit(RT2860_NVRAM);
    websHeader(wp);
    websWrite(wp, T("<h2>force mem upgrade</h2>\n"));
    websWrite(wp, T("mode: %s<br>\n"), mode);
    websFooter(wp);
    websDone(wp, 200);
}
#endif // Tommy debug

/* goform/setOpMode */
#if 0 // Tommy debug
static void setOpMode(webs_t wp, char_t *path, char_t *query)
{
	char_t	*mode, *nat_en;
	char	*old_mode = nvram_bufget(RT2860_NVRAM, "OperationMode");
	char	*old_nat = nvram_bufget(RT2860_NVRAM, "natEnabled");
	int		need_commit = 0;
#if defined CONFIG_RAETH_ROUTER || defined CONFIG_MAC_TO_MAC_MODE || defined CONFIG_RT_3052_ESW || defined CONFIG_ICPLUS_PHY
#else
	char	*wan_ip, *lan_ip;
#endif
// Tommy, 2008/12/23 09:08上午
//#ifdef CONFIG_RT2860V2_STA_DPB
	char_t	*econv = "";
//#endif
#if defined INIC_SUPPORT || defined INICv2_SUPPORT
	char_t	*mii;
#endif

	mode = websGetVar(wp, T("opMode"), T("0"));
	nat_en = websGetVar(wp, T("natEnbl"), T("0"));

	if (!strncmp(old_mode, "0", 2)) {
	}
	else if (!strncmp(old_mode, "1", 2) || !strncmp(old_mode, "3", 2)) {
		if (!strncmp(mode, "0", 2)) {
#if defined CONFIG_RAETH_ROUTER || defined CONFIG_MAC_TO_MAC_MODE || defined CONFIG_RT_3052_ESW || defined CONFIG_ICPLUS_PHY
#else
			/*
			 * mode: gateway (or ap-client) -> bridge
			 * config: wan_ip(wired) overwrites lan_ip(bridge)
			 */
			wan_ip = nvram_bufget(RT2860_NVRAM, "wan_ipaddr");
			nvram_bufset(RT2860_NVRAM, "lan_ipaddr", wan_ip);
			need_commit = 1;
#endif
		}
		if (!strncmp(mode, "2", 2)) {
#if defined CONFIG_RAETH_ROUTER || defined CONFIG_MAC_TO_MAC_MODE || defined CONFIG_RT_3052_ESW || defined CONFIG_ICPLUS_PHY
#else
			/*
			 * mode: gateway (or ap-client) -> ethernet-converter
			 * config: wan_ip(wired) overwrites lan_ip(wired)
			 *         lan_ip(wireless) overwrites wan_ip(wireless)
			 */
			wan_ip = nvram_bufget(RT2860_NVRAM, "wan_ipaddr");
			lan_ip = nvram_bufget(RT2860_NVRAM, "lan_ipaddr");
			nvram_bufset(RT2860_NVRAM, "lan_ipaddr", wan_ip);
			nvram_bufset(RT2860_NVRAM, "wan_ipaddr", lan_ip);
			need_commit = 1;
#endif
		}
	}
	else if (!strncmp(old_mode, "2", 2)) {
		if (!strncmp(mode, "0", 2)) {
			/*
			 * mode: wireless-isp -> bridge
			 * config: lan_ip(wired) overwrites lan_ip(bridge) -> the same
			 */
		}
		else if (!strncmp(mode, "1", 2) || !strncmp(mode, "3", 2)) {
#if defined CONFIG_RAETH_ROUTER || defined CONFIG_MAC_TO_MAC_MODE || defined CONFIG_RT_3052_ESW || defined CONFIG_ICPLUS_PHY
#else
			/*
			 * mode: ethernet-converter -> gateway (or ap-client)
			 * config: lan_ip(wired) overwrites wan_ip(wired)
			 *         wan_ip(wireless) overwrites lan_ip(wireless)
			 */
			wan_ip = nvram_bufget(RT2860_NVRAM, "wan_ipaddr");
			lan_ip = nvram_bufget(RT2860_NVRAM, "lan_ipaddr");
			nvram_bufset(RT2860_NVRAM, "lan_ipaddr", wan_ip);
			nvram_bufset(RT2860_NVRAM, "wan_ipaddr", lan_ip);
			need_commit = 1;
#endif
		}
	}

#ifdef CONFIG_RT2860V2_STA_DPB
	if (!strncmp(mode, "0", 2)) {
		char *old;

		econv = websGetVar(wp, T("ethConv"), T("0"));
		old = nvram_bufget(RT2860_NVRAM, "ethConvert");
		if (strncmp(old, econv, 2)) {
			nvram_bufset(RT2860_NVRAM, "ethConvert", econv);
			need_commit = 1;
		}
// Tommy, 2008/10/27 11:36上午
#if 0
		if (!strncmp(econv, "1", 2)) {
#else
		if (strncmp(econv, "0", 2)) {
#endif
			//disable dhcp server in this mode
			old = nvram_bufget(RT2860_NVRAM, "dhcpEnabled");
			if (!strncmp(old, "1", 2)) {
				nvram_bufset(RT2860_NVRAM, "dhcpEnabled", "0");
				need_commit = 1;
			}
		}
	}
#endif // #ifdef CONFIG_RT2860V2_STA_DPB

	//new OperationMode
// Tommy 2008/10/29 09:47上午
	if (strncmp(mode, old_mode, 2)) {
		nvram_bufset(RT2860_NVRAM, "OperationMode", mode);
	}
		//from or to ap client mode
//		if (!strncmp(mode, "3", 2))
		if ( (!strncmp(mode, "3", 2)) || (!strncmp(econv, "2", 2)) )
			nvram_bufset(RT2860_NVRAM, "ApCliEnable", "1");
//		else if (!strncmp(old_mode, "3", 2))
		else if (strncmp(econv, "2", 2)) {
			nvram_bufset(RT2860_NVRAM, "ApCliEnable", "0");
			doSystem("ifconfig apcli0 down");
	}
		need_commit = 1;
//	} //if (strncmp(mode, old_mode, 2))

	if (strncmp(nat_en, old_nat, 2)) {
		nvram_bufset(RT2860_NVRAM, "natEnabled", nat_en);
		need_commit = 1;
	}

	// For 100PHY  ( Ethernet Convertor with one port only)
	// If this is one port only board(IC+ PHY) then redirect
	// the user browser to our alias ip address.
	if( getOnePortOnly() ){
		//     old mode is Gateway, and new mode is BRIDGE/WirelessISP/Apcli
		if (    (!strcmp(old_mode, "1") && !strcmp(mode, "0"))  ||
				(!strcmp(old_mode, "1") && !strcmp(mode, "2"))  ||
				(!strcmp(old_mode, "1") && !strcmp(mode, "3"))  ){
			char redirect_url[512];
			char *lan_ip = nvram_bufget(RT2860_NVRAM, "lan_ipaddr");

			if(! strlen(lan_ip))
				lan_ip = "10.10.10.254";
			snprintf(redirect_url, 512, "http://%s", lan_ip);
			redirect_wholepage(wp, redirect_url);
			goto final;
        }

		//     old mode is BRIDGE/WirelessISP/Apcli, and new mode is Gateway
		if (    (!strcmp(old_mode, "0") && !strcmp(mode, "1"))  ||
				(!strcmp(old_mode, "2") && !strcmp(mode, "1"))  ||
				(!strcmp(old_mode, "3") && !strcmp(mode, "1"))  ){
			redirect_wholepage(wp, "http://172.32.1.254");
			goto final;
		}
	}

#if defined INIC_SUPPORT || defined INICv2_SUPPORT
	mii = websGetVar(wp, T("miiMode"), T("0"));
	if (!strncmp(mode, "0", 2)) {
		char *old_mii = nvram_bufget(RTINIC_NVRAM, "InicMiiEnable");

		if (strncmp(mii, old_mii, 2)) {
			nvram_set(RTINIC_NVRAM, "InicMiiEnable", mii);
			need_commit = 1; //force to run initInternet
		}
	}
#endif

	websHeader(wp);
	websWrite(wp, T("<h2>Operation Mode</h2>\n"));
	websWrite(wp, T("mode: %s<br>\n"), mode);
	if (strncmp(mode, "0", 2))
		websWrite(wp, T("NAT enabled: %s<br>\n"), nat_en);
#ifdef CONFIG_RT2860V2_STA_DPB
	else
		websWrite(wp, T("DPB station: %s<br>\n"), econv);
#endif
#if defined INIC_SUPPORT || defined INICv2_SUPPORT
	websWrite(wp, T("INIC MII mode: %s<br>\n"), mii);
#endif
	websFooter(wp);
	websDone(wp, 200);

final:
	sleep(1);	// wait for websDone() to finish tcp http session(close socket)

	//restart internet if any changes
	if (need_commit) {
		nvram_commit(RT2860_NVRAM);
		updateFlash8021x(RT2860_NVRAM);
		initInternet();
	}
}
#endif // Tommy debug

// Tommy, 2008/12/16 08:15下午
/* goform/SetOperationMode */
static void SetOperationMode(webs_t wp, char_t *path, char_t *query)
{
	char_t	*mode;
	mode = websGetVar(wp, T("OPMode"), T("0"));   
	char buf_ipaddr[TMP_LEN], buf_mask[TMP_LEN], wispchannel[TMP_LEN],apchannel[TMP_LEN] ;
	char wispchannel1[TMP_LEN],apchannel1[TMP_LEN] ;
	char *ipAddr, *netMask, *channel,*channel1; 

// Tommy, don't need to set these addresses
#if 0
    char	*lan_ip_router = nvram_get("lan_ipaddr_router");
    char	*lan_mask_router = nvram_get("lan_netmask_router");
    char	*lan_ip_ap = nvram_get("lan_ipaddr_ap");
    char	*lan_mask_ap = nvram_get("lan_netmask_ap");
#endif

	if (!strncmp(mode, "0", 2)) {
	    // Router Mode
	        /* 
		  * added by Bruce Liu, 2013-4-16
		  * kill some processes related to IPv6 */
		system("kill `pidof dhcp6c`");
		system("kill `pidof dhcp6s`");
		system("kill `pidof radvd`");
		/* ended by Bruce Liu */
	    system("/sbin/ezp-wl-mode normal");
// Tommy, don't need to set these addresses
#if 1
		ezplib_get_attr_val("lan_static_rule", 0, "ipaddr_normal", buf_ipaddr, TMP_LEN, EZPLIB_USE_CLI);
		ezplib_get_attr_val("lan_static_rule", 0, "mask_normal", buf_mask, TMP_LEN, EZPLIB_USE_CLI);
		ipAddr = buf_ipaddr;
		netMask = buf_mask;
		nvram_set("lan0_ipaddr", ipAddr);
		nvram_set("lan0_mask", netMask);
		ezplib_get_attr_val("wl_basic_rule", 0, "channel", wispchannel, TMP_LEN, EZPLIB_USE_CLI);             
		ezplib_get_attr_val("wl5g_basic_rule", 0, "channel", wispchannel1, TMP_LEN, EZPLIB_USE_CLI);    
		channel = wispchannel;
		channel1 = wispchannel1;
		nvram_set("WISP_Channel", channel);
		nvram_set("WISP1_Channel", channel1);
		ezplib_replace_attr("wl_basic_rule", 0, "channel", nvram_safe_get("AP_Channel"));
		ezplib_replace_attr("wl5g_basic_rule", 0, "channel", nvram_safe_get("AP1_Channel"));
		
		//Steve set lan0_proto to static when set router mode
		nvram_set("lan0_proto", "static");
#endif
	}else if (!strncmp(mode, "1", 2)){
		/* 
		  * added by Bruce Liu, 2013-4-16
		  * kill some processes related to IPv6 */
		system("kill `pidof dhcp6c`");
		system("kill `pidof dhcp6s`");
		system("kill `pidof radvd`");
		/* ended by Bruce Liu */
	    //AP Mode
	    system("/sbin/ezp-wl-mode ap");

// Liteon Modified 
#if 1
		ezplib_get_attr_val("lan_static_rule", 0, "ipaddr_ap", buf_ipaddr, TMP_LEN, EZPLIB_USE_CLI);
		ezplib_get_attr_val("lan_static_rule", 0, "mask_ap", buf_mask, TMP_LEN, EZPLIB_USE_CLI);
		ipAddr = buf_ipaddr;
		netMask = buf_mask;
		nvram_set("lan0_ipaddr", ipAddr);
		nvram_set("lan0_mask", netMask);
		nvram_set("lan0_proto", "static");
		ezplib_get_attr_val("wl_ap_basic_rule", 0, "channel", wispchannel, TMP_LEN, EZPLIB_USE_CLI);				 
		ezplib_get_attr_val("wl5g_basic_rule", 0, "channel", wispchannel1, TMP_LEN, EZPLIB_USE_CLI);				
		channel = wispchannel;
		channel1 = wispchannel1;
		nvram_set("WISP_Channel", channel);
		nvram_set("WISP1_Channel", channel1);
		nvram_commit();
		system("/sbin/ezp-wl-ctrl basic_ap &");
#endif

// Tommy, don't need to set these addresses
#if 0
		ezplib_get_attr_val("lan_static_rule", 0, "ipaddr_ap", buf_ipaddr, TMP_LEN, EZPLIB_USE_CLI);
		ezplib_get_attr_val("lan_static_rule", 0, "mask_ap", buf_mask, TMP_LEN, EZPLIB_USE_CLI);
		ipAddr = buf_ipaddr;
		netMask = buf_mask;
		nvram_set("lan0_ipaddr", ipAddr);
		nvram_set("lan0_mask", netMask);
		//ezplib_replace_attr("lan_static_rule", 0, "ipaddr", ipAddr);
		//ezplib_replace_attr("lan_static_rule", 0, "mask", netMask);
		ezplib_get_attr_val("wl_basic_rule", 0, "channel", wispchannel, TMP_LEN, EZPLIB_USE_CLI);
		ezplib_get_attr_val("wl5g_basic_rule", 0, "channel", wispchannel1, TMP_LEN, EZPLIB_USE_CLI);
		//printf("wispchannel====%s*******\n",wispchannel);
		channel = wispchannel;
		channel1 = wispchannel1;
		nvram_set("WISP_Channel", channel);
		nvram_set("WISP1_Channel", channel1);
		//printf("WISP_Channel ===%s******\n", channel);
		ezplib_replace_attr("wl_basic_rule", 0, "channel", nvram_safe_get("AP_Channel"));
		ezplib_replace_attr("wl5g_basic_rule", 0, "channel", nvram_safe_get("AP1_Channel"));
#endif		
		//nvram_bufset(RT2860_NVRAM, "lanIp_mode", "1");//fixed IP : Need to implement
	}else if (!strncmp(mode, "2", 2)){
	    // Client Mode
	}else if (!strncmp(mode, "3", 2)){
	    // WDS Mode
	}else if (!strncmp(mode, "4", 2)){
	    // AP + WDS
	}else if (!strncmp(mode, "5", 2)){
	    // Universal Repeater (AP + Client)
		/* 
		  * added by Bruce Liu, 2013-4-16
		  * kill some processes related to IPv6 */
		system("kill `pidof dhcp6c`");
		system("kill `pidof dhcp6s`");
		system("kill `pidof radvd`");
		/* ended by Bruce Liu */
#if 1
		//nvram_set("lan0_proto", "static");
		ezplib_get_attr_val("lan_static_rule", 0, "ipaddr_ap", buf_ipaddr, TMP_LEN, EZPLIB_USE_CLI);
		ezplib_get_attr_val("lan_static_rule", 0, "mask_ap", buf_mask, TMP_LEN, EZPLIB_USE_CLI);
		ipAddr = buf_ipaddr;
		netMask = buf_mask;
		nvram_set("lan0_ipaddr", ipAddr);
		nvram_set("lan0_mask", netMask);
		/*char *br_mode = websGetVar(wp, T("ModeType"),T(""));
		ezplib_replace_attr("bridge_rule", 0, "brg_mode", br_mode);*/
		ezplib_replace_attr("bridge_rule", 0, "brg_mode", "0");
		//ezplib_replace_attr("lan_static_rule", 0, "ipaddr", ipAddr);
		//ezplib_replace_attr("lan_static_rule", 0, "mask", netMask);
#endif

		system("/sbin/ezp-wl-mode ur");
		nvram_commit();
		system("/sbin/ezp-wl-ctrl ur_brg &");
		//nvram_bufset(RT2860_NVRAM, "lanIp_mode", "1");//fixed IP : Need to implement
	}else if (!strncmp(mode, "6", 2)){
	    // WISP Mode <Ralink: Ethernet Converter>
	    //TODO 2.4g 5G
	    char tm[16];
	    char boardtype[TMP_LEN];
		ezplib_get_attr_val("board_model_rule", 0, "bd_model", boardtype, TMP_LEN, EZPLIB_USE_CLI);	
		char *if_ptr = websGetVar(wp, T("connectionType"), T(""));
		char *dev_ptr = nvram_safe_get("wan0_ifname");
		ezplib_get_attr_val("board_model_rule", 0, "bd_model", buf_mask, TMP_LEN, EZPLIB_USE_CLI);
#if 0
		if(!if_ptr) {
			memset(tm,0,sizeof(tm));
			if_ptr = tm;
			strcpy(if_ptr,"apclii0");
			nvram_set("wan0_device",if_ptr);
			system("/sbin/ezp-wl0-mode wisp_single_raido");
		}else if(!strcmp(if_ptr, "apclii0")){
			nvram_set("wan0_device",if_ptr);
			system("/sbin/ezp-wl1-mode wisp_ap");
			system("/sbin/ezp-wl0-mode wisp");
		}else if(!strcmp(if_ptr, "apcli0")){
			nvram_set("wan0_device",if_ptr);
			system("/sbin/ezp-wl0-mode wisp_ap");
			system("/sbin/ezp-wl1-mode wisp");
		}else{
			memset(tm,0,sizeof(tm));
			if_ptr = tm;
			strcpy(if_ptr,"apclii0");
			nvram_set("wan0_device",if_ptr);
			system("/sbin/ezp-wl0-mode wisp_single_radio");
		}
#endif
	    if(!strncmp(boardtype, "0A22", 4) || !strncmp(boardtype, "0A52", 4)){
			memset(tm,0,sizeof(tm));
			if_ptr = tm;
			strcpy(if_ptr,"apcli0");
			nvram_set("wan0_device",if_ptr);
			system("/sbin/ezp-wl0-mode wisp_ap");
			system("/sbin/ezp-wl1-mode wisp");
		} else {
			memset(tm,0,sizeof(tm));
			if_ptr = tm;
			strcpy(if_ptr,"apclii0");
			nvram_set("wan0_device",if_ptr);
			system("/sbin/ezp-wl0-mode wisp_single_radio");
		}

		if (!strncmp(dev_ptr, "ppp", 3)) {
			//if (snprintf(device, 16, "%s_device", type) >= 16)
			//if (snprintf(device, 16, "wan0_device") >= 16)
			//        return -1;
		}else{
			nvram_set("wan0_ifname", if_ptr);
		}
		
// Enable apcli0 interface in WISP
		ezplib_replace_attr("wl0_apcli_rule", 0, "enable", "1");
		if(!strncmp(boardtype, "0A22", 4) || !strncmp(boardtype, "0A52", 4))
			ezplib_replace_attr("wl1_apcli_rule", 0, "enable", "1");
	    
// Tommy, don't need to set these addresses
#if 1
		nvram_set("lan0_proto", "static");
		ezplib_get_attr_val("lan_static_rule", 0, "ipaddr_normal", buf_ipaddr, TMP_LEN, EZPLIB_USE_CLI);
		ezplib_get_attr_val("lan_static_rule", 0, "mask_normal", buf_mask, TMP_LEN, EZPLIB_USE_CLI);
		ipAddr = buf_ipaddr;
		netMask = buf_mask;
		nvram_set("lan0_ipaddr", ipAddr);
		nvram_set("lan0_mask", netMask);
		      
		ezplib_get_attr_val("wl_basic_rule", 0, "channel", apchannel, TMP_LEN, EZPLIB_USE_CLI); 
		if(!strncmp(boardtype, "0A22", 4) || !strncmp(boardtype, "0A52", 4))
			ezplib_get_attr_val("wl5g_basic_rule", 0, "channel", apchannel1, TMP_LEN, EZPLIB_USE_CLI);
		channel = apchannel;
		channel1 = apchannel1;
		nvram_set("AP_Channel", channel);
		if(!strncmp(boardtype, "0A22", 4) || !strncmp(boardtype, "0A52", 4))
			nvram_set("AP1_Channel", channel1);
		
		ezplib_replace_attr("wl_basic_rule", 0, "channel", nvram_safe_get("WISP_Channel"));
		if(!strncmp(boardtype, "0A22", 4) || !strncmp(boardtype, "0A52", 4))
			ezplib_replace_attr("wl5g_basic_rule", 0, "channel", nvram_safe_get("WISP1_Channel"));
#endif
		nvram_commit();
		if(!strncmp(boardtype, "0A22", 4) || !strncmp(boardtype, "0A52", 4)){
			if(!strcmp(if_ptr, "apclii0")) {
				system("/sbin/ezp-wl1-ctrl wisp_ap &");
				system("/sbin/ezp-wl0-ctrl wisp &");
			}else{
				system("/sbin/ezp-wl0-ctrl wisp_ap &");
				system("/sbin/ezp-wl1-ctrl wisp &");
			}
        }else {
        	system("/sbin/ezp-wl0-ctrl wisp_single_radio &");
        }
			
	}else if (!strncmp(mode, "7", 2)){
#if 0
		// WISP Mode + Universal Repeater Mode
		system("/sbin/ezp-wl-mode wisp_ur");

		// Enable apcli0 interface in WISP	    
		ezplib_replace_attr("wl0_apcli_rule", 0, "enable", "1");

		//ezplib_replace_attr("wl_mode_rule", 0, "mode", "wisp");
// Tommy, don't need to set these addresses
#if 1
		nvram_set("lan0_proto", "static");
		ezplib_get_attr_val("lan_static_rule", 0, "ipaddr_normal", buf_ipaddr, TMP_LEN, EZPLIB_USE_CLI);
		ezplib_get_attr_val("lan_static_rule", 0, "mask_normal", buf_mask, TMP_LEN, EZPLIB_USE_CLI);
		ipAddr = buf_ipaddr;
		netMask = buf_mask;
		nvram_set("lan0_ipaddr", ipAddr);
		nvram_set("lan0_mask", netMask);
		
		//ezplib_replace_attr("lan_static_rule", 0, "ipaddr", ipAddr);
		//ezplib_replace_attr("lan_static_rule", 0, "mask", netMask);
		
		ezplib_get_attr_val("wl_basic_rule", 0, "channel", apchannel, TMP_LEN, EZPLIB_USE_CLI);
		//printf("apchannel ===%s******\n", apchannel);
		channel = apchannel;
		nvram_set("AP_Channel", channel);
		//printf("AP_Channel ===%s******\n", channel);
		ezplib_replace_attr("wl_basic_rule", 0, "channel", nvram_safe_get("WISP_Channel"));
#endif
#endif
	}
        nvram_commit();

	//added by Gordon  2009/03/18
	// system("sleep 3 && reboot &");
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


// Steve, 2009/01/16 02:45下午
/*  */
char *ReadInfo(int type,char *s, int index)
{
	static char str[64]={0};
	char buf[70]={0};
	char file_name[20]={0};

	if(type==0){ //lan
		if(index==0)//num
		    sprintf(file_name, "/var/lltd_num_lan");
		else //info
			sprintf(file_name, "/var/lltd_inf_lan%d", index);
	}else{
	    if(index==0)//num
	    	sprintf(file_name, "/var/lltd_num_wlan");
	    else
	    	sprintf(file_name, "/var/lltd_inf_wlan%d", index);
	}

    FILE *fp = fopen(file_name, "r");
    if(!fp){
        //printf("no file ===> \n");
        return "NULL";
    }

    while(fgets(buf, 64, fp)){
    	memset(str,0,sizeof(str));
        if(!strncmp(buf,s,2)){
        	memcpy(str,buf+3,(strlen(buf)-3));
        	if (str[strlen(buf)-4]=='\n')
        		str[strlen(buf)-4]=0;
        	break;
        }
        memset(buf,0,strlen(buf));
    }
    fclose(fp);

	return str;
}

static int getCfgLLTD(int eid, webs_t wp, int argc, char_t **argv)
{
	int type, idx;
	char_t *field;
	char *value;
	char *nth;

	if (ejArgs(argc, argv, T("%d %s %d"), &type, &field, &idx) < 3) {
		return websWrite(wp, T("Insufficient args\n"));
	}

	return websWrite(wp, T("%s"), ReadInfo(type,field,idx));

}

#if 1//Arthur Chow 2009-02-15: For easy mode page
static int startLLTDcheck(int eid, webs_t wp, int argc, char_t **argv)
{

printf("startLLTDcheck\n");
#if 1
    FILE *fp = NULL;
    int scanned=1;
 
    fp = fopen("/var/lltd_num_lan", "r");
    if(!fp){
        scanned=0;
    }
		else
		{
			fclose(fp);
    	fp = fopen("/var/lltd_num_wlan", "r");
    	if(!fp){
        scanned=0;
    	}
			fclose(fp);
  	}

	if (!scanned)
	{
	system("lltd.sh 5 &");
	return websWrite(wp, T("1"));
}
	else	
	{
		return websWrite(wp, T("0"));
	}
#else
printf("\nSteve marked Temporarily\n");
return websWrite(wp, T("0"));
#endif
}

static int statusRoutercheck(int eid, webs_t wp, int argc, char_t **argv)
{
	char buf[1024];
	FILE *fp = NULL;
	int len=0;

	if (NULL == (fp = fopen("/tmp/ping_router", "r")))
	{
		websWrite(wp, T("0"));
		return 1;
	}
	len = fscanf(fp, "%s", buf);
//	printf("1.buf[%s]\n", buf);
	len = fscanf(fp, "%s", buf);
//	printf("2.buf[%s]\n", buf);
	len = fscanf(fp, "%s", buf);
//	printf("3.buf[%s]\n", buf);
	len = fscanf(fp, "%s", buf);
//	printf("4.buf[%s]\n", buf);
	len = fscanf(fp, "%s", buf);
//	printf("5.buf[%s]\n", buf);
	len = fscanf(fp, "%s", buf);
//	printf("6.buf[%s]\n", buf);
	len = fscanf(fp, "%s", buf);
//	printf("7.buf[%s]\n", buf);
	fclose(fp);
	if (len==1)
	{
		if (!strcmp(buf, "64"))
			websWrite(wp, T("1"));
		else
			websWrite(wp, T("0"));
		return 1;
	}
	websWrite(wp, T("0"));
	return 1;
}

static int statusInternetcheck(int eid, webs_t wp, int argc, char_t **argv)
{
	char buf[1024];
	FILE *fp = NULL;
	int len=0;

	if (NULL == (fp = fopen("/tmp/ping_internet", "r")))
	{
		websWrite(wp, T("0"));
		return 1;
	}
	len = fscanf(fp, "%s", buf);
//	printf("1.buf[%s]\n", buf);
	len = fscanf(fp, "%s", buf);
//	printf("2.buf[%s]\n", buf);
	len = fscanf(fp, "%s", buf);
//	printf("3.buf[%s]\n", buf);
	len = fscanf(fp, "%s", buf);
//	printf("4.buf[%s]\n", buf);
	len = fscanf(fp, "%s", buf);
//	printf("5.buf[%s]\n", buf);
	len = fscanf(fp, "%s", buf);
//	printf("6.buf[%s]\n", buf);
	len = fscanf(fp, "%s", buf);
//	printf("7.buf[%s]\n", buf);
	fclose(fp);
	if (len==1)
	{
		if (!strcmp(buf, "64"))
			websWrite(wp, T("1"));
		else
			websWrite(wp, T("0"));
		return 1;
	}
	websWrite(wp, T("0"));
	return 1;
}

static int startInternetcheck(int eid, webs_t wp, int argc, char_t **argv)
{
  FILE *fp = NULL;
 
  fp = fopen("/tmp/ping_internet", "r");
  if(fp){
		fclose(fp);
		return websWrite(wp, T("1"));
	}
	else
	{
	system("ping 168.95.1.1 -c 1 > /tmp/ping_internet &");
	return websWrite(wp, T("1"));
	}
}

static int javascriptLLTD(int eid, webs_t wp, int argc, char_t **argv)
{
	int idx=0;
	int lan_num=0;
	int wlan_num=0;

	lan_num=atoi(ReadInfo(0,"00",0));

	websWrite(wp, T("var lan_num=%d;\n"), lan_num);
	if (lan_num)
	{
	 websWrite(wp, T("var lan_mac=new Array(%d);\n"), lan_num);
	 websWrite(wp, T("var lan_ip=new Array(%d);\n"), lan_num);
	 websWrite(wp, T("var lan_name=new Array(%d);\n"), lan_num);
	 websWrite(wp, T("var lan_type=new Array(%d);\n"), lan_num);

	 for (idx=1; idx<=lan_num; idx++)
	 {
		websWrite(wp, T("lan_mac[%d]='%s';\n"), idx-1, ReadInfo(0, "01",idx));
		websWrite(wp, T("lan_ip[%d]='%s';\n"), idx-1, ReadInfo(0, "07",idx));
		websWrite(wp, T("lan_name[%d]='%s';\n"), idx-1, ReadInfo(0, "15",idx));
		websWrite(wp, T("lan_type[%d]=%s;\n"), idx-1, ReadInfo(0, "99",idx));
	 }
	}

	wlan_num=atoi(ReadInfo(1,"00",0));
	websWrite(wp, T("var wlan_num=%d;\n"), wlan_num);
	if (wlan_num)
	{
	 websWrite(wp, T("var wlan_mac=new Array(%d);\n"), wlan_num);
	 websWrite(wp, T("var wlan_ip=new Array(%d);\n"), wlan_num);
	 websWrite(wp, T("var wlan_name=new Array(%d);\n"), wlan_num);
	 websWrite(wp, T("var wlan_type=new Array(%d);\n"), wlan_num);

	 for (idx=1; idx<=wlan_num; idx++)
	 {
		websWrite(wp, T("wlan_mac[%d]='%s';\n"), idx-1, ReadInfo(1, "01",idx));
		websWrite(wp, T("wlan_ip[%d]='%s';\n"), idx-1, ReadInfo(1, "07",idx));
		websWrite(wp, T("wlan_name[%d]='%s';\n"), idx-1, ReadInfo(1, "15",idx));
		websWrite(wp, T("wlan_type[%d]=%s;\n"), idx-1, ReadInfo(1, "99",idx));
	 }
	}

	//Steve add 2009/03/20
	system("lltd.sh 1 &");
		
	return 1;
}

/* goform/easy_setting */
extern int WLAN_Scheduler(int setting);
extern int parental_control_switch(int flag);
extern int QoS_EasyMode(int qos_on);
extern int firewall_switch(int flag);

static void easy_setting(webs_t wp, char_t *path, char_t *query)
{
	char_t	*job_str;
	char_t	*mode_str;
	int job=0;
	int mode=0;

	job_str = websGetVar(wp, T("easy_set_button_job"), T("0"));
	if (job_str[0]!=0)
		job=atoi(job_str);

	mode_str = websGetVar(wp, T("easy_set_button_mode"), T("0"));
	if (mode_str[0]!=0)
		mode=atoi(mode_str);

	if (job==1)
	{
		if (ezModeGameEngine(mode))
		{
		}
	}

	if (job==2)
	{
		if (WLAN_Scheduler(mode))
		{
		}
	}

	if (job==3)
	{
		if (parental_control_switch(mode))
		{
		}
	}

	if (job==4)
	{
		if (QoS_EasyMode(mode))
		{
		}
	}

	if (job==5)
	{
		if (firewall_switch(mode))
		{
		}
	}

	if (job==6)
	{
//		if (set_wireless_security(mode))
		{
			//nvram_bufset(RT2860_NVRAM, "easy_wireless_security", mode_str);
			//nvram_commit(RT2860_NVRAM);
		}
	}

	websRedirect(wp, "local/easy_info.asp");
}

static int clearLLTDInfo(int eid, webs_t wp, int argc, char_t **argv)
{
	system("rm /tmp/ping_internet");
	system("rm /tmp/ping_router");
	system("rm /var/lltd_num_lan");
	system("rm /var/lltd_num_wlan");
	return websWrite(wp, T(""));
}

static int existLLTDinfo(int eid, webs_t wp, int argc, char_t **argv)
{
    FILE *fp = NULL;
    int scanned=1;
 
    fp = fopen("/var/lltd_num_lan", "r");
    if(!fp){
        scanned=0;
    }
		else
		{
			fclose(fp);
    	fp = fopen("/var/lltd_num_wlan", "r");
    	if(!fp){
        scanned=0;
    	}
			fclose(fp);
  	}

	if (!scanned)
	{
	return websWrite(wp, T("0"));
}
	else	
	{
		return websWrite(wp, T("1"));
	}
}
#endif

//FW_DATE & FW_VERSION at utils.h
#if 1//Arthur Chow 2009-03-30
static int getFirmwareDate(int eid, webs_t wp, int argc, char_t **argv)
{
    websWrite(wp, "%s ", FW_DATE);  
    return 1;
}
static int getFirmwareVersion(int eid, webs_t wp, int argc, char_t **argv)
{
    FILE *fp;
    char c;
    char ch;
    int count=0,i=0;
	
#if 1
    if(!(fp=fopen("/version","r"))){
    	 printf("cant open file!\n");
	 websWrite(wp, "%s", FW_VERSION);
	 return;
    }
    ch=fgetc(fp);
    while(ch!= EOF)
    {
        ch=fgetc(fp);
        count++;
    }

    char *buffer = (char*)malloc(count*sizeof(char));
	if(buffer != NULL)
	{
		memset(buffer,0,count*sizeof(char));
	}
	if (!(fp = fopen("/version", "r")))
        return;
        printf("get value=\n");
    while ((c = getc(fp)) != EOF) {
        printf("%c",c);
        buffer[i] = c;
		i++;
    }
	i--;
    fclose(fp);
    buffer[i] = '\0';
    websWrite(wp, "%s", buffer);
	//printf("\n\nbuffer value=%s\n\n",buffer);
    free(buffer);
#else
    websWrite(wp, "%s ", nvram_safe_get("fw_version"));
    websWrite(wp, "(%s.", nvram_safe_get("prod_cat"));
    websWrite(wp, "%s)", nvram_safe_get("prod_subcat"));
#endif
    return 1;
}
#endif

//Steve 2010-08-27
static int getGetUploadFlag(int eid, webs_t wp, int argc, char_t **argv)
{
	FILE *fs;
	char str[2];
	if((fs=fopen("/tmp/uploadOK","r"))==NULL){
	 	websWrite(wp, T("0"));
	    return 0;
	}else
	{
	    fgets(str,2,fs);
	    //printf("The str[0]:%c ,The str[1]:%c \n",str[0],str[1]);
	}
	fclose(fs);
	if (str[0] == '1'){
	     websWrite(wp, T("1"));
	     return 1;
	}else{
	     websWrite(wp, T("0"));
	     return 0;
	}
}

#endif //Steve2023

