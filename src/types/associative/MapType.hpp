/**
 * @file MapType.hpp
 * @brief Defines the MapType class representing associative key-value mappings in the Lynx type system.
 *
 * The `MapType` encapsulates dynamically sized collections that map unique keys to values.
 * It provides key-based access, insertion, assignment, and LLVM IR generation support for maps.
 *
 * **Key Responsibilities:**
 * - Represents associative maps with specific key and value types.
 * - Supports variable instantiation, assignment, and value creation.
 * - Provides key-based element access and manipulation utilities.
 * - Integrates with `TypeVisitor` for semantic checks.
 * - Produces DWARF-compatible debug information for map types.
 *
 * **Integration Points:**
 * - Used in map declarations, key-based retrieval, and iteration.
 * - Keys and values can be any built-in or user-defined types.
 * - Provides default values for keys and values where applicable.
 *
 * **LLVM Details:**
 * - Maps to LLVM types suitable for associative storage and efficient key-value access.
 * - Provides pointer types for IR generation and element manipulation.
 *
 * **Additional Features:**
 * - Supports querying both key and value types.
 * - Supports type equality checks and cloning for map types.
 *
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_MAP_TYPE_HPP
#define LYNX_MAP_TYPE_HPP

#include <types/interfaces/AssociativeType.hpp>

namespace LynxTypes {

    class MapType : public AssociativeType {

        protected:
        
            llvm::Type* computeLLVMType() const override;

            const BaseType* createWithStatic(bool newIsStatic) const override;

            const BaseType* createWithConst(bool newIsConst) const override;

        public:

            // Use explicit constructor for RAII
            explicit MapType(AstContext* context) : AssociativeType(context) {}

            // Public copy constructor, needed for clone()
            MapType(const MapType& other) : AssociativeType(other.getContext()) {
                setConst(other.isConst());
                setStatic(other.isStatic());
            }

            // Rule of five: allow default destructor, delete others
            ~MapType() override = default;
            MapType& operator=(const MapType&) = delete;
            MapType(MapType&&) = delete;
            MapType& operator=(MapType&&) = delete;

            // Clone: polymorphic RAII-safe copy
            std::unique_ptr<BaseType> clone() const override {
                return std::make_unique<MapType>(*this);
            }
            
            bool isAssociative() const noexcept override { return true; }

            DataType getTypeTag() const override { return DataType::MAP; }

            llvm::Value* createInstance(const std::string& variableName) override;

            llvm::Value* assignTo(llvm::Value* lhs, llvm::Value* rhs) override;

            llvm::Value* createValue(std::vector<std::pair<llvm::Value*, llvm::Value*>> pairs) const override;

            llvm::Type* getLLVMPointerType() const override;

            llvm::Value* getDefaultValue() override;

            bool equals(const BaseType* other) const override;

            std::string getDebugName() const override { return "map"; }

            llvm::DIType* getDIType(llvm::DIScope* scope) const override;

            uint64_t getDebugSizeInBits() const override;

            uint32_t getDebugAlignInBits() const override;

            llvm::DINode::DIFlags getDIFlags() const override;
    };
}

#endif
