
---

## 🧩 Key Components

### `RuntimeFunction`
- Abstract base class for all runtime-callable functions.
- Each function subclass encapsulates its logic and signature.

### `RuntimeFunctionRegistry`
- Singleton registry that maps function names to implementations.
- Used by the compiler backend to resolve and invoke runtime APIs.

### `RuntimeModule`
- Abstract class for standard runtime modules (e.g., filesystem, collections).
- Exposes `registerFunctions()` and `name()` for dynamic loading.

### `RuntimeModuleFactory`
- Factory pattern that creates instances of modules from an enum value.

### `RuntimeModuleLoader`
- Loads a module **once**, caches it, and registers its functions with the registry.

---

## ✅ Supported Design Patterns

| Pattern           | Purpose                                                   | Where It's Used                                 |
|------------------|-----------------------------------------------------------|--------------------------------------------------|
| **Factory**       | Decouple module creation from usage                       | `RuntimeModuleFactory`                          |
| **Singleton**     | Ensure a single point of access to runtime function registry | `RuntimeFunctionRegistry`                    |
| **Registry**      | Maintain a map of runtime functions by name               | `RuntimeFunctionRegistry`                      |
| **Lazy Initialization** | Avoid redundant module loads                    | `RuntimeModuleLoader::loadedModules`           |
| **Interface/Polymorphism** | Abstract modules via `RuntimeModule`         | Enables extensibility and clean dispatch       |

---

## 🛠️ Example Workflow

```cpp
// 1. Load the module (once)
RuntimeModuleLoader::loadModule(RuntimeModuleType::FILESYSTEM);

// 2. Resolve a function
auto func = RuntimeFunctionRegistry::getInstance().resolveFunction("fs::open");

// 3. Invoke it during IR generation or runtime interpretation
if (func) {
    func->invoke(...); // or emitCall(...)
}
