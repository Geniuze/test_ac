#ifndef __DSTRING_H__
#define __DSTRING_H__

#include <string>
#include <vector>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define ETHER_ADDR_FMT "%02X-%02X-%02X-%02X-%02X-%02X"
#define ETHER_ADDR_VAL(addr) (addr[0]),(addr[1]),   \
        (addr[2]),(addr[3]),                        \
        (addr[4]),(addr[5])
#define ETHER_ADDR_RVAL(addr) (&addr[0]),(&addr[1]), \
        (&addr[2]),(&addr[3]), \
        (&addr[4]),(&addr[5])

#define IP_ADDR_FMT "%u.%u.%u.%u"
#define IP_ADDR_VAL(addr) (addr[0]),(addr[1]),(addr[2]),(addr[3])
#define IP_ADDR_RVAL(addr) (&addr[0]),(&addr[1]),(&addr[2]),(&addr[3])



#define STRING_AP_MAC "apmac"
#define STRING_AP_MAC_LEN "apmac_len"
#define STRING_SERIAL_NUMBER "serial_number"
#define STRING_DEV_MODEL "dev_model"
#define STRING_COMPANY_ID "company_id"
#define STRING_HARDWARE_VERSION "hardware_version"
#define STRING_SOFTWARE_VERSION "software_version"
#define STRING_LAN_IPADDR_TYPE "lan_ipaddr_type"
#define STRING_LAN_IPADDR "lan_ipaddr"
#define STRING_WAN_IPADDR "wan_ipaddr"
#define STRING_RADIO_ID "radio_id"
#define STRING_RADIO_TYPE_11N "radio_type_11n"
#define STRING_RADIO_TYPE_11A "radio_type_11a"
#define STRING_RADIO_TYPE_11B "radio_type_11b"
#define STRING_RADIO_TYPE_11G "radio_type_11g"
#define STRING_SHORT_PREAMBLE "short_preamble"
#define STRING_BSSID "bssid"
#define STRING_BSSIDS_NUMBER  "bssids_number"
#define STRING_DTIM_PERIOD "dtim_period"
#define STRING_BEACON_PERIOD "beacon_period"
#define STRING_COUNTRY_STRING "country"
#define STRING_CURRENT_TXPOWER "cur_txpower"
#define STRING_CURRENT_CHANNEL "cur_channel"
#define STRING_CURRENT_CCA "cur_cca"
#define STRING_ENERGY_DETECT_THRESHOLD "energy_detect_threshold"
#define STRING_CTRL_IPV4_ADDR "ctrl_ipv4_address"
#define STRING_AC_NAME "ac_name"
#define STRING_AP_TRANS_TYPE "ap_trans_type"
#define STRING_MAX_RADIOS "max_radios"
#define STRING_RADIO_INUSE "radio_inuse"
#define STRING_ENCRYPT_NUMBER "encrypt_number"
#define STRING_ENCRYPT_WBID "encrypt_wbid"
#define STRING_ENCRYPT_CAPABILITIES "encrypt_capabilities"
#define STRING_RESULT_CODE "result_code"
#define STRING_RADIO_STATE "radio_state"
#define STRING_RADIO_CAUSE "radio_cause"
#define STRING_RTS_THRESHOLD "rts_threshold"
#define STRING_SHORT_RETRY "short_retry"
#define STRING_FRAG_THR "frag_thr"
#define STRING_TX_MSDU_LIFETIME "tx_msdu_lifetime"
#define STRING_RX_MSDU_LIFETIME "rx_msdu_lifetime"
#define STRING_BAND_WIDTH "band_width"
#define STRING_TI_THRESHOLD "ti_threshold"

