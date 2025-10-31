#ifndef LYNX_IR_GENERATOR_HPP
#define LYNX_IR_GENERATOR_HPP

/**
 * @brief Responsible for generating LLVM IR from the AST.
 * 
 * The IRGenerator translates high-level AST representations of the Lynx language
 * into LLVM Intermediate Representation (IR) modules. It handles module ordering,
 * target machine setup, and writing IR to file.
*/

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
             * @brief Get the map of module names to their corresponding LLVM modules.
             *
             * Provides read-only access to the LLVM IR modules generated for each parsed source file.
             * Useful for inspection, analysis, or linking operations.
             *
             * @return A const map from module names to their LLVM module objects.
             */
            std::unordered_map<std::string, std::unique_ptr<llvm::Module>> takeLinkerModules() { return std::move(llvmModules); }

            /**
             * @brief Destructor for IRGenerator.
             */
            ~IRGenerator() = default;
    };

}

#endif
