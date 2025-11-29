CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror -pedantic
TARGET = number_converter
SOURCES = main.c dec_to_base.c

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

test: $(TARGET)
	./$(TARGET)

.PHONY: clean test