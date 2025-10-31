set(LIB_COMMON_TYPES_HPP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/BaseType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/BuiltInType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/WrapperType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/UserDefinedType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/CollectionType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/SequentialType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/AssociativeType.hpp
)

set(LIB_COMMON_TYPES_CPP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/interfaces/impl/BaseType.cpp
)

set(LIB_BUILTIN_TYPES_HPP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/builtins/ByteType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/builtins/CharType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/builtins/StringType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/builtins/BooleanType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/builtins/ShortType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/builtins/IntegerType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/builtins/LongType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/builtins/FloatType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/builtins/DoubleType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/builtins/VoidType.hpp
)

set(LIB_BUILTIN_TYPES_CPP_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/builtins/impl/ByteType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/builtins/impl/CharType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/builtins/impl/StringType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/builtins/impl/BooleanType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/builtins/impl/ShortType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/builtins/impl/LongType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/builtins/impl/IntegerType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/builtins/impl/FloatType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/builtins/impl/DoubleType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/builtins/impl/VoidType.cpp
)

set(LIB_SEQUENTIAL_TYPES_HPP_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/sequential/ArrayType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sequential/VectorType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sequential/ListType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sequential/StackType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sequential/QueueType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sequential/SetType.hpp
)

set(LIB_SEQUENTIAL_TYPES_CPP_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/sequential/impl/ArrayType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sequential/impl/VectorType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sequential/impl/ListType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sequential/impl/StackType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sequential/impl/QueueType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sequential/impl/SetType.cpp
)

set(LIB_ASSOCIATIVE_TYPES_HPP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/associative/MapType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/associative/DictionaryType.hpp
)

set(LIB_ASSOCIATIVE_TYPES_CPP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/associative/impl/MapType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/associative/impl/DictionaryType.cpp
)

set(LIB_USER_DEFINED_TYPES_HPP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/userdefined/EnumType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/userdefined/FileType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/userdefined/DateType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/userdefined/ClassType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/userdefined/MixinType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/userdefined/DateTimeType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/userdefined/FunctionType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/userdefined/InterfaceType.hpp
)

set(LIB_USER_DEFINED_TYPES_CPP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/userdefined/impl/EnumType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/userdefined/impl/FileType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/userdefined/impl/DateType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/userdefined/impl/ClassType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/userdefined/impl/MixinType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/userdefined/impl/FunctionType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/userdefined/impl/DateTimeType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/userdefined/impl/InterfaceType.cpp
)

set(LIB_WRAPPER_TYPES_HPP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/wrapper/PointerType.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/wrapper/ReferenceType.hpp
)

set(LIB_WRAPPER_TYPES_CPP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/wrapper/impl/PointerType.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/wrapper/impl/ReferenceType.cpp
)

set(LIB_INFERENCE_TYPES_HPP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/infrerence/AutoType.hpp
)

set(LIB_INFERENCE_TYPES_CPP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/infrerence/impl/AutoType.cpp
)

set(LIB_TEMPLATE_TYPES_HPP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/tmpl/TypeCaster.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tmpl/TypeChecker.hpp
)

set(LIBRARY_INCLUDES "./" "${CMAKE_BINARY_DIR}/configured_files/include")

add_library(LynxTypes STATIC 
    ${LIB_BUILTIN_TYPES_HPP_SOURCES} 
    ${LIB_BUILTIN_TYPES_CPP_SOURCES}
    ${LIB_SEQUENTIAL_TYPES_HPP_SOURCES}
    ${LIB_SEQUENTIAL_TYPES_CPP_SOURCES}
    ${LIB_ASSOCIATIVE_TYPES_HPP_SOURCES}
    ${LIB_ASSOCIATIVE_TYPES_CPP_SOURCES}
    ${LIB_USER_DEFINED_TYPES_HPP_SOURCES}
    ${LIB_USER_DEFINED_TYPES_CPP_SOURCES}
    ${LIB_WRAPPER_TYPES_HPP_SOURCES}
    ${LIB_WRAPPER_TYPES_CPP_SOURCES}
    ${LIB_INFERENCE_TYPES_HPP_SOURCES}
    ${LIB_INFERENCE_TYPES_CPP_SOURCES}
    ${LIB_COMMON_TYPES_HPP_SOURCES} 
    ${LIB_COMMON_TYPES_CPP_SOURCES}
    ${LIB_TEMPLATE_TYPES_HPP_SOURCES}
)

target_include_directories(LynxTypes PUBLIC ${LIBRARY_INCLUDES})

target_link_libraries(LynxTypes PRIVATE
    LynxErrors
    LynxLogger 
    LynxConstants 
    LynxContext 
    LynxResolver
)