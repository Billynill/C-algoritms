CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror -pedantic
TARGET = postal_service
SOURCES = main.c heap.c postal_service.c

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET) *.log output_letters.txt

test: $(TARGET)
	./$(TARGET) mappings.txt system.log

.PHONY: clean test