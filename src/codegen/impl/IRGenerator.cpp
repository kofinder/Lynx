#include "IRGenerator.hpp"
#include "IRPlanner.hpp"
#include <exceptions/BaseException.hpp>
#include <context/GlobalSymbolContext.hpp>
#include "llvm/IR/Verifier.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include <llvm/Target/TargetMachine.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/Host.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/ErrorHandling.h>
#include <logger/Logger.hpp>

using namespace LynxLogger;
using namespace LynxExceptions;
using namespace LynxContext;

namespace LynxCodegen {

    llvm::LLVMContext& IRGenerator::getContext() {
        if (!sharedContext) {
            sharedContext = std::make_shared<llvm::LLVMContext>();
        }
        return *sharedContext;
    }


    void IRGenerator::execute() {
        try {
            IRPlanner planner(sourceFolders, moduleAstMap);
            planner.planModuleOrder();
            const auto& orderedModules = planner.getOrderedModules();

            if (!sharedContext) {
                sharedContext = std::make_shared<llvm::LLVMContext>();
            }

            auto sharedTypes = std::make_shared<std::map<std::string, std::shared_ptr<BaseType>>>();

            generateModulesIR(orderedModules, sharedTypes);

        } catch (BaseException *e) {
            LOG_ERROR("Exception: {}", e->getMessage());
        } catch (const std::exception& e) {
            LOG_ERROR("Standard exception: {}", e.what());
        } catch (...) {
            LOG_ERROR("Unknown exception in generateAbstractSyntax");
        }
    }

    void IRGenerator::generateModulesIR(const std::vector<std::string>& orderedModules, 
        std::shared_ptr<std::map<std::string, std::shared_ptr<BaseType>>> sharedTypes) {

        for (const auto& file : orderedModules) {

            auto astIt = moduleAstMap.find(file);
            if (astIt == moduleAstMap.end()) {
                LOG_WARN("No AST found for file: {}", file);
                continue;
            }
    
            LOG_INFO("Generate IR file: {}", file);
    
            auto& ast = astIt->second;
    
            fs::path importPath(file);
            std::string moduleName = importPath.stem().string();
    
            auto astCtx = std::make_shared<AstContext>(
                moduleName,
                sharedContext,
                driver->getGlobalContext(),
                sharedTypes
            );
    
            auto* module = astCtx->getModule();
            ast->startCodeGen(astCtx);
            setupTargetMachine(module);

            if (llvm::verifyModule(*module, &llvm::errs())) {
                LOG_ERROR("Invalid LLVM IR in module '{}'", moduleName);
                module->print(llvm::errs(), nullptr);
                throw std::runtime_error("Aborting IR generation due to invalid IR.");
            }

            writeIRFile(importPath, moduleName, module);

            auto cloned = llvm::CloneModule(*module);
            if (!cloned) {
                throw std::runtime_error("CloneModule failed!");
            }

            llvmModules[file] = std::move(cloned);
        }       
    }


    void IRGenerator::setupTargetMachine(llvm::Module* module) {
        // Set the target triple for the module
        auto targetTriple = llvm::sys::getDefaultTargetTriple();
        module->setTargetTriple(targetTriple);

        // Initialize target registry and obtain target
        std::string error;
        const llvm::Target* target = llvm::TargetRegistry::lookupTarget(targetTriple, error);
        if (!target) {
            throw std::runtime_error("Failed to initialize target: " + error);
        }

        // Create a TargetMachine for the target
        llvm::TargetOptions opt;
        std::string cpu = "generic"; // Specify a generic CPU for simplicity
        std::string features = "";   // No additional CPU features
        auto targetMachine = std::unique_ptr<llvm::TargetMachine>(
            target->createTargetMachine(targetTriple, cpu, features, opt, llvm::Reloc::PIC_)
        );

        targetMachine = std::unique_ptr<llvm::TargetMachine>(
            target->createTargetMachine(targetTriple, cpu, features, opt, llvm::Reloc::PIC_)
        );

        // Set the DataLayout for the module
        module->setDataLayout(targetMachine->createDataLayout());
    }

    void IRGenerator::writeIRFile(const fs::path& outputPath, const std::string& irFile, llvm::Module* module) {
        fs::path relative   = fs::relative(outputPath, rootPath);
        fs::path buildPath  = fs::path(buildDir) / relative;
        fs::path folder     = buildPath.parent_path();
        fs::path llFilePath = folder / (irFile + ".ll");

        std::error_code ec;
        if (!fs::exists(folder)) {
            if (!fs::create_directories(folder)) {
                std::cerr << "Failed to create directory: " << folder << " — " << ec.message() << std::endl;
            } 
        } else {
            LOG_INFO("Directory already exists");
        }
    
        llvm::raw_fd_ostream outLL(llFilePath.string(), ec);
        if (ec) {
            std::cerr << "Failed to write to file: " << llFilePath << " — " << ec.message() << std::endl;
            return;
        }
    
        module->print(outLL, nullptr);
    }
}
