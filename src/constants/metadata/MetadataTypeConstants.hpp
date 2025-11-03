
#ifndef LYNX_METADATA_CONSTANTS_HPP
#define LYNX_METADATA_CONSTANTS_HPP

#include <string>

namespace MetadataTypeConstants {

    // *** General Metadata Prefix ***
    inline constexpr const char* prefix = "lynx";
    inline constexpr const char* lynxDataType = "lynx.type";

    inline constexpr const char* structureCharType = "lynx.char.type";
    inline constexpr const char* structureCharInstance = "lynx.char.stack";

    inline constexpr const char* structureByteType = "lynx.byte.type";
    inline constexpr const char* structureByteInstance = "lynx.byte.stack";

    inline constexpr const char* shortType = "lynx.short.type"; 
    inline constexpr const char* shortInstance = "lynx.short.stack";

    inline constexpr const char* booleanType = "lynx.boolean.type";
    inline constexpr const char* booleanInstance = "lynx.boolean.stack";

    inline constexpr const char* doubleType = "lynx.double.type";
    inline constexpr const char* doubleInstance = "lynx.double.stack";

    inline constexpr const char* floatType = "lynx.float.type";
    inline constexpr const char* floatInstance = "lynx.float.stack";

    inline constexpr const char* intType = "lynx.int.type"; 
    inline constexpr const char* intInstance = "lynx.int.stack";

    inline constexpr const char* longType = "lynx.long.type";
    inline constexpr const char* longInstance = "lynx.long.stack";

    inline constexpr const char* stringType = "lynx.string.type";
    inline constexpr const char* stringInstance = "lynx.string.stack";
    inline constexpr const char* stringConstant = "lynx.string.constant";

    inline constexpr const char* structureEnumType = "lynx.enum.type";
    inline constexpr const char* structureEnumInstance = "lynx.enum.stack";

    inline constexpr const char* voidType = "lynx.void.type";

    inline constexpr const char* fileType = "File";
    inline constexpr const char* structureDateType = "lynx.date.type";
    inline constexpr const char* dateType = "lynx.date.type";

    inline constexpr const char* structureDateTimeType = "lynx.datetime.type";
    inline constexpr const char* dateTimeType = "lynx.datetime.type";

    inline constexpr const char* autoType = "lynx.auto.type";
    inline constexpr const char* vectorType = "lynx.vector.type"; 

    inline constexpr const char* simpleArray = "simple_array"; 
    inline constexpr const char* outerArray = "outer_array"; 
    inline constexpr const char* innerArray = "inner_array"; 


    // array
    inline constexpr const char* arrayType = "lynx.array.type"; 
    inline constexpr const char* byteArrayType = "lynx.byte_array.type"; 
    inline constexpr const char* shortArrayType = "lynx.short_array.type"; 
    inline constexpr const char* intArrayType = "lynx.int_array.type"; 
    inline constexpr const char* longArrayType = "lynx.long_array.type"; 
    inline constexpr const char* floatArrayType = "lynx.float_array.type"; 
    inline constexpr const char* doubleArrayType = "lynx.double_array.type"; 
    inline constexpr const char* boolArrayType = "lynx.bool_array.type"; 
    inline constexpr const char* charArrayType = "lynx.char_array.type"; 
    inline constexpr const char* stringArrayType = "lynx.string_array.type"; 
    inline constexpr const char* clazzArrayType = "lynx.clazz_array.type"; 


    // *** Complex Data Structures Metadata ***
    inline constexpr const char* classType = "lynx.class.type";     // Class type metadata
    inline constexpr const char* interfaceType = "lynx.interface.type"; // Interface type metadata
    inline constexpr const char* structType = "lynx.struct.type";   // Struct type metadata
    inline constexpr const char* unionType = "lynx.union.type";     // Union type metadata

    // *** Array and Collection Metadata ***
    inline constexpr const char* listType = "lynx.list.type";       // List type metadata
    inline constexpr const char* mapType = "lynx.map.type";         // Map type metadata
    inline constexpr const char* setType = "lynx.set.type";         // Set type metadata

    // *** Pointer Metadata ***
    inline constexpr const char* pointerType = "lynx.pointer.type"; // Pointer type metadata
    inline constexpr const char* referenceType = "lynx.reference.type"; // Reference type metadata

    // *** Function Metadata ***
    inline constexpr const char* functionType = "lynx.function.type"; // Function type metadata
    inline constexpr const char* lambdaType = "lynx.lambda.type";     // Lambda or closure type metadata

    // *** Variable Metadata ***
    inline constexpr const char* globalVariable = "lynx.global.variable"; // Global variable metadata
    inline constexpr const char* localVariable = "lynx.local.variable";   // Local variable metadata
    inline constexpr const char* constantVariable = "lynx.constant.variable"; // Constant variable metadata
    inline constexpr const char* staticVariable = "lynx.static.variable";     // Static variable metadata

    // *** Debug and Profiling Metadata ***
    inline constexpr const char* debugTypeInfo = "lynx.debug.typeinfo"; // Debug information for type
    inline constexpr const char* profilingInfo = "lynx.profiling.info"; // Profiling information
    inline constexpr const char* memoryAccessInfo = "lynx.memory.access"; // Memory access profiling

    // *** Security Metadata ***
    inline constexpr const char* sensitiveData = "lynx.sensitive.data"; // Mark sensitive data
    inline constexpr const char* secureVariable = "lynx.secure.variable"; // Mark secure variables
    inline constexpr const char* integrityCheck = "lynx.integrity.check"; // Metadata for integrity checks

    // *** Custom Metadata ***
    inline constexpr const char* customAnnotation = "lynx.custom.annotation"; // Custom annotations
    inline constexpr const char* experimentalFeature = "lynx.experimental.feature"; // Experimental feature marker
    inline constexpr const char* documentation = "lynx.documentation"; // Attach documentation

