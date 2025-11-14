/**
 * @file DateTimeMethodResolver.hpp
 * @brief Concrete TypeMethodResolver for the `DateTime` type.
 *
 * `DateTimeMethodResolver` implements the `resolveMethod` interface to handle
 * method calls on `DateTime` instances in Lynx. It maps method names to their
 * corresponding LLVM IR instructions or runtime operations for date and time manipulation.
 *
 * Example usage:
 * @code
 * DateTimeMethodResolver resolver;
 * llvm::Value* result = resolver.resolveMethod("addDays", dateInstance, {llvmArg}, astContext);
 * @endcode
 *
 * * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/


#ifndef LYNX_DATE_TIME_METHOD_RESOLVER_HPP
#define LYNX_DATE_TIME_METHOD_RESOLVER_HPP

#include "resolver/TypeMethodResolver.hpp"

namespace LynxTypes {

    class DateTimeMethodResolver : public TypeMethodResolver {

        public:

            llvm::Value* resolveMethod(
                AstContext& ctx,
                llvm::Value* instance,
                const std::string& method, 
                const std::vector<llvm::Value*>& args
            ) noexcept override;

    };
}

#endif
