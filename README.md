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

### Requirements
- C++23 or newer  
- CMake 3.26+  
- Boehm GC  
- (Optional) LLVM or compatible JIT backend  

---

## Credit
- [Flex & Bison C++ Example](https://panthema.net/2007/flex-bison-cpp-example/)
- [2019 LLVM Developers’ Meeting: S. Haastregt & A. Stulova An overview of Clang](https://www.youtube.com/watch?v=5kkMpJpIGYU)
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
- [IOCCC Source Code Repository](https://github.com/ioccc-src/winner)
- [Clang-Tidy](https://clang.llvm.org/extra/clang-tidy/)



; ModuleID = 'main'
source_filename = "main"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%map_key = type { ptr, i64 }
%map_pair = type { ptr, i64, ptr }
%struct_map = type { ptr, i64, i64 }

@str_a = private constant [2 x i8] c"a\00"
@map_key_str_a = private constant %map_key { ptr @str_a, i64 0 }
@str_b = private constant [2 x i8] c"b\00"
@map_key_str_b = private constant %map_key { ptr @str_b, i64 0 }
@map_key.0 = private constant [1 x %map_key] [%map_key @map_key_str_a]
@map_key.1 = private constant [1 x %map_key] [%map_key @map_key_str_b]
@map_value.0 = private constant [3 x i32] [i32 1, i32 2, i32 3]
@map_value.1 = private constant [3 x i32] [i32 4, i32 5, i32 6]

@flat_array_const = private constant [2 x %map_pair] [
  %map_pair { ptr @map_key.0, i64 1, ptr @map_value.0 }, 
  %map_pair { ptr @map_key.1, i64 1, ptr @map_value.1 }
]

define dso_local i32 @main() !lynx.signature !0 {
entry:
  %flat_array_raw = call ptr @GC_malloc(i64 48)
  call void @llvm.memcpy.p0.p0.i64(ptr %flat_array_raw, ptr @flat_array_const, i64 48, i1 false)
  %map_raw = call ptr @GC_malloc(i64 24)
  %ptr_gep = getelementptr inbounds nuw %struct_map, ptr %map_raw, i32 0, i32 0
  %length_gep = getelementptr inbounds nuw %struct_map, ptr %map_raw, i32 0, i32 1
  %cap_gep = getelementptr inbounds nuw %struct_map, ptr %map_raw, i32 0, i32 2
  store ptr %flat_array_raw, ptr %ptr_gep, align 8
  store i64 2, ptr %length_gep, align 4
  store i64 2, ptr %cap_gep, align 4
  ret i32 0
}

declare ptr @GC_malloc(i64)

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #0

attributes #0 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }

!0 = !{!"main(int)"}