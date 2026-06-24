CC = gcc
CFLAGS = -Wall -Wextra
TARGET = minishell
SRC = miniShell.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) *.o
