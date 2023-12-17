/********************************** (C) COPYRIGHT *******************************
 * File Name          : system_ch32v00x.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2022/08/08
 * Description        : CH32V00x Device Peripheral Access Layer System Source File.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#include <stdbool.h>
#include <ch32v00x/ch32v00x.h>

/* 
* Uncomment the line corresponding to the desired System clock (SYSCLK) frequency (after 
* reset the HSI is used as SYSCLK source).
* If none of the define below is enabled, the HSI is used as System clock source. 
*/

//#define SYSCLK_FREQ_8MHz_HSI    8000000
//#define SYSCLK_FREQ_24MHZ_HSI   HSI_VALUE
//#define SYSCLK_FREQ_48MHZ_HSI   48000000
//#define SYSCLK_FREQ_8MHz_HSE    8000000
//#define SYSCLK_FREQ_24MHz_HSE   HSE_VALUE
#define SYSCLK_FREQ_48MHz_HSE   48000000

/* Clock Definitions */
#ifdef SYSCLK_FREQ_8MHz_HSI
  uint32_t SystemCoreClock         = SYSCLK_FREQ_8MHz_HSI;          /* System Clock Frequency (Core Clock) */
#elif defined SYSCLK_FREQ_24MHZ_HSI
  uint32_t SystemCoreClock         = SYSCLK_FREQ_24MHZ_HSI;        /* System Clock Frequency (Core Clock) */
#elif defined SYSCLK_FREQ_48MHZ_HSI
  uint32_t SystemCoreClock         = SYSCLK_FREQ_48MHZ_HSI;        /* System Clock Frequency (Core Clock) */
#elif defined SYSCLK_FREQ_8MHz_HSE
  uint32_t SystemCoreClock         = SYSCLK_FREQ_8MHz_HSE;         /* System Clock Frequency (Core Clock) */
#elif defined SYSCLK_FREQ_24MHz_HSE
  uint32_t SystemCoreClock         = SYSCLK_FREQ_24MHz_HSE;        /* System Clock Frequency (Core Clock) */
#elif defined SYSCLK_FREQ_48MHz_HSE
  uint32_t SystemCoreClock         = SYSCLK_FREQ_48MHz_HSE;        /* System Clock Frequency (Core Clock) */
#else
  uint32_t SystemCoreClock         = HSI_VALUE;
#endif

__I uint8_t AHBPrescTable[16] = {1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8};


/* system_private_function_proto_types */
static void SetSysClock(void);

#ifdef SYSCLK_FREQ_8MHz_HSI
  static void SetSysClockTo_8MHz_HSI(void);
#elif defined SYSCLK_FREQ_24MHZ_HSI
  static void SetSysClockTo_24MHZ_HSI(void);
#elif defined SYSCLK_FREQ_48MHZ_HSI
  static void SetSysClockTo_48MHZ_HSI(void);
#elif defined SYSCLK_FREQ_8MHz_HSE
  static void SetSysClockTo_8MHz_HSE(void);
#elif defined SYSCLK_FREQ_24MHz_HSE
  static void SetSysClockTo_24MHz_HSE(void);
#elif defined SYSCLK_FREQ_48MHz_HSE
  static void SetSysClockTo_48MHz_HSE(void);
#endif


/*********************************************************************
 * @fn      SystemInit
 *
 * @brief   Setup the microcontroller system Initialize the Embedded Flash Interface,
 *        the PLL and update the SystemCoreClock variable.
 *
 * @return  none
 */
void SystemInit (void)
{
  RCC->CTLR |= (uint32_t)0x00000001;
  RCC->CFGR0 &= (uint32_t)0xFCFF0000;
  RCC->CTLR &= (uint32_t)0xFEF6FFFF;
  RCC->CTLR &= (uint32_t)0xFFFBFFFF;
  RCC->CFGR0 &= (uint32_t)0xFFFEFFFF;
  RCC->INTR = 0x009F0000;

  SetSysClock();
}


/*********************************************************************
 * @fn      SystemCoreClockUpdate
 *
 * @brief   Update SystemCoreClock variable according to Clock Register Values.
 *
 * @return  none
 */
void SystemCoreClockUpdate (void)
{
    uint32_t tmp = 0, pllsource = 0;

    tmp = RCC->CFGR0 & RCC_SWS;

    switch (tmp)
    {
        case 0x00:
            SystemCoreClock = HSI_VALUE;
            break;
        case 0x04:
            SystemCoreClock = HSE_VALUE;
            break;
        case 0x08:
            pllsource = RCC->CFGR0 & RCC_PLLSRC;
            if (pllsource == 0x00)
            {
                SystemCoreClock = HSI_VALUE * 2;
            }
            else
            {
                SystemCoreClock = HSE_VALUE * 2;
            }
            break;
        default:
            SystemCoreClock = HSI_VALUE;
            break;
    }

    tmp = AHBPrescTable[((RCC->CFGR0 & RCC_HPRE) >> 4)];

    if(((RCC->CFGR0 & RCC_HPRE) >> 4) < 8)
    {
        SystemCoreClock /= tmp;
    }
    else
    {
        SystemCoreClock >>= tmp;
    }
}


