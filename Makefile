# Makefile for mac-free
#
# A 'free' command replacement for macOS
# License: MIT

# ============================================================================
# Configuration
# ============================================================================

# Compiler and flags
CC       = clang
CFLAGS   = -Wall -Wextra -Werror -pedantic -std=c11
CFLAGS  += -O2 -fstack-protector-strong
LDFLAGS  =

# Debug build flags
DEBUG_CFLAGS = -g -O0 -DDEBUG -fsanitize=address,undefined
DEBUG_LDFLAGS = -fsanitize=address,undefined

# Directories
SRC_DIR  = src
OBJ_DIR  = obj
BIN_DIR  = bin

# Target
TARGET   = $(BIN_DIR)/free

# Source files
SOURCES  = $(wildcard $(SRC_DIR)/*.c)
OBJECTS  = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
DEPS     = $(OBJECTS:.o=.d)

# Include paths
INCLUDES = -I$(SRC_DIR)

# Installation
PREFIX   ?= /usr/local
BINDIR   = $(PREFIX)/bin
MANDIR   = $(PREFIX)/share/man/man1

# ============================================================================
# Targets
# ============================================================================

.PHONY: all clean debug install uninstall test help

# Default target
all: $(TARGET)

# Link target
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $@ $^
	@echo "Build complete: $@"

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c -o $@ $<

# Create directories
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Include dependency files
-include $(DEPS)

# Debug build
debug: CFLAGS += $(DEBUG_CFLAGS)
debug: LDFLAGS += $(DEBUG_LDFLAGS)
debug: clean $(TARGET)
	@echo "Debug build complete"

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Clean complete"

# Install
install: $(TARGET)
	install -d $(DESTDIR)$(BINDIR)
	install -m 755 $(TARGET) $(DESTDIR)$(BINDIR)/free
	@echo "Installed to $(DESTDIR)$(BINDIR)/free"
	@echo ""
	@echo "Note: This will shadow the system 'free' command (if any)."
	@echo "You may need to restart your shell or run 'hash -r'"

# Uninstall
uninstall:
	rm -f $(DESTDIR)$(BINDIR)/free
	@echo "Uninstalled from $(DESTDIR)$(BINDIR)/free"

# Run basic tests
test: $(TARGET)
	@echo "Running basic tests..."
	@echo ""
	@echo "Test 1: Default output"
	@$(TARGET)
	@echo ""
	@echo "Test 2: Human-readable output"
	@$(TARGET) -h
	@echo ""
	@echo "Test 3: Wide output"
	@$(TARGET) -w
	@echo ""
	@echo "Test 4: Human-readable wide output with totals"
	@$(TARGET) -h -w -t
	@echo ""
	@echo "Test 5: Bytes output"
	@$(TARGET) -b
	@echo ""
	@echo "Test 6: Mebibytes output"
	@$(TARGET) -m
	@echo ""
	@echo "Test 7: Gibibytes output"
	@$(TARGET) -g
	@echo ""
	@echo "Test 8: Version"
	@$(TARGET) -V
	@echo ""
	@echo "All tests passed!"

# Help
help:
	@echo "mac-free Makefile"
	@echo ""
	@echo "Targets:"
	@echo "  all       Build the project (default)"
	@echo "  debug     Build with debug flags and sanitizers"
	@echo "  clean     Remove build artifacts"
	@echo "  install   Install to $(BINDIR)"
	@echo "  uninstall Remove from $(BINDIR)"
	@echo "  test      Run basic tests"
	@echo "  help      Show this help message"
	@echo ""
	@echo "Variables:"
	@echo "  PREFIX    Installation prefix (default: /usr/local)"
	@echo "  CC        C compiler (default: clang)"
	@echo "  CFLAGS    Additional compiler flags"
	@echo "  LDFLAGS   Additional linker flags"
	@echo ""
	@echo "Examples:"
	@echo "  make"
	@echo "  make debug"
	@echo "  make install PREFIX=/opt/local"
	@echo "  make clean"
