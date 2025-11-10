/**
 * @file gpio.h
 * @brief GPIO Driver for CI1302 Chip
 * @version 1.0
 * @date 2025-11-10
 */

#ifndef __GPIO_H__
#define __GPIO_H__

#include <stdint.h>

/* GPIO Port Definitions */
#define GPIOA_BASE    0x40010000
#define GPIOB_BASE    0x40010400
#define GPIOC_BASE    0x40010800

/* GPIO Register Offsets */
#define GPIO_MODE_OFFSET    0x00
#define GPIO_OTYPE_OFFSET   0x04
#define GPIO_OSPEED_OFFSET  0x08
#define GPIO_PUPD_OFFSET    0x0C
#define GPIO_IDR_OFFSET     0x10
#define GPIO_ODR_OFFSET     0x14
#define GPIO_BSRR_OFFSET    0x18
#define GPIO_LCKR_OFFSET    0x1C
#define GPIO_AFR_OFFSET     0x20

/* GPIO Mode */
#define GPIO_MODE_INPUT     0x00
#define GPIO_MODE_OUTPUT    0x01
#define GPIO_MODE_AF        0x02
#define GPIO_MODE_ANALOG    0x03

/* GPIO Output Type */
#define GPIO_OTYPE_PP       0x00  /* Push-Pull */
#define GPIO_OTYPE_OD       0x01  /* Open-Drain */

/* GPIO Speed */
#define GPIO_SPEED_LOW      0x00
#define GPIO_SPEED_MEDIUM   0x01
#define GPIO_SPEED_HIGH     0x02
#define GPIO_SPEED_VERY_HIGH 0x03

/* GPIO Pull-up/Pull-down */
#define GPIO_PUPD_NONE      0x00
#define GPIO_PUPD_PU        0x01  /* Pull-up */
#define GPIO_PUPD_PD        0x02  /* Pull-down */

/* GPIO Pin Definitions */
#define GPIO_PIN_0          0x0001
#define GPIO_PIN_1          0x0002
#define GPIO_PIN_2          0x0004
#define GPIO_PIN_3          0x0008
#define GPIO_PIN_4          0x0010
#define GPIO_PIN_5          0x0020
#define GPIO_PIN_6          0x0040
#define GPIO_PIN_7          0x0080
#define GPIO_PIN_8          0x0100
#define GPIO_PIN_9          0x0200
#define GPIO_PIN_10         0x0400
#define GPIO_PIN_11         0x0800
#define GPIO_PIN_12         0x1000
#define GPIO_PIN_13         0x2000
#define GPIO_PIN_14         0x4000
#define GPIO_PIN_15         0x8000
#define GPIO_PIN_ALL        0xFFFF

/* GPIO Configuration Structure */
typedef struct {
    uint32_t pin;       /* GPIO pin number */
    uint32_t mode;      /* GPIO mode */
    uint32_t otype;     /* GPIO output type */
    uint32_t speed;     /* GPIO speed */
    uint32_t pupd;      /* GPIO pull-up/pull-down */
    uint32_t alternate; /* GPIO alternate function */
} GPIO_InitTypeDef;

/* Function Prototypes */
void GPIO_Init(uint32_t gpio_base, GPIO_InitTypeDef *gpio_init);
void GPIO_WritePin(uint32_t gpio_base, uint16_t pin, uint8_t state);
uint8_t GPIO_ReadPin(uint32_t gpio_base, uint16_t pin);
void GPIO_TogglePin(uint32_t gpio_base, uint16_t pin);

#endif /* __GPIO_H__ */
