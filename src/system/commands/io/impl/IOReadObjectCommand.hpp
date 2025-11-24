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
 * @date: Jan 2, 2022
*/

#ifndef LYNX_SYSTEM_IO_READ_OBJECT_COMMAND_HPP
#define LYNX_SYSTEM_IO_READ_OBJECT_COMMAND_HPP

#include "system/commands/io/IOReadBizCommand.hpp"

namespace LynxSystem {

    class IOReadObjectCommand final : public IOReadBizCommand {

        public:

            llvm::Value* execute(std::shared_ptr<AstContext> /*context*/, std::vector<llvm::Value*> /*calleeArgs*/) override {
                return nullptr;
            }
    

    };
        
}

#endif



