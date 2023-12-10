#include <ch32v00x/ch32v00x.h>
#include <ch32v00x/debug.h>

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SystemCoreClockUpdate();
    Delay_Init();

    GPIO_InitTypeDef g = {0};
    g.GPIO_Pin = GPIO_Pin_1;
    g.GPIO_Mode = GPIO_Mode_Out_OD;
    g.GPIO_Speed = GPIO_Speed_2MHz;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_Init(GPIOC, &g);
    
    g.GPIO_Pin = GPIO_Pin_6;
    g.GPIO_Mode = GPIO_Mode_AF_PP;
    g.GPIO_Speed = GPIO_Speed_50MHz;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1, ENABLE);
    GPIO_Init(GPIOD, &g);
    GPIO_PinRemapConfig(GPIO_PartialRemap2_USART1, ENABLE);

    USART_InitTypeDef USART_InitStructure = {0};
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);

    printf("SystemClk:%lu\r\n",SystemCoreClock);
    printf( "ChipID:%08lx\r\n", DBGMCU_GetCHIPID() );


    int i = 0;
    
    sum(1, 2);

    while (1)
    {
        Delay_Ms(100);
        GPIO_WriteBit(GPIOC,  GPIO_Pin_1, Bit_RESET);
        Delay_Ms(100);
        GPIO_WriteBit(GPIOC,  GPIO_Pin_1, Bit_SET);
        
        printf("count: %d\r\n", ++i);
    }
}
