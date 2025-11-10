# Makefile for CI1302 Voice Chip + BL Bluetooth System

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2 -pthread
LDFLAGS = -pthread

# Target executable
TARGET = ci1302_bluetooth_system

# Source files
SOURCES = main.cpp ci1302_voice_chip.cpp bl_bluetooth_chip.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Header files
HEADERS = ci1302_voice_chip.h bl_bluetooth_chip.h

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS)
	@echo "Linking $(TARGET)..."
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "Build complete: $(TARGET)"

# Compile source files
%.o: %.cpp $(HEADERS)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -f $(OBJECTS) $(TARGET)
	@echo "Clean complete"

# Run the program
run: $(TARGET)
	@echo "Running $(TARGET)..."
	./$(TARGET)

# Show help
help:
	@echo "CI1302 Voice Chip + BL Bluetooth System - Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  all     - Build the project (default)"
	@echo "  clean   - Remove build artifacts"
	@echo "  run     - Build and run the program"
	@echo "  help    - Show this help message"
	@echo ""
	@echo "Usage examples:"
	@echo "  make          # Build the project"
	@echo "  make clean    # Clean build artifacts"
	@echo "  make run      # Build and run"

.PHONY: all clean run help
