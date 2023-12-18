#include "ch32v00x_it.h"

void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/**
 * @fn      HardFault_Handler
 *
 * @brief   This function handles Hard Fault exception.
 *
 * @return  none
 */
void HardFault_Handler(void)
{
	while (1)
	{
	}
}
