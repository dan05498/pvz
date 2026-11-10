TARGET = pvz

CC = gcc

CFLAGS = -Wall -Wextra -std=c11 -Iinclude

LDFLAGS = -lSDL3 -lSDL3_image -lSDL3_mixer #Banderas para SLD3

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

SRCS_COMMON = $(SRC_DIR)/entradas.c $(SRC_DIR)/logica.c $(SRC_DIR)/visuales.c $(SRC_DIR)/audio.c
OBJS_COMMON = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS_COMMON))

SRC_MAIN = $(SRC_DIR)/main.c
OBJ_MAIN = $(OBJ_DIR)/main.o

SRC_PRUEBA = $(SRC_DIR)/main_prueba.c
OBJ_PRUEBA = $(OBJ_DIR)/main_prueba.o

all: clean directories $(BIN_DIR)/$(TARGET)

directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

$(BIN_DIR)/$(TARGET): $(OBJS_COMMON) $(OBJ_MAIN)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean directories