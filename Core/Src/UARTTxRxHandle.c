/*
 * UARTTxRxHandle.c
 *
 *  Created on: Jan 21, 2023
 *      Author: MKS
 */
#include <aco_generic_enumeration.h>
#include"main.h"
#include <string.h>

extern UART_HandleTypeDef hlpuart1;
extern UART_HandleTypeDef huart1;
uint8_t rxTempBuff[5];
uint8_t Rx_Dwin_Buff[3];
uint8_t Rx_Dwin_Point;
uint8_t Rx_Dwin_Data_Buff_Point;
uint8_t Rx_Dwin_Complete;
uint8_t No_Of_Dwin_Bytes;
uint8_t Rx_Dwin_Data_Buff[50];

uint8_t checkbuff[500];
uint8_t raw_furnace_result[500];
uint16_t reference;
uint8_t startrxframe;
uint8_t SuccessfulResultRx;



extern uint8_t Rxseqdecoder;
extern void ESPRxDecoder(unsigned char Rxwifi_data,unsigned char Rxseqdecoder);

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &hlpuart1)
	{
		if((Rx_Dwin_Buff[0] ==0x83)&&(Rx_Dwin_Point==0))
		{
			Rx_Dwin_Point=1;
		}
		else if(Rx_Dwin_Point==1)
		{
			if(Rx_Dwin_Buff[0] == 0x30){
				Rx_Dwin_Point=2;
			}
			else{
				Rx_Dwin_Point=0;
			}
		}
		else if((Rx_Dwin_Buff[0] == 0x00)&&(Rx_Dwin_Point==2))
		{
			Rx_Dwin_Point=3;
		}
		else if(Rx_Dwin_Point==3)
		{
			Rx_Dwin_Point=4;
			No_Of_Dwin_Bytes = Rx_Dwin_Buff[0]*2;
			Rx_Dwin_Data_Buff_Point = 0;
		}
		else if(Rx_Dwin_Point==4)
		{
			//Rx_Dwin_Point=4;
			Rx_Dwin_Data_Buff[Rx_Dwin_Data_Buff_Point]= Rx_Dwin_Buff[0];
			No_Of_Dwin_Bytes = No_Of_Dwin_Bytes-1;
			Rx_Dwin_Data_Buff_Point = Rx_Dwin_Data_Buff_Point+1;
			if(No_Of_Dwin_Bytes==0){
				Rx_Dwin_Complete = 1;
				Rx_Dwin_Point=0;
			}
		}
			HAL_UART_Receive_IT(&hlpuart1,Rx_Dwin_Buff,1);
	}

	if(huart == &huart1)
	{
		HAL_UART_Receive_IT(&huart1,rxTempBuff,1);
		if((rxTempBuff[0] == '$')&&(startrxframe==0))
		{
			startrxframe=1;
			reference=0;
		}
		if(startrxframe==1)
		{
			checkbuff[reference]= rxTempBuff[0];
			reference++;
			if(rxTempBuff[0] == '#')
			{
				startrxframe=0;
				memcpy(raw_furnace_result,checkbuff,500);
				memset(checkbuff,0,sizeof(checkbuff));
				SuccessfulResultRx = 1;
			}
		}

	}
}



