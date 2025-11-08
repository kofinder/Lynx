find_package(LLVM REQUIRED CONFIG)
message(STATUS "LLVM_DIR ${LLVM_DIR}")
message(STATUS "LLVM_FOUND ${LLVM_FOUND}")
message(STATUS "CMAKE_ROOT ${CMAKE_ROOT}")
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}")
message(STATUS "LLVM_INCLUDE_DIRS: ${LLVM_INCLUDE_DIRS}")
message(STATUS "LLVM_DEFINITIONS: ${LLVM_DEFINITIONS}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")


find_package(Boost REQUIRED COMPONENTS system filesystem program_options)

add_definitions(${LLVM_DEFINITIONS})
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR} 
    ${CMAKE_CURRENT_BINARY_DIR} 
    ${LLVM_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    /usr/lib/llvm-14/include
)

add_subdirectory(lang)
add_subdirectory(constants)
add_subdirectory(exceptions)
add_subdirectory(logger)
add_subdirectory(context)
add_subdirectory(types)
add_subdirectory(ast)
add_subdirectory(mangler)
add_subdirectory(resolver)
add_subdirectory(jit)
add_subdirectory(config)
add_subdirectory(codegen)
add_subdirectory(linker)
add_subdirectory(attributes)
add_subdirectory(callconv)
add_subdirectory(analyzer)
add_subdirectory(lto)
add_subdirectory(core)
add_subdirectory(ui)
add_subdirectory(platform)
add_subdirectory(libruntime)
add_subdirectory(collections)
add_subdirectory(errors)
add_subdirectory(cli)
add_subdirectory(system)
add_subdirectory(stdapi)

set(MAIN_ENTRY_SOURCES main.cpp Lynx.hpp Lynx.cpp)

add_executable(${LYNX_EXECUTABLE} ${MAIN_ENTRY_SOURCES})

llvm_map_components_to_libnames(LLVM_LIBRARIES 
    mcjit 
    executionengine 
    native 
    support 
    core 
    passes
    irreader 
    codegen 
    mc 
    mcparser 
    
    nativecodegen
    x86codegen
    x86desc
    x86asmparser
    x86info  
    option
)

target_link_libraries(Lynx PUBLIC 
    LynxLogger
    LynxExceptions
    LynxConstants
    LynxContext
    LynxTypes
    LynxLang
    LynxAst
    LynxMangler
    LynxJIT
    LynxResolver
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
    LynxRuntime
    LynxPlatform
    LynxCollection
    LynxUI
    ${LLVM_LIBRARIES}
    ${OPENCL_CLANG_LINK_LIBS}
    Boost::system 
    Boost::filesystem
    Boost::program_options
    yaml-cpp
)


# ===``````````````````````==== #
# === LSP server executable === # 
# ===``````````````````````==== #

set(LSP_SOURCES
    lsp/lsp_server.hpp
    lsp/lsp_server.cpp
    lsp/lsp_protocol.hpp
    lsp/lsp_app.cpp
)

add_executable(LynxLSPServer ${LSP_SOURCES})

target_include_directories(LynxLSPServer PRIVATE src/lang)

target_link_libraries(LynxLSPServer PRIVATE 
    LynxLogger
    LynxExceptions
    LynxConstants
    LynxContext
    LynxTypes
    LynxLang
    LynxAst
    LynxMangler
    LynxResolver
    LynxProgramConfig
    LynxCodegen
    LynxLinker
    LynxFunctionAttr
    LynxCallConv
    LynxAnalyzer
    LynxLTO
    LynxCore
    LynxLibRuntime
    LynxErrors
    LynxRuntime
    LynxCollection
    LynxSystem
    nlohmann_json::nlohmann_json
    Boost::system
    Boost::filesystem
    Boost::program_options
)
