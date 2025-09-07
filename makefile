CFLAGS := $(shell sdl2-config --cflags) -Iinclude
LIBS := $(shell sdl2-config --libs) -lSDL2_ttf

BIN := bin
SRCS := src/main.c src/utils.c

# Phony targets
.PHONY: clean all

# Default target: run the binary
all: $(BIN)
	./$(BIN)

# Link the binary
$(BIN): $(SRCS)
	clang $(CFLAGS) $^ -o $@ $(LIBS)

clean: 
	rm -rf bin
