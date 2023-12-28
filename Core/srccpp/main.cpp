#include <BussinessLogic/ApplicationSW.h>
#include "main.h"
#include "GLCD.h"
#include "Modbusrtu.h"
#include "DisplayRoutine.h"
#include "OfflineStorage.h"
#include "ESP8266.h"
#include "SHIFT.h"
#include <stdio.h>
#include <string.h>
using namespace std;

extern "C" {
void Send_WifiCmd(void);
void W25qxx_EraseChip(void);
void ESPRXDataSeg(void);
void ReadOnlineData(void);
void ethernetHTTPRoutine(void);
void wiz5500Init(void);
void initializeHttp(void);
}

extern uint8_t TxSeqComplete;
extern uint8_t Flag1Second;
extern uint8_t Flag1MS;
extern uint8_t Flag2Second;
extern uint8_t Flag100milliSeconds;



/**
  * @brief  The application entry point for cpp
  * @retval int
  */
//TIM_HandleTypeDef htim6;
void cppMain()
{
	GLCD glcd;
	DisplayRoutine displayRoutineInst;
	BusinessLogic businessLogicInst;
	OfflineStorage offlineStorageInst;
	Modbusrtu dwininst;

//	ESP8266 esp8266Inst;
//	SHIFT shiftInst;
	offlineStorageInst.ReadOfflinedataInit();
	displayRoutineInst.Init();

	wiz5500Init();
//	esp8266Inst.Init();
//	ReadOnlineData();
	TxSeqComplete=1;


	while(1)
	{

		if(Flag1MS)
		{
			Flag1MS=0;
			businessLogicInst.run();
			offlineStorageInst.run();
		}
		if(Flag1Second)
		{
			Flag1Second=0;
			displayRoutineInst.run();
			ethernetHTTPRoutine();
			dwininst.dwinFrame();
			dwininst.dwinDecoder();
		}
		if(Flag2Second)
		{
			Flag2Second=0;

		}
	}
}
