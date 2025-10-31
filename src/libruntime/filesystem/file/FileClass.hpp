#ifndef LYNX_LIB_RUNTIME_FILE_CLASS_HPP
#define LYNX_LIB_RUNTIME_FILE_CLASS_HPP

#include <string>
#include <optional>
#include "RuntimeClass.hpp"

#include "includes/FSOpenFunction.hpp"
#include "includes/FSReadFunction.hpp"
#include "includes/FSWriteFunction.hpp"
#include "includes/FSCloseFunction.hpp"
#include "includes/FSExistsFunction.hpp"

#include <ast/LiteralNode.hpp>
#include <ast/PrimaryExpressionNode.hpp>


namespace LynxLibRuntime {

    class FileClass : public RuntimeClass {

        private:

            std::optional<std::string> extractStringLiteral(Node* node) {
                if (auto* primary = dynamic_cast<PrimaryExpressionNode*>(node)) {
                    if (auto* literal = dynamic_cast<LiteralNode*>(primary->getInnerExpression())) {
                        auto value = literal->getLiteralValue();
                        if (std::holds_alternative<std::string>(value)) {
                            return std::get<std::string>(value);
                        }
                    }
                }

                return std::nullopt;            
            }

        public:

            std::string getName() const override {
                return "File";
            }

            void registerMethods(RuntimeFunctionRegistry& registry) override {
              
                auto openFn = std::make_shared<FSOpenFunction>();
                auto readFn = std::make_shared<FSReadFunction>();
                auto writeFn = std::make_shared<FSWriteFunction>();
                auto closeFn = std::make_shared<FSCloseFunction>();
                auto existsFn = std::make_shared<FSExistsFunction>();
            
                registry.registerFunction("File.open", openFn);
                registry.registerFunction("File.read", readFn);
                registry.registerFunction("File.write", writeFn);
                registry.registerFunction("File.close", closeFn);
                registry.registerFunction("File.exists", existsFn);
            
                methods["open"] = openFn;
                methods["read"] = readFn;
                methods["write"] = writeFn;
                methods["close"] = closeFn;
                methods["exists"] = existsFn;
            }
        };
}

#endif
