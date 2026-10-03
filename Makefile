CFLAGS = -std=c99 -Wall -Wextra -g
BUILD_DIR = build/
MAIN_FILE = src/main.c
TARGET = cagents

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(MAIN_FILE)
	mkdir -p $(BUILD_DIR)
	gcc $(CFLAGS) $(MAIN_FILE) -o $(BUILD_DIR)/$(TARGET)

run: $(BUILD_DIR)/$(TARGET)
	./$(BUILD_DIR)/$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean