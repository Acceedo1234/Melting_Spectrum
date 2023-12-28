
/*
 * LpdcLogic.cpp
 *
 *  Created on: 04-Feb-2023
 *      Author: MKS
 */
extern "C"{
#include "Struct_Input_output.h"
#include <string.h>
}
#include <BussinessLogic/ApplicationSW.h>
#include "main.h"
#include "Struct_Input_output.h"


GPIO_PinState GPIO_PinStateMac;
GPIO_PinState GPIO_PinStateRej;


/*Data shared btw server & client*/
//Client -server
uint8_t status_to_server;
uint16_t ProcessId_Value;
//server - client
extern uint8_t skipIdTCP;
extern uint8_t statusTCP;
extern uint16_t seq1_count_inc,seq2_count_inc;

/**Spectrum result*/
//Global share
extern uint8_t SuccessfulResultRx;
extern uint8_t raw_furnace_result[500];
uint8_t *ptr;
uint16_t track_furnaceno;
SpectrumResult SpectrumResult_furnace[4];
uint8_t furnaceno;
uint8_t Trigger_Memory_Update,Trigger_Memory_Furnaceno;
uint8_t changeInFrame;


BusinessLogic::BusinessLogic() {
	// TODO Auto-generated constructor stub

}

BusinessLogic::~BusinessLogic() {
	// TODO Auto-generated destructor stub
};

void BusinessLogic::run()
{
	if(SuccessfulResultRx != 1){return;}
	SuccessfulResultRx=0;

	track_furnaceno=0;

	while(raw_furnace_result[track_furnaceno] != '#')
	{
		track_furnaceno = track_furnaceno+1;
	}
	furnaceno = raw_furnace_result[track_furnaceno-1]-48;

//	changeInFrame = memcmp(&SpectrumResult_furnace[furnaceno-1].result_spectrum[0],raw_furnace_result,418);
	memcpy(&SpectrumResult_furnace[furnaceno-1].result_spectrum[0],raw_furnace_result,500);
	Trigger_Memory_Update = 1;
	Trigger_Memory_Furnaceno = furnaceno-1;

}
