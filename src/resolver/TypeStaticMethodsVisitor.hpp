#ifndef LYNX_TYPE_STATIC_METHOD_VISITOR_HPP
#define LYNX_TYPE_STATIC_METHOD_VISITOR_HPP

#include <logger/Logger.hpp>
#include "TypeVisitor.hpp"

namespace LynxResolver {

    using namespace LynxLogger;

    class TypeStaticMethodsVisitor : public TypeVisitor {

        public:

            void visit(ByteType& type) override {
                LOG_ERROR("ByteType Static Visiter");
            }

            void visit(ShortType& type) override {
                LOG_ERROR("ShortType Static Visiter");
            }

            void visit(IntegerType& type) override {
                LOG_ERROR("IntegerType Static Visiter");
            }

            void visit(LongType& type) override {
                LOG_ERROR("LongType Static Visiter");
            }

            void visit(FloatType& type) override {
                LOG_ERROR("FloatType Static Visiter");
            }

            void visit(DoubleType& type) override {
                LOG_ERROR("DoubleType Static Visiter");
            }

            void visit(BooleanType& type) override {
                LOG_ERROR("BooleanType Static Visiter");
            }

            void visit(CharType& type) override {
                LOG_ERROR("CharType Static Visiter");
            }

            void visit(StringType& type) override {
                LOG_ERROR("StringType Static Visiter");
            }

            void visit(DateTimeType& type) override {
                LOG_ERROR("DateTimeType Static Visiter");
            }

            void visit(AutoType& type) override {
                LOG_ERROR("AutoType Static Visiter");
            }

            ~TypeStaticMethodsVisitor() override = default;
    };
}

#endif 
