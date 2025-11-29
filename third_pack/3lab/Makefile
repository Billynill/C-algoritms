CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror -pedantic
TARGET = liver_manager
SOURCES = main.c liver.c linked_list.c

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET) test_livers.txt output_livers.txt

test: $(TARGET)
	./$(TARGET)

.PHONY: clean test