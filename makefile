CC = cc
WARN_FLAGS = -Wall -Wextra -pedantic -Wformat=2 -Werror
DEBUG_FLAGS = -ggdb
# SANITIZE_FLAGS = -fsanitize=leak,memory,thread,safe-stack
SANITIZE_FLAGS = -fsanitize=address,undefined
CFLAGS = $(WARN_FLAGS) $(DEBUG_FLAGS) $(SANITIZE_FLAGS) -lpthread -std=c99
TARGET = bin/server.out
SRC_DIR = ./src
SRC = $(shell find $(SRC_DIR) -type f -name "*.c")

.PHONY: all build run clean

all: build run

build: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
