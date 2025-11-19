set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(LIB_BIZE_MODULE_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/RuntimeClass.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/RuntimeFunction.hpp

  ${CMAKE_CURRENT_SOURCE_DIR}/RuntimeModule.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/RuntimeClassRegistry.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/RuntimeFunctionRegistry.hpp

  ${CMAKE_CURRENT_SOURCE_DIR}/RuntimeModuleFactory.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/RuntimeModuleLoader.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/RuntimeModuleLoader.cpp
)

set(LIB_FILE_SYSTEM_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/filesystem/FilesystemModule.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/filesystem/FilesystemModule.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/filesystem/file/FileClass.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/filesystem/file/FileFunctions.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/filesystem/file/FileFunctionTypes.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/filesystem/file/includes/FSCloseFunction.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/filesystem/file/includes/FSExistsFunction.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/filesystem/file/includes/FSOpenFunction.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/filesystem/file/includes/FSReadFunction.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/filesystem/file/includes/FSWriteFunction.hpp

  ${CMAKE_CURRENT_SOURCE_DIR}/filesystem/directory/DirectoryFunctions.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/filesystem/directory/DirectoryFunctions.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/filesystem/path/PathFunctions.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/filesystem/path/PathFunctions.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/filesystem/util/FSUtilFunctions.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/filesystem/util/FSUtilFunctions.cpp
)

set(LIB_FILE_COLLECTIONS_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/collections/CollectionModule.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/collections/CollectionModule.cpp

  #ARRRAY
  ${CMAKE_CURRENT_SOURCE_DIR}/collections/array/ArrayClass.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/collections/array/ArrayFunctions.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/collections/array/includes/AtFunction.hpp
)

add_library(LynxLibRuntime STATIC 
  ${LIB_BIZE_MODULE_SOURCES} 
  ${LIB_FILE_SYSTEM_SOURCES}
  ${LIB_FILE_COLLECTIONS_SOURCES}
)

set_target_properties(LynxLibRuntime PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_link_libraries(LynxLibRuntime PRIVATE
  LynxAst
  LynxContext
  LynxLogger
  LynxTypes
  LynxConstants
)


target_include_directories(LynxLibRuntime PUBLIC ${LIBRARY_INCLUDES})



