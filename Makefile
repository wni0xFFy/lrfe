TARGET = lrfe

CC = clang

CFLAGS = -Wall -Wextra -O2 -I./include/ -g #-fsanitize=address
BUILD_DIR = build
BIN_DIR = bin

SRCS := $(shell find . -name "*.c" )
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)
EXEC =  $(TARGET)

#
all: $(EXEC)
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)
	@rm -rf $(BUILD_DIR)
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: all
