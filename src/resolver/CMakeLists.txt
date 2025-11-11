file(GLOB_RECURSE RESOLVER_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/methods/impl/*.cpp"
)

file(GLOB_RECURSE RESOLVER_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/methods/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/strategies/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/TypeVisitor.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/TypeMethodResolver.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/TypeResolverFactory.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/TypeNumericResolver.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/TypeResolverMixins.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/TypeResolverConstant.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/TypeNumericConstants.hpp"
)

add_library(LynxResolver STATIC ${RESOLVER_SOURCES} ${RESOLVER_HEADER})

target_include_directories(LynxResolver PUBLIC ${LIBRARY_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(LynxResolver PUBLIC 
    spdlog::spdlog
    cxxopts::cxxopts 
    LynxLogger 
    LynxExceptions 
    LynxTypes 
    LynxConstants 
    LynxContext
    LynxAst
)

# if(MSVC)
#     target_compile_options(LynxResolver PRIVATE /W4 /permissive-)
# else()
#     target_compile_options(LynxResolver PRIVATE -Wall -Wextra -Wpedantic -Werror)
# endif()

target_compile_features(LynxResolver PUBLIC cxx_std_20)

