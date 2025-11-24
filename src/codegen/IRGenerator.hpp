/**
 * @file IRGenerator.hpp
 * @brief Declares the IRGenerator class for generating LLVM IR from Lynx ASTs.
 * 
 * The IRGenerator class is responsible for translating parsed Lynx AST nodes
 * into LLVM Intermediate Representation (IR). It manages module-level LLVM
 * contexts, target machine configuration, and IR serialization to disk.
 * 
 * **Key Responsibilities:**
 * - Maintains LLVMContext for IR generation.
 * - Converts AST modules into LLVM IR modules.
 * - Configures target machine and data layout for each LLVM module.
 * - Serializes LLVM IR to files in the specified build directory.
 * - Provides access to LLVM modules for downstream linking or JIT execution.
 * 
 * **Used By:**
 * - The Lynx compiler frontend for code generation.
 * - Linker and JIT engines for executing generated code.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_IR_GENERATOR_HPP
#define LYNX_IR_GENERATOR_HPP

#include <string>
#include <unordered_map>
#include <memory>

#include <boost/filesystem.hpp>
#include <llvm/IR/Value.h>
#include <llvm/IR/LLVMContext.h>

#include <ast/Node.hpp>
#include <lang/LynxDriver.hpp>
#include <config/ProgramSourceProcessor.hpp>

namespace fs = boost::filesystem;

namespace LynxCodegen {

    using namespace LynxAst;
    using namespace LynxLang;
    
    class IRGenerator {

        private:

            std::string rootPath;
            std::string buildDir;
            std::vector<std::string> sourceFolders;

            std::unique_ptr<LynxDriver> driver;
            std::shared_ptr<llvm::LLVMContext> sharedContext;

            const std::unordered_map<std::string, std::unique_ptr<Node>>& moduleAstMap;
            std::unordered_map<std::string, std::unique_ptr<llvm::Module>> llvmModules;

        private:
        
            /**
             * @brief Configure target machine and data layout for the given module.
             * 
             * @param module LLVM module to configure.
             */
            void setupTargetMachine(llvm::Module* module);

            /**
             * @brief Serialize and save the LLVM IR to a file.
             * 
             * @param outputPath Directory where file should be written.
             * @param irFile Filename (typically "out.ll").
             * @param module LLVM module to write.
             */
            void writeIRFile(const fs::path& outputPath, const std::string& irFile, llvm::Module* module);

            /**
             * @brief Generate IR for each module in the specified order.
             * 
             * @param orderedModules List of modules sorted topologically.
             */
            void generateModulesIR(
                const std::vector<std::string>& orderedModules, 
                std::shared_ptr<std::map<std::string, std::shared_ptr<BaseType>>> sharedTypes
            );

        public:

            /**
             * @brief Construct an IRGenerator for a given AST map and build context.
             * 
             * @param astModuleMap Reference to the map of parsed AST modules.
             * @param rootPath Project root path.
             * @param buildDir Directory for generated IR files.
             */
            IRGenerator(
                const std::string& rootPath,
                const std::string& buildDir,
                std::vector<std::string> folders,
                std::unique_ptr<LynxDriver> _driver,
                const std::unordered_map<std::string, std::unique_ptr<Node>>& astModuleMap
            ) : moduleAstMap(astModuleMap), 
                driver(std::move(_driver)), 
                sourceFolders(folders), 
                rootPath(rootPath), 
                buildDir(buildDir) {}

            /**
             * @brief Get the LLVMContext used by this IRGenerator.
             * @return Reference to the LLVMContext instance.
             */
            llvm::LLVMContext& getContext();
            
            /**
             * @brief Orchestrates full IR generation for all modules.
             * 
             * This is the main entry point that prepares LLVM modules from the full parsed AST.
             */
            void execute();

            /**
             * @brief Transfer ownership of LLVM modules.
             *
             * This function moves the internal map of LLVM modules out of this object,
             * effectively transferring ownership to the caller. After this call, the
             * internal map will be empty.
             *
             * Use this when you want to pass the modules to the linker or take full
             * ownership for further processing.
             *
             * @return An unordered_map containing the LLVM modules mapped by their names.
             */
            std::unordered_map<std::string, std::unique_ptr<llvm::Module>> takeLinkerModules() {
                return std::move(llvmModules);
            }

            /**
             * @brief Access the LLVM modules without transferring ownership.
             *
             * Provides read/write access to the internal map of LLVM modules. Modifying
             * the returned map will affect the internal state of this object.
             *
             * Use this when you need to inspect or modify the modules in-place.
             *
             * @return A reference to the internal unordered_map of LLVM modules.
             */
            std::unordered_map<std::string, std::unique_ptr<llvm::Module>>& getLinkerModules() {
                return llvmModules;
            }
            
            /**
             * @brief Destructor for IRGenerator.
             */
            ~IRGenerator() = default;
    };

}

#endif
