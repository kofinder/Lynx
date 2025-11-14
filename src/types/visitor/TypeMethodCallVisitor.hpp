#ifndef LYNX_TYPE_METHOD_VISITOR_HPP
#define LYNX_TYPE_METHOD_VISITOR_HPP

#include <logger/Logger.hpp>
#include "TypeVisitor.hpp"
#include <llvm/IR/Value.h>
#include "TypeMethodRegistry.hpp"
#include "interfaces/BaseType.hpp"
#include "TypeMethodSupport.hpp"

namespace LynxTypes {

    struct TypeMethodCallVisitor : public TypeVisitor {

        private:

            std::string methodName;
            llvm::Value* instance = nullptr;
            const std::vector<llvm::Value*>& argValuesRef;
        
        public:

            llvm::Value* result = nullptr;

        private:

            template<typename T>
            void dispatch(T& type) {
                if constexpr (MethodCapable<T>) {
                    result = codegenMethod(type, instance, methodName, argValuesRef);
                } else {
                    std::cerr << "This type has no static method support \n";
                    result = nullptr;
                }
            }

        public:

            TypeMethodCallVisitor(
                const std::string& name, 
                const std::vector<llvm::Value*>& argValues
            ): methodName(std::move(name)), argValuesRef(std::move(argValues)) {}

            TypeMethodCallVisitor(
                std::string name, 
                llvm::Value* inst,
                const std::vector<llvm::Value*>& argValues
            ) : methodName(std::move(name)), instance(inst), argValuesRef(argValues) {}    

            void visit(ByteType& type) override { dispatch(type); }
            void visit(ShortType& type) override { dispatch(type); }
            void visit(IntegerType& type) override { dispatch(type); }
            void visit(LongType& type) override { dispatch(type); }
            void visit(FloatType& type) override { dispatch(type); }
            void visit(DoubleType& type) override { dispatch(type); }
            void visit(BooleanType& type) override { dispatch(type); }
            void visit(CharType& type) override { dispatch(type); }
            void visit(StringType& type) override { dispatch(type); } 

            ~TypeMethodCallVisitor() override = default;
    };
}

#endif 
