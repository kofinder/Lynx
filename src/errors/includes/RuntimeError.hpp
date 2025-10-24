#ifndef LYNX_RUNTIME_ERROR_HPP
#define LYNX_RUNTIME_ERROR_HPP

#include <errors/ErrorVisitor.hpp>
#include <errors/LynxError.hpp>

namespace LynxErrors {

    class RuntimeError : public LynxError {

        private:
            std::string message;

        public:
            RuntimeError(std::string msg) : message(std::move(msg)) {}

            void add(std::shared_ptr<LynxError>) final override {
                assert(false && "RuntimeError is a leaf and cannot have children.");
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
