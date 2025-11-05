/**
 * @file ErrorVisitor.hpp
 * @brief Declares the abstract ErrorVisitor interface for the Lynx compiler.
 * 
 * ErrorVisitor provides a mechanism to traverse and handle different types
 * of errors in a unified way, following the Visitor Design Pattern.
 * Each error type in the compiler invokes the corresponding visit() method,
 * allowing customized handling for parsing, semantic, linking, runtime, and
 * code generation errors.
 * 
 * **Key Responsibilities:**
 * - Define a unified interface for handling all compiler/runtime errors.
 * - Facilitate double-dispatch for error-specific behavior.
 * - Enable logging, reporting, or recovery strategies based on error type.
 * 
 * **Used By:**
 * - Compiler diagnostic subsystems.
 * - Error reporting and logging mechanisms.
 * - Runtime exception handling infrastructure.
 * 
 * * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/


#ifndef LYNX_ERROR_VISITOR_HPP
#define LYNX_ERROR_VISITOR_HPP


namespace LynxErrors {

    class CodeGenError;
    class ContextError;
    class LinkError;
    class ParserError;
    class RuntimeError;
    class SymbolError;
    class SyntaxError;
    class CompositeError;
    
    class ErrorVisitor {

        public:
        
            virtual ~ErrorVisitor() = default;
        
            virtual void visit(class ParserError& err) = 0;

            virtual void visit(class ContextError& err) = 0;

            virtual void visit(class RuntimeError& err) = 0;

            virtual void visit(class CompositeError& err) = 0;

            virtual void visit(class SymbolError& err) = 0;

            virtual void visit(class SyntaxError& err) = 0;

            virtual void visit(class LinkError& err) = 0;

            virtual void visit(class CodeGenError& err) = 0;

            virtual void visit(class TypeError& err) = 0;

        };
}

#endif
