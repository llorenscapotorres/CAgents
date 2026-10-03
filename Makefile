PRE_CFLAGS = -std=c99 -Wall -Wextra -g -Isrc
POST_CFLAG = -lcurl -lcjson
BUILD_DIR = build
MAIN_FILE = src/main.c
SUBFILES = src/generate/invoke.c
TARGET = cagents

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(MAIN_FILE) $(SUBFILES)
	mkdir -p $(BUILD_DIR)
	gcc $(PRE_CFLAGS) $(MAIN_FILE) $(SUBFILES) -o $(BUILD_DIR)/$(TARGET) $(POST_CFLAG)

run: $(BUILD_DIR)/$(TARGET)
	./$(BUILD_DIR)/$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean