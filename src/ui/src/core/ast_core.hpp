#pragma once
#include <string>
#include <iostream>

// Base AST Node for core language
struct ASTNode {
    virtual ~ASTNode() {}
    virtual void dump() const = 0;
};

// Example: integer literal node
struct ASTInt : ASTNode {
    int value;
    ASTInt(int v) : value(v) {}
    void dump() const override { std::cout << "Int(" << value << ")"; }
};
