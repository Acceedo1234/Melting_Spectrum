/*
 * w5500_spi.c
 *
 *  Created on: Jan 10, 2023
 *      Author: MKS
 */

#include "main.h"
#include "wizchip_conf.h"
#include "stdio.h"
#include "socket.h"
#include "dhcp.h"
#include "dns.h"
#include <stdio.h>
#include "httpClient.h"
#include "Struct_Input_output.h"



#define SOCK_DHCP               3
#define SOCK_DNS                4
#define DATA_BUF_SIZE	2048

wiz_NetInfo gWIZNETINFO = { .mac = {0x00, 0x03, 0x19,0x45, 0x00, 0x02},
                            .ip = {192, 168, 0, 4},
                            .sn = {255,255,255,0},
                            .gw = {192, 168, 0, 1},
                            .dns = {0,0,0,0},
                            .dhcp = NETINFO_STATIC };

wiz_NetInfo checkgWIZNETINFO;

#define DHCP_SOCKET 0
#define DNS_SOCKET 1
#define HTTP_SOCKET 2

uint8_t flag_sent_http_request = DISABLE;
uint8_t connectionCheck= HTTPC_FALSE;

uint8_t g_send_buf[DATA_BUF_SIZE];
uint8_t g_recv_buf[DATA_BUF_SIZE];

uint8_t data_buf [DATA_BUF_SIZE];
// 1K should be enough, see https://forum.wiznet.io/t/topic/1612/2
uint8_t dhcp_buffer [ 1024 ] ;
// 1K seems to be enough for this buffer as well
uint8_t dns_buffer [ 1024 ] ;

uint8_t Domain_IP[4]  = {52,184,16,136};
//uint8_t Domain_IP[4]  = {0,0,0,0};

uint8_t Domain_name[] = "isc1-melt.acceedo.in";//
//uint8_t URI[] = "http://usm3-ht.acceedo.in:9005/set_temp?u=5&p=00000001&tm=0030&tl=0320&th=0030&h=030&l=080&ht=070&lt=080&rv=0130&bv=0650&yv=1230&rc=2705&bc=2909&yc=0500&k=203040&x=1";
//uint8_t URI[] = "/search?ei=BkGsXL63AZiB-QaJ8KqoAQ&q=W6100&oq=W6100&gs_l=psy-ab.3...0.0..6208...0.0..0.0.0.......0......gws-wiz.eWEWFN8TORw";
uint8_t URI[550];
extern SPI_HandleTypeDef hspi2;
#define _W5500_SPI       hspi2
uint16_t wiz5500_var;
uint8_t statusChipInit,statusPhysLink;
uint8_t retDHCPstatus;

/*Client to server exchange*/
uint8_t SelectFurnace=4;
uint8_t loc_spectrumresult[550];
uint8_t st;
extern uint8_t Ip_config_Ip[4],Ip_Config_Subnet[4],Ip_config_gateway[4],Ip_config_DNS[4],Ip_config_Server[4];

extern SpectrumResult SpectrumResult_furnace[5];

void processDHCP(void);

void ethernetHTTPRoutine(void);

void initializeHttp(void);

void wizchip_select(void)
{
	HAL_GPIO_WritePin(GPIOB, W5500_CS_Pin, GPIO_PIN_RESET);
}

void wizchip_deselect(void)
{
	HAL_GPIO_WritePin(GPIOB, W5500_CS_Pin, GPIO_PIN_SET);
}

void W5500_ReadBuff(uint8_t* buff,uint16_t len​​)
{
    HAL_SPI_Receive(&_W5500_SPI,buff,len​​,HAL_MAX_DELAY) ;
}

void W5500_WriteBuff(uint8_t* buff,uint16_t len)
{
    HAL_SPI_Transmit(&_W5500_SPI,buff,len,HAL_MAX_DELAY) ;
}

