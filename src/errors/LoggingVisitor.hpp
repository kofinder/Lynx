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
