/**
 * @file gpio.c
 * @brief GPIO Driver Implementation for CI1302 Chip
 * @version 1.0
 * @date 2025-11-10
 */

#include "gpio.h"

/* Register Access Macros */
#define REG32(addr)    (*(volatile uint32_t *)(addr))

/**
 * @brief Initialize GPIO pin
 * @param gpio_base GPIO port base address
 * @param gpio_init GPIO initialization structure
 */
void GPIO_Init(uint32_t gpio_base, GPIO_InitTypeDef *gpio_init)
{
    uint32_t position = 0;
    uint32_t temp = 0;
    
    /* Configure each pin in the pin mask */
    for (position = 0; position < 16; position++)
    {
        if (gpio_init->pin & (1 << position))
        {
            /* Configure GPIO mode */
            temp = REG32(gpio_base + GPIO_MODE_OFFSET);
            temp &= ~(0x03 << (position * 2));
            temp |= (gpio_init->mode << (position * 2));
            REG32(gpio_base + GPIO_MODE_OFFSET) = temp;
            
            if (gpio_init->mode == GPIO_MODE_OUTPUT || gpio_init->mode == GPIO_MODE_AF)
            {
                /* Configure GPIO output type */
                temp = REG32(gpio_base + GPIO_OTYPE_OFFSET);
                temp &= ~(0x01 << position);
                temp |= (gpio_init->otype << position);
                REG32(gpio_base + GPIO_OTYPE_OFFSET) = temp;
                
                /* Configure GPIO speed */
                temp = REG32(gpio_base + GPIO_OSPEED_OFFSET);
                temp &= ~(0x03 << (position * 2));
                temp |= (gpio_init->speed << (position * 2));
                REG32(gpio_base + GPIO_OSPEED_OFFSET) = temp;
            }
            
            /* Configure GPIO pull-up/pull-down */
            temp = REG32(gpio_base + GPIO_PUPD_OFFSET);
            temp &= ~(0x03 << (position * 2));
            temp |= (gpio_init->pupd << (position * 2));
            REG32(gpio_base + GPIO_PUPD_OFFSET) = temp;
            
            /* Configure alternate function if needed */
            if (gpio_init->mode == GPIO_MODE_AF)
            {
                uint32_t afr_offset = (position < 8) ? GPIO_AFR_OFFSET : (GPIO_AFR_OFFSET + 4);
                uint32_t afr_position = (position < 8) ? position : (position - 8);
                
                temp = REG32(gpio_base + afr_offset);
                temp &= ~(0x0F << (afr_position * 4));
                temp |= (gpio_init->alternate << (afr_position * 4));
                REG32(gpio_base + afr_offset) = temp;
            }
        }
    }
}

/**
 * @brief Write GPIO pin state
 * @param gpio_base GPIO port base address
 * @param pin GPIO pin number
 * @param state Pin state (0 or 1)
 */
void GPIO_WritePin(uint32_t gpio_base, uint16_t pin, uint8_t state)
{
    if (state)
    {
        /* Set pin */
        REG32(gpio_base + GPIO_BSRR_OFFSET) = pin;
    }
    else
    {
        /* Reset pin */
        REG32(gpio_base + GPIO_BSRR_OFFSET) = (pin << 16);
    }
}

/**
 * @brief Read GPIO pin state
 * @param gpio_base GPIO port base address
 * @param pin GPIO pin number
 * @return Pin state (0 or 1)
 */
uint8_t GPIO_ReadPin(uint32_t gpio_base, uint16_t pin)
{
    return (REG32(gpio_base + GPIO_IDR_OFFSET) & pin) ? 1 : 0;
}

/**
 * @brief Toggle GPIO pin state
 * @param gpio_base GPIO port base address
 * @param pin GPIO pin number
 */
void GPIO_TogglePin(uint32_t gpio_base, uint16_t pin)
{
    uint32_t odr = REG32(gpio_base + GPIO_ODR_OFFSET);
    REG32(gpio_base + GPIO_BSRR_OFFSET) = ((odr & pin) << 16) | (~odr & pin);
}