uint8_t W5500_Spi(uint8_t Data)
{
	uint8_t ret;
	HAL_SPI_TransmitReceive(&_W5500_SPI, &Data, &ret, 1, 100);
	return ret;
}
/*
void spiWriteByte(uint8_t tx)
{
	uint8_t rx;
	HAL_SPI_TransmitReceive(&_W5500_SPI, &tx, &rx, 1, 10);
}

uint8_t spiReadByte(void)
{
	uint8_t rx = 0, tx = 0xFF;
	HAL_SPI_TransmitReceive(&_W5500_SPI, &tx, &rx, 1, 10);
	return rx;
}*/
/*
uint8_t W5500_ReadByte ( void ) {
    uint8_t byte ;
    W5500_ReadBuff (& byte , sizeof ( byte ) ) ;
    return byte ;
}

void W5500_WriteByte ( uint8_t byte ) {
    W5500_WriteBuff ( & byte , sizeof ( byte ) ) ;
}
*/
volatile uint8_t ip_assigned = 0;

void Callback_IPAssigned(void) {
    ip_assigned = 1;
}

void Callback_IPConflict(void) {

}
/*UDP
void wiz5500Init(void)
{

	//hardware reset the module
	HAL_GPIO_WritePin(W5500RST_GPIO_Port, W5500RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(15000);
	HAL_GPIO_WritePin(W5500RST_GPIO_Port, W5500RST_Pin, GPIO_PIN_SET);

	reg_wizchip_cs_cbfunc(wizchip_select,wizchip_deselect);
	reg_wizchip_spi_cbfunc(W5500_Spi,W5500_Spi);
	reg_wizchip_spiburst_cbfunc(W5500_ReadBuff,W5500_WriteBuff);
	wiz5500_var = 0;
	uint8_t rx_tx_buff_sizes [ ] = { 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 } ;
	wizchip_init ( rx_tx_buff_sizes , rx_tx_buff_sizes ) ;


	wiz_NetInfo net_info = {
	    . mac  = { 0xEA , 0x11 , 0x22 , 0x33 , 0x44 , 0xEA } ,
	    . dhcp = NETINFO_DHCP
	} ;
	// set MAC address before using DHCP
	setSHAR ( net_info. mac ) ;
	DHCP_init ( DHCP_SOCKET , dhcp_buffer ) ;

	reg_dhcp_cbfunc (
	    Callback_IPAssigned ,
	    Callback_IPAssigned ,
	    Callback_IPConflict
	) ;

	uint32_t ctr = 10000 ;
	while ( ( ! ip_assigned ) && ( ctr > 0 ) ) {
	    DHCP_run ( ) ;
	    ctr --;
	}
	if ( ! ip_assigned ) {
	    return ;
	}

	getIPfromDHCP(net_info.ip);
	getGWfromDHCP(net_info.gw);
	getSNfromDHCP(net_info.sn);
	uint8_t dns[4];
	getDNSfromDHCP(dns);

	wizchip_setnetinfo ( & net_info ) ;
}*/


