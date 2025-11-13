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

all: create build

# -------------------------------------------------------------------
# Create the build folder if it does not exist
# This ensures the build system has a proper workspace.
# Using `mkdir -p` guarantees no error if the folder already exists.
# -------------------------------------------------------------------
create:
	@echo "✨ Checking and creating build directory..."
	@mkdir -p $(BUILD_DIR)
	@echo "✅ Build directory ready at $(BUILD_DIR)"

configure:
	@echo "🛠 Configuring the project with LLVM 21 / clang-21"
	cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=Debug \
	    -DCMAKE_C_COMPILER=/usr/local/llvm-21/bin/clang \
	    -DCMAKE_CXX_COMPILER=/usr/local/llvm-21/bin/clang++ \
	    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..

build: configure
	@echo "🚀 Building in $(BUILD_DIR)"
	cd $(BUILD_DIR) && cmake --build . -- -j$(nproc)

install: build
	@echo "📊 Generating Graphviz dependency graph"
	cd $(BUILD_DIR) && cmake .. --graphviz=graph.dot && dot -Tpng graph.dot -o graph.png

validate: build
	@echo "✅ Running unit tests"
	cd $(BUILD_DIR)/tests && ./unit_tests

execute: build
	@echo "▶️ Running $(EXECUTABLE_NAME)"
	./$(EXECUTABLE_NAME) run -c $(EXAMPLE_DIR)/app_config.yaml -e main.lynx

cli: build
	@echo "🦊 Running Lynx CLI with args: '$(ARGS)'"
	./$(EXECUTABLE_NAME) $(ARGS);

syntax:
	@echo "📜 Running Bison..."
	bison --report=all --report-file=$(BISON_REPORT) -d $(BISON_DIR)
	@echo "📝 Bison report saved to $(BISON_REPORT)"
	@grep -i conflict $(BISON_REPORT) || echo "✅ No conflicts found."

trace: build
	@echo "🕵️ Running Valgrind..."
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXECUTABLE_NAME) run -c $(EXAMPLE_DIR)/app_config.yaml -e main.lynx
	#valgrind --leak-check=full --show-leak-kinds=all --gen-suppressions=all ./$(EXECUTABLE_NAME) run -c ${EXAMPLE_DIR}/app_config.yaml -e main.lynx
	#valgrind -s --leak-check=full --suppressions=llvm.supp --track-origins=yes --show-leak-kinds=all ./$(EXECUTABLE_NAME) run -c ${EXAMPLE_DIR}/app_config.yaml -e main.lynx

debug: build
	@echo "🐞 Starting GDB..."
	gdb -ex "set breakpoint pending on" -ex run --args ./$(EXECUTABLE_NAME) run -c $(EXAMPLE_DIR)/app_config.yaml -e main.lynx

clean:
	@echo "🧹 Cleaning up..."
	@sh sh_clean.sh
	cd ${BISON_DIR}/ && sh run.sh clean
	# cd ${SKIA_DIR}/ && rm -rf out/Static

.PHONY: all create configure install build validate execute cli debug trace clean syntax
