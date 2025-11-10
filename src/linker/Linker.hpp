/**
 * @file Linker.hpp
 * @brief LLVM-based module linker for Lynx language projects.
 *
 * This class handles linking multiple LLVM modules, writing LLVM IR to files,
 * and managing the main module of the compilation unit. It encapsulates
 * the logic for merging modules, dumping IR, and finalizing linked output.
 *
 * * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_LINKER_HPP
#define LYNX_LINKER_HPP

#include <llvm/IR/Module.h>
#include <llvm/Linker/Linker.h>

namespace LynxLinker {

    class Linker {

        private:

            std::string entryFile;

            std::string buildDir;

            std::unique_ptr<llvm::Module> mainModule;

            std::unordered_map<std::string, std::unique_ptr<llvm::Module>> linkerModules;

        private:
        
            /**
             * @brief Writes LLVM IR to the specified path using given file system flags.
             * @param path The output file path.
             * @param flags File open mode flags.
            */
            void writeIR(const std::string& path, llvm::sys::fs::OpenFlags flags) const;

            /**
             * @brief Save the main module's IR to a file (LLVM Assembly format).
             */
            void dumpMainModuleIR(const std::string& outputFile) const;

            /**
             * @brief Write the final linked module to the specified file.
             */
            void writeToFile(const std::string& outputPath) const;

        public:

            Linker(
                const std::string& outputPath,
                const std::string& entrySource,
                std::unordered_map<std::string, std::unique_ptr<llvm::Module>>&& modules
            ) : linkerModules(std::move(modules)), buildDir(outputPath), entryFile(entrySource) {}

            /**
             * @brief Performs the linking process of all modules.
             */

            void execute();

            /**
             * @brief Returns ownership of the linked main module.
             * @return A unique pointer to the final linked module.
            */
            std::unique_ptr<llvm::Module> takeLinkedModule();

            /**
             * @brief Set or replace the current linked module.
             * @param finalModule New linked module to store.
             */
            void setLinkedModule(std::unique_ptr<llvm::Module> finalModule);

            ~Linker() = default;
    };

}
#endif