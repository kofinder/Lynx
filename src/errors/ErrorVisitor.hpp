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
