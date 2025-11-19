#include "FilesystemModule.hpp"
#include "path/PathFunctions.hpp"
#include "util/FSUtilFunctions.hpp"
#include "file/FileFunctions.hpp"
#include "file/FileClass.hpp"
#include "directory/DirectoryFunctions.hpp"

namespace LynxRuntime {

    void FilesystemModule::registerFunctions(RuntimeFunctionRegistry& registry) {

        registerFileFunctions(registry);

        registerPathFunctions(registry);

        registerDirectoryFunctions(registry);

        registerUtilFunctions(registry);
            
    }

    void FilesystemModule::registerClasses(RuntimeClassRegistry& registry) {
        auto runtimeRegistry = RuntimeFunctionRegistry::getInstance();
        
        auto fileClass = std::make_shared<FileClass>();
        fileClass->registerMethods(runtimeRegistry);
        registry.registerClass("File", fileClass);
        
    }
}