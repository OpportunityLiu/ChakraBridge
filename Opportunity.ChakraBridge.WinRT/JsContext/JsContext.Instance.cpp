#include "pch.h"
#include "JsContext.Instance.h"

using namespace Opportunity::ChakraBridge::WinRT;

JsContext::JsContext(JsContextRef ref)
    :Reference(ref)
{
    CHAKRA_CALL(JsAddRef(ref, nullptr));
}

JsContext::~JsContext()
{
    if (this->Reference == JS_INVALID_REFERENCE)
        return;
    JsContextRef ct;
    CHAKRA_CALL(JsGetCurrentContext(&ct));
    if (this->Reference == ct)
        throw ref new Platform::FailureException("Can not dispose JsContexts in using.");
    this->Runtime->Contexts.erase(this->Reference);
    CHAKRA_CALL(JsRelease(this->Reference, nullptr));
    this->Reference = JS_INVALID_REFERENCE;
}

JsRuntime^ JsContext::Runtime::get()
{
    JsRuntimeHandle rth;
    CHAKRA_CALL(JsGetRuntime(this->Reference, &rth));
    return JsRuntime::RuntimeDictionary[rth];
}

JsContextScope^ JsContext::Use(bool disposeContext)
{
    return ref new JsContextScope(this, disposeContext);
}