/*********************************************************************
 * @fn      SetSysClock
 *
 * @brief   Configures the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers.
 *
 * @return  none
 */
static void SetSysClock(void)
{
RCC->APB2PCENR |= RCC_APB2Periph_GPIOD;
GPIOD->CFGLR&=(~0xF0);
GPIOD->CFGLR|=0x80;
GPIOD->BSHR =0x2;
//GPIO_IPD_Unused();
#ifdef SYSCLK_FREQ_8MHz_HSI
    SetSysClockTo_8MHz_HSI();
#elif defined SYSCLK_FREQ_24MHZ_HSI
    SetSysClockTo_24MHZ_HSI();
#elif defined SYSCLK_FREQ_48MHZ_HSI
    SetSysClockTo_48MHZ_HSI();
#elif defined SYSCLK_FREQ_8MHz_HSE
    SetSysClockTo_8MHz_HSE();
#elif defined SYSCLK_FREQ_24MHz_HSE
    SetSysClockTo_24MHz_HSE();
#elif defined SYSCLK_FREQ_48MHz_HSE
    SetSysClockTo_48MHz_HSE();
#endif
 
 /* If none of the define above is enabled, the HSI is used as System clock.
  * source (default after reset) 
	*/ 
}


#ifdef SYSCLK_FREQ_8MHz_HSI

/*********************************************************************
 * @fn      SetSysClockTo_8MHz_HSI
 *
 * @brief   Sets HSE as System clock source and configure HCLK, PCLK2 and PCLK1 prescalers.
 *
 * @return  none
 */
static void SetSysClockTo_8MHz_HSI(void)
{
    /* Flash 0 wait state */
    FLASH->ACTLR &= (uint32_t)((uint32_t)~FLASH_ACTLR_LATENCY);
    FLASH->ACTLR |= (uint32_t)FLASH_ACTLR_LATENCY_0;

    /* HCLK = SYSCLK = APB1 */
    RCC->CFGR0 |= (uint32_t)RCC_HPRE_DIV3;
}

#elif defined SYSCLK_FREQ_24MHZ_HSI

/*********************************************************************
 * @fn      SetSysClockTo_24MHZ_HSI
 *
 * @brief   Sets System clock frequency to 24MHz and configure HCLK, PCLK2 and PCLK1 prescalers.
 *
 * @return  none
 */
static void SetSysClockTo_24MHZ_HSI(void)
{
    /* Flash 0 wait state */
    FLASH->ACTLR &= (uint32_t)((uint32_t)~FLASH_ACTLR_LATENCY);
    FLASH->ACTLR |= (uint32_t)FLASH_ACTLR_LATENCY_0;

    /* HCLK = SYSCLK = APB1 */
    RCC->CFGR0 |= (uint32_t)RCC_HPRE_DIV1;
}


#elif defined SYSCLK_FREQ_48MHZ_HSI

/*********************************************************************
 * @fn      SetSysClockTo_48MHZ_HSI
 *
 * @brief   Sets System clock frequency to 48MHz and configure HCLK, PCLK2 and PCLK1 prescalers.
 *
 * @return  none
 */
static void SetSysClockTo_48MHZ_HSI(void)
{
    /* Flash 0 wait state */
    FLASH->ACTLR &= (uint32_t)((uint32_t)~FLASH_ACTLR_LATENCY);
    FLASH->ACTLR |= (uint32_t)FLASH_ACTLR_LATENCY_1;

    /* HCLK = SYSCLK = APB1 */
    RCC->CFGR0 |= (uint32_t)RCC_HPRE_DIV1;

    /* PLL configuration: PLLCLK = HSI * 2 = 48 MHz */
    RCC->CFGR0 &= (uint32_t)((uint32_t)~(RCC_PLLSRC));
    RCC->CFGR0 |= (uint32_t)(RCC_PLLSRC_HSI_Mul2);

    /* Enable PLL */
    RCC->CTLR |= RCC_PLLON;
    /* Wait till PLL is ready */
    while((RCC->CTLR & RCC_PLLRDY) == 0)
    {
    }
    /* Select PLL as system clock source */
    RCC->CFGR0 &= (uint32_t)((uint32_t)~(RCC_SW));
    RCC->CFGR0 |= (uint32_t)RCC_SW_PLL;    
    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR0 & (uint32_t)RCC_SWS) != (uint32_t)0x08)
    {
    }
}

#elif defined SYSCLK_FREQ_8MHz_HSE

