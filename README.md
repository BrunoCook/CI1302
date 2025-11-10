# CI1302
CI1302芯片开发

## 项目简介 (Project Overview)

本项目为CI1302芯片的嵌入式开发项目，包含GPIO配置和系统初始化功能。

This project is an embedded development project for the CI1302 chip, including GPIO configuration and system initialization functions.

## 项目结构 (Project Structure)

```
CI1302/
├── inc/                # 头文件目录 (Header files)
│   ├── gpio.h         # GPIO驱动头文件
│   └── system.h       # 系统初始化头文件
├── src/                # 源文件目录 (Source files)
│   ├── main.c         # 主函数
│   ├── gpio.c         # GPIO驱动实现
│   └── system.c       # 系统初始化实现
├── build/              # 编译输出目录 (Build output)
├── Makefile            # 构建脚本
└── README.md           # 项目说明
```

## 功能特性 (Features)

- **系统初始化**: 系统时钟配置，支持72MHz主频
- **GPIO驱动**: 完整的GPIO驱动，支持输入/输出/复用/模拟模式
- **时钟管理**: GPIO端口时钟使能
- **示例应用**: LED闪烁和按键检测示例

## 编译环境 (Build Environment)

需要安装ARM交叉编译工具链:
- arm-none-eabi-gcc
- arm-none-eabi-objcopy
- arm-none-eabi-size

## 编译方法 (Build Instructions)

```bash
# 编译项目
make

# 清理编译文件
make clean

# 查看帮助
make help
```

编译成功后，会在 `build/` 目录下生成以下文件:
- `ci1302_app.elf` - ELF格式可执行文件
- `ci1302_app.hex` - HEX格式固件
- `ci1302_app.bin` - BIN格式固件
- `ci1302_app.map` - 链接映射文件

## GPIO配置示例 (GPIO Configuration Example)

```c
GPIO_InitTypeDef gpio_init;

/* 配置PA5为输出 (Configure PA5 as output) */
gpio_init.pin = GPIO_PIN_5;
gpio_init.mode = GPIO_MODE_OUTPUT;
gpio_init.otype = GPIO_OTYPE_PP;
gpio_init.speed = GPIO_SPEED_HIGH;
gpio_init.pupd = GPIO_PUPD_NONE;
GPIO_Init(GPIOA_BASE, &gpio_init);

/* 设置引脚电平 (Set pin state) */
GPIO_WritePin(GPIOA_BASE, GPIO_PIN_5, 1);  // 高电平
GPIO_WritePin(GPIOA_BASE, GPIO_PIN_5, 0);  // 低电平

/* 翻转引脚 (Toggle pin) */
GPIO_TogglePin(GPIOA_BASE, GPIO_PIN_5);
```

## 使用说明 (Usage)

1. 系统上电后自动调用 `SystemInit()` 进行系统初始化
2. 主函数中使能所需的GPIO端口时钟
3. 配置GPIO引脚的功能和参数
4. 在主循环中实现应用逻辑

## 主要API (Main APIs)

### 系统初始化 (System Initialization)
- `void SystemInit(void)` - 系统初始化
- `void SystemClock_Config(void)` - 时钟配置
- `void GPIO_ClockEnable(uint32_t gpio_base)` - 使能GPIO时钟

### GPIO操作 (GPIO Operations)
- `void GPIO_Init(uint32_t gpio_base, GPIO_InitTypeDef *gpio_init)` - GPIO初始化
- `void GPIO_WritePin(uint32_t gpio_base, uint16_t pin, uint8_t state)` - 写GPIO引脚
- `uint8_t GPIO_ReadPin(uint32_t gpio_base, uint16_t pin)` - 读GPIO引脚
- `void GPIO_TogglePin(uint32_t gpio_base, uint16_t pin)` - 翻转GPIO引脚

## 许可证 (License)

This project is open source.
