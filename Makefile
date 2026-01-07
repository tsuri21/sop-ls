CC = gcc

SRC_DIR = src
TEST_DIR = tests
INC_DIR = include
BUILD_DIR = build

APP_NAME = els
TEST_NAME = test_runner

CFLAGS = -Wall -Wextra -std=gnu11 -O0 -g
INCLUDES = -I$(INC_DIR)
LIBS = -pthread

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
TEST_FILES = $(wildcard $(TEST_DIR)/*.c)

APP_OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))
TEST_OBJS = $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/test_%.o,$(TEST_FILES))

APP_BIN = $(BUILD_DIR)/$(APP_NAME)
TEST_BIN = $(BUILD_DIR)/$(TEST_NAME)

.PHONY: all clean test run

all: $(APP_BIN)

$(APP_BIN): $(APP_OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(APP_OBJS) $(LIBS) -o $(APP_BIN)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/*.h
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

test: $(TEST_BIN)
	$(TEST_BIN)

$(TEST_BIN): $(APP_OBJS) $(TEST_OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(APP_OBJS) $(TEST_OBJS) $(LIBS) -o $(TEST_BIN)

$(BUILD_DIR)/test_%.o: $(TEST_DIR)/%.c $(INC_DIR)/*.h
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

run: $(APP_BIN)
	@$(APP_BIN) $(ARGS)

clean:
	rm -rf $(BUILD_DIR)
