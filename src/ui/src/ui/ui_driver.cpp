#include "ast_ui.hpp"
#include <cstdio>

extern int yyparse();
extern FILE *yyin;
extern UIASTNode* root_ui;

bool parseUIFile(const char* filename) {
    yyin = fopen(filename, "r");
    if (!yyin) {
        perror("Could not open UI file");
        return false;
    }
    if (yyparse() != 0) {
        fclose(yyin);
        return false;
    }
    fclose(yyin);
    return true;
}

void dumpUIAST() {
    if (root_ui) root_ui->dump();
    else printf("No UI AST generated.\n");
}
