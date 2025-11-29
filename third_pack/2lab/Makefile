CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror -pedantic
TARGET = vector_test
SOURCES = main_int_vector.c vector.c

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

test: $(TARGET)
	./$(TARGET)

.PHONY: clean test