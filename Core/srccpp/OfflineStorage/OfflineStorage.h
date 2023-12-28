/*
 * OfflineStorage.h
 *
 *  Created on: 09-Feb-2023
 *      Author: MKS
 */

#ifndef SRCCPP_OFFLINESTORAGE_OFFLINESTORAGE_H_
#define SRCCPP_OFFLINESTORAGE_OFFLINESTORAGE_H_

class OfflineStorage {
public:
	OfflineStorage();
	virtual ~OfflineStorage();
	void run();
	void ReadOfflinedataInit();
	void ECUProductionInit(void);
private:
	uint32_t sectorselect;
	uint8_t m_readFlashBuf[30];
	uint8_t m_writeDwinBuf[30];

	void m_writesetdata(void);

};

#endif /* SRCCPP_OFFLINESTORAGE_OFFLINESTORAGE_H_ */
