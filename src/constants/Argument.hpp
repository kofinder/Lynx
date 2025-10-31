#ifndef LYNX_ARGUMENT_HPP
#define LYNX_ARGUMENT_HPP  

#include <ast/Node.hpp>

using namespace LynxAst;

namespace LynxConstants {

    class Argument {

        public:
        
            std::unique_ptr<Node> expression;

            explicit Argument(std::unique_ptr<Node> expr): expression(std::move(expr)) {}

            std::unique_ptr<Argument> clone() const {
                return std::make_unique<Argument>(expression ? expression->clone() : nullptr);
            }
    };

}

#endif 
