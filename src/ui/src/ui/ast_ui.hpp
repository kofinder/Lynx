#pragma once
#include <string>
#include <iostream>

// Base AST node for UI
struct UIASTNode {
    virtual ~UIASTNode() {}
    virtual void dump() const = 0;
};

struct UICanvas : UIASTNode {
    int width, height;
    UICanvas(int w, int h) : width(w), height(h) {}
    void dump() const override {
        std::cout << "Canvas(" << width << "x" << height << ")";
    }
};
