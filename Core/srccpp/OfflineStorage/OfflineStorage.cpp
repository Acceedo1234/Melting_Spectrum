/*
 * OfflineStorage.cpp
 *
 *  Created on: 09-Feb-2023
 *      Author: MKS
 */
#include "main.h"
#include "OfflineStorage.h"
#include <stdio.h>
#include <string.h>
#include "Struct_Input_output.h"
using namespace std;

#define MAXNOOFBLOCK 32

extern "C" {
void W25qxx_WriteSector(uint8_t *pBuffer, uint32_t Sector_Address, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_SectorSize);
void W25qxx_ReadSector(uint8_t *pBuffer, uint32_t Sector_Address, uint32_t OffsetInByte, uint32_t NumByteToRead_up_to_SectorSize);
void W25qxx_EraseSector(uint32_t SectorAddr);
bool W25qxx_IsEmptyBlock(uint32_t Block_Address, uint32_t OffsetInByte, uint32_t NumByteToCheck_up_to_BlockSize);
void W25qxx_EraseBlock(uint32_t BlockAddr);
void W25qxx_EraseChip(void);
}

uint8_t ProductionSet_uintFormat[100]={0};
char ProductionSet_charFormat[100]={'\0'};
uint16_t SectorPos;

/*Data need to saved in flash*/

extern uint8_t Ip_config_Ip[4],Ip_Config_Subnet[4],Ip_config_gateway[4],Ip_config_DNS[4],Ip_config_Server[4];
extern uint8_t Update_Dwin_Set_Data;
extern uint16_t Ip_config_Server_Port;

/*check for change in data*/
extern uint8_t Trigger_Memory_Update,Trigger_Memory_Furnaceno;
extern SpectrumResult SpectrumResult_furnace[4];


OfflineStorage::OfflineStorage() {
	// TODO Auto-generated constructor stub

}

OfflineStorage::~OfflineStorage() {
	// TODO Auto-generated destructor stub
}

void OfflineStorage::run()
{
	m_writesetdata();
}

void OfflineStorage::m_writesetdata(){
	if(Trigger_Memory_Update)
	{
		Trigger_Memory_Update=0;

		sectorselect = Trigger_Memory_Furnaceno*2;
		W25qxx_EraseSector(sectorselect);
		W25qxx_WriteSector(&SpectrumResult_furnace[Trigger_Memory_Furnaceno].result_spectrum[0],sectorselect,0,218);
		W25qxx_EraseSector(sectorselect+1);
		W25qxx_WriteSector(&SpectrumResult_furnace[Trigger_Memory_Furnaceno].result_spectrum[218],(sectorselect+1),0,200);
		Trigger_Memory_Furnaceno=0;
	}
	if(Update_Dwin_Set_Data)
	{
		Update_Dwin_Set_Data=0;
		W25qxx_EraseSector(100);
		m_writeDwinBuf[0] = Ip_config_Ip[0];
		m_writeDwinBuf[1] = Ip_config_Ip[1];
		m_writeDwinBuf[2] = Ip_config_Ip[2];
		m_writeDwinBuf[3] = Ip_config_Ip[3];

		m_writeDwinBuf[4] = Ip_Config_Subnet[0];
		m_writeDwinBuf[5] = Ip_Config_Subnet[1];
		m_writeDwinBuf[6] = Ip_Config_Subnet[2];
		m_writeDwinBuf[7] = Ip_Config_Subnet[3];

		m_writeDwinBuf[8] = Ip_config_gateway[0];
		m_writeDwinBuf[9] = Ip_config_gateway[1];
		m_writeDwinBuf[10] = Ip_config_gateway[2];
		m_writeDwinBuf[11] = Ip_config_gateway[3];

		m_writeDwinBuf[12] = Ip_config_DNS[0];
		m_writeDwinBuf[13] = Ip_config_DNS[1];
		m_writeDwinBuf[14] = Ip_config_DNS[2];
		m_writeDwinBuf[15] = Ip_config_DNS[3];

		m_writeDwinBuf[16] = Ip_config_Server[0];
		m_writeDwinBuf[17] = Ip_config_Server[1];
		m_writeDwinBuf[18] = Ip_config_Server[2];
		m_writeDwinBuf[19] = Ip_config_Server[3];

		m_writeDwinBuf[20] = (unsigned char)(Ip_config_Server_Port>>8)&0x00ff;
		m_writeDwinBuf[21] = (unsigned char)(Ip_config_Server_Port)&0x00ff;
		W25qxx_WriteSector(m_writeDwinBuf,100,0,22);
	}
}


void OfflineStorage::ReadOfflinedataInit()
{
	W25qxx_ReadSector(&SpectrumResult_furnace[0].result_spectrum[0],0,0,218);
	W25qxx_ReadSector(&SpectrumResult_furnace[0].result_spectrum[218],1,0,200);


	W25qxx_ReadSector(&SpectrumResult_furnace[1].result_spectrum[0],2,0,218);
	W25qxx_ReadSector(&SpectrumResult_furnace[1].result_spectrum[218],3,0,200);

	W25qxx_ReadSector(&SpectrumResult_furnace[2].result_spectrum[0],4,0,218);
	W25qxx_ReadSector(&SpectrumResult_furnace[2].result_spectrum[218],5,0,200);

	W25qxx_ReadSector(&SpectrumResult_furnace[3].result_spectrum[0],6,0,218);
	W25qxx_ReadSector(&SpectrumResult_furnace[3].result_spectrum[218],7,0,200);

	W25qxx_ReadSector(m_readFlashBuf,100,0,22);

	 Ip_config_Ip[0] = m_readFlashBuf[0];
	 Ip_config_Ip[1] = m_readFlashBuf[1];
	 Ip_config_Ip[2] = m_readFlashBuf[2];
	 Ip_config_Ip[3] = m_readFlashBuf[3];

	 Ip_Config_Subnet[0]= m_readFlashBuf[4];
	 Ip_Config_Subnet[1]= m_readFlashBuf[5];
	 Ip_Config_Subnet[2]= m_readFlashBuf[6];
	 Ip_Config_Subnet[3]= m_readFlashBuf[7];

	 Ip_config_gateway[0] = m_readFlashBuf[8];
	 Ip_config_gateway[1] = m_readFlashBuf[9];
	 Ip_config_gateway[2] = m_readFlashBuf[10];
	 Ip_config_gateway[3] = m_readFlashBuf[11];

	 Ip_config_DNS[0] = m_readFlashBuf[12];
	 Ip_config_DNS[1] = m_readFlashBuf[13];
	 Ip_config_DNS[2] = m_readFlashBuf[14];
	 Ip_config_DNS[3] = m_readFlashBuf[15];

	 Ip_config_Server[0] = m_readFlashBuf[16];
	 Ip_config_Server[1] = m_readFlashBuf[17];
	 Ip_config_Server[2] = m_readFlashBuf[18];
	 Ip_config_Server[3] = m_readFlashBuf[19];

	 Ip_config_Server_Port = ((m_readFlashBuf[20]<<8)|(m_readFlashBuf[21]));

}

void OfflineStorage::ECUProductionInit(void)
{

}

