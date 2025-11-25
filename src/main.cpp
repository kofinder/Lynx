/**
 * --------------------------------------------------------------------------
 *  Lynx Compiler Frontend - Main Entry Point
 * --------------------------------------------------------------------------
 *  This file contains the primary execution flow for the Lynx compiler /
 *  runtime system. It initializes program options, dispatches CLI commands,
 *  orchestrates source parsing, semantic analysis, IR generation, linking,
 *  LTO optimization, and final JIT execution.
 *
 *  Key Features:
 *    - Uses ProgramOptionConfig for command-line configuration
 *    - Supports CLI subcommands such as "create", "generate", and "scaffold"
 *    - Provides a timing utility (timeExecution) to measure compiler phases
 *    - Executes a full compilation pipeline using the Lynx core:
 *         1. LLVM Initialization
 *         2. Parsing
 *         3. Analysis
 *         4. IR Generation
 *         5. Linking
 *         6. LTO Optimization
 *         7. JIT Execution
 *
 *  Notes:
 *    - Formatting and color macros come from ProgramTerminalColor.hpp
 *    - LLVM resources are properly released at program termination
 *    - Execution timing is printed for developer diagnostics
 *
 *  This file serves as the high-level coordinator of the entire compilation
 *  workflow and is intentionally lightweight, delegating real work to the
 *  Lynx core components.
 *
 *  @author: Ko Thein (Nathan Mratt)
 *  @date:   Jan 2, 2022
 * --------------------------------------------------------------------------
*/

#include <chrono>
#include <iostream>
#include <string>
#include <cstdlib>
#include "Lynx.hpp"
#include <cli/CLI.hpp>  
#include <llvm/Support/ManagedStatic.h>
#include <config/ProgramTerminalColor.hpp>
#include <config/ProgramOptionConfig.hpp>

namespace {

    template<typename Func>
    auto timeExecution(const std::string& label, Func&& func) {
        auto start = std::chrono::steady_clock::now();
        std::cout << LYNX_GREEN << "[Executing] " << label << "..." << LYNX_RESET << "\n";
        
        auto result = std::forward<Func>(func)(); 

        auto end = std::chrono::steady_clock::now();
        auto taken = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        std::cout << LYNX_CYAN << "⏱ " << label << " took " << taken << " ms" << LYNX_RESET << "\n";
        return result;
    }

}

int main(int argc, char const *argv[]) {

    auto total_start = std::chrono::steady_clock::now();
    const std::string separator = std::string(LYNX_RED) + LYNX_SEPARATOR() + std::string(LYNX_RESET);

    std::cout << separator << "\n" << LYNX_BLUE;
    std::cout << "********* Begin Program Execution *********" << LYNX_CYAN << "\n";
    
    auto& config = ProgramOptionConfig::instance();
    config.initialize(argc, argv);
    const std::string& cmd = config.getCommand();

    if (cmd == "create" || cmd == "generate" || cmd == "scaffold") {
        LynxCLI::CLI cli;
        cli.run();
        std::cout << LYNX_GREEN << "[Success] CLI command executed successfully." << LYNX_RESET << "\n";
        return EXIT_SUCCESS;
    }
    
    int exitCode = 0;
    {
        Lynx lynx(config);

        timeExecution("Step 0: LLVM Initialization...", [&]() {
            lynx.initializeLLVM();
            return 0;
        });

        const int parseStatus = timeExecution("Step 1: Parsing Sources", [&]() {
            return lynx.parseSource();
        });

        if (parseStatus != 0) {
            std::cerr << LYNX_RED << "❌ Parsing failed. Exiting." << LYNX_RESET << "\n";
            return 1;
        }

        timeExecution("Step 2: Analyzing Source", [&]() {
            lynx.analyzeSource();
            return 0;
        });

        timeExecution("Step 3: Generating IR", [&]() {
            lynx.generateIR();
            return 0;
        });

        timeExecution("Step 4: Linking", [&]() {
            lynx.linking();
            return 0;
        });

        timeExecution("Step 5: Perform LTO optimization", [&]() {
            lynx.performLTO();
            return 0;
        });
        
        exitCode = timeExecution("Step 6: Executing JIT", [&]() {
            return lynx.executeJIT();
        });

        std::cout << LYNX_CYAN << "✔ JIT Execution finished" << LYNX_RESET << "\n";
    }

    auto total_end = std::chrono::steady_clock::now();
    auto total_diff = std::chrono::duration_cast<std::chrono::milliseconds>(total_end - total_start).count();

    std::cout << LYNX_BLUE;
    std::cout << "---------------------------------------------" << "\n";
    std::cout << "🕒 Total Elapsed Time: " << total_diff << " ms" << "\n";
    std::cout << "********* End Program Execution *********" << "\n";
    std::cout << separator << "\n";

    llvm::llvm_shutdown();
    return exitCode;
}