void wiz5500Init(void)
{

	uint8_t memsize[2][8] = { {2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};
	uint8_t tmp;
	//hardware reset the module
	HAL_GPIO_WritePin(W5500RST_GPIO_Port, W5500RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(15000);
	HAL_GPIO_WritePin(W5500RST_GPIO_Port, W5500RST_Pin, GPIO_PIN_SET);

	reg_wizchip_cs_cbfunc(wizchip_select,wizchip_deselect);
	reg_wizchip_spi_cbfunc(W5500_Spi,W5500_Spi);
//	reg_wizchip_spiburst_cbfunc(W5500_ReadBuff,W5500_WriteBuff);

	if(ctlwizchip(CW_INIT_WIZCHIP,(void*)memsize) == -1)
	{
		statusChipInit=1;
		return;
	}
	/* PHY link status check */
	do
	{
		if(ctlwizchip(CW_GET_PHYSTATUS, (void*)&tmp) == -1)
		{
			statusPhysLink=1;
			return;
		}
		statusPhysLink=0;
	} while (tmp == PHY_LINK_OFF);
	HAL_Delay(3000);
	//getVERSIONR();
	gWIZNETINFO.ip[0]=Ip_config_Ip[0];
	gWIZNETINFO.ip[1]=Ip_config_Ip[1];
	gWIZNETINFO.ip[2]=Ip_config_Ip[2];
	gWIZNETINFO.ip[3]=Ip_config_Ip[3];

	gWIZNETINFO.sn[0]=Ip_Config_Subnet[0];
	gWIZNETINFO.sn[1]=Ip_Config_Subnet[1];
	gWIZNETINFO.sn[2]=Ip_Config_Subnet[2];
	gWIZNETINFO.sn[3]=Ip_Config_Subnet[3];

	gWIZNETINFO.gw[0]=Ip_config_gateway[0];
	gWIZNETINFO.gw[1]=Ip_config_gateway[1];
	gWIZNETINFO.gw[2]=Ip_config_gateway[2];
	gWIZNETINFO.gw[3]=Ip_config_gateway[3];

	wizchip_setnetinfo(&gWIZNETINFO);
	HAL_Delay(1000);


	//processDHCP();

	wizchip_getnetinfo(&checkgWIZNETINFO);

	initializeHttp();

}

void processDHCP(void)
{
	uint8_t dhcp_retry;
	//DHCP Client running
	DHCP_init(SOCK_DHCP, data_buf);
	while(1)
	{
		retDHCPstatus = DHCP_run();
		if(retDHCPstatus == DHCP_IP_LEASED)
		{
			break;
		}
		else if(retDHCPstatus == DHCP_FAILED)
		{
			dhcp_retry++;
			if(dhcp_retry <= 3) {

			}
		}

		if(dhcp_retry > 3)
		{
			DHCP_stop();
			break;
		}
	}
}

void initializeHttp(void)
{
	Domain_IP[0] = Ip_config_Server[0];
	Domain_IP[1] = Ip_config_Server[1];
	Domain_IP[2] = Ip_config_Server[2];
	Domain_IP[3] = Ip_config_Server[3];

 	httpc_init(0, Domain_IP, 9006, g_send_buf, g_recv_buf);
}

void ethernetHTTPRoutine(void)
{

	receivehttpcheck();
	httpc_connection_handler();

	if(httpc_isSockOpen)
	{
		connectionCheck = httpc_connect();
	}
	if(httpc_isConnected)
	{
	//	if(!flag_sent_http_request)
		{
		// Send: HTTP request
		switch(st)
		{
			case 0:
				SelectFurnace=1;
				st=1;
			break;
			case 1://dummy furnace
				SelectFurnace=5;
				st=2;
			break;
			case 2:
				SelectFurnace=2;
				st=3;
			break;
			case 3://dummy furnace
				SelectFurnace=5;
				st=4;
			break;
			case 4:
				SelectFurnace=3;
				st=5;
			break;
			case 5://dummy furnace
				SelectFurnace=5;
				st=6;
			break;
			case 6:
				SelectFurnace=4;
				st=7;
			break;
			case 7:
				SelectFurnace=5;
				st=0;
			break;
			default:
			break;
		}
		//SelectFurnace=4;
		if(SelectFurnace == 10)
		{
			memset(loc_spectrumresult, 0, 550);
			memcpy(loc_spectrumresult,&SpectrumResult_furnace[SelectFurnace-1].result_spectrum[0],500);
			sprintf(URI,"http://isc1-melt.acceedo.in:9006/spectrumResult?d=%.*s",418,loc_spectrumresult);
		}
		else
		{
			memset(loc_spectrumresult, 0, 550);
			memcpy(loc_spectrumresult,&SpectrumResult_furnace[SelectFurnace-1].result_spectrum[0],500);
			sprintf(URI,"http://isc1-melt.acceedo.in:9006/spectrumResult?d=%.*s",418,loc_spectrumresult);
		}

		request.method = (uint8_t *)HTTP_GET;
		request.uri = (uint8_t *)URI;
		request.host = (uint8_t *)Domain_name;
	    request.connection=(uint8_t *)HTTP_CONNECTION_CLOSE;
		request.content_type = (uint8_t *)HTTP_CTYPE_TEXT_HTML;

		// HTTP client example #1: Function for send HTTP request (header and body fields are integrated)
			{
			httpc_send(&request, g_recv_buf, g_send_buf, 0);
			}
			flag_sent_http_request = ENABLE;
		}

		// Recv: HTTP response

	}

}

