# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall 

# Source files
SRCS = main.c shell.c utils.c

# Object files 
OBJS = main.o shell.o utils.o

# Output executable
TARGET = myshell

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c shell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) *.o

run: $(TARGET)
	./$(TARGET)
