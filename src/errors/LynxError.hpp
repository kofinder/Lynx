/**
 * @file Error.hpp
 * @brief Declares the abstract LynxError class, the base for all compiler/runtime errors.
 * 
 * LynxError defines the common interface for all error types in the Lynx compiler
 * and runtime. It supports the Visitor pattern via `accept()`, optional hierarchical
 * error composition, and standard `what()` messaging from `std::exception`.
 * 
 * **Key Responsibilities:**
 * - Provide a common interface for all errors.
 * - Enable tree-like composition of errors via `add()` and `getChildren()`.
 * - Integrate with ErrorVisitor for type-specific handling.
 * - Ensure consistent error messaging via `what()`.
 * 
 * **Used By:**
 * - All concrete error types such as ParserError, SemanticError, LinkError, etc.
 * - Compiler diagnostics and runtime error reporting systems.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/


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
