/**
 * @file TypeMethodCallVisitor.hpp
 * @brief Implements a visitor for invoking methods on Lynx types via LLVM IR.
 *
 * This header defines `TypeMethodCallVisitor`, a concrete subclass of
 * `TypeVisitor`, designed to perform method calls on built-in Lynx types.
 * The visitor uses compile-time concepts (`MethodCapable`) to ensure that
 * only types supporting method emission can be dispatched.
 *
 * Key features:
 *  - Dispatches to `emitMethodCall` for types implementing `MethodCapable`.
 *  - Supports both instance and static methods.
 *  - Collects method results in the `result` member for further IR generation.
 *  - Provides type-safe, centralized method invocation for compiler backends.
 *
 * Benefits:
 *  - Reduces repetitive method call logic across different type implementations.
 *  - Ensures consistent LLVM IR generation for method calls.
 *  - Leverages compile-time checks to prevent invalid method dispatch.
 *
 * Usage:
 *  - Instantiate the visitor with a method name and argument list.
 *  - Pass the visitor to a type's `accept(TypeVisitor&)` method.
 *  - Retrieve the generated LLVM IR value from the `result` member.
 *
 * This design follows the Visitor pattern, allowing new operations to be
 * added without modifying the type classes themselves.
 *
 * @author Ko Thein (Nathan Mratt)
 * @date   November 2, 2024
*/

#ifndef LYNX_TYPE_METHOD_CALL_VISITOR_HPP
#define LYNX_TYPE_METHOD_CALL_VISITOR_HPP

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
            llvm::Value* instancePtr = nullptr;
            llvm::Value* result = nullptr;
            std::vector<llvm::Value*> argValues;
        
        private:

            template<typename T>
            void dispatch(T& type) {
                if constexpr (MethodCapable<T>) {
                    result = codegenMethod(type, instance, instancePtr, methodName, argValues);
                } else {
                    std::cerr << "This type has no static method support \n";
                    result = nullptr;
                }
            }

        public:

            TypeMethodCallVisitor(
                const std::string& name, 
                std::vector<llvm::Value*> args
            ) : methodName(name), argValues(std::move(args)) {}

            TypeMethodCallVisitor(
                std::string name, 
                llvm::Value* inst, 
                llvm::Value* instPtr, 
                std::vector<llvm::Value*> args
            ) : methodName(std::move(name)), instance(inst), instancePtr(instPtr), argValues(std::move(args)) {}

            [[nodiscard]] llvm::Value* getResult() const noexcept { return result; }

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
            TypeMethodCallVisitor(const TypeMethodCallVisitor&) = delete;
            TypeMethodCallVisitor& operator=(const TypeMethodCallVisitor&) = delete;
            TypeMethodCallVisitor(TypeMethodCallVisitor&&) = delete;
            TypeMethodCallVisitor& operator=(TypeMethodCallVisitor&&) = delete;        
    };
}

#endif 
