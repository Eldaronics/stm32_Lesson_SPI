/*
 * nrf_reg_data.c
 *
 *  Created on: Jan 6, 2020
 *      Author: Eldar-Beta
 */
#include "main.h"
#include "nrf.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

uint8_t printDetails(uint8_t nrf){
	printf("showing info for nrf%d\n",nrf);
	getConfig(nrf);

	printf("\n___Channel Register___\n");
	printf("channel is: %d",getChannel(nrf));

	getSetup(nrf);
	getStatus(nrf);
	getSetup_RETR(nrf);
	getFIFO_Status(nrf);

	printf("________________________________\n");
	return 0;
}
uint8_t getStatus(uint8_t nrf){
	registerParse(readRegister(nrf,STATUS));
	registerParse(readRegister(nrf,STATUS));

	printf("\n___STATUS register___\n");
	convertedVal[TX_FULL] == 1?printf("0)TX is full(1)\n"):printf("0)TX is not full(0)\n");
	convertedVal[1] == 1?printf("1)Data pipe number for the payload(1): 1\n"):printf("1)Data pipe number for the payload(1): 0\n");
	convertedVal[2] == 1?printf("2)Data pipe number for the payload(2): 1\n"):printf("2)Data pipe number for the payload(2): 0\n");
	convertedVal[3] == 1?printf("3)Data pipe number for the payload(3): 1\n"):printf("3)Data pipe number for the payload(3): 0\n");
	convertedVal[MAX_RT] == 1?printf("4)Maximum number of TX retries interrupt is set(1)\n"):printf("4)Maximum number of TX retries interrupt is not set(0)\n");
	convertedVal[TX_DS] == 1?printf("5)Data Sent TX FIFO interrupt is set (1)\n"):printf("5)Data Sent TX FIFO interrupt is not set (0)\n");
	convertedVal[RX_DR] == 1?printf("6)Data Ready RX FIFO interrupt is set (1)\n"):printf("6)Data Ready RX FIFO interrupt is not set (0)\n");
	convertedVal[7] == 1?printf("7)RESERVED bit (1)\n"):printf("7)RESERVED bit (0)\n");

	memset(convertedVal,0,sizeof(convertedVal));
	return 0;
}
uint8_t getConfig(uint8_t nrf){
	registerParse(readRegister(nrf,CONFIG));
	registerParse(readRegister(nrf,CONFIG));

	printf("\n___CONFIG register___\n");
	convertedVal[PRIM_RX] == 1?printf("0)PRX is activated (1)\n"):printf("0)PTX is activated (0)\n");
	convertedVal[PWR_UP] == 1?printf("1)POWER UP mode is activated (1)\n"):printf("1)POWER DOWN mode is activated (0)\n");
	convertedVal[CRCO] == 1?printf("2)CRC encoding scheme: 2 bytes (1)\n"):printf("2)CRC encoding scheme: 1 bytes (0)\n");
	convertedVal[EN_CRC] == 1?printf("3)CRC is enabled (1)\n"):printf("3CRC is not enabled (0)\n");
	convertedVal[MASK_MAX_RT] == 1?printf("4)MAX_RT is not reflected on IRQ pin (1)\n"):printf("4)MAX_RT is reflected on IRQ pin as active LOW (0)\n");
	convertedVal[MASK_TX_DS] == 1?printf("5)TX_DS is not reflected on IRQ pin (1)\n"):printf("5)TX_DS is  reflected on IRQ pin as active LOW (0)\n");
	convertedVal[MASK_RX_DR] == 1?printf("6)RX_DR is not reflected on IRQ pin (1)\n"):printf("6)RX_DR is not reflected on IRQ pin as active LOW (0)\n");
	convertedVal[7] == 1?printf("7)RESERVED bit (1)\n"):printf("7)RESERVED bit (0)\n");

	memset(convertedVal,0,sizeof(convertedVal));
	return 0;
}
uint8_t getSetup(uint8_t nrf){
	registerParse(readRegister(nrf,RF_SETUP));
	registerParse(readRegister(nrf,RF_SETUP));
	uint8_t rf_power = 0;

	printf("\n___SETUP register___\n");
	convertedVal[LNA_HCURR] == 1?printf("0)Setup LNA gain (1)\n"):printf("0)Setup LNA gain (1)\n");

	if(convertedVal[1] == 1 && convertedVal[1] == 1){
		rf_power = 0;
	}
	else if(convertedVal[1] == 0 && convertedVal[1] == 1){
		rf_power = 12;
	}
	else if(convertedVal[1] == 1 && convertedVal[1] == 0){
		rf_power = 6;
	}
	else{
		rf_power = 18;
	}
//aa
	printf("2)RF output power is: %d dBm\n",rf_power);
	convertedVal[RF_DR] == 1?printf("3)RF data rate is 2 Mbps (1)\n"):printf("3)RF data rate is 1 Mbps (0) \n");
	convertedVal[PLL_LOCK] == 1?printf("4)PLL lock is set (1)\n"):printf("4)PLL lock is not set (0)\n");
	printf("5) to 7) - reserved bits \n");

	memset(convertedVal,0,sizeof(convertedVal));
	return 0;
}
uint8_t getSetup_RETR(uint8_t nrf){
	registerParse(readRegister(nrf,SETUP_RETR));
	registerParse(readRegister(nrf,SETUP_RETR));

	printf("\n___Setup_RETR register___\n");
	printf("0) to 4) Auto Re-Transmit Count:\n");
	for(int i = 0;i<4;i++){
		printf("%d",convertedVal[i]);
	}
	printf("\n");
	printf("5) to 7) Auto Re-transmit Delay:\n");
	for(int i = 4;i<8;i++){
		printf("%d",convertedVal[i]);
	}
	printf("\n");
	memset(convertedVal,0,sizeof(convertedVal));
	return 0;
}
uint8_t getFIFO_Status(uint8_t nrf){
	printf("\n___FIFO_STATUS Register___\n");
	registerParse(readRegister(nrf,FIFO_STATUS));
	registerParse(readRegister(nrf,FIFO_STATUS));

	convertedVal[RX_EMPTY] == 1? printf("1)RX FIFO is empty (1)\n"): printf("1)RX FIFO is full (0)\n");
	convertedVal[RX_FULL] == 1? printf("2)RX FIFO is full (1)\n"): printf("2)RX FIFO is empty (0)\n");
	printf("2) and 3) reserved\n");
	convertedVal[TX_EMPTY] == 1? printf("4)TX FIFO is empty (1)\n"): printf("4)TX FIFO is full (0)\n");
	convertedVal[TX_FULL] == 1? printf("5)TX FIFO is full (1)\n"): printf("5)TX FIFO is empty (0)\n");
	convertedVal[TX_REUSE] == 1? printf("6)Reuse data packet bit (1)\n"): printf("6)Reuse data packet bit (0)\n");

	printf("7) reserved\n");
	return 0;
}
uint8_t getChannel(uint8_t nrf){
	uint8_t channel;
	channel = readRegister(nrf,RF_CH);
	return channel;
}
uint8_t registerParse(uint8_t val){
	uint8_t shiftedVal,countVal = 0;
	for(;countVal<8;countVal++){
		shiftedVal = val >> countVal;
		(shiftedVal & 1)? (convertedVal[countVal] = 1):(convertedVal[countVal] = 0);
		pointerToString = &convertedVal[countVal];
	}
	return 0;
}
uint8_t readInTurn(uint8_t nrf){
	static int cnt = 0;
	char buf[32];
	itoa(readRegister(nrf,cnt),buf,2);
	printf("%d register data :%d in binary: %s\n",cnt,readRegister(nrf,cnt),buf);
	cnt++;
	if(cnt >17){
		cnt = 0;
		printf("____________________________\n");
	}
	HAL_Delay(100);
	return 0;
}

