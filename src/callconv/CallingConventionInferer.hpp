/**
 * @file CallingConventionInferer.hpp
 * @brief Defines the logic for inferring LLVM calling conventions using specifications.
 * 
 * The `CallingConventionInferer` class is responsible for determining the most appropriate
 * LLVM calling convention (`CallingConventionType`) for a given `llvm::Function`.
 * 
 * It evaluates the function against a set of predefined `CallingConventionSpecification`
 * objects, each representing a distinct calling convention rule or attribute-based heuristic.
 * 
 * This system is extensible: new specifications can be registered easily by
 * adding them to the constructor.
 * 
 * @see CallingConventionSpecification
 * @see AndSpecification
 * @see HasAttributeSpec
 * @see IsVarArgSpec
 * 
 * @namespace LynxCallConv
 * Provides tools and specifications for analyzing and applying LLVM calling conventions.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date:: November 4, 2025
*/


#ifndef LYNX_CALLING_CONV_INFERER_HPP
#define LYNX_CALLING_CONV_INFERER_HPP


#include <vector>
#include <memory>

#include "AndSpecification.hpp"
#include "HasAttributeSpec.hpp"
#include "IsVarArgSpec.hpp"
#include "CallingConventionSpecification.hpp"

#include "includes/AnyRegCallingConvSpec.hpp"
#include "includes/ColdCallingConvSpec.hpp"
#include "includes/CFGuardCheckSpec.hpp"
#include "includes/CXXFastTLSSpec.hpp"
#include "includes/FastCallingConvSpec.hpp"
#include "includes/FastCallSpec.hpp"
#include "includes/GHCCallingConvSpec.hpp"
#include "includes/HiPECallingConvSpec.hpp"
#include "includes/NumberedCallingConvSpec.hpp"
#include "includes/PreserveAllSpec.hpp"
#include "includes/PreserveMostSpec.hpp"
#include "includes/PreserveNoneSpec.hpp"
#include "includes/StdCallSpec.hpp"
#include "includes/SwiftSpec.hpp"
#include "includes/SwiftTailSpec.hpp"
#include "includes/ThisCallSpec.hpp"
#include "includes/TailSpec.hpp"


namespace LynxCallConv {

    class CallingConventionInferer {

        std::vector<std::unique_ptr<CallingConventionSpecification>> specs;

        public:
            CallingConventionInferer() {
                specs.push_back(std::make_unique<AndSpecification>(
                    std::make_unique<HasAttributeSpec>("cold", CallingConventionType::Cold),
                    std::make_unique<IsVarArgSpec>(CallingConventionType::Cold),
                    CallingConventionType::Cold
                ));
        
                specs.push_back(std::make_unique<AndSpecification>(
                    std::make_unique<HasAttributeSpec>("fast", CallingConventionType::Fast),
                    std::make_unique<IsVarArgSpec>(CallingConventionType::C), // Not fast if vararg
                    CallingConventionType::C
                ));

                specs.push_back(std::make_unique<AnyRegCallingConvSpec>());
                specs.push_back(std::make_unique<CFGuardCheckSpec>());
                specs.push_back(std::make_unique<ColdCallingConvSpec>());
                specs.push_back(std::make_unique<CXXFastTLSSpec>());
                specs.push_back(std::make_unique<FastCallingConvSpec>());
                specs.push_back(std::make_unique<GHCCallingConvSpec>());
                specs.push_back(std::make_unique<HiPECallingConvSpec>());
                specs.push_back(std::make_unique<PreserveMostSpec>());
                specs.push_back(std::make_unique<PreserveAllSpec>());
                specs.push_back(std::make_unique<PreserveNoneSpec>());
                specs.push_back(std::make_unique<TailSpec>());
                specs.push_back(std::make_unique<SwiftSpec>());
                specs.push_back(std::make_unique<SwiftTailSpec>());
                specs.push_back(std::make_unique<StdCallSpec>());
                specs.push_back(std::make_unique<FastCallSpec>());
                specs.push_back(std::make_unique<ThisCallSpec>());
                specs.push_back(std::make_unique<NumberedCallingConvSpec>());
            }
        
            CallingConventionType infer(llvm::Function* func) {
                for (const auto& spec : specs) {
                    if (spec->isSatisfiedBy(func)) {
                        return spec->getCallingConvention();
                    }
                }
                return CallingConventionType::C;
            }
    };
    
}

#endif
