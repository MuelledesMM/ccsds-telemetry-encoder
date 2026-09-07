CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I./src

# Archivos fuente
SRC = src/ccsds.c
TEST_SRC = tests/test_main.c

# Pruebas
TARGET = test_ccsds

# Compilar todo
all: $(TARGET)

$(TARGET): $(SRC) $(TEST_SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(TEST_SRC)

# COMPILATION
test: $(TARGET)
	@echo "Ejecutando pruebas CCSDS..."
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o
	@echo "Limpieza completada."

# Para evitar conflictos con archivos que se llamen igual que las reglas
.PHONY: all test clean