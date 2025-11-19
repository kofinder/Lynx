/**
 * @file LoggingVisitor.hpp
 * @brief Implements an ErrorVisitor that logs all LynxErrors to the console.
 * 
 * LoggingVisitor provides a concrete implementation of the Visitor pattern
 * for error reporting. Each type of error (ParserError, RuntimeError, etc.)
 * is printed with a prefixed label to distinguish its category.
 * Composite errors recursively log all contained child errors.
 * 
 * **Key Responsibilities:**
 * - Visit each type of LynxError and output formatted messages.
 * - Recursively traverse composite errors to report all nested issues.
 * - Provide a simple console-based debugging mechanism for compiler/runtime errors.
 * 
 * **Used By:**
 * - Compiler frontend for error reporting during parsing and semantic analysis.
 * - Runtime for logging runtime or linking errors.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_LOGGING_VISITOR_HPP
#define LYNX_LOGGING_VISITOR_HPP

#include <iostream>
#include "ErrorVisitor.hpp"

namespace LynxErrors {

    class LoggingVisitor : public ErrorVisitor {

        public:

            void visit(ParserError& err) override {
                std::cout << "[ParserError] " << err.what() << std::endl;
            }
        
            void visit(ContextError& err) override {
                std::cout << "[ContextError] " << err.what() << std::endl;
            }
        
            void visit(RuntimeError& err) override {
                std::cout << "[RuntimeError] " << err.what() << std::endl;
            }
        
            void visit(CompositeError& err) override {
                std::cout << "[CompositeError] contains " << err.getChildren().size() << " errors:" << std::endl;
                for (auto& child : err.getChildren()) {
                    child->accept(*this);  // recursively visit children
                }
            }

            void visit(SymbolError& err) override {
                std::cout << "[SymbolError] " << err.what() << std::endl;
            }

            void visit(SyntaxError& err) override {
                std::cout << "[SyntaxError] " << err.what() << std::endl;

            }

            void visit(LinkError& err) override {
                std::cout << "[LinkError] " << err.what() << std::endl;
            }

            void visit(CodeGenError& err) override {
                std::cout << "[CodeGenError] " << err.what() << std::endl;
            }

            void visit(TypeError& err) override {
                std::cout << "[TypeError] " << err.what() << std::endl;
            }

        };
        
}

#endif
