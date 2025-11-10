/**
 * @file system.h
 * @brief System Initialization for CI1302 Chip
 * @version 1.0
 * @date 2025-11-10
 */

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <stdint.h>

/* RCC (Reset and Clock Control) Base Address */
#define RCC_BASE            0x40021000

/* RCC Register Offsets */
#define RCC_CR_OFFSET       0x00
#define RCC_CFGR_OFFSET     0x04
#define RCC_APB2ENR_OFFSET  0x18
#define RCC_APB1ENR_OFFSET  0x1C

/* RCC APB2 Peripheral Clock Enable */
#define RCC_APB2ENR_IOPAEN  (1 << 2)
#define RCC_APB2ENR_IOPBEN  (1 << 3)
#define RCC_APB2ENR_IOPCEN  (1 << 4)

/* System Clock Configuration */
#define SYSTEM_CLOCK_FREQ   72000000  /* 72 MHz */

/* Function Prototypes */
void SystemInit(void);
void SystemClock_Config(void);
void GPIO_ClockEnable(uint32_t gpio_base);
uint32_t SystemCoreClock_Get(void);

#endif /* __SYSTEM_H__ */
