/**
 * @file main.c
 * @brief Main Application Entry Point for CI1302 Chip
 * @version 1.0
 * @date 2025-11-10
 */

#include "system.h"
#include "gpio.h"

/* Delay function for basic timing */
static void delay(uint32_t count)
{
    volatile uint32_t i;
    for (i = 0; i < count; i++)
    {
        __asm__("nop");
    }
}

/**
 * @brief Main function with GPIO initialization
 * @return Should never return in embedded systems
 */
int main(void)
{
    GPIO_InitTypeDef gpio_init;
    
    /* System Initialization */
    SystemInit();
    
    /* Enable GPIO Port A clock */
    GPIO_ClockEnable(GPIOA_BASE);
    
    /* Enable GPIO Port B clock */
    GPIO_ClockEnable(GPIOB_BASE);
    
    /* Enable GPIO Port C clock */
    GPIO_ClockEnable(GPIOC_BASE);
    
    /* Configure PA5 as output (LED example) */
    gpio_init.pin = GPIO_PIN_5;
    gpio_init.mode = GPIO_MODE_OUTPUT;
    gpio_init.otype = GPIO_OTYPE_PP;
    gpio_init.speed = GPIO_SPEED_HIGH;
    gpio_init.pupd = GPIO_PUPD_NONE;
    gpio_init.alternate = 0;
    GPIO_Init(GPIOA_BASE, &gpio_init);
    
    /* Configure PB0 as input with pull-up (Button example) */
    gpio_init.pin = GPIO_PIN_0;
    gpio_init.mode = GPIO_MODE_INPUT;
    gpio_init.otype = GPIO_OTYPE_PP;
    gpio_init.speed = GPIO_SPEED_LOW;
    gpio_init.pupd = GPIO_PUPD_PU;
    gpio_init.alternate = 0;
    GPIO_Init(GPIOB_BASE, &gpio_init);
    
    /* Configure PC13 as output (Status LED) */
    gpio_init.pin = GPIO_PIN_13;
    gpio_init.mode = GPIO_MODE_OUTPUT;
    gpio_init.otype = GPIO_OTYPE_PP;
    gpio_init.speed = GPIO_SPEED_MEDIUM;
    gpio_init.pupd = GPIO_PUPD_NONE;
    gpio_init.alternate = 0;
    GPIO_Init(GPIOC_BASE, &gpio_init);
    
    /* Main loop */
    while (1)
    {
        /* Toggle PA5 LED */
        GPIO_TogglePin(GPIOA_BASE, GPIO_PIN_5);
        delay(1000000);
        
        /* Toggle PC13 Status LED */
        GPIO_TogglePin(GPIOC_BASE, GPIO_PIN_13);
        delay(1000000);
        
        /* Read button state and control LED */
        if (GPIO_ReadPin(GPIOB_BASE, GPIO_PIN_0) == 0)
        {
            /* Button pressed - turn on PA5 */
            GPIO_WritePin(GPIOA_BASE, GPIO_PIN_5, 1);
        }
    }
    
    return 0;
}
