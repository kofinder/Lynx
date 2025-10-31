#ifndef LYNX_ERROR_HELPERS_HPP
#define LYNX_ERROR_HELPERS_HPP

#include <memory>
#include <vector>
#include <string>
#include <sstream>

#include "includes/CodeGenError.hpp"
#include "includes/ContextError.hpp"
#include "includes/LinkError.hpp"
#include "includes/ParserError.hpp"
#include "includes/RuntimeError.hpp"
#include "includes/SymbolError.hpp"
#include "includes/SyntaxError.hpp"
#include "includes/TypeError.hpp"
#include "includes/CompositeError.hpp"

namespace LynxErrors {

    inline std::shared_ptr<ParserError> makeParserError(const std::string& msg) {
        return std::make_shared<ParserError>(msg);
    }

    inline std::shared_ptr<ContextError> makeContextError(const std::string& msg) {
        return std::make_shared<ContextError>(msg);
    }

    inline std::shared_ptr<RuntimeError> makeRuntimeError(const std::string& msg) {
        return std::make_shared<RuntimeError>(msg);
    }

    inline std::shared_ptr<SymbolError> makeSymbolError(const std::string& msg) {
        return std::make_shared<SymbolError>(msg);
    }

    inline std::shared_ptr<SyntaxError> makeSyntaxError(const std::string& msg) {
        return std::make_shared<SyntaxError>(msg);
    }

    inline std::shared_ptr<LinkError> makeLinkError(const std::string& msg) {
        return std::make_shared<LinkError>(msg);
    }

    inline std::shared_ptr<CodeGenError> makeCodeGenError(const std::string& msg) {
        return std::make_shared<CodeGenError>(msg);
    }

    inline std::shared_ptr<TypeError> makeTypeError(const std::string& msg) {
        return std::make_shared<TypeError>(msg);
    }

    inline std::shared_ptr<CompositeError> groupErrors(const std::vector<std::shared_ptr<LynxError>>& errors) {
        auto composite = std::make_shared<CompositeError>();
        for (const auto& err : errors) {
            composite->add(err);
        }
        return composite;
    }

    inline std::string formatError(const std::string& message, int line = -1, int column = -1) {
        std::ostringstream oss;
        if (line >= 0 && column >= 0) {
            oss << "Line " << line << ", Column " << column << ": ";
        }
        oss << message;
        return oss.str();
    }

}

#endif
