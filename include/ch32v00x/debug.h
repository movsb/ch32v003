#pragma once

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdio.h>
#include <ch32v00x/ch32v00x.h>

void Delay_Init(void);
void Delay_Us(uint32_t n);

/**
 * @brief 把 printf 绑定到串口上。
 * 
 * 端口号为：PD6，重映射为 UTX_2.
 * 
 * 不支持 SDIO  printf，那样会把 SDIO 占用，导致无法烧录程序。
 * 
 * @param baudrate  波特率
*/
void USART_Printf_Init(uint32_t baudrate);

#ifdef __cplusplus
}
#endif
