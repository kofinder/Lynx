#ifndef LYNX_FIELD_TYPE_HELPER_HPP
#define LYNX_FIELD_TYPE_HELPER_HPP

#include <string>
#include <types/interfaces/BaseType.hpp>
#include <constants/AccessModifierType.hpp>

namespace LynxTypes {

    using namespace LynxConstants;

    class FieldType {

        private:

            std::string name;

            bool is_const;

            bool is_static;

            std::unique_ptr<BaseType> type;

            AccessModifierType accessType;

            int index;
        
        public:

            FieldType(
                AccessModifierType access,
                std::string fieldName, 
                std::unique_ptr<BaseType> fieldType,
                int idx = -1, 
                bool _isConst = false, 
                bool _isStatic = false
            ): name(std::move(fieldName)), accessType(access), 
                type(std::move(fieldType)), index(idx), 
                is_const(_isConst), is_static(_isStatic) {}

            [[nodiscard]] const std::string& getName() const { return name; }

            [[nodiscard]] BaseType* getType() const { return type.get(); }
        
            [[nodiscard]] bool getIsConst() const { return is_static; }
        
            [[nodiscard]] bool getIsStatic() const { return is_static; }

            [[nodiscard]] AccessModifierType getAccessType() { return accessType; }
        
            std::unique_ptr<FieldType> clone() const {
                return std::make_unique<FieldType>(
                    accessType,
                    name, 
                    type ? type->clone() : nullptr,
                    is_static,
                    is_static
                );
            }            

    };
}

#endif