# 🦊 Lynx

**Lynx** is a new experimental **programming language** written in **C++**, designed to combine the best of **Object-Oriented Programming (OOP)** and **Functional Programming (FP)** paradigms.

Lynx aims to improve **developer productivity**, **code maintainability**, and **cross-platform flexibility** by blending expressive language features with modern runtime technologies such as **JIT compilation** and **Boehm Garbage Collection**.

---

## 🚀 Vision

Lynx explores how modern programming languages can unify OOP and FP to create a more balanced, intuitive, and scalable development experience.

### Core Principles
- ⚙️ Combine **OOP** and **FP** in one coherent design  
- 💡 Focus on **clarity**, **safety**, and **developer experience**  
- ⚡ Use **JIT compilation** for optimized runtime performance  
- 🧹 Include **automatic memory management** with Boehm GC  
- 💻 Fully **cross-platform** — works across different systems

---

## 🧩 Current Features

Lynx is still under active development, currently focusing on **object-oriented concepts**.  

### Supported So Far
- ✅ **Encapsulation**, **Abstraction**, **Composition**, and **Aggregation**  
- ✅ **Classes**, **Interfaces**, **Enums**, and **Mixins**  
- 🧱 Foundation for runtime type and object systems  
- 🧰 Experimental JIT integration

### Coming Next
- 🧠 Functional programming features (immutability, lambdas, pattern matching)  
- 📦 Extended standard library  
- 🧩 Compiler optimizations and improved runtime  
- 💬 REPL and scripting support

---

## 🛠️ Build & Run

### Requirements
- C++17 or newer  
- CMake 3.10+  
- Boehm GC  
- (Optional) LLVM or compatible JIT backend  

### Build Instructions
```bash
git clone https://github.com/kofinder/Lynx.git
cd Lynx
mkdir build && cd build
cmake ..
make
```

---

## 💡 Example Concept

```cpp
// Example of OOP-style syntax in Lynx (conceptual)
interface Drawable {
    void draw();
}

class Shape : Drawable {
    int x, y;
    void draw() {
        print("Drawing Shape at (" + x + ", " + y + ")");
    }
}

mixin Color {
    string color;
}

class Circle : Shape, Color {
    int radius;
    void draw() {
        print("Drawing " + color + " circle with radius " + radius);
    }
}
```

---

## 🤝 Contributing

Lynx is an open, community-driven language project.  
We’re looking for **enthusiastic C++ developers** who want to explore programming language design and compiler development.

### How to Contribute
1. Fork the repository  
2. Create a new branch (`feature/your-feature`)  
3. Commit and push your changes  
4. Open a Pull Request

---

## ⚖️ License

This project is licensed under the **MIT License**.  
You’re free to use, modify, and distribute it with proper attribution.

---

## 🌟 Acknowledgements

- **Boehm GC** for garbage collection  
- **LLVM/JIT** for runtime performance  
- The **open-source community** for continuous inspiration  

---

> 🧠 *Lynx is an evolving programming language experiment — aiming to merge the power of OOP and FP into a clean, modern, and productive developer experience.*

## Credit
- [Flex & Bison C++ Example](https://panthema.net/2007/flex-bison-cpp-example/)
- [Jonathan Beard Tutorial on Flex & Bison](https://www.jonathanbeard.io/tutorials/FlexBisonC%2B%2B)
- [LLVM Language Reference – Identifiers](https://llvm.org/docs/LangRef.html#identifiers)
- [GNU Bison Manual – Location Type](https://www.gnu.org/software/bison/manual/html_node/Location-Type.html)
- [C++ Core Guidelines – Source](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-source)
- [LLVM Documentation (PDF)](https://readthedocs.org/projects/llvm/downloads/pdf/latest/)
- [Poco Project Documentation](https://pocoproject.org/documentation.html)
- [Exploring OS – Source Tutorials](https://mohitmishra786.github.io/exploring-os/src/)
- [ISPC](https://github.com/ispc/ispc)
- [Flex Manual – Functions and Macros](https://westes.github.io/flex/manual/Index-of-Functions-and-Macros.html#Index-of-Functions-and-Macros)
- [GNU Bison Manual – C++ Unions](https://www.gnu.org/software/bison/manual/bison.html#C_002b_002b-Unions)
- [GCC Wiki](https://gcc.gnu.org/wiki/HomePage)
- [Boehm Garbage Collector](https://www.hboehm.info/gc/)
- [Resource Allocation Graph Theory](https://mohitmishra786.github.io/exploring-os/src/day-42-resource-allocation-graph-theory.html)
- [C++ Draft Standard](https://eel.is/c++draft/)
- [GCC Predefined Macros](https://gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html)
- [Microsoft C++ Preprocessor Documentation](https://learn.microsoft.com/en-us/cpp/preprocessor/preprocessor?view=msvc-170)
- [Skia Graphics API](https://skia.org/docs/user/api/)
- [Compiler Explorer](https://godbolt.org/)
- [LSP Documentation](https://microsoft.github.io/language-server-protocol/)


## Obfuscated C Code Contest
- [IOCCC Source Code Repository](https://github.com/ioccc-src/winner)