/*********************************************************************
 * @fn      SetSysClockTo_8MHz_HSE
 *
 * @brief   Sets System clock frequency to 56MHz and configure HCLK, PCLK2 and PCLK1 prescalers.
 *
 * @return  none
 */
static void SetSysClockTo_8MHz_HSE(void)
{
    __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

    /* Close PA0-PA1 GPIO function */
    RCC->APB2PCENR |= RCC_AFIOEN;
    AFIO->PCFR1 |= (1<<15);

    RCC->CTLR |= ((uint32_t)RCC_HSEON);

    /* Wait till HSE is ready and if Time out is reached exit */
    do
    {
        HSEStatus = RCC->CTLR & RCC_HSERDY;
        StartUpCounter++;
    } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    RCC->APB2PCENR |= RCC_AFIOEN;
    AFIO->PCFR1 |= (1<<15);

    if ((RCC->CTLR & RCC_HSERDY) != RESET)
    {
        HSEStatus = (uint32_t)0x01;
    }
    else
    {
        HSEStatus = (uint32_t)0x00;
    }

    if (HSEStatus == (uint32_t)0x01)
    {
        /* Flash 0 wait state */
        FLASH->ACTLR &= (uint32_t)((uint32_t)~FLASH_ACTLR_LATENCY);
        FLASH->ACTLR |= (uint32_t)FLASH_ACTLR_LATENCY_0;

        /* HCLK = SYSCLK = APB1 */
        RCC->CFGR0 |= (uint32_t)RCC_HPRE_DIV3;

        /* Select HSE as system clock source */
        RCC->CFGR0 &= (uint32_t)((uint32_t)~(RCC_SW));
        RCC->CFGR0 |= (uint32_t)RCC_SW_HSE;
        /* Wait till HSE is used as system clock source */
        while ((RCC->CFGR0 & (uint32_t)RCC_SWS) != (uint32_t)0x04)
        {
        }
    }
    else
    {
        /*
         * If HSE fails to start-up, the application will have wrong clock
     * configuration. User can add here some code to deal with this error
         */
    }
}

#elif defined SYSCLK_FREQ_24MHz_HSE

/*********************************************************************
 * @fn      SetSysClockTo_24MHz_HSE
 *
 * @brief   Sets System clock frequency to 72MHz and configure HCLK, PCLK2 and PCLK1 prescalers.
 *
 * @return  none
 */
static void SetSysClockTo_24MHz_HSE(void)
{
    __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

    /* Close PA0-PA1 GPIO function */
    RCC->APB2PCENR |= RCC_AFIOEN;
    AFIO->PCFR1 |= (1<<15);

    RCC->CTLR |= ((uint32_t)RCC_HSEON);

    /* Wait till HSE is ready and if Time out is reached exit */
    do
    {
        HSEStatus = RCC->CTLR & RCC_HSERDY;
        StartUpCounter++;
    } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    RCC->APB2PCENR |= RCC_AFIOEN;
    AFIO->PCFR1 |= (1<<15);

    if ((RCC->CTLR & RCC_HSERDY) != RESET)
    {
        HSEStatus = (uint32_t)0x01;
    }
    else
    {
        HSEStatus = (uint32_t)0x00;
    }

    if (HSEStatus == (uint32_t)0x01)
    {
        /* Flash 0 wait state */
        FLASH->ACTLR &= (uint32_t)((uint32_t)~FLASH_ACTLR_LATENCY);
        FLASH->ACTLR |= (uint32_t)FLASH_ACTLR_LATENCY_0;

        /* HCLK = SYSCLK = APB1 */
        RCC->CFGR0 |= (uint32_t)RCC_HPRE_DIV1;

        /* Select HSE as system clock source */
        RCC->CFGR0 &= (uint32_t)((uint32_t)~(RCC_SW));
        RCC->CFGR0 |= (uint32_t)RCC_SW_HSE;
        /* Wait till HSE is used as system clock source */
        while ((RCC->CFGR0 & (uint32_t)RCC_SWS) != (uint32_t)0x04)
        {
        }
    }
    else
    {
        /*
         * If HSE fails to start-up, the application will have wrong clock
     * configuration. User can add here some code to deal with this error
         */
    }
}

#elif defined SYSCLK_FREQ_48MHz_HSE

/*********************************************************************
 * @fn      SetSysClockTo_48MHz_HSE
 *
 * @brief   Sets System clock frequency to 72MHz and configure HCLK, PCLK2 and PCLK1 prescalers.
 *
 * @return  none
 */
