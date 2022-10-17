BUILD_DIR := build

.PHONY: all clean run help

all: $(BUILD_DIR)
	@cmake -S . -B $(BUILD_DIR) -G "MinGW Makefiles"
	@$(MAKE) --no-print-directory -C $(BUILD_DIR)
	@$(MAKE) --no-print-directory -C $(BUILD_DIR) Shaders

$(BUILD_DIR):
	@mkdir -p $@

clean:
	@cmake --build $(BUILD_DIR) --target clean -- --no-print-directory
	@rm -rf $(BUILD_DIR)

run: all
	@cd build && ./VulkanEngine

help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... run"
	@echo "... help"