    // ==================== Arithmetic Operations ====================
    inline constexpr const char* OPR_ADD       = "arith.add";        // Addition
    inline constexpr const char* OPR_SUB       = "arith.sub";        // Subtraction
    inline constexpr const char* OPR_MUL       = "arith.mul";        // Multiplication
    inline constexpr const char* OPR_DIV       = "arith.div";        // Division
    inline constexpr const char* OPR_MOD       = "arith.mod";        // Modulus
    inline constexpr const char* OPR_XOR       = "arith.xor";        // Bitwise XOR

    // ==================== Temporary Results for Arithmetic ====================
    inline constexpr const char* TMP_SUM       = "tmp.sum";          // Result of addition
    inline constexpr const char* TMP_DIFF      = "tmp.diff";         // Result of subtraction
    inline constexpr const char* TMP_PROD      = "tmp.prod";         // Result of multiplication
    inline constexpr const char* TMP_QUOT      = "tmp.quot";         // Result of division
    inline constexpr const char* TMP_XOR       = "tmp.xor";          // Result of XOR

    // ==================== Boolean Operations ====================
    inline constexpr const char* OPR_AND       = "bool.and";         // Logical AND
    inline constexpr const char* OPR_OR        = "bool.or";          // Logical OR
    inline constexpr const char* OPR_NOT       = "bool.not";         // Logical NOT

    // ==================== Temporary Boolean ====================
    inline constexpr const char* TMP_BOOL      = "tmp.bool";         // Boolean result temporary

    // ==================== Comparison Operations ====================
    inline constexpr const char* OPR_GT        = "cmp.gt";           // Greater than
    inline constexpr const char* OPR_LT        = "cmp.lt";           // Less than
    inline constexpr const char* OPR_GE        = "cmp.ge";           // Greater or equal
    inline constexpr const char* OPR_LE        = "cmp.le";           // Less or equal
    inline constexpr const char* OPR_EQ        = "cmp.eq";           // Equal
    inline constexpr const char* OPR_NE        = "cmp.ne";           // Not equal

    // ==================== Unary Operations ====================
    inline constexpr const char* OPR_NEG       = "unary.neg";        // Unary minus
    inline constexpr const char* OPR_INC       = "unary.inc";        // Increment
    inline constexpr const char* OPR_DEC       = "unary.dec";        // Decrement
    inline constexpr const char* OPR_BIT_NOT   = "unary.bitnot";     // Bitwise NOT (~)

    // ==================== Temporary Unary Results ====================
    inline constexpr const char* TMP_NEG       = "tmp.neg";          // Result of negation
    inline constexpr const char* TMP_INC       = "tmp.inc";          // Result of increment
    inline constexpr const char* TMP_DEC       = "tmp.dec";          // Result of decrement
    inline constexpr const char* TMP_BIT_NOT   = "tmp.bitnot";       // Result of bitwise NOT

    // ==================== Miscellaneous ====================
    inline constexpr const char* TMP_GENERIC   = "tmp.value";        // Generic unnamed temporary
    inline constexpr const char* TMP_COMPARE   = "tmp.compare";      // Result of comparison



    // // *** For arithmetic operations: ***
    // inline constexpr auto OPR_ADD_TMP = "addTmp";
    // inline constexpr const char* oprAddTmp = "sum";    // If the operation is addition.
    // inline constexpr const char* oprSubTmp = "sub_tmp";    // If the operation is subtraction.
    // inline constexpr const char* oprMulTmp = "mul_thmp";   // If the operation is multiplication.
    // inline constexpr const char* oprDivTmp = "div_tmp";    // If the operation is division.
    // inline constexpr const char* oprXor = "xor_tmp";       // If the operation is binary.


    // // *** For specific operation types: ***
    // inline constexpr const char* oprSumTmp = "lynx.sumtmp";    // If you're adding.
    // inline constexpr const char* oprDiffTmp = "lynx.difftmp";  // If you're subtracting.
    // inline constexpr const char* oprProdTmp = "lynx.prodtmp";  // If you're multiplying.
    // inline constexpr const char* oprQuotTmp = "lynx.quottmp";  // If the operation is division.


    // // *** For operations involving boolean expressions: ***
    // inline constexpr const char* oprAndTmp = "lynx.andtmp";    // If it's an AND operation.
    // inline constexpr const char* oprOrTmp = "lynx.ortmp";      // If it's an OR operation.
    // inline constexpr const char* oprBoolResult = "lynx.boolresult"; // For boolean results in general.


    // inline constexpr const char* oprGreaterThanTmp = "greaterthan_tmp"; // For boolean results in general.
    // inline constexpr const char* oprLessThanTmp = "lessthan_tmp"; // For boolean results in general.
    // inline constexpr const char* oprGreaterEqualTmp = "greaterequal_tmp"; // For boolean results in general.
    // inline constexpr const char* oprLessEqualTmp = "lessequal_tmp"; // For boolean results in general.
    // inline constexpr const char* oprEqualityTmp = "equality_tmp"; // For boolean results in general.
    // inline constexpr const char* oprNotEqualityTmp = "notequality_tmp"; // For boolean results in general.

    // inline constexpr const char* oprNegTmp = "neg_tmp";    // If the operation is subtraction.
    // inline constexpr const char* oprIncTmp = "inc_tmp";   // If the operation is multiplication.
    // inline constexpr const char* oprDecTmp = "dec_tmp";    // If the operation is division.
    // inline constexpr const char* oprNotTmp = "not_tmp";    // If the operation is division.
    // inline constexpr const char* oprXorTmp = "xor_tmp";    // If the operation is division.

}

#endif

    

