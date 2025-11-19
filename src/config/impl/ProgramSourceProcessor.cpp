
#include "config/ProgramSourceProcessor.hpp"
#include "config/ProgramOptionConstant.hpp"

#include <logger/Logger.hpp>
#include <context/AstContext.hpp>
#include <exceptions/BaseException.hpp>
#include <llvm/IR/Verifier.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/ErrorHandling.h>
#include <llvm/Transforms/Utils/Cloning.h>

using namespace LynxExceptions;
using namespace LynxContext;
using namespace LynxLogger;

namespace LynxProgramConfig {

    void ProgramSourceProcessor::load(const std::string& buildDir,
        const std::string& entrySource, 
        const std::vector<std::string>& sourceFolders) {            
        this->sourceFolders = sourceFolders;
        this->entryFile = entrySource;
        this->buildDir = buildDir;
    }

    void ProgramSourceProcessor::parseSources() {

        std::ifstream entryStream(entryFile);
        if (!entryStream.is_open()) {
            LOG_ERROR("Failed to open the entry file: {}", entryFile);
            exitCode = 1;
            return;
        }

        
        driver->setSourceFolders(sourceFolders);

        parsedFiles.insert(entryFile);

        driver->setInputStream(entryStream);

        parseStream(entryFile);

        auto importsCopy = driver->getImports();

        for (const auto& importName : importsCopy) {

            LOG_WARN("importName parsed: {}", importName);

            if (importName.empty()) continue; 

            if (parsedFiles.count(importName)) {
                LOG_WARN("Already parsed: {}", importName);
                driver->removeImport(importName);
                continue;
            }

            parseFileRecursive(importName);
            driver->removeImport(importName);
        }

    }

    void ProgramSourceProcessor::parseFileRecursive(const std::string& filePath) {

        if (parsedFiles.contains(filePath)) {
            LOG_WARN("Already parsed: {}", filePath);
            return;
        }
    
        if (moduleAstMap.contains(filePath)) {
            LOG_ERROR("Cycle detected: {}", filePath);
            return; // Prevent infinite recursion
        }
    
        std::string moduleName = deriveModuleName(filePath, sourceFolders);
        driver->setCurrentModuleName(moduleName);
    
        std::ifstream file(filePath);
        if (!file.is_open()) {
            LOG_ERROR("Failed to open file: {}", filePath);
            exitCode = 1;
            return;
        }
    
        driver->setInputStream(file);
        parseStream(filePath);
        parsedFiles.insert(filePath);

        auto importsCopy = driver->getImports();
        for (const auto& importName : importsCopy) {

            LOG_ERROR("resolvedFile: {}", importName);

            if (importName.empty()) continue; 

            if (parsedFiles.count(importName)) {
                LOG_ERROR("Already parsed: {}", importName);
                driver->removeImport(importName);
                continue;
            }

            parseFileRecursive(importName);

            driver->removeImport(importName);

        }
    }  
   
    std::string ProgramSourceProcessor::deriveModuleName(const std::string& filePath, const std::vector<std::string>& sourceFolders) {
        fs::path absoluteFilePath = fs::absolute(filePath);
    
        for (const auto& folder : sourceFolders) {
            fs::path absoluteRootPath = fs::absolute(folder);
    
            if (std::mismatch(absoluteRootPath.begin(), absoluteRootPath.end(), absoluteFilePath.begin()).first == absoluteRootPath.end()) {
                fs::path relative = fs::relative(absoluteFilePath, absoluteRootPath);
                relative.replace_extension();
                return (absoluteRootPath.filename() / relative).generic_string();
            }
        }
    
        fs::path fallback = absoluteFilePath.filename();
        fallback.replace_extension();
        return fallback.string();
    }

    void ProgramSourceProcessor::parseStream(const std::string& streamName) {
        try {
            std::cout << "[ParseStream]: " << streamName << std::endl;
            driver->parse();                  // build the AST
            auto parsedAst = driver->takeAstRoot();
            if (parsedAst) {
                moduleAstMap[streamName] = std::move(parsedAst);
            }
        } catch (const char* errorMessage) {
            LOG_ERROR("Error occurred while parsing: {}", errorMessage);  
        }
    }

    std::vector<std::string>& ProgramSourceProcessor::getUserDefinedClasses() {
        static std::vector<std::string> userClasses = { "chatuser" };
        return userClasses;    
    }


    int ProgramSourceProcessor::getExitCode() const {
        return exitCode;
    }
}
