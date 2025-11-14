# ==========================
# === LLVM 21 Configuration
# ==========================
set(LLVM_DIR "/usr/local/llvm-21/lib/cmake/llvm" CACHE PATH "Path to LLVM 21Config.cmake")
set(LLVM_USE_SHARED_LIBS ON)
find_package(LLVM REQUIRED CONFIG)

# # Fail if major version is not 20
string(REGEX MATCH "^21" LLVM_MAJOR "${LLVM_PACKAGE_VERSION}")
if(NOT LLVM_MAJOR)
    message(FATAL_ERROR "Found LLVM version ${LLVM_PACKAGE_VERSION}, but LLVM 21.x is required")
endif()
message(STATUS "Using LLVM ${LLVM_PACKAGE_VERSION} in ${LLVM_DIR}")
message(STATUS "LLVM_INCLUDE_DIRS: ${LLVM_INCLUDE_DIRS}")
message(STATUS "LLVM_DEFINITIONS: ${LLVM_DEFINITIONS}")

# Include LLVM directories and add definitions
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${LLVM_INCLUDE_DIRS}
)
add_definitions(${LLVM_DEFINITIONS})

# ==========================
# === Boost
# ==========================
find_package(Boost REQUIRED COMPONENTS system filesystem program_options)
include_directories(${Boost_INCLUDE_DIRS})

# ==========================
# === Zstd via pkg-config
# ==========================
find_package(PkgConfig REQUIRED)
pkg_check_modules(ZSTD REQUIRED libzstd)
include_directories(${ZSTD_INCLUDE_DIRS})

# ==========================
# === Lynx modules
# ==========================
set(LYNX_MODULES
    lang constants exceptions logger context types ast mangler
    jit config codegen linker attributes callconv
    analyzer lto core ui platform libruntime collections
    errors cli system stdapi
)

foreach(module IN LISTS LYNX_MODULES)
    add_subdirectory(${module})
endforeach()

# ==========================
# === Main executable
# ==========================
set(MAIN_ENTRY_SOURCES
    main.cpp
    Lynx.hpp
    Lynx.cpp
)

add_executable(${LYNX_EXECUTABLE} ${MAIN_ENTRY_SOURCES})

# Map LLVM components to libraries (LLVM 20)
llvm_map_components_to_libnames(LLVM_LIBS
    core
    mcjit
    executionengine
    native
    nativecodegen
    orcjit
    interpreter
    support
    irreader
    codegen
    transformutils
    scalaropts
)

target_link_libraries(${LYNX_EXECUTABLE} PUBLIC
    LynxLogger 
    LynxExceptions 
    LynxConstants 
    LynxContext 
    LynxTypes
    LynxLang 
    LynxAst 
    LynxMangler 
    LynxJIT 
    LynxProgramConfig
    LynxCLI 
    LynxCodegen 
    LynxLinker 
    LynxFunctionAttr 
    LynxCallConv
    LynxAnalyzer 
    LynxLTO 
    LynxCore 
    LynxLibRuntime 
    LynxErrors
    LynxBridge 
    LynxPlatform 
    LynxCollection 
    LynxUI
    ${LLVM_LIBS} 
    ${OPENCL_CLANG_LINK_LIBS}
    Boost::system Boost::filesystem Boost::program_options
    yaml-cpp
    ${ZSTD_LIBRARIES}
)

# ==========================
# === LSP Server
# ==========================
set(LSP_SOURCES
    lsp/lsp_server.hpp
    lsp/lsp_server.cpp
    lsp/lsp_protocol.hpp
    lsp/lsp_app.cpp
)

add_executable(LynxLSPServer ${LSP_SOURCES})
target_include_directories(LynxLSPServer PRIVATE src/lang ${ZSTD_INCLUDE_DIRS})

target_link_libraries(LynxLSPServer PRIVATE
    LynxLogger LynxExceptions LynxConstants LynxContext LynxTypes
    LynxLang LynxAst LynxMangler LynxProgramConfig
    LynxCodegen LynxLinker LynxFunctionAttr LynxCallConv LynxAnalyzer
    LynxLTO LynxCore LynxLibRuntime LynxErrors LynxBridge
    LynxCollection LynxSystem
    nlohmann_json::nlohmann_json
    Boost::system Boost::filesystem Boost::program_options
    ${ZSTD_LIBRARIES}
)

# ==========================
# === Compiler settings (C++20)
# ==========================
target_compile_features(${LYNX_EXECUTABLE} PUBLIC cxx_std_23)
target_compile_features(LynxLSPServer PUBLIC cxx_std_23)