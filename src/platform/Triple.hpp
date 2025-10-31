#ifndef LYNX_PLATFORM_TRIPLE_HPP
#define LYNX_PLATFORM_TRIPLE_HPP

#include <cstdint>
#include <constants/ArchType.hpp>
#include <constants/TargetOSType.hpp>
#include <constants/LynxTargetType.hpp>

using namespace LynxConstants;

namespace LynxPlatform {

    class Triple {

        private:
            LynxTargetType targetType;

            TargetOSType osType;

            ArchType archType;

            std::string m_env;

        public:

            Triple(
                LynxTargetType target, 
                TargetOSType targetOS, 
                ArchType arch,
                const std::string& env = ""
            ) : targetType(target), osType(targetOS), archType(arch), m_env(env) {};

            ArchType getArch() const { return archType; }

            TargetOSType getOS() const { return osType; }

            LynxTargetType getTarget() const { return targetType; }

            const std::string& getEnvironment() const { return m_env; }
        
            std::string toString() const;
            static Triple fromString(const std::string& tripleStr);
        
            std::string toLLVMTriple() const;
            static Triple fromLLVMTriple(const std::string& llvmTripleStr);

            uint32_t encode() const;
    };
    
}

#endif
