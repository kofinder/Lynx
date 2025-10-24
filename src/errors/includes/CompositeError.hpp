#ifndef LYNX_COMPOSITE_ERROR_HPP
#define LYNX_COMPOSITE_ERROR_HPP

#include <errors/ErrorVisitor.hpp>
#include <errors/LynxError.hpp>


namespace LynxErrors {

    class CompositeError : public LynxError {

        private:
        
            std::vector<std::shared_ptr<LynxError>> children;

        public:

            void add(std::shared_ptr<LynxError> error) override {
                children.push_back(std::move(error));
            }
        
            const std::vector<std::shared_ptr<LynxError>>& getChildren() const override {
                return children;
            }
        
            void accept(ErrorVisitor& visitor) override {
                visitor.visit(*this);
            }
        
            const char* what() const noexcept override {
                return "CompositeError: multiple errors";
            }
    };
    
}

#endif 
