/*
 * Modbusrtu.cpp
 *
 *  Created on: 07-Jan-2023
 *      Author: MKS
 */

#include <aco_generic_enumeration.h>
#include "Modbusrtu.h"
#include "Modbus_types.h"

/*DWIN hmi uart data*/

constexpr uint8_t START_BYTE_1=0x5A;
constexpr uint8_t START_BYTE_2=0xA5;
constexpr uint8_t multipleWriteRequestH = 0x0B;
constexpr uint8_t multipleWriteRequestL = 0x82;
constexpr uint8_t multipleReadRequestH = 0x04;
constexpr uint8_t multipleReadRequestL = 0x83;

extern uint8_t Rx_Dwin_Point;
extern uint8_t Rx_Dwin_Data_Buff[50];
extern uint8_t Rx_Dwin_Complete;

extern uint8_t httpc_isConnected;

uint8_t test;
uint8_t testarray[50];
uint8_t No_Of_Furnace;

uint8_t Ip_config_Ip[4],Ip_Config_Subnet[4],Ip_config_gateway[4],Ip_config_DNS[4],Ip_config_Server[4];
uint8_t Update_Dwin_Set_Data;
uint16_t Ip_config_Server_Port;
uint16_t Ip_config_Server_Port_K1;
uint8_t No_Of_Meter_K1,No_Of_Meter;

uint8_t TxSeqComplete;
Modbusrtu::Modbusrtu() {
	// TODO Auto-generated constructor stub

}

Modbusrtu::~Modbusrtu() {
	// TODO Auto-generated destructor stub
}

void Modbusrtu::dwinFrame(void)
{
	test=test+1;
	switch(Cntid)
	{
	case 0:
		u8ModbusRegister[0] = START_BYTE_1;
		u8ModbusRegister[1] = START_BYTE_2;
		u8ModbusRegister[2] = multipleWriteRequestH;
		u8ModbusRegister[3] = multipleWriteRequestL;
		u8ModbusRegister[4] = 0x20;
		u8ModbusRegister[5] = 0x00;
		u8ModbusRegister[6] = 0x00;
		u8ModbusRegister[7] = test;
		u8ModbusRegister[8] = 0;
		u8ModbusRegister[9] = httpc_isConnected;
		u8ModbusRegister[10] = 0;
		u8ModbusRegister[11] = 1;
		noOfData=12;
		Cntid=1;
	break;
	case 1:
			u8ModbusRegister[0] = START_BYTE_1;
			u8ModbusRegister[1] = START_BYTE_2;
			u8ModbusRegister[2] = multipleWriteRequestH;
			u8ModbusRegister[3] = multipleWriteRequestL;
			u8ModbusRegister[4] = 0x20;
			u8ModbusRegister[5] = 0x00;
			u8ModbusRegister[6] = 0x00;
			u8ModbusRegister[7] = test;
			u8ModbusRegister[8] = 0;
			u8ModbusRegister[9] = httpc_isConnected;
			u8ModbusRegister[10] = 0;
			u8ModbusRegister[11] = 1;
			noOfData=12;
			Cntid=2;
	break;
	case 2:
		u8ModbusRegister[0] = START_BYTE_1;
		u8ModbusRegister[1] = START_BYTE_2;
		u8ModbusRegister[2] = multipleReadRequestH;
		u8ModbusRegister[3] = multipleReadRequestL;
		u8ModbusRegister[4] = 0x30;
		u8ModbusRegister[5] = 0x00;
		u8ModbusRegister[6] = 0x16;
		//HAL_UART_Receive_IT(&hlpuart1,testarray,25);
		Rx_Dwin_Point=0;
		noOfData=7;
		Cntid=0;
	break;
	default:
		Cntid=0;
	break;
	}
	//out_read_rxint_set.Noofbytesrx = (_u16ReadQty*2)+5;
	HAL_UART_Transmit_IT(&hlpuart1,u8ModbusRegister,noOfData);

}

