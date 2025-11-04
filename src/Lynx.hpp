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

        ~Lynx() {
            SystemModuleLoader::shutdown();
        }
        

};

#endif