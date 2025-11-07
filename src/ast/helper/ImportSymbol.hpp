/**
 * @file ImportSymbol.hpp
 * @brief Declares the ImportSymbol struct representing imported module symbols 
 *        and their optional aliases in the Lynx AST.
 * 
 * The ImportSymbol struct models a single imported entity from another module, 
 * tracking both its original identifier and any alias assigned during import. 
 * It serves as a lightweight data structure used during module resolution and 
 * namespace management.
 * 
 * **Key Responsibilities:**
 * - Stores the original symbol name and its alias (if provided).
 * - Supports aliasing for imported modules or specific members.
 * - Provides simple construction and data access for import processing.
 * 
 * **Used By:**
 * - Import declaration nodes within the AST.
 * - Module and namespace resolution components.
 * 
 * @see ImportNode, ModuleResolver
 * 
 * @note The alias field is optional and may be empty if no alias is specified 
 *       in the import statement.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: November 4, 2025
*/

#ifndef LYNX_IMPORT_SYMOBL_HPP
#define LYNX_IMPORT_SYMOBL_HPP

#include <string>

namespace LynxAst {

    struct ImportSymbol {
        std::string original;
        std::string alias;
    
        explicit ImportSymbol(
            const std::string& orig, 
            const std::string& ali = ""
        ) : original(orig), alias(ali) {}
    };
    
}

#endif 
