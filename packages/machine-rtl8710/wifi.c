#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#include <wifi_structures.h>
#include <wifi_constants.h>
#include <wireless.h>

// Use realtek firmware blob implementations
#define memset _memset
#define memcpy _memcpy

extern __attribute__ ((long_call)) uint32_t
DiagPrintf(
    const char *fmt, ...
);

#define MAC_FMT "%02x:%02x:%02x:%02x:%02x:%02x"
#define MAC_ARG(x) ((uint8_t*)(x))[0],((uint8_t*)(x))[1],((uint8_t*)(x))[2],((uint8_t*)(x))[3],((uint8_t*)(x))[4],((uint8_t*)(x))[5]

static void print_scan_result( rtw_scan_result_t* record ) {
    DiagPrintf("%s,%d,%s,%d,"MAC_FMT"", record->SSID.val, record->channel,
    		( record->security == RTW_SECURITY_OPEN ) ? "Open" :
            ( record->security == RTW_SECURITY_WEP_PSK ) ? "WEP" :
            ( record->security == RTW_SECURITY_WPA_TKIP_PSK ) ? "WPA TKIP" :
            ( record->security == RTW_SECURITY_WPA_AES_PSK ) ? "WPA AES" :
            ( record->security == RTW_SECURITY_WPA2_AES_PSK ) ? "WPA2 AES" :
            ( record->security == RTW_SECURITY_WPA2_TKIP_PSK ) ? "WPA2 TKIP" :
            ( record->security == RTW_SECURITY_WPA2_MIXED_PSK ) ? "WPA2 Mixed" :
            ( record->security == RTW_SECURITY_WPA_WPA2_MIXED ) ? "WPA/WPA2 AES" : "Unknown",
            record->signal_strength, MAC_ARG(record->BSSID.octet)   );
}

static rtw_result_t app_scan_result_handler( rtw_scan_handler_result_t* malloced_scan_result ) {
	static int ApNum = 0;
	if (malloced_scan_result->scan_complete != RTW_TRUE) {
		rtw_scan_result_t* record = &malloced_scan_result->ap_details;
		record->SSID.val[record->SSID.len] = 0; /* Ensure the SSID is null terminated */

		DiagPrintf("\r\nAP : %d,", ++ApNum);
		print_scan_result(record);
		if(malloced_scan_result->user_data)
			memcpy((void *)((char *)malloced_scan_result->user_data+(ApNum-1)*sizeof(rtw_scan_result_t)), (char *)record, sizeof(rtw_scan_result_t));
	} else{
		if(malloced_scan_result->user_data)
			free(malloced_scan_result->user_data);
		DiagPrintf("\r\n[ATWS] OK");
		ApNum = 0;
	}
	return RTW_SUCCESS;
}

typedef rtw_result_t (*rtw_scan_result_handler_t)( rtw_scan_handler_result_t* malloced_scan_result );

typedef struct internal_scan_handler{
	rtw_scan_result_t** pap_details;
	rtw_scan_result_t * ap_details;
	int	scan_cnt;
	rtw_bool_t	scan_complete;
	unsigned char	max_ap_size;
	rtw_scan_result_handler_t gscan_result_handler;
#if SCAN_USE_SEMAPHORE
	void *scan_semaphore;
#else
	int 	scan_running;
#endif
	void*	user_data;
	unsigned int	scan_start_time;
} internal_scan_handler_t;

static internal_scan_handler_t scan_result_handler_ptr = {0, 0, 0, RTW_FALSE, 0, 0, 0, 0, 0};


#define WIFI_EVENT_MAX_ROW	3

typedef void (*rtw_event_handler_t)(char *buf, int buf_len, int flags, void* handler_user_data );

typedef struct {
	rtw_event_handler_t	handler;
	void*	handler_user_data;
} event_list_elem_t;

static event_list_elem_t event_callback_list[WIFI_EVENT_MAX][WIFI_EVENT_MAX_ROW];

