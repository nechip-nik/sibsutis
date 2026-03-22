CC = gcc 
CFLAGS = -Wall -Wextra 

# ifeq ($(MU), 1)
# CFLAGS += -DMU
# endif

# ifeq ($(LAMBDA), 1)
# CFLAGS += -DLAMBDA
# endif

# ifeq ($(M), 1)
# CFLAGS += -DM
# endif

SRC_DIR = src
OBJ_DIR = obj

SRC = $(SRC_DIR)/main.c
OBJ = $(OBJ_DIR)/main.o
BIN = lab2

$(BIN): %: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $< -lm

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ -lm

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(BIN) $(OBJ_DIR)

run:
	./$(BIN)

.PHONY: all clean