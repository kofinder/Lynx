# Lynx Type System

This directory defines the core type system for the Lynx language. It is organized to support both built-in and user-defined types as well as complex data structures like collections and associative types. Below is an overview of the directory structure, type categories, and their responsibilities.

---

## Directory Structure

```text
src/types
├── associative/         # Map and dictionary-like types
├── builtins/            # Primitive and language-native types (e.g., int, float)
├── infrerence/          # Type inference support (e.g., AutoType)
├── interfaces/          # Core type interfaces and base classes
├── sequential/          # Array, list, vector, queue, etc.
├── userdefined/         # Classes, enums, interfaces
├── wrapper/             # Pointer and reference types
```

---

## Type Categories

### 1. Built-in Types (`builtins/`)

These are primitive or native types supported directly by the language.

- `IntegerType`
- `FloatType`, `DoubleType`
- `ByteType`, `ShortType`, `LongType`
- `BooleanType`, `CharType`
- `StringType`, `VoidType`, `NullPtrType`

### 2. Wrapper Types (`wrapper/`)

Wrap built-in types to provide advanced semantics like pointers or references.

- `PointerType`
- `ReferenceType`

### 3. Sequential Types (`sequential/`)

Indexed collections that maintain ordering.

- `ArrayType`, `ListType`, `VectorType`
- `QueueType`, `StackType`, `SetType`

### 4. Associative Types (`associative/`)

Key-value based containers.

- `DictionaryType`
- `MapType`

### 5. User-Defined Types (`userdefined/`)

Types defined by the user using language constructs.

- `ClassType`, `InterfaceType`, `EnumType`
- `DateType`, `DateTimeType`, `FileType`

### 6. Inference Types (`infrerence/`)

Used for type inference and deduced types.

- `AutoType`

### 7. Interfaces (`interfaces/`)

Core base types and interfaces from which others derive.

- `BaseType`, `BuiltInType`, `UserDefinedType`
- `CollectionType`, `AssociativeType`, `SequentialType`
- `WrapperType`, `TypeCaster`

---

## Design Notes

- `` is the root for all types and defines core APIs like equality, cloning, and debug metadata.
- `` is the base for all primitive types and simplifies LLVM IR integration.
- **DIType Generation** for LLVM debug info is implemented per type.

---

## Possible Diagrams

### UML-like Class Hierarchy

```
                BaseType
                   |
        +----------+----------+
        |          |          |
 BuiltInType  UserDefinedType  WrapperType
     |               |             |
  IntegerType    ClassType     PointerType
  FloatType      EnumType     ReferenceType
  ...                            
```

### Module Dependency Graph