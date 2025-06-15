# Cross-Platform Inventory Management System Makefile

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LIBS = `pkg-config --cflags --libs gtk+-3.0`

# Directories
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin

# Target executable
TARGET = $(BINDIR)/inventory_system

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Include directories
INCLUDES = -I$(INCDIR)

# Default target
all: directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(OBJDIR) $(BINDIR)

# Link the executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LIBS)
	@echo "Build complete: $(TARGET)"

# Compile source files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(OBJDIR) $(BINDIR)
	@echo "Clean complete"

# Install dependencies (Ubuntu/Debian)
install-deps-ubuntu:
	sudo apt-get update
	sudo apt-get install build-essential pkg-config libgtk-3-dev

# Install dependencies (CentOS/RHEL/Fedora)
install-deps-redhat:
	sudo yum install gcc pkg-config gtk3-devel
	# For newer versions: sudo dnf install gcc pkg-config gtk3-devel

# Install dependencies (macOS with Homebrew)
install-deps-macos:
	brew install gtk+3 pkg-config

# Install dependencies (Windows with MSYS2)
install-deps-windows:
	pacman -S mingw-w64-x86_64-gtk3 mingw-w64-x86_64-pkg-config mingw-w64-x86_64-gcc

# Run the application
run: $(TARGET)
	./$(TARGET)

# Debug build
debug: CFLAGS += -DDEBUG -g3 -O0
debug: $(TARGET)

# Release build
release: CFLAGS += -O2 -DNDEBUG
release: clean $(TARGET)

# Check for required dependencies
check-deps:
	@echo "Checking for required dependencies..."
	@pkg-config --exists gtk+-3.0 && echo "✓ GTK+3 found" || echo "✗ GTK+3 not found"
	@which gcc > /dev/null && echo "✓ GCC found" || echo "✗ GCC not found"
	@which pkg-config > /dev/null && echo "✓ pkg-config found" || echo "✗ pkg-config not found"

# Help target
help:
	@echo "Available targets:"
	@echo "  all              - Build the application (default)"
	@echo "  clean            - Remove build artifacts"
	@echo "  run              - Build and run the application"
	@echo "  debug            - Build with debug symbols"
	@echo "  release          - Build optimized release version"
	@echo "  check-deps       - Check for required dependencies"
	@echo "  install-deps-*   - Install dependencies for specific platforms"
	@echo "  help             - Show this help message"

.PHONY: all clean run debug release check-deps help directories
.PHONY: install-deps-ubuntu install-deps-redhat install-deps-macos install-deps-windows