void init_event_callback_list(){
	memset(event_callback_list, 0, sizeof(event_callback_list));
}

void wifi_reg_event_handler(unsigned int event_cmds, rtw_event_handler_t handler_func, void *handler_user_data)
{
	int i = 0, j = 0;
	if(event_cmds < WIFI_EVENT_MAX){
		for(i=0; i < WIFI_EVENT_MAX_ROW; i++){
			if(event_callback_list[event_cmds][i].handler == NULL){
			    for(j=0; j<WIFI_EVENT_MAX_ROW; j++){
			        if(event_callback_list[event_cmds][j].handler == handler_func){
			            return;
			        }
			    }
				event_callback_list[event_cmds][i].handler = handler_func;
				event_callback_list[event_cmds][i].handler_user_data = handler_user_data;
				return;
			}
		}
	}
}

void wifi_unreg_event_handler(unsigned int event_cmds, rtw_event_handler_t handler_func)
{
	int i;
	if(event_cmds < WIFI_EVENT_MAX){
		for(i = 0; i < WIFI_EVENT_MAX_ROW; i++){
			if(event_callback_list[event_cmds][i].handler == handler_func){
				event_callback_list[event_cmds][i].handler = NULL;
				event_callback_list[event_cmds][i].handler_user_data = NULL;
				return;
			}
		}
	}
}

int iw_ioctl(const char * ifname, unsigned long request, struct iwreq *	pwrq)
{
	memcpy(pwrq->ifr_name, ifname, 5);
	return rltk_wlan_control(request, (void *) pwrq);
}

int wext_set_scan(const char *ifname, char *buf, uint16_t buf_len, uint16_t flags)
{
	struct iwreq iwr;
	int ret = 0;

	memset(&iwr, 0, sizeof(iwr));
	iwr.u.data.pointer = buf;
	iwr.u.data.flags = flags;
	iwr.u.data.length = buf_len;
	if (iw_ioctl(ifname, SIOCSIWSCAN, &iwr) < 0) {
		DiagPrintf("\n\rioctl[SIOCSIWSCAN] error");
		ret = -1;
	}
	return ret;
}

int wext_get_scan(const char *ifname, char *buf, __u16 buf_len)
{
	struct iwreq iwr;
	int ret = 0;

	iwr.u.data.pointer = buf;
	iwr.u.data.length = buf_len;
	if (iw_ioctl(ifname, SIOCGIWSCAN, &iwr) < 0) {
		DiagPrintf("\n\rioctl[SIOCGIWSCAN] error");
		ret = -1;
	}else
		ret = iwr.u.data.flags;
	return ret;
}

#define CMP_MAC( a, b )  (((a[0])==(b[0]))&& \
                          ((a[1])==(b[1]))&& \
                          ((a[2])==(b[2]))&& \
                          ((a[3])==(b[3]))&& \
                          ((a[4])==(b[4]))&& \
                          ((a[5])==(b[5])))

