CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror -pedantic
TARGET = bracket_checker
SOURCES = main.c brackets.c

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

test: $(TARGET)
	@echo "=== ТЕСТИРОВАНИЕ ==="
	@./$(TARGET) 2

run: $(TARGET)
	./$(TARGET)

.PHONY: clean test run