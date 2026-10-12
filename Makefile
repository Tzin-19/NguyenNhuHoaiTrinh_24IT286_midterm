CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -std=c99 -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=700
OBJ = src/main.o src/flags.o src/file_info.o src/sort.o src/display.o
TARGET = my_ls

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)

.PHONY: all clean