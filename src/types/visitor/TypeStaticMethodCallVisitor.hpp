#ifndef LYNX_TYPE_STATIC_METHOD_VISITOR_HPP
#define LYNX_TYPE_STATIC_METHOD_VISITOR_HPP

#include <logger/Logger.hpp>
#include "TypeVisitor.hpp"
#include <types/interfaces/BaseType.hpp>
#include "TypeStaticMethodRegistry.hpp"

namespace LynxTypes {

    class TypeStaticMethodCallVisitor : public TypeVisitor {

        private:

            TypeStaticMethodRegistry& registry;

        private:

            void registerFromType(BaseType* type) {
                const auto& methods = type->getInstanceMethodRegistry();
                std::string typeName = type->getDebugName();
                for(auto [methodName, count]: methods) {
                    registry.registerMethod(typeName, methodName, 
                        [type, methodName] (const std::vector<llvm::Value*>& args) -> llvm::Value* {
                            return type->codegenStaticMethod(methodName, args); 
                        }
                    );     
                }
            }
        
        public:

            TypeStaticMethodCallVisitor(TypeStaticMethodRegistry& modReg): registry(modReg) {}

            void visit(ByteType& type) override { registerFromType(&type); }

            void visit(ShortType& type) override { registerFromType(&type); }

            void visit(IntegerType& type) override { registerFromType(&type); }

            void visit(LongType& type) override { registerFromType(&type); }

            void visit(FloatType& type) override { registerFromType(&type); }

            void visit(DoubleType& type) override  { registerFromType(&type); }

            void visit(BooleanType& type) override { registerFromType(&type); }

            void visit(CharType& type) override { registerFromType(&type); }

            void visit(StringType& type) override { registerFromType(&type); }

            ~TypeStaticMethodCallVisitor() override = default;
    };
}

#endif 
