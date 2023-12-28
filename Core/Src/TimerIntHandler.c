/*
 * UARTTxRxHandle.c
 *
 *  Created on: Jan 21, 2023
 *      Author: MKS
 */
 #include"main.h"
#include "dhcp.h"
 
 extern TIM_HandleTypeDef htim7;
 extern TIM_HandleTypeDef htim6;
 uint8_t Flag1MS;
 uint8_t Flag1Second;
 uint8_t Flag2Second;
 uint8_t Flag100milliSeconds;
 uint8_t SimCount;
 uint8_t LocCount1S;
 
 extern uint16_t seq1_duration_total,seq2_duration_total;
 extern uint8_t start_process_control_timer;
 extern uint8_t stop_timer_count;
 extern uint8_t process_complete;
 
uint16_t seq1_count_inc,seq2_count_inc;
uint16_t seq1_remaining_time_total_min,seq2_remaining_time_total_min;
uint8_t seq1_remaining_time_Hr,seq1_remaining_time_min,seq2_remaining_time_Hr,seq2_remaining_time_min;
uint8_t Two_Second_Count;

 void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
 {
 	if(htim == &htim7)
 	{
 		Flag1MS=1;
 	}
 	if(htim == &htim6)
	{
 		Flag100milliSeconds=1;
 		if(++LocCount1S >= 10)
		{
			LocCount1S=0;
			if(++SimCount >= 20)
			{
				SimCount=0;
				HAL_GPIO_TogglePin(GPIOC, RELAY4_Pin);
			}
			Flag1Second =1;
			if(++Two_Second_Count >1){
				Two_Second_Count=0;
				Flag2Second=1;
			}
			//DHCP_time_handler( );

		}


	}
 
 }
