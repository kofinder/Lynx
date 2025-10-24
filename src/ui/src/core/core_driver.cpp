#include "ast_core.hpp"
#include <cstdio>

extern int yyparse();
extern FILE *yyin;
extern ASTNode* root;

bool parseCoreFile(const char* filename) {
    yyin = fopen(filename, "r");
    if (!yyin) {
        perror("Could not open core file");
        return false;
    }
    if (yyparse() != 0) {
        fclose(yyin);
        return false;
    }
    fclose(yyin);
    return true;
}

void dumpCoreAST() {
    if (root) root->dump();
    else printf("No AST generated.\n");
}
