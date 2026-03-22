CC = gcc 
CFLAGS = -Wall -Wextra -lm

ifeq ($(MU), 1)
CFLAGS += -DMU
endif

ifeq ($(LAMBDA), 1)
CFLAGS += -DLAMBDA
endif

ifeq ($(M), 1)
CFLAGS += -DM
endif

SRC_DIR = src
OBJ_DIR = obj

SRC = $(SRC_DIR)/main.c
OBJ = $(OBJ_DIR)/main.o
BIN = lab1

$(BIN): %: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(BIN) $(OBJ_DIR)

run:
	./$(BIN)

.PHONY: all clean