CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror -pedantic
TARGET = math_interpreter
SOURCES = main.c interpreter.c

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET) *.log *.txt

test: $(TARGET)
	@echo "=== ТЕСТИРОВАНИЕ ==="
	@echo "A = 2" > test_input.txt
	@echo "B = 3" >> test_input.txt
	@echo "C = A + B * 2" >> test_input.txt
	@echo "D = C ^ B" >> test_input.txt
	@echo "print(D)" >> test_input.txt
	@echo "E = -5 + A * 3" >> test_input.txt
	@echo "print(E)" >> test_input.txt
	./$(TARGET) test_input.txt test_trace.log
	@echo "=== ФАЙЛ ТРАССИРОВКИ ==="
	@cat test_trace.log

run: $(TARGET)
	./$(TARGET) interactive

.PHONY: clean test run