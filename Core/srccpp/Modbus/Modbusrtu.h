/*
 * Modbusrtu.h
 *
 *  Created on: 07-Jan-2023
 *      Author: MKS
 */

#ifndef SRCCPP_MODBUS_MODBUSRTU_H_
#define SRCCPP_MODBUS_MODBUSRTU_H_

#include "main.h"


extern UART_HandleTypeDef hlpuart1;


class Modbusrtu {
public:
	Modbusrtu();
	virtual ~Modbusrtu();
	void dwinFrame(void);
	void dwinDecoder(void);

	uint8_t  Cntid;
	uint16_t u16CRC;
	uint8_t  noOfData;
	uint8_t u8ModbusRegister[13];

private:

};

#endif /* SRCCPP_MODBUS_MODBUSRTU_H_ */
