/*
 ETH.h - espre ETH PHY support.
 Based on WiFi.h from Arduino WiFi shield library.
 Copyright (c) 2011-2014 Arduino.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "ETH.h"
#include "eth_phy/phy.h"
#include "eth_phy/phy_tlk110.h"
#include "eth_phy/phy_lan8720.h"
#include "eth_phy/phy_ip101.h"
#include "lwip/err.h"
#include "lwip/dns.h"

extern void tcpipInit();

static int _eth_phy_mdc_pin = -1;
static int _eth_phy_mdio_pin = -1;
static int _eth_phy_power_pin = -1;
static eth_phy_power_enable_func _eth_phy_power_enable_orig = NULL;

static void _eth_phy_config_gpio(void)
{
    if(_eth_phy_mdc_pin < 0 || _eth_phy_mdio_pin < 0){
        log_e("MDC and MDIO pins are not configured!");
        return;
    }
    phy_rmii_configure_data_interface_pins();
    phy_rmii_smi_configure_pins(_eth_phy_mdc_pin, _eth_phy_mdio_pin);
}

static void _eth_phy_power_enable(bool enable)
{
    pinMode(_eth_phy_power_pin, OUTPUT);
    digitalWrite(_eth_phy_power_pin, enable);
    delay(1);
}

ETHClass::ETHClass():initialized(false),started(false),staticIP(false)
{
}

ETHClass::~ETHClass()
{}

bool ETHClass::begin(uint8_t phy_addr, int power, int mdc, int mdio, eth_phy_type_t type, eth_clock_mode_t clock_mode)
{
    esp_err_t err;
    if(initialized){
        err = esp_eth_enable();
        if(err){
            log_e("esp_eth_enable error: %d", err);
            return false;
        }
        started = true;
        return true;
    }
    _eth_phy_mdc_pin = mdc;
    _eth_phy_mdio_pin = mdio;
    _eth_phy_power_pin = power;

    if(type == ETH_PHY_LAN8720)
	{
        eth_config_t config = phy_lan8720_default_ethernet_config;
        memcpy(&eth_config, &config, sizeof(eth_config_t));
    }
	else if(type == ETH_PHY_TLK110)
	{
        eth_config_t config = phy_tlk110_default_ethernet_config;
        memcpy(&eth_config, &config, sizeof(eth_config_t));
    }
	else if(type == ETH_PHY_IP101)
	{
       eth_config_t config = phy_ip101_default_ethernet_config;
       memcpy(&eth_config, &config, sizeof(eth_config_t));
    }
	else
	{
        log_e("Bad ETH_PHY type: %u", (uint8_t)type);
        return false;
    }

    eth_config.phy_addr = (eth_phy_base_t)phy_addr;
    eth_config.clock_mode = clock_mode;
    eth_config.gpio_config = _eth_phy_config_gpio;
    eth_config.tcpip_input = tcpip_adapter_eth_input;
    if(_eth_phy_power_pin >= 0){
        _eth_phy_power_enable_orig = eth_config.phy_power_enable;
        eth_config.phy_power_enable = _eth_phy_power_enable;
    }

    tcpipInit();
    err = esp_eth_init(&eth_config);
    if(!err){
        initialized = true;
        err = esp_eth_enable();
        if(err){
            log_e("esp_eth_enable error: %d", err);
        } else {
            started = true;
            return true;
        }
    } else {
        log_e("esp_eth_init error: %d", err);
    }
    return false;
}

bool ETHClass::config(IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dns1, IPAddress dns2)
{
    esp_err_t err = ESP_OK;
    tcpip_adapter_ip_info_t info;

    if(local_ip != (uint32_t)0x00000000){
        info.ip.addr = static_cast<uint32_t>(local_ip);
        info.gw.addr = static_cast<uint32_t>(gateway);
        info.netmask.addr = static_cast<uint32_t>(subnet);
    } else {
        info.ip.addr = 0;
        info.gw.addr = 0;
        info.netmask.addr = 0;
	}

    err = tcpip_adapter_dhcpc_stop(TCPIP_ADAPTER_IF_ETH);
    if(err != ESP_OK && err != ESP_ERR_TCPIP_ADAPTER_DHCP_ALREADY_STOPPED){
        log_e("DHCP could not be stopped! Error: %d", err);
        return false;
    }

    err = tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_ETH, &info);
    if(err != ERR_OK){
        log_e("STA IP could not be configured! Error: %d", err);
        return false;
}
    if(info.ip.addr){
        staticIP = true;
    } else {
        err = tcpip_adapter_dhcpc_start(TCPIP_ADAPTER_IF_ETH);
        if(err != ESP_OK && err != ESP_ERR_TCPIP_ADAPTER_DHCP_ALREADY_STARTED){
            log_w("DHCP could not be started! Error: %d", err);
            return false;
        }
        staticIP = false;
    }

    ip_addr_t d;
    d.type = IPADDR_TYPE_V4;

    if(dns1 != (uint32_t)0x00000000) {
        // Set DNS1-Server
        d.u_addr.ip4.addr = static_cast<uint32_t>(dns1);
        dns_setserver(0, &d);
    }

    if(dns2 != (uint32_t)0x00000000) {
        // Set DNS2-Server
        d.u_addr.ip4.addr = static_cast<uint32_t>(dns2);
        dns_setserver(1, &d);
    }

    return true;
}

IPAddress ETHClass::localIP()
{
    tcpip_adapter_ip_info_t ip;
    if(tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_ETH, &ip)){
        return IPAddress();
    }
    return IPAddress(ip.ip.addr);
}

IPAddress ETHClass::subnetMask()
{
    tcpip_adapter_ip_info_t ip;
    if(tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_ETH, &ip)){
        return IPAddress();
    }
    return IPAddress(ip.netmask.addr);
}

IPAddress ETHClass::gatewayIP()
{
    tcpip_adapter_ip_info_t ip;
    if(tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_ETH, &ip)){
        return IPAddress();
    }
    return IPAddress(ip.gw.addr);
}

IPAddress ETHClass::dnsIP(uint8_t dns_no)
{
#ifdef ESP_IDF_VERSION_VAL //idf_version.h header file is only available in ESP-IDF v4.0 -> this is equal to: #if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 0, 0)
    const ip_addr_t *dns_ip = dns_getserver(dns_no);
    return IPAddress(dns_ip->u_addr.ip4.addr);
#else
    ip_addr_t dns_ip = dns_getserver(dns_no);
    return IPAddress(dns_ip.u_addr.ip4.addr);
#endif
}

IPAddress ETHClass::broadcastIP()
{
    tcpip_adapter_ip_info_t ip;
    if(tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_ETH, &ip)){
        return IPAddress();
    }
    return WiFiGenericClass::calculateBroadcast(IPAddress(ip.gw.addr), IPAddress(ip.netmask.addr));
}

IPAddress ETHClass::networkID()
{
    tcpip_adapter_ip_info_t ip;
    if(tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_ETH, &ip)){
        return IPAddress();
    }
    return WiFiGenericClass::calculateNetworkID(IPAddress(ip.gw.addr), IPAddress(ip.netmask.addr));
}

uint8_t ETHClass::subnetCIDR()
{
    tcpip_adapter_ip_info_t ip;
    if(tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_ETH, &ip)){
        return (uint8_t)0;
    }
    return WiFiGenericClass::calculateSubnetCIDR(IPAddress(ip.netmask.addr));
}

const char * ETHClass::getHostname()
{
    const char * hostname;
    if(tcpip_adapter_get_hostname(TCPIP_ADAPTER_IF_ETH, &hostname)){
        return NULL;
    }
    return hostname;
}

bool ETHClass::setHostname(const char * hostname)
{
    return tcpip_adapter_set_hostname(TCPIP_ADAPTER_IF_ETH, hostname) == 0;
}

bool ETHClass::fullDuplex()
{
    return eth_config.phy_get_duplex_mode();
}

bool ETHClass::linkUp()
{
    return eth_config.phy_check_link();
}

uint8_t ETHClass::linkSpeed()
{
    return eth_config.phy_get_speed_mode()?100:10;
}

bool ETHClass::enableIpV6()
{
    return tcpip_adapter_create_ip6_linklocal(TCPIP_ADAPTER_IF_ETH) == 0;
}

IPv6Address ETHClass::localIPv6()
{
    static ip6_addr_t addr;
    if(tcpip_adapter_get_ip6_linklocal(TCPIP_ADAPTER_IF_ETH, &addr)){
        return IPv6Address();
    }
    return IPv6Address(addr.addr);
}

uint8_t * macAddress(uint8_t* mac)
{
    if(!mac){
        return NULL;
    }
    esp_eth_get_mac(mac);
    return mac;
}

String ETHClass::macAddress(void)
{
    uint8_t mac[6];
    char macStr[18] = { 0 };
    esp_eth_get_mac(mac);
    sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return String(macStr);
}

ETHClass ETH;
