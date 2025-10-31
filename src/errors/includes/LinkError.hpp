#ifndef LYNX_LINK_ERROR_HPP
#define LYNX_LINK_ERROR_HPP

#include <errors/ErrorVisitor.hpp>
#include <errors/LynxError.hpp>

namespace LynxErrors {

    class LinkError : public LynxError {

        private:
        
            std::string message;

        public:

            explicit LinkError(std::string msg) : message(std::move(msg)) {}

            void add(std::shared_ptr<LynxError>) final override {
                assert(false && "LinkerError is a leaf and cannot have children.");
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
