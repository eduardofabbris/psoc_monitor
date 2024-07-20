# C compiler
CC = gcc

# Files path
SRC_DIR = src
LIB_DIR = src/include

C_FILES = $(wildcard $(SRC_DIR)/*.c)
H_FILES = $(wildcard $(LIB_DIR)/*.h)

OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(SRC_DIR)/%.o, $(C_FILES))

C_FLAGS = -Wall -Wextra -lncurses

.PHONY: all main clean


all: monitor_psoc clean


monitor_psoc: $(OBJ_FILES) 
	$(CC) -o $@ $(OBJ_FILES) $(C_FLAGS) -I$(LIB_DIR)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c 
	$(CC) -c -o $@ $< $(C_FLAGS) -I$(LIB_DIR)    


clean:
	rm -f src/*.o

