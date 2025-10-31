#ifndef LYNX_CONTEXT_ERROR_HPP
#define LYNX_CONTEXT_ERROR_HPP

#include <errors/ErrorVisitor.hpp>
#include <errors/LynxError.hpp>

namespace LynxErrors {

    class ContextError : public LynxError {

        private:
        
            std::string message;

        public:

            explicit ContextError(std::string msg) : message(std::move(msg)) {}

            void add(std::shared_ptr<LynxError>) final override {
                assert(false && "ContextError is a leaf and cannot have children.");
            }  
        
            void accept(ErrorVisitor& visitor) override {
                visitor.visit(*this);
            }

            const char* what() const noexcept override {
                return message.c_str();
            }
    };
    
}

#endif