#define STRING_A_MSDU "amsdu"
#define STRING_A_MPDU "ampdu"
#define STRING_11N_ONLY "11n_only"
#define STRING_SHORT_GI "short_gi"
#define STRING_MAX_SUPPORT_MCS "max_support_mcs"
#define STRING_MAX_MANDANTORY_MCS "max_mandantory_mcs"
#define STRING_TX_ANTENNA "tx_antenna"
#define STRING_RX_ANTENNA "rx_antenna"
#define STRING_AUTO_CHANNEL_SWITCH "auto_channel_switch"
#define STRING_AUTO_CHANNEL_PERIOD "auto_channel_period"
#define STRING_AUTO_POWER_SWITCH "auto_power_switch"
#define STRING_ACK_TIMEOUT "ack_timeout"
#define STRING_BEACON_RATE "beacon_rate"
#define STRING_PROTECT_MODE "protect_mode"
#define STRING_PROBE_RSSI "probe_rssi"
#define STRING_MIN_CONTRACT_RATE "min_contract_rate"
#define STRING_ECHO_INTERVAL "echo_interval"
#define STRING_ECHO_TIMEOUT_COUNT "echo_timeout_cnt"
#define STRING_TRAFFIC_STATICS_INTERVAL "traffic_statics_interval"
#define STRING_TRAFFIC_SWITCH "traffic_switch"
#define STRING_NTP_SERVER_INTERVAL "ntp_server_interval"
#define STRING_NTP_SERVER "ntp_server"
#define STRING_MIN_POWER "min_power"
#define STRING_MIN_THROUGHPUT_THRESHOLD "min_throughput_threshold"
#define STRING_MIN_RATE_THRESHOLD "min_rate_threshold"
#define STRING_PPC_ENABLE "ppc_enable"
#define STRING_CCA_ADJ "cca_adj"
#define STRING_CCA_THR "cca_thr"
#define STRING_AP_SPACE "ap_space"
#define STRING_DATA_TYPE "data_type"
#define STRING_DATA_MODE "data_mode"
#define STRING_DATA_DATA "data"
#define STRING_REPORT_STATION_INFO_ENABLE "report_station_info_enable"
#define STRING_REPORT_STATION_INFO_INTERVAL "report_station_info_interval"
#define STRING_ROMING_CONFIG_ENABLE "roming_config_enable"
#define STRING_ROMING_CONFIG_SIGNAL "roming_config_signal"
#define STRING_WP_ENABLE "wp_enable"
#define STRING_WP_INTERVAL "wp_interval"
#define STRING_WP_SERVER_IP_TYPE "wp_server_ip_type"
#define STRING_WP_SERVER_IP_ADDR "wp_server_ip_addr"
#define STRING_WP_SERVER_PORT "wp_server_port"
#define STRING_WP_SCAN_TYPE "wp_scan_type"
#define STRING_WP_CODE "wp_code"
#define STRING_WP_PROTO "wp_proto"
#define STRING_EF_ENABLE "ef_enable"
#define STIRNG_EF_CODE "ef_code"
#define STIRNG_EF_PROTO "ef_proto"
#define STIRNG_EF_INTERVAL "ef_interval"
#define STIRNG_EF_SCAN_TYPE "ef_scan_type"
#define STIRNG_EF_SERVER_IP_TYPE "ef_server_ip_tyep"
#define STRING_EF_SERVER_IP_ADDR "ef_server_ip_addr"
#define STRING_EF_SERVER_PORT "ef_server_port"
#define STRING_WE_AD_INTERVAL "we_ad_interval"
#define STRING_WE_CHANNEL_2G "we_channel_2g"
#define STRING_WE_CHANNEL_5G "we_channel_5g"
#define STRING_WE_AD_RSSI "we_ad_rssi"
#define STRING_RFG_ENABLE "rfg_enable"
#define STRING_RFG_ASSOCMAX "rfg_assocmax"
#define STRING_RFG_TIMEOUT "rfg_timeout"
#define STRING_RFG_MAXSTA "rfg_maxsta"
#define STRING_RFG_METHOD "rfg_method"
#define STRING_AP_LOADBALANCE_ENABLE "ap_lb_enable"
#define STRING_AP_LOADBALANCE_THRESHOLD "ap_lb_threshold"
#define STRING_AP_LOADBALANCE_INTERVAL "ap_lb_interval"
#define STRING_RATE_SET_11A_LEN "rate_set_11a_len"
#define STRING_RATE_SET_11A_RATE "rate_set_11a_rate"
#define STRING_RATE_SET_11BG_LEN "rate_set_11bg_len"
#define STRING_RATE_SET_11BG_RATE "rate_set_11bg_rate"
#define STRING_RATE_SET_11N_LEN "rate_set_11n_len"
#define STRING_RATE_SET_11N_RATE "rate_set_11n_rate"
#define STRING_RATE_SET_11AC_LEN "rate_set_11ac_len"
#define STRING_RATE_SET_11AC_RATE "rate_set_11ac_rate"
#define STRING_LOW_RSSI_REFUSE_ENABLE "low_rssi_refuse_enable"
#define STRING_LOW_RSSI_THRESHOLD "low_rssi_threshold"
#define STRING_LAN_VLAN_ID "lan_vlan_id"
#define STRING_AUDIT_ENABLE "audit_enable"
#define STRING_APPRI_ENABLE "appri_enable"
#define STRING_REMOTE_SYNC_ENABLE "remote_sync_enable"
#define STRING_LAN_PORTAL_ENABLE "lan_portal_enable"
#define STRING_LAN_PORTAL_URL "lan_portal_url"


#define STRING_PORTAL_CUSTOM_COUNT "portal_custom_count"
#define STRING_PORTAL_CUSTOM_KEY_LEN "portal_custom_key_len"
#define STRING_PORTAL_CUSTOM_KEY "portal_custom_key"
#define STRING_PORTAL_CUSTOM_ALIAS_LEN "portal_custom_alias_len"
#define STRING_PORTAL_CUSTOM_ALIAS "portal_custom_alias"
#define STRING_PORTAL_CUSTOM_VALUE_LEN "portal_custom_value_len"
#define STRING_PORTAL_CUSTOM_VALUE "portal_custom_value"
#define STRING_TIME_STAMP "time_stamp"
#define STRING_BY_PASS_ENABLE "by_pass_enable"

string toString(uint32_t value);
string toString(int value);
string toString(time_t value);
string toString(char *value);
string toString(uint8_t value);
string toString(uint16_t value);
uint8_t  toInt8(string value);
uint16_t toInt16(string value);
uint32_t toInt32(string value);
uint32_t toInt(string value);
uint64_t toInt64(string value);
vector<string> split(string &str, string d=";");
int format_macaddr(string &mac);

#endif
