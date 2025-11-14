#ifndef LYNX_TYPE_STATIC_METHOD_REGISTRY_HPP
#define LYNX_TYPE_STATIC_METHOD_REGISTRY_HPP

#include <functional>
#include <unordered_map>
#include <string>
#include <memory>
#include <llvm/IR/Value.h>

namespace LynxTypes {

    class TypeStaticMethodRegistry {

        public:

            using StaticMethodFunc = std::function<llvm::Value*(std::vector<llvm::Value*> args)>;

        private:

            std::unordered_map<std::string, std::unordered_map<std::string, StaticMethodFunc>> registry;

        public:

            void registerMethod(const std::string& typeName, const std::string& methodName, StaticMethodFunc func) {
                registry[typeName][methodName] = std::move(func);
            }

            bool hasMethod(const std::string& typeName, const std::string& methodName) const {
                auto it = registry.find(typeName);
                if (it == registry.end()) return false;
                return it->second.find(methodName) != it->second.end();
            }

            llvm::Value* invoke(const std::string& typeName, const std::string& methodName, const std::vector<llvm::Value*>& args = {}) const {
                auto it = registry.find(typeName);
                if (it == registry.end()) return nullptr;

                auto mit = it->second.find(methodName);
                if (mit == it->second.end()) return nullptr;

                return mit->second(args);
            }
    };
}

#endif 
