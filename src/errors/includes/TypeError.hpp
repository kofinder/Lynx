#ifndef LYNX_TYPE_ERROR_HPP
#define LYNX_TYPE_ERROR_HPP

#include <errors/ErrorVisitor.hpp>
#include <errors/LynxError.hpp>

namespace LynxErrors {
    
    class TypeError : public LynxError {

        private:
        
            std::string message;

        public:

            explicit TypeError(std::string msg) : message(std::move(msg)) {}

            void add(std::shared_ptr<LynxError>) final override {
                assert(false && "TypeError is a leaf and cannot have children.");
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
