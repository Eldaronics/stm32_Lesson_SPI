#include "DWT.h"
#include "stdio.h"

/**
 * @brief  Initializes DWT_Clock_Cycle_Count for DWT_Delay_us function
 * @return Error DWT counter
 *         1: clock cycle counter not started
 *         0: clock cycle counter works
 */
uint16_t __dwt_divider = 0;
uint8_t __dwt_init = 0;
uint32_t DWT_Delay_Init(void) {
	__dwt_init = 1;
	__dwt_divider = (uint16_t)(HAL_RCC_GetHCLKFreq() / 1000000);
  /* Disable TRC */
  CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; // ~0x01000000;
  /* Enable TRC */
  CoreDebug->DEMCR |=  CoreDebug_DEMCR_TRCENA_Msk; // 0x01000000;

  /* Disable clock cycle counter */
  DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; //~0x00000001;
  /* Enable  clock cycle counter */
  DWT->CTRL |=  DWT_CTRL_CYCCNTENA_Msk; //0x00000001;
  /* Reset the clock cycle counter value */
  DWT->CYCCNT = 0;

     /* 3 NO OPERATION instructions */
     __ASM volatile ("NOP");
     __ASM volatile ("NOP");
  __ASM volatile ("NOP");

  /* Check if clock cycle counter has started */
     if(DWT->CYCCNT)
     {
       return 0; /*clock cycle counter started*/
     }
     else
  {
    while(1){
    	//TODO 0,add error led here in future
    	printf("ERROR =>Line %d, File %s\n",__LINE__,__FILE__);
		HAL_Delay(500);
    }
    return 1; /*clock cycle counter not started*/
  }
}

/* Use DWT_Delay_Init (); and DWT_Delay_us (microseconds) in the main */
