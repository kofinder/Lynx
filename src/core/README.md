# LynxCore Runtime

LynxCore provides foundational runtime services for the Lynx programming language and JIT compiler.
This includes memory management, threading, interop, and scheduling systems. Designed with modularity,
debuggability, and long-term sustainability in mind.

---

## 📁 Project Structure

```
lynx-core/
├── bindings/
│   ├── RuntimeBindingManager.cpp
│   └── RuntimeBindingManager.hpp
├── interop/
│   ├── InteropManager.cpp
│   └── InteropManager.hpp
├── memory/
│   ├── MemoryManager.cpp
│   └── MemoryManager.hpp
├── scheduler/
│   ├── SchedulerManager.cpp
│   └── SchedulerManager.hpp
├── threads/
│   ├── ThreadManager.cpp
│   └── ThreadManager.hpp
├── CMakeLists.txt
└── README.md
```

---

## 🧠 Modules Overview

### ✅ Memory
- Uses Boehm GC (`GC_malloc`)
- Tracks allocations with optional labels
- Reports leaks for diagnostics

### ✅ Threads
- Lightweight wrapper over `pthread`
- Simplifies thread creation, join, and self

### ✅ Scheduler
- Task-based scheduler using worker thread
- Handles async execution in a queue

### ✅ Bindings
- Declares runtime-external symbols (e.g., `GC_malloc`)
- Integrates with JIT-compiled LLVM modules

### ✅ Interop
- (Planned) Support for native FFI
- Marshalling between native & Lynx types

---

## ⚙️ Usage Example

```cpp
#include <memory/MemoryManager.hpp>
#include <threads/ThreadManager.hpp>
#include <scheduler/SchedulerManager.hpp>

using namespace LynxCore;

void* my_alloc = MemoryManager::allocate(128, "example");

SchedulerManager scheduler;
scheduler.start();
scheduler.enqueueTask([]() {
    printf("Async task running.\n");
});
scheduler.shutdown();
```

---

## 🛠️ Building with CMake

```bash
cd lynx-core
mkdir build && cd build
cmake ..
make
```

---

## 🧩 Extending

You can extend the runtime by adding:
- Custom allocators (e.g., arena)
- Advanced task schedulers (multi-threaded)
- FFI bridges in the `interop` module

---
