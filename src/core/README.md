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

## 🧩 Extending

You can extend the runtime by adding:
- Custom allocators (e.g., arena)
- Advanced task schedulers (multi-threaded)
- FFI bridges in the `interop` module

---
