find_package(LLVM REQUIRED CONFIG)
message(STATUS "LLVM_DIR ${LLVM_DIR}")
message(STATUS "LLVM_FOUND ${LLVM_FOUND}")
message(STATUS "CMAKE_ROOT ${CMAKE_ROOT}")
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}")
message(STATUS "LLVM_INCLUDE_DIRS: ${LLVM_INCLUDE_DIRS}")
message(STATUS "LLVM_DEFINITIONS: ${LLVM_DEFINITIONS}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Boost
find_package(Boost REQUIRED COMPONENTS system filesystem program_options)

# Include LLVM and Boost directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${LLVM_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
)

add_definitions(${LLVM_DEFINITIONS})

# ==============================
# === Add subdirectories =======
# ==============================
set(LYNX_MODULES
    lang constants exceptions logger context types ast mangler
    resolver jit config codegen linker attributes callconv
    analyzer lto core ui platform libruntime collections
    errors cli system stdapi
)

foreach(module IN LISTS LYNX_MODULES)
    add_subdirectory(${module})
endforeach()

# ==========================
# === Main executable ======
# ==========================
set(MAIN_ENTRY_SOURCES
    main.cpp
    Lynx.hpp
    Lynx.cpp
)

add_executable(${LYNX_EXECUTABLE} ${MAIN_ENTRY_SOURCES})

# Map LLVM components to libraries
llvm_map_components_to_libnames(LLVM_LIBRARIES
    mcjit executionengine native support core passes irreader
    codegen mc mcparser nativecodegen
    x86codegen x86desc x86asmparser x86info option
)

# Link libraries for main executable
target_link_libraries(${LYNX_EXECUTABLE} PUBLIC
    LynxLogger LynxExceptions LynxConstants LynxContext LynxTypes
    LynxLang LynxAst LynxMangler LynxJIT LynxResolver LynxProgramConfig
    LynxCLI LynxCodegen LynxLinker LynxFunctionAttr LynxCallConv
    LynxAnalyzer LynxLTO LynxCore LynxLibRuntime LynxErrors
    LynxRuntime LynxPlatform LynxCollection LynxUI
    ${LLVM_LIBRARIES} ${OPENCL_CLANG_LINK_LIBS}
    Boost::system Boost::filesystem Boost::program_options
    yaml-cpp
)

# ==========================
# === LSP Server ===========
# ==========================
set(LSP_SOURCES
    lsp/lsp_server.hpp
    lsp/lsp_server.cpp
    lsp/lsp_protocol.hpp
    lsp/lsp_app.cpp
)

add_executable(LynxLSPServer ${LSP_SOURCES})

target_include_directories(LynxLSPServer PRIVATE src/lang)

target_link_libraries(LynxLSPServer PRIVATE
    LynxLogger LynxExceptions LynxConstants LynxContext LynxTypes
    LynxLang LynxAst LynxMangler LynxResolver LynxProgramConfig
    LynxCodegen LynxLinker LynxFunctionAttr LynxCallConv LynxAnalyzer
    LynxLTO LynxCore LynxLibRuntime LynxErrors LynxRuntime
    LynxCollection LynxSystem
    nlohmann_json::nlohmann_json
    Boost::system Boost::filesystem Boost::program_options
)

# ======================================
# === Compiler settings (modern C++) ===
# ======================================
# Enable C++20 and warnings for main Lynx target
target_compile_features(${LYNX_EXECUTABLE} PUBLIC cxx_std_20)

# # Common warnings for GCC and Clang
# set(COMMON_WARNINGS -Wall -Wextra -Wpedantic)
# target_compile_options(${LYNX_EXECUTABLE} PRIVATE
#     $<$<OR:$<CXX_COMPILER_ID:GNU>,
#     $<CXX_COMPILER_ID:Clang>>:${COMMON_WARNINGS}>
# )
