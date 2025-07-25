TARGET = procplanner

CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Source files
SRCS = main.c

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
