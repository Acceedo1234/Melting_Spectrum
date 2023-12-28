/*
 * DisplayRoutine.h
 *
 *  Created on: 01-Feb-2023
 *      Author: MKS
 */

#ifndef SRCCPP_DISPLAYCPP_DISPLAYROUTINE_H_
#define SRCCPP_DISPLAYCPP_DISPLAYROUTINE_H_
#include "GLCD.h"


class DisplayRoutine {
public:
	DisplayRoutine();
	virtual ~DisplayRoutine();
	void run();
	void Init();

private:
	GLCD m_gLCDDrive;
	unsigned char Lpdc[15]="      LPDC   ";
	unsigned char colon[2]=":";
	unsigned char space[4]="  ";
	unsigned char backslash[2]="/";
	unsigned char production[13]="Production:";
	unsigned char rejection[13] ="Rejection :";
	unsigned char Mrejection[13]="MRejection:";
	void value2Digit(uint8_t value,uint8_t posLine,uint8_t posCol);
	void value4Digit(uint16_t value4dig,uint8_t posLine,uint8_t posCol);
	uint8_t GLCDNumericArray[2];
	uint8_t gLCDNumericArraySize_4[4];

};

#endif /* SRCCPP_DISPLAYCPP_DISPLAYROUTINE_H_ */
