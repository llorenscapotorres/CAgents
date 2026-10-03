PRE_CFLAGS = -std=c99 -Wall -Wextra -g
POST_CFLAG = -lcurl -lcjson
BUILD_DIR = build/
MAIN_FILE = src/main.c
TARGET = cagents

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(MAIN_FILE)
	mkdir -p $(BUILD_DIR)
	gcc $(PRE_CFLAGS) $(MAIN_FILE) -o $(BUILD_DIR)/$(TARGET) $(POST_CFLAG)

run: $(BUILD_DIR)/$(TARGET)
	./$(BUILD_DIR)/$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean