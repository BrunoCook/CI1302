# Makefile for CI1302 Project
# CI1302 Chip Development

# Project name
PROJECT = ci1302_app

# Compiler and tools
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# Directories
SRC_DIR = src
INC_DIR = inc
BUILD_DIR = build

# Source files
C_SOURCES = \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/gpio.c \
	$(SRC_DIR)/system.c

# Include paths
INCLUDES = -I$(INC_DIR)

# Compiler flags
CFLAGS = -mcpu=cortex-m3 -mthumb
CFLAGS += -Wall -Wextra -O2 -g
CFLAGS += $(INCLUDES)
CFLAGS += -DUSE_STDPERIPH_DRIVER

# Linker flags
LDFLAGS = -mcpu=cortex-m3 -mthumb
LDFLAGS += -specs=nosys.specs
LDFLAGS += -Wl,-Map=$(BUILD_DIR)/$(PROJECT).map,--cref
LDFLAGS += -Wl,--gc-sections

# Object files
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))

# Default target
all: $(BUILD_DIR)/$(PROJECT).elf $(BUILD_DIR)/$(PROJECT).hex $(BUILD_DIR)/$(PROJECT).bin
	@echo "Build complete!"
	$(SIZE) $(BUILD_DIR)/$(PROJECT).elf

# Create build directory
$(BUILD_DIR):
	mkdir -p $@

# Compile C source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files
$(BUILD_DIR)/$(PROJECT).elf: $(OBJECTS)
	@echo "Linking $@"
	$(CC) $(LDFLAGS) $^ -o $@

# Generate hex file
$(BUILD_DIR)/$(PROJECT).hex: $(BUILD_DIR)/$(PROJECT).elf
	@echo "Creating hex file $@"
	$(OBJCOPY) -O ihex $< $@

# Generate binary file
$(BUILD_DIR)/$(PROJECT).bin: $(BUILD_DIR)/$(PROJECT).elf
	@echo "Creating binary file $@"
	$(OBJCOPY) -O binary $< $@

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all clean

# Display help
help:
	@echo "CI1302 Makefile"
	@echo "Available targets:"
	@echo "  all    - Build the project (default)"
	@echo "  clean  - Remove build artifacts"
	@echo "  help   - Display this help message"