void wifi_scan_each_report_hdl( char* buf, int buf_len, int flags, void* userdata)
{
    int i =0;
    int j =0;
    int insert_pos = 0;
    rtw_scan_result_t** result_ptr = (rtw_scan_result_t**)buf;
    rtw_scan_result_t* temp = NULL;

    for(i=0; i<scan_result_handler_ptr.scan_cnt; i++){
        if(CMP_MAC(scan_result_handler_ptr.pap_details[i]->BSSID.octet, (*result_ptr)->BSSID.octet)){
            if((*result_ptr)->signal_strength > scan_result_handler_ptr.pap_details[i]->signal_strength){
                temp = scan_result_handler_ptr.pap_details[i];	
                for(j = i-1; j >= 0; j--){
                    if(scan_result_handler_ptr.pap_details[j]->signal_strength >= (*result_ptr)->signal_strength)
                        break;	
                    else
                        scan_result_handler_ptr.pap_details[j+1] = scan_result_handler_ptr.pap_details[j];
                }	
                scan_result_handler_ptr.pap_details[j+1] = temp;
                scan_result_handler_ptr.pap_details[j+1]->signal_strength = (*result_ptr)->signal_strength;
            }
            memset(*result_ptr, 0, sizeof(rtw_scan_result_t));
            return;
        }
    }

    scan_result_handler_ptr.scan_cnt++;

    if(scan_result_handler_ptr.scan_cnt > scan_result_handler_ptr.max_ap_size){
        scan_result_handler_ptr.scan_cnt = scan_result_handler_ptr.max_ap_size;
        if((*result_ptr)->signal_strength > scan_result_handler_ptr.pap_details[scan_result_handler_ptr.max_ap_size-1]->signal_strength){
            memcpy(scan_result_handler_ptr.pap_details[scan_result_handler_ptr.max_ap_size-1], *result_ptr, sizeof(rtw_scan_result_t));
            temp = scan_result_handler_ptr.pap_details[scan_result_handler_ptr.max_ap_size -1];
        }else
            return;
    }else{
        memcpy(&scan_result_handler_ptr.ap_details[scan_result_handler_ptr.scan_cnt-1], *result_ptr, sizeof(rtw_scan_result_t));
    }

    for(i=0; i< scan_result_handler_ptr.scan_cnt-1; i++){
        if((*result_ptr)->signal_strength > scan_result_handler_ptr.pap_details[i]->signal_strength)
            break;
    }
    insert_pos = i;

    for(i = scan_result_handler_ptr.scan_cnt-1; i>insert_pos; i--)
        scan_result_handler_ptr.pap_details[i] = scan_result_handler_ptr.pap_details[i-1];

    if(temp != NULL)
        scan_result_handler_ptr.pap_details[insert_pos] = temp;
    else
        scan_result_handler_ptr.pap_details[insert_pos] = &scan_result_handler_ptr.ap_details[scan_result_handler_ptr.scan_cnt-1];
    memset(*result_ptr, 0, sizeof(rtw_scan_result_t));
}

void wifi_scan_done_hdl( char* buf, int buf_len, int flags, void* userdata)
{
    int i = 0;
    rtw_scan_handler_result_t scan_result_report;

    for(i=0; i<scan_result_handler_ptr.scan_cnt; i++){
        memcpy(&scan_result_report.ap_details, scan_result_handler_ptr.pap_details[i], sizeof(rtw_scan_result_t));
        scan_result_report.scan_complete = scan_result_handler_ptr.scan_complete;
        scan_result_report.user_data = scan_result_handler_ptr.user_data;
        (*scan_result_handler_ptr.gscan_result_handler)(&scan_result_report);
    }

    scan_result_handler_ptr.scan_complete = RTW_TRUE;
    scan_result_report.scan_complete = RTW_TRUE;
    (*scan_result_handler_ptr.gscan_result_handler)(&scan_result_report);

    free(scan_result_handler_ptr.ap_details);
    free(scan_result_handler_ptr.pap_details);
    scan_result_handler_ptr.scan_running = 0;
    wifi_unreg_event_handler(WIFI_EVENT_SCAN_RESULT_REPORT, wifi_scan_each_report_hdl);
    wifi_unreg_event_handler(WIFI_EVENT_SCAN_DONE, wifi_scan_done_hdl);
    return;
}

typedef struct {
	char *buf;
	int buf_len;
} scan_buf_arg;

int wifi_scan(rtw_scan_type_t scan_type, rtw_bss_type_t bss_type, void* result_ptr) {
    int ret;
    scan_buf_arg * pscan_buf;
    uint16_t flags = scan_type | (bss_type << 8);
    if(result_ptr != NULL){
        pscan_buf = (scan_buf_arg *)result_ptr;
        ret = wext_set_scan(WLAN0_NAME, (char*)pscan_buf->buf, pscan_buf->buf_len, flags);
    }else{
        wifi_reg_event_handler(WIFI_EVENT_SCAN_RESULT_REPORT, wifi_scan_each_report_hdl, NULL);
        wifi_reg_event_handler(WIFI_EVENT_SCAN_DONE, wifi_scan_done_hdl, NULL);
        ret = wext_set_scan(WLAN0_NAME, NULL, 0, flags);
    }

    if(ret == 0) {
        if(result_ptr != NULL){
            ret = wext_get_scan(WLAN0_NAME, pscan_buf->buf, pscan_buf->buf_len);
        }
    }
    return ret;
}

