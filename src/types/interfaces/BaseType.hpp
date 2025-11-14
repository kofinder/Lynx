/**
 * @file BaseType.hpp
 * @brief Declares the BaseType class, the abstract foundation for all type representations in the Lynx compiler.
 * 
 * The BaseType class defines the common interface and behavior shared by all data types in the Lynx language.
 * It acts as the root of the type system hierarchy, supporting LLVM IR integration, type metadata generation,
 * and advanced semantic checks such as const/static qualifiers, nullability, and assignment rules.
 * 
 * **Key Responsibilities:**
 * - Defines a consistent interface for type computation, comparison, and LLVM IR code generation.
 * - Supports qualifiers such as `const` and `static` for type specialization.
 * - Provides virtual methods for creating, assigning, and inspecting types.
 * - Integrates tightly with LLVM’s IRBuilder and DIBuilder for code generation and debugging metadata.
 * 
 * **Used By:**
 * - Derived type classes such as primitive types, composite types, reference types, and user-defined types.
 * - The semantic analysis and type resolution subsystems via `TypeVisitor` and `TypeMethodResolver`.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 2, 2024
*/

#ifndef LYNX_BASE_TYPE_HPP
#define LYNX_BASE_TYPE_HPP

#include <iostream>
#include <string>
#include <variant>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/DIBuilder.h>
#include <llvm/IR/DataLayout.h>

#include <errors/ErrorHelpers.hpp>
#include <constants/LValueType.hpp>
#include <constants/DataType.hpp>
#include <constants/OperatorType.hpp>
#include <constants/metadata/MetadataTypeConstants.hpp>

namespace LynxContext {
    class AstContext;
}

namespace LynxResolver {
    class TypeMethodResolver;    
}

using namespace LynxErrors;
using namespace LynxContext;
using namespace LynxConstants;
using namespace LynxResolver;
using namespace MetadataTypeConstants;

namespace LynxTypes {

    class TypeVisitor;

    class BaseType {

        protected:
        
            mutable llvm::Type* cachedLLVMType = nullptr;
                
            bool constFlag = false;

            bool staticFlag = false;

            AstContext* astContext;        

            /**
             * @brief Computes and returns the LLVM type associated with this language type.
             * Must be implemented by all derived types.
            */
            virtual llvm::Type* computeLLVMType() const = 0;


            /**
             * @brief Returns a version of this type with the specified const qualification.
             * 
             * This is used to construct modified versions of types like `const int`, `const MyStruct`, etc.
             *
             * @param newIsConst Whether the new type should be const-qualified.
             * @return Pointer to a new BaseType instance with the requested const qualifier.
            */
            virtual const BaseType* createWithConst(bool newIsConst) const = 0;

            /**
             * @brief Returns a version of this type with the specified static qualification.
             * 
             * This is useful when differentiating static vs non-static members or variables.
             * 
             * @param newIsStatic Whether the new type should be static-qualified.
             * @return Pointer to a new BaseType instance with the requested static qualifier.
            */
            virtual const BaseType* createWithStatic(bool newIsStatic) const { return this; }

        public:

            /**
             * @brief Constructor initializing the AST context.
             * @param context Pointer to the AstContext associated with this type.
            */
            BaseType(AstContext* context) : astContext(context), cachedLLVMType(nullptr) {}

            /**
             * @brief Set or update the AST context for this type.
             * @param context Pointer to the new AstContext.
            */
            void setContext(AstContext* context) noexcept { astContext = context; }

            /**
             * @brief Returns the AST context currently associated with this type.
             * @return Pointer to the AstContext.
            */
            inline AstContext* getContext() const noexcept { return astContext; }

            /**
             * @brief Returns the LLVM type, computing it if not already cached.
            */
            llvm::Type* getLLVMType() const;
            

            /**
             * @brief Accepts a visitor to perform operations on the type.
             * Used in the visitor pattern for type-related traversals and transformations.
            */
            virtual void accept(TypeVisitor& visitor) {}

            /**
             * @brief Creates a method resolver for this type (if it supports methods).
             * @return A unique pointer to the method resolver, or nullptr if not applicable.
            */
            // virtual std::unique_ptr<TypeMethodResolver> createMethodResolver() const;

            /**
             * @brief Returns the registry of static methods supported by this type.
             *
             * This registry maps method names to their expected parameter counts and is used
             * during semantic analysis to validate static method calls such as `Int.max()` 
             * or `String.fromString("...")`. Each derived type should override this method
             * to expose the static methods relevant to that type.
             *
             * The registry is typically implemented as a constant lookup table defined in
             * the type’s corresponding constant file (e.g., StaticMethodConstants.hpp).
             *
             * @return A reference to an unordered map of method names to parameter counts.
             */
            virtual const std::unordered_map<std::string, int>& getStaticMethodRegistry() const;

