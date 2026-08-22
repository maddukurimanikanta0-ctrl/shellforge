CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC = src/token.c \
      src/lexer.c \
      src/parser.c \
      src/expand.c \
      src/builtin.c \
      src/executor.c \
      src/main.c

OBJ = $(SRC:.c=.o)

shellforge: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o shellforge

clean:
	rm -f $(OBJ) shellforge
