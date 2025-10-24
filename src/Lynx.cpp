#include "Lynx.hpp"
#include "JITFactory.hpp"
#include "JITConstants.hpp"
#include <lto/FullLTO.hpp>
#include <core/interop/InteropManager.hpp>
#include <core/memory/MemoryManager.hpp>

using namespace LynxCore;

void Lynx::initializeLLVM() {
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    std::cout << "[Lynx] LLVM Initialized.\n";
}

int Lynx::loadSources() {
    processor = std::make_unique<ProgramSourceProcessor>();
    processor->load(config.buildDir, config.entrySource, config.sourceFolders);
    processor->parseSources();

    int exitCode = processor->getExitCode();
    if (exitCode != 0) {
        std::cerr << "[Lynx] Source parsing failed with exit code: " << exitCode << "\n";
    }
    return exitCode;
}

int Lynx::parseSource() {
    initializeLLVM();
    return loadSources();
}


void Lynx::analyzeSource() {

    if (!processor) {
        std::cerr << "[Lynx] Error: Processor not initialized before IR generation.\n";
        return;
    }

    semanticAnalyzer = std::make_unique<SemanticAnalyzer>();
    driver = std::move(processor->getDriver());
    auto& analyzerNodes = driver->getAllAnalyzerNodes();
    for (auto node : *analyzerNodes) {
        std::cout << "[Node] Type: " << parseNodeTypeToString(node->getNodeType()) << std::endl;
        semanticAnalyzer->analyzeASTParallel(node);
    }
    
    std::cout << "[Lynx] Semantic analysis completed.\n";
}

void Lynx::generateIR() {
    if (!processor) {
        std::cerr << "[Lynx] Error: Processor not initialized before IR generation.\n";
        return;
    }

    const auto& moduleAstMap = processor->getModuleAstMap();
    irGenerator = std::make_unique<IRGenerator>(
        config.rootPath,
        config.buildDir,
        config.sourceFolders,
        std::move(driver),
        moduleAstMap
    );

    irGenerator->execute();

    ltoFacade = std::make_unique<LTOFacade>(
        irGenerator->getContext(),
        std::make_unique<FullLTO>()
    );

    std::cout << "[Lynx] IR generation completed.\n";
}

void Lynx::linking() {
    if (!irGenerator) {
        std::cerr << "[Lynx] Error: Source processor not available for linking.\n";
        return;
    }

    auto linkerModules = irGenerator->takeLinkerModules();

    linker = std::make_unique<Linker>(config.buildDir, config.entrySource, std::move(linkerModules));
    linker->execute();

    std::cout << "[Lynx] Linking completed.\n";
}

void Lynx::performLTO() {
    if (!linker) {
        std::cerr << "[Lynx] Error: Linker not available for LTO.\n";
        return;
    }

    auto finalModule = linker->takeLinkedModule();

    if (!finalModule) {
        std::cerr << "[Lynx] Error: No module available for LTO optimization.\n";
        return;
    }

    if (!ltoFacade) {
        std::cerr << "[Lynx] Error: LTOFacade not initialized.\n";
        return;
    }

    ltoFacade->optimize(*finalModule);

    // Optionally emit the optimized bitcode for inspection/debugging
    ltoFacade->emitBitcode(*finalModule, config.buildDir + "/optimized.bc");
    ltoFacade->emitAssembly(*finalModule, config.buildDir + "/optimized.s");

    // Put back the optimized module into the linker for later use
    linker->setLinkedModule(std::move(finalModule));

    std::cout << "[Lynx] LTO optimization completed.\n";
}

int Lynx::executeJIT() {
    
    if (!linker) {
        std::cerr << "[Lynx] Error: Linker is not available for JIT .\n";
        return -1;
    }

    InteropManager::initialize();
    
    auto finalModule = linker->takeLinkedModule();
    excutor = JITFactory::create(JITType::MCJIT);
    excutor->initialize(std::move(finalModule));
    int result = excutor->execute();

    MemoryManager::reportLeaks();

    return result;
}
