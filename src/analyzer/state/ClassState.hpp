#ifndef LYNX_CLASS_STATE_HPP
#define LYNX_CLASS_STATE_HPP

#include <unordered_map>
#include "interfaces/SemanticState.hpp"
#include <types/userdefined/helper/MethodType.hpp>
#include <types/userdefined/helper/FieldType.hpp>

namespace LynxAnalyzer {

    using namespace LynxTypes;

    class ClassState : public SemanticState {

        private:

            /**
             * @brief Checks whether a field is accessible from the given class context.
             * 
             * @param currentClass The class currently being analyzed.
             * @param field The field type being accessed.
             * @return true if accessible, false otherwise.
            */
            bool canAccessField(const std::string& currentClass, const FieldType* method) const;

            /**
             * @brief Checks whether a method is accessible from the given class context.
            */
            bool canAccessMethod(const std::string& currentClass, const MethodType* method) const;

            /**
             * @brief Determines whether `derived` is a subclass of `base`.
            */
            bool isSubclass(const std::string& derived, const std::string& base) const;

        public:
        
            /**
             * @brief Handle nodes relevant to class semantics.
             * 
             * This is called by SemanticAnalyzer when traversing AST.
            */
            void handleNode(SemanticAnalyzer& analyzer, Node* node) override;

    };
        
}

#endif 
