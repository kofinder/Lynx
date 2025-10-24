
# 🧩 Lynx Name Mangler Module

This module provides a pluggable and extensible system for generating mangled names for various language entities, such as functions, classes, variables, etc.

## 📁 Directory Structure

```
src/mangler/
├── IMangleStrategy.hpp       # Interface for all mangling strategies
├── ItaniumMangler.hpp        # Itanium ABI mangler
├── ManglerFactory.hpp        # Factory to produce manglers based on type
├── ManglingEngine.hpp        # Unified entry point for mangling
├── NameManglingContext.hpp   # Context class to set and use mangler strategies
└── kinds/
    ├── ItaniumMangler.cpp
    ├── MicrosoftMangler.cpp
    └── LynxMangler.hpp
```

## 🚀 Usage

```cpp
#include <mangler/ManglingEngine.hpp>

using namespace LynxMangler;
using namespace LynxConstants;

int main() {
    ManglingEngine engine(ManglerType::ITANIUM);
    std::string mangled = engine.mangle(ManglerKind::FUNCTION, "foo", {"int", "float"});
    std::cout << "Mangled name: " << mangled << std::endl;
}
```

## 🧠 Design Patterns

### Strategy Pattern

The core mangling logic is encapsulated using the **Strategy Pattern**.

- `IMangleStrategy`: Base interface
- `ItaniumMangler`, `MicrosoftMangler`, etc.: Implement the strategy interface
- `NameManglingContext`: Maintains current strategy

### Factory Pattern

The `ManglerFactory` is responsible for creating the appropriate strategy object.

## 🔧 Adding New Manglers

1. Create a new class implementing `IMangleStrategy`
2. Implement all mangling methods
3. Register it in `ManglerFactory::create()`