            /**
             * @brief Returns the registry of instance methods supported by this type.
             *
             * This is the counterpart to the static method registry and defines methods
             * callable on instances of the type (e.g., `"hello".length()`, `array.push(x)`).
             *
             * During semantic analysis, this registry is consulted to:
             * - verify that a method exists on the instance type,
             * - ensure the parameter count matches,
             * - allow code generation to dispatch correctly to the method implementation.
             *
             * Types without instance methods should return an empty registry.
             *
             * @return A reference to an unordered map of method names to parameter counts.
            */
            virtual const std::unordered_map<std::string, int>& getInstanceMethodRegistry() const;

            /**
             * @brief Generates LLVM IR for invoking a static method on this type.
             *
             * This function is called during code generation after the semantic phase has
             * validated that the method exists using `getStaticMethodRegistry()`. The 
             * implementation of this method is type-specific and should emit the 
             * appropriate LLVM instructions to compute the result of the static call.
             *
             * @param methodName The name of the static method being invoked.
             * @param args A list of LLVM IR values representing the method arguments.
             * @return An LLVM Value representing the result of the static method call.
            */
            virtual llvm::Value* codegenStaticMethod(const std::string& methodName, const std::vector<llvm::Value*>& args);
            
            /**
             * @brief Returns whether the type is const-qualified.
             * 
             * Checks if the type has been marked as `const`.
             * @return true if the type is const-qualified, false otherwise.
            */
            inline bool isConst() const noexcept { return constFlag; }

            /**
             * @brief Sets the const qualifier for this type.
             * 
             * Marks the type as `const` or removes the const qualification.
             * @param value true to mark the type as const, false to remove const.
            */
            inline void setConst(bool value) noexcept { constFlag = value; }

           /**
             * @brief Returns whether the type is static-qualified.
             * @return True if the type is marked as static.
            */
            inline bool isStatic() const noexcept { return staticFlag; }

            /**
             * @brief Sets the static qualifier of the type.
             * @param value True to mark the type as static; false otherwise.
            */
            inline void setStatic(bool value) noexcept { staticFlag = value; }

            /**
             * @brief Indicates whether the type supports assignment operations.
             * @return True if the type can be assigned to; false by default.
            */
            virtual inline bool supportsAssignment() const noexcept { return false; }

            /**
             * @brief Indicates whether the type supports nullability (e.g., pointers).
             * @return True if the type can be null; false by default.
            */
            virtual inline bool isNullable() const noexcept { return false; }

            /**
             * @brief Indicates whether the type supports index-based access (e.g., arrays).
             * @return True if the type is indexable; false by default.
            */
            virtual inline bool isIndexable() const noexcept { return false; }

            /**
             * @brief Indicates whether the type supports key-based lookup (e.g., maps).
             * @return True if the type allows key lookup; false by default.
            */
            virtual inline bool supportsKeyLookup() const noexcept { return false; }
        
           /**
             * @brief Returns true if the type is a built-in primitive type (e.g., int, float, bool).
             * 
             * This typically includes types directly supported by the language or runtime.
            */
            virtual inline bool isBuiltInType() const noexcept { return false; }

            /**
             * @brief Returns true if the type represents a collection (e.g., array, list, map).
             * 
             * Useful for distinguishing container types from scalar or object types.
            */
            virtual inline bool isCollectionType() const noexcept { return false; }

            /**
             * @brief Returns true if the type is user-defined (e.g., class, struct, enum).
             * 
             * Indicates types created by the user rather than built-in to the language.
            */
            virtual inline bool isUserDefinedType() const noexcept { return false; }

            /**
             * @brief Returns true if the type is a wrapper around another type (e.g., Option<T>, Ref<T>).
             * 
             * Used for recognizing smart pointers, optional types, or language-level type wrappers.
             */
            virtual inline bool isWrapperType() const noexcept { return false; }

            /**
             * @brief Returns a tag indicating the specific type category (e.g., IntType, FloatType, VectorType).
             * This tag helps identify the type class during semantic analysis and codegen.
            */
            virtual DataType getTypeTag() const = 0;

            /**
             * @brief Returns the default LLVM value (e.g., zero for int) for this type.
            */
            virtual llvm::Value* getDefaultValue() = 0;

            /**
             * @brief Compares this type with another for equality.
             * 
             * This method checks whether two types represent the same semantic type.
             * It should be overridden by derived classes to perform type-specific comparisons.
             * 
             * @param other Pointer to another BaseType instance to compare against.
             * @return true if both types are considered equal, false otherwise.
            */
            virtual bool equals(const BaseType* other) const = 0;

