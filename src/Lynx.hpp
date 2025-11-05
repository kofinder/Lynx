/**
 * @file Lynx.hpp
 * @brief Defines the `Lynx` class, the main driver for compiling and executing Lynx programs.
 *
 * The `Lynx` class orchestrates the full compilation and execution pipeline, including
 * source processing, semantic analysis, LLVM IR generation, linking, LTO optimization,
 * and JIT execution. It also manages system module initialization and integrates
 * with the compiler's backend and runtime components.
 *
 * **Key Responsibilities:**
 * - Initialize LLVM targets and system modules.
 * - Load and parse program source files.
 * - Perform semantic analysis on parsed ASTs.
 * - Generate and manage LLVM IR modules.
 * - Link modules and optionally perform LTO optimizations.
 * - Execute programs using JIT compilation.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_HPP
#define LYNX_HPP

#include <iostream>
#include <fstream>
#include <jit/IJIT.hpp>
#include <lto/LTOFacade.hpp>
#include <linker/Linker.hpp>
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

        bool systemModulesInitialized = false;

        /**
         * @brief Initialize LLVM targets and necessary backend support.
         */
        void initializeLLVM();

        /**
         * @brief Load and parse sources using processor.
         * @return int Exit code (0 for success).
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
         * @brief Parse the source files and prepare internal representation.
         * @return Exit code from source processor (0 for success).
         */
        int parseSource();

        /**
         * @brief Perform semantic analysis on the parsed ASTs.
         */
        void analyzeSource();

        /**
         * @brief Generate LLVM IR from parsed source.
         */
        void generateIR();

        /**
         * @brief Link all LLVM modules together into a final unit.
         */
        void linking();

          /**
         * @brief Link all LLVM modules together into a final unit.
         */
        void performLTO();

        /**
         * @brief Execute the program via JIT compilation.
         * @return Program exit code.
         */
        int executeJIT();

        ~Lynx() { SystemModuleLoader::shutdown(); }
};

#endif