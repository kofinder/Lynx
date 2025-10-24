#include <chrono>
#include <iostream>
#include <fstream>
#include "Lynx.hpp"
#include <llvm/Support/ManagedStatic.h>

template<typename Func>
auto timeExecution(const std::string& label, Func&& func) {
    auto start = std::chrono::steady_clock::now();
    std::cout << LYNX_GREEN << "[Executing] " << label << "..." << LYNX_RESET << std::endl;
    
    auto result = func();

    auto end = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << LYNX_CYAN << "⏱ " << label << " took " << ms << " ms" << LYNX_RESET << std::endl;
    return result;
}


int main(int argc, char const *argv[]) {

    auto total_start = std::chrono::steady_clock::now();
    std::string separator = std::string(LYNX_RED) + LYNX_SEPARATOR() + std::string(LYNX_RESET);

    std::cout << separator << std::endl << LYNX_BLUE;
    std::cout << "********* Begin Program Execution *********" << LYNX_CYAN << std::endl;
    
    auto& config = ProgramOptionConfig::instance();
    config.initialize(argc, argv);
    const std::string& cmd = config.getCommand();

    int exitCode = 0;
    {
        Lynx lynx(config);

        int parseStatus = timeExecution("Step 1: Parsing Sources", [&]() {
            return lynx.parseSource();
        });

        if (parseStatus != 0) {
            std::cerr << LYNX_RED << "❌ Parsing failed. Exiting." << LYNX_RESET << std::endl;
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

        std::cout << LYNX_CYAN << "✔ JIT Execution finished" << LYNX_RESET << std::endl;
    }

    auto total_end = std::chrono::steady_clock::now();
    auto total_diff = std::chrono::duration_cast<std::chrono::milliseconds>(total_end - total_start).count();

    std::cout << LYNX_BLUE;
    std::cout << "---------------------------------------------" << std::endl;
    std::cout << "🕒 Total Elapsed Time: " << total_diff << " ms" << std::endl;
    std::cout << "********* End Program Execution *********" << std::endl;
    std::cout << separator << std::endl;

    llvm::llvm_shutdown();
    return exitCode;
}

