#ifndef LYNX_INTERFACE_STATE_HPP
#define LYNX_INTERFACE_STATE_HPP

#include <unordered_map>
#include "interfaces/SemanticState.hpp"
#include <types/userdefined/helper/MethodType.hpp>
#include <types/userdefined/helper/FieldType.hpp>

namespace LynxAnalyzer {
    
    using namespace LynxTypes;

    class InterfaceState : public SemanticState {

        private:

            bool canAccessField(const std::string& currentClass, const FieldType* method) const;

            bool canAccessMethod(const std::string& currentClass, const MethodType* method) const;

            bool isSubclass(const std::string& derived, const std::string& base) const;

        public:

            void handleNode(SemanticAnalyzer& analyzer, Node* node) override;
    };
        
}

#endif 
