#include "Linker.hpp"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <logger/Logger.hpp>
#include <llvm/Linker/Linker.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Transforms/Utils/Cloning.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>


namespace fs = boost::filesystem;

namespace LynxLinker {

    using namespace LynxLogger;

    void Linker::setLinkedModule(std::unique_ptr<llvm::Module> finalModule) {
        mainModule = std::move(finalModule);
    }
 
    std::unique_ptr<llvm::Module> Linker::takeLinkedModule() {
        return std::move(mainModule);
    }

    void Linker::execute() {

        if (!linkerModules.contains(entryFile)) {
            throw std::runtime_error("Entry file not found in llvmModules: " + entryFile);
        }

        auto& mainMod = linkerModules[entryFile];
        if (!mainMod) {
            throw std::runtime_error("Main module is null");
        }

        mainModule = llvm::CloneModule(*mainMod);
        if (!mainModule) {
            throw std::runtime_error("Failed to clone main module for final linking");
        }

        llvm::Linker linker(*mainModule);

        for (const auto& [file, mod] : linkerModules) {
            if (file == entryFile) continue;

            if (!mod) {
                throw std::runtime_error("Null module found for file: " + file);
            }

            auto cloned = llvm::CloneModule(*mod);
            if (!linker.linkInModule(std::move(cloned))) {
                std::cout << "[Linked] " << file << std::endl;
            } else {
                throw std::runtime_error("Failed to link module: " + file);
            }
        }

        if (llvm::verifyModule(*mainModule, &llvm::errs())) {
            throw std::runtime_error("Linked module verification failed");
        }

        const std::string linkerPath = (fs::path(buildDir) / "linked.ll").string();
        writeToFile(linkerPath);
    }

    void Linker::dumpMainModuleIR(const std::string& outputFile) const {
        writeIR(outputFile, llvm::sys::fs::OF_Text);
    }

    void Linker::writeToFile(const std::string& outputPath) const {
        writeIR(outputPath, llvm::sys::fs::OF_Text);
    }

    void Linker::writeIR(const std::string& path, llvm::sys::fs::OpenFlags flags) const {
        if (!mainModule) {
            llvm::errs() << "[Linker] Cannot write IR. Main module is null.\n";
            return;
        }

        std::error_code EC;
        llvm::raw_fd_ostream out(path, EC, flags);
        if (EC) {
            llvm::errs() << "[Linker] Error writing IR: " << EC.message() << "\n";
            return;
        }

      //  mainModule->print(out, nullptr);
    }

}
