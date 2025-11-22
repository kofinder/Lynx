#ifndef LYNX_CONSTRUCTOR_TYPE_HELPER_HPP
#define LYNX_CONSTRUCTOR_TYPE_HELPER_HPP

#include <string>
#include <types/interfaces/BaseType.hpp>

namespace LynxTypes {

    class ConstructorType {

        private:

            std::string mangledName;

            std::vector<std::unique_ptr<BaseType>> parameterTypes;

        public:

            ConstructorType(
                std::string funcName, 
                std::vector<std::unique_ptr<BaseType>> params
            ) : mangledName(std::move(funcName)), parameterTypes(std::move(params)) {}

            [[nodiscard]] const std::string& getMangledName() const { return mangledName; }
        
            [[nodiscard]] const std::vector<std::unique_ptr<BaseType>>& getParameterTypes() const { return parameterTypes; }
    
            std::unique_ptr<ConstructorType> clone() const {
                std::vector<std::unique_ptr<BaseType>> clonedParams;
                clonedParams.reserve(parameterTypes.size());
                for (const auto& param : parameterTypes) {
                    clonedParams.push_back(param ? param->clone() : nullptr);
                }
                return std::make_unique<ConstructorType>(mangledName, std::move(clonedParams));
            }
    };
    
}
#endif