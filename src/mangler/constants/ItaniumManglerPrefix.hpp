/**
 * @file ItaniumManglerPrefix.hpp
 * @brief Defines prefix and suffix constants for Itanium C++ name mangling.
 *
 * This header provides the string constants used in the Itanium C++ ABI for
 * encoding function names, class names, nested names, special member functions,
 * thunks, typeinfo, vtables, guard variables, and global constructors/destructors.
 *
 * Key Constants:
 * - `MANGLE_PREFIX`: Base prefix for all mangled C++ names (`_Z`).
 * - `PREFIX_NESTED_NAME` / `SUFFIX_END_NESTED_NAME`: Marks nested scopes (namespaces, classes).
 * - `SUFFIX_CONSTRUCTOR_PRIMARY`, `SUFFIX_CONSTRUCTOR_COMPLETE`, `SUFFIX_CONSTRUCTOR_ALLOCATING`: Encodes special constructors.
 * - `SUFFIX_DESTRUCTOR_BASE`, `SUFFIX_DESTRUCTOR_COMPLETE`, `SUFFIX_DESTRUCTOR_DELETING`: Encodes special destructors.
 * - `PREFIX_TYPEINFO`, `PREFIX_VTABLE`, `PREFIX_VTT`: Used for RTTI and virtual table symbols.
 * - `PREFIX_GUARD_VARIABLE`: Used for static local variable guards.
 * - `PREFIX_NON_VIRTUAL_THUNK`, `PREFIX_VIRTUAL_THUNK`, `PREFIX_COVARIANT_THUNK`: Encodes thunk functions.
 * - `PREFIX_GLOBAL_CTOR`, `PREFIX_GLOBAL_DTOR`: Encodes global constructors and destructors.
 *
 * Example Usage:
 * @code
 * std::string mangledName = MANGLE_PREFIX + PREFIX_NESTED_NAME + encodeName("MyClass") + SUFFIX_END_NESTED_NAME;
 * @endcode
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/


#ifndef LYNX_ITANIUM_MANGLER_PREFIX_HPP
#define LYNX_ITANIUM_MANGLER_PREFIX_HPP

namespace LynxMangler {

    // Base prefix for all mangled C++ names
    constexpr const char* MANGLE_PREFIX                 = "_Z";

    // Nested name (e.g. class/function inside namespace or class)
    constexpr const char* PREFIX_NESTED_NAME            = "N";

    // End of a nested name
    constexpr const char* SUFFIX_END_NESTED_NAME        = "E";

    // Typeinfo structures (RTTI)
    constexpr const char* PREFIX_TYPEINFO               = "TI";
    constexpr const char* PREFIX_TYPEINFO_NAME          = "TS";
    constexpr const char* PREFIX_TYPEINFO_STRUCTURE     = "TIS";

    // VTable, VTT, typeinfo, etc.
    constexpr const char* PREFIX_VTABLE                 = "TV";
    constexpr const char* PREFIX_VTT                    = "TT";
    constexpr const char* PREFIX_CONSTRUCTION_VTABLE    = "TC";
    constexpr const char* PREFIX_TYPEINFO_FOR           = "TI";
    constexpr const char* PREFIX_TYPEINFO_NAME_FOR      = "TS";

    // Guard variable (for static locals)
    constexpr const char* PREFIX_GUARD_VARIABLE         = "GV";

    // Thunks
    constexpr const char* PREFIX_NON_VIRTUAL_THUNK      = "Th";
    constexpr const char* PREFIX_VIRTUAL_THUNK          = "Tv";
    constexpr const char* PREFIX_COVARIANT_THUNK        = "Tc";

    // Special member function kinds
    constexpr const char* SUFFIX_CONSTRUCTOR_PRIMARY    = "C1"; // Base constructor
    constexpr const char* SUFFIX_CONSTRUCTOR_COMPLETE   = "C2"; // Complete object constructor
    constexpr const char* SUFFIX_CONSTRUCTOR_ALLOCATING = "C3"; // Allocating constructor

    constexpr const char* SUFFIX_DESTRUCTOR_BASE        = "D0"; // Base destructor
    constexpr const char* SUFFIX_DESTRUCTOR_COMPLETE    = "D1"; // Complete destructor
    constexpr const char* SUFFIX_DESTRUCTOR_DELETING    = "D2"; // Deleting destructor
    constexpr const char* SUFFIX_VOID_PARAMS            = "Ev"; // Empty parameter list (void)


    // Operator names (encoded differently, but here as reference)
    constexpr const char* PREFIX_OPERATOR_NAME          = "op"; // Not part of ABI; placeholder

    // Global constructors/destructors
    constexpr const char* PREFIX_GLOBAL_CTOR            = "GLOBAL__I_";
    constexpr const char* PREFIX_GLOBAL_DTOR            = "GLOBAL__D_";
}

#endif 
