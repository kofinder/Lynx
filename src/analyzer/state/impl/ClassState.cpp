#include "state/ClassState.hpp"
#include <interfaces/SemanticAnalyzer.hpp>

namespace LynxAnalyzer {

    void ClassState::handleNode(SemanticAnalyzer& analyzer, Node* node) {}

    bool ClassState::canAccessField(const std::string& currentClass, const FieldType* method) const { return false; }

    bool ClassState::canAccessMethod(const std::string& currentClass, const MethodType* method) const { return false; }

    bool ClassState::isSubclass(const std::string& derived, const std::string& base) const { return false; }

}