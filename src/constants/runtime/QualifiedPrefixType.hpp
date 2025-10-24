#ifndef LYNX_QUALIFIED_PREFIX_TYPE_HPP
#define LYNX_QUALIFIED_PREFIX_TYPE_HPP

#include <string>
#include <algorithm>
#include <cctype>

namespace LynxConstants {

    enum class SystemPrefixType { FS, STD, LOG, JSON, OTHERS };

    class QualifiedPrefixType {

        public:
        
            explicit QualifiedPrefixType(const std::string& name) : prefixName(name) {}

            SystemPrefixType getPrefixType() const noexcept { return parsePrefix(prefixName); }

            static std::string getPrefixName(SystemPrefixType type) noexcept { return toString(type); }

            const std::string& getRawPrefix() const noexcept { return prefixName; }

        private:

            std::string prefixName;

            static SystemPrefixType parsePrefix(const std::string& prefix) noexcept {
                std::string lcase = prefix;
                std::transform(lcase.begin(), lcase.end(), lcase.begin(),
                            [](unsigned char c) { return std::tolower(c); });

                if (lcase == "fs")    return SystemPrefixType::FS;
                if (lcase == "std")   return SystemPrefixType::STD;
                if (lcase == "log")   return SystemPrefixType::LOG;
                if (lcase == "json")  return SystemPrefixType::JSON;
                return SystemPrefixType::OTHERS;
            }

            static std::string toString(SystemPrefixType type) noexcept {
                switch (type) {
                    case SystemPrefixType::FS:     return "fs";
                    case SystemPrefixType::STD:    return "std";
                    case SystemPrefixType::LOG:    return "log";
                    case SystemPrefixType::JSON:   return "json";
                    case SystemPrefixType::OTHERS: return "others";
                }
                return "unknown"; 
            }
    };

}

#endif
