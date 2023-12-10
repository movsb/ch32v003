#include <ch32v00x/debug.h>

static uint8_t  p_us = 0;
static uint16_t p_ms = 0;

/*********************************************************************
 * @fn      Delay_Init
 *
 * @brief   Initializes Delay Function.
 *
 * @return  none
 */
void Delay_Init(void)
{
    p_us = SystemCoreClock / 8000000;
    p_ms = (uint16_t)p_us * 1000;
}

/*********************************************************************
 * @fn      Delay_Us
 *
 * @brief   Microsecond Delay Time.
 *
 * @param   n - Microsecond number.
 *
 * @return  None
 */
void Delay_Us(uint32_t n)
{
    uint32_t i;

    SysTick->SR &= ~(1 << 0);
    i = (uint32_t)n * p_us;

    SysTick->CMP = i;
    SysTick->CNT = 0;
    SysTick->CTLR |=(1 << 0);

    while((SysTick->SR & (1 << 0)) != (1 << 0));
    SysTick->CTLR &= ~(1 << 0);
}

/*********************************************************************
 * @fn      Delay_Ms
 *
 * @brief   Millisecond Delay Time.
 *
 * @param   n - Millisecond number.
 *
 * @return  None
 */
void Delay_Ms(uint32_t n)
{
    uint32_t i;

    SysTick->SR &= ~(1 << 0);
    i = (uint32_t)n * p_ms;

    SysTick->CMP = i;
    SysTick->CNT = 0;
    SysTick->CTLR |=(1 << 0);

    while((SysTick->SR & (1 << 0)) != (1 << 0));
    SysTick->CTLR &= ~(1 << 0);
}

void USART_Printf_Init(uint32_t baudrate)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1, ENABLE);

	// 把 PD6 映射为 UTX_2，避免与 SDIO 冲突
	GPIO_PinRemapConfig(GPIO_PartialRemap2_USART1, ENABLE);

	GPIO_InitTypeDef g = {
		.GPIO_Pin   = GPIO_Pin_6,
		.GPIO_Mode  = GPIO_Mode_AF_PP,
		.GPIO_Speed = GPIO_Speed_2MHz,
	};
	GPIO_Init(GPIOD, &g);
	
	USART_InitTypeDef u = {
		.USART_BaudRate             = baudrate,
		.USART_WordLength           = USART_WordLength_8b,
		.USART_StopBits             = USART_StopBits_1,
		.USART_Parity               = USART_Parity_No,
		.USART_HardwareFlowControl  = USART_HardwareFlowControl_None,
		.USART_Mode                 = USART_Mode_Tx,
	};

	USART_Init(USART1, &u);
	USART_Cmd(USART1, ENABLE);
}

int __attribute__((used)) _write(int __attribute__((unused)) fd, char *buf, int size)
{
    for(int i = 0; i < size; i++){
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        USART_SendData(USART1, *buf++);
    }
    return size;
}
