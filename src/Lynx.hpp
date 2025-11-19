/**
 * @file Lynx.hpp
 * @brief Defines the `Lynx` class, the main driver for compiling and executing Lynx programs.
 *
 * The `Lynx` class orchestrates the full compilation and execution pipeline, including
 * source processing, semantic analysis, LLVM IR generation, linking, LTO optimization,
 * and JIT execution. It also manages system module initialization and integrates
 * with the compiler's backend and runtime components.
 *
 * **Compilation & Execution Pipeline:**
 *
 * 1. **Initialize LLVM** (`initializeLLVM()`)
 *    - Set up native target, ASM printer, and parser for LLVM backend.
 *
 * 2. **Load Sources** (`loadSources()`)
 *    - Scan entry source and source folders.
 *    - Load raw source files into the `ProgramSourceProcessor`.
 *    - Perform lexical and syntax parsing.
 *
 * 3. **Parse Sources** (`parseSource()`)
 *    - Wrapper to call LLVM initialization + load sources.
 *
 * 4. **Semantic Analysis** (`analyzeSource()`)
 *    - Type checking, symbol resolution, AST validation.
 *    - Executed in parallel over all AST nodes.
 *
 * 5. **Generate IR** (`generateIR()`)
 *    - Initialize `CoreManager` (MemoryManager / GC, ThreadManager, Scheduler, Interop, RuntimeBindings).
 *    - Convert AST → LLVM IR modules via `IRGenerator`.
 *    - Apply runtime bindings and GC setup for user-defined classes.
 *
 * 6. **Linking** (`linking()`)
 *    - Merge all IR modules into a single LLVM module.
 *    - Resolve references between modules.
 *
 * 7. **Perform LTO** (`performLTO()`)
 *    - Optional Link-Time Optimization on final module.
 *    - Emit optimized bitcode / assembly for inspection.
 *
 * 8. **Execute JIT** (`executeJIT()`)
 *    - Initialize JIT engine and execute compiled code.
 *    - Runtime memory allocation and GC handled by `CoreManager`/`MemoryManager`.
 *    - Dashboard monitoring active.
 *    - Report memory leaks and clean shutdown.
 *
 * 9. **Shutdown** (`~Lynx()`)
 *    - Stop threads, scheduler, dashboard.
 *    - Release runtime resources.
 *
 * **Note on CoreManager / GC:**
 * - `CoreManager` is initialized **after IR generation** but **before JIT execution**.
 * - This ensures runtime objects, GC, and threads are only active when the program is actually executed.
 *
 * **Key Responsibilities:**
 * - Initialize LLVM targets and system modules.
 * - Load and parse program source files.
 * - Perform semantic analysis on parsed ASTs.
 * - Generate and manage LLVM IR modules.
 * - Link modules and optionally perform LTO optimizations.
 * - Execute programs using JIT compilation.
 * - Manage runtime subsystems (GC, threads, scheduler, interop, runtime bindings).
 *
 * @author: Ko Thein
 * @date: November 2, 2024
*/


#ifndef LYNX_HPP
#define LYNX_HPP

#include <iostream>
#include <fstream>
#include <jit/IJIT.hpp>
#include <lto/LTOFacade.hpp>
#include <linker/Linker.hpp>
#include <core/CoreManager.hpp>
#include <codegen/IRGenerator.hpp>
#include <boost/program_options.hpp>
#include <llvm/Support/TargetSelect.h>
#include <system/SystemModuleLoader.hpp>
#include <boost/program_options/errors.hpp>
#include <config/ProgramTerminalColor.hpp>
#include <config/ProgramOptionConfig.hpp>
#include <config/ProgramSourceProcessor.hpp>
#include <analyzer/interfaces/SemanticAnalyzer.hpp>

using namespace LynxJIT;
using namespace LynxLTO;
using namespace LynxLinker;
using namespace LynxAnalyzer;
using namespace LynxCodegen;
using namespace LynxSystem;
using namespace LynxCore;
using namespace LynxProgramConfig;

namespace po = boost::program_options;

class Lynx {

    private:
    
        ProgramOptionConfig& config;

        std::unique_ptr<ProgramSourceProcessor> processor;

        std::unique_ptr<IRGenerator> irGenerator;

        std::unique_ptr<Linker> linker;
        
        std::unique_ptr<IJIT> excutor;

        std::unique_ptr<LTOFacade> ltoFacade;

        std::unique_ptr<LynxDriver> driver;

        std::unique_ptr<SemanticAnalyzer> semanticAnalyzer;

        std::unique_ptr<CoreManager> coreManager; // what about t his way?

        bool systemModulesInitialized = false;


        /**
         * @brief Load and parse all source files for the project.
         *
         * Responsibilities:
         *  - Load entry source file and all source folders
         *  - Parse into internal AST representations
         *  - Prepare for semantic analysis
         *
         * @return int Exit code (0 = success)
        */
        int loadSources();

    public:
    
        explicit Lynx(ProgramOptionConfig& cfg) : config(cfg) {
            if (!systemModulesInitialized) {
                SystemModuleLoader::initializeCoreModules();
                systemModulesInitialized = true;
            }    
        }

        /**
         * @brief Initialize LLVM targets (native) and backend support.
         *
         * Must be called before IR generation or JIT compilation.
        */
        void initializeLLVM();

        /**
         * @brief Parse the source files into AST.
         *
         * Dependencies: LLVM initialization must be done.
         * Pipeline stage: Step 1 (after system modules & LLVM)
         *
         * @return int Exit code (0 = success)
        */
        int parseSource();

        /**
         * @brief Perform semantic analysis on parsed AST nodes.
         *
         * Responsibilities:
         *  - Validate types, variables, and scope rules
         *  - Populate driver for IR generation
         *
         * Pipeline stage: Step 2 (after parseSource)
        */
        void analyzeSource();

        /**
         * @brief Generate LLVM IR from parsed and analyzed sources.
         *
         * Responsibilities:
         *  - Initialize CoreManager (MemoryManager, GC, Threads, Scheduler, Interop, Bindings)
         *  - Build LLVM modules
         *  - Apply runtime bindings (including GC setup for user classes)
         *
         * Pipeline stage: Step 3 (after analyzeSource)
        */
        void generateIR();

        /**
         * @brief Link all LLVM modules into a single executable unit.
         *
         * Pipeline stage: Step 4 (after generateIR)
        */
        void linking();

        /**
         * @brief Perform LTO optimization on linked LLVM module.
         *
         * Pipeline stage: Step 5 (after linking)
        */
        void performLTO();

        /**
         * @brief Execute the compiled program via JIT.
         *
         * Responsibilities:
         *  - Initialize JIT engine
         *  - Run program
         *  - Report memory leaks via MemoryManager
         *  - Shutdown CoreManager subsystems
         *
         * Pipeline stage: Step 6 (after performLTO)
         *
         * @return int Program exit code
        */
        int executeJIT();

        /**
         * @brief Destructor.
         *
         * Ensures system modules are shut down after pipeline execution.
        */
        ~Lynx() { SystemModuleLoader::shutdown(); }
};

#endif