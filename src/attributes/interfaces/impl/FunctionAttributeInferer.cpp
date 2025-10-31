#include "llvm/IR/Attributes.h"

#include "interfaces/FunctionAttributeInferer.hpp"
#include "handler/SafeDefaultsHandler.hpp"
#include "handler/AlwaysInlineHandler.hpp"
#include "handler/NoInlineHandler.hpp"
#include "handler/NoReturnHandler.hpp"
#include "handler/ReadOnlyHandler.hpp"
#include "handler/ReadNoneHandler.hpp"
#include "handler/NoCaptureHandler.hpp"
#include "handler/NoUnwindHandler.hpp"
#include "handler/SanitizeAddressHandler.hpp"
#include "handler/OptimizeForSizeHandler.hpp"
#include "handler/InlineHintHandler.hpp"
#include "handler/StackProtectorHandler.hpp"
#include "handler/NoInlineHintHandler.hpp"
#include "handler/NoDeadStripHandler.hpp"
#include "handler/IntrinsicHandler.hpp"
#include "handler/NoRecurseHandler.hpp"
#include "handler/SanitizeThreadHandler.hpp"
#include "handler/SanitizeMemoryHandler.hpp"
#include "handler/SanitizeHWAddressHandler.hpp"
#include "handler/NoInlineAsmHandler.hpp"
#include "handler/UWTableHandler.hpp"
#include "handler/ZeroExtendHandler.hpp"
#include "handler/SignExtendHandler.hpp"
#include "handler/ArgMemOnlyHandler.hpp"
#include "handler/ReturnsTwiceHandler.hpp"
#include "handler/StrictFPHandler.hpp"
#include "handler/SanitizeCoverageHandler.hpp"
#include "handler/AssumeSameAlignedHandler.hpp"
#include "handler/NoSanitizeMemoryHandler.hpp"
#include "handler/NoSanitizeThreadHandler.hpp"
#include "handler/NoSanitizeAddressHandler.hpp"
#include "handler/DereferenceableParamHandler.hpp"

namespace LynxFunctionAttr {

    FunctionAttributeInferer::FunctionAttributeInferer() {
        // Build the chain of responsibility
        auto headHandler = std::make_unique<SafeDefaultsHandler>();
        FunctionAttributeHandler* current = headHandler.get();

        auto append = [&](std::unique_ptr<FunctionAttributeHandler> next) {
            current->setNext(std::move(next));
            current = current->getNext(); // assumes getNext() returns raw ptr for chaining
        };

        append(std::make_unique<AlwaysInlineHandler>());
        append(std::make_unique<NoInlineHandler>());
        append(std::make_unique<NoReturnHandler>());
        append(std::make_unique<ReadOnlyHandler>());
        append(std::make_unique<ReadNoneHandler>());
        append(std::make_unique<NoCaptureHandler>());
        append(std::make_unique<NoUnwindHandler>());
        append(std::make_unique<SanitizeAddressHandler>());
        append(std::make_unique<OptimizeForSizeHandler>());
        append(std::make_unique<InlineHintHandler>());
        append(std::make_unique<StackProtectorHandler>());
        append(std::make_unique<NoInlineHintHandler>());
        append(std::make_unique<NoDeadStripHandler>());
        append(std::make_unique<IntrinsicHandler>());
        append(std::make_unique<NoRecurseHandler>());
        append(std::make_unique<SanitizeThreadHandler>());
        append(std::make_unique<SanitizeMemoryHandler>());
        append(std::make_unique<SanitizeHWAddressHandler>());
        append(std::make_unique<NoInlineAsmHandler>());
        append(std::make_unique<UWTableHandler>());
        append(std::make_unique<ZeroExtendHandler>());
        append(std::make_unique<SignExtendHandler>());
        append(std::make_unique<ArgMemOnlyHandler>());
        append(std::make_unique<ReturnsTwiceHandler>());
        append(std::make_unique<StrictFPHandler>());
        append(std::make_unique<SanitizeCoverageHandler>());
        append(std::make_unique<AssumeSameAlignedHandler>());
        append(std::make_unique<NoSanitizeMemoryHandler>());
        append(std::make_unique<NoSanitizeThreadHandler>());
        append(std::make_unique<NoSanitizeAddressHandler>());
        append(std::make_unique<DereferenceableParamHandler>());

        head = std::move(headHandler);
    }

    void FunctionAttributeInferer::inferAndApply(llvm::Function* func) {      
        FunctionAttributeBuilder builder(func);
        if (head) {
            head->handle(func, builder);
        }
        builder.apply();
    }

    FunctionAttributeInferer::~FunctionAttributeInferer() = default;
}
