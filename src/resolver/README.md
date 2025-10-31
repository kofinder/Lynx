# M🧠 Lynx Type Resolver System

This module defines how the **Lynx compiler** resolves methods and behaviors on types like `int`, `DateTime`, etc. It leverages design patterns to support clean code generation and semantic resolution.

---

## 🧹 Key Components

### `BaseType`

- Abstract interface implemented by all types in the system (e.g., `IntType`, `DateTimeType`)
- Supports common APIs like:
  - `createInstance()`
  - `getDefaultValue()`
  - `accept(visitor)`

### `TypeMethodResolver`

- Interface for resolving type-specific method calls
- Implemented per type (e.g., `IntMethodResolver`, `DateTimeMethodResolver`)
- Emits LLVM IR for supported methods

### `TypeVisitor`

- Visitor pattern interface to operate over all supported types
- Allows traversal/analysis without if-else chains

### `IntMethodResolver`, `DateTimeMethodResolver`

- Concrete classes implementing logic for methods like `abs()`, `getYear()`

### `TypeResolverFactory`

- Dynamically selects the correct `TypeMethodResolver` based on type name

---

## ✅ Supported Design Patterns

| Pattern                | Purpose                                 | Where It's Used                     |
| ---------------------- | --------------------------------------- | ----------------------------------- |
| Visitor                | Separate operations from type hierarchy | `TypeVisitor`, `BaseType::accept()` |
| Factory                | Create method resolvers dynamically     | `TypeResolverFactory`               |
| Interface/Polymorphism | Generalize behavior across all types    | `BaseType`, `TypeMethodResolver`    |
| Separation of Concerns | Keep IR codegen logic out of AST nodes  | `TypeMethodResolver`, `TypeVisitor` |
| Lazy Initialization    | Avoid repeated LLVM type creation       | `DateTimeType::computeLLVMType()`   |

---

## 🛠️ Example Workflow

```cpp
// 1. Create a type dynamically (e.g., for variable of type "DateTime")
std::unique_ptr<BaseType> type = std::make_unique<DateTimeType>();

// 2. Get a resolver for that type
auto resolver = type->createMethodResolver();

// 3. Resolve a method call (e.g., dt.getYear())
llvm::Value* result = resolver->resolveMethod("getYear", instance, {}, astContext);
```

---

## 📁 Folder Structure

```bash
resolver/
├── TypeMethodResolver.hpp          # Interface for method dispatch
├── TypeVisitor.hpp                 # Visitor base class
├── IntMethodResolver.hpp/.cpp      # Int method logic
├── DateTimeMethodResolver.hpp/.cpp # DateTime method logic
├── impl/                           # Implementation directory
├── CMakeLists.txt
└── README.md                       # You are here
```

---

## ➕ Adding a New Type

To add a new type (e.g., `BoolType`):

1. **Inherit from **``**:**

```cpp
class BoolType : public BaseType {
    // implement required methods
};
```

2. **Implement a **``**:**

```cpp
class BoolMethodResolver : public TypeMethodResolver {
    // implement method resolution logic
};
```

3. **Register it in **``**:**

```cpp
if (typeName == "bool") {
    return std::make_unique<BoolMethodResolver>();
}
```

---

Happy compiling! 🚀