static void SetSysClockTo_48MHz_HSE(void)
{
    __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

    /* Close PA0-PA1 GPIO function */
    RCC->APB2PCENR |= RCC_AFIOEN;
    AFIO->PCFR1 |= (1<<15);

    RCC->CTLR |= ((uint32_t)RCC_HSEON);

    /* Wait till HSE is ready and if Time out is reached exit */
    do
    {
        HSEStatus = RCC->CTLR & RCC_HSERDY;
        StartUpCounter++;
    } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    if ((RCC->CTLR & RCC_HSERDY) != RESET)
    {
        HSEStatus = (uint32_t)0x01;
    }
    else
    {
        HSEStatus = (uint32_t)0x00;
    }

    if (HSEStatus == (uint32_t)0x01)
    {
        /* Flash 0 wait state */
        FLASH->ACTLR &= (uint32_t)((uint32_t)~FLASH_ACTLR_LATENCY);
        FLASH->ACTLR |= (uint32_t)FLASH_ACTLR_LATENCY_1;

        /* HCLK = SYSCLK = APB1 */
        RCC->CFGR0 |= (uint32_t)RCC_HPRE_DIV1;

        /* PLL configuration: PLLCLK = HSE * 2 = 48 MHz */
        RCC->CFGR0 &= (uint32_t)((uint32_t)~(RCC_PLLSRC));
        RCC->CFGR0 |= (uint32_t)(RCC_PLLSRC_HSE_Mul2);

        /* Enable PLL */
        RCC->CTLR |= RCC_PLLON;
        /* Wait till PLL is ready */
        while((RCC->CTLR & RCC_PLLRDY) == 0)
        {
        }
        /* Select PLL as system clock source */
        RCC->CFGR0 &= (uint32_t)((uint32_t)~(RCC_SW));
        RCC->CFGR0 |= (uint32_t)RCC_SW_PLL;
        /* Wait till PLL is used as system clock source */
        while ((RCC->CFGR0 & (uint32_t)RCC_SWS) != (uint32_t)0x08)
        {
        }
    }
    else
    {
        /*
         * If HSE fails to start-up, the application will have wrong clock
     * configuration. User can add here some code to deal with this error
         */
    }
}
#endif

static uint32_t _usPerOsTick;
static uint8_t  _ticksPerUs;

/**
 * @brief 启动系统定时器。
 * 
 * @param msPerOsTick   一个操作系统节拍需要多少毫秒。
 * 
 * @note 一般来说，类似 FreeRTOS 的 \p msPerOsTick 为 10ms。
*/
void SysTick_Init(uint32_t msPerOsTick) {
	_usPerOsTick = msPerOsTick * 1000;

	// 一微秒的时钟个数（注意这个 Ticks 和 参数里面的 Ticks 的含义不同）。
	// 按照 8MHz、24、48、144…… 来算，一个字节足够存。
	_ticksPerUs = SystemCoreClock / 1000000 / 8;
	
	// 把一个操作系统 Tick 需要的时间转换成时钟 Ticks 的个数。
	uint32_t ticksPerOsTick = _usPerOsTick * (uint32_t)_ticksPerUs;

	// CNTIF = 0, disable
	SysTick->SR &= ~(1 << 0);
	SysTick->CMP = ticksPerOsTick;
	SysTick->CNT = 0;
	SysTick->CTLR = (1 << 3) | (1 << 1) | (1 << 0);
}

static volatile uint64_t _osTicks = 0;

// TODO 从汇编层面补上中断处理函数占用的周期数
// TODO 用软件压栈的方式是否更好？
void __attribute__((interrupt("WCH-Interrupt-fast"))) SysTick_Handler() {
	_osTicks++;
	SysTick->SR &= ~(1 << 0);
}

/**
 * @note \p _osTicks 与 \p (SysTick->CNT) 不是同时获取的（即原子操作），需要“上锁”。
 *      比如期望的值是：_osTicks，CNT
 *      但是实际可能是：_osTicks，中断，CNT
 *      导致结果可能是：_osTicks 和上次一样，但是由于被中断了一次，CNT 可能归零（或比上次小）
 *      也就是，两次调用本函数，出现第二交比第一次小的情况。
 *      为了解决这个问题，有以下几种解决方案：
 *          - 进入此函数的时候暂停 SysTick 定时器
 *          - 直到比第一次大才返回（不会死循环），但是会多两个静态变量，费空间
 *          - 先取 CNT，如果没中断，那比原来大，结果完全正确；如果中断，_osTicks 比原来大（不对）
 *          - 两次获取 CNT，第二次一定比第一次大才有效（两次获取间隔不可能超过一个 OsTick）（最佳）。
*/

uint64_t SysTick_GetUptime() {
	uint64_t osTicks;
	uint32_t cnt1, cnt2;

	do {
		cnt1 = SysTick->CNT;
		osTicks = _osTicks;
		cnt2 = SysTick->CNT;
	} while(cnt1 >= cnt2);

	return _usPerOsTick * osTicks
		+ cnt2 / (uint32_t)_ticksPerUs;
}
