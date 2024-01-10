#include <stdbool.h>
#include <stddef.h>
#include <inttypes.h>
#include <ch32v00x/debug.h>
#include <ch32v00x/system_ch32v00x.h>

/*********************************************************************
 * @brief   Microsecond Delay Time.
 * @param   n - Microsecond number.
 * @return  None
 */
void Delay_Us(uint32_t n)
{
	uint64_t t = SysTick_GetUptime();
	while(SysTick_GetUptime() - t < n);
}

/*********************************************************************
 * @brief   Millisecond Delay Time.
 * @param   n - Millisecond number.
 * @return  None
 */
void Delay_Ms(uint32_t n)
{
	return Delay_Us(n*1000);
}

static bool g_usart_printf_initialized = false;

void USART_Printf_Init(uint32_t baudrate)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

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

	g_usart_printf_initialized = true;
}

static void _usart_send_char(uint8_t c) {
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	USART_SendData(USART1, c);
}

/**
 * @brief 重定向底层输出函数，以使得 printf 可以输出到串口设备。
 * 
 * @see [Retargeting printf](https://www.imagecraft.com/help/ICCV9CORTEX/web/55_Retargetingprintf.htm)
*/
int __attribute__((used)) _write(int __attribute__((unused)) fd, char *buf, int size)
{
	// 调用 __libc_init_array 初始化构造函数的时候串口还没有初始化，
	// 所以会卡死在 while 语句那里。这里加个全局变量避免一下。
	if (!g_usart_printf_initialized) {
		return size;
	}

	for(int i = 0; i < size; i++){
		if (*buf == '\n') _usart_send_char('\r');
		_usart_send_char(*buf);
		buf++;
	}
	return size;
}

/**
 * @brief 动态改变数据段的边界地址（即堆内存）。
 * 
 * @see 几个外部变量的定义来自于链接器脚本文件中。
 * 
 *  - \p _heap_start    是堆内存的起始地址（ 即 \p .bss 段的结束地址）；
 *  - \p _heap_end      是堆内存的结束地址（栈的起始地址）；
 *  
 * @note _sbrk(0) 可以在启动时拿到堆内存起始地址。
 *  
 * @return 改变前的地址。如果失败，返回 -1.
*/
void __attribute__((used)) *_sbrk(ptrdiff_t incr)
{
	extern char _heap_start[];
	extern char _heap_end[];

	static char *cur = _heap_start;
	
	if (cur + incr < _heap_start || cur + incr > _heap_end)
		return NULL - 1;
	
	char *r = cur;
	cur += incr;

	return r;
}

// C++ 支持
// Use MRS Create C++ project.pdf
void _init() { }
void _fini() { }