int wifi_scan_networks(rtw_scan_result_handler_t results_handler, void* user_data) {
	unsigned int max_ap_size = 64;

	if(scan_result_handler_ptr.scan_running){
		int count = 100;
		while(scan_result_handler_ptr.scan_running && count > 0) {
            //TODO: SLEEP PROPERLY HERE
            for(int i = 0; i != 1000000; ++i);
			count --;
		}
		if(count == 0){
			DiagPrintf("\n\r[%d]WiFi: Scan is running. Wait 2s timeout.", rtw_get_current_time());
			return RTW_TIMEOUT;
		}
	}
	scan_result_handler_ptr.scan_start_time = rtw_get_current_time();
	scan_result_handler_ptr.scan_running = 1;

	scan_result_handler_ptr.gscan_result_handler = results_handler;

	scan_result_handler_ptr.max_ap_size = max_ap_size;
	scan_result_handler_ptr.ap_details = (rtw_scan_result_t*)malloc(max_ap_size*sizeof(rtw_scan_result_t));
	if(scan_result_handler_ptr.ap_details == NULL){
		goto err_exit;
	}
	memset(scan_result_handler_ptr.ap_details, 0, max_ap_size*sizeof(rtw_scan_result_t));

	scan_result_handler_ptr.pap_details = (rtw_scan_result_t**)malloc(max_ap_size*sizeof(rtw_scan_result_t*));
	if(scan_result_handler_ptr.pap_details == NULL)
		goto error2_with_result_ptr;
	memset(scan_result_handler_ptr.pap_details, 0, max_ap_size);

	scan_result_handler_ptr.scan_cnt = 0;

	scan_result_handler_ptr.scan_complete = RTW_FALSE;
	scan_result_handler_ptr.user_data = user_data;

	if (wifi_scan( RTW_SCAN_COMMAMD<<4 | RTW_SCAN_TYPE_ACTIVE, RTW_BSS_TYPE_ANY, NULL) != RTW_SUCCESS)
	{
		goto error1_with_result_ptr;
	}

	return RTW_SUCCESS;

error1_with_result_ptr:
	free((uint8_t*)scan_result_handler_ptr.pap_details);
	scan_result_handler_ptr.pap_details = NULL;

error2_with_result_ptr:
	free((uint8_t*)scan_result_handler_ptr.ap_details);
	scan_result_handler_ptr.ap_details = NULL;

err_exit:
	memset((void *)&scan_result_handler_ptr, 0, sizeof(scan_result_handler_ptr));
	return RTW_ERROR;
}


void fATWS( ) {
    int ret = RTW_SUCCESS;
    uint8_t *inic_scan_buf = NULL;

    DiagPrintf("[ATWS]: _AT_WLAN_SCAN_\n\r");

    inic_scan_buf = malloc(65*sizeof(rtw_scan_result_t));
    if(inic_scan_buf == NULL){
        ret = RTW_BUFFER_UNAVAILABLE_TEMPORARY;
        goto exit;
    }
    memset(inic_scan_buf, 0, 65*sizeof(rtw_scan_result_t));
    if((ret = wifi_scan_networks(app_scan_result_handler, inic_scan_buf)) != RTW_SUCCESS){
        DiagPrintf("[ATWS]ERROR: wifi scan failed\n\r");
        goto exit;
    }
exit:
    if(ret != RTW_SUCCESS){
        if(inic_scan_buf)
            free(inic_scan_buf);
        DiagPrintf("ATWS failed: %d", ret);
    }
}
