# Compiler config
CXX = clang++
CXXFLAGS = -std=c++20 -I./include -Wall -Wextra

# Build config
TEST_DIR := tests
BUILD_DIR := .build
TIMESTAMP := $(shell date +%s)
SRC = src

# internal/net
NET_SRC = $(shell find $(SRC)/internal/net -name '*.cpp')

setup_package:
	@echo "- Setting up packages dir"
	@mkdir -p $(BUILD_DIR)/packages

define setup_objs_dir
	@echo "- Setting up $(1) dir at $(TIMESTAMP)"
	@mkdir -p $(BUILD_DIR)/objs/$(1)_$(TIMESTAMP)
endef

define build_obj_file
	$(foreach f,$(1), \
		@echo "-- Compiling $(f) to $(BUILD_DIR)/objs/$(2)_$(TIMESTAMP)/$(basename $(notdir $(f))).o"; \
		$(CXX) $(CXXFLAGS) -c $(f) -o $(BUILD_DIR)/objs/$(2)_$(TIMESTAMP)/$(basename $(notdir $(f))).o; 
	)
endef

define build_package
	@echo "-- Creating package $(1).a"
	@objs="$$(ls $(BUILD_DIR)/objs/$(1)_$(TIMESTAMP)/*.o 2>/dev/null)"; \
	if [ -z "$$objs" ]; then \
		echo "-- ERROR: no object files found for package $(1)"; \
		exit 1; \
	fi; \
	ar rcs $(BUILD_DIR)/packages/$(1).a $$objs
endef

build-net: setup_package
	$(call setup_objs_dir,net)
	@echo "- Building $(SRC)/internal/net obj files"
	$(call build_obj_file,$(NET_SRC),net)
	@echo "- Building $(SRC)/internal/net package"
	$(call build_package,net)
	@echo "- Done building $(SRC)/internal/net package"

test-net: build-net 
	@echo "- Setting up dirs for testing"
	@mkdir -p $(BUILD_DIR)/tests 
	@echo "- Building the binary"
	@$(CXX) $(CXXFLAGS) $(TEST_DIR)/net/*.cpp $(BUILD_DIR)/packages/net.a -Isrc -o $(BUILD_DIR)/tests/net.elf 
	@chmod +x ./$(BUILD_DIR)/tests/net.elf 
	@echo "- Executing $(BUILD_DIR)/tests/net.elf"
	@./$(BUILD_DIR)/tests/net.elf
	@echo "- Done testing $(SRC)/internal/net package"

clean-objs:
	@echo "- Removing objs"
	@rm -rf $(BUILD_DIR)/objs

clean-test:
	@echo "- Removing test files"
	@rm -rf $(BUILD_DIR)/tests

clean-packages:
	@echo "- Removing packages"
	@rm -rf $(BUILD_DIR)/packages

clean-all: clean-objs clean-packages clean-test

clean: clean-all

