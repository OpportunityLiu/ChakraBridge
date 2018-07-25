#include "pch.h"
#include "JsContext.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsContextScope::JsContextScope(JsContext^ jsContext, bool disposeContext)
    : previous(JsContext::Current), current(jsContext), disposeContext(disposeContext)
{
    _ASSERTE(jsContext != nullptr);
    JsContext::Current = jsContext;
}

JsContextScope::~JsContextScope()
{
    auto c = this->current;
    this->current = nullptr;
    if (c == nullptr)
        return;
    JsContext::Current = this->previous;
    if (this->disposeContext)
        delete c;
    this->previous = nullptr;
}
