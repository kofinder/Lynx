# Makefile for Lynx Project (Rewritten, Ninja-safe, Clang-tidy integrated)

# =============================
# Project Flags
# =============================
DEBUG ?= 1
BUILD_DIR = build
EXAMPLE_DIR = toy
LSP_CLIENT_DIR = extension/build
BUILD_SOURCE_DIR = $(BUILD_DIR)/src
EXECUTABLE_NAME = $(BUILD_SOURCE_DIR)/Lynx
LSP_SERVER_NAME = $(BUILD_SOURCE_DIR)/LynxLSPServer
GRAMMAR_JSON = $(BUILD_SOURCE_DIR)/lang/grammar.json
UNIT_TESTS = $(BUILD_DIR)/tests/unit_tests

BISON_DIR = bison
SKIA_DIR = external/skia
BISON_REPORT = $(BUILD_DIR)/bison_report.txt

# =============================
# Tooling Integration
# =============================
CLANG_FORMAT ?= clang-format
CLANG_TIDY ?= clang-tidy
TIDY_THREADS ?= $(shell nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 8)
SRC_FILES := $(shell find src include -name '*.cpp' -o -name '*.hpp')
RUN_CLANG_TIDY_SCRIPT = tools/run-clang-tidy.sh

# =============================
# Utility Macros
# =============================
# Detect whether Ninja or Make is being used inside build/
BUILD_SYSTEM = $(shell if [ -f "$(BUILD_DIR)/build.ninja" ]; then echo ninja; else echo make; fi)

PARALLEL_JOBS = $(shell nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 8)

# =============================
# Main Targets
# =============================
all: create build

create:
	@echo "✨ Checking and creating build directory..."
	@mkdir -p $(BUILD_DIR) $(BISON_DIR)
	@echo "✅ Build directory ready at $(BUILD_DIR)"

configure:
	@echo "🛠 Configuring the project with LLVM 21 / clang-21"
	@cd $(BUILD_DIR) && cmake -G Ninja \
		-DCMAKE_BUILD_TYPE=Debug \
		-DCMAKE_C_COMPILER=/usr/local/llvm-21/bin/clang \
		-DCMAKE_CXX_COMPILER=/usr/local/llvm-21/bin/clang++ \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..

# Build target — auto-detect Ninja or Make
build: configure
	@echo "🚀 Building in $(BUILD_DIR) using $(BUILD_SYSTEM) with $(PARALLEL_JOBS) threads..."
	@cd $(BUILD_DIR) && \
	if [ "$(BUILD_SYSTEM)" = "ninja" ]; then \
		ninja -j$(PARALLEL_JOBS); \
	else \
		cmake --build . -- -j$(PARALLEL_JOBS); \
	fi

# =============================
# Install / Utility
# =============================
install: build
	@echo "📊 Generating Graphviz dependency graph"
	@cd $(BUILD_DIR) && cmake .. --graphviz=graph.dot && dot -Tpng graph.dot -o graph.png

validate: build
	@echo "✅ Running unit tests"
	@cd $(BUILD_DIR)/tests && ./unit_tests

execute: build
	@echo "▶️ Running $(EXECUTABLE_NAME)"
	@./$(EXECUTABLE_NAME) run -c $(EXAMPLE_DIR)/app_config.yaml -e main.lynx

cli: build
	@echo "🦊 Running Lynx CLI with args: '$(ARGS)'"
	@./$(EXECUTABLE_NAME) $(ARGS);

# =============================
# Parsing / Debug Tools
# =============================
syntax:
	@echo "📜 Running Bison..."
	@bison --report=all --report-file=$(BISON_REPORT) -d $(BISON_DIR)
	@echo "📝 Bison report saved to $(BISON_REPORT)"
	@grep -i conflict $(BISON_REPORT) || echo "✅ No conflicts found."

trace: build
	@echo "🕵️ Running Valgrind..."
	@valgrind --leak-check=full --show-leak-kinds=all ./$(EXECUTABLE_NAME) run -c $(EXAMPLE_DIR)/app_config.yaml -e main.lynx

debug: build
	@echo "🐞 Starting GDB..."
	@gdb -ex "set breakpoint pending on" -ex run --args ./$(EXECUTABLE_NAME) run -c $(EXAMPLE_DIR)/app_config.yaml -e main.lynx

# =============================
# Formatting & Static Analysis
# =============================
format:
	@echo "🎨 Running clang-format..."
	@$(CLANG_FORMAT) -i $(SRC_FILES)
	@echo "✅ Formatting done."

tidy: build
	@echo "🧹 Running clang-tidy across project..."
	@if [ -x "$(RUN_CLANG_TIDY_SCRIPT)" ]; then \
		$(RUN_CLANG_TIDY_SCRIPT) $(TIDY_THREADS) -header-filter='^src/'; -system-headers; \
	else \
		$(CLANG_TIDY) $(SRC_FILES) -p $(BUILD_DIR) -header-filter='^src/'; -system-headers; \
	fi
	@echo "✅ Clang-tidy analysis done."


# =============================
# Cleanup
# =============================
clean:
	@echo "🧹 Cleaning up..."
	@sh tools/build-cleanup.sh

.PHONY: all create configure build install validate execute cli debug trace clean syntax format tidy