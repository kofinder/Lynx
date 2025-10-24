set(LIB_MODULE_LTO_STRATEGY_SOURCES 
  ${CMAKE_CURRENT_SOURCE_DIR}/strategy/DefaultLTOOptimizationStrategy.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/strategy/AdvancedLTOOptimizationStrategy.hpp 
)

set(LIB_MODULE_LTO_FACADE_SOURCES 
  ${CMAKE_CURRENT_SOURCE_DIR}/FullLTO.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/ThinLTO.hpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/LTOFacade.hpp 
)


set(LIB_MODULE_LTO_PASSES_SOURCES 
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/GlobalDCEPPass.hpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/AttributorPass.hpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/DevirtualizePass.hpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/FunctionInliningPass.hpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/FunctionMetadataPass.hpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/FunctionSecurityPass.hpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/RequireAnalysisPass.hpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/TargetIRVerifierPass.hpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/VTableExtractorPass.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/passes/AccessModifierPass.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/passes/VTableExtractorPass.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/passes/UndenfinedBehaviorPass.hpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/FunctionDocDumperPass.hpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/RemoveUnusedParamsPass.hpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/WholeProgramDevirtPass.hpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/DeadCodeEliminationPass.hpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/FunctionDocExtractorPass.hpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/FunctionSignatureAuditPass.hpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/VirtualCallOptimizationPass.hpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/DevirtualizeFunctionCallsPass.hpp
)

set(LIB_MODULE_LTO_PASSES_IMPL_SOURCES 
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/impl/GlobalDCEPPass.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/impl/AttributorPass.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/impl/DevirtualizePass.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/impl/RequireAnalysisPass.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/passes/impl/VTableExtractorPass.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/passes/impl/AccessModifierPass.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/passes/impl/UndenfinedBehaviorPass.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/impl/VTableExtractorPass.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/impl/FunctionInliningPass.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/impl/FunctionMetadataPass.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/impl/FunctionSecurityPass.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/impl/TargetIRVerifierPass.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/impl/FunctionDocDumperPass.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/impl/RemoveUnusedParamsPass.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/impl/DeadCodeEliminationPass.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/impl/WholeProgramDevirtPass.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/impl/FunctionDocExtractorPass.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/impl/FunctionSignatureAuditPass.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/impl/VirtualCallOptimizationPass.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/passes/impl/DevirtualizeFunctionCallsPass.cpp
)

add_library(LynxLTO STATIC 
  ${LIB_MODULE_LTO_STRATEGY_SOURCES} 
  ${LIB_MODULE_LTO_FACADE_SOURCES}
  ${LIB_MODULE_LTO_PASSES_SOURCES}
  ${LIB_MODULE_LTO_PASSES_IMPL_SOURCES}
)

llvm_map_components_to_libnames(LLVM_LIBS
  core
  irreader
  scalaropts
  ipo
  vectorize
  transformutils
  analysis
  passes
  support
)

target_link_libraries(LynxLTO PRIVATE LynxLogger ${LLVM_LIBS})

target_include_directories(LynxLTO PUBLIC ${LIBRARY_INCLUDES} )



