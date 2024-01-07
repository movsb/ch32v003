#include <ch32v00x/debug.h>
#include <ch32v00x/ch32v00x_rcc.h>
#include <ch32v00x/system_ch32v00x.h>

int main(void)
{
	SystemCoreClockUpdate();
	NVIC_EnableIRQ(SysTicK_IRQn);
	USART_Printf_Init(115200);
	SysTick_Init(100);

	printf("SystemClk:%lu\n",SystemCoreClock);
	printf( "ChipID:%08lx\n", DBGMCU_GetCHIPID() );

	uint8_t cali = RCC->CTLR >> 8 & 0xFF;
	uint8_t trim = RCC->CTLR >> 3 & 0x1F;
	printf("Calibration: %d, trim: %d\n", cali, trim);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef g = {
		.GPIO_Pin   = GPIO_Pin_1,
		.GPIO_Speed = GPIO_Speed_10MHz,
		.GPIO_Mode  = GPIO_Mode_Out_OD,
	};
	GPIO_Init(GPIOC, &g);
	
	for (int i = 1;; i++) {
		Delay_Ms(500);
		GPIO_WriteBit(GPIOC,  GPIO_Pin_1, Bit_RESET);
		Delay_Ms(500);
		GPIO_WriteBit(GPIOC,  GPIO_Pin_1, Bit_SET);
		printf("SysTicks: %llu seconds.\n", SysTick_GetUptime()/1000/1000);
	}
}
