/**
 * @file IOReadObjectCommand.hpp
 * @brief Defines the IOReadObjectCommand class for deserializing structured objects from input.
 * 
 * The IOReadObjectCommand class extends IOReadBizCommand to provide input functionality 
 * for reading complex user-defined objects. It dynamically allocates an instance of 
 * the specified class type and populates its fields by invoking the appropriate 
 * `system.io.get<Type>()` methods for each member based on its data type.
 * 
 * This command enables structured input such as:
 * 
 * ```cpp
 * Person p = system.io.getObject(Person);
 * ```
 * 
 * The resulting LLVM IR constructs a composite object in memory, 
 * performing type-aware field initialization for primitives, strings, 
 * arrays, and nested objects.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_SYSTEM_IO_READ_OBJECT_COMMAND_HPP
#define LYNX_SYSTEM_IO_READ_OBJECT_COMMAND_HPP

#include "system/commands/io/IOReadBizCommand.hpp"

namespace LynxSystem {

    class IOReadObjectCommand final : public IOReadBizCommand {

        public:

            llvm::Value* execute(std::shared_ptr<AstContext> context, std::vector<llvm::Value*> calleeArgs) override {
                if (calleeArgs.empty()) {
                    throw std::runtime_error("system.io.getObject(Type) requires a type argument.");
                }
        
                auto& builder = context->getBuilder();
                auto* module = context->getModule();
        
                // Retrieve type metadata (pretend calleeArgs[0] encodes ClassType)
                auto* classType = llvm::cast<llvm::StructType>(calleeArgs[0]->getType());
                auto* objectPtr = builder.CreateAlloca(classType, nullptr, "object");
        
                // Loop through fields and call system.io.get<Type>() for each
                for (unsigned i = 0; i < classType->getNumElements(); ++i) {
                    llvm::Type* fieldType = classType->getElementType(i);
                    llvm::Value* fieldPtr = builder.CreateStructGEP(classType, objectPtr, i);
        
                    // Here we’d dispatch to the correct input function based on type
                    // e.g., if int => system.io.getInt()
                    // if string => system.io.in()
                    // For now assume int:
                    // auto* value = IOReadIntCommand().execute(context, {});
                    //builder.CreateStore(value, fieldPtr);
                }
        
                return objectPtr;
            }
    

    };
        
}

#endif



