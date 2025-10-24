#ifndef LYNX_PARAMETER_HPP  
#define LYNX_PARAMETER_HPP

#include <string>  
#include "VariableType.hpp"  


namespace LynxConstants {

    class Parameter {  
        
        public:  
        
            std::string name;  

            std::shared_ptr<VariableType> type;  

            explicit Parameter(
                std::shared_ptr<VariableType> varType, 
                const std::string& varName
            ) : name(varName), type(std::move(varType)) {}
    };  
}

#endif
