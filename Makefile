# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Target executable
TARGET = deadlock

# All source files
SRCS = main.c simulator.c predictor.c visualizer.c utils.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default rule — build everything
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile each .c to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program
run: all
	./$(TARGET)