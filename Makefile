BUILD_DIR := cli/build
BINARY := $(BUILD_DIR)/stu

.PHONY: all run clean

all: $(BINARY)

$(BUILD_DIR)/Makefile: cli/CMakeLists.txt
	cmake -S cli -B $(BUILD_DIR)

$(BINARY): $(BUILD_DIR)/Makefile cli/main.cpp
	cmake --build $(BUILD_DIR)

run: $(BINARY)
	$(BINARY) --help

clean:
	rm -rf $(BUILD_DIR)
