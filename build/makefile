TARGET = procplanner

SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I$(INC_DIR)

# Source files
SRCS = $(SRC_DIR)/main.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default rule
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET) $(OBJS)

# Rule to force recompilation
rebuild: clean all
