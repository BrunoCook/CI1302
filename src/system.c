/**
 * @file system.c
 * @brief System Initialization Implementation for CI1302 Chip
 * @version 1.0
 * @date 2025-11-10
 */

#include "system.h"
#include "gpio.h"

/* Register Access Macros */
#define REG32(addr)    (*(volatile uint32_t *)(addr))

/* System Core Clock */
static uint32_t SystemCoreClock = SYSTEM_CLOCK_FREQ;

/**
 * @brief System Initialization
 * @note Called at startup before main()
 */
void SystemInit(void)
{
    /* Configure system clock */
    SystemClock_Config();
}

/**
 * @brief Configure System Clock
 * @note Sets up the system clock to 72 MHz
 */
void SystemClock_Config(void)
{
    uint32_t temp;
    
    /* Enable HSI (High Speed Internal) oscillator */
    temp = REG32(RCC_BASE + RCC_CR_OFFSET);
    temp |= (1 << 0);  /* HSION */
    REG32(RCC_BASE + RCC_CR_OFFSET) = temp;
    
    /* Wait for HSI ready */
    while (!(REG32(RCC_BASE + RCC_CR_OFFSET) & (1 << 1)));
    
    /* Configure PLL and system clock dividers */
    temp = REG32(RCC_BASE + RCC_CFGR_OFFSET);
    temp &= ~0x0000FFFF;
    temp |= 0x00001D00;  /* PLL configuration */
    REG32(RCC_BASE + RCC_CFGR_OFFSET) = temp;
    
    /* Update system core clock variable */
    SystemCoreClock = SYSTEM_CLOCK_FREQ;
}

/**
 * @brief Enable GPIO port clock
 * @param gpio_base GPIO port base address
 */
void GPIO_ClockEnable(uint32_t gpio_base)
{
    uint32_t temp = REG32(RCC_BASE + RCC_APB2ENR_OFFSET);
    
    switch (gpio_base)
    {
        case GPIOA_BASE:
            temp |= RCC_APB2ENR_IOPAEN;
            break;
        case GPIOB_BASE:
            temp |= RCC_APB2ENR_IOPBEN;
            break;
        case GPIOC_BASE:
            temp |= RCC_APB2ENR_IOPCEN;
            break;
        default:
            return;
    }
    
    REG32(RCC_BASE + RCC_APB2ENR_OFFSET) = temp;
}

/**
 * @brief Get System Core Clock frequency
 * @return System core clock in Hz
 */
uint32_t SystemCoreClock_Get(void)
{
    return SystemCoreClock;
}
