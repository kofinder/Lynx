#include "BaseException.hpp"

namespace LynxExceptions {
    class SemanticException : public BaseException {  
        protected:  
            int lineNumber;  // The line number where the semantic error occurred  

        public:  
            // Constructor that initializes error message and line number  
            SemanticException(const std::string& error, int lineNumber)  
                : BaseException(error), lineNumber(lineNumber) {}  

            // Override the getMessage method  
            std::string getMessage() const override {  
                std::ostringstream oss;  // Use ostringstream to construct the message  
                oss << "Error: " << message << " at line " << lineNumber;  
                return oss.str();  // Return the constructed message  
            }  
    };  
}