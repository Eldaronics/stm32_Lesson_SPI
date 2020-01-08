
/*
 * nrf.c
 *
 *  Created on: Dec 28, 2019
 *      Author: Eldar-Beta
 */
#include "main.h"
#include "nrf.h"
#include "stdio.h"
#include "string.h"
#include "DWT.h"
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;

uint8_t readRegister(uint8_t nrf,uint8_t readReg){
	uint8_t receiveData = 0;
	switch(nrf){
	case nrf1:
		HAL_GPIO_WritePin(CSN1_GPIO_Port,CSN1_Pin,0);
		HAL_SPI_Transmit(&hspi1,&readReg,1,100);
		HAL_SPI_TransmitReceive(&hspi1,&readReg,&receiveData,1,100);
		HAL_GPIO_WritePin(CSN1_GPIO_Port,CSN1_Pin,1);
		break;
	case nrf2:
		HAL_GPIO_WritePin(CSN2_GPIO_Port,CSN2_Pin,0);
		HAL_SPI_Transmit(&hspi2,&readReg,1,100);
		HAL_SPI_TransmitReceive(&hspi2,&readReg,&receiveData,1,100);
		HAL_GPIO_WritePin(CSN2_GPIO_Port,CSN2_Pin,1);
		break;
	}
	return receiveData;
}

uint8_t writeRegister(uint8_t nrf,uint8_t writeReg,uint8_t writeData){

	uint8_t writtenData = 0;
	//	uint8_t convertedIn = (W_REGISTER | writeReg);
	uint8_t convertedIn = (W_REGISTER | writeReg);
	switch(nrf){
	case nrf1:
		HAL_GPIO_WritePin(CSN1_GPIO_Port,CSN1_Pin,0);
		HAL_SPI_Transmit(&hspi1,&convertedIn,1,100);
		HAL_SPI_Transmit(&hspi1,&convertedIn,1,100);
		HAL_SPI_TransmitReceive(&hspi1,&writeData,&writtenData,1,100);
		HAL_SPI_TransmitReceive(&hspi1,&writeData,&writtenData,1,100);
		HAL_GPIO_WritePin(CSN1_GPIO_Port,CSN1_Pin,1);

		//send the same data again
		HAL_GPIO_WritePin(CSN1_GPIO_Port,CSN1_Pin,0);
		HAL_SPI_Transmit(&hspi1,&convertedIn,1,100);
		HAL_SPI_Transmit(&hspi1,&convertedIn,1,100);
		HAL_SPI_TransmitReceive(&hspi1,&writeData,&writtenData,1,100);
		HAL_SPI_TransmitReceive(&hspi1,&writeData,&writtenData,1,100);
		HAL_GPIO_WritePin(CSN1_GPIO_Port,CSN1_Pin,1);
		break;
	case nrf2:
		HAL_GPIO_WritePin(CSN2_GPIO_Port,CSN2_Pin,0);
		HAL_SPI_Transmit(&hspi2,&convertedIn,1,100);
		HAL_SPI_Transmit(&hspi2,&convertedIn,1,100);
		HAL_SPI_TransmitReceive(&hspi2,&writeData,&writtenData,1,100);
		HAL_SPI_TransmitReceive(&hspi2,&writeData,&writtenData,1,100);
		HAL_GPIO_WritePin(CSN2_GPIO_Port,CSN2_Pin,1);

		//send the same data again
		HAL_GPIO_WritePin(CSN2_GPIO_Port,CSN2_Pin,0);
		HAL_SPI_Transmit(&hspi2,&convertedIn,1,100);
		HAL_SPI_Transmit(&hspi2,&convertedIn,1,100);
		HAL_SPI_TransmitReceive(&hspi2,&writeData,&writtenData,1,100);
		HAL_SPI_TransmitReceive(&hspi2,&writeData,&writtenData,1,100);
		HAL_GPIO_WritePin(CSN2_GPIO_Port,CSN2_Pin,1);
		break;
	}
	return writtenData;
}

