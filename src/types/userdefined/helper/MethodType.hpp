#ifndef LYNX_METHOD_TYPE_HELPER_HPP
#define LYNX_METHOD_TYPE_HELPER_HPP

#include <string>
#include <types/interfaces/BaseType.hpp>
#include <constants/AccessModifierType.hpp>

namespace LynxTypes {

    using namespace LynxConstants;

    enum MethodKind { CONSTRUCTOR, METHOD };

    struct ResolvedCall { std::vector<llvm::Type*> argTypes; std::string managledName; };

    struct Candidate { std::string mangled; int64_t score; };

    class MethodType {

        private:

            std::string name;

            std::unique_ptr<BaseType> returnType;

            std::vector<std::unique_ptr<BaseType>> parameterTypes;

            AccessModifierType accessType;

            bool is_virtual;

            bool is_static;

            bool is_abstract;

            int index;

        public:

            MethodType(
                AccessModifierType access,
                std::string funcName, 
                std::unique_ptr<BaseType> retType, 
                std::vector<std::unique_ptr<BaseType>> params,
                int idx = -1, 
                bool _isAbstract = false,
                bool _isStatic = false,
                bool _isVirtual = false
            ) : name(std::move(funcName)), accessType(access), returnType(std::move(retType)), 
                parameterTypes(std::move(params)), index(idx),
                is_abstract(_isAbstract), is_static(_isStatic), is_virtual(_isVirtual) {}

            [[nodiscard]] const std::string& getName() const { return name; }

            [[nodiscard]] std::string getMethodSignature() const { return "methodSig"; }

            [[nodiscard]] const BaseType* getReturnType() const { return returnType.get(); }
        
            [[nodiscard]] const std::vector<std::unique_ptr<BaseType>>& getParameterTypes() const { return parameterTypes; }
        
            [[nodiscard]] bool isAbstract() const { return is_abstract; }

            [[nodiscard]] bool isStatic() const { return is_static; }

            [[nodiscard]] bool isVirtual() const { return is_virtual; }

            [[nodiscard]] AccessModifierType getAccessType() { return accessType; }

            std::vector<BaseType*> getParameterRawTypes() const {
                std::vector<BaseType*> result;
                result.reserve(parameterTypes.size());
                for (const auto& p : parameterTypes) result.push_back(p.get());
                return result;
            }

            std::unique_ptr<MethodType> clone() const {

                auto clonePtr = [](const std::unique_ptr<BaseType>& ptr) { return ptr ? ptr->clone() : nullptr; };
                auto clonedReturnType = clonePtr(returnType);
                
                std::vector<std::unique_ptr<BaseType>> clonedParams;
                clonedParams.reserve(parameterTypes.size());
                for (const auto& param : parameterTypes) {
                    clonedParams.push_back(clonePtr(param));
                }
            
                return std::make_unique<MethodType>(
                    accessType,
                    name,
                    std::move(clonedReturnType),
                    std::move(clonedParams),
                    is_abstract,
                    is_static,
                    is_virtual
                );
            }
    };
    
}

#endif