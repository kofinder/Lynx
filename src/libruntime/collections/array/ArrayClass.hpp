#ifndef LYNX_LIB_RUNTIME_ARRAY_CLASS_HPP
#define LYNX_LIB_RUNTIME_ARRAY_CLASS_HPP


#include <string>
#include <optional>
#include "RuntimeClass.hpp"

#include "includes/AtFunction.hpp"

#include <ast/LiteralNode.hpp>
#include <ast/PrimaryExpressionNode.hpp>


namespace LynxLibRuntime {

    class ArrayClass : public RuntimeClass {

        public:

            std::string getName() const override { return "array"; }
               
            void registerMethods(RuntimeFunctionRegistry& registry) override {
                auto atFn = std::make_shared<AtFunction>();
            
                registry.registerFunction("Arrays.at", atFn);
            
                methods["at"] = atFn;
            }

    };
}

#endif
