#include "CollectionModule.hpp"
#include "array/ArrayClass.hpp"
#include "array/ArrayFunctions.hpp"

namespace LynxLibRuntime {

    void CollectionModule::registerFunctions(RuntimeFunctionRegistry& registry) {
        registerArrayFunctions(registry);            
    }

    void CollectionModule::registerClasses(RuntimeClassRegistry& registry) {
        auto runtimeRegistry = RuntimeFunctionRegistry::getInstance();
        
        auto arrayClass = std::make_shared<ArrayClass>();
        arrayClass->registerMethods(runtimeRegistry);
        registry.registerClass("array", arrayClass);
    }
}