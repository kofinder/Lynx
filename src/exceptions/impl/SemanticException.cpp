/**
 * @file SemanticException.hpp
 * @brief Defines the SemanticException class, used for semantic analysis errors in Lynx.
 * 
 * SemanticException extends BaseException to include the line number where the error occurred.
 * This allows precise error reporting during AST traversal and semantic analysis.
 * 
 * **Key Responsibilities:**
 * - Store an error message describing the semantic issue.
 * - Track the line number in the source code where the error occurred.
 * - Provide a formatted error message including both description and line number.
 * 
 * **Used By:**
 * - SemanticAnalyzer and its related states when reporting semantic errors.
 * - Any component performing semantic checks on Lynx source code.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#include <string>
#include <sstream>
#include "BaseException.hpp"

namespace LynxExceptions {

    class SemanticException final : public BaseException {  

        private:

            int lineNumber;

        public:  

            SemanticException(
                const std::string& error, 
                int lineNumber
            )  : BaseException(error), lineNumber(lineNumber) {}  

            [[nodiscard]] auto getMessage() const -> std::string override {
                std::ostringstream oss;
                oss << "Error: " << BaseException::getMessage() << " at line " << lineNumber;
                return oss.str();            
            }  
    };  
}