            /**
             * @brief Checks if this type can accept another type, considering implicit conversions.
             * 
             * This method determines whether a value of the given type (`other`) can be used
             * where this type is expected. Unlike equals(), it allows for subtype relationships
             * (e.g., subclass → base class) or implicit type conversions (e.g., int → float).
             * 
             * Derived classes should override this to implement language-specific conversion rules.
             * 
             * @param other Pointer to another BaseType instance representing the source type.
             * @return true if a value of type @p other can be converted to this type, false otherwise.
            */
            virtual bool canAccept(const BaseType* other) const = 0;
        
            /**
             * @brief Creates an LLVM value of this type from an LValueType wrapper.
             * @param lvalueType Encapsulated data value to convert.
            */
            virtual llvm::Value* createValue(const LValueType value) const = 0;

            /**
             * @brief Creates an LLVM value using an existing LLVM type and value.
             * Useful for implicit conversions or LLVM-level operations.
             *
             * @param valueType The LLVM type.
             * @param value The corresponding LLVM value.
             */
            virtual llvm::Value* createValue(std::vector<llvm::Value*> values) const = 0;
           
            /**
             * @brief Creates an LLVM value from key-value input (used for associative types like Map).
             *
             * @param keyType The LLVM type of the key.
             * @param keyValue The LLVM value of the key.
            */
            virtual llvm::Value* createValue(std::vector<std::pair<llvm::Value*, llvm::Value*>> paris) const = 0;
                       
            /**
             * @brief Returns the LLVM pointer type corresponding to this base type.
             * Used when referencing or storing variables of this type.
             */
            virtual llvm::Type* getLLVMPointerType() const = 0;

            /**
             * @brief Instantiates this type, typically used for variable declarations.
             *
             * @param variableName Name of the variable being created.
             * @return LLVM value representing the instance.
            */
            virtual llvm::Value* createInstance(const std::string variableName) = 0;

            /**
             * @brief Assigns a right-hand value (rhs) to a left-hand variable (lhs).
             * @param lhs The target (e.g., alloca pointer).
             * @param rhs The value to store.
             * @return Resulting LLVM instruction.
            */
            virtual llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) = 0;

            /**
             * @brief Returns the human-readable name of this type for debugging metadata.
             * 
             * Used in DWARF debug information to label types such as "int", "float", "MyStruct".
             * 
             * @return A string representing the type's debug name.
            */
            virtual std::string getDebugName() const = 0;

            /**
             * @brief Generates the LLVM DIType (debug information) for this type.
             * 
             * Called during debug info emission (e.g., when declaring a local variable or struct layout).
             * This method should return the correct LLVM metadata node representing this type in DWARF.
             * 
             * @param scope The lexical scope (e.g., function, file, namespace) in which this type is defined.
             * @return The corresponding LLVM debug type node (e.g., DIBasicType, DICompositeType).
            */
            virtual llvm::DIType* getDIType(llvm::DIScope* scope) const = 0;

            /**
             * @brief Returns the size of this type in bits for debug metadata.
             * 
             * Used when constructing debug types to indicate layout size (e.g., for createMemberType).
             * This should match the size returned by LLVM’s DataLayout.
             * 
             * @return Size of the type in bits.
            */
            virtual uint64_t getDebugSizeInBits() const = 0;

            /**
             * @brief Returns the alignment of this type in bits for debug metadata.
             * 
             * Used in debug info generation to align fields properly in memory layout.
             * Should be computed using LLVM’s DataLayout (getPrefTypeAlignment).
             * 
             * @return Alignment in bits.
            */
            virtual uint32_t getDebugAlignInBits() const = 0;

            /**
             * @brief Returns DWARF debug flags for this type (e.g., const, static).
             * 
             * These flags are used when generating DWARF debug metadata (e.g., using LLVM's DIBuilder)
             * to indicate qualifiers such as `const`, `static`, or `volatile`.
             * 
             * Override this method in derived types to reflect the appropriate flags.
             * Default should be `llvm::DINode::FlagZero` for unqualified types.
             * 
             * @return A set of DWARF flags from `llvm::DINode::DIFlags`.
            */
            virtual llvm::DINode::DIFlags getDIFlags() const = 0;

            /**
             * @brief Clones the current type polymorphically.
             * @return A unique pointer to a new copy of the derived type.
             */
            virtual std::unique_ptr<BaseType> clone() const = 0;

            /// @brief Virtual destructor for proper cleanup of derived types.
            virtual ~BaseType() {};
    };
}

#endif
