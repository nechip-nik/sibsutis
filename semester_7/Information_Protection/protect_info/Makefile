CC = g++
LIB_DIR = cryptolib
OBJ_DIR = obj
CFLAGS = -Wall -Wextra -I$(LIB_DIR) -fPIC

LIB_SRC = $(wildcard $(LIB_DIR)/*.cpp)
LIB_OBJ = $(patsubst $(LIB_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(LIB_SRC))
LIB_NAME = libcrypto.so

APP_SRC = main.cpp
APP_OBJ = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(APP_SRC))
BIN = crypt

all: $(BIN)

$(BIN) : $(APP_OBJ) $(LIB_NAME)
	$(CC) $(CFLAGS) -o $@ $(APP_OBJ) -L. -lcrypto

$(LIB_NAME): $(LIB_OBJ)
	$(CC) -shared -o $@ $^

$(OBJ_DIR)/%.o: $(LIB_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(BIN) $(OBJ_DIR) $(LIB_NAME)

run:
	export LD_LIBRARY_PATH=.:$$LD_LIBRARY_PATH && ./$(BIN)
	

.PHONY: all clean run