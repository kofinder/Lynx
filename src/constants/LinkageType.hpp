#ifndef LYNX_LINKAGE_TYPE_HPP
#define LYNX_LINKAGE_TYPE_HPP

#include <llvm/IR/Value.h>
#include <llvm/IR/GlobalValue.h>

namespace LynxConstants {


    // External linkage: visible outside this module; corresponds to 'extern' in C/C++
    // Use for globals/functions that should be accessible from other modules.
    static constexpr auto extType = llvm::GlobalValue::ExternalLinkage;

    // Private linkage: visible only in this module; corresponds to 'static' at global scope in C/C++
    // Use for symbols that must not be exported.
    static constexpr auto privateType = llvm::GlobalValue::PrivateLinkage;

    // Internal linkage: like private; corresponds to 'static' but used internally by the compiler
    // Similar to privateType; typically for module-local symbols.
    static constexpr auto internalType = llvm::GlobalValue::InternalLinkage;

    // LinkOnce: allows multiple definitions across modules; linker merges them
    // Corresponds to 'inline' in C++ for functions in header files.
    static constexpr auto linkOnceType = llvm::GlobalValue::LinkOnceAnyLinkage;

    // WeakAny: weak symbol; can be overridden by another definition
    // Often used for optional globals or symbols provided by libraries.
    static constexpr auto weakAnyType = llvm::GlobalValue::WeakAnyLinkage;

    // WeakODR: weak symbol for ODR-compliant C++ symbols (inline/templated)
    // Corresponds to 'inline' functions or template instantiations that may appear in multiple modules.
    static constexpr auto weakODRType = llvm::GlobalValue::WeakODRLinkage;

    // Appending: used for global arrays that must be concatenated from multiple modules
    // Typically used for metadata tables or registration arrays.
    static constexpr auto appendingType = llvm::GlobalValue::AppendingLinkage;

    // Common: uninitialized global; linker merges duplicates
    // Similar to declaring 'int x;' at global scope without 'extern' or initializer in C/C++.
    static constexpr auto commonType = llvm::GlobalValue::CommonLinkage;

    // AvailableExternally: declaration only; symbol not emitted in this module
    // Use when you only need to declare a function for optimization; corresponds to 'extern' declaration without definition.
    static constexpr auto availExternallyType = llvm::GlobalValue::AvailableExternallyLinkage;

    // ExternalWeak: weak symbol visible externally; may be overridden
    // Useful for optional symbols that can be replaced by other modules.
    static constexpr auto externalWeakType = llvm::GlobalValue::ExternalWeakLinkage;

    // LinkOnceODR: like LinkOnce but specifically for ODR-compliant symbols
    // Typically used for 'inline' functions in C++ headers.
    static constexpr auto linkOnceODRType = llvm::GlobalValue::LinkOnceODRLinkage;

}


#endif
