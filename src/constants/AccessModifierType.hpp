#ifndef LYNX_ACCESS_MODIFIER_TYPE_HPP  
#define LYNX_ACCESS_MODIFIER_TYPE_HPP  

namespace LynxConstants {

    enum class AccessModifierType {

        DEFAULT = 0,    // package-private (default access modifier)

        PUBLIC = 1,     // Most permissive

        PROTECTED = 2,  // Inherited but not externally accessible

        PRIVATE = 3     // Most restrictive
    };

    inline AccessModifierType operator++(AccessModifierType& accessModifierType, int) {
        auto old = accessModifierType;

        switch (accessModifierType) {
            case AccessModifierType::DEFAULT:   accessModifierType = AccessModifierType::PUBLIC;    break;
            case AccessModifierType::PUBLIC:    accessModifierType = AccessModifierType::PROTECTED; break;
            case AccessModifierType::PROTECTED: accessModifierType = AccessModifierType::PRIVATE;   break;
            case AccessModifierType::PRIVATE:   accessModifierType = AccessModifierType::DEFAULT;   break;        
        }
        
        return old;
    }

    inline AccessModifierType parseStringToModifierType(const std::string& type) {
        std::string lower = type;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        if (lower == "public")    return AccessModifierType::PUBLIC;
        if (lower == "protected") return AccessModifierType::PROTECTED;
        if (lower == "private")   return AccessModifierType::PRIVATE;
        if (lower == "default")   return AccessModifierType::DEFAULT;

        // fallback: default
        return AccessModifierType::DEFAULT;
    }

    inline std::string parseModifierTypeToString(AccessModifierType type) {
        switch (type) {
            case AccessModifierType::PUBLIC:    return "public";
            case AccessModifierType::PROTECTED: return "protected";
            case AccessModifierType::PRIVATE:   return "private";
            case AccessModifierType::DEFAULT:   return "default";
        }
        return "default"; // fallback
    }

} 

#endif 
