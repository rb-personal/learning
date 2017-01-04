# IPC Makefile

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

CC = gcc
CFLAGS = -Wall -c -I$(SRC_DIR)

LINKER = gcc -o
LIBS =
LFLAGS = -Wall -I$(INC_DIR) $(LIBS)

SRCS := $(wildcard $(SRC_DIR)/*.c)
INCS := $(wildcard $(SRC_DIR)/*.h)

OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
EXES := $(SRCS:$(SRC_DIR)/%.c=$(BIN_DIR)/%.exe)

rm = rm -f

$(OBJS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	$(rm) $(OBJS)
	$(rm) $(EXES)