uint8_t init(uint8_t nrf){
	switch(nrf){
	case nrf1:
		HAL_GPIO_WritePin(CSN1_GPIO_Port,CSN1_Pin,1);
		HAL_GPIO_WritePin(CE1_GPIO_Port,CE1_Pin,0);

		writeRegister(nrf1,SETUP_RETR,(0b0100<<ARD)|(0b1111<<ARC));//Auto Re-transmit delay set for 1500 us;Auto Retransmit Count set for 15 tries
		writeRegister(nrf1,RF_SETUP,((_BV(RF_PWR_LOW)| _BV(RF_PWR_HIGH)) & ~_BV(RF_DR)));//RF Output power set to 0 dBm,data rate set to 1Mbps
		writeRegister(nrf1,CONFIG,_BV(EN_CRC) | _BV(CRCO)); //set crc mode to 16 bits
		writeRegister(nrf1,DYNPD,0);//disable dynamic payloads
		writeRegister(nrf1,STATUS,_BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT)); //set corresponding interrupts
		writeRegister(nrf1,RF_CH,42); //set channel

		flushRx(nrf1);
		flushTx(nrf1);
	case nrf2:
		HAL_GPIO_WritePin(CSN2_GPIO_Port,CSN2_Pin,1);
		HAL_GPIO_WritePin(CE2_GPIO_Port,CE2_Pin,0);

		writeRegister(nrf2,SETUP_RETR,(0b0100<<ARD)|(0b1111<<ARC));//Auto Re-transmit delay set for 1500 us;Auto Retransmit Count set for 15 tries
		writeRegister(nrf2,RF_SETUP,((_BV(RF_PWR_LOW)| _BV(RF_PWR_HIGH)) & ~_BV(RF_DR)));//RF Output power set to 0 dBm
		writeRegister(nrf2,CONFIG,_BV(EN_CRC) | _BV(CRCO)); //set crc mode to 16 bits
		writeRegister(nrf2,DYNPD,0);//disable dynamic payloads
		writeRegister(nrf2,STATUS,_BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT)); //set corresponding interrupts
		writeRegister(nrf2,RF_CH,42); //set channel

		flushRx(nrf2);
		flushTx(nrf2);
	}
	return 0;
}
uint8_t flushRx(uint8_t nrf){
	uint8_t result = 0;
	switch(nrf){
	case nrf1:
		HAL_GPIO_WritePin(CSN1_GPIO_Port,CSN1_Pin,0);
		HAL_SPI_Transmit(&hspi1,(uint8_t*)FLUSH_RX,1,100);
		HAL_SPI_TransmitReceive(&hspi1,(uint8_t*)FLUSH_RX,&result,1,100);
		HAL_GPIO_WritePin(CSN1_GPIO_Port,CSN1_Pin,1);
		break;
	case nrf2:
		HAL_GPIO_WritePin(CSN2_GPIO_Port,CSN2_Pin,0);
		HAL_SPI_Transmit(&hspi2,(uint8_t*)FLUSH_RX,1,100);
		HAL_SPI_TransmitReceive(&hspi2,(uint8_t*)FLUSH_RX,&result,1,100);
		HAL_GPIO_WritePin(CSN2_GPIO_Port,CSN2_Pin,1);
		break;
	}
	return result;
}
uint8_t flushTx(uint8_t nrf){
	uint8_t result = 0;
	switch(nrf){
	case nrf1:
		HAL_GPIO_WritePin(CSN1_GPIO_Port,CSN1_Pin,0);
		HAL_SPI_Transmit(&hspi1,(uint8_t*)FLUSH_TX,1,100);
		HAL_SPI_TransmitReceive(&hspi1,(uint8_t*)FLUSH_TX,&result,1,100);
		HAL_GPIO_WritePin(CSN1_GPIO_Port,CSN1_Pin,1);
		break;
	case nrf2:
		HAL_GPIO_WritePin(CSN2_GPIO_Port,CSN2_Pin,0);
		HAL_SPI_Transmit(&hspi2,(uint8_t*)FLUSH_TX,1,100);
		HAL_SPI_TransmitReceive(&hspi2,(uint8_t*)FLUSH_TX,&result,1,100);
		HAL_GPIO_WritePin(CSN2_GPIO_Port,CSN2_Pin,1);
		break;
	}
	return result;
}
uint8_t starTransmission(uint8_t nrf,uint8_t data){
	switch(nrf){
	case nrf1:
		writeRegister(nrf1,CONFIG,(CONFIG | _BV(PWR_UP)) & ~_BV(PRIM_RX)); //power up nrf device set it to PTX mode
		delay_us(150);//wait nrf to wake up
		printf("%d \n",writePayload(nrf1,data));
		HAL_GPIO_WritePin(CE1_GPIO_Port,CE1_Pin,1);
		HAL_GPIO_WritePin(CE1_GPIO_Port,CE1_Pin,0);
		break;
		//	case nrf2:
		//		break;
	}
	return 0;
}
uint8_t writePayload(uint8_t nrf,uint8_t data){
	uint8_t status = 0;
	switch(nrf){
	case nrf1:
		HAL_GPIO_WritePin(CSN1_GPIO_Port,CSN1_Pin,0);
		HAL_SPI_Transmit(&hspi1,(uint8_t*)W_TX_PAYLOAD,100,1);
		HAL_SPI_TransmitReceive(&hspi1,&status,&data,100,1);
		HAL_GPIO_WritePin(CSN1_GPIO_Port,CSN1_Pin,1);
		break;
		//	case nrf2:
		//		break;
	}
	return status;
}
