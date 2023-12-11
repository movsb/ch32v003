#include <ch32v00x/ch32v00x.h>
#include <ch32v00x/debug.h>

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SystemCoreClockUpdate();
	Delay_Init();
	USART_Printf_Init(115200);

	printf("SystemClk:%lu\r\n",SystemCoreClock);
	printf( "ChipID:%08lx\r\n", DBGMCU_GetCHIPID() );
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef g = {
		.GPIO_Pin   = GPIO_Pin_1,
		.GPIO_Speed = GPIO_Speed_10MHz,
		.GPIO_Mode  = GPIO_Mode_Out_OD,
	};
	GPIO_Init(GPIOC, &g);

	for (int i = 1;; i++)
	{
		Delay_Ms(100);
		GPIO_WriteBit(GPIOC,  GPIO_Pin_1, Bit_RESET);
		Delay_Ms(100);
		GPIO_WriteBit(GPIOC,  GPIO_Pin_1, Bit_SET);
		
		printf("count: %d\r\n", i);
	}
}
