PROJECT := ether

SRC_DIR := ether
INC_DIR := ether/include

BIN_DIR := build/bin
OBJ_DIR := build/obj

C_FILES := $(shell find $(SRC_DIR) -name "*.cpp")
ASM_FILES := $(shell find $(SRC_DIR) -name "*.asm")

OBJ_FILES := $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(C_FILES)))
OBJ_FILES += $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(ASM_FILES)))
BIN_FILE := $(BIN_DIR)/$(PROJECT)

CC := g++
LD := g++

CFLAGS := -I$(INC_DIR) -D_DEBUG -Wall -Wextra -Wshadow -Wno-write-strings -m64 -g -O0
LDFLAGS :=

run: $(BIN_FILE)
	$(BIN_FILE) -o hello res/hello.eth
	$(LD) -o $(BIN_DIR)/a.out res/hello.o -Wl,--dynamic-linker=/usr/lib64/ld-linux-x86-64.so.2 

debug: $(BIN_FILE)
	gdb --args $(BIN_FILE) -o hello res/hello.eth

$(BIN_FILE): $(OBJ_FILES)
	mkdir -p $(dir $@)
	$(LD) -o $@ $(OBJ_FILES)

$(OBJ_DIR)/%.cpp.o: %.cpp
	mkdir -p $(OBJ_DIR)/$(dir $^)
	$(CC) -c $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.asm.o: %.asm
	mkdir -p $(OBJ_DIR)/$(dir $^)
	nasm -felf64 -o $@ $^

clean:
	rm -rf $(OBJ_FILES)
	rm -rf $(BIN_FILE)

loc:
	find ether -name "*.cpp" -or \
						-name "*.h" -or \
						-name "*.asm" | xargs cat | wc -l

.PHONY: run clean loc
