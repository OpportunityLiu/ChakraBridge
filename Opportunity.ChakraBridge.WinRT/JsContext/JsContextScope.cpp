#include "pch.h"
#include "JsContext.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsContextScope::JsContextScope(JsContext^ jsContext, bool disposeContext)
    : current(jsContext), disposeContext(disposeContext)
{
    CHAKRA_CALL(JsGetCurrentContext(&this->previous));
    JsContext::Current = jsContext;
    if (this->previous != JS_INVALID_REFERENCE)
        CHAKRA_CALL(JsAddRef(this->previous, nullptr));
}

JsContextScope::~JsContextScope()
{
    auto c = this->current;
    this->current = nullptr;
    if (c == nullptr)
        return;
    CHAKRA_CALL(JsSetCurrentContext(this->previous));
    if (this->disposeContext)
        delete c;
    if (this->previous != JS_INVALID_REFERENCE)
        CHAKRA_CALL(JsRelease(this->previous, nullptr));
    this->previous = JS_INVALID_REFERENCE;
}
