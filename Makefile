BUILD_DIR ?= build

.PHONY: all configure build run clean

all: build

configure:
	cmake -S . -B $(BUILD_DIR)

build: configure
	cmake --build $(BUILD_DIR)

run: build
	cd $(BUILD_DIR) && ./main

clean:
	rm -rf $(BUILD_DIR)
