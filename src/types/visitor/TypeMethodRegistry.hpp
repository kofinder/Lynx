/**
 * @file TypeMethodRegistry.hpp
 * @brief Provides a centralized registry for methods supported by Lynx types.
 *
 * This header defines the `TypeMethodRegistry` class, which maintains
 * metadata about available methods for each type in the Lynx type system.
 * It tracks method names, parameter counts, and whether methods are static.
 *
 * Key features:
 *  - Register individual or multiple methods for a type.
 *  - Check if a type supports a given method (`hasMethod`).
 *  - Validate method calls by argument count (`validateMethodCall`).
 *  - Retrieve expected parameter count for a method (`getExpectedParamCount`).
 *
 * Benefits:
 *  - Ensures consistent method handling across all types.
 *  - Simplifies backend code generation and compiler validation.
 *  - Reduces runtime errors related to invalid or unsupported method calls.
 *
 * The design uses nested unordered_maps for fast lookup and is intended
 * primarily for compiler internal use.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date:   Jan 2, 2022
*/

#ifndef LYNX_TYPE_METHOD_REGISTRY_HPP
#define LYNX_TYPE_METHOD_REGISTRY_HPP

#include <functional>
#include <unordered_map>
#include <string>
#include <memory>
#include <llvm/IR/Value.h>

namespace LynxTypes {

    struct TypeName { std::string value; };
    struct MethodName { std::string value; };
    struct MethodInfo { bool isStaticMethod = true; size_t paramCount = 0; };

    class TypeMethodRegistry {

        private:

            std::unordered_map<std::string, std::unordered_map<std::string_view, MethodInfo>> registry;

        public:

            void registerMethod(const std::string& typeName, const std::string_view& methodName, size_t paramCount = 0) {
                registry[typeName][methodName] = MethodInfo{.isStaticMethod = true, .paramCount = paramCount};
            }

            void registerMethods(const std::string& typeName, const std::unordered_map<std::string_view, int>& methods) {
                for (const auto& [methodName, paramCount] : methods) {
                    registerMethod(typeName, methodName, static_cast<size_t>(paramCount));
                }
            }

            bool hasMethod(const TypeName& type, const MethodName& method) const {
                auto typeIt = registry.find(type.value);
                if (typeIt == registry.end()) return false;
                return typeIt->second.contains(method.value);
            }    
            
            bool validateMethodCall(const TypeName& type, const MethodName& method, size_t argCount) const {
                auto typeIt = registry.find(type.value);
                if (typeIt == registry.end()) return false;
                auto methodIt = typeIt->second.find(method.value);
                if (methodIt == typeIt->second.end()) return false;
                return methodIt->second.paramCount == argCount;
            }
            
            size_t getExpectedParamCount(const TypeName& type, const MethodName& method) const {
                auto typeIt = registry.find(type.value);
                if (typeIt == registry.end()) return 0;
                auto methodIt = typeIt->second.find(method.value);
                if (methodIt == typeIt->second.end()) return 0;
                return methodIt->second.paramCount;
            }    

    };
}

#endif 
