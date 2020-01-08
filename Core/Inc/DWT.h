#ifndef DWT_H
#define DWT_H

#ifdef __cplusplus
extern "C" {
#endif


#include "main.h"


/**
 * @brief  Initializes DWT_Cycle_Count for DWT_Delay_us function
 * @return Error DWT counter
 *         1: DWT counter Error
 *         0: DWT counter works
 */
uint32_t DWT_Delay_Init(void);

uint16_t __dwt_divider;
uint8_t __dwt_init;

/**
 * @brief  This function provides a delay (in microseconds)
 * @param  microseconds: delay in microseconds
 */
__STATIC_INLINE void delay_us(volatile uint32_t microseconds)
{
	if(!__dwt_init){
		DWT_Delay_Init();
	}

  uint32_t clk_cycle_start = DWT->CYCCNT;

  /* Go to number of cycles for system */
  microseconds *= __dwt_divider;

  /* Delay till end */
  while ((DWT->CYCCNT - clk_cycle_start) < microseconds);
}

__STATIC_INLINE void delay(volatile uint32_t milliseconds)
{
	HAL_Delay(milliseconds);
}

__STATIC_INLINE uint32_t micros()
{
	if(!__dwt_init){
		DWT_Delay_Init();
	}
	return DWT->CYCCNT/__dwt_divider;
}

__STATIC_INLINE uint32_t millis()
{
	return HAL_GetTick();
}

__STATIC_INLINE void dwt_delay(volatile uint32_t milliseconds)
{
	delay_us(milliseconds*1000);
}

__STATIC_INLINE uint32_t dwt_millis()
{
	return (uint32_t)(micros()/1000);
}

__STATIC_INLINE uint32_t millis_diff(uint32_t time_ms)
{
	return (uint32_t)(millis() - time_ms);
}

__STATIC_INLINE uint32_t micros_diff(uint32_t time_us)
{
	return (uint32_t)(micros() - time_us);
}

#ifdef __cplusplus
}
#endif

#endif
