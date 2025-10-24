#ifndef LYNX_LIB_RUNTIME_COLLECTION_MODULE_HPP
#define LYNX_LIB_RUNTIME_COLLECTION_MODULE_HPP


#include <string>
#include <RuntimeModule.hpp>

namespace LynxLibRuntime {

    class CollectionModule : public RuntimeModule {

        public:
        
            void registerFunctions(RuntimeFunctionRegistry& registry) override;

            void registerClasses(RuntimeClassRegistry& registry) override;

            std::string name() const override { return "collections"; }
    };
}

#endif
