.PHONY: test test-fast build-test clean-test help

BUILD_DIR := build
CMAKE := cmake
MAKE := make

configure:
	@cd $(BUILD_DIR) && $(CMAKE) ..

build-test: configure
	@cd $(BUILD_DIR) && $(MAKE) RaylibGame_tests

test: build-test
	@cd $(BUILD_DIR) && ctest --output-on-failure

build:
	@cd $(BUILD_DIR) && $(MAKE)