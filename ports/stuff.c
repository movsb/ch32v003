#include <stdint.h>
#include <inttypes.h>
#include <ch32v00x/debug.h>

void __stuff_base_time_sleep_us(int64_t microseconds) {
	// TODO 类型转换警告
	Delay_Us((uint32_t)(microseconds));
}
