/**
 * @file ManglingContext.hpp
 * @brief Provides a context for delegating name mangling using a selected strategy.
 *
 * The `ManglingContext` class allows setting a specific mangling strategy at runtime
 * and delegates mangling requests to that strategy. It abstracts the details of
 * individual mangling schemes (Itanium, Microsoft, custom Lynx) from client code.
 *
 * **Key Features:**
 * - Holds a unique pointer to an `IMangleStrategy` instance.
 * - Allows switching strategies dynamically with `setStrategy()`.
 * - Provides `delegateMangle()` to perform name mangling for functions, member functions,
 *   constructors, and classes based on a `MangleParameter`.
 * - Throws descriptive exceptions if required parameters are missing for a given `ManglerKind`.
 *
 * Usage Example:
 * @code
 * LynxMangler::ManglingContext context;
 * context.setStrategy(std::make_unique<LynxMangler::ItaniumMangler>());
 * std::string mangled = context.delegateMangle(
 *     LynxMangler::MangleParameter::makeFunction("foo", params)
 * );
 * @endcode
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/


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
