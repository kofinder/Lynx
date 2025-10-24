#ifndef LYNX_MANGLING_CONTEXT_HPP
#define LYNX_MANGLING_CONTEXT_HPP

#include <memory>
#include "IMangleStrategy.hpp"

namespace LynxMangler {

    class ManglingContext {

        private:

            std::unique_ptr<IMangleStrategy> strategy;

        public:

            void setStrategy(std::unique_ptr<IMangleStrategy> newStrategy) {
                strategy = std::move(newStrategy);
            }
        
            std::string delegateMangle(const MangleParameter& param) {
                const auto& [kind, nameSpace, clazzName, methodName, args] = param;
                switch (kind) {
                    case ManglerKind::FUNCTION:
                        if (!methodName) throw std::invalid_argument("FUNCTION requires methodName");
                        return strategy->mangleFunction(*methodName, args);

                    case ManglerKind::MEMBER_FUNCTION:
                        if (!clazzName || !methodName) throw std::invalid_argument("MEMBER_FUNCTION requires clazzName and methodName");
                        return strategy->mangleMemberFunction(*clazzName, *methodName, args);

                    case ManglerKind::CONSTRUCTOR:
                        if (!clazzName) throw std::invalid_argument("CONSTRUCTOR requires clazzName");
                        return strategy->mangleConstructor(*clazzName, args);

                    case ManglerKind::CLASS:
                        if (!clazzName) throw std::invalid_argument("CLASS requires clazzName");
                        return strategy->mangleClass(*clazzName);

                    default:
                        if (!methodName) throw std::invalid_argument("Unknown ManglerKind and no methodName provided");
                        return *methodName;
                }
            }

    };
}

#endif
