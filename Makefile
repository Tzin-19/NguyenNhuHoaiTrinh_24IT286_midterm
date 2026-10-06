CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -std=c99 -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=700
SRC = src/main.c src/flags.c src/file_info.c src/sort.c src/display.c
OBJ = $(SRC:.c=.o)
TARGET = my_ls

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET) $(TARGET).exe

.PHONY: all clean