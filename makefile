CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

TARGET = shellforge

SRC = src/main.c src/lexer.c src/token.c
OBJ = $(SRC:.c=.o)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -lreadline -o $(TARGET)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
