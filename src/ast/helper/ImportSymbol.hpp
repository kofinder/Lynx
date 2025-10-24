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
