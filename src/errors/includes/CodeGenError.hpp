#ifndef LYNX_CODE_GEN_ERROR_HPP
#define LYNX_CODE_GEN_ERROR_HPP

#include <errors/ErrorVisitor.hpp>
#include <errors/LynxError.hpp>

namespace LynxErrors {
    
    class CodeGenError : public LynxError {

        private:
        
            std::string message;

        public:

            explicit CodeGenError(std::string msg) : message(std::move(msg)) {}

            void add(std::shared_ptr<LynxError>) final override {
                assert(false && "CodeGenError is a leaf and cannot have children.");
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
