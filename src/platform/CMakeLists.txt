set(LIB_MODULE_HPP_SOURCES 
  ${CMAKE_CURRENT_SOURCE_DIR}/Triple.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Bitcode.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/TargetRegistry.hpp
)

set(LIB_MODULE_CPP_SOURCES 
  ${CMAKE_CURRENT_SOURCE_DIR}/impl/Triple.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/impl/Bitcode.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/impl/TargetRegistry.cpp
)

add_library(LynxPlatform STATIC ${LIB_MODULE_HPP_SOURCES} ${LIB_MODULE_CPP_SOURCES})

llvm_map_components_to_libnames(LLVM_LIBS core native support codegen)

target_link_libraries(LynxPlatform PRIVATE LynxLogger LynxConstants ${LLVM_LIBS})

target_include_directories(LynxPlatform PUBLIC ${LIBRARY_INCLUDES})



