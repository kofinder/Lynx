#ifndef LYNX_ERROR_HPP
#define LYNX_ERROR_HPP

#include <exception>
#include <memory>
#include <vector>
#include <cassert> 

namespace LynxErrors {

    class ErrorVisitor;

    class LynxError : public std::exception {

        public:

            virtual ~LynxError() = default;
        
            virtual void add(std::shared_ptr<LynxError> error) {
                assert(false && "add() called on a leaf error node");
            }
        
            virtual const std::vector<std::shared_ptr<LynxError>>& getChildren() const {
                static const std::vector<std::shared_ptr<LynxError>> empty;
                return empty; 
            }
            
            inline static const auto& emptyChildren() {
                static const std::vector<std::shared_ptr<LynxError>> empty;
                return empty;
            }                       
        
            virtual void accept(ErrorVisitor& visitor) = 0;
        
            [[nodiscard]] virtual const char* what() const noexcept override = 0;
        };  
}


#endif 