void Modbusrtu::dwinDecoder(void)
{
	if(Rx_Dwin_Complete == 0){return;}
	Rx_Dwin_Complete=0;

	if(Rx_Dwin_Data_Buff[43] !=0 )
	{
		for(d=0,x=1;d<=3;d++,x=x+2)
		{
			if(Ip_config_Ip[d]!=Rx_Dwin_Data_Buff[x])
			{
				Update_Dwin_Set_Data =1;
			}
		}
		Ip_config_Ip[0] = Rx_Dwin_Data_Buff[1];
		Ip_config_Ip[1] = Rx_Dwin_Data_Buff[3];
		Ip_config_Ip[2] = Rx_Dwin_Data_Buff[5];
		Ip_config_Ip[3] = Rx_Dwin_Data_Buff[7];
		for(d=0,x=9;d<=3;d++,x=x+2)
		{
			if(Ip_Config_Subnet[d]!=Rx_Dwin_Data_Buff[x])
			{
				Update_Dwin_Set_Data =1;
			}
		}
		Ip_Config_Subnet[0] = Rx_Dwin_Data_Buff[9];
		Ip_Config_Subnet[1] = Rx_Dwin_Data_Buff[11];
		Ip_Config_Subnet[2] = Rx_Dwin_Data_Buff[13];
		Ip_Config_Subnet[3] = Rx_Dwin_Data_Buff[15];
		for(d=0,x=17;d<=3;d++,x=x+2)
		{
			if(Ip_config_gateway[d]!=Rx_Dwin_Data_Buff[x])
			{
				Update_Dwin_Set_Data =1;
			}
		}
		Ip_config_gateway[0] = Rx_Dwin_Data_Buff[17];
		Ip_config_gateway[1] = Rx_Dwin_Data_Buff[19];
		Ip_config_gateway[2] = Rx_Dwin_Data_Buff[21];
		Ip_config_gateway[3] = Rx_Dwin_Data_Buff[23];
		for(d=0,x=25;d<=3;d++,x=x+2)
		{
			if(Ip_config_DNS[d]!=Rx_Dwin_Data_Buff[x])
			{
				Update_Dwin_Set_Data =1;
			}
		}
		Ip_config_DNS[0] = Rx_Dwin_Data_Buff[25];
		Ip_config_DNS[1] = Rx_Dwin_Data_Buff[27];
		Ip_config_DNS[2] = Rx_Dwin_Data_Buff[29];
		Ip_config_DNS[3] = Rx_Dwin_Data_Buff[31];
		for(d=0,x=33;d<=3;d++,x=x+2)
		{
			if(Ip_config_Server[d]!=Rx_Dwin_Data_Buff[x])
			{
				Update_Dwin_Set_Data =1;
			}
		}
		Ip_config_Server[0] = Rx_Dwin_Data_Buff[33];
		Ip_config_Server[1] = Rx_Dwin_Data_Buff[35];
		Ip_config_Server[2] = Rx_Dwin_Data_Buff[37];
		Ip_config_Server[3] = Rx_Dwin_Data_Buff[39];
		Ip_config_Server_Port_K1 = ((Rx_Dwin_Data_Buff[40]<<8)|(Rx_Dwin_Data_Buff[41]));
		if(Ip_config_Server_Port != Ip_config_Server_Port_K1)
		{
			Ip_config_Server_Port = Ip_config_Server_Port_K1;
			Update_Dwin_Set_Data =1;
		}
		No_Of_Meter_K1 		= Rx_Dwin_Data_Buff[43];
		if(No_Of_Meter != No_Of_Meter_K1)
		{
			No_Of_Meter 		= No_Of_Meter_K1;
			Update_Dwin_Set_Data =1;
		}
	}
}
//Hardware callback
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	//HAL_UART_Receive_IT(&huart1,u8rxbuf,9);
}
