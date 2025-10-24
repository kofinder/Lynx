# Lynx JIT Module

This is the **JIT (Just-In-Time) compilation module** for the Lynx project, built using [LLVM](https://llvm.org/).  
It provides a unified interface to dynamically select and run JIT backends (like **MCJIT** and **ORCJIT**) through an abstract `IJIT` interface and factory class.

---

## 📁 Folder Structure

```
src/jit/
├── IJIT.hpp               # Abstract interface for all JIT engines
├── MCJITEngine.hpp        # MCJIT backend header
├── ORCJITEngine.hpp       # ORCJIT backend header
├── JITFactory.hpp         # Factory class to create JIT engines dynamically
├── JITConstants.hpp       # Enum JITType { MCJIT, ORCJIT }
├── JIT.hpp                # High-level wrapper (entry point to the JIT system)
├── impl/
│   ├── JIT.cpp            # Implementation of JIT.hpp
│   ├── MCJITEngine.cpp    # MCJIT engine implementation
│   └── ORCJITEngine.cpp   # ORCJIT engine implementation
├── CMakeLists.txt         # Build instructions (uses LLVM)
```

---

## ✨ Features

- **Supports Multiple Backends**: MCJIT and ORCJIT (LLJIT)
- **Factory Pattern**: Select the backend at runtime
- **Modular and Extendable**: Easily add new engines
- **Clean Abstractions**: Common interface via `IJIT`

---

## 🛠️ Building (Local Module Only)

Assuming you're already in `Lynx/src/jit` and LLVM is installed:

### With CMake

```bash
mkdir build && cd build
cmake ..
make
```

You may need to set:

```bash
export LLVM_DIR=/path/to/llvm/lib/cmake/llvm
```

Or modify `CMakeLists.txt` to find your LLVM installation properly.

---

## 🚀 Example Usage

```cpp
#include "JITFactory.hpp"
#include "JITConstants.hpp"

using namespace LynxJIT;

int main() {
    // Choose a JIT engine at runtime
    std::unique_ptr<IJIT> jit = JITFactory::create(JITType::ORCJIT);

    // Load/generate an LLVM module externally
    std::unique_ptr<llvm::Module> module = generateModule();

    // Initialize and execute
    jit->initialize(std::move(module));
    int result = jit->execute();

    return result;
}
```

---

## 🧩 Extending the JIT

To add a new backend:
1. Create a new class derived from `IJIT`.
2. Implement `initialize()` and `execute()`.
3. Add the case to `JITFactory::create()`.
4. Add enum value in `JITConstants.hpp`.

---

## 📝 License

This project is licensed under the MIT License. See [LICENSE](../../LICENSE) for details.

---

## 🤝 Contributing

Contributions, issues, and feature requests are welcome.  
Please open a PR or issue on the main Lynx repository